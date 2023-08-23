#include "raylib.h"
#define LINHAS 16
#define COLUNAS 24
#define MAX_INIMIGO 10

/*Dado um inimigo, as settings e o mapa do jogo, inicializa a posição do inimigo fornecido*/
void inicializa_pos_inimigo(Character enemy[], Infos *settings, char mapaJogo[][COLUNAS])
{
    //le o mapa e salva posição de cada inimigo
    int inimigo = 0;
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (mapaJogo[i][j] == 'M')
            {
                enemy[inimigo].posXinicial = (j*50) + settings->gameArea.x; // coluna*50pixel(tamanho da sprite) + posição x ocupada pelo jogo na tela
                enemy[inimigo].posYinicial = (i*50) + settings->gameArea.y; // linhaa*50pixel(tamanho da sprite) + posição y ocupada pelo jogo na tela
                inimigo++;
            }
        }
    }

}

/*Dado um inimigo, as sprites e as settings, inicializa as variáveis do inimigo fornecido*/
void inicializa_inimigo(Character enemy[], Textures *sprites, Infos *settings)
{
    //inicializa todos possiveis inimigos
    for (int inimigo = 0; inimigo < MAX_INIMIGO; inimigo++)
    {

        enemy[inimigo].rec = (Rectangle)
        {
            x:
                enemy[inimigo].posXinicial,
            y:
                enemy[inimigo].posYinicial,
            width:
                  sprites->enemy_down.width,
            height:
                  sprites->enemy_down.height
        };
        //inimigos pares iniciam olhando para o S e impares olhando para D
        if (inimigo%2 == 0)
        {
            enemy[inimigo].direction = sprites->enemy_down;
            enemy[inimigo].rotacao = 'S';
        }
        else
        {
            enemy[inimigo].direction = sprites->enemy_right;
            enemy[inimigo].rotacao = 'D';
        }
        //se o inimigo esta no mapa nicia com 1 ponto de vida se nao não tem vida
        if(enemy[inimigo].rec.x>=0 && enemy[inimigo].rec.x <= WIDTH-50 && enemy[inimigo].rec.y >=60 && enemy[inimigo].rec.y <=HEIGHT-50 )
            enemy[inimigo].vidas = 1;
        else
            enemy[inimigo].vidas = 0;

        enemy[inimigo].velocidade = 2;

        enemy[inimigo].attackUp = false;
        enemy[inimigo].attackDown = false;
        enemy[inimigo].attackRight = false;
        enemy[inimigo].attackLeft = false;
        enemy[inimigo].collisionUp = false;
        enemy[inimigo].collisionDown = false;
        enemy[inimigo].collisionRight = false;
        enemy[inimigo].collisionLeft = false;
        enemy[inimigo].attackTimer = 0;
        enemy[inimigo].walking =false;
        enemy[inimigo].damage = LoadSound("./Sons/damageenemy.mp3");
    }

}

/*Dado um inimigo, o faz se mover aleatoriamente*/
void move_inimigo(Character *enemy)
{
    //variaveis de aleatoriedade
    int r,rmin=0, rmax=10;
    r= rmin + (rand() % (rmax-rmin-1));

    //se inimigo esta vivo E ele esta parado(chegou ao centro de um elemento do mapa)
    if (enemy->vidas > 0 && enemy->walking == false)
    {
        switch(enemy->rotacao)          //confere para que lado o inimigo esta virado
        {
        case 'S':
            if(r >=0 && r <=4)          //5 de 10 de não virar e caminhar
            {
                enemy->rotacao = 'S';
                enemy->walking = true;
            }
            else if(r >=5 && r <=6)     //2 de 10 de virar para o lado e caminhar
            {
                enemy->rotacao = 'A';
                enemy->walking = true;
            }
            else if(r >=7 && r <=8)     //2 de 10 de virar para o lado e caminhar
            {
                enemy->rotacao = 'D';
                enemy->walking = true;
            }
            else if(r == 9 )            //1 de 10 de virar para tras e caminhar
            {
                enemy->rotacao = 'W';
                enemy->walking = true;
            }
            break;
        case 'W':
            if(r >=0 && r <=4)
            {
                enemy->rotacao = 'W';
                enemy->walking = true;
            }
            else if(r >=5 && r <=6)
            {
                enemy->rotacao = 'A';
                enemy->walking = true;
            }
            else if(r >=7 && r <=8)
            {
                enemy->rotacao = 'D';
                enemy->walking = true;
            }
            else if(r == 9 )
            {
                enemy->rotacao = 'S';
                enemy->walking = true;
            }
            break;
        case 'D':
            if(r >=0 && r <=4)
            {
                enemy->rotacao = 'D';
                enemy->walking = true;
            }
            else if(r >=5 && r <=6)
            {
                enemy->rotacao = 'W';
                enemy->walking = true;
            }
            else if(r >=7 && r <=8)
            {
                enemy->rotacao = 'S';
                enemy->walking = true;
            }
            else if(r == 9 )
            {
                enemy->rotacao = 'A';
                enemy->walking = true;
            }
            break;
        case 'A':
            if(r >=0 && r <=4)
            {
                enemy->rotacao = 'A';
                enemy->walking = true;
            }
            else if(r >=5 && r <=6)
            {
                enemy->rotacao = 'W';
                enemy->walking = true;
            }
            else if(r >=7 && r <=8)
            {
                enemy->rotacao = 'S';
                enemy->walking = true;
            }
            else if(r == 9 )
            {
                enemy->rotacao = 'D';
                enemy->walking = true;
            }
            break;
        }
    }
    //se o inimigo esta vivo e esta caminhando (caminha e checa colisão(se colisão vira sentido oposto))
    if (enemy->vidas > 0 && enemy->walking == true)
    {
        switch(enemy->rotacao)
        {
        case 'S':
            if (enemy->collisionDown == false)
                enemy->rec.y += enemy->velocidade;
            else if (enemy->collisionDown == true)
                enemy->rotacao = 'W';
            break;
        case 'W':
            if (enemy->collisionUp == false)
                enemy->rec.y -= enemy->velocidade;
            else if (enemy->collisionUp == true)
                enemy->rotacao = 'S';
            break;
        case 'D':
            if (enemy->collisionRight == false)
                enemy->rec.x += enemy->velocidade;
            else if (enemy->collisionRight == true)
                enemy->rotacao = 'A';
            break;
        case 'A':
            if (enemy->collisionLeft == false)
                enemy->rec.x -= enemy->velocidade;
            else if (enemy->collisionLeft == true)
                enemy->rotacao = 'D';
            break;
        }
        //se inimigo chega no centro de um elemento do mapa ele para
        if((int)enemy->rec.x % 50 == 0 && ((int)enemy->rec.y - 10) % 50 == 0)
        {
            enemy->walking = false;
        }
    }
}

