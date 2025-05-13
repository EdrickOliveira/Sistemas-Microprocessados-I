# Efeitos com LEDs Controlados por Botões – STM32F4

Este projeto implementa três efeitos visuais com **8 LEDs conectados aos pinos PC0 a PC7** de um STM32F4. A escolha do efeito é feita através de dois botões conectados aos pinos **PA0** e **PA1**. A animação é temporizada usando o **Timer 11**, que gera um "tick" a cada **250ms**.

---

## Funcionamento Geral

O comportamento dos LEDs depende do estado dos dois botões (PA0 e PA1), lidos simultaneamente como os dois bits menos significativos de `GPIOA->IDR`.

### Estado dos botões:
- **PA1 PA0 = 00** → Efeito 1: LED único indo e voltando.
- **PA1 PA0 = 01** → Efeito 2: LEDs acendendo da esquerda para a direita, depois apagando da direita para a esquerda.
- **PA1 PA0 = 10** → Efeito 3: "ping-pong progressivo", com alcance crescente e decrescente.

Cada efeito é executado enquanto os botões permanecem no mesmo estado. Ao mudar o estado dos botões, o efeito correspondente começa a ser executado.

---

## Efeitos Disponíveis

### 🟢 Efeito 1 – LED vai-e-volta (PA1 PA0 = `00`)
- Um único LED acende por vez.
- Ele se desloca da esquerda (PC0) até a direita (PC7) e depois volta.
- A direção é invertida automaticamente quando o LED atinge uma das pontas.

### 🟡 Efeito 2 – Crescente e decrescente (PA1 PA0 = `01`)
- LEDs se acendem um a um, da esquerda para a direita (PC0 até PC7).
- Em seguida, se apagam um a um da direita para a esquerda (PC7 até PC0).
- O processo se repete continuamente.

### 🔵 Efeito 3 – Ping-pong progressivo (PA1 PA0 = `10`)
- Um único LED "ping-ponga" como no efeito 1, mas com **amplitude progressiva**:
  - Vai e volta entre PC0 ↔ PC1, depois PC0 ↔ PC2, até PC0 ↔ PC7.
- Após isso, o movimento regride (PC0 ↔ PC6, depois PC0 ↔ PC5, etc.).
- O ciclo reinicia quando retorna à amplitude mínima.

---

## Entradas e Saídas

### Entradas (Botões)
- **PA0 e PA1**: usados para selecionar o efeito desejado.
- Configurados como **entradas com pull-down**.

### Saídas (LEDs)
- **PC0 a PC7**: conectados aos LEDs, configurados como **saídas digitais**.

---

## Temporização

### Timer 11 (TIM11)
- Responsável pelo controle do tempo dos efeitos.
- Configurado para gerar um evento a cada **250ms**.
- Verificação feita via **polling da flag UIF** (`TIM11->SR & 1`).

### Configuração:
- `ARR = 1999`
- `PSC = 1999`

---

## Funções Principais

### `main()`
- Inicializa o sistema.
- Limpa os LEDs.
- Executa continuamente a função `efeito()`, que determina qual animação exibir com base na leitura dos botões.

### `efeito()`
- Lê o valor dos botões e executa o efeito correspondente enquanto o estado não mudar.
- Implementa a lógica dos três efeitos.

### `deslocar_led(sentido_t sentido)`
- Desloca o LED aceso para a **direita** ou **esquerda**.
- `sentido` é um `enum` com dois valores: `DIREITA` e `ESQUERDA`.

---

## Observações

- O controle de tempo é feito por **polling**, sem interrupções.
- As direções dos LEDs e o sentido da animação são controlados por simples operações de deslocamento de bits (`<<` e `>>`) e máscaras.
- Os efeitos são responsivos ao estado dos botões, mudando dinamicamente sem reiniciar o microcontrolador.

---

## Código de Exemplo:

```c
GPIOC->ODR = GPIOC->ODR << 1;  // Desloca LED para a esquerda
GPIOC->ODR = GPIOC->ODR >> 1;  // Desloca LED para a direita
```

Essas operações compõem a base do controle de movimento dos LEDs.

---

Este projeto é ideal para praticar:
- Leitura de entradas digitais.
- Controle de saídas com registradores.
- Manipulação de bits.
- Temporização com timers no STM32F4.
- Organização de lógica condicional em sistemas embarcados.
