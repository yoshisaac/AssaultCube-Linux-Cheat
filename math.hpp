#include <cmath>

#include "memory.hpp"

inline static float distance_3d(float location_one[3], float location_two[2]) {
  return sqrt(((location_one[1] - location_two[1])*(location_one[1] - location_two[1])) +
	     ((location_one[0] - location_two[0])*(location_one[0] - location_two[0])) +
	     ((location_one[2] - location_two[2])*(location_one[2] - location_two[2])));
}

inline uintptr_t vmatrix_address;
inline static float vmatrix[4][4];
inline static bool world_to_screen(pid_t gamePid, const float vIn[3], float vOut[2], bool ignore_oof=true) {
  Memory::read(gamePid, vmatrix_address, &vmatrix, sizeof(vmatrix));
  
  //multiply vector against matrix
  float screenX = (vmatrix[0][0] * vIn[0]) + (vmatrix[1][0] * vIn[1]) + (vmatrix[2][0] * vIn[2]) + vmatrix[3][0];
  float screenY = (vmatrix[0][1] * vIn[0]) + (vmatrix[1][1] * vIn[1]) + (vmatrix[2][1] * vIn[2]) + vmatrix[3][1];
  float screenW = (vmatrix[0][3] * vIn[0]) + (vmatrix[1][3] * vIn[1]) + (vmatrix[2][3] * vIn[2]) + vmatrix[3][3];

  //camera position (eye level/middle of screen)
  float camX = 1920 / 2.f;
  float camY = 1080 / 2.f;

  //convert to homogeneous position
  float x = camX + (camX * screenX / screenW);
  float y = camY - (camY * screenY / screenW);
  vOut[0] = x;
  vOut[1] = y;

  if (x > 1920 || x < 0 || y > 1080 || y < 0) return false;

  //check if object is behind camera / off screen (not visible)
  //w = z where z is relative to the camera 
  return (screenW > 0.001f);
}
