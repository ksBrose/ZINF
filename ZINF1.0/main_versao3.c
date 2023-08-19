#include <stdio.h>
#include <stdlib.h>
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
    int rotacao;
    int score;
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
    Texture2D player_front;
    Texture2D player_right;
    Texture2D player_left;
    Texture2D enemy_front;
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
    bool inicializa;
    int enemiesCounter;
    int enemiesTimer;
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

void carrega_sprites(Textures *textures)
{
    textures->ground = LoadTexture("sprites/Ground.png");
    textures->obstacle = LoadTexture("sprites/Obstacle.png");
    textures->attack_up = LoadTexture("sprites/Attack_up.png");
    textures->attack_down = LoadTexture("sprites/Attack_down.png");
    textures->attack_right = LoadTexture("sprites/Attack_right.png");
    textures->attack_left = LoadTexture("sprites/Attack_left.png");
    textures->player_front = LoadTexture("sprites/Link_front.png");
    textures->player_left = LoadTexture("sprites/Link_left.png");
    textures->player_right = LoadTexture("sprites/Link_right.png");
    textures->enemy_front = LoadTexture("sprites/Enemy_front.png");
    textures->enemy_left = LoadTexture("sprites/Enemy_left.png");
    textures->enemy_right = LoadTexture("sprites/Enemy_right.png");
}


void descarrega_sprites(Textures *textures)
{

    UnloadTexture(textures->ground);
    UnloadTexture(textures->obstacle);
    UnloadTexture(textures->attack_up);
    UnloadTexture(textures->attack_down);
    UnloadTexture(textures->attack_right);
    UnloadTexture(textures->attack_left);
    UnloadTexture(textures->player_front);
    UnloadTexture(textures->player_left);
    UnloadTexture(textures->player_right);
    UnloadTexture(textures->enemy_front);
    UnloadTexture(textures->enemy_left);
    UnloadTexture(textures->enemy_right);
}

void inicializa_jogo(Infos *settings)
{

    settings->gameArea = (Rectangle) {0, 60, WIDTH, HEIGHT};
    settings->won = false;
    settings->closeGame = false;
    settings->inicializa = true;
    settings->enemiesCounter = 0;
    settings->enemiesTimer = 0;
    settings->currentLevel = 1;
    settings->currentState = TITLE;
    settings->optionSelected = 0;
    settings->score = 0;
}

void le_nivel(Textures *textures, Infos *settings, char mapaJogo[][24])
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

void inicializa_pos_jogador(Character *player, Infos *settings, char mapaJogo[][24])
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

void inicializa_pos_inimigo(Character enemy[], Infos *settings, char mapaJogo[][24])
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
    player->width = sprites->player_front.width;
    player->height = sprites->player_front.height;
    //player->posXatual = player->posXinicial; // pos inicial já inicializa
    //player->posYatual = player->posYinicial; // pos inicial já inicializa

    player->rec = (Rectangle)
    {
        x: player->posXinicial,
        y: player->posYinicial,
        width: sprites->player_front.width,
        height: sprites->player_front.height
    };

    player->direction = sprites->player_front;
    player->rotacao = 0;

    player->vidas = 3;
    player->score = 0;
    player->velocidade = 3;

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
        enemy[inimigo].width = sprites->enemy_front.width;
        enemy[inimigo].height = sprites->enemy_front.height;
        enemy[inimigo].rec.x = enemy[inimigo].posXinicial; // pos inicial já inicializa
        enemy[inimigo].rec.y = enemy[inimigo].posYinicial; // pos inicial já inicializa

        enemy[inimigo].direction = sprites->enemy_front;
        enemy[inimigo].rotacao = 0;

        enemy[inimigo].vidas = 1;
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

