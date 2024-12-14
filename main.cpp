#include <raylib.h>
#include <iostream>
#include <button.hpp>
using namespace std;

Color Green = Color{130, 185, 154, 255};
Color Dark_BLue = Color{0, 255, 255, 255};
Color Light_Blue = Color{0, 191, 255, 255};
Color Yellow = Color{243, 213, 91, 255};

int player1_score = 0;
int cpu_score = 0;

class Ball {
 public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, ORANGE);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }
        // Cpu wins
        if (x + radius >= GetScreenWidth()) {
            cpu_score++;
            ResetBall();
        }

        if (x - radius <= 0) {
            player1_score++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
 protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

 public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, RED);
    }
    void Draw1() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, BLACK);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle {
 public:
    void Update(int ball_y){
        if (y + height / 2 > ball_y) {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y) {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player1, player2;
CpuPaddle cpu;
Texture2D background = LoadTexture("Graphics/background.png");
Button startButton{"Graphics/start_button.png", {540, 150}, 0.65};
Button exitButton{"Graphics/exit_button.png", {540, 300}, 0.65};
bool exit = false;

void 1vs1(){
    while (1){

            BeginDrawing();
            // Updating

            ball.Update();
            player1.Update();
            cpu.Update(ball.y);

            // Checking for collisions
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player1.x, player1.y, player1.width, player1.height})) {
                ball.speed_x *= -1;
            }

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpu.x, cpu.y, cpu.width, cpu.height})) {
                ball.speed_x *= -1;
            }

            // Drawing
            ClearBackground(Dark_BLue);
            DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Light_Blue);
            DrawCircle(screen_width / 2, screen_height / 2, 150, BLUE);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            ball.Draw();
            cpu.Draw1();
            player1.Draw();
            DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player1_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
            cout << "1" << endl;
            EndDrawing(); 

            if(WindowShouldClose() == true){
                    CloseWindow();
            }

            }


}
int main() {

    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player1.width = 25;
    player1.height = 120;
    player1.x = screen_width - player1.width - 10;
    player1.y = screen_height / 2 - player1.height / 2;
    player1.speed = 6;

    player2.height = 120;
    player2.width = 25;
    player2.x = 10;
    player2.y = screen_height / 2 - player2.height / 2;
    player2.speed = 6;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;


    while (WindowShouldClose() == false && exit == false) {

        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        BeginDrawing();

        if(startButton.isPressed(mousePosition, mousePressed)){
            
            1vs1();
        }

        if(exitButton.isPressed(mousePosition, mousePressed))
        {
            exit = true;
        }
        ClearBackground(BLACK);
        DrawTexture(background, 0, 0, WHITE);
        startButton.Draw();
        exitButton.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

