#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "menu.h"
#define LINHAS 16
#define COLUNAS 24
#define MAX_INIMIGO 8

typedef struct character
{
    int width;
    int height;
    int posXinicial;
    int posYinicial;
    //int posXatual;
    //int posYatual;
    Rectangle rec;
    int vidas;
    int score;
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
    Texture2D direction;
} Character;

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

typedef enum GameScreen
{
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    EXTRA,
    SCOREBOARD,
    ENDING
} GameScreen;

typedef struct informacoes
{
    GameScreen currentState;
    Rectangle gameArea;
    bool won;
    bool closeGame;
    int currentLevel;
    int optionSelected;
    int score;
} Infos;

void SetupWindow()
{
    InitWindow(WIDTH, HEIGHT, "ZINF");
    //ToggleFullscreen();
    SetTargetFPS(FPS);
    HideCursor();
    GameScreen currentScreen = LOGO;
}

void ProgSelector(Triangle *T,int *select, char menu [TAMMENU][15])
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
    case 4:
        T->origem.x=(WIDTH - MeasureText(menu[*select], Mfonte))/2-50;
        T->origem.y=Yi+ Mfonte/2 + *select * (Mfonte + E);
        break;
    }
}

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


void descarrega_sprites(Textures *textures)
{

    UnloadTexture(textures->ground);
    UnloadTexture(textures->obstacle);
    UnloadTexture(textures->attack_up);
    UnloadTexture(textures->attack_down);
    UnloadTexture(textures->attack_right);
    UnloadTexture(textures->attack_left);
    UnloadTexture(textures->player_up);
    UnloadTexture(textures->player_down);
    UnloadTexture(textures->player_left);
    UnloadTexture(textures->player_right);
    UnloadTexture(textures->enemy_up);
    UnloadTexture(textures->enemy_down);
    UnloadTexture(textures->enemy_left);
    UnloadTexture(textures->enemy_right);
}

void inicializa_jogo(Infos *settings)
{

    settings->gameArea = (Rectangle) {0, 60, WIDTH, HEIGHT};
    settings->won = false;
    settings->closeGame = false;
    settings->currentLevel = 1;
    settings->currentState = TITLE;
    settings->optionSelected = 0;
    settings->score = 0;
}

void le_nivel(Textures *textures, Infos *settings, char mapaJogo[][COLUNAS])
{
    FILE *arq_nivel;
    char caractere, arq_nome[30];

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
            }
        }
        fclose(arq_nivel);
    }
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
                enemy[inimigo].vidas = 1;
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
    //player->posXatual = player->posXinicial; // pos inicial já inicializa
    //player->posYatual = player->posYinicial; // pos inicial já inicializa

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
    player->velocidade = 200/FPS;

    player->attackUp = false;
    player->attackDown = false;
    player->attackRight = false;
    player->attackLeft = false;
    player->collisionUp = false;
    player->collisionDown = false;
    player->collisionRight = false;
    player->collisionLeft = false;

    return 0;
}

void inicializa_inimigo(Character enemy[], Textures *sprites)
{
    for (int inimigo = 0; inimigo < MAX_INIMIGO; inimigo++)
    {
        //enemy[inimigo].width = sprites->enemy_down.width;
        //enemy[inimigo].height = sprites->enemy_down.height;
        //enemy[inimigo].posXatual = enemy[inimigo].posXinicial; // pos inicial já inicializa
        //enemy[inimigo].posYatual = enemy[inimigo].posYinicial; // pos inicial já inicializa

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

        //enemy[inimigo].vidas = 1;
        enemy[inimigo].velocidade = 1.5;

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
    char nivel[7], vidas[2], escore[10];
    Rectangle areia, pedras;

    ClearBackground(BLACK);
    DrawRectangleRec(settings->gameArea, BROWN);


//  Desenha barra de status

    sprintf(vidas, "VIDAS: %d", player->vidas);
    DrawText(vidas, 30, 15, 30, WHITE);

    sprintf(nivel, "NIVEL: %d", settings->currentLevel);
    DrawText(nivel, 185, 15, 30, WHITE);

    sprintf(escore, "ESCORE: %d", player->score);
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

    for (int inimigo = 0; inimigo < MAX_INIMIGO; inimigo++)
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
            DrawRectangle(enemy[inimigo].rec.x,enemy[inimigo].rec.y,50,50, RED);
            DrawTexture(enemy[inimigo].direction, enemy[inimigo].rec.x, enemy[inimigo].rec.y, WHITE);
        }
    }


