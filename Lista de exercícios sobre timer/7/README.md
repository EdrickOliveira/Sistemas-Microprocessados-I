# Animação de LED com Deslocamento Bidirecional

Este projeto implementa uma animação com 8 LEDs conectados aos pinos **PC0 a PC7** do STM32F4, onde um único LED se desloca da esquerda para a direita e, ao chegar na extremidade, inverte o sentido e se desloca de volta para a esquerda, repetindo o ciclo continuamente. O controle da temporização é feito utilizando o **Timer 11** (TIM11).

## Funcionamento Geral

- Inicialmente, apenas o LED **PC0** está aceso.
- A cada **250 milissegundos**, o LED aceso "anda" para a esquerda (**PC0 até PC7**), um passo por vez.
- Ao alcançar a extremidade (**PC7**), o LED inverte o sentido e começa a se deslocar para a direita (**PC7 até PC0**).
- Esse ciclo se repete indefinidamente, criando um efeito de "ping-pong" com o LED.

## Timer Utilizado

### Timer 11 (TIM11)

- Período: 250 ms
- Configuração:
  - `ARR = 1999`
  - `PSC = 1999`
- O tempo é controlado por verificação da flag `UIF` (Update Interrupt Flag), sem uso de interrupções.
- Cada vez que a flag é setada, um passo da animação é executado.

## Configuração de GPIO

- **GPIOA**: Parcialmente reservado para uso do programador/debugger.
- **GPIOC (PC0 a PC7)**: Saídas digitais conectadas aos LEDs.
  - Todos os pinos são configurados como **saídas**.
  - O registrador `GPIOC->ODR` é utilizado para definir qual LED está aceso.

## Lógica de Animação

### Conceito de Sentido

Um `enum` chamado `sentido_t` define o sentido de deslocamento:

```c
typedef enum{
    DIREITA=0, ESQUERDA
}sentido_t;
```

A variável `sentido` controla se o LED se move para a **direita** (bit shift para a direita) ou para a **esquerda** (bit shift para a esquerda).

### Estrutura do Loop Principal

O loop principal tem duas partes:

1. **Deslocamento da esquerda para a direita (PC0 → PC7)**
   - A variável `fim_linha` começa em `0x02` e vai até `0x80`.
   - A cada 250ms, o LED aceso é deslocado para a esquerda.
   - Quando o LED atinge o valor de `fim_linha`, o sentido é invertido.

2. **Deslocamento da direita para a esquerda (PC7 → PC0)**
   - `fim_linha` agora começa em `0x80` e vai até `0x02`.
   - O LED é deslocado para a direita.
   - Ao atingir o valor mínimo (`0x01`), o sentido é novamente invertido.

### Função `deslocar_led(sentido_t sentido)`

Responsável por mover o LED aceso conforme o sentido indicado:

```c
void deslocar_led(sentido_t sentido){
    switch(sentido){
        case ESQUERDA:
            GPIOC->ODR = GPIOC->ODR << 1;
            break;
        case DIREITA:
            GPIOC->ODR = GPIOC->ODR >> 1;
            break;
    }
}
```

### Exemplo Visual

```
Animação: PC0 → PC1 → ... → PC7 → PC6 → ... → PC0 → ...
```

## Funções Principais

- `main()`: Gerencia o loop de animação, alternando o sentido e controlando os limites do deslocamento.
- `inicializar()`: Configura o Timer 11 e os GPIOs.
- `deslocar_led(sentido_t sentido)`: Move o LED aceso para a esquerda ou direita usando operações de bit shift.

## Observações

- A lógica é simples, baseada em deslocamento de bits (bit shift) sobre o registrador `ODR`.
- O controle é feito via polling (verificação direta da flag `UIF`).
- O projeto é excelente como introdução à manipulação de bits, controle de tempo com timers e uso de enums para controle de estados.

```c
GPIOC->ODR = GPIOC->ODR << 1;  // Move LED aceso para a esquerda
GPIOC->ODR = GPIOC->ODR >> 1;  // Move LED aceso para a direita
```

Essas duas instruções, combinadas com a troca de sentido, são a base de toda a animação.
