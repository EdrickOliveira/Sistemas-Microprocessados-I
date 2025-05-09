# Exemplo de interrupções por timer
Este código é um exemplo simples do uso de interrupções por timer, para facilitar o seu uso.

As linhas 40 a 44 configuram o timer, do mesmo modo que quando não é usada a sua interrupção (vide "Exercícios sobre interruções" para explicações).

A linha 46 habilita a interrupção do timer 11. Deste modo, quando a flag TIM11->SR for "estourada" (passar seu valor de 0 para 1), será chamada a função de interrupção automaticamente.

Se, enquanto uma interrupção estiver sendo executada, outra for chamada, o programa pode esperar a interrupção em execução ser finalizada para então executar a próxima, ou pausar a execução da atual para executar a próxima e depois retornar à primeira.
O que define qual delas terá prioridade, é a configuração na linha 48.
Cada timer tem a sua própria função de prioridade, como exemplificado:

Timer 10: NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 2);

Timer 11: NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 5);

A interrupção que tem prioridade é a com o menor número.

A linha 49 está dizendo "a partir daqui, quando a flag do timer 11 (TIM11->SR) passar para 1, chame a função de interrupção". Ou seja, quando a flag for estourada, o que está dentro da função de interrupção do timer 11 será executado, independente do que estiver acontecendo no loop principal.

As linhas 52 a 60 realizam as mesmas configurações, mas para o timer 10.

As funções de interrupção são:

Timer 11: void TIM1_TRG_COM_TIM11_IRQHandler(void);

Timer 10: void TIM1_UP_TIM10_IRQHandler(void);

Observe que a flag deve ser "limpa" (assinalada como 0 novamente) dentro da interrupção para que ela seja chamada novamente no próximo estouro.