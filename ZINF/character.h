#include "raylib.h"

typedef struct character
{
    Rectangle rec;
    int width;
    int height;
    int posXinicial;
    int posYinicial;
    int vidas;
    int score;
    int attackTimer;
    char rotacao;
    float velocidade;
    bool collisionUp;
    bool collisionDown;
    bool collisionRight;
    bool collisionLeft;
    bool attackUp;
    bool attackDown;
    bool attackRight;
    bool attackLeft;
    bool walking;
    Texture2D direction;
} Character;
