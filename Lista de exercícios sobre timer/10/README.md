# Execução de Melodia com Temporização – STM32F4

Este projeto executa automaticamente uma **melodia pré-programada** por meio de um buzzer conectado ao pino **PA0**. A reprodução das notas é feita com dois timers: **Timer 11** para gerar o som (frequência da nota) e **Timer 10** para controlar a **duração de cada nota**.

---

## Funcionamento Geral

- As notas da melodia estão armazenadas em um array (`waltz_n2[]`).
- O sistema percorre esse array sequencialmente, atualizando a frequência da nota no Timer 11.
- Cada nota dura **300ms** (tempo completo), sendo que a **mudança de nota** ocorre a cada **150ms** (meio tempo), garantindo boa definição sonora.
- O buzzer em PA0 emite som por meio de uma **onda quadrada**, alternando nível alto/baixo com base na frequência.

---

## Geração de Som

### Timer 11 (TIM11)
- Responsável por gerar a **frequência da nota**.
- A cada estouro, inverte o nível de PA0.
- A frequência é controlada ajustando o valor de `PSC`.

### Timer 10 (TIM10)
- Controla o **tempo de duração de cada nota**.
- A cada 150ms (meio tempo), avança para a próxima nota no array.
- Garante sincronização correta entre as notas.

---

## Representação das Notas

- As notas são representadas por valores inteiros no `enum nota`, que refletem diretamente os valores de `PSC` para gerar as frequências corretas.
- Funções auxiliares como `oitava_acima()` e `oitava_abaixo()` são usadas para gerar variações da mesma nota em outras oitavas.

---

## Estrutura da Melodia

- O array `waltz_n2[]` contém uma sequência longa de notas e suas variações.
- A música é executada indefinidamente em loop.
- Quando chega ao fim do array, retorna ao início.
- A música em `waltz_n2[]` é a `Waltz No. 2`, composição por `Dmitri Shostakovitch`. A versão original da valsa pode ser acessada em https://www.youtube.com/watch?v=phBThlPTBEg, para fins de verificação de funcionamento do exercício.

---

## Funções Principais

### `main()`
- Inicializa o sistema com a primeira nota da melodia.
- A cada estouro do **Timer 10**, muda para a próxima nota.
- O **Timer 11** gera a onda sonora alternando o pino PA0.

### `inicializar(int primeira_nota)`
- Configura GPIOs.
- Ativa e configura os timers:
  - `TIM11` para som (PA0)
  - `TIM10` para temporização das notas

### `oitava_acima()` e `oitava_abaixo()`
- Funções auxiliares que retornam o valor do prescaler correspondente à mesma nota em outra oitava.

---

## Observações

- Controle feito com **polling**, sem interrupções.
- A saída em PA0 é uma onda quadrada.

---

## Código de Destaque

```c
if(TIM11->SR & 1) {
	GPIOA->ODR ^= 1;  // Alterna estado do alto-falante
	TIM11->SR &= ~(0x1);
	TIM11->CNT = 0;
}
```

Esse trecho é responsável por gerar a onda sonora para cada nota.