void display_jogo(Character *player, Character enemy[], char mapaJogo[][24], Textures *sprites, Infos *settings, Rectangle obstacle[LINHAS][COLUNAS])
{
    char nivel[7], vidas[2], escore[10];
    Rectangle areia, pedras;

    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangleRec(settings->gameArea, BROWN);

  //settings->enemiesTimer++;


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
                obstacle[linha][coluna].x = (coluna*50) + settings->gameArea.x;
                obstacle[linha][coluna].y = (linha*50) + settings->gameArea.y;
                obstacle[linha][coluna].width = 50;
                obstacle[linha][coluna].height = 50;
                pedras.x = (coluna*50) + settings->gameArea.x;
                pedras.y = (linha*50) + settings->gameArea.y;
                DrawRectangleRec(obstacle[linha][coluna], WHITE);
                DrawTexture(sprites->obstacle, pedras.x, pedras.y, WHITE);
            }
        }
    }

//  Desenha monstros

    for (int inimigo = 0; inimigo < MAX_INIMIGO; inimigo++)
        DrawTexture(sprites->enemy_front, enemy[inimigo].rec.x, enemy[inimigo].rec.y, WHITE);


//  Desenha jogador

    player->collisionUp = false;
    player->collisionDown = false;
    player->collisionRight = false;
    player->collisionLeft = false;

    DrawRectangle(player->rec.x,player->rec.y,50,50, BLUE);
    DrawTexture(sprites->player_front, player->rec.x, player->rec.y, WHITE);

    return 0;
}

void move_jogador(Character *player, Rectangle obstacle[LINHAS][COLUNAS])
{
    if(IsKeyDown(KEY_W))
    {
        player->rec.y -= player->velocidade;
    }
    for(int linha = 0; linha < 16; linha++)
    {
        for(int coluna = 0; coluna < 24; coluna++)
        {

            while(CheckCollisionRecs(player->rec, obstacle[linha][coluna]))
            {
                player->rec.y++;
                player->rec.y = floor(player->rec.y);
            }

        }
    }
    if(IsKeyDown(KEY_S))
    {
        player->rec.y += player->velocidade;
    }
        for(int linha = 0; linha < 16; linha++)
    {
        for(int coluna = 0; coluna < 24; coluna++)
        {

            while(CheckCollisionRecs(player->rec, obstacle[linha][coluna]))
            {
                player->rec.y--;
                player->rec.y = ceil(player->rec.y);
            }

        }
    }
    if(IsKeyDown(KEY_A))
    {
        player->rec.x -= player->velocidade;
    }
        for(int linha = 0; linha < 16; linha++)
    {
        for(int coluna = 0; coluna < 24; coluna++)
        {

            while(CheckCollisionRecs(player->rec, obstacle[linha][coluna]))
            {
                player->rec.x++;
                player->rec.x = floor(player->rec.x);
            }

        }
    }
    if(IsKeyDown(KEY_D))
    {
        player->rec.x += player->velocidade;
    }
        for(int linha = 0; linha < 16; linha++)
    {
        for(int coluna = 0; coluna < 24; coluna++)
        {
            while(CheckCollisionRecs(player->rec, obstacle[linha][coluna]))
            {
                player->rec.x--;
                player->rec.x = ceil(player->rec.x);
            }
        }
    }

}
/*
void move_inimigo(Character *enemy)
*/

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
    settings.inicializa=true;

    Rectangle obstacle[16][24];
    Rectangle ground[16][24];



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
            if (framesCounter > 120)
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
            if (settings.inicializa==true){

            inicializa_jogo(&settings);
            le_nivel(&sprites, &settings, mapaJogo);
            inicializa_pos_jogador(&player, &settings, mapaJogo);
            inicializa_pos_inimigo(enemy, &settings, mapaJogo);
            inicializa_jogador(&player, &sprites);
            inicializa_inimigo(enemy, &sprites);

            printf("le_nivel\n");
            settings.inicializa = false;
            //imprime nivel para testes
                for (int i = 0; i < 16; i++)
                {
                    for(int j = 0; j < 24; j++)
                    {
                        printf("%c",mapaJogo[i][j]);
                    }
                    printf("\n");
                }


            }

            move_jogador(&player, obstacle);
            display_jogo(&player, enemy, mapaJogo, &sprites, &settings, obstacle);


            // Press enter to change to TITLE screen
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = TITLE;
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
            DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
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
