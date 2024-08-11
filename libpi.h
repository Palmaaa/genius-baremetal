
#define GPIO_FUNC_INPUT       0
#define GPIO_FUNC_OUTPUT      1
#define GPIO_FUNC_ALT5        2
#define GPIO_FUNC_ALT4        3
#define GPIO_FUNC_ALT0        4
#define GPIO_FUNC_ALT1        5
#define GPIO_FUNC_ALT2        6
#define GPIO_FUNC_ALT3        7

#define GPIO_PULL_NONE        0
#define GPIO_PULL_DOWN        1
#define GPIO_PULL_UP          2

#define ARM_MODE_USER         0b10000
#define ARM_MODE_FIQ          0b10001
#define ARM_MODE_IRQ          0b10010
#define ARM_MODE_SVR          0b10011
#define ARM_MODE_MONITOR      0b10110
#define ARM_MODE_ABORT        0b10111
#define ARM_MODE_UNDEF        0b11011
#define ARM_MODE_HYPER        0b11010
#define ARM_MODE_SYSTEM       0b11111

/***
 * delay 
 * -- parâmtros: r0 (tempo)
 * -- retorno: -
 * Espera r0 loops vazios
 ***/
void delay(unsigned);

/***
 * get_arm_mode 
 * -- parâmtros: -
 * -- retorno: r0 (modo atual ARM_MODE_....))
 * Lê o modo do processador no registrador cpsr
 ***/
unsigned get_arm_mode(void);

/***
 * set_arm_mode 
 * -- parâmtros: r0 (modo desejado)
 * -- retorno -
 * Muda o modo do processador (ARM_MODE...)
 ***/
void set_arm_mode(unsigned);

/***
 * get_core 
 * -- parâmtros: -
 * -- retorno r0 (Índice do núcleo atual (afinidade))
 * Identifica o núcleo (0 a 3) que está executando.
 ***/
unsigned get_core(void);

/***
 * enable_irq 
 * -- parâmtros: r0 (booleano habilita ou desabilita)
 * -- retorno -
 * Habilita ou desabilita as interrupções normais (flag I).
 ***/
void enable_irq(unsigned);

/***
 * enable_fiq 
 * -- parâmtros: r0 (booleano habilita ou desabilita)
 * -- retorno -
 * Habilita ou desabilita as interrupções rápidas.
 ***/
void enable_fiq(unsigned);

/***
 * gpio_init 
 * -- parâmtros: r0 (índice do GPIO, de 0 a 53)
 * --            r1 (função desejada GPIO_FUNC_...)
 * -- retorno -
 * Configura um GPIO antes do uso.
 ***/
void gpio_init(unsigned, unsigned);

/*** 
 * gpio_put 
 * -- parâmtros: r0 (índice do GPIO, de 0 a 53)
 * --            r1 (valor a escrever, 0 ou 1)
 * -- retorno -
 * Altera o valor de um GPIO configurado como saída
 ***/
void gpio_put(unsigned, unsigned);

/***
 * gpio_get 
 * -- parâmtros: r0 (índice do GPIO, de 0 a 53)
 * -- retorno r0 (valor atual do GPIO)
 * Lê o estado atual de um GPIO
 ***/
unsigned gpio_get(unsigned);

/***
 * gpio_toggle 
 * -- parâmtros: r0 (índice do GPIO, de 0 a 53)
 * -- retorno -
 * Alterna o estado de um GPIO configurado como saída.
 ***/
void gpio_toggle(unsigned);

/***
 * gpio_set_pulls 
 * -- parâmtros: r0 (índice do GPIO, de 0 a 53)
 * --            r1 (configuração de pull-up (GPIO_PULL_...))
 * -- retorno -
 * Configura os resistores internos conectados a um GPIO.
 ***/
void gpio_set_pulls(unsigned, unsigned);

/***
 * uart_init 
 * -- parâmtros: r0 (baudrate (por exemplo, 9600))
 * -- retorno -
 * Configura a uart antes de sua utilização.
 ***/
void uart_init(unsigned);

/***
 * uart_putc 
 * -- parâmtros: r0 (caractere a enviar)
 * -- retorno -
 * Envia um byte através da uart
 ***/
void uart_putc(char);

/***
 * uart_puts 
 * -- parâmtros: r0 (ponteiro para um string C)
 * -- retorno -
 * Envia um string terminado por zero através da uart.
 ***/
void uart_puts(char*);

/*** 
 * uart_getc 
 * -- parâmtros: -
 * -- retorno Valor do caractere recebido
 * Lê um byte através da uart.
 ***/
char uart_getc(void);

