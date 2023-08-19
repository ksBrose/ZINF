#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "menu.h"
#include "scoreboard.h"
#include "character.h"
#include "sprites.h"
#include "espada.h"
#define LINHAS 16
#define COLUNAS 24
#define MAX_INIMIGO 10
#define MAX_SCORES 5
#define MAX_INPUT_CHARS 15

void SetupWindow()
{
    InitWindow(WIDTH, HEIGHT, "ZINF");
    SetTargetFPS(FPS);
    HideCursor();
}

void inicializa_jogo(Infos *settings)
{

    settings->gameArea = (Rectangle) {0, 60, WIDTH, HEIGHT};
    settings->closeGame = false;
    settings->currentLevel = 1;
    settings->currentState = TITLE;
    settings->score = 0;
    settings->num_inimigos = 0;
    settings->num_inimigos_mortos = 0;
}
/*
void inicializa_save(int *letterCount,char *name, int *framesCounter)
{

    char name[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 0;

    Rectangle textBox = { WIDTH/2.0f - 100, 180, 225, 50 };
    int framesCounter = 0;
}*/

void prog_save(int *letterCount,char *name, int framesCounter)
{
    // Get char pressed (unicode character) on the queue
    int key = GetCharPressed();

    // Check if more characters have been pressed on the same frame
    while (key > 0)
    {
        // NOTE: Only allow keys in range [32..125]
        if ((key >= 32) && (key <= 125) && (*letterCount < MAX_INPUT_CHARS))
        {
            name[*letterCount] = (char)key;
            name[*letterCount+1] = '\0'; // Add null terminator at the end of the string.
            *letterCount += 1;
        }

        key = GetCharPressed();  // Check next character in the queue
    }

    if (IsKeyPressed(KEY_BACKSPACE) && *letterCount>0)
    {
        *letterCount -= 1;
        name[*letterCount] = '\0';
    }
    framesCounter++;
}


void le_nivel(Textures *textures, Infos *settings, char mapaJogo[][COLUNAS])
{
    FILE *arq_nivel;
    char caractere, arq_nome[30];

    settings->num_inimigos = 0; // zera a contagem pois o nivel é lido varias vezes durante a execução do jogo

    sprintf(arq_nome, "nivel%d.txt", settings->currentLevel); // retorna uma string nivel(x).txt na variavel arq_nivel onde x é o nivel atual

    arq_nivel = fopen(arq_nome, "r");


    if(arq_nivel != NULL)
    {
        for (int i = 0; i < LINHAS; i++)
        {
            for (int j = 0; j < COLUNAS + 1; j++) // fim dos caracteres mais "\n"
            {
                caractere = getc(arq_nivel);
                if (caractere != '\n' || caractere != '\0')
                {
                    mapaJogo[i][j] = caractere;
                }
                if (caractere == 'M')
                    settings->num_inimigos++;
            }
        }
        fclose(arq_nivel);
    }
    else
        settings->currentState = GAMEOVER;
}

void inicializa_pos_jogador(Character *player, Infos *settings, char mapaJogo[][COLUNAS])
{
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

    return 0;
}

void inicializa_pos_inimigo(Character enemy[], Infos *settings, char mapaJogo[][COLUNAS])
{
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

    return 0;
}

void inicializa_jogador(Character *player, Textures *sprites)
{
    player->width = sprites->player_down.width;
    player->height = sprites->player_down.height;

        player->rec = (Rectangle)
    {
        x: player->posXinicial,
        y: player->posYinicial,
        width: sprites->player_down.width,
        height: sprites->player_down.height
    };

    player->direction = sprites->player_down;
    player->rotacao = 'S';

    player->vidas = 3;
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

    player->attackTimer = 0;

    return 0;
}

void inicializa_inimigo(Character enemy[], Textures *sprites, Infos *settings)
{
    for (int inimigo = 0; inimigo < MAX_INIMIGO; inimigo++)
    {

        enemy[inimigo].rec = (Rectangle)
        {
            x:      enemy[inimigo].posXinicial,
            y:      enemy[inimigo].posYinicial,
            width:  sprites->enemy_down.width,
            height: sprites->enemy_down.height
        };

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

    }

    return 0;
}

