//Lab6 sampling32
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#define defaultFreq 1700
#define freq0 100
float zeta=0;
Adafruit_MCP4725 dac;
int delay0;
double xx[31];
void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");

  dac.begin(0x64);
  delay0 = (1000000/freq0 - 1000000/defaultFreq)/32;

  Serial.print("delay0 is : ");
  Serial.println(delay0);
   for(int i=0; i<32 ; i++)
  {
     double x = sin(2*3.141*i/32);
     xx[i] = mapX(x,-1,1,0,4095); 
  }
}
void loop() { 
  for(int i=0; i<32 ; i++)
  {  
     dac.setVoltage((int)xx[i], false);
     //Serial.print(zeta);
     //Serial.print("   ");
     //Serial.println(delay0);
     //delay(200);
     delayMicroseconds(delay0);    
  }
}
double mapX(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
