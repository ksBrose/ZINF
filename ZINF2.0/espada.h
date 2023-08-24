#include "raylib.h"
#include "Infos.h"
#define LINHAS 16
#define COLUNAS 24
#define TIMEATTACK 250
#define MAX_INIMIGO 10

typedef struct
{
    Rectangle rec;
    float velocidade;
    char rotacao;
    //Color cor;
    Texture2D direction;
    Sound sound;
    Sound hitwall;
} Sword;

/*Dada uma espada e as sprites, inicializa as variáveis da espada.*/
void inicializa_espada(Sword *espada, Textures *sprites)
{
    espada->rec = (Rectangle)
    {
        x: 0,
        y: 0,
width:
        sprites->attack_down.width,
height:
        sprites->attack_down.height
    };
    espada->direction = sprites->attack_down;
    espada->rotacao = 'S';
    espada->velocidade = 400/FPS;
    espada->sound = LoadSound("./Sons/sword.mp3");
    espada->hitwall = LoadSound("./Sons/swordwall.mp3");

}

/*Dada uma espada e o personagem que possui essa espada, realiza as movimentações dela*/
void move_espada(Sword *espada, Character *player)
{
    //move a espada no sentido que ela esta vidara
    switch (espada->rotacao)
    {
    case 'W':
                if(player->attackUp == true)
        {
        espada->rec.y -= espada->velocidade;
        }
        break;
    case 'S':
                if(player->attackDown == true)
        {
        espada->rec.y += espada->velocidade;
        }
        break;
    case 'A':
                if(player->attackLeft == true)
        {
        espada->rec.x -= espada->velocidade;
        }
        break;
    case 'D':
                if(player->attackRight == true)
        {
        espada->rec.x += espada->velocidade;
        }
        break;
    }
}

/*Dado um inimigo, o mapa do jogo, as settings, o personagem e a espada, checa se a espada atingiu um inimigo. Caso tenha atingido aumenta a pontuação do jogador e faz o inimigo perder uma vida.*/
void checa_colisao_espada(Character *enemy, char mapaJogo[][COLUNAS], Infos *settings, Character *player, Sword espada)
{

    //se a espada encostar no inimigo ativando a colisão o inimigo perde uma vida, é adicionado 100 pontos
    //atacke se torna falso e zera o timer da espada (que tem que estar zerado para o proximo ataque)
    int ja_morreu = 0;

    switch (player->rotacao)
    {
    case 'W':
        if(player->attackUp == true)
        {

            for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
            {
                if (ja_morreu == 0 && enemy[inimigo].vidas > 0)
                {
                    if (CheckCollisionRecs(espada.rec, enemy[inimigo].rec))
                    {
                        enemy[inimigo].vidas--;
                        ja_morreu = 1;
                        if(settings->currentMode == EASY)
                        settings->score += 50;
                        if(settings->currentMode == NORMAL || settings->currentMode == ENDLESS)
                        settings->score += 100;
                        if(settings->currentMode == HARD)
                        settings->score += 150;
                        settings->num_inimigos_mortos++;
                        player->attackUp = false;
                        player->attackTimer = 0;
                        PlaySound(enemy[inimigo].damage);
                    }
                }
            }

            break;
        case 'S':

            if(player->attackDown == true)
            {

                for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
                {
                    if (ja_morreu == 0 && enemy[inimigo].vidas > 0)
                    {
                        if (CheckCollisionRecs(espada.rec, enemy[inimigo].rec))
                        {
                            enemy[inimigo].vidas--;
                            ja_morreu = 1;
                            if(settings->currentMode == EASY)
                            settings->score += 50;
                            if(settings->currentMode == NORMAL || settings->currentMode == ENDLESS)
                            settings->score += 100;
                            if(settings->currentMode == HARD)
                            settings->score += 150;
                            settings->num_inimigos_mortos++;
                            player->attackDown = false;
                            player->attackTimer = 0;
                            PlaySound(enemy[inimigo].damage);
                        }
                    }
                }
            }
            break;
        case 'A':

            if(player->attackLeft == true)
            {

                for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
                {
                    if (ja_morreu == 0 && enemy[inimigo].vidas > 0)
                    {
                        if (CheckCollisionRecs(espada.rec, enemy[inimigo].rec))
                        {
                            enemy[inimigo].vidas--;
                            ja_morreu = 1;
                            if(settings->currentMode == EASY)
                            settings->score += 50;
                            if(settings->currentMode == NORMAL || settings->currentMode == ENDLESS)
                            settings->score += 100;
                            if(settings->currentMode == HARD)
                            settings->score += 150;
                            settings->num_inimigos_mortos++;
                            player->attackLeft = false;
                            player->attackTimer = 0;
                            PlaySound(enemy[inimigo].damage);
                        }

                    }
                }
            }
            break;
        case 'D':

            if(player->attackRight == true)
            {

                for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
                {
                    if (ja_morreu == 0 && enemy[inimigo].vidas > 0)
                    {
                        if (CheckCollisionRecs(espada.rec, enemy[inimigo].rec))
                        {
                            enemy[inimigo].vidas--;
                            ja_morreu = 1;
                            if(settings->currentMode == EASY)
                            settings->score += 50;
                            if(settings->currentMode == NORMAL || settings->currentMode == ENDLESS)
                            settings->score += 100;
                            if(settings->currentMode == HARD)
                            settings->score += 150;
                            settings->num_inimigos_mortos++;
                            player->attackRight = false;
                            player->attackTimer = 0;
                            PlaySound(enemy[inimigo].damage);
                        }

                    }
                }
            }
            break;
        }
    }
}

