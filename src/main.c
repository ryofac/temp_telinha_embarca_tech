/**
 * @file main.c
 * @brief Exibe a temperatura interna do RP2040 em um display, com efeitos visuais em matriz de LEDs WS2812B.
 *
 * Este projeto demonstra como ler a temperatura interna do microcontrolador e exibir no display.
 * Também inicializa uma matriz de LEDs e desenha um quadrado visual usando LEDs verdes.
 *
 * @author Luís Felipe Patrocínio, Ruan Macedo e Ryan Faustino
 * @date 2025-05-06
 */

#include <stdio.h>
#include "pico/stdlib.h"

#include "drivers/display.h"
#include "drivers/temp.h"

#include "ws2812b_animation.h"

/**
 * @brief Inicializa a matriz de LEDs WS2812B com configurações padrão.
 */
void init_led_matrix()
{
    ws2812b_init(pio0, 7, 25);
    ws2812b_set_global_dimming(7);
    ws2812b_fill_all(GRB_BLACK);
}

/**
 * @brief Desenha um quadrado verde na matriz de LEDs.
 */
void draw_square()
{
    ws2812b_fill(6, 8, GRB_GREEN);
    ws2812b_fill(11, 13, GRB_GREEN);
    ws2812b_fill(16, 18, GRB_GREEN);
}

/**
 * @brief Atualiza a matriz de LEDs, aplicando as mudanças visuais.
 */
void update_led_matrix()
{
    ws2812b_render();
}

/**
 * @brief Função de configuração principal do sistema.
 *
 * Inicializa a comunicação padrão, sensores e periféricos como display e matriz de LEDs.
 */
void setup()
{
    stdio_init_all();
    init_temp_sensor();
    init_display();
    init_led_matrix();
}

/**
 * @brief Função principal do programa.
 *
 * Exibe continuamente a temperatura interna no display e alterna mensagens.
 * Também realiza a renderização inicial do quadrado na matriz de LEDs.
 *
 * @return int Nunca retorna, pois o loop é infinito.
 */
int main()
{
    setup();
    draw_square();
    update_led_matrix();

    while (true)
    {
        clear_display(true);
        char msg[20];
        float temperature = read_internal_temperature();
        snprintf(msg, sizeof(msg), "Temp: %.2f C", temperature);

        show(msg, false);
        show("Refatorando...", true);
        sleep_ms(500);
    }
}
