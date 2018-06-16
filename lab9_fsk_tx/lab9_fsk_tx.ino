//Lab9 fsk tx
#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

#define defaultFreq 1700 //DAC speed (Hz)
#define f0  386
#define f1  630
#define f2  1012
#define f3  1492

int delay0, delay1, delay2, delay3 ,state;
const double S_DAC[4] = {sin(0), sin(1.5708), sin(3.14159), sin(4.71239)};
const float A[4] = {1.25, 2.50, 3.75, 5.00};

int input[3];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dac.begin(0x64);
  
  delay0 = 10*(1000000/f0 - 588)/4 - 1;  // 500
  delay1 = (1 + (1000000/f1 - 588)/4)*10;  // 250
  delay2 = 10*(1000000/f2 - 588)/4;  // 100
  delay3 = 20*(1000000/f3 - 588)/4;  // 10

  Serial.print(delay0);
  Serial.print("  ");
  Serial.print(delay1);
  Serial.print("  ");
  Serial.flush();
  Serial.print(delay2);
  Serial.print("  ");
  Serial.println(delay3);

}

void loop() {

    int x = 0;
    char inData = "";

    if (Serial.available() > 0) {
    
    //inData = 'z';
    inData = Serial.read();
    //time start
    float t0 = micros();
    Serial.print("0x");
    Serial.println(inData, HEX);
    state = !state;


    if (state and inData != 0x0D and inData != 0x0Aa) {
      for (int k = 0; k < 4; k++) { //00 01 10 11
        int tmp = inData & 3;
        Serial.print("inData =  ");
        Serial.print(inData, BIN );
        Serial.print("   tmp =  ");
        
        Serial.print(tmp );

        if(tmp == 0){
          for (int sl = 0; sl < 5; sl++) {
            for (int s = 0; s < 4; s++) {              
              x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);              
              dac.setVoltage(x, false);              
              delayMicroseconds(delay0); 
            }
          }
        }else if(tmp == 1){
          for (int sl = 0; sl < 8; sl++) {
            for (int s = 0; s < 4; s++) {              
              x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);              
              dac.setVoltage(x, false);              
              delayMicroseconds(delay1); 
            }
          }         
        }else if(tmp == 2){
          for (int sl = 0; sl < 16; sl++) {
            for (int s = 0; s < 4; s++) {              
              x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);              
              dac.setVoltage(x, false);              
              delayMicroseconds(delay2); 
            }
          }
            
        }else if(tmp == 3){
          for (int sl = 0; sl < 32; sl++) {
            for (int s = 0; s < 4; s++) {              
              x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);              
              dac.setVoltage(x, false);              
              delayMicroseconds(delay3);
            }
          }         
        }
        inData >>= 2;
        Serial.println();
      }
      state = !state;
    }
    Serial.println(micros()-t0);
    }
}

double mapX(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