/*Dado um personagem, a espada e as sprites checa a posição em que a espada está virada e se o jogador tiver acionado o ataque desenha a espada durante determinado período.*/
void checa_ataque(Character *player, Sword *espada, Textures *sprites)
{
    //checa posição que jogador e lado que esta virado e joga espana neste sentido se J for presionado
    //bota a espada na mesma posição do jogador com o sprite correto
    //obs: se o timer da espada não for zero tu não pode atacar novamente
    switch(player->rotacao)
    {
    case 'S':
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackDown = true;
            espada->rec = player->rec;
            espada->rotacao = 'S';
            espada->direction = sprites->attack_down;
            PlaySound(espada->sound);
        }

        break;
    case 'W':
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackUp = true;
            espada->rec = player->rec;
            espada->rotacao = 'W';
            espada->direction = sprites->attack_up;
            PlaySound(espada->sound);
        }

        break;
    case 'D':
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackRight = true;
            espada->rec = player->rec;
            espada->rotacao = 'D';
            espada->direction = sprites->attack_right;
            PlaySound(espada->sound);
        }

        break;
    case 'A':
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackLeft = true;
            espada->rec = player->rec;
            espada->rotacao = 'A';
            espada->direction = sprites->attack_left;
            PlaySound(espada->sound);
        }

        break;
    }
    //se ataque verdadeiro (espada esta ativada)
    //move a espada com velocidade da espada depois de um tempo TIMEATACK a espada desaparece
    //quando desaparecer sua posição zera timer zera (jogador não podera atacar denovo até estar zerado)e seta ataque como falso
    if(player->attackUp == true)
    {
        DrawTexture(espada->direction, espada->rec.x, espada->rec.y, GREEN);
        player->attackTimer += 10;
        if(player->attackTimer > TIMEATTACK)
        {
            player->attackUp = false;
            player->attackTimer = 0;
            espada->rec.x=0;
            espada->rec.y=0;
        }
    }
    if(player->attackDown == true)
    {
        DrawTexture(espada->direction, espada->rec.x, espada->rec.y, GREEN);
        player->attackTimer += 10;
        if(player->attackTimer > TIMEATTACK)
        {
            player->attackDown = false;
            player->attackTimer = 0;
            espada->rec.x=0;
            espada->rec.y=0;
        }
    }
    if(player->attackLeft == true)
    {
        DrawTexture(espada->direction, espada->rec.x, espada->rec.y, GREEN);
        player->attackTimer += 10;
        if(player->attackTimer > TIMEATTACK)
        {
            player->attackLeft = false;
            player->attackTimer = 0;
            espada->rec.x=0;
            espada->rec.y=0;
        }
    }
    if(player->attackRight == true)
    {
        DrawTexture(espada->direction, espada->rec.x, espada->rec.y, GREEN);
        player->attackTimer += 10;
        if(player->attackTimer > TIMEATTACK)
        {
            player->attackRight = false;
            player->attackTimer = 0;
            espada->rec.x=0;
            espada->rec.y=0;
        }
    }
}
