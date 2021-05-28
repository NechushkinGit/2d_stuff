#include "world.h"


void update_falling_points(){
  int n = falling_pts.size();
  for (size_t i = 0; i < n; i++)
  {
    FallingPoint fp = falling_pts.front();
    falling_pts.pop();
    if (out_of_bounds(fp.x, fp.y, scr_w, scr_h)) continue;
    if (is_solid(grid[fp.x][fp.y])){

    }
    int t = fp.y - (int)floor(fp.speed);
    int fl = get_floor(fp.x, fp.y);
    if (fp.speed >= 1.0){
      fp.y = t;
      if (t <= fl){
        fp.speed = 1.1;
        if (fl == 0){
          grid[fp.x][0] = 1;
          continue;
        }
        if (fp.x == 0){
          if (is_solid(grid[fp.x + 1][fl - 1])){
            grid[fp.x][fl] = 1;
            continue;
          }
          else{
            fp.x += 1;
            fp.y = fl;
          }
        }
        else if (fp.x == (scr_w-1)){
          if (is_solid(grid[fp.x - 1][fl - 1])){
            grid[fp.x][fl] = 1;
            continue;
          }
          else{
            fp.x -= 1;
            fp.y = fl;
          }
        }
        else{
          if (is_solid(grid[fp.x + 1][fl - 1]) && is_solid(grid[fp.x - 1][fl - 1])){
            grid[fp.x][fl] = 1;
            continue;
          }
          else if (is_solid(grid[fp.x + 1][fl - 1])){
            fp.x -= 1;
            fp.y = fl;
          }
          else if (is_solid(grid[fp.x - 1][fl - 1])){
            fp.x += 1;
            fp.y = fl;
          }
          else{
            //int r = rand() % 2;
            int r = std::rand()/((RAND_MAX + 1u)/2);
            if (r) fp.x += 1;
            else fp.x -= 1;
            fp.y = fl;
          }
        }
      }
    }
    else{
      fp.y = t;
    }
    fp.speed += 0.2;
    
    
    falling_pts.push(fp);
  }  
}

void update_fire_points(){
  int n = fire_pts.size();
  for (size_t i = 0; i < n; i++)
  {
    FirePoint fp = fire_pts.front();
    fire_pts.pop();
    fp.lifetime += 1;
    
    if (fp.lifetime > 25) continue;
    if (out_of_bounds(fp.x, fp.y, scr_w, scr_h)) continue;
    if (is_solid(grid[fp.x][fp.y])) continue;

    vector<int> offvx {1, 0, -1, 0};
    vector<int> offvy {0, -1, 0, 1};

    for (size_t i = 0; i < 4; i++)
    {
      int fx = (int)fp.x + offvx[i];
      int fy = (int)fp.y + offvy[i];
      if (grid[fx][fy] == WOOD_ID){
        int r = rand() % 100;
        if (r < 2){
          grid[fx][fy] = 0;
          uncover_light(fx, fy);
          FirePoint fp1 = FirePoint(fx, fy, glm::vec2(0, 0), -120);
          fire_pts.push(fp1);
        }
      } 
    }

    if (fp.lifetime > 0){
      fp.x += fp.speed.x;
      fp.y += fp.speed.y;
      fp.speed += glm::vec2(-fp.speed.x * 0.2, 0.2 - fp.speed.y * 0.02);
    }
    else{
      paint_fire_square(fp.x, scr_h - fp.y, 1);
    }
    fire_pts.push(fp);
  }
}

void update_water_points(){
  int n = water_pts.size();
  for (size_t i = 0; i < n; i++)
  {
    WaterPoint p = water_pts.front();
    water_pts.pop();
    if (out_of_bounds(p.x, p.y, scr_w, scr_h)) continue;
    if (is_not_air(grid[p.x][p.y])){

    }
    int t = p.y - (int)floor(p.speed);
    int fl = get_not_air_floor(p.x, p.y);
    if (p.speed >= 1.0){
      p.y = t;
      if (t <= fl){
        p.speed = 1.1;
        if (fl == 0){
          grid[p.x][0] = WATER_ID;
          continue;
        }
        if (p.x == 0){
          if (is_not_air(grid[p.x + 1][fl - 1])){
            if (p.dir == 0){
              p.dir = 1;
              p.x += 1;
              p.y = fl;
            }
            else{
              grid[p.x][fl] = WATER_ID;
              continue;
            }
          }
          else{
            p.x += 1;
            p.y = fl;
          }
        }
        else if (p.x == (scr_w-1)){
          if (is_not_air(grid[p.x - 1][fl - 1])){
            if (p.dir == 0){
              p.dir = -1;
              p.x -= 1;
              p.y = fl;
            }
            else{
              grid[p.x][fl] = WATER_ID;
              continue;
            }
          }
          else{
            p.x -= 1;
            p.y = fl;
          }
        }
        else{
          if (is_not_air(grid[p.x + 1][fl - 1]) && is_not_air(grid[p.x - 1][fl - 1])){
            if (p.dir == 0) p.dir = (rand()%2)*2-1;
            if (is_not_air(grid[p.x + p.dir][fl])){
              grid[p.x][fl] = WATER_ID;
              //paint_fire_square(400, 400, 3);
              continue;              
            }
            else{
              p.x += p.dir;
              p.y = fl;
            }
          }
          else if (is_not_air(grid[p.x + 1][fl - 1])){
            if (p.dir == 0) p.dir = -1;
            p.x -= 1;
            p.y = fl;
          }
          else if (is_not_air(grid[p.x - 1][fl - 1])){
            if (p.dir == 0) p.dir = 1;
            p.x += 1;
            p.y = fl;
          }
          else{
            //int r = rand() % 2;
            int r = std::rand()/((RAND_MAX + 1u)/2);
            if (p.dir == 0) p.dir = r * 2 - 1;
            p.x += p.dir;
            p.y = fl;
          }
        }
      }
    }
    else{
      p.y = t;
    }
    p.speed += 0.2;
    
    water_pts.push(p);
  }  
}