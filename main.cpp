#include "world.h"
 
vector<vector<int>> grid = vector<vector<int>>();
vector<vector<float>> light_grid = vector<vector<float>>();
queue<FallingPoint> falling_pts = queue<FallingPoint>();
queue<FirePoint> fire_pts = queue<FirePoint>();
queue<WaterPoint> water_pts = queue<WaterPoint>();

int scr_w = 0;
int scr_h = 0;

const double frame_delay = 1.0 / 60.0; // 60 FPS
double last_render = 0;

int cur_mat = 1;

bool mDown = false;
int mouseX = 0;
int mouseY = 0;
int cursor_width = 3;


void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  draw_background();

  //glPointSize(10);
  draw_falling(scr_w, scr_h);
  draw_fire(scr_w, scr_h);
  draw_water(scr_w, scr_h);
  draw_grid(scr_w, scr_h);

  draw_cursor(mouseX, mouseY, scr_w, scr_h, cursor_width);

  //glFlush();
  glutSwapBuffers();
}

void Idle() {
  const double current_time = GetCurrentTime();
  if( ( current_time - last_render ) > frame_delay ){
    last_render = current_time;

    update_water_points();
    update_falling_points();
    update_fire_points();

    if (mDown){
      switch (cur_mat) {
        case SAND_ID:
          paint_falling_square(mouseX, mouseY, cursor_width);
          break;
        case FIRE_ID:
          paint_fire_square(mouseX, mouseY, cursor_width);
          break;
        case WATER_ID:
          paint_water_square(mouseX, mouseY, cursor_width);
          break;
        default:
          break;
      }
    }

    glutPostRedisplay();
  }
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP: cursor_width += 1; break;
	case GLUT_KEY_DOWN: if (cursor_width > 1) cursor_width -= 1; break;	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
  case '0':
    cur_mat = 0;
		break;
	case '1':
    cur_mat = 1;
		break;
  case '2':
    cur_mat = 2;
		break;
  case '3':
    cur_mat = 3;
		break;
  case '4':
    cur_mat = 4;
		break;
  case '5':
    cur_mat = 5;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void mouseEvents(int button, int state, int x, int y)
{
	switch (button)
	{
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_DOWN)
      {
        mouseX = x;
        mouseY = y;
        mDown = true;
      }
      else
      {
        mDown = false;
      }
    }
	}
}

/*
double offX = (mouseX / (glutGet(GLUT_WINDOW_WIDTH) * 1.0)) * 2 - 1;
double offY = -((mouseY / (glutGet(GLUT_WINDOW_HEIGHT) * 1.0)) * 2 - 1);
*/

void mouse_passive_move(int x, int y){
  mouseX = x;
  mouseY = y;
}

void mouseMove(int x, int y)
{
	if (mDown)
	{
    if (cur_mat == STONE_ID || cur_mat == WOOD_ID || cur_mat == AIR_ID){
      if (out_of_bounds(x, y, scr_w, scr_h)) {
        mouseX = x;
        mouseY = y;
        return;
      }
      if (cur_mat == 0){
        bresenham(0, mouseX, scr_h - mouseY, x, scr_h - y, cursor_width);
      }
      else
        bresenham(cur_mat, mouseX, scr_h - mouseY, x, scr_h - y, cursor_width);
    }
    mouseX = x;
    mouseY = y;
	}
}

void init() {
  glClearColor(0.1, 0.39, 0.88, 1.0);
  //glColor3f(1.0, 1.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //glEnable(GL_POINT_SMOOTH);

  grid = vector<vector<int>>(scr_w);
  light_grid = vector<vector<float>>(scr_w);
  for (size_t i = 0; i < scr_w; i++)
  {
    grid[i] = vector<int>(scr_h);
    light_grid[i] = vector<float>(scr_h);
    for (size_t j = 0; j < scr_h; j++)
    {
      grid[i][j] = 0;
      light_grid[i][j] = 1;
    }    
  } 
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 800);
  glutCreateWindow("World");
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
  srand(time(nullptr));

  scr_w = glutGet(GLUT_WINDOW_WIDTH);
  scr_h = glutGet(GLUT_WINDOW_HEIGHT);

  glutDisplayFunc(display);
  glutIdleFunc(Idle);
  glutMouseFunc(mouseEvents);
  glutMotionFunc(mouseMove);
  glutPassiveMotionFunc(mouse_passive_move);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeys);
  init();
  glutMainLoop();
}
