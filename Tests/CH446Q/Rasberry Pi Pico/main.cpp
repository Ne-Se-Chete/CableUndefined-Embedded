#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define ADDR0 9
#define ADDR1 10  
#define ADDR2 11  
#define ADDR3 12 

#define AY0 4
#define AY1 5
#define AY2 6

#define AX0 0
#define AX1 1
#define AX2 2
#define AX3 3

#define STB 8
#define DAT 7

#define MAX_ADDRESS 15
#define MAX_AX 15
#define MAX_AY 7

int setConnection(uint8_t addr, uint8_t AX, uint8_t AY, bool mode) {
    if (addr > MAX_ADDRESS || AX > MAX_AX || AX > MAX_AY) {
        return 1;
    }

    gpio_put(ADDR0, (addr & 0b0001) );
    gpio_put(ADDR1, (addr & 0b0010) );
    gpio_put(ADDR2, (addr & 0b0100) );
    gpio_put(ADDR3, (addr & 0b1000) );

    gpio_put(AX0, (AX & 0b0001) );
    gpio_put(AX1, (AX & 0b0010) );
    gpio_put(AX2, (AX & 0b0100) );
    gpio_put(AX3, (AX & 0b1000) );

    gpio_put(AY0, (AY & 0b0001) );
    gpio_put(AY1, (AY & 0b0010) );
    gpio_put(AY2, (AY & 0b0100) );

    gpio_put(DAT, mode);

    gpio_put(STB, 1);
    sleep_us(2);
    gpio_put(STB, 0);

    return 0;
}

int main() {
    stdio_init_all();

    gpio_init(ADDR0);
    gpio_init(ADDR1);
    gpio_init(ADDR2);
    gpio_init(ADDR3);

    gpio_init(AX0);
    gpio_init(AX1);
    gpio_init(AX2);
    gpio_init(AX3);

    gpio_init(AY0);
    gpio_init(AY1);
    gpio_init(AY2);

    gpio_init(DAT);
    gpio_init(STB);

    gpio_put(ADDR0, 0);
    gpio_put(ADDR1, 0);
    gpio_put(ADDR2, 0);
    gpio_put(ADDR3, 0);

    gpio_put(AX0, 0);
    gpio_put(AX1, 0);
    gpio_put(AX2, 0);
    gpio_put(AX3, 0);

    gpio_put(AY0, 0);
    gpio_put(AY1, 0);
    gpio_put(AY2, 0);

    gpio_put(DAT, 0);
    gpio_put(STB, 0);


    gpio_set_dir(ADDR0, GPIO_OUT);
    gpio_set_dir(ADDR1, GPIO_OUT);
    gpio_set_dir(ADDR2, GPIO_OUT);
    gpio_set_dir(ADDR3, GPIO_OUT);

    gpio_set_dir(AX0, GPIO_OUT);
    gpio_set_dir(AX1, GPIO_OUT);
    gpio_set_dir(AX2, GPIO_OUT);
    gpio_set_dir(AX3, GPIO_OUT);

    gpio_set_dir(AY0, GPIO_OUT);
    gpio_set_dir(AY1, GPIO_OUT);
    gpio_set_dir(AY2, GPIO_OUT);

    gpio_set_dir(DAT, GPIO_OUT);
    gpio_set_dir(STB, GPIO_OUT);

    setConnection(0b1111, 1, 1, true);

    while (1) {
        // Your main loop code goes here
    }

    return 0;
}
