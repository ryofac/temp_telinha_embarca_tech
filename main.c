#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "ssd1306.h"

ssd1306_t display;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SDA_PIN 14
#define SCL_PIN 15
#define DISPLAY_ADDRESS 0x3c

void setup()
{
    stdio_init_all();

    // Initialize I2C
    uint freq = 400 * 1000; // 400 Khz
    i2c_init(i2c1, freq);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Initialize I2C
    ssd1306_init(&display, SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_ADDRESS, i2c1);

    // Initialize ADC
    adc_init();
}

float read_temperature()
{
    adc_select_input(4);
    float conversion_factor = 3.3f / (1 << 12); // 12-bit ADC
    uint16_t raw = adc_read();
    float voltage = raw * conversion_factor;
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;
    float temperature_converted = (temperature - 32) * 5.0f / 9.0f; // Convert to Celsius
    return temperature_converted;
}

int main()
{

    setup();

    while (true)
    {
        ssd1306_clear(&display);
        char msg[20];
        float temperature = read_temperature();
        snprintf(msg, sizeof(msg), "Temp: %.2f C", temperature);
        ssd1306_draw_string(&display, 0, 0, 2, msg);
        ssd1306_show(&display);
        sleep_ms(500);
    }
}
