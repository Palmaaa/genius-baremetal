#include <stdint.h>
#include "libpi.h"
#include "bcm.h"

extern void main(void);
extern uint8_t inicio_bss;
extern uint8_t fim_bss;

#define ANSWERS_SIZE 5

#define PWM 5

#define RED_LED 6
#define GREEN_LED 13
#define BLUE_LED 19
#define YELLOW_LED 26

#define RED_BTN 12
#define GREEN_BTN 16
#define BLUE_BTN 20
#define YELLOW_BTN 21

volatile uint32_t ticks;

void __attribute__((interrupt("IRQ"))) trata_irq(void) {
     // Interrupções comuns ("basic")
    int basic = IRQ_REG(pending_basic);

    if(bit_is_set(basic, 9)) {

            // Interrupções do grupo 2
        int pend = IRQ_REG(pending_2);
        if(bit_is_set(pend, 20)) {
        // IRQ 52 = interrupção GPIO
        uint32_t ev = GPIO_REG(gpeds[0]);
        if (bit_is_set(ev, RED_BTN)) {
        }
        GPIO_REG(gpeds[0]) = ev; // reconhece
        }
    }

    if (bit_is_set(basic, 0)) {
        TIMER_REG(ack) = 1;
        ticks++;
    }
}

/**
 * Inicializa o timer para gerar interrupção a cada 1 ms.
 */
void configura_timer(void) {
   TIMER_REG(load) = 1000;             // 1MHz / 1000 = 1kHz
   TIMER_REG(control) = __bit(9)       // habilita free-running counter
                      | __bit(7)       // habilita timer
                      | __bit(5)       // habilita interrupção
                      | __bit(1);      // timer de 23 bits

   IRQ_REG(enable_basic) = __bit(0);   // habilita interrupção básica 0 (timer)
}

/**
 * Monitora o contador de ticks para a passagem de x milissegundos.
 */
void wait(uint32_t x) {
   uint32_t inicio = ticks;
   while((ticks - inicio) < x) ;
}

void config_gpios() {
    gpio_init(PWM, GPIO_FUNC_OUTPUT);

    gpio_init(RED_LED, GPIO_FUNC_OUTPUT);
    gpio_init(GREEN_LED, GPIO_FUNC_OUTPUT);
    gpio_init(BLUE_LED, GPIO_FUNC_OUTPUT);
    gpio_init(YELLOW_LED, GPIO_FUNC_OUTPUT);

    gpio_init(RED_BTN, GPIO_FUNC_INPUT);
    gpio_set_pulls(RED_BTN, GPIO_PULL_UP);
}

void beep(uint32_t tone) {
    uint32_t inicio = ticks;

    while((ticks - inicio) < 500) {
        gpio_put(PWM, 1);
        wait(tone);
        gpio_put(PWM, 0);        
        wait(tone);
    }
}

uint32_t answers[ANSWERS_SIZE] = {0, 1, 2, 3, 3};
uint32_t answer_to_led[4] = {RED_LED, BLUE_LED, GREEN_LED, YELLOW_LED};
uint32_t answer_to_tone[4] = {1, 3, 6, 7};

void show_sequence(int round) {
    uart_puts("Round");
    uart_puts("\n");
    for (int i = 0; i <= round; i++) {
        int answer = answers[i];
        int led = answer_to_led[answer];
        int tone = answer_to_tone[answer];

        gpio_put(led, 1);
        beep(tone);
        gpio_put(led, 0);
    }
    wait(500);
}

void start_game(void) {
    while (1) {
        for (int round = 0; round < ANSWERS_SIZE; round++) {
            show_sequence(round);
        }
    }
}

void main(void) {
    config_gpios();

    GPIO_REG(gpren[0]) |= (1 << RED_BTN); // detectar borda de subida
    IRQ_REG(enable_2) |= (1 << 20); // habilita interrupcao GPIO (52)

    ticks = 0;
    configura_timer();
    enable_irq(1);

    start_game();

}

