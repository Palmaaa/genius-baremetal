#include <stdio.h>
#include <stdlib.h>

#define ANSWERS_SIZE 100

int answers[ANSWERS_SIZE];

void config_gpios();
int read_start_gpio();
void start_game();
void show_sequence(int round);
int generate_answer();
void led_on(int answer);
void beep(int answer);
void led_off(int answer);
void timer_init();
int read_timer_reg();
int read_buttons();
void handle_wrong_play();

void config_gpios() {
    // Implementar configuração dos GPIOs
    // 6 botoes entrada
    // 4 leds saida
    // 1 buzzer saida pwm 
}

int read_start_gpio() {
    // Implementar leitura do GPIO de start
    return 0;
}

int main() {
    config_gpios();

    while (1) {
        if (read_start_gpio()) {
            start_game();
        }
    }

    return 0;
}

void show_sequence(int round) {
    for (int i = 0; i <= round; i++) {
        int answer = answers[i];
        led_on(answer);
        beep(answer);
        led_off(answer);
    }
}

void show_button_press(int button) {
    led_on(button);
    beep(button);
    led_off(button);
}

void start_game() {
    // Inicializa o vetor com a sequencia de respostas
    for (int i = 0; i < ANSWERS_SIZE; i++) {
        answers[i] = generate_answer();
    }

    // Inicia iteracao dos rounds
    for (int round = 0; round < ANSWERS_SIZE; round++) {
        show_sequence(round);

        int round_play = 0;

        timer_init();

        while (1) {
            int timeout = read_timer_reg();
            if (!timeout) {
                // Handle timeout
                break;
            }

            // Se acertar vai para proxima jogada do round
            int pressed_button = read_buttons();
            show_button_press(pressed_button);
            int correct = pressed_button == answers[round_play];
            if (correct) {
                round_play++;
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

// Funções não implementadas
int generate_answer() {
    return rand() % 4; // Exemplo: gera uma resposta aleatória entre 0 e 3
}

void led_on(int answer) {
    // Implementar acender LED correspondente
}

void beep(int answer) {
    // Implementar beep correspondente
}

void led_off(int answer) {
    // Implementar apagar LED correspondente
}

void timer_init() {
    // Implementar inicialização do timer
}

int read_timer_reg() {
    // Implementar leitura do registrador do timer
    return 0;
}

int read_buttons() {
    // Implementar leitura dos botões
    return 0;
}

void handle_wrong_play() {
    // Implementar tratamento de jogada errada
}
