#include "raylib.h"
int main()
{
    // Window dimensions
    int width = 800;
    int height = 450;

    InitWindow(width, height, "Fish Feeding Frenzy");

    // Load sprite image
    Texture2D fishSprite = LoadTexture("Characters/Fish Sprite Sheet4.png");

    // Player 1 coordinates and size
    float player1_x = 100;
    float player1_y = height / 2;
    int player1_size = 25;
    int player1_powerUps = 0;
    Vector2 player1_direction = {1, 0}; // Initial direction

    // Player 2 coordinates and size
    float player2_x = width - 100;
    float player2_y = height / 2;
    int player2_size = 25;
    int player2_powerUps = 0;
    Vector2 player2_direction = {1, 0}; // Initial direction

    // Food coordinates and size
    int food_x = GetRandomValue(50, width - 50);
    int food_y = GetRandomValue(50, height - 50);
    int food_size = 15;

    // Hook coordinates, size, and speed
    float hook_x;
    float hook_y;
    float hook_width = 10.0f; // Changed to float
    float hook_height = 50.0f; // Changed to float
    int hook_speed = 5;

    // Initialize hook
    hook_x = GetRandomValue(50, width - 50);
    hook_y = 0;

    // Scores
    bool player1Wins = false;
    bool player2Wins = false;
    bool gameOver = false;

    // Animation variables for both players
    float runningTime{};
    int frame{};
    const int maxFrames{4};
    const float updateTime{1.f/12.f};

    SetTargetFPS(60);

    while (!WindowShouldClose() && !gameOver)
    {
        // Player 1 movement controls
        if (IsKeyDown(KEY_W) && player1_y > player1_size) {
            player1_y -= 5;
            player1_direction = {0, -1}; // Up
        }
        else if (IsKeyDown(KEY_S) && player1_y < height - player1_size) {
            player1_y += 5;
            player1_direction = {0, 1}; // Down
        }
        else if (IsKeyDown(KEY_A) && player1_x > player1_size) {
            player1_x -= 5;
            player1_direction = {-1, 0}; // Left
        }
        else if (IsKeyDown(KEY_D) && player1_x < width - player1_size) {
            player1_x += 5;
            player1_direction = {1, 0}; // Right
        }

        // Player 2 movement controls
        if (IsKeyDown(KEY_UP) && player2_y > player2_size) {
            player2_y -= 5;
            player2_direction = {0, -1}; // Up
        }
        else if (IsKeyDown(KEY_DOWN) && player2_y < height - player2_size) {
            player2_y += 5;
            player2_direction = {0, 1}; // Down
        }
        else if (IsKeyDown(KEY_LEFT) && player2_x > player2_size) {
            player2_x -= 5;
            player2_direction = {-1, 0}; // Left
        }
        else if (IsKeyDown(KEY_RIGHT) && player2_x < width - player2_size) {
            player2_x += 5;
            player2_direction = {1, 0}; // Right
        }

        // Hook movement
        hook_y += hook_speed;
        if (hook_y > height)
        {
            hook_x = GetRandomValue(50, width - 50);
            hook_y = 0;
        }

        // Check collision between players and hooks
        if (CheckCollisionCircleRec((Vector2){player1_x, player1_y}, player1_size, (Rectangle){hook_x, hook_y, hook_width, hook_height}) ||
            CheckCollisionCircleRec((Vector2){player2_x, player2_y}, player2_size, (Rectangle){hook_x, hook_y, hook_width, hook_height}))
        {
            gameOver = true;
        }

        // Check collision between players and food
        if (CheckCollisionCircleRec((Vector2){player1_x, player1_y}, player1_size, (Rectangle){(float)(food_x - (food_size / 2)), (float)(food_y - (food_size / 2)), (float)food_size, (float)food_size}))
        {
            player1_powerUps++;
            food_x = GetRandomValue(50, width - 50);
            food_y = GetRandomValue(50, height - 50);
            if (player1_powerUps >= 10) {
                player1Wins = true;
                gameOver = true;
            }
        }

        if (CheckCollisionCircleRec((Vector2){player2_x, player2_y}, player2_size, (Rectangle){(float)(food_x - (food_size / 2)), (float)(food_y - (food_size / 2)), (float)food_size, (float)food_size}))
        {
            player2_powerUps++;
            food_x = GetRandomValue(50, width - 50);
            food_y = GetRandomValue(50, height - 50);
            if (player2_powerUps >= 10) {
                player2Wins = true;
                gameOver = true;
            }
        }

        BeginDrawing();
        ClearBackground(WHITE);

        // Update animation frame
        runningTime += GetFrameTime();
        if (runningTime  >= updateTime)
        {
            frame++;
            runningTime = 0.f;
            if (frame > maxFrames) frame = 0;
        }

        // Draw Player 1
        Rectangle source1{frame * (float)fishSprite.width/4.f, 0.f, (float)fishSprite.width/4.f, (float)fishSprite.height};
        Rectangle dest1{player1_x - fishSprite.width / 8, player1_y - fishSprite.height / 2, (float)fishSprite.width/4.f * 3, (float)fishSprite.height * 3};
        if (player1_direction.x < 0) // If player 1 is moving left, flip the sprite horizontally
        {
            source1.width *= -1;
            dest1.x += dest1.width; // Adjust destination x position when flipping
        }
        DrawTexturePro(fishSprite, source1, dest1, Vector2{}, 0.f, WHITE);

        // Draw Player 2
        Rectangle source2{frame * (float)fishSprite.width/4.f, 0.f, (float)fishSprite.width/4.f, (float)fishSprite.height};
        Rectangle dest2{player2_x - fishSprite.width / 8, player2_y - fishSprite.height / 2, (float)fishSprite.width/4.f * 3, (float)fishSprite.height * 3};
        if (player2_direction.x < 0) // If player 2 is moving left, flip the sprite horizontally
        {
            source2.width *= -1;
            dest2.x += dest2.width; // Adjust destination x position when flipping
        }
        DrawTexturePro(fishSprite, source2, dest2, Vector2{}, 0.f, WHITE);

        // Draw food
        DrawCircle(food_x, food_y, food_size, GREEN);

        // Draw hook
        DrawRectangle((int)(hook_x - hook_width / 2), (int)(hook_y - hook_height / 2), (int)hook_width, (int)hook_height, BLACK);

        // Draw scores
        DrawText(TextFormat("Player 1 Power-ups: %d", player1_powerUps), 10, 10, 20, BLACK);
        DrawText(TextFormat("Player 2 Power-ups: %d", player2_powerUps), width - 200, 10, 20, BLACK);

        if (player1Wins)
        {
            DrawText("Player 1 Wins!", width / 2 - MeasureText("Player 1 Wins!", 40) / 2, height / 2 - 20, 40, RED);
        }
        else if (player2Wins)
        {
            DrawText("Player 2 Wins!", width / 2 - MeasureText("Player 2 Wins!", 40) / 2, height / 2 - 20, 40, RED);
        }
        else if (gameOver)
        {
            DrawText("Game Over!", width / 2 - MeasureText("Game Over!", 40) / 2, height / 2 - 20, 40, RED);
        }

        EndDrawing();
    }

    // Unload sprite image
    UnloadTexture(fishSprite);

    CloseWindow();
    return 0;
}
//

