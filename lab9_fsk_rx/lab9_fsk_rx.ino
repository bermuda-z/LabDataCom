//Lab9 fsk rx
#include "Adafruit_ADS1015.h"
#include "Adafruit_MCP4725.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define r_slope 5.3

#define f0min 1380
#define f0max 1420

#define f1min 850
#define f1max 970

#define f2min 570
#define f2max 640

#define f3min 390
#define f3max 450



int _max = 0, i = 0, check_count = 0;
int prev = 0; 
int output = -1;
int c[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int sum[] = {0, 0};
int keep,loop_pulse = 0;;
int pulse[10];
int sum_count = 0 , start_count = 0, count;
unsigned long start_time;
unsigned long stop_time;
int check = 0;
int bit0,bit1,bit2,bit3;
int count0,count1,count2,count3;
int all_bit[3];
int res;


void setup() {
  Serial.begin(115200);
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  cli();//stop interrupts
//set timer interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);


  sei();//allow interrupts


}

ISR(TIMER2_COMPA_vect){//timer1 interrupt 8kHz 
//generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
  int tmp = analogRead(A0);
//int pulsein = pulseIn(A0,1);
//if(pulsein != 0){
//
//          Serial.println(pulsein);
//}
  if (tmp - prev > r_slope && check == 0) {
    _max = 0;
    check = 1;
  }

  if (tmp > _max) {
    _max = tmp;
  }
    
  if (_max - tmp > r_slope) {
    
    if (check) {
//          sum_bit += pulse[loop_pulse] = pulseIn(A0,1);
      int pulsein = pulseIn(A0,1);
      if(pulsein > 0){
//          Serial.print(pulsein);  
          
          if(pulsein > f3min and pulsein < f3max){
            count0++;
            if(count0 >= 8) {
//              Serial.print("  ");
//              Serial.print("bit3");
              bit0 = 3;
              count0 = 0;
              sum_count++;
            }
          }
          if(pulsein > f2min and pulsein < f2max){
            count1++;
            if(count1 >= 6) {
//              Serial.print("  ");
//              Serial.print("bit2");
              bit0 = 2;
              count1 = 0;
              sum_count++;
            }
          }
          if(pulsein > f1min and pulsein < f1max){
            count2++;
            if(count2 >= 4) {
//              Serial.print("  ");
//              Serial.print("bit1");
              bit0 = 1;
              count2 = 0;
              sum_count++;
            }
          }
          if(pulsein > f0min and pulsein < f0max){
            count3++;
            if(count3 >= 2) {
//              Serial.print("  ");
//              Serial.print("bit0");
              bit0 = 0;
              count3 = 0;
              sum_count++;
            }
          }
//          Serial.print("  ");
//          Serial.println(sum_count);
          
          if(sum_count == 1){
            all_bit[0] = bit0;
            res |= all_bit[0] << 0;
          }if (sum_count == 2){
             all_bit[1] = bit0;
             res |= all_bit[1] << 2; 
          }if (sum_count == 3){
             all_bit[2] = bit0;
             res |= all_bit[2] << 4;
          }if (sum_count == 4){
             all_bit[3] = bit0;
             res |= all_bit[3] << 6;
//             Serial.print(all_bit[0]);
//             Serial.print("  ");
//             Serial.print(all_bit[1]);
//             Serial.print("  ");
//             Serial.print(all_bit[2]);
//             Serial.print("  ");
//             Serial.print(all_bit[3]);
//             Serial.println("  ");

             sum_count = 0;
             count3 = 0;
             count2 = 0;
             count1 = 0;
             count0 = 0;
             Serial.print(char(res));
             Serial.print(" ");
             Serial.println(res,BIN);
             res = 0;
          }
      _max = 0;
    }
  prev = tmp;
  }
  }
}



void loop() {


}


   
