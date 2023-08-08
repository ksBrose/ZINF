#include "menu.h"

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

void DesenhaOrigem(Vector2 O)
{
    DrawRectangle(O.x-10, O.y, 21, 1, RED);
    DrawRectangle(O.x, O.y-10, 1, 21, RED);
}

void DrawMenu(char menu [TAMMENU][15])
{
    int i;
    for(int i = 0; i < TAMMENU ; i++)
    {
        DrawText(menu[i], (WIDTH/2 - MeasureText(menu[i],Mfonte)/2), Yi + i * (Mfonte + E), Mfonte, LIGHTGRAY);
    }
}