void display_jogo(Character *player, Character enemy[], char mapaJogo[][COLUNAS], Textures *sprites, Infos *settings)
{
    char nivel[7], vidas[2], escore[10], inimigos_mortos[10], inimigos[10];
    Rectangle areia, pedras;

    ClearBackground(BLACK);
    DrawRectangleRec(settings->gameArea, BROWN);


//  Desenha barra de status

    sprintf(vidas, "VIDAS: %d", player->vidas);
    DrawText(vidas, 30, 15, 30, WHITE);

    sprintf(nivel, "NIVEL: %d", settings->currentLevel);
    DrawText(nivel, 185, 15, 30, WHITE);

    sprintf(escore, "ESCORE: %d", settings->score);
    DrawText(escore, 330, 15, 30, WHITE);


//  Desenha chao

    for(int linha = 0; linha < 16; linha++)
    {
        for(int coluna = 0; coluna < 24; coluna++)
        {
            areia.x = (coluna*50) + settings->gameArea.x;
            areia.y = (linha*50) + settings->gameArea.y;
            DrawTexture(sprites->ground, areia.x, areia.y, WHITE);
        }
    }

//  Desenha obstaculos

    for(int linha = 0; linha < 16; linha++)
    {
        for(int coluna = 0; coluna < 24; coluna++){
            if(mapaJogo[linha][coluna] == 'P'){
                pedras.x = (coluna*50) + settings->gameArea.x;
                pedras.y = (linha*50) + settings->gameArea.y;
                DrawTexture(sprites->obstacle, pedras.x, pedras.y, WHITE);
            }
        }
    }

//  Desenha monstros

    for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
    {
        if (enemy[inimigo].vidas > 0)
        {
            switch(enemy[inimigo].rotacao)
            {
            case 'W':
                enemy[inimigo].direction = sprites->enemy_up;
                break;
            case 'S':
                enemy[inimigo].direction = sprites->enemy_down;
                break;
            case 'A':
                enemy[inimigo].direction = sprites->enemy_left;
                break;
            case 'D':
                enemy[inimigo].direction = sprites->enemy_right;
                break;
            }
            DrawTexture(enemy[inimigo].direction, enemy[inimigo].rec.x, enemy[inimigo].rec.y, WHITE);
        }
    }


//  Desenha jogador
    if (player->vidas > 0)
    {
        switch (player->rotacao)
        {
        case 'W':
            player->direction = sprites->player_up;

            break;
        case 'S':
            player->direction = sprites->player_down;

            break;
        case 'A':
            player->direction = sprites->player_left;

            break;
        case 'D':
            player->direction = sprites->player_right;

            break;
        }
        DrawTexture(player->direction, player->rec.x, player->rec.y, WHITE);
    }
}

void move_jogador(Character *player)
{
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

void move_inimigo(Character *enemy)
{
    if (enemy->vidas > 0)
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

    }
}

void checa_colisao_mapa(Character *player, char mapaJogo[][COLUNAS], Infos *settings, Textures *sprites, Sword *espada)
{
    // Reseta os status de colisao do personagem
    player->collisionUp = false;
    player->collisionDown = false;
    player->collisionRight = false;
    player->collisionLeft = false;

    // Checa colisao com os limites do mapa
       // WIDTH - 50 = Largura max do mapa - 50 pixel (tamanho do sprite do corpo do link), se a pos atual for maior do que isso significa que ao dar o prox mov o corpo ja ficará pra fora do mapa
       if (player->rec.x >= WIDTH - 50)
       {
           player->collisionRight = true;
       }
       // se a pos x for menor que 0 ao dar o prox mov o corpo ficará pra fora do mapa
       if (player->rec.x <= 0)
       {
           player->collisionLeft = true;
       }
       // se a pos y for menor que 65 (o espaco que o link se move a cada aperto de tecla + 60 de deslocamento de altura sem a barra de status) significa que ao dar o prox mov o corpo ficará pra fora do mapa
       if (player->rec.y <= 60 + 0)
       {
           player->collisionUp = true;
       }
       // HEIGHT - 50 = Altura max do mapa - 50 pixel (tamanho do sprite do corpo do link), se a pos atual for maior do que isso significa que ao dar o prox mov o corpo ja ficará pra fora do mapa
       if (player->rec.y >= HEIGHT - 50)
       {
           player->collisionDown = true;
       }

// Checa colisao com as pedras

    Rectangle pedra;

    for (int linha = 0; linha < LINHAS; linha++)
    {
        for (int coluna = 0; coluna < COLUNAS; coluna++)
        {
            if (mapaJogo[linha][coluna] == 'P')
            {
                pedra.x = (coluna*50) + settings->gameArea.x;
                pedra.y = (linha*50) + settings->gameArea.y;
                pedra.width = sprites->obstacle.width - 15;
                pedra.height = sprites->obstacle.height - 15;

                if(CheckCollisionRecs(espada->rec, pedra))
                {
                    player->attackUp = false;
                    player->attackDown = false;
                    player->attackLeft = false;
                    player->attackRight = false;
                    espada->rec.x=0;
                    espada->rec.y=0;
                    player->attackTimer = 0;

                }

                if (player->rotacao == 'W')
                {
                    while(CheckCollisionRecs(player->rec, pedra))
                    {
                        player->collisionUp = true;
                        player->rec.y++; // joga o player 1 pixel para baixo, de forma a nao colidir com o objeto na direcao W
                        player->rec.y = floor(player->rec.y); // seta o y do player em seu menor valor
                    }


                }
                else if (player->rotacao == 'S')
                {
                    while(CheckCollisionRecs(player->rec, pedra))
                    {
                        player->collisionDown = true;
                        player->rec.y--;
                        player->rec.y = ceil(player->rec.y);
                    }

                }
                else if (player->rotacao == 'D')
                {
                    while(CheckCollisionRecs(player->rec, pedra))
                    {
                        player->collisionRight = true;
                        player->rec.x--;
                        player->rec.x = ceil(player->rec.x);
                    }

                }
                else if (player->rotacao == 'A')
                {
                    while(CheckCollisionRecs(player->rec, pedra))
                    {
                        player->collisionLeft = true;
                        player->rec.x++;
                        player->rec.x = floor(player->rec.x);
                    }

                }
            }
        }
    }
}

