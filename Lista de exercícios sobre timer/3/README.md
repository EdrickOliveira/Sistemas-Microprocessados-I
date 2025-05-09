# Controle de Pisca-LED com Duração Variável via Entradas Digitais

Este projeto implementa um sistema de controle de LED (PA8) com **duração de pisca programável**. A duração é definida dinamicamente por três pinos de entrada digital (PB3, PB4 e PB5), que representam um valor binário de 3 bits.

## Funcionamento Geral

- Os pinos **PB3, PB4 e PB5** são usados para formar um valor binário entre **0 e 7**.
- Esse valor define por quanto tempo o LED irá piscar (em número de transições).
- O LED (PA8) é ligado e pisca por até **10 ciclos de 100 ms**, controlados por **Timer 11**.
- A cada overflow do Timer 11 (aproximadamente 100 ms), o LED inverte seu estado.
- Após 10 ciclos, o loop reinicia e lê novamente o valor nas entradas PB3–PB5.

## Configuração de GPIO

- **GPIOA (PA8)**: Saída digital conectada ao LED.
- **GPIOA (PA13, PA14)**: Pinos configurados para uso com programador/debugger.
- **GPIOB (PB3, PB4, PB5)**: Entradas digitais com resistores de pull-down, representam um valor binário de 3 bits (0 a 7).

## Timer Utilizado

### Timer 11 (TIM11)

- Frequência de overflow: 10 Hz (período de 100 ms)
- Configuração:
  - `PSC = 1599`
  - `ARR = 999`
- A cada overflow (flag UIF ativada), o LED pode inverter seu estado dependendo do valor de entrada lido.

## Lógica de Controle

1. O valor em PB3–PB5 é lido e convertido para um número (`codigo`) entre 0 e 7.
2. O LED é ligado (PA8 = 1).
3. Um laço é executado por 10 ciclos de temporizador:
   - A cada estouro de `TIM11`, a flag `UIF` é verificada.
   - O LED inverte seu estado **apenas se `i <= codigo*2`** (ou seja, o número de transições depende do valor de entrada).
4. Após 10 ciclos, o processo se repete, lendo novamente as entradas.

### Exemplo:

| Entradas (PB5 PB4 PB3) | `codigo` | Pisca por |
|------------------------|----------|-----------|
| 000                    | 0        | Apenas liga o LED, sem piscar. |
| 001                    | 1        | 2 transições (ligado → desligado → ligado). |
| 010                    | 2        | 4 transições. |
| 111                    | 7        | 14 transições. |

## Funções Principais

- `main()`: Loop principal que lê entradas, reinicia o timer e gerencia o LED com base na contagem de ciclos.
- `inicializar()`: Configura GPIOs e inicializa o Timer 11 com a frequência adequada.

## Observações

- O LED sempre inicia **ligado** a cada novo ciclo.
- As entradas PB3–PB5 são lidas em tempo real e definem o comportamento na iteração seguinte.
- A lógica é baseada em polling: a flag `UIF` é verificada manualmente em vez de usar interrupções.

```c
int codigo = (GPIOB->IDR & 0x38) >> 3;
```
Essa linha extrai os bits 3 a 5 de GPIOB->IDR e os alinha à direita, representando o valor de controle do LED da seguinte maneira:

```c
00000111000 = 56
00000111000>>3 = 00000000111 = 7
```
Assim, adaptamos o input, que está nos bits 3 a 5, para os 0 a 2, para que o valor do código resultante fica compatível com o código entre 0 e 7.