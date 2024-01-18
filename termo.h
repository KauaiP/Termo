#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

static void termo(char text[10][6], char word[5], int collor[6][5])
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (text[i][j] == '\0')
            {
                // normal
                collor[i][j] = 0;
            }

            else if (text[i][j] == word[j])
            {
                // verde
                collor[i][j] = 2;
            }

            else if (text[i][j] != word[0] && text[i][j] != word[1] &&
                     text[i][j] != word[2] && text[i][j] != word[3] && text[i][j] != word[4] && text[i][j] != '\0')
            {
                // cinza chumbo
                collor[i][j] = 1;
            }
            else
            {
                // amarelo
                collor[i][j] = 3;
            }
        }
    }
}

static void rectangle_collors(int valor, int *r, int *g, int *b)
{
    switch (valor)
    {
    case (0):
        *r = 75;
        *g = 65;
        *b = 60;
        break;

    case (1):
        *r = 32;
        *g = 32;
        *b = 32;
        break;

    case (2):
        *r = 0;
        *g = 255;
        *b = 0;
        break;

    case (3):
        *r = 204;
        *g = 204;
        *b = 0;
        break;

    default:
        break;
    }
}

bool verifica_vitoria(int matriz[6][5], int line)
{
    int cont = 0;
    for (int i = 0; i < 5; i++)
    {
        if (matriz[line][i] == 2)
        {
            cont += 1;
        }
    }
    if (cont == 5)
    {
        return true;
    }
    else
    {
        return false;
    }

    cont = 0;
}
