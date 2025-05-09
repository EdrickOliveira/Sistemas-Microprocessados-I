# Contador com Display Multiplexado e Dois Botões

Neste exercício implementa-se um contador de 4 dígitos utilizando displays de 7 segmentos multiplexados. A contagem pode ser **incrementada** ou **decrementada** manualmente por dois botões conectados às portas **PA0** e **PA1**.

A multiplexação garante que os dígitos sejam exibidos um de cada vez, mas em frequência suficientemente alta para que o olho humano perceba como se todos estivessem acesos ao mesmo tempo.

## Funcionamento Geral

- O contador varia de **0 até 9999**, voltando a 0 se passar de 9999, ou para 9999 se for decrementado em 0.
- Dois botões controlam o valor exibido:
  - **PA0**: incrementa o número.
  - **PA1**: decrementa o número.
- A multiplexação dos displays é feita com **Timer 11 (200 Hz)**.
- A leitura dos botões e a lógica de contagem são feitas na função `main()` com debounce por software.

## Configuração de GPIO

- **GPIOA**
  - `PA0`: Entrada com resistor de pull-down (botão de incremento).
  - `PA1`: Entrada com resistor de pull-down (botão de decremento).
- **GPIOB (PB0 a PB3)**: Seleciona qual dos quatro displays está ativo (conectados ao GND de cada display).
- **GPIOC (PC0 a PC6)**: Controla os segmentos (a a g) dos displays de 7 segmentos.

## Timers e Interrupções

### Timer 11 (TIM11) – Atualização dos displays

- Frequência: 200 Hz
- Interrupção: `TIM1_TRG_COM_TIM11_IRQHandler`
- Alterna entre os 4 dígitos ativos.
- Atualiza os segmentos de acordo com o número armazenado na variável `number`.

## Leitura dos Botões

- Feita na função `main()` com uso de variáveis auxiliares para **debounce por software**.
- Quando PA0 ou PA1 ficam pressionados por tempo suficiente (aproximadamente 50.000 ciclos), o número é atualizado.
- O código espera o botão ser **solto** antes de permitir nova ação.

## Funções Principais

- `setup()`: Configura os GPIOs, Timer 11 e ativa sua interrupção.
- `main()`: Monitora os botões, aplica debounce e altera a variável `number`.
- `TIM1_TRG_COM_TIM11_IRQHandler()`: Alterna o display ativo e atualiza os segmentos.
- `get_digit(int num, uint32_t display)`: Retorna o dígito correto com base no display ativo (unidade, dezena, centena, milhar).
- `convert_to_display(int num)`: Converte um número de 0 a 9 para o padrão de segmentos correspondente ao display de 7 segmentos.

## Exibição no Display

Cada número (0–9) é convertido para um padrão de 7 bits que representa os segmentos acesos no display. Exemplo:

- `0` → `0b1111110`
- `1` → `0b0110000`
- `8` → `0b1111111`

## Observações

- O controle dos displays é feito com lógica invertida (`~`), já que os displays são ligados ao GND (nível baixo ativa).
- O deslocamento de bits em `GPIOB->ODR` determina qual display está ativo a cada momento.
- A multiplexação permite economia de pinos no microcontrolador e atualização fluida dos dígitos.

---

### Exemplo de Conexão do Display (resumo)

| Dígito | Pino de Seleção (PBx) |
|--------|------------------------|
| milhar | PB3                   |
| centena| PB2                   |
| dezena | PB1                   |
| unidade| PB0                   |

Cada pino é colocado em nível baixo (0) para ativar o display correspondente, enquanto os segmentos (PC0 a PC6) mostram o número.

