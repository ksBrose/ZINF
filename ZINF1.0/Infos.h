typedef enum GameScreen
{
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    EXTRA,
    SCOREBOARD,
    SAIR,
    GAMEOVER
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
    int num_inimigos;
    int num_inimigos_mortos;
} Infos;