//  Desenha jogador
    if (player->vidas > 0)
    {
        DrawRectangle(player->rec.x,player->rec.y,50,50, BLUE);
        DrawTexture(player->direction, player->rec.x, player->rec.y, WHITE);

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
    switch(enemy->rotacao)
    {
    case 'S':
        if (enemy->collisionDown == false)
            enemy->rec.y += enemy->velocidade;
        else if (enemy->collisionDown == true){
            enemy->rotacao = 'W';
            enemy->rec.y --;}
        break;
    case 'W':
        if (enemy->collisionUp == false)
            enemy->rec.y -= enemy->velocidade;
        else if (enemy->collisionUp == true){
            enemy->rotacao = 'S';
            enemy->rec.y ++;}
        break;
    case 'D':
        if (enemy->collisionRight == false)
            enemy->rec.x += enemy->velocidade;
        else if (enemy->collisionRight == true){
            enemy->rotacao = 'A';
            enemy->rec.x --;}
        break;
    case 'A':
        if (enemy->collisionLeft == false)
            enemy->rec.x -= enemy->velocidade;
        else if (enemy->collisionLeft == true){
            enemy->rotacao = 'D';
            enemy->rec.x ++;}
        break;
    }
}

void checa_colisao_mapa(Character *player, char mapaJogo[][COLUNAS], Infos *settings, Textures *sprites)
{
/**
    // Checa colisao com os limites do mapa
       // WIDTH - 50 = Largura max do mapa - 50 pixel (tamanho do sprite do corpo do link), se a pos atual for maior do que isso significa que ao dar o prox mov o corpo ja ficará pra fora do mapa
       if (player->rec.x >= WIDTH - 50)
       {
           player->collisionRight = true;
           player->rec.x--;
           player->rec.x = ceil(player->rec.x);
       }
       else if (player->rec.x < WIDTH - 50)
           player->collisionRight = false;
       // se a pos x for menor que 5 (o espaco que o link se move a cada aperto de tecla) significa que ao dar o prox mov o corpo ficará pra fora do mapa
       if (player->rec.x <= 0)
       {
           player->collisionLeft = true;
           player->rec.x++;
           player->rec.x = floor(player->rec.x);
       }
       else if (player->rec.x > 0)
           player->collisionLeft = false;
       // se a pos y for menor que 65 (o espaco que o link se move a cada aperto de tecla + 60 de deslocamento de altura sem a barra de status) significa que ao dar o prox mov o corpo ficará pra fora do mapa
       if (player->rec.y <= 60 + 0)
       {
           player->collisionUp = true;
           player->rec.y++;
           player->rec.y = floor(player->rec.y);
       }
       else if (player->rec.y > 60 + 0)
           player->collisionUp = false;
       // HEIGHT - 50 = Altura max do mapa - 50 pixel (tamanho do sprite do corpo do link), se a pos atual for maior do que isso significa que ao dar o prox mov o corpo ja ficará pra fora do mapa
       if (player->rec.y >= HEIGHT - 50)
       {
           player->collisionDown = true;
           player->rec.y--;
           player->rec.y = ceil(player->rec.y);
       }
       else if (player->rec.y < HEIGHT - 50)
           player->collisionDown = false;

**/
    if (player->rec.y < HEIGHT - 50)
        player->collisionDown = false;
    if (player->rec.y > 60 + 0)
        player->collisionUp = false;
    if (player->rec.x > 0)
        player->collisionLeft = false;
    if (player->rec.x < WIDTH - 50)
        player->collisionRight = false;

    Rectangle borda;


    borda.x = 49;
    borda.y =  60+49 ;
    borda.width =   (WIDTH -98);
    borda.height =  (HEIGHT -158);

    if (player->rotacao == 'W')
    {
        if(!CheckCollisionRecs(player->rec, borda))
        {
            player->collisionUp = true;
            player->rec.y++;
            player->rec.y = floor(player->rec.y);
        }

//player->collisionUp = false;
    }
    else if (player->rotacao == 'S')
    {
        if(!CheckCollisionRecs(player->rec, borda))
        {
            player->collisionDown = true;
            player->rec.y--;
            player->rec.y = ceil(player->rec.y);
        }
//player->collisionDown = false;
    }
    else if (player->rotacao == 'D')
    {
        if(!CheckCollisionRecs(player->rec, borda))
        {
            player->collisionRight = true;
            player->rec.x--;
            player->rec.x = ceil(player->rec.x);
        }
//player->collisionRight = false;
    }
    else if (player->rotacao == 'A')
    {
        if(!CheckCollisionRecs(player->rec, borda))
        {
            player->collisionLeft = true;
            player->rec.x++;
            player->rec.x = floor(player->rec.x);
        }
//player->collisionLeft = false;
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

                if (player->rotacao == 'W')
                {
                    while(CheckCollisionRecs(player->rec, pedra))
                    {
                        player->collisionUp = true;
                        player->rec.y++;
                        player->rec.y = floor(player->rec.y);
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
    for (int inimigo = 0; inimigo < MAX_INIMIGO; inimigo++)
    {
        if (ja_morreu == 0)
        {
            if (CheckCollisionRecs(player->rec, enemy[inimigo].rec))
            {
                player->vidas--;
                player->rec.x = player->posXinicial;
                player->rec.y = player->posYinicial;
                ja_morreu = 1;
            }

        }
    }
 }



int main()
{
    int select = 0;
    char menu [TAMMENU][15] = {"Iniciar","Carregar Jogo","Extra","Scoreboard", "Sair"};
    GameScreen currentScreen = LOGO;
    int framesCounter = 0;
    SetupWindow();
    Triangle bloco = SetupTriangle();

    char mapaJogo[16][24];

    Textures sprites;
    carrega_sprites(&sprites); // carrega sprites

    Character player;
    Character enemy[MAX_INIMIGO];

    Infos settings;

    inicializa_jogo(&settings);
    le_nivel(&sprites, &settings, mapaJogo);
    inicializa_pos_jogador(&player, &settings, mapaJogo);
    inicializa_pos_inimigo(enemy, &settings, mapaJogo);
    inicializa_jogador(&player, &sprites);
    inicializa_inimigo(enemy, &sprites);


    while(!WindowShouldClose())
    {
        BeginDrawing();
        //Prog
        ProgSelector(&bloco, &select, menu);
        switch(currentScreen)
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
               currentScreen = TITLE;
            }
        }
        break;
        case TITLE:
        {
            // TODO: Update TITLE screen variables here!
            DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
            DrawText("PRESS ENTER to JUMP to SELECTED SCREEN", 0, 0, 20, DARKGREEN);
            DrawSelector(bloco);
            DrawTitle();
            DrawMenu(menu);

            // Press select and press ENTER to choose screen
            if (IsKeyPressed(KEY_ENTER))
            {
                switch (select)
                {
                case 0:
                    currentScreen = GAMEPLAY;
                    break;
                case 1:
                    currentScreen = EXTRA;
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
                if (player.vidas == 0) // pula pra tela de game over e reinicializa o nivel para seu estado inicial
                {
                    currentScreen = ENDING;
                    inicializa_jogo(&settings);
                    le_nivel(&sprites, &settings, mapaJogo);
                    inicializa_pos_jogador(&player, &settings, mapaJogo);
                    inicializa_pos_inimigo(enemy, &settings, mapaJogo);
                    inicializa_jogador(&player, &sprites);
                    inicializa_inimigo(enemy, &sprites);
                }
                le_nivel(&sprites, &settings, mapaJogo);
                display_jogo(&player, enemy, mapaJogo, &sprites, &settings);
                move_jogador(&player);
                checa_colisao_mapa(&player, mapaJogo, &settings, &sprites);
                for (int inimigo = 0; inimigo < MAX_INIMIGO; inimigo++)
                {
                    move_inimigo(&enemy[inimigo]);
                    checa_colisao_mapa(&enemy[inimigo], mapaJogo, &settings, &sprites);
                }
                checa_colisao_personagem(enemy, mapaJogo, &settings, &player);
                printf("%f , %f\n",player.rec.x,player.rec.y);
                // Press enter to change to TITLE screen
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = ENDING;
                }
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
                currentScreen = TITLE;
            }
        }
        case SCOREBOARD:
        {
            // TODO: Update SCOREBOARD screen variables here!
            DrawRectangle(0, 0, WIDTH, HEIGHT, RED);
            DrawText("SCOREBOARD SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER to RETURN to TITLE SCREEN", 0, 0, 20, DARKBLUE);

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
            DrawRectangle(0, 0, WIDTH, HEIGHT, BLUE);
            DrawText("GAME OVER", 300, 400, 40, DARKBLUE);
            DrawText("PRESS ENTER RETURN to TITLE SCREEN", 0, 0, 20, DARKBLUE);

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
        EndDrawing();

    }

    descarrega_sprites(&sprites);

    return 0;
}
