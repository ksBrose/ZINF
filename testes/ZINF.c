#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define HEIGHT 860
#define WIDTH 1200
#define FPS 60
#define VelRot 60
#define Tfonte 200
#define Mfonte 60
#define TAMMENU 4
#define E 20
#define Yi 400

typedef enum GameScreen
{
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    SCOREBOARD,
    ENDING
} GameScreen;

typedef struct
{
    Vector2 origem;
    int sides;
    int radius;
    float r;
    Color cor;
}
Triangle;

void SetupWindow()
{
    InitWindow(WIDTH, HEIGHT, "ZINF");
    //ToggleFullscreen();
    SetTargetFPS(FPS);
    HideCursor();
    GameScreen currentScreen = LOGO;
}

Triangle SetupTriangle()
{
    return (Triangle)
    {
origem:
        (Vector2)
        {
            x: 300,
            y: 300
        },
sides:
        3,
radius:
        30,
r:
        0,
cor:
        (Color)
        {
            r: 200,
            g: 200,
            b: 0,
            a: 255
        }
    };
}

void ProgBloco(Triangle *T,int *select, char menu [TAMMENU][15])
{
    T->r += VelRot/FPS;
    /*B->rec.x = GetMouseX();
    B->rec.y = GetMouseY();*/
    if(IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_J))
    {
        T->cor = (Color)
        {
r:
            GetRandomValue(0, 255),
               g:
                           GetRandomValue(0, 255),
               b:
                           GetRandomValue(0, 255),
                           a: 255
        };
    }
    if(IsKeyPressed(KEY_W))
    {
        if(*select > 0)
            (*select)-= 1;
        else
            (*select = TAMMENU-1);
    }
    if(IsKeyPressed(KEY_S))
    {
        if(*select < TAMMENU-1)
            (*select)+=1;
        else
            (*select)=0;
    }
    switch (*select)
    {
    case 0:
        T->origem.x=(WIDTH - MeasureText(menu[*select], Mfonte))/2-50;
        T->origem.y=Yi+ Mfonte/2 + *select * (Mfonte + E);
        break;
    case 1:
        T->origem.x=(WIDTH - MeasureText(menu[*select], Mfonte))/2-50;
        T->origem.y=Yi+ Mfonte/2 + *select * (Mfonte + E);
        break;
    case 2:
        T->origem.x=(WIDTH - MeasureText(menu[*select], Mfonte))/2-50;
        T->origem.y=Yi+ Mfonte/2 + *select * (Mfonte + E);
        break;
    case 3:
        T->origem.x=(WIDTH - MeasureText(menu[*select], Mfonte))/2-50;
        T->origem.y=Yi+ Mfonte/2 + *select * (Mfonte + E);
        break;
    }
}

void DesenhaOrigem(Vector2 O)
{
    DrawRectangle(O.x-10, O.y, 21, 1, RED);
    DrawRectangle(O.x, O.y-10, 1, 21, RED);
}

void DrawSelector(Triangle T)
{

    DrawPoly(T.origem, T.sides, T.radius, T.r, T.cor);
    //DesenhaOrigem((Vector2){T.origem.x, T.origem.y});
}

void DrawTitle()
{
    ClearBackground(BLACK);
    DrawText("ZINF", (WIDTH - MeasureText("ZINF",Tfonte))/2, 100, Tfonte, RAYWHITE);
}

void DrawMenu(char menu [TAMMENU][15])
{
    int i;
    for(int i = 0; i < TAMMENU ; i++)
    {
        DrawText(menu[i], (WIDTH/2 - MeasureText(menu[i],Mfonte)/2), Yi + i * (Mfonte + E), Mfonte, LIGHTGRAY);
    }
}

int main()
{
    int select = 0;
    char menu [TAMMENU][15] = {"Iniciar","Carregar Jogo","Scoreboard", "Sair"};
    GameScreen currentScreen = LOGO;
    int framesCounter = 0;
    SetupWindow();
    Triangle bloco = SetupTriangle();
    while(!WindowShouldClose())
    {
        //Prog
        ProgBloco(&bloco, &select, menu);
        switch(currentScreen)
        {
        case LOGO:
        {
            // TODO: Update LOGO screen variables here!

            framesCounter++;    // Count frames

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (framesCounter > 120)
            {
                currentScreen = TITLE;
            }
        }
        break;
        case TITLE:
        {
            // TODO: Update TITLE screen variables here!

            // Press select and press ENTER to choose screen
            if (IsKeyPressed(KEY_ENTER))
            {
                switch (select)
                {
                case 0:
                    currentScreen = GAMEPLAY;
                    break;
                case 1:

                    break;
                case 2:
                    currentScreen = SCOREBOARD;
                    break;
                case 3:
                    currentScreen = ENDING;
                    break;
                }
            }
        }
        break;
        case GAMEPLAY:
        {
            // TODO: Update GAMEPLAY screen variables here!

            // Press enter to change to TITLE screen
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = TITLE;
            }
        }
        break;
        case SCOREBOARD:
        {
            // TODO: Update SCOREBOARD screen variables here!

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = TITLE;
            }
        }
        break;
        case ENDING:
        {
            // TODO: Update ENDING screen variables here!

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = TITLE;
            }
        }
        break;
        default:
            break;
        }
        //Draw
        BeginDrawing();

        switch(currentScreen)
        {
        case LOGO:
        {
            // TODO: Draw LOGO screen here!
            DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
            DrawText("WAIT for 2 SECONDS...", 0, 0, 20, GRAY);
            DrawTitle();

        }
        break;
        case TITLE:
        {
            // TODO: Draw TITLE screen here!
            //DrawRectangle(0, 0, WIDTH, HEIGHT, GREEN);
            DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
            DrawText("PRESS ENTER to JUMP to SELECTED SCREEN", 0, 0, 20, DARKGREEN);
            DrawSelector(bloco);
            DrawTitle();
            DrawMenu(menu);

        }
        break;
        case GAMEPLAY:
        {
            // TODO: Draw GAMEPLAY screen here!
            DrawRectangle(0, 0, WIDTH, HEIGHT, PURPLE);
            DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
            DrawText("PRESS ENTER to JUMP to ENDING SCREEN", 0, 0, 20, MAROON);

        }
        break;
        case SCOREBOARD:
        {
            // TODO: Draw ENDING screen here!
            DrawRectangle(0, 0, WIDTH, HEIGHT, RED);
            DrawText("SCOREBOARD SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER to RETURN to TITLE SCREEN", 0, 0, 20, DARKBLUE);

        }
        break;
        case ENDING:
        {
            // TODO: Draw ENDING screen here!
            DrawRectangle(0, 0, WIDTH, HEIGHT, BLUE);
            DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER RETURN to TITLE SCREEN", 0, 0, 20, DARKBLUE);

        }
        break;
        default:
            break;
        }
        EndDrawing();
    }

    return 0;
}
