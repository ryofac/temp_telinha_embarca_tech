#include <stdio.h>
#include "pico/stdlib.h"

#include "drivers/display.h"
#include "drivers/temp.h"

void setup()
{
    stdio_init_all();
    init_temp_sensor();
    init_display();
}

int main()
{
    setup();
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
