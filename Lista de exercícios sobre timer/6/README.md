# Sequência Crescente e Decrescente com LEDs

Este projeto implementa uma animação com 8 LEDs conectados aos pinos **PC0 a PC7**, onde os LEDs são acesos um a um da esquerda para a direita e depois apagados um a um da direita para a esquerda. O controle é feito por temporização com o **Timer 11** do STM32F4.

## Funcionamento Geral

- Todos os LEDs são inicialmente desligados.
- Um LED é aceso a cada **0,5 segundos**, do **PC0 até PC7**.
- Após todos acesos, os LEDs são apagados da direita para a esquerda (**PC7 até PC0**), também com intervalo de **0,5 segundos**.
- A sequência então se repete continuamente.

## Timer Utilizado

### Timer 11 (TIM11)

- Período: 0,5 segundos
- Configuração:
  - `ARR = 1999`
  - `PSC = 3999`
- A flag `UIF` é verificada para indicar o momento de acionar ou apagar um LED.

## Configuração de GPIO

- **GPIOA**: Configurado parcialmente para uso do programador/debugger.
- **GPIOC (PC0 a PC7)**: Saídas digitais conectadas aos LEDs.
  - Todos os pinos são configurados como saída.
  - `GPIOC->ODR` é manipulado para acender e apagar os LEDs bit a bit.

## Lógica de Animação

1. **Fase de acendimento:**
   - A cada 0,5s, um bit é ativado usando `GPIOC->ODR |= (1 << i)`.
   - Isso acende os LEDs de **PC0 até PC7**.
   - A fase termina quando todos os 8 bits estiverem em 1 (`GPIOC->ODR == 0xFF`).

2. **Fase de apagamento:**
   - A cada 0,5s, um bit é desativado usando `GPIOC->ODR &= ~(0x80 >> i)`.
   - Isso apaga os LEDs de **PC7 até PC0**.
   - A fase termina quando todos os LEDs estiverem desligados (`GPIOC->ODR == 0`).

### Exemplo Visual:

```
Fase 1 (acendendo):    PC0 → PC1 → ... → PC7
Fase 2 (apagando):     PC7 → PC6 → ... → PC0
```

## Funções Principais

- `main()`: Executa o loop de animação com duas fases (acendimento e apagamento).
- `inicializar()`: Configura os GPIOs e o Timer 11 para gerar ticks de 0,5 segundos.

## Observações

- O controle é feito via polling (verificação direta da flag `UIF`), sem uso de interrupções.
- A manipulação dos bits no registrador `ODR` permite controlar individualmente os LEDs com operações simples de deslocamento e máscara.
- Esse projeto é útil como exercício introdutório em manipulação de bits e temporização com timers.

```c
GPIOC->ODR |= (1 << i);      // Acende LED PCi
GPIOC->ODR &= ~(0x80 >> i);  // Apaga LED PC(7 - i)
```

Essas duas operações compõem toda a lógica de exibição.
