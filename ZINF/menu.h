#include "raylib.h"

#define HEIGHT 860
#define WIDTH 1200
#define FPS 60
#define VelRot 60
#define Tfonte 200
#define Mfonte 60
#define TAMMENU 5
#define E 20
#define Yi 400

typedef struct
{
    Vector2 origem;
    int sides;
    int radius;
    float r;
    Color cor;
}
Triangle;

Triangle SetupTriangle();

void DrawSelector(Triangle T);

void DrawTitle();

void DesenhaOrigem(Vector2 O);

void DrawMenu(char menu [TAMMENU][15]);

void ProgSelector(Triangle *T,int *select, char menu [][15]);
