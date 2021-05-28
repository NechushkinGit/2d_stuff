#ifndef HEADERFILE_H
#define HEADERFILE_H 1

#include <vector>
#include <GL/glew.h> 
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <chrono>
#include <cmath>
#include <queue>
#include <ctime>

using namespace std;

/*
0 - air
1 - sand
2 - stone
3 - wood
4 - fire
5 - water
*/
const int AIR_ID = 0;
const int SAND_ID = 1;
const int STONE_ID = 2;
const int WOOD_ID = 3;
const int FIRE_ID = 4;
const int WATER_ID = 5;

struct FallingPoint{
    int x;
    int y;
    float speed;
    FallingPoint(int _x, int _y, float _s){
        x = _x;
        y = _y;
        speed = _s;
    }
    FallingPoint(int _x, int _y){
        x = _x;
        y = _y;
        speed = 0;
    }
    FallingPoint(){
        x = 0;
        y = 0;
        speed = 0;
    }
};

struct FirePoint{
    float x;
    float y;
    glm::vec2 speed;
    int lifetime;
    FirePoint(float _x, float _y, glm::vec2 _s, int lt){
        x = _x;
        y = _y;
        speed = _s;
        lifetime = lt;
    }
    FirePoint(float _x, float _y, glm::vec2 _s){
        x = _x;
        y = _y;
        speed = _s;
        lifetime = 0;
    }
    FirePoint(float _x, float _y){
        x = _x;
        y = _y;
        speed = glm::vec2(0, 0);
        lifetime = 0;
    }
    FirePoint(){
        x = 0;
        y = 0;
        speed = glm::vec2(0, 0);
        lifetime = 0;
    }
};

struct WaterPoint{
    int x;
    int y;
    float speed;
    int dir;
    WaterPoint(int _x, int _y, float _s, int d){
        x = _x;
        y = _y;
        speed = _s;
        dir = d;
    }
    WaterPoint(int _x, int _y, float _s){
        x = _x;
        y = _y;
        speed = _s;
        dir = 0;
    }
    WaterPoint(int _x, int _y){
        x = _x;
        y = _y;
        speed = 0;
        dir = 0;
    }
    WaterPoint(){
        x = 0;
        y = 0;
        speed = 0;
        dir = 0;
    }
};

extern vector<vector<int>> grid;
extern vector<vector<float>> light_grid;
extern queue<FallingPoint> falling_pts;
extern queue<FirePoint> fire_pts;
extern queue<WaterPoint> water_pts;

extern int scr_w;
extern int scr_h;
extern int cursor_width;

bool out_of_bounds(int x, int y, int scr_w, int scr_h);
bool is_solid(int ind);
bool is_not_air(int ind);
int get_floor(int x, int y);
int get_not_air_floor(int x, int y);
double GetCurrentTime();

void draw_background();
void draw_cursor(int x, int y, int w, int h, int width);
void draw_grid(int w, int h);
void blur_falling(FallingPoint fp, int w, int h);
void blur_water(WaterPoint wp, int w, int h);
void draw_falling(int w, int h);
void draw_fire(int w, int h);
void draw_water(int w, int h);

void try_spawn_falling(int x, int y);
void try_spawn_fire(int x, int y);
void try_spawn_water(int x, int y);
void paint_square(int x, int y, int rad, int mat);
void paint_falling_square(int x, int y, int rad);
void paint_fire_square(int x, int y, int rad);
void paint_water_square(int x, int y, int rad);
void bresenham(int mat, int x0, int y0, int x1, int y1, int width);

void uncover_light(int x, int y);

void update_falling_points();
void update_fire_points();
void update_water_points();
#endif