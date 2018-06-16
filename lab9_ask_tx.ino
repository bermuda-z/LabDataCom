//Lab9 ask tx
#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;


#define defaultFreq 1700 //DAC speed (Hz)
#define freq0 1500

const double S_DAC[4] = {sin(0), sin(1.5708), sin(3.14159), sin(4.71239)};
int delay0 , state = 0;
float A[] = {1.25,2.5,3.75,5.00} ;
void setup() {
  Serial.begin(115200);
  dac.begin(0x64);
  delay0 = (1000000 / freq0 - 1000000 / defaultFreq) / 4;
  Serial.flush();

}

void loop() {
  int i = 0, x = 0;
  char inData = "";

  if (Serial.available() > 0) {

    inData = Serial.read();
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
        for (int sl = 0; sl < 5; sl++) {
          for (int s = 0; s < 4; s++) {
            x = mapX(S_DAC[s] * A[tmp], -5, 5, 0, 4095);
            dac.setVoltage(x, false);
            delayMicroseconds(delay0); 
          }
        }
        inData >>= 2;
        Serial.println();
      }
      state = !state;
    }
  }

  dac.setVoltage(0, false);
}
// 11 00 00 00    01 00 00 00    10 00 00 00

double mapX(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
