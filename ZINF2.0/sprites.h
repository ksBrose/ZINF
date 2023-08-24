#include "raylib.h"

typedef struct textures
{
    Texture2D ground;
    Texture2D obstacle;
    Texture2D attack_up;
    Texture2D attack_down;
    Texture2D attack_right;
    Texture2D attack_left;
    Texture2D player_up;
    Texture2D player_down;
    Texture2D player_right;
    Texture2D player_left;
    Texture2D enemy_up;
    Texture2D enemy_down;
    Texture2D enemy_left;
    Texture2D enemy_right;
} Textures;

/*Dado uma variavel sprites, carrega as sprites do jogo.*/
void carrega_sprites(Textures *sprites)
{
    sprites->ground = LoadTexture("sprites/Ground.png");
    sprites->obstacle = LoadTexture("sprites/Obstacle.png");
    sprites->attack_up = LoadTexture("sprites/Attack_up.png");
    sprites->attack_down = LoadTexture("sprites/Attack_down.png");
    sprites->attack_right = LoadTexture("sprites/Attack_right.png");
    sprites->attack_left = LoadTexture("sprites/Attack_left.png");
    sprites->player_down = LoadTexture("sprites/Link_front.png");
    sprites->player_up = LoadTexture("sprites/Link_back.png");
    sprites->player_left = LoadTexture("sprites/Link_left.png");
    sprites->player_right = LoadTexture("sprites/Link_right.png");
    sprites->enemy_down = LoadTexture("sprites/Enemy_front.png");
    sprites->enemy_up = LoadTexture("sprites/Enemy_back.png");
    sprites->enemy_left = LoadTexture("sprites/Enemy_left.png");
    sprites->enemy_right = LoadTexture("sprites/Enemy_right.png");
}

/*Dado uma variavel sprites, descarrega as sprites do jogo após o mesmo chegar ao fim.*/
void descarrega_sprites(Textures *sprites)
{

    UnloadTexture(sprites->ground);
    UnloadTexture(sprites->obstacle);
    UnloadTexture(sprites->attack_up);
    UnloadTexture(sprites->attack_down);
    UnloadTexture(sprites->attack_right);
    UnloadTexture(sprites->attack_left);
    UnloadTexture(sprites->player_up);
    UnloadTexture(sprites->player_down);
    UnloadTexture(sprites->player_left);
    UnloadTexture(sprites->player_right);
    UnloadTexture(sprites->enemy_up);
    UnloadTexture(sprites->enemy_down);
    UnloadTexture(sprites->enemy_left);
    UnloadTexture(sprites->enemy_right);
}
