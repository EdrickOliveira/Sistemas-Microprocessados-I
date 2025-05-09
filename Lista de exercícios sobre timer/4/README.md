# Simulação de Duas Sinaleiras com Temporização

Este projeto implementa uma simulação de **duas sinaleiras (semáforos)** que alternam seus estados em tempo real. Cada sinaleira possui três LEDs (vermelho, amarelo e verde), representados pelos pinos PC0 a PC5.

A alternância é feita com base em contagem de tempo controlada pelo **Timer 11**, simulando o comportamento real de um semáforo veicular.

## Funcionamento Geral

- Existem **duas sinaleiras**, cada uma com três LEDs:
  - **Sinaleira 0:** usa os pinos **PC0, PC1, PC2**.
  - **Sinaleira 1:** usa os pinos **PC3, PC4, PC5**.
- Inicialmente:
  - Sinaleira 0 → verde.
  - Sinaleira 1 → vermelho.
- A cada ciclo, os estados mudam conforme o tempo decorrido:
  - **0,5 segundos:** verde → amarelo.
  - **0,7 segundos:** amarelo → vermelho, e a outra sinaleira passa a verde.
- Após um ciclo de 0,7 segundos, o processo reinicia com as sinaleiras trocando de papel.

## Temporização

- O tempo é contado em **decisegundos (0,1 s)**.
- A contagem é feita por polling (verificação manual) da flag `UIF` do Timer 11.
- Um ciclo completo de cada sinaleira leva **0,7 s**, e o processo se repete indefinidamente com alternância.

## Timer Utilizado

### Timer 11 (TIM11)

- Configuração:
  - `ARR = 999`
  - `PSC = 1599`
  - Frequência do overflow: 10 Hz → um estouro a cada 100 ms.
- A flag `UIF` é usada para marcar a passagem do tempo.

## Configuração de GPIO

- **GPIOA:** Configurado parcialmente para comunicação com o programador/debugger.
- **GPIOC:**
  - **PC0 a PC2:** LEDs da sinaleira 0 (verde, amarelo, vermelho).
  - **PC3 a PC5:** LEDs da sinaleira 1 (verde, amarelo, vermelho).
  - Todos os pinos são configurados como saída.

## Funções Principais

- `main()`: Implementa a lógica de alternância de estado entre as duas sinaleiras com base na contagem de tempo.
- `inicializar()`: Configura os GPIOs e inicia o Timer 11.
- `atualizar_leds(sinaleira_t s, int offset)`: Atualiza os pinos de LED com base na estrutura da sinaleira (`verde`, `amarelo`, `vermelho`) e o deslocamento correspondente (`0` para sinaleira 0, `3` para sinaleira 1).

## Estrutura de Dados

```c
typedef struct {
    int verde, amarelo, vermelho;
} sinaleira_t;
```
Cada sinaleira é representada por uma estrutura que define quais LEDs estão acesos.

| Tempo decorrido | Sinaleira 0 | Sinaleira 1 |
| --------------- | ----------- | ----------- |
| 0,0s            | Verde       | Vermelho    |
| 0,5s            | Amarelo     | Vermelho    |
| 0,7s            | Vermelho    | Verde       |
| ...             | Alterna     | Alterna     |

### Observações
- O tempo total do ciclo de cada sinaleira é de 0,7 segundos, após o qual os estados são invertidos.

- A lógica é duplicada no código: o ciclo é executado duas vezes no main() para garantir a troca de papéis entre as sinaleiras.

```c
atualizar_leds(sinaleira[0], 0);  // PC0–PC2
atualizar_leds(sinaleira[1], 3);  // PC3–PC5
```
Essas chamadas garantem que o estado de ambas as sinaleiras seja atualizado visualmente.