# Tocador de Notas com Teclado de 12 Teclas – STM32F4

Este projeto permite tocar diferentes notas musicais com base na leitura de **12 teclas digitais conectadas aos pinos PC0 a PC11**. Cada tecla corresponde a uma nota da escala cromática, e o som é gerado por meio de um buzzer ou alto-falante conectado ao pino **PA0**, usando o **Timer 11** do STM32F4.

---

## Funcionamento Geral

- Ao pressionar uma tecla (PC0 a PC11), a nota correspondente é emitida em **PA0**.
- Se nenhuma tecla estiver pressionada, a saída é desligada.
- A geração do som é feita com **modulação por frequência**: alternando o estado do pino PA0 com base na frequência da nota desejada.
- O controle é feito por verificação da flag de estouro (`UIF`) do **Timer 11**.

---

## Notas Disponíveis

As 12 notas mapeadas são:

```
DO, DO#, RE, RE#, MI, FA, FA#, SOL, SOL#, LA, LA#, SI
```

E mais uma posição adicional (`OFF`) para indicar que nenhuma tecla está pressionada.

---

## Temporização com Timer 11

- O **Timer 11** controla a frequência da nota.
- A cada vez que a flag `UIF` (Update Interrupt Flag) é ativada, o pino **PA0** é invertido (alternando entre nível alto e baixo), gerando uma onda quadrada.
- A frequência dessa oscilação determina a nota ouvida.

### Fórmula:

```
Período total da nota = 1 / frequência
Período de ativação do timer = período total / 2

ARR fixo em 2
PSC calculado para gerar a frequência desejada
```

---

## Entradas e Saídas

- **Entradas (Teclado):**
  - **PC0 a PC11** configurados como entradas com **pull-down**.
  - Apenas uma tecla deve estar pressionada por vez.

- **Saída (Som):**
  - **PA0** conectado ao buzzer.
  - Configurado como saída digital.

---

## Funções Principais

### `main()`
- Chama `inicializar()`.
- Executa continuamente `atualizar_nota()` para verificar qual tecla foi pressionada.
- Gera a onda de áudio em PA0 enquanto a nota estiver ativa.

### `atualizar_nota()`
- Verifica qual tecla está ativa em PC0 a PC11.
- Atualiza o prescaler do Timer 11 (`TIM11->PSC`) com base na nota correspondente.

### `inicializar()`
- Configura os GPIOs de entrada e saída.
- Inicializa o Timer 11.

---

## Observações

- O sistema funciona com **polling**, sem uso de interrupções.
- Somente uma tecla é interpretada por vez.
- As notas são representadas por valores de prescaler que resultam nas frequências musicais desejadas.
