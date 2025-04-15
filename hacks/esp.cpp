#include "esp.hpp"
#include "draw.hpp"

#include <string>

#include "player_info.hpp"

#include "../math.hpp"

void esp(pid_t game_pid, XdbeBackBuffer back_buffer, Display* draw_display, Window window, GC gc) {
  if (Xutil::window_to_pid(draw_display, Xutil::focused_window(draw_display)) != game_pid) return;
  
  for (unsigned int i = 0; i < 32; ++i) {
    PlayerInfo::Player player = PlayerInfo::get_player(i);
    PlayerInfo::Player plocal = PlayerInfo::get_local_player();

    if (player.health <= 0 || player.health >= 999) continue;
    
    float screen[2];
    if (!world_to_screen(game_pid, player.location, screen)) continue;    
    
    float distance = distance_3d(plocal.location, player.location);

    //snap lines
    XSetForeground(draw_display, gc, Draw::green);
    XSetLineAttributes(draw_display, gc, 0, LineSolid, CapButt, JoinMiter);
      
    XDrawLine(draw_display, back_buffer, gc, 1920/2, 1080, screen[0], screen[1]);

    //index
    XSetFont(draw_display, gc, Draw::shadowfont->fid);
    XSetForeground(draw_display, gc, Draw::black);

      
    XDrawString(draw_display, back_buffer, gc, screen[0] + 1, screen[1] + 1, std::to_string(player.index).c_str(), strlen(std::to_string(player.index).c_str()));

    XSetFont(draw_display, gc, Draw::font->fid);
    XSetForeground(draw_display, gc, Draw::white);

      
    XDrawString(draw_display, back_buffer, gc, screen[0], screen[1], std::to_string(player.index).c_str(), strlen(std::to_string(player.index).c_str()));
 
  }
}
