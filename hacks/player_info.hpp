#include <unistd.h>
#include <stdint.h>

void player(pid_t game_pid);

namespace PlayerInfo {
  inline uintptr_t player_list;
  inline uintptr_t local_player;

  class Player {
  public:
    //player structure
    unsigned int index;
    int health;
    float location[3];
    float view_angles[2];
    
    Player() {
      index = -1;
      health = -1;
    }

    Player(int index, int health, float location[3], float view_angles[2]) {
      this->index = index;
      this->health = health;
      
      this->location[0] = location[0];
      this->location[1] = location[1];
      this->location[2] = location[2];

      this->view_angles[0] = view_angles[0];
      this->view_angles[1] = view_angles[1];
    }
  };
  
  inline Player l_players[32];

  static Player get_player(unsigned int index) {
    if (index > 32) return Player();
    return l_players[index];
  }

  inline Player plocal;
  static Player get_local_player(void) {
    return plocal;
  }
} 
