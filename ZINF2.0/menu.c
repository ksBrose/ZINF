#include "menu.h"

Triangle SetupTriangle()
{
    return (Triangle)
    {
origem:
        (Vector2) {x: 300, y: 300},
sides:
        3,
radius:
        30,
r:
        0,
cor:
        (Color)
        {
            r: 200,
            g: 200,
            b: 0,
            a: 255
        }
    };
}

/*Dado um triangulo -o icone de seleção do menu- o desenha na tela.*/
void DrawSelector(Triangle T)
{
    DrawPoly(T.origem, T.sides, T.radius, T.r, T.cor);
}

/*Desenha o título na tela.*/
void DrawTitle()
{
    ClearBackground(BLACK);
    DrawText("ZINF", (WIDTH - MeasureText("ZINF",Tfonte))/2, 100, Tfonte, RAYWHITE);
}

/*Dado um array com as opções do menu, o desenha na tela.*/
void DrawMenu(char menu [TAMMENU][15])
{
    //desenha o vetor texto menu
    //centralizando cada palavra do seu tamanho
    //e espaçando Fonte + Espaçamento da posição inicial
    int i;
    for(int i = 0; i < TAMMENU ; i++)
    {
        DrawText(menu[i], (WIDTH/2 - MeasureText(menu[i],Mfonte)/2), Yi + i * (Mfonte + E), Mfonte, LIGHTGRAY);
    }
}

/*Dado um triângulo, um ponteiro que indica a opção atual, e as opções do menu verifica qual opção foi selecionada.*/
void ProgSelector(Triangle *T,int *select, char menu [][15])
{
    T->r += VelRot/FPS;
    if(IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_J))
    {
        T->cor = (Color)
        {
            r: GetRandomValue(0, 255),
            g: GetRandomValue(0, 255),
            b: GetRandomValue(0, 255),
            a: 255
        };
    }
    //apertar W sobe seletor 1 posição no menu e S baixa 1 posição
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
        break;
    }
}