void checa_colisao_personagem(Character *enemy, char mapaJogo[][COLUNAS], Infos *settings, Character *player)
{
    int ja_morreu = 0;
    for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
    {
        if (ja_morreu == 0)
        {
            if (enemy[inimigo].vidas > 0)
            {
                if (CheckCollisionRecs(player->rec, enemy[inimigo].rec))
                {
                    player->vidas--;
                    player->rec.x = player->posXinicial;
                    player->rec.y = player->posYinicial;
                    for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
                    {
                        enemy[inimigo].rec.x = enemy[inimigo].posXinicial;
                        enemy[inimigo].rec.y = enemy[inimigo].posYinicial;
                    }
                    ja_morreu = 1;
                }
            }
        }
    }
}





int main()
{
    int select = 0;
    char menu [TAMMENU][15] = {"Iniciar","Extra","Scoreboard", "Sair"};


    SetupWindow();
    Triangle triangulo = SetupTriangle();

    char mapaJogo[16][24];

    Textures sprites;
    carrega_sprites(&sprites); // carrega sprites

    Character player;
    Character enemy[MAX_INIMIGO];

    Sword espada;

    SCORE highscores[MAX_SCORES];
    SCORE player_score;

    le_arquivo_score(highscores);

    Infos settings;
    settings.currentState = LOGO;

    //inicializa_save();
    char name[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 0;

    Rectangle textBox = { WIDTH/2.0f - 250, 400, 500, 50 };
    int framesCounter = 0;

    inicializa_jogo(&settings);
    le_nivel(&sprites, &settings, mapaJogo);
    inicializa_pos_jogador(&player, &settings, mapaJogo);
    inicializa_pos_inimigo(enemy, &settings, mapaJogo);
    inicializa_jogador(&player, &sprites);
    inicializa_inimigo(enemy, &sprites, &settings);
    inicializa_espada(&espada, &sprites);

    while(!WindowShouldClose())
    {
        //Prog
        ProgSelector(&triangulo, &select, menu);

        BeginDrawing();

        switch(settings.currentState)
        {
        case LOGO:
        {
            // TODO: Update LOGO screen variables here!
            DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
            DrawText("WAIT for 2 SECONDS...", 0, 0, 20, GRAY);
            DrawTitle();

            framesCounter++;    // Count frames

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (framesCounter > 60)
            {
                settings.currentState = TITLE;
            }
        }
        break;
        case TITLE:
        {
            // TODO: Update TITLE screen variables here!
            DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
            DrawText("PRESS ENTER to JUMP to SELECTED SCREEN", 0, 0, 20, DARKGREEN);
            DrawSelector(triangulo);
            DrawTitle();
            DrawMenu(menu);

            // Press select and press ENTER to choose screen
            if (IsKeyPressed(KEY_ENTER))
            {
                switch (select)
                {
                case 0:
                    settings.currentState = GAMEPLAY;
                    break;
                case 1:
                    settings.currentState = EXTRA;
                    break;
                case 2:
                    settings.currentState = SCOREBOARD;
                    break;
                case 3:
                    settings.currentState = SAIR;
                    break;
                }
            }
        }
        break;
        case GAMEPLAY:
        {

            // TODO: Update GAMEPLAY screen variables here!
            if (player.vidas == 0) // pula pra tela de game over e reinicializa o nivel para seu estado inicial
            {
                inicializa_jogo(&settings);
                le_nivel(&sprites, &settings, mapaJogo);
                inicializa_pos_jogador(&player, &settings, mapaJogo);
                inicializa_pos_inimigo(enemy, &settings, mapaJogo);
                inicializa_jogador(&player, &sprites);
                inicializa_inimigo(enemy, &sprites, &settings);
                settings.currentState = GAMEOVER;
            }

                   // passar de nivel
            else if (settings.num_inimigos_mortos == settings.num_inimigos)
            {
                settings.currentLevel++;
                // Reseta num de inimigos mortos para o proximo nivel
                settings.num_inimigos_mortos = 0;

                le_nivel(&sprites, &settings, mapaJogo);

                // Reseta rotaçao jogador
                player.rotacao = 'S';

                inicializa_pos_jogador(&player, &settings, mapaJogo);

                // Reseta rectangle jogador com as novas posicoes iniciais já que a rotina inicializa_jogador nao pode ser chamada pois resetaria o score e as vidas
                   player.rec = (Rectangle)
                {
                    x: player.posXinicial,
                    y: player.posYinicial,
                    width: sprites.player_down.width,
                    height: sprites.player_down.height
                };

                inicializa_pos_inimigo(enemy, &settings, mapaJogo);

                inicializa_inimigo(enemy, &sprites, &settings);

                for (int inimigo = 0; inimigo < settings.num_inimigos; inimigo++)
                {
                    enemy[inimigo].velocidade += 0.5; // A cada passagem de nivel velocidade dos inimigos e a dificuldade aumenta
                }

            }

            le_nivel(&sprites, &settings, mapaJogo);
            display_jogo(&player, enemy, mapaJogo, &sprites, &settings);
            move_jogador(&player);
            move_espada(&espada, &player);
            checa_colisao_mapa(&player, mapaJogo, &settings, &sprites, &espada);
            checa_ataque(&player, &espada, &sprites);
            for (int inimigo = 0; inimigo < settings.num_inimigos; inimigo++)
            {
                move_inimigo(&enemy[inimigo]);
                checa_colisao_mapa(&enemy[inimigo], mapaJogo, &settings, &sprites, &espada);
            }
            checa_colisao_personagem(enemy, mapaJogo, &settings, &player);
            checa_colisao_espada(enemy, mapaJogo, &settings, &player, espada);

            for(int i=0; i<MAX_INIMIGO;i++)
            {
                printf("%d ",enemy[i].vidas);
            }
                printf("\n");

        }
        break;
        case EXTRA:
        {

            DrawRectangle(0, 0, WIDTH, HEIGHT, PINK);
            DrawText("EXTRA SCREEN", 20, 20, 40, MAROON);
            DrawText("PRESS ENTER to JUMP to ENDING SCREEN", 0, 0, 20, MAROON);
            // TODO: Update LOADGAME screen variables here!
            if (IsKeyPressed(KEY_ENTER))
            {
                settings.currentState = TITLE;
            }

        }
        break;
        case SCOREBOARD:
        {
            // TODO: Update SCOREBOARD screen variables here!
            le_arquivo_score(highscores);
            DrawRectangle(0, 0, WIDTH, HEIGHT, BLACK);
            /*
            DrawText("SCOREBOARD SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER to RETURN to TITLE SCREEN", 0, 0, 20, DARKBLUE); */
            DrawScoreboard(highscores);

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER))
            {
                settings.currentState = TITLE;
            }
        }
        break;
        case GAMEOVER:
            {
                // TODO: Update ENDING screen variables here!
                // Pedir infos e atualizar hihgscores
                DrawRectangle(0, 0, WIDTH, HEIGHT, BLACK);
                DrawText("GAME OVER", 300, 200, 100, WHITE);
                DrawText("INFORME SEU NOME", 395, 330, 40, ORANGE);

                // Pega o nome do jogadorF
                prog_save(&letterCount,&name,framesCounter);

                DrawRectangleRec(textBox, LIGHTGRAY);
                DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

                DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, WHITE);

                DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, MAX_INPUT_CHARS), 350, 470, 20, DARKGRAY);


                if (letterCount < MAX_INPUT_CHARS)
                {
                    // Draw blinking underscore char
                    if (((framesCounter/10)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, WHITE);
                }
                else DrawText("Press BACKSPACE to delete chars...", 350, 500, 20, GRAY);

                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER))
                {

                    strncpy (player_score.nome, name, MAX_INPUT_CHARS);
                    player_score.score = settings.score;

                    atualiza_highscores(highscores, player_score);


                    escreve_arquivo(highscores, "highscores.bin");
                    settings.currentState = TITLE;
                }
            }
        break;
        case SAIR:
        {
            // TODO: Update ENDING screen variables here!
            // Pedir infos e atualizar hihgscores
            DrawRectangle(0, 0, WIDTH, HEIGHT, BLUE);
            DrawText("FECHANDO JOGO", 300, 400, 40, DARKBLUE);
            DrawText("PRESS ENTER RETURN to TITLE SCREEN", 0, 0, 20, DARKBLUE);

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER))
            {
                settings.currentState = TITLE;
            }
        }
        break;
        default:
            break;
        }
        EndDrawing();

    }

    descarrega_sprites(&sprites);

    return 0;
}
