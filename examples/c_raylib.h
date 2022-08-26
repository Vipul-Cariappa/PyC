#include <stdbool.h>

struct Vector2 {
  float x; // Vector x component
  float y; // Vector y component
};

struct Color {
  unsigned char r; // Color red value
  unsigned char g; // Color green value
  unsigned char b; // Color blue value
  unsigned char a; // Color alpha value
};

void InitWindow(int width, int height, const char *title);
void SetTargetFPS(int fps);
int GetFPS(void);
bool WindowShouldClose(void);
bool IsKeyDown(int key);
void BeginDrawing(void);
void ClearBackground(struct Color color);
void DrawText(const char *text, int posX, int posY, int fontSize,
              struct Color color);
void DrawCircleV(struct Vector2 center, float radius, struct Color color);
void EndDrawing(void);
void CloseWindow(void);
void SetConfigFlags(unsigned int flags);
