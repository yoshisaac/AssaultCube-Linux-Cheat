#include "player_info.hpp"

#include <unistd.h>
#include <stdint.h>

#include "../memory.hpp"
#include "../math.hpp"

void player(pid_t game_pid) {
  for (unsigned int i = 0; i < 32; ++i) {
    uintptr_t player;
    Memory::read(game_pid, PlayerInfo::player_list + i*8, &player, sizeof(uintptr_t));
    if (!player) {
      PlayerInfo::l_players[i] = PlayerInfo::Player();
      continue;
    }
    
    int health;
    Memory::read(game_pid, player + 0x100, &health, sizeof(int));
      
    float view_angles[2];
    Memory::read(game_pid, player + 0x38, &view_angles, sizeof(float[2]));

    float location[3];
    Memory::read(game_pid, player + 0x8, &location, sizeof(float[3]));
    
    PlayerInfo::l_players[i] = PlayerInfo::Player(i, health, location, view_angles);

    uintptr_t local_player;
    Memory::read(game_pid, PlayerInfo::local_player, &local_player, sizeof(uintptr_t));

    int local_health;
    Memory::read(game_pid, local_player + 0x100, &local_health, sizeof(int));

    float local_view_angles[2];
    Memory::read(game_pid, local_player + 0x38, &local_view_angles, sizeof(float[2]));

    float local_location[3];
    Memory::read(game_pid, local_player + 0x8, &local_location, sizeof(float[3]));

    PlayerInfo::plocal = PlayerInfo::Player(-1, local_health, local_location, local_view_angles);
  }
}
