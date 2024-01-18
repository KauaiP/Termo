#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_video.h>
#include <allegro5/keyboard.h>
#include "termo.h"

#define MAX_LEN 6 // 5 letras + 1 para o caractere nulo '\0'

char *obterPalavraAleatoria(char *file_name)
{
    FILE *arquivo = fopen(file_name, "r");

    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int totalPalavras;
    fscanf(arquivo, "%d", &totalPalavras);
    fgetc(arquivo); // Consumir o caractere de nova linha

    // Alocar memória para armazenar as palavras
    char **palavras = (char **)malloc(totalPalavras * sizeof(char *));

    if (palavras == NULL)
    {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < totalPalavras; ++i)
    {
        palavras[i] = (char *)malloc(MAX_LEN * sizeof(char));

        if (palavras[i] == NULL)
        {
            perror("Erro ao alocar memória");
            exit(EXIT_FAILURE);
        }

        fgets(palavras[i], MAX_LEN, arquivo);
    }

    fclose(arquivo);

    char *palavraEscolhida = (char *)malloc(MAX_LEN * sizeof(char));

    if (palavraEscolhida == NULL)
    {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    do
    {
        int indiceAleatorio = rand() % totalPalavras;
        // Copiar a palavra escolhida para a memória alocada
        strcpy(palavraEscolhida, palavras[indiceAleatorio]);
    } while (palavraEscolhida[0] == '\n');

    // Liberar a memória alocada para as palavras
    for (int i = 0; i < totalPalavras; ++i)
    {
        free(palavras[i]);
    }
    free(palavras);

    return palavraEscolhida;
}

void zera_elementos(int matriz[6][5])
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            matriz[i][j] = 0;
        }
    }
}

void zera_caracter(char matriz[10][6])
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            matriz[i][j] = '\0';
        }
    }
}

int main()
{
    // iniciando recursos da allegro que serão implementados
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    // display
    ALLEGRO_DISPLAY *display = al_create_display(640, 480);
    al_set_window_position(display, 200, 200);

    // escrita
    ALLEGRO_FONT *titulo = al_load_font("./font.ttf", 25, 0);
    ALLEGRO_FONT *escrita = al_load_font("./font.ttf", 20, 0);
    ALLEGRO_FONT *teclado = al_load_font("./font.ttf", 15, 0);

    // timer
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);

    // fila de eventos
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    // vaiáveis necessárias para a escrita do usuário
    char text[10][6];
    char *word = obterPalavraAleatoria("palavras.csv");
    int collor[6][5];
    int line = 0;

    bool result; // variável que controla o resultado do jogo

    // iniciando as duas matrizes com todos os elementos nulos
    zera_elementos(collor);
    zera_caracter(text);

    //---------------------------------execução da janela----------------------------------------

    while (true)
    {
        // declarando a fila de eventos
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        // fecha a janela se o usuário clicar no x
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        // caso contrário apresenta a janela
        else if (event.type == ALLEGRO_EVENT_TIMER)
        {
            al_clear_to_color(al_map_rgb(135, 100, 100));                         // cor do plano de fundo
            al_draw_text(titulo, al_map_rgb(255, 255, 255), 260, 20, 0, "Termo"); // escrita do titulo e sua cor

            // mostrando quadradinhos
            float x1 = 225.0, y1 = 100.0, x2 = 255.0, y2 = 130.0;
            int r = 75, g = 65, b = 60;

            for (int i = 0; i < 6; i++)
            {
                x1 = 225.0;
                x2 = 255.0;
                for (int j = 0; j < 5; j++)
                {
                    rectangle_collors(collor[i][j], &r, &g, &b); // função que define cor
                    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(r, g, b));
                    x1 += 33;
                    x2 += 33;
                }
                y1 += 33;
                y2 += 33;
            }

            // mostrando resultado do jogo
            if (result == true)
            {
                al_draw_text(escrita, al_map_rgb(0, 255, 0), 260, 300, 0, "acertou!");
                al_draw_text(escrita, al_map_rgb(255, 255, 255), 215, 330, 0, "Palavra correta:");
                al_draw_textf(escrita, al_map_rgb(255, 255, 255), 375, 330, ALLEGRO_ALIGN_LEFT, "%s", word);
            }
            else if (result == false && line == 6)
            {
                al_draw_text(escrita, al_map_rgb(255, 0, 0), 210, 300, 0, "esgotou as tentativas!");
                al_draw_text(escrita, al_map_rgb(255, 255, 255), 210, 330, 0, "Palavra correta:");
                al_draw_textf(escrita, al_map_rgb(255, 255, 255), 380, 330, ALLEGRO_ALIGN_LEFT, "%s", word);
            }

            // mostrando a entrada de texto do jogador
            for (int i = 0; i <= line; i++)
            {
                float x = 230;
                for (int j = 0; j < strlen(text[i]); j++)
                {
                    al_draw_textf(escrita, al_map_rgb(255, 255, 255), x, 100 + i * 33, ALLEGRO_ALIGN_LEFT, "%c", text[i][j]);
                    x += 33;
                }
            }

            al_flip_display();
        }

        // se o jogador digitar alguma coisa
        else if (event.type == ALLEGRO_EVENT_KEY_CHAR && line < 10)
        {
            int keycode = event.keyboard.keycode;

            if (keycode >= ALLEGRO_KEY_A && keycode <= ALLEGRO_KEY_Z && strlen(text[line]) < 5)
            {
                char buf[2] = {event.keyboard.unichar, '\0'};
                strcat(text[line], buf); // adciona o caracter na linha atual
            }
            else if (keycode == ALLEGRO_KEY_ENTER && strlen(text[line]) == 5)
            {
                termo(text, word, collor);
                result = verifica_vitoria(collor, line);
                line++;
            }
            else if (keycode == ALLEGRO_KEY_BACKSPACE && strlen(text[line]) > 0)
            {
                text[line][strlen(text[line]) - 1] = '\0';
            }
        }
    }

    // destruindo os recursos utilizados
    al_destroy_font(titulo);
    al_destroy_font(escrita);
    al_destroy_font(teclado);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}