#include "raylib.h"
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <raymath.h>
#include <sys/socket.h>
#include <sys/types.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define TEXT_NORMAL 20

enum GameState { MENU, PLAY, END };

void menu(GameState &gs) {
  int recBoxPadding = 20;

  ClearBackground(BLACK);
  DrawText("PongMulti", (WINDOW_WIDTH - MeasureText("PongMulti", 50)) / 2, 100,
           50, WHITE);
  Rectangle rec1 = {
      (float)((WINDOW_WIDTH - MeasureText("PLAY", TEXT_NORMAL)) / 2.0) -
          recBoxPadding,
      (float)400 - 10,
      (float)MeasureText("PLAY", TEXT_NORMAL) + (2 * recBoxPadding), 35};

  DrawRectangleRounded(rec1, 10, 5, WHITE);
  DrawText("PLAY", (WINDOW_WIDTH - MeasureText("PLAY", TEXT_NORMAL)) / 2, 400,
           TEXT_NORMAL, BLACK);

  Rectangle rec2 = {
      (float)((WINDOW_WIDTH - MeasureText("EXIT", TEXT_NORMAL)) / 2.0) -
          recBoxPadding,
      (float)500 - 10,
      (float)MeasureText("EXIT", TEXT_NORMAL) + (2 * recBoxPadding), 35};
  DrawRectangleRounded(rec2, 10, 5, WHITE);
  DrawText("EXIT", (WINDOW_WIDTH - MeasureText("EXIT", TEXT_NORMAL)) / 2, 500,
           TEXT_NORMAL, BLACK);

  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(GetMousePosition(), rec1)) {
      TraceLog(LOG_INFO, "PLAY");
      gs = GameState::PLAY;
    } else if (CheckCollisionPointRec(GetMousePosition(), rec2)) {
      TraceLog(LOG_INFO, "EXIT");
      CloseWindow();
    }
  }
}

float playerPosY = (WINDOW_HEIGHT - 60.0) / 2;
Vector2 ballPos = {WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0};
Vector2 ballDirection = Vector2Normalize({0.5, 0.5});
float ballSpeed = 4.0;

void play(GameState &gs) {
  ClearBackground(BLACK);

  const float platformWidth = 10.0;
  const float platformHeight = 60.0;
  const float speed = 6.0;
  const float edgePadding = 4.0;
  const float ballRadius = 15.0;

  Rectangle player = {10, playerPosY, platformWidth, platformHeight};
  DrawCircle(ballPos.x, ballPos.y, ballRadius, WHITE);

  DrawRectangleRec(player, WHITE);

  if (IsKeyDown(KEY_DOWN)) {
    if (playerPosY + speed + platformHeight + edgePadding <= WINDOW_HEIGHT)
      playerPosY += speed;
  } else if (IsKeyDown(KEY_UP)) {
    if (playerPosY - speed - edgePadding >= 0)
      playerPosY -= speed;
  }

  if (CheckCollisionCircleRec(ballPos, ballRadius, player)) {
    TraceLog(LOG_INFO, "BALL HIT");
    ballDirection.x = -ballDirection.x;
    float factor = (ballPos.y - player.y) / platformHeight;
    factor *= 2.0;
    factor -= 1.0;
    ballDirection.y = factor;
    ballDirection = Vector2Normalize(ballDirection);
  }

  if (ballPos.x + (ballDirection.x * ballSpeed) + edgePadding + ballRadius <
      WINDOW_WIDTH)
    ballPos.x += ballDirection.x * ballSpeed;
  else {
    ballDirection.x = -ballDirection.x;
  }
  if (ballPos.y + (ballDirection.y * ballSpeed) + edgePadding + ballRadius <
          WINDOW_HEIGHT &&
      (ballPos.y + (ballDirection.y * ballSpeed) - edgePadding > 0))
    ballPos.y += ballDirection.y * ballSpeed;
  else {
    ballDirection.y = -ballDirection.y;
  }

  if (ballPos.x < 0) {
    gs = GameState::END;
  }
}
void end(GameState &gs) {
  ClearBackground(BLACK);
  playerPosY = (WINDOW_HEIGHT - 60.0) / 2;
  ballPos = {WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0};
  ballDirection = Vector2Normalize({0.5, 0.5});

  float recBoxPadding = 20;

  DrawText("You Lost! Sikes",
           (WINDOW_WIDTH - MeasureText("You Lost! Sikes", 50)) / 2, 100, 50,
           WHITE);

  Rectangle rec1 = {
      (float)((WINDOW_WIDTH - MeasureText("PLAY AGAIN", TEXT_NORMAL)) / 2.0) -
          recBoxPadding,
      (float)400 - 10,
      (float)MeasureText("PLAY AGAIN", TEXT_NORMAL) + (2 * recBoxPadding), 35};

  DrawRectangleRounded(rec1, 10, 5, WHITE);
  DrawText("PLAY AGAIN",
           (WINDOW_WIDTH - MeasureText("PLAY AGAIN", TEXT_NORMAL)) / 2, 400,
           TEXT_NORMAL, BLACK);

  Rectangle rec2 = {
      (float)((WINDOW_WIDTH - MeasureText("EXIT", TEXT_NORMAL)) / 2.0) -
          recBoxPadding,
      (float)500 - 10,
      (float)MeasureText("EXIT", TEXT_NORMAL) + (2 * recBoxPadding), 35};
  DrawRectangleRounded(rec2, 10, 5, WHITE);
  DrawText("EXIT", (WINDOW_WIDTH - MeasureText("EXIT", TEXT_NORMAL)) / 2, 500,
           TEXT_NORMAL, BLACK);

  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(GetMousePosition(), rec1)) {
      TraceLog(LOG_INFO, "PLAY AGAIN");
      gs = GameState::PLAY;
    } else if (CheckCollisionPointRec(GetMousePosition(), rec2)) {
      TraceLog(LOG_INFO, "EXIT");
      CloseWindow();
    }
  }
}

int main(int argc, char *argv[]) {

  /*  if (argc != 4) {
      TraceLog(LOG_FATAL, "./a.out IpAddr_to_connect PORT");
      std::exit(1);
    }
    */

  GameState gs = GameState::MENU;

  SetTargetFPS(120);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PongMulti");

  // setLocalSockFD(argv[3]);
  // setConnectSocketFD(argv[1], argv[2]);

  while (!WindowShouldClose()) {
    BeginDrawing();

    DrawFPS(WINDOW_WIDTH - MeasureText("XXX FPS", 20), 0);
    if (gs == GameState::MENU) {
      menu(gs);
    } else if (gs == GameState::PLAY) {
      play(gs);
    } else if (gs == GameState::END) {
      end(gs);
    }

    if (!WindowShouldClose())
      EndDrawing();
  }
}
