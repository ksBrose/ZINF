#include "raylib.h"
#define LINHAS 16
#define COLUNAS 24

/*Dado um personagem, as settings e o mapa do jogo, inicializa a posição do personagem fornecido.*/
void inicializa_pos_jogador(Character *player, Infos *settings, char mapaJogo[][COLUNAS])
{
    //salva posição do jogador no mapa
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (mapaJogo[i][j] == 'J')
            {
                player->posXinicial = (j*50) + settings->gameArea.x; // coluna*50pixel(tamanho da sprite) + posição x ocupada pelo jogo na tela
                player->posYinicial = (i*50) + settings->gameArea.y; // linhaa*50pixel(tamanho da sprite) + posição y ocupada pelo jogo na tela
            }
        }
    }

}

/*Dado um personagem, as sprites e as settings, inicializa as variáveis do personagem fornecido.*/
void inicializa_jogador(Character *player, Textures *sprites, Infos *settings)
{
    player->width = sprites->player_down.width;
    player->height = sprites->player_down.height;

    player->rec = (Rectangle)
    {
x:
        player->posXinicial,
       y:
               player->posYinicial,
       width:
               sprites->player_down.width,
       height:
               sprites->player_down.height
    };
    //inicia o jogo virado para baixo
    player->direction = sprites->player_down;
    player->rotacao = 'S';
    if(settings->currentMode == EASY)
    player->vidas = 5;
    if(settings->currentMode == NORMAL || settings->currentMode == ENDLESS)
    player->vidas = 3;
    if(settings->currentMode == HARD)
    player->vidas = 1;
    player->score = 0;
    player->velocidade = 4;

    player->attackUp = false;
    player->attackDown = false;
    player->attackRight = false;
    player->attackLeft = false;
    player->collisionUp = false;
    player->collisionDown = false;
    player->collisionRight = false;
    player->collisionLeft = false;
    //timer da espada (visivel)
    player->attackTimer = 0;
    player->damage = LoadSound("./Sons/damageplayer.mp3");

}

/*Dado um personagem, o move de acordo com as teclas pressionadas pelo jogador.*/
void move_jogador(Character *player)
{
    //move o jogador usando WASD ou setas
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        player->rotacao = 'W';
        if (player->collisionUp == false)
            player->rec.y -= player->velocidade;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        player->rotacao = 'S';
        if (player->collisionDown == false)
            player->rec.y += player->velocidade;
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        player->rotacao = 'A';
        if (player->collisionLeft == false)
            player->rec.x -= player->velocidade;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        player->rotacao = 'D';
        if (player->collisionRight == false)
            player->rec.x += player->velocidade;
    }

}

/*Dado um inimigo, o mapa do jogo, as settings e um personagem, checa se o personagem se choca a um inimigo. Nesse caso, o tira uma vida e faz todos personagens voltarem às posições iniciais do nivel.*/
void checa_colisao_personagem(Character *enemy, char mapaJogo[][COLUNAS], Infos *settings, Character *player)
{
    int ja_morreu = 0;  //variavel para nao perder multiplas vidas
    for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++) //checa para inimigos vivos vezes
    {
        if (ja_morreu == 0)
        {
            if (enemy[inimigo].vidas > 0) //se inimigo ainda esta vivo
            {
                if (CheckCollisionRecs(player->rec, enemy[inimigo].rec)) //checa coolisão entre blocos
                {
                    player->vidas--;                        //perde 1 vida
                    player->rec.x = player->posXinicial;    //retorna posição inicial
                    player->rec.y = player->posYinicial;
                    PlaySound(player->damage);
                    for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++) //retorna todos inimigos vivos para posição inicial respectiva no mapa
                    {
                        enemy[inimigo].rec.x = enemy[inimigo].posXinicial;
                        enemy[inimigo].rec.y = enemy[inimigo].posYinicial;
                    }
                    ja_morreu = 1;  //variavel controle para nao perder multiplas vidas
                }
            }
        }
    }
}

