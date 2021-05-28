#include "world.h"

bool out_of_bounds(int x, int y, int scr_w, int scr_h){
  if (x < 0 || x >= scr_w || y < 0 || y >= scr_h) return true;
  else return false;
}

bool is_solid(int ind){
  return (ind == SAND_ID) || (ind == WOOD_ID) || (ind == STONE_ID);
}

bool is_not_air(int ind){
  return (ind != 0);
}

int get_floor(int x, int y){
  for (size_t i = y; i > 0; i--)
  {
    if (is_solid(grid[x][i-1])) return i;
  }
  return 0;
}

int get_not_air_floor(int x, int y){
  for (size_t i = y; i > 0; i--)
  {
    if (is_not_air(grid[x][i-1])) return i;
  }
  return 0;
}

double GetCurrentTime()
{
	using Duration = std::chrono::duration<double>;
	return std::chrono::duration_cast<Duration>( 
		std::chrono::high_resolution_clock::now().time_since_epoch() 
	).count();
}

void try_spawn_falling(int x, int y){
  if (out_of_bounds(x, y, scr_w, scr_h)) return;
  int n = falling_pts.size();
  for (size_t i = 0; i < n; i++)
  {
    FallingPoint fp = falling_pts.front();
    falling_pts.pop();
    if (fp.x == x && fp.y == y) {
      falling_pts.push(fp);
      return;
    }
    falling_pts.push(fp);
  }
  falling_pts.push(FallingPoint(x, scr_h - y));
}

void try_spawn_water(int x, int y){
  if (out_of_bounds(x, y, scr_w, scr_h)) return;
  int n = water_pts.size();
  for (size_t i = 0; i < n; i++)
  {
    WaterPoint wp = water_pts.front();
    water_pts.pop();
    if (wp.x == x && wp.y == y) {
      water_pts.push(wp);
      return;
    }
    water_pts.push(wp);
  }
  water_pts.push(WaterPoint(x, scr_h - y));
}

void try_spawn_fire(int x, int y){
  if (out_of_bounds(x, y, scr_w, scr_h)) return;
  int n = fire_pts.size();
  for (size_t i = 0; i < n; i++)
  {
    FirePoint fp = fire_pts.front();
    fire_pts.pop();
    if (fp.x == x && fp.y == y){
      fire_pts.push(fp);
      return;
    }
    fire_pts.push(fp);
  }
  float x1 = (rand() % 20) - 10;
  float y1 = (rand() % 20) - 10;
  glm::vec2 sp = glm::normalize(glm::vec2(x1, y1)) * (float)0.8;
  fire_pts.push(FirePoint(x, scr_h - y, sp));
}

void cover_light(int x, int y){
  while (x > 0 && y > 0){
    x -= 1;
    y -= 1;
    if (light_grid[x][y] > 0){
      light_grid[x][y] = 0;
      // if (grid[x][y] == AIR_ID)
      //   light_grid[x][y] = 0;
      // else light_grid[x][y] = max(0.0f, light_grid[x+1][y+1] - 0.005f);
    }
    else return;
  }
}

void uncover_light(int x, int y){
  while (x > 0 && y > 0){
    if (light_grid[x][y] == 0.0f) return;
    x -= 1;
    y -= 1;
    if (grid[x][y] == AIR_ID)
      light_grid[x][y] = light_grid[x+1][y+1];
    else return;
  }
}

void paint_square(int x, int y, int rad, int mat){
  for (size_t i = x - rad + 1; i < x + rad; i++)
  {
    for (size_t j = y - rad + 1; j < y + rad; j++)
    {
      if (out_of_bounds(i, j, scr_w, scr_h)) continue;
      grid[i][j] = mat;
      if (is_not_air(mat))
        cover_light(i, j);
      else uncover_light(i, j);
    }    
  }
  if (mat == AIR_ID){
    for (size_t i = x - rad; i < x + rad + 1; i++)
    {
      if (grid[i][y + rad + 1] == SAND_ID){
        //grid[i][y + rad + 1] = AIR_ID;
        paint_square(i, y + rad + 1, 1, 0);
        falling_pts.push(FallingPoint(i, y + rad + 1));
      }
    }
    for (size_t i = y - rad + 2; i < y + rad; i++)
    {
      if (grid[x - rad][i] == SAND_ID){
        //grid[x - rad][i] = AIR_ID;
        paint_square(x - rad, i, 1, 0);
        falling_pts.push(FallingPoint(x - rad, i));
      }
      if (grid[x + rad + 1][i] == SAND_ID){
        //grid[x + rad + 1][i] = AIR_ID;
        paint_square(x + rad + 1, i, 1, 0);
        falling_pts.push(FallingPoint(x + rad + 1, i));
      }
    }   
  }  
}

void paint_falling_square(int x, int y, int rad){
  for (size_t i = x - rad + 1; i < x + rad; i++)
  {
    for (size_t j = y - rad + 1; j < y + rad; j++)
    {
      try_spawn_falling(i, j);
    }    
  } 
}

void paint_water_square(int x, int y, int rad){
  for (size_t i = x - rad + 1; i < x + rad; i++)
  {
    for (size_t j = y - rad + 1; j < y + rad; j++)
    {
      try_spawn_water(i, j);
    }    
  } 
}

void paint_fire_square(int x, int y, int rad){
  for (size_t i = x - rad + 1; i < x + rad; i++)
  {
    for (size_t j = y - rad + 1; j < y + rad; j++)
    {
      try_spawn_fire(i, j);
    }    
  } 
}

void bresenham(int mat, int x0, int y0, int x1, int y1, int width){
    float deltax = x1 - x0;
    float deltay = y1 - y0;
    float deltaerr = abs(deltay / deltax);
    float error = 0.0;
    if (deltaerr < 1.0){
      int y = y0;
      int offY = (y1 > y0) ? 1 : -1;
      if (x0 > x1){
        swap(x0, x1);
        offY *= -1;
      }
      for (size_t i = x0; i <= x1; i++){
        paint_square(i, y, width, mat);
        error += deltaerr;
        if (error >= 0.5){
          y += offY; // sign(deltaerr)
        error -= 1.0;
        }
      } 
    }
    else{
      int x = x0;
      int offX = (x1 > x0) ? 1 : -1;
      if (y0 > y1){
        swap(y0, y1);
        offX *= -1;
      }
      deltaerr = 1.0 / deltaerr;
      for (size_t i = y0; i <= y1; i++){
        paint_square(x, i, width, mat);
        error += deltaerr;
        if (error >= 0.5){
          x += offX; // sign(deltaerr)
        error -= 1.0;
        }
      } 
    }
}