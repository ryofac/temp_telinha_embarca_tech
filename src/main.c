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
#define DISPLAY_FREQUENCY 400000 // 400 kHz
#define DISPLAY_ADDRESS 0x3c

#define ADC_TEMP_CHANNEL 4
#define ADC_VREF 3.3f
#define ADC_RESOLUTION 4096.0f // 12-bit resolution

void i2c_setup()
{

    i2c_init(i2c1, DISPLAY_FREQUENCY);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

void init_display()
{
    i2c_setup();
    ssd1306_init(&display, SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_ADDRESS, i2c1);
}

float voltage_to_temperature(float voltage)
{
    // acccording to the datasheet, the temperature is calculated as:
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;
    return temperature;
}

float convert_to_celsius(float temperature_fahrenheit)
{
    return (temperature_fahrenheit - 32) * 5.0f / 9.0f;
}

float read_temperature()
{
    adc_select_input(ADC_TEMP_CHANNEL);
    float conversion_factor = ADC_VREF / ADC_RESOLUTION;
    uint16_t raw = adc_read();

    float voltage = raw * conversion_factor;
    float temperature = voltage_to_temperature(voltage);
    float temperature_converted = convert_to_celsius(temperature);
    return temperature_converted;
}

void setup()
{
    stdio_init_all();
    init_display();
    adc_init();
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
