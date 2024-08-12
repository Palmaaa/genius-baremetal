#include <stdint.h>
#include "libpi.h"
#include "bcm.h"

extern void main(void);
extern uint8_t inicio_bss;
extern uint8_t fim_bss;

#define ANSWERS_SIZE 20

#define PWM 5

#define RED_LED 6
#define GREEN_LED 13
#define BLUE_LED 19
#define YELLOW_LED 26

#define RED_BTN 12
#define GREEN_BTN 16
#define BLUE_BTN 20
#define YELLOW_BTN 21

#define START_BTN 9

volatile uint32_t ticks;

void __attribute__((interrupt("IRQ"))) trata_irq(void) {
    int basic = IRQ_REG(pending_basic);

    if (bit_is_set(basic, 0)) {
        TIMER_REG(ack) = 1;
        ticks++;
    }
}

void start_game(void);

/**
 * Inicializa o timer para gerar interrupção a cada 1 ms.
 */
void config_timer(void) {
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
    gpio_init(BLUE_BTN, GPIO_FUNC_INPUT);
    gpio_set_pulls(BLUE_BTN, GPIO_PULL_UP);
    gpio_init(GREEN_BTN, GPIO_FUNC_INPUT);
    gpio_set_pulls(GREEN_BTN, GPIO_PULL_UP);
    gpio_init(YELLOW_BTN, GPIO_FUNC_INPUT);
    gpio_set_pulls(YELLOW_BTN, GPIO_PULL_UP);

    gpio_init(START_BTN, GPIO_FUNC_INPUT);
    gpio_set_pulls(START_BTN, GPIO_PULL_UP);
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

uint32_t answers[ANSWERS_SIZE];
uint32_t answer_to_led[4] = {RED_LED, GREEN_LED, BLUE_LED, YELLOW_LED};
uint32_t answer_to_tone[4] = {1, 3, 6, 7};

void show_button_with_tone(uint32_t button) {
    uart_puts("Exibindo botao");
    int led = answer_to_led[button];
    int tone = answer_to_tone[button];
    gpio_put(led, 1);
    beep(tone);
    gpio_put(led, 0);
}

void show_sequence(int round) {
    uart_puts("Round");
    for (int i = 0; i <= round; i++) {
        uint32_t answer = answers[i];
        show_button_with_tone(answer);
    }
    wait(500);
}

uint32_t read_buttons(void) {
    int red_btn = gpio_get(RED_BTN);
    int blue_btn = gpio_get(BLUE_BTN);
    int green_btn = gpio_get(GREEN_BTN);
    int yellow_btn = gpio_get(YELLOW_BTN);
    if (!red_btn) {
        uart_puts("Botao vermelho apertado");
        return 0;
    }
    if (!green_btn) {
        uart_puts("Botao azul apertado");
        return 1;
    }
    if (!blue_btn) {
        uart_puts("Botao verde apertado");
        return 2;
    }
    if (!yellow_btn) {
        uart_puts("Botao amarelo apertado");
        return 3;
    }

    return -1;
}

/**
 * Produz 3 beeps para informar que o jogador errou
 * Reinicia o jogo posteriormente
 */
void handle_wrong_play() {
    wait(500);
    beep(9);
    wait(250);
    beep(9);
    wait(250);
    beep(9);
    
    start_game();
}

/**
 * Gera uma resposta pseudoaleatoria entre 0 e 3 com base em um seed
 */
int next;
int generate_answer(void) {
    next =  next * 1103515245 + 12345;
    return ((next/65536) % 32768) % 4;
}

/**
 * Aguarda o inicio do jogo lendo o botao de start
 */
void read_start(void) {
    while(gpio_get(START_BTN)) {
        uart_puts("Esperando inicio");
    }
}

/**
 * Da inicio ao jogo gerando as respostas aleatorias e iterando os rounds
 */
void start_game(void) {
    // Usa a contagem de ticks como seed para geracao de pseudoaleatorios
    next = ticks;
    for (int i = 0; i < ANSWERS_SIZE; i++) {
        answers[i] = generate_answer();
    }

    // Inicia a iteracao dos rounds
    for (int round = 0; round < ANSWERS_SIZE; round++) {
        show_sequence(round);

        uint32_t round_play = 0;

        uint32_t inicio = ticks;

        // Aguarda deteccao de jogada ou timeout
        while (1) {
            if (gpio_get(START_BTN)) main();

            uint32_t timeout = (ticks - inicio) > 10000;
            if (timeout) handle_wrong_play();

            uint32_t pressed_button = read_buttons();

            if (pressed_button == -1) continue;

            show_button_with_tone(pressed_button);

            uint32_t correct = pressed_button == answers[round_play];
            if (correct) {
                round_play++;
                wait(250);
            } else {
                handle_wrong_play();
                break;
            }
            
            if (round_play > round) {
                break;
            }
        }
    }
}

void main(void) {
    config_gpios();

    ticks = 0;
    config_timer();
    enable_irq(1);

    read_start();
    wait(250);
    start_game();
}

