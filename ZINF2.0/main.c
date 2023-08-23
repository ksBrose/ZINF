#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "menu.h"
#include "scoreboard.h"
#include "character.h"
#include "sprites.h"
#include "espada.h"
#include "player.h"
#include "enemy.h"
#include "map.h"

void SetupWindow()
{
    InitWindow(WIDTH, HEIGHT, "ZINF");
    SetTargetFPS(FPS);
    HideCursor();
    InitAudioDevice();
}

void inicializa_jogo(Infos *settings)
{
    //area do jogo fica 60 pixels abaixo da janela onde ficaria as informaçoes de vida, nivel e escore
    settings->gameArea = (Rectangle)
    {
        0, 60, WIDTH, HEIGHT
    };
    settings->closeGame = false;
    settings->requestcloseGame = false;
    settings->currentLevel = 1;
    settings->currentState = TITLE;
    settings->score = 0;
    settings->num_inimigos = 0;
    settings->num_inimigos_mortos = 0;
}

void BGM(Sound GameOver)
{
    if(!IsSoundPlaying(GameOver))
    {
        PlaySound(GameOver);
    }
    SetSoundVolume(GameOver, 0.5);
}


int main()
{
    SetExitKey(KEY_NULL);       // Disable KEY_ESCAPE to close window, X-button still works

    int select = 0;
    int selectextra = 0;
    int contadornivel = 0;
    char menu [TAMMENU][15] = {"Iniciar","Extra","Scoreboard", "Sair"};
    char extra [TAMMENU][15] = {"Easy","Normal","Hard", "Endless"};
    srand(time(NULL));

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
    settings.currentState = TITLE;
    settings.currentMode = NORMAL;

    //inicializa_save();
    char name[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 0;

    Rectangle textBox = { WIDTH/2.0f - 250, 400, 500, 50 };
    int framesCounter = 0;

    inicializa_jogo(&settings);
    le_nivel(&sprites, &settings, mapaJogo, &player_score, &player);
    inicializa_pos_jogador(&player, &settings, mapaJogo);
    inicializa_pos_inimigo(enemy, &settings, mapaJogo);
    inicializa_jogador(&player, &sprites, &settings);
    inicializa_inimigo(enemy, &sprites, &settings);
    inicializa_espada(&espada, &sprites);

    Sound Main = LoadSound("./Sons/maincut.mp3");
    Sound GO = LoadSound("./Sons/gameover.mp3");

    while(!settings.closeGame)
    {
        //Prog
        ProgSelector(&triangulo, &select, menu);

        if (IsKeyPressed(KEY_ESCAPE))
            settings.closeGame = true;

        BeginDrawing();

        switch(settings.currentState)
        {
        case TITLE:
        {
            StopSound(Main);
            StopSound(GO);
            //DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
            //DrawText("PRESS ENTER to JUMP to SELECTED SCREEN", 0, 0, 20, DARKGREEN);
            DrawSelector(triangulo);    //seletor
            DrawTitle();                //desenha o Titulo
            DrawMenu(menu);             //desenha as opçoes do menu

            if (IsKeyPressed(KEY_ENTER)) // Press select and press ENTER to choose screen
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

            // jogador derrotado
            if (player.vidas == 0) // pula pra tela de game over e reinicializa o nivel para seu estado inicial
            {
                player_score.score = settings.score;
                inicializa_jogo(&settings);
                le_nivel(&sprites, &settings, mapaJogo, &player_score, &player);
                inicializa_pos_jogador(&player, &settings, mapaJogo);
                inicializa_pos_inimigo(enemy, &settings, mapaJogo);
                inicializa_jogador(&player, &sprites, &settings);
                inicializa_inimigo(enemy, &sprites, &settings);
                settings.currentState = GAMEOVER;
            }

            // passar de nivel
            else if (settings.num_inimigos_mortos == settings.num_inimigos)
            {
                settings.currentLevel++;
                // Reseta num de inimigos mortos para o proximo nivel
                settings.num_inimigos_mortos = 0;
                // le o proximo nivel
                le_nivel(&sprites, &settings, mapaJogo, &player_score, &player);
                le_nivel(&sprites, &settings, mapaJogo, &player_score, &player);
                // Reseta rotaçao jogador
                player.rotacao = 'S';
                //cola o jogador na posição lida no le_nivel
                inicializa_pos_jogador(&player, &settings, mapaJogo);

                // Reseta rectangle jogador com as novas posicoes iniciais já que a rotina inicializa_jogador nao pode ser chamada pois resetaria o score e as vidas
                player.rec = (Rectangle)
                {
                x:
                    player.posXinicial,
                y:
                    player.posYinicial,
                width:
                    sprites.player_down.width,
                height:
                    sprites.player_down.height
                };

                inicializa_pos_inimigo(enemy, &settings, mapaJogo);

                inicializa_inimigo(enemy, &sprites, &settings);
                contadornivel ++;

                for (int inimigo = 0; inimigo < settings.num_inimigos; inimigo++)
                {
                    enemy[inimigo].velocidade += 0.5 * contadornivel; // A cada passagem de nivel velocidade dos inimigos e a dificuldade aumenta
                }

            }


            BGM(Main);
            // inicio de jogo
            le_nivel(&sprites, &settings, mapaJogo, &player_score, &player);
            display_jogo(&player, enemy, mapaJogo, &sprites, &settings);
            move_jogador(&player);
            move_espada(&espada, &player);
            checa_colisao_mapa(&player, mapaJogo, &settings, &sprites, &espada);
            checa_ataque(&player, &espada, &sprites);
            //para inimigos ainda vivos
            for (int inimigo = 0; inimigo < settings.num_inimigos; inimigo++)
            {
                move_inimigo(&enemy[inimigo]);
                checa_colisao_mapa(&enemy[inimigo], mapaJogo, &settings, &sprites, &espada);
            }
            checa_colisao_personagem(enemy, mapaJogo, &settings, &player);
            checa_colisao_espada(enemy, mapaJogo, &settings, &player, espada);

        }
        break;
        case EXTRA:
        {
            DrawRectangle(0, 0, WIDTH, HEIGHT, PURPLE);
            DrawText("EXTRA", 20, 20, 40, MAROON);
            DrawText("PRESS ENTER to JUMP to TITLE SCREEN", 0, 0, 20, MAROON);
            ProgSelector(&triangulo, &selectextra, extra);
            DrawSelector(triangulo);    //seletor
            DrawTitle();                //desenha o Titulo
            DrawMenu(extra);             //desenha as opçoes do menu

            if (IsKeyPressed(KEY_ENTER)) // Press select and press ENTER to choose screen
            {
                switch (selectextra)
                {
                case 0:
                    settings.currentMode = EASY;
                    inicializa_jogador(&player, &sprites, &settings);
                    break;
                case 1:
                    settings.currentMode = NORMAL;
                    inicializa_jogador(&player, &sprites, &settings);
                    break;
                case 2:
                    settings.currentMode = HARD;
                    inicializa_jogador(&player, &sprites, &settings);
                    break;
                case 3:
                    settings.currentMode = ENDLESS;
                    inicializa_jogador(&player, &sprites, &settings);
                    break;
                }
                settings.currentState = TITLE;
            }
        }
        break;
        case SCOREBOARD:
        {

            //le arquivos highscores.bin e salva em uma struct SCORE
            le_arquivo_score(highscores);
            //Desenha fundo preto
            DrawRectangle(0, 0, WIDTH, HEIGHT, BLACK);

            //DrawText("SCOREBOARD SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER to RETURN to TITLE SCREEN", 0, 0, 20, DARKBLUE);

            // Desenha o scoreboard na tela
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
            StopSound(Main);
            BGM(GO);
            //Desenha um fundo preto e texto GAME OVER (centralizado)
            DrawRectangle(0, 0, WIDTH, HEIGHT, BLACK);
            DrawText("GAME OVER", (WIDTH/2 - MeasureText("GAME OVER",100)/2), 200, 100, WHITE);
            //checa se sua pontuação foi suficiente para entrar no highscore
            if(player_score.score > highscores[4].score)
            {
                DrawText("INFORME SEU NOME", 395, 330, 40, ORANGE);
                // Pega o nome do jogador
                prog_save(&letterCount,&name,framesCounter);

                //desenha caixa de input
                DrawRectangleRec(textBox, LIGHTGRAY);
                //linhas externas da caixa de input
                DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
                //desenha o nome pegue pela função prog_save
                DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, WHITE);
                //desenha quantos caracteres foram usados e o maximo
                DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, MAX_INPUT_CHARS), 350, 470, 20, DARKGRAY);


                if (letterCount < MAX_INPUT_CHARS)
                {
                    // Draw blinking underscore char
                    if (((framesCounter/10)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, WHITE);
                }
                else DrawText("Press BACKSPACE to delete chars...", 350, 500, 20, GRAY); //quando chegar no maximo de carateres aparece esta mensagem
            }
            else  //se a pontuação foi insuficiente para entrar no highscore imprime a mensagem abaixo
            {
                DrawText("Sua pontuação foi pior ou igualou ao ultimo colocado no Highscore", (WIDTH/2 - MeasureText("Sua pontuação foi pior ou igualou ao ultimo colocado no Highscore",20)/2), 400, 20, DARKGRAY);
            }

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER))
            {

                strncpy (player_score.nome, name, MAX_INPUT_CHARS);

                atualiza_highscores(highscores, player_score);


                escreve_arquivo(highscores, "highscores.bin");

                //zera escores e nivel atual que não é zerado ao terminar o jogo completando todos o niveis
                inicializa_jogo(&settings);
                le_nivel(&sprites, &settings, mapaJogo, &player_score, &player);
                inicializa_pos_jogador(&player, &settings, mapaJogo);
                inicializa_pos_inimigo(enemy, &settings, mapaJogo);
                inicializa_jogador(&player, &sprites, &settings);
                inicializa_inimigo(enemy, &sprites, &settings);
                inicializa_espada(&espada, &sprites);
                contadornivel = 0;
            }
        }
        break;
        case SAIR:
        {
            DrawRectangle(0, 0, WIDTH, HEIGHT, BLUE);
            DrawText("FECHANDO JOGO", (WIDTH/2 - MeasureText("FECHANDO JOGO",100)/2), 100, 75, DARKBLUE);
            DrawText("PRESS ENTER RETURN to TITLE SCREEN", 0, 0, 20, DARKBLUE);

            if (WindowShouldClose() || IsKeyPressed(KEY_BACKSPACE))
                settings.requestcloseGame = true;

            if (settings.requestcloseGame)
            {
                if (IsKeyPressed(KEY_Y))
                    settings.closeGame = true;
                else if (IsKeyPressed(KEY_N))
                    settings.requestcloseGame = false;
            }

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER))
            {
                settings.currentState = TITLE;
            }
            if (settings.requestcloseGame)
            {
                DrawRectangle(0, 300, WIDTH, 200, BLACK);
                DrawText("Are you sure you want to exit program? [Y/N]", 40, 380, 30, WHITE);
            }
            else DrawText("Try to close the window or press BACKSPACE to get confirmation message!", 120, 200, 20, LIGHTGRAY);
        }
        break;
        default:
            break;
        }
        EndDrawing();

    }
    CloseAudioDevice();
    descarrega_sprites(&sprites);

    return 0;
}
