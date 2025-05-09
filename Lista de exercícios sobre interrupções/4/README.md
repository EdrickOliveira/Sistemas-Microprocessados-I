# Rolagem de Texto com Display Multiplexado

Neste exercício implementa-se a **exibição sequencial de texto ("scrolling")** em um conjunto de 4 displays de 7 segmentos multiplexados. A palavra ou frase programada percorre os displays da direita para a esquerda, simulando um letreiro digital.

## Funcionamento Geral

- O texto `"   marco sauer    "` é exibido em partes, 4 letras por vez, como se estivesse "passando" pelo visor.
- A cada passo, os 4 displays mostram um novo trecho da frase.
- Dois timers controlam o funcionamento:
  - **TIM10 (3 Hz)**: Atualiza o trecho de texto a ser exibido (rolagem).
  - **TIM11 (200 Hz)**: Atualiza qual display está ativo (multiplexação).
- A multiplexação permite exibir os quatro dígitos de forma sequencial e rápida, criando a ilusão de que estão acesos simultaneamente.

## Configuração de GPIO

- **GPIOA**: Não utilizado diretamente para entradas ou saídas neste projeto.
- **GPIOB (PB0 a PB3)**: Seleciona o display ativo (conectado ao GND de cada display). Cada display é ativado quando seu pino correspondente está em nível baixo (0).
- **GPIOC (PC0 a PC6)**: Controla os segmentos dos displays de 7 segmentos (a a g).

## Timers e Interrupções

### Timer 11 (TIM11) – Multiplexação

- Frequência: 200 Hz
- Interrupção: `TIM1_TRG_COM_TIM11_IRQHandler`
- Alterna entre os quatro displays, exibindo a letra correspondente ao display ativo.

### Timer 10 (TIM10) – Avanço do Texto

- Frequência: 3 Hz
- Interrupção: `TIM1_UP_TIM10_IRQHandler`
- Atualiza os caracteres armazenados, avançando uma letra por vez dentro da frase `"   marco sauer    "`.

## Estrutura de Dados

```c
typedef struct
{
  char letters[4]; // 4 letras a serem exibidas
  int end;         // Índice final do trecho atual na frase
} displaying_t;
```

A variável `displaying` mantém as letras atuais que serão exibidas nos quatro displays.

## Funções Principais

- `setup()`: Configura GPIOs, timers e interrupções. Inicializa os primeiros caracteres da frase.
- `TIM1_TRG_COM_TIM11_IRQHandler()`: Alterna entre os displays e mostra o caractere correspondente.
- `TIM1_UP_TIM10_IRQHandler()`: Avança uma posição no texto e atualiza os quatro caracteres que serão exibidos.
- `update_letters(char *letters, int end_index)`: Atualiza o conteúdo do vetor `letters[]` com base na posição atual do texto.
- `get_letter(char *letters, uint32_t display)`: Retorna a letra correspondente ao display ativo.
- `convert_to_display(char letter)`: Converte uma letra para o padrão de segmentos do display de 7 segmentos.

## Conversão de Letras para o Display

Cada letra minúscula (de 'a' a 'z') e o espaço (' ') são mapeados para uma combinação de segmentos. Exemplo:

- `'a'` → `0b1110111`
- `'m'` → `0b1010100`
- `' '` → `0b0000000` (display apagado)

Letras não mapeadas aparecem como todos os segmentos acesos (`0b1111111`).

## Observações

- A rolagem do texto reinicia automaticamente ao final da frase.
- Os displays são ativados por **nível baixo**, por isso os bits são invertidos com `~`.
- O controle da ativação dos displays é feito deslocando um único bit em `GPIOB->ODR`.

---

### Exemplo de Conexão do Display (resumo)

| Dígito | Pino de Seleção (PBx) |
|--------|------------------------|
| milhar | PB3                   |
| centena| PB2                   |
| dezena | PB1                   |
| unidade| PB0                   |

Cada pino é colocado em nível baixo (0) para ativar o display correspondente, enquanto os segmentos (PC0 a PC6) exibem a letra convertida.