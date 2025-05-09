# Relógio com Alarme e Display Multiplexado

Este projeto implementa um relógio digital com exibição em displays de 7 segmentos multiplexados e funcionalidade de alarme configurável. O relógio exibe **horas, minutos e segundos (hhmmss)** com atualização a cada segundo, e permite configurar um alarme com dois botões.

## Funcionamento Geral

- O tempo é contado internamente em segundos.
- O número exibido é convertido para o formato `hhmmss` nos 6 displays.
- Dois botões (PA0 e PA1) são usados para configurar o tempo e o alarme.
- Um alarme pode ser definido, e um sinal sonoro (PC8) é ativado quando o horário atual coincidir com o horário do alarme.
- O LED em PC7 pisca durante o modo de configuração do alarme.
- A multiplexação permite a exibição sequencial dos 6 dígitos.

## Configuração de GPIO

- **GPIOA (PA0 e PA1)**: Entradas com pull-down, utilizadas como botões para controlar hora, minuto e alarme.
- **GPIOB (PB0 a PB5)**: Seleção dos displays ativos (6 dígitos).
- **GPIOC (PC0 a PC6)**: Segmentos dos displays de 7 segmentos.
- **GPIOC (PC7)**: LED indicador (pisca durante configuração do alarme).
- **GPIOC (PC8)**: Saída do alarme sonoro.

## Timers e Interrupções

### Timer 10 – Contador de Tempo
- Frequência: 1 Hz
- Interrupção: `TIM1_UP_TIM10_IRQHandler`
- Incrementa `time_number` a cada segundo.

### Timer 11 – Multiplexação do Display
- Frequência: 400 Hz
- Interrupção: `TIM1_TRG_COM_TIM11_IRQHandler`
- Alterna os displays ativos e exibe o número atual (tempo ou alarme).

### Timer 13 – Piscar do LED
- Frequência: 2 Hz
- Interrupção: `TIM8_UP_TIM13_IRQHandler`
- Pisca o LED (PC7) durante a configuração do alarme.

## Controles com Botões

- **PA0 (botão 0)**: Adiciona 1 minuto ao tempo atual.
- **PA1 (botão 1)**: Adiciona 1 hora ao tempo atual.
- **PA0 + PA1 pressionados por 1 segundo**:
  - Entra no modo de configuração do alarme.
  - Sai do modo de configuração do alarme.
- **Durante a configuração do alarme**:
  - PA0 adiciona 1 minuto.
  - PA1 adiciona 1 hora.
  - Segurar qualquer botão por mais de 3 segundos sai da configuração e cancela o alarme.

## Funções Principais

- `setup()`: Configura os GPIOs, timers e interrupções.
- `main()`: Gerencia os botões, incrementa o tempo, ativa/desativa o alarme.
- `alarm_settings()`: Lida com a configuração do alarme.
- `to_display_number(int num)`: Converte o tempo (em segundos) para `hhmmss`.
- `get_digit(int num, uint32_t display)`: Extrai o dígito a ser exibido com base no display ativo.
- `convert_to_display(int num)`: Converte um dígito numérico (0 a 9) para o padrão de segmentos.

## Exibição no Display

A exibição segue o formato **hhmmss**, como por exemplo:

- `00:00:00` → `000000`
- `12:30:45` → `123045`

Cada dígito é convertido para o padrão de 7 segmentos correspondente. Exemplo:

- `0` → `0b1111110`
- `1` → `0b0110000`
- `8` → `0b1111111`

## Observações

- A contagem é em segundos, mas a exibição em display mostra hora, minuto e segundo.
- O alarme pode ser cancelado tanto durante a configuração quanto quando é disparado.
- Os displays são ativados por nível baixo (bit invertido com `~`).

---

### Mapeamento dos Displays (PBx)

| Dígito   | PBx Ativo (nível 0) |
|----------|---------------------|
| Unidade  | PB0                |
| Dezena   | PB1                |
| Centena  | PB2                |
| Milhar   | PB3                |
| Dez mil  | PB4                |
| Centena milhar | PB5         |