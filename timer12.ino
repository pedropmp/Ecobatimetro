#define F_CPU 16000000UL
#include <wiring.c>
#include <avr/io.h>
#include <avr/interrupt.h>

#define _DISABLE_ARDUINO_TIMER0_INTERRUPT_HANDLER_
//--------------- MACROS Genericas ---------------//
#define clear_bit(p,b) (p&=~(0b1<<b))
#define seta_bit(p,b) (p|=0b1<<b)
#define inverte_bit(p,b) (p ^= 0b1<<b)
#define bit(p,b) ((p>>b)&0b1)
//---------------- MACROS PORTAS ----------------//
#define entrada_AI_B(b) clear_bit(DDRB,b); clear_bit(PORTB,b)
#define entrada_PU_B(b) clear_bit(DDRB,b); seta_bit(PORTB,b)
#define saida_B(b) seta_bit(DDRB,b)
#define inverte_B(b) inverte_bit(PORTB,b)
#define eh_um_B(b) bit(PINB,b)
#define eh_zero_B(b) !bit(PINB,b)
//---------------- MACROS TIMERS ----------------//
#define OVF 0                                    // bit habilitacao de interrupcao por OVF de TIMSK1
#define T1A 1                                    // bit habilitacao de interrupcao por T1A de TIMSK1
#define T1B 2                                    // bit habilitacao de interrupcao por T1B de TIMSK1
#define T1_clear TCCR1A = TCCR1B = 0;            // limpa registradores do T1
#define T1_prescaller_8 seta_bit(TCCR1B,CS11)    // prescaller 8 (CS11)
#define T1_prescaller_64 seta_bit(TCCR1B,CS10);seta_bit(TCCR1B,CS11); // prescaller 64 (CS10,CS11)
#define T1_prescaller_256 seta_bit(TCCR1B,CS12)  // prescaller 256 (CS12)
#define T1_prescaller_1024 seta_bit(TCCR1B,CS10);seta_bit(TCCR1B,CS12); // prescaller 64 (CS10,CS11)
#define T1_CTC_OCR1A seta_bit(TCCR1B,WGM12);     // CTC - overflow em OCR1A
#define T1_int(x) seta_bit(TIMSK1,x);            // habilita interrupção (OVF/T1A/T1B)
#define T1_nint(x) clear_bit(TIMSK1,x);          // desabilita interrupção (OVF/T1A/T1B)

#define T1OVF_v TIMER1_OVF_vect                  // bit habilitar interr OVF em TIMSK1
#define T1A_v TIMER1_COMPA_vect                  // bit habilitar interr T1A em TIMSK1
#define T1B_v TIMER1_COMPB_vect                  // bit habilitar interr T1B em TIMSK1

#define T2A 1
#define T2B 2
#define T2_clear TCCR2A = TCCR2B = 0;            // limpa registradores do T1
#define T2_prescaller_8 seta_bit(TCCR2B,CS21);    // prescaller 8 (CS21)
#define T2_CTC_OCR2A seta_bit(TCCR2A,WGM21);     // CTC - overflow em OCR2A

#define T2_int(x) seta_bit(TIMSK2,x);            // habilita interrupção (OVF/T2A/T2B)
#define T2_nint(x) clear_bit(TIMSK2,x);          // desabilita interrupção (OVF/T2A/T2B)
#define T2OVF_v TIMER2_OVF_vect                  // bit habilitar interr OVF em TIMSK2
#define T2A_v TIMER2_COMPA_vect                  // bit habilitar interr T2A em TIMSK2

#define T0A 1
#define T0B 2
#define T0OVF 0
#define T0_clear TCCR0A = TCCR0B = 0;            // limpa registradores do T0
#define T0_prescaller_8 seta_bit(TCCR0B,CS00) seta_bit(TCCR0B,CS02);    // prescaller 8 (CS21)
#define T0_CTC_OCR0A seta_bit(TCCR0A,WGM01);     // CTC - overflow em OCR0A

#define T0_int(x) seta_bit(TIMSK0,x);            // habilita interrupção (OVF/T0A/T0B)
#define T0_nint(x) clear_bit(TIMSK0,x);          // desabilita interrupção (OVF/T0A/T0B)
#define T0OVF_v TIMER0_OVF_vect                  // bit habilitar interr OVF em TIMSK0
#define T0A_v TIMER0_COMPA_vect                  // bit habilitar interr T0A em TIMSK0
//---------------- FIM MACROS -------------------//

void setup(){
  saida_B(5);                           // define B(5) como saída(MEGA PB5->pin11)
  saida_B(4);
  
  T1_clear;                             // limpa registradores de configuracao do TIMER1
  T1_CTC_OCR1A;                         // modo: CTC - overflow em OCR1A
  T1_prescaller_64;                     // prescaller TIMER1 64
  OCR1A = (17856);                      // (tempo_s*clock)/prescaller-1 = ((71,4286ms*16Mhz)/64)-1 
  OCR1B = 356;                          // (tempo_s*clock)/prescaller-1 = ((1,4286ms*16Mhz)/64)-1 

  //Timer2
  TCNT2 = 0;
  T2_clear;
  T2_CTC_OCR2A;
  T2_prescaller_8;
  OCR2A = 4; 

    //Timer0
  T1_int(T1A);                          // enable interrupt in T1 (OCIEA - TIMER1_COMPA_vect)
  T1_int(T1B);
  T2_int(T2A);
  /*T0_int(T0A);
  T0_nint(T0OVF);*/
}

void loop(){}

ISR(T1A_v){                      // interrupcao OCR1A match
  inverte_bit(PORTB,5);
  //T2_int(T2A);
}

ISR(T1B_v){                     // interrupcao OCR1B match
  //T2_nint(T2A);               // coloca PB5 em 0V
  clear_bit(PORTB,4);
}

ISR(T2A_v){
  inverte_bit(PORTB,4);
}

/*ISR(T0A_v){
  inverte_bit(PORTB,4);
}*/
