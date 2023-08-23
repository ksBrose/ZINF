typedef enum GameScreen
{
    TITLE,
    GAMEPLAY,
    EXTRA,
    SCOREBOARD,
    SAIR,
    GAMEOVER
} GameScreen;

typedef enum GameMode
{
    EASY,
    NORMAL,
    HARD,
    ENDLESS
} GameMode;

typedef struct informacoes
{
    GameScreen currentState;
    GameMode currentMode;
    Rectangle gameArea;
    bool closeGame;
    bool requestcloseGame;
    int currentLevel;
    int score;
    int num_inimigos;
    int num_inimigos_mortos;
} Infos;
