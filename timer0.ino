/*
  Configuracao do timer 0
*/

//flag necessária para disabilitar no wiring.c o código que seta o timer 0
#define _DISABLE_ARDUINO_TIMER0_INTERRUPT_HANDLER_
#include <wiring.c>
#include <avr/io.h>
#include <avr/interrupt.h>

int counter;

ISR(TIMER0_OVF_vect){

  counter ++;

  if(counter == 49){
    PORTD ^= 0x01;          //Inverte o bit PD0
    counter = 0x00;         // Zera o counter
  }
}



void setup() {

  cli();                    // Desabilita a interrupcao global        
  
  DDRD |= (1<<0);           // PD0 saída digital
  PORTD &= ~(1<<0);         // inicia PD0 em nivel low


  TCNT0 = 0x00;             // inicia o timer 0 em 0
  TCCR0B = 0x04;            // configura o prescaler para 1 : 256
  TIMSK0 = 0x01;            // habilita a interrupcao por estouro do timer0

  sei();                    // habilita a interrupcao global

/*
  Periodo: 400ms
  Troca de estado de PD0: 200ms


  Ciclo de máquina: 1/16Mhz = 62.5ns
  
  Estouro = timer0*prescaler*cicloDeMaquina = 4.08ms
  
  Troca de estado = Estouro*counter
  200 = 4.08ms*counter
  counter = 49.01
  
*/
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
