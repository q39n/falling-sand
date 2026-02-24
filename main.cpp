#include <cstdlib>
#include <raylib.h>
#include <string>
#include <vector>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

using namespace std;


#define SW 1000
#define SH 1000

#define PS 2


void draw_grid(int *grid) {
  for (int y=0; y<SH/PS; y++) {
    for (int x=0; x<SW/PS; x++) {
      Color c;
      int pixel = grid[y*(SW/PS) + x];

      switch (pixel) {
        case 0:
          c = {0,0,0,255};
          break;  
        case 1:
          c = {246,215,176, 255};
          break;
        case 2:
          c = {236,204,162, 255};
          break;
        case 3:
          c = {242,210,169, 255};
          break;
        case 4:
          c = {97, 214, 250, 255};
          break;
        case 5:
          c = {121, 220, 251, 255};
          break;
        case 6:
          c = {146, 226, 252, 255};
          break;
        case 7:
          c = {73,60,60, 255};
          break;
        case 8:
          c = {58,50,50, 255};
          break;
        case 9:
          c = {45,44,44, 255};
          break;
        default:
          break;
      }
      DrawRectangle(x * PS, y * PS, PS, PS, c);
    }
  }
}

void apply_physics(int *grid) {
  int gw = SH/PS;
  int gh = SH/PS;
  int dir = -1;
  for (int y=gh-1; y>=0; y--) {
    for (int x=0; x<gw; x++) {
      int pos = y*gw + x;
      dir = (rand() % 2) * 2 - 1;
      bool BORDER = x > 0 && x < gw-1;
      // SAND
      if (grid[pos] >= 1 && grid[pos] <= 3) {
        if (grid[pos+gw] == 0) {
          grid[pos+gw] = grid[pos];
          grid[pos] = 0;
        } else if (BORDER && grid[pos + gw + dir] == 0) {
          grid[pos+gw +dir] = grid[pos];
          grid[pos] = 0;
        }
      }
      // WATER 
      if (grid[pos] >= 4 && grid[pos] <= 6) {
        if (grid[pos+gw] == 0) {
          grid[pos+gw] = grid[pos];
          grid[pos] = 0;
        } else if (BORDER && grid[pos+dir] == 0) {
          grid[pos+dir] = grid[pos];
          grid[pos] = 0;
        } else if (BORDER && grid[pos + gw + dir] == 0) {
          grid[pos+gw +dir] = grid[pos];
          grid[pos] = 0;
        }
      }
    }
  }
}

int main() {
  InitWindow(SW, SH, "Falling Sand");
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20); 
  SetTargetFPS(60);

  static int grid[SW*SH];

  vector<string> types = {"Erase", "Sand", "Water", "Rock"};

  int item = 1;
  float radius = 15;

  bool menu_open = false;
  float menu_w = 600;
  float menu_h = 200;
  float menu_x = (float)SW/2-menu_w/2;
  float menu_y = (float)SH/2-menu_h/2;
  
  float btn_w = 100;
  float btn_h = 50;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_grid(grid);
    DrawFPS(4, 4);

    Vector2 mousePos = GetMousePosition();
    int mx = int(mousePos.x) / PS;
    int my = int(mousePos.y) / PS;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !menu_open) {
      for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
          if (dx*dx + dy*dy <= radius*radius) {
            int nx = mx + dx;
            int ny = my + dy;
            if (nx >= 0 && nx < SW/PS && ny >= 0 && ny < SH/PS) {
              grid[ny*(SW/PS) + nx] = item!=0 ? (rand()%3+1)+((item-1)*3) : item;
            }
          }
        }
      }
    }
  
    DrawRing(mousePos, radius*PS, radius*PS+1, 0, 360, 0, GRAY);

    if (IsKeyPressed(KEY_SPACE)) menu_open = !menu_open;

    if (menu_open) {
      GuiPanel((Rectangle) {menu_x, menu_y, menu_w, menu_h}, "Selection");
      for (int i=0; i<types.size(); i++) {
        float x = btn_w/4 + menu_x + i * menu_w / types.size();
        float y = menu_y + 30;
        if (GuiButton({x, y, btn_w, btn_h}, TextFormat("%s", types[i].c_str()))) item = i;
      }
      DrawText(TextFormat("Drawing size: %.0f", radius), menu_x+(menu_w/2)-100, menu_y+btn_h+50, 20, GRAY);
      GuiSlider(Rectangle{menu_x+30, menu_y + btn_h + 80, menu_w - 60, 40}, "1", "15", &radius, 1, 15);
    }
    apply_physics(grid);

    EndDrawing();
  }
  CloseWindow();
  return 0;

}

