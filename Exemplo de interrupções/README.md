# Exemplo de Interrupções por Timer

Este código é um exemplo simples do uso de interrupções por timer, com o objetivo de facilitar a sua utilização.

As linhas 40 a 44 configuram o timer da mesma forma que quando não se utiliza a sua interrupção (consulte "Exercícios sobre interrupções" para explicações adicionais).

A linha 46 habilita a interrupção do timer 11. Dessa forma, quando a flag `TIM11->SR` for "estourada" (ou seja, mudar de 0 para 1), a função de interrupção será chamada automaticamente.

Se, enquanto uma interrupção estiver sendo executada, outra for chamada, o programa pode esperar a execução da interrupção atual terminar para, então, executar a próxima — ou pode pausar a execução da atual para executar a próxima, retornando à anterior depois.
O que define qual delas terá prioridade é a configuração na linha 48.

Cada timer possui sua própria definição de prioridade, como exemplificado:

- Timer 10: `NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 2);`
- Timer 11: `NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 5);`

A interrupção que tiver o menor número de prioridade (número mais baixo) será executada primeiro.

A linha 49 indica: "a partir daqui, quando a flag do timer 11 (`TIM11->SR`) passar para 1, chame a função de interrupção". Ou seja, quando a flag for estourada, o conteúdo da função de interrupção do timer 11 será executado, independentemente do que estiver acontecendo no loop principal.

As linhas 52 a 60 realizam as mesmas configurações, porém para o timer 10.

As funções de interrupção são:

- Timer 11: `void TIM1_TRG_COM_TIM11_IRQHandler(void);`
- Timer 10: `void TIM1_UP_TIM10_IRQHandler(void);`

Observe que a flag deve ser "limpa" (redefinida para 0) dentro da função de interrupção, para que ela possa ser chamada novamente no próximo estouro.
