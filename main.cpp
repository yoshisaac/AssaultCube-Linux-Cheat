#include <stdio.h>
#include <stdint.h>
#include <thread>

#include "hacks/player_info.hpp"
#include "hacks/draw.hpp"

#include "process.hpp"
#include "memory.hpp"
#include "math.hpp"
#include "xutil.hpp"

int main() {
  pid_t game_pid = Process::name_to_pid("native_client");
  if (!game_pid) {
    printf("Please open the game\n");
    return 1;
  }
  printf("%d\n", game_pid);

  uintptr_t base_address = Memory::module_base_address(game_pid,  "native_client");
  printf("%p\n", base_address);

  PlayerInfo::local_player = base_address + 0x1E2600;

  Memory::read(game_pid, base_address + 0x1C7138, &PlayerInfo::player_list, sizeof(uintptr_t));

  vmatrix_address = base_address + 0x1C6BF8;  

  /* X11 initiation */
  Display* display = XOpenDisplay(NULL);
  if (!display) return 1;

  Display* draw_display = XOpenDisplay(NULL);
  if (!draw_display) return 1;

  Display* bhop_display = XOpenDisplay(NULL);
  if (!bhop_display) return 1;

  Display* aim_display = XOpenDisplay(NULL);
  if (!aim_display) return 1;  
  
  int screen = DefaultScreen(display);

  int shape_event_base;
  int shape_error_base;

  if (!XShapeQueryExtension(display, &shape_event_base, &shape_error_base)) {
    printf("cs-source-hack: NO shape extension in your system !\n");
    return 1;
  }

  Window root = DefaultRootWindow(display);

  Visual* visual = DefaultVisual(display, screen);

  XVisualInfo vinfo;
  XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);
  Colormap colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);

  Xutil::Color bgcolor = Xutil::xcolor_from_rgba(0, 0, 0, 0, display);
  
#define BASIC_EVENT_MASK (StructureNotifyMask|ExposureMask|PropertyChangeMask|EnterWindowMask|LeaveWindowMask|KeyPressMask|KeyReleaseMask|KeymapStateMask)
#define NOT_PROPAGATE_MASK (KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask)

  XSetWindowAttributes attr;
  attr.background_pixmap = None;
  attr.background_pixel = bgcolor;
  attr.border_pixel=0;
  attr.win_gravity=NorthWestGravity;
  attr.bit_gravity=ForgetGravity;
  attr.save_under=1;
  attr.event_mask=BASIC_EVENT_MASK;
  attr.do_not_propagate_mask=NOT_PROPAGATE_MASK;
  attr.override_redirect=1; // OpenGL > 0
  attr.colormap = colormap;

  unsigned long mask = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask | CWWinGravity|CWBitGravity | CWSaveUnder | CWDontPropagate | CWOverrideRedirect;

  Window window = XCreateWindow(display, root, 0, 0, 1920, 1080, 0, vinfo.depth, InputOutput, vinfo.visual, mask, &attr);
  
  XShapeCombineMask(display, window, ShapeInput, 0, 0, None, ShapeSet);
    
  XShapeSelectInput(display, window, ShapeNotifyMask);
  
  XserverRegion region = XFixesCreateRegion(display, NULL, 0);
  XFixesSetWindowShapeRegion(display, window, ShapeInput, 0, 0, region);
  XFixesDestroyRegion(display, region);

  XdbeBackBuffer back_buffer = XdbeAllocateBackBufferName(display, window, 0);
  
  XMapWindow(display, window);

  Draw::black = Xutil::xcolor_from_rgb(0, 0, 0, display);
  Draw::gray = Xutil::xcolor_from_rgb(210, 210, 210, display);
  Draw::green = Xutil::xcolor_from_rgb(0, 255, 50, draw_display);
  Draw::red = Xutil::xcolor_from_rgb(255, 0, 50, draw_display);
  Draw::white = Xutil::xcolor_from_rgb(255, 255, 255, draw_display);
  Draw::cyan = Xutil::xcolor_from_rgb(0, 255, 240, draw_display);
  Draw::yellow = Xutil::xcolor_from_rgb(255, 255, 0, draw_display);
  Draw::orange = Xutil::xcolor_from_rgb(255, 170, 0, draw_display); 
  
  Draw::shadowfont = XLoadQueryFont(display, "6x13bold");
  Draw::font = XLoadQueryFont(display, "6x13");
  /* X11 initiation end */

  std::thread draw_thread(draw, game_pid, back_buffer, draw_display, window);
  pthread_setname_np(draw_thread.native_handle(), "draw_thread");
  
  for (;;) {
    player(game_pid);
    usleep(1000*1000/250);
  }
}
