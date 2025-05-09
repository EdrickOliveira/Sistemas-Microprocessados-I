# Contador com Display Multiplexado e Botão

Neste exercício implementa-se um contador de 4 dígitos utilizando displays de 7 segmentos multiplexados. A contagem é feita por meio de um **botão conectado ao pino PA0**, que incrementa o número exibido a cada clique.

A multiplexação faz com que os dígitos não sejam acesos simultaneamente, mas alternadamente em alta velocidade, o que cria a ilusão visual de que todos estão acesos ao mesmo tempo.

## Funcionamento Geral

- O contador vai de 0 até 9999 e reinicia.
- O número só é incrementado quando o botão (PA0) for pressionado e mantido por tempo suficiente (debounce por software).
- Apenas o Timer 11 é usado, para atualizar o display ativo.
- A leitura do botão e a contagem são feitas na `main()`.

## Configuração de GPIO

- **GPIOA**
  - `PA0`: Entrada com resistor de pull-down ativado (utilizado como botão).
  - Outros pinos configurados conforme necessário pelo código.
- **GPIOB (PB0 a PB3)**: Controle do dígito ativo (multiplexação), ligados ao GND de cada display. O display é ativado quando seu respectivo pino está em nível baixo (0).
- **GPIOC (PC0 a PC6)**: Controle dos segmentos dos displays de 7 segmentos.

## Timers e Interrupções

### Timer 11 (TIM11) – Atualização dos displays

- Frequência: 200 Hz
- Interrupção: `TIM1_TRG_COM_TIM11_IRQHandler`
- Alterna rapidamente entre os quatro dígitos, atualizando os segmentos para exibir o número correspondente ao dígito atual.

## Leitura do Botão

- A leitura do botão é feita em `main()`, monitorando `PA0`.
- Um contador (`button`) atua como **debounce** por software: só após ultrapassar um certo valor é que o número é incrementado.
- Após o incremento, o código aguarda o botão ser solto antes de permitir uma nova contagem.

## Funções Principais

- `setup()`: Configura GPIOs, timers e interrupções.
- `main()`: Aguarda pressionamento do botão, implementa debounce e incrementa o número exibido.
- `TIM1_TRG_COM_TIM11_IRQHandler()`: Alterna o display ativo e atualiza o valor exibido.
- `get_digit(int num, uint32_t display)`: Retorna o dígito correspondente ao display ativo.
- `convert_to_display(int num)`: Converte um número de 0 a 9 para o padrão de segmentos do display de 7 segmentos.

## Exibição no Display

Cada número é convertido em um padrão de 7 bits para acionar os LEDs do display. Exemplo:

- `0` → `0b1111110`
- `1` → `0b0110000`
- `8` → `0b1111111`

## Observações

- A multiplexação é feita invertendo-se os bits com `~`, pois os displays são ativados em nível baixo (conectados ao GND via PBx).
- O controle de qual dígito está ativo é feito deslocando um único bit pela porta `GPIOB->ODR`.
- O uso de apenas um timer torna o projeto mais simples, com o controle da lógica de contagem feito via polling e debounce.

---

### Exemplo de Conexão do Display (resumo)

| Dígito | Pino de Seleção (PBx) |
|--------|------------------------|
| milhar | PB3                   |
| centena| PB2                   |
| dezena | PB1                   |
| unidade| PB0                   |

Cada pino é colocado em nível baixo (0) para ativar seu display correspondente, enquanto os segmentos (PC0 a PC6) exibem o número.

