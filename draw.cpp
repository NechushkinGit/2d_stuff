#include "world.h"


void draw_background(){
  glBegin(GL_QUADS);
  glColor4f(0.2, 0.2, 0.2, 1); glVertex2f(-1, -1);
  glColor4f(0.2, 0.2, 0.2, 1); glVertex2f(1, -1);
  glColor4f(0.2, 0.2, 0.2, 1); glVertex2f(1, 1);
  glColor4f(0.2, 0.2, 0.2, 1); glVertex2f(-1, 1);
  glEnd();
}

void draw_cursor(int x, int y, int w, int h, int width){
  glLineWidth(3);
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  float x1 = -1 + 2.0 * x / ((float)w);
  float y1 = -1 + 2.0 * y / ((float)h);
  float wid = 2.0 * width / ((float)w);
  float hei = 2.0 * width / ((float)h);
  glBegin(GL_QUADS);
  glColor4f(1, 0, 0, 0.5);
  glVertex2f(x1 - wid, -1 * (y1 - hei));
  glVertex2f(x1 - wid, -1 * (y1 + hei));
  glVertex2f(x1 + wid, -1 * (y1 + hei));
  glVertex2f(x1 + wid, -1 * (y1 - hei));
  glEnd();
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  // for (size_t i = x - width; i < x + width + 1; i++)
  // {
  //   float x1 = -1 + 2.0 * i / ((float)w);
  // }  
}

void draw_grid(int w, int h){
  glBegin(GL_POINTS);
  for (size_t i = 0; i < w; i++)
  {
    for (size_t j = 0; j < h; j++)
    {
      float x = -1 + 2.0 * i / ((float)w);
      float y = -1 + 2.0 * j / ((float)h);
      if (grid[i][j] != AIR_ID){
        switch (grid[i][j])
        {        
        case SAND_ID:
          glColor4f(1, 0.8, 0.2, 1);
          glVertex2f(x, y);
          break;

        case STONE_ID:
          glColor4f(0.8, 0.8, 0.8, 1);
          glVertex2f(x, y);
          break;

        case WOOD_ID:
          glColor4f(0.4, 0.3, 0.1, 1);
          glVertex2f(x, y);
          break;

        case WATER_ID:
          glColor4f(0.2, 0.3, 1, 1);
          glVertex2f(x, y);
          break;
        
        default:
          break;
        }
      }
      glColor4f(1, 1, 0.7, 0.03 * light_grid[i][j]);
      glVertex2f(x, y);

    }    
  }
  glEnd();
}

void blur_falling(FallingPoint fp, int w, int h){
  int sp = (int)floor(fp.speed);
  for (int i = fp.y - sp + 1; i < fp.y + sp; i++)
  {
    if (out_of_bounds(fp.x, i, w, h)) continue;
    float alpha = 1.0 / (1 + abs(i - fp.y));
    glColor4f(1, 0.8, 0.2, pow(alpha, 2));
    glVertex2f(-1 + 2.0 * fp.x / ((float)w), -1 + 2.0 * i / ((float)h));
  }  
}

void blur_water(WaterPoint wp, int w, int h){
  int sp = (int)floor(wp.speed * 2);
  for (int i = wp.y - sp + 1; i < wp.y + sp; i++)
  {
    if (out_of_bounds(wp.x, i, w, h)) continue;
    float alpha = 1.0 / (1 + abs(i - wp.y));
    glColor4f(0.2, 0.3, 1, pow(alpha, 2));
    glVertex2f(-1 + 2.0 * wp.x / ((float)w), -1 + 2.0 * i / ((float)h));
  }  
}

void draw_falling(int w, int h)
{
  glBegin(GL_POINTS);
  int n = falling_pts.size();
  for (size_t i = 0; i < n; i++)
  {
    FallingPoint fp = falling_pts.front();
    falling_pts.pop();
    
    blur_falling(fp, w, h);
    //glColor4f(1, 0.8, 0.2, 1);
    //glVertex2f(-1 + 2.0 * fp.x / ((float)w), 1*(-1 + 2.0 * fp.y / ((float)h)));
    falling_pts.push(fp);
  }
  glEnd();
}

void draw_fire(int w, int h)
{
  glBegin(GL_POINTS);
  int n = fire_pts.size();
  for (size_t i = 0; i < n; i++)
  {
    FirePoint fp = fire_pts.front();
    fire_pts.pop();

    int f_life = 10;
    
    float red = max(0, min(f_life, f_life * 2 - fp.lifetime)) / (f_life * 1.0);//0 - 1, 60 - 1, 120 - 0, 180 - 0
    float green = max(0, min(f_life, f_life - fp.lifetime)) / (f_life * 1.0);//0 - 1, 60 - 0
    float blue = max(0, min(f_life, f_life / 2 - fp.lifetime)) / (f_life * 4.0);
    float alpha = max(0, min(f_life, f_life * 5 / 2 - fp.lifetime)) / (f_life * 1.0);
    
    glColor4f(red, green, blue, alpha);
    glVertex2f(-1 + 2.0 * fp.x / ((float)w), 1*(-1 + 2.0 * fp.y / ((float)h)));
    fire_pts.push(fp);
  }
  glEnd();
}

void draw_water(int w, int h)
{
  glBegin(GL_POINTS);
  int n = water_pts.size();
  for (size_t i = 0; i < n; i++)
  {
    WaterPoint wp = water_pts.front();
    water_pts.pop();
    
    blur_water(wp, w, h);
    water_pts.push(wp);
  }
  glEnd();
}