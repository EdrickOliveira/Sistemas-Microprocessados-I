# Efeito Ping-Pong com LEDs (Deslocamento Bidirecional)

Este projeto implementa o clássico efeito **"ping-pong"** com LEDs, onde um único LED aceso se desloca para a direita e para a esquerda entre os extremos. O movimento é controlado por temporização usando o **Timer 11** do STM32F4.

## Funcionamento Geral

- LEDs conectados aos pinos **PC0 a PC7** (8 bits).
- Apenas **um LED aceso por vez**.
- O LED se desloca em uma direção (direita ou esquerda) a cada **0,5 segundos**.
- Quando o LED atinge uma das extremidades (PC0 ou PC7), o sentido é invertido automaticamente.

## Timer Utilizado

### Timer 11 (TIM11)

- Período: 0,5 segundos
- Configuração:
  - `ARR = 1999`
  - `PSC = 3999`
- A flag `UIF` é verificada no `main()` para controlar o tempo de atualização do LED.

## Configuração de GPIO

- **GPIOA**: Usado parcialmente para comunicação com o programador/debugger.
- **GPIOC (PC0 a PC7)**: Saídas digitais conectadas aos LEDs.
  - Todos os pinos são configurados como saída (`MODER`).
  - O estado inicial é `PC0` aceso (`GPIOC->ODR = 1`).

## Enumeração de Sentido

```c
typedef enum {
    DIREITA = 0,
    ESQUERDA
} sentido_t;
```

Define o sentido atual de deslocamento dos LEDs.

## Lógica de Deslocamento

- A cada overflow do Timer 11:
  - Se o LED aceso estiver em **PC0 (bit 0)** ou **PC7 (bit 7)**, o sentido de deslocamento é invertido.
  - O LED é deslocado uma posição à direita ou à esquerda, conforme o sentido atual.

### Exemplo de sequência (8 ciclos):

```
PC0 → PC1 → PC2 → PC3 → PC4 → PC5 → PC6 → PC7 → PC6 → ... (vai e volta)
```

## Funções Principais

- `main()`: Contém o laço principal. Verifica a flag do Timer 11 e chama a função de deslocamento.
- `inicializar()`: Configura os GPIOs e inicia o Timer 11.
- `deslocar_led(sentido_t sentido)`: Realiza o deslocamento do bit aceso para a esquerda ou direita, conforme o valor da enumeração.

## Observações

- O projeto utiliza apenas verificação de flag (`polling`) para controlar o tempo, sem uso de interrupções.
- A troca de sentido é feita com XOR:

```c
sentido ^= 1;
```

- Isso alterna entre `DIREITA` (0) e `ESQUERDA` (1) de forma simples e eficiente.

## Aplicações

- Efeitos visuais com LEDs.
- Sinalização sequencial.
- Exercício de manipulação de bits e controle por temporizador.

```c
GPIOC->ODR = GPIOC->ODR << 1;  // ESQUERDA
GPIOC->ODR = GPIOC->ODR >> 1;  // DIREITA
```

Essas operações manipulam diretamente o registrador de saída, movendo o LED aceso entre os pinos.
