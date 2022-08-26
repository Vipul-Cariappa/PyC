import PyC


screenWidth = 800
screenHeight = 450
title = "raylib [core] example - keyboard input"
KEY_RIGHT = 262
KEY_LEFT = 263
KEY_UP = 265
KEY_DOWN = 264


def main(lib):
    WHITE = lib.Color() # (245, 245, 245, 255)
    WHITE.r = 245; WHITE.g = 245; WHITE.b = 245; WHITE.a = 255; 
    
    BLACK = lib.Color() # (0, 0, 0, 255)
    BLACK.r = 0; BLACK.g = 0; BLACK.b = 0; BLACK.a = 255; 
    
    RED = lib.Color() # (245, 0, 0, 255)
    RED.r = 245; RED.g = 0; RED.b = 0; RED.a = 255; 

    lib.SetConfigFlags(32)
    lib.InitWindow(screenWidth, screenHeight, title)
    lib.SetTargetFPS(60)

    ballPosition = lib.Vector2()
    ballPosition.x = screenWidth/2
    ballPosition.y = screenHeight/2
    
    # while True:
    while not lib.WindowShouldClose():
        if lib.IsKeyDown(KEY_RIGHT):
            ballPosition.x += 2
        if lib.IsKeyDown(KEY_LEFT):
            ballPosition.x -= 2
        if lib.IsKeyDown(KEY_UP):
            ballPosition.y -= 2
        if lib.IsKeyDown(KEY_DOWN):
            ballPosition.y += 2

        # Draw
        lib.BeginDrawing()
        lib.ClearBackground(WHITE)
        lib.DrawText(f"FPS: {lib.GetFPS()}\nmove the ball with arrow keys.", 10, 10, 20, BLACK)
        lib.DrawCircleV(ballPosition, 50, RED)
        lib.EndDrawing()
        # endDraw


if __name__ == "__main__":
    # lib = PyC.LoadCpp("/home/vipul/bin/raylib/libraylib.so", "examples/c_raylib.h")
    lib = PyC.LoadCpp("/home/vipul/bin/raylib/libraylib.so", "/home/vipul/bin/raylib/src/raylib.h")
    
    main(lib)
