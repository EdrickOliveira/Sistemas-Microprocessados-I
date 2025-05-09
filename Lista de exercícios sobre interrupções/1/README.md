Contador com Display Multiplexado

Neste exercício implementa-se um contador de 4 dígitos em display de 7 segmentos multiplexados, ou seja, cada dígito é mostrado no display correspondente não todos ao mesmo tempo, mas um de cada vez, em uma frequência alta o suficiente para "enganar" o olho humano e parecer que estão todos ligados ao mesmo tempo.

## Funcionamento Geral

- O contador incrementa de 0 até 9999 e reinicia.
- Cada dígito é exibido sequencialmente por multiplexação (um de cada vez).
- Dois timers controlam a lógica:
  - **TIM10 (2 Hz)**: Incrementa o número exibido.
  - **TIM11 (200 Hz)**: Altera o display que está ativo (multiplexação entre os 4 dígitos).

## Configuração de GPIO

- **GPIOA**: Pinos configurados conforme necessário pelo projeto (não utilizados explicitamente neste código).
- **GPIOB (PB0 a PB3)**: Seleção do dígito ativo (multiplexação dos displays), ligados ao terra de cada display. O display é ligado quando sua porta PB correspondente vale 0.
- **GPIOC (PC0 a PC6)**: Segmentos do display de 7 segmentos.

## Timers e Interrupções

### Timer 10 (TIM10) – Incremento do número

- Frequência: 2 Hz
- Interrupção: `TIM1_UP_TIM10_IRQHandler`
- Responsável por incrementar o número exibido.
- Reinicia para 0 após atingir 9999.

### Timer 11 (TIM11) – Atualização dos displays

- Frequência: 200 Hz
- Interrupção: `TIM1_TRG_COM_TIM11_IRQHandler`
- Controla a alternância entre os dígitos (multiplexação).
- Usa o valor do número atual para determinar qual dígito exibir com base no display ativo.

## Funções Principais

- `setup()`: Inicializa GPIOs, timers e configura as interrupções.
- `TIM1_UP_TIM10_IRQHandler()`: Incrementa o número e atualiza o display ativo.
- `TIM1_TRG_COM_TIM11_IRQHandler()`: Muda para o próximo display e atualiza o valor exibido.
- `get_digit(int num, uint32_t display)`: Extrai o dígito correto com base no display ativo.
- `convert_to_display(int num)`: Converte o dígito para o padrão binário correspondente ao display de 7 segmentos.

## Exibição no Display

Cada número (0 a 9) é convertido para um padrão de 7 bits correspondente ao estado dos LEDs do display. Exemplo:

- `0` → `0b1111110`
- `1` → `0b0110000`
- `8` → `0b1111111`

## Observações

- A multiplexação é feita por negação do valor binário (`~`), já que o display é ativo quando o pino ligado ao seu GND vale 0.
- A ordem de ativação dos dígitos é controlada por deslocamento de bits em `GPIOB->ODR`.

---

### Exemplo de Conexão do Display (resumo)

| Dígito | Pino de Seleção (PBx) |
|--------|------------------------|
| milhar | PB3                   |
| centena| PB2                   |
| dezena | PB1                   |
| unidade| PB0                   |

Cada pino ativa (é igual a 0) um dígito por vez, enquanto os segmentos (PC0-PC6) mostram o número.
