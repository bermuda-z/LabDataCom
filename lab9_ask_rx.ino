//Lab9 ask rx
#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

#define defaultFreq 1700 //DAC speed (Hz)
#define freq0 1500
const float Amp[4] = {1.25, 2.5, 3.75, 5};
int delay0;
#define a0min 369
#define a0max 631
#define a1min 235
#define a1max 763
#define a2min 104
#define a2max 897
#define a3min 22
#define a3max 978

#define r_slope 5.3
int data = 0;
int sum[2] = {0, 0};
int maxi = 0;
int prev = 0;
int check = false;
int x = 0;
int count = 0;
int c[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int i = 7;
int keep;


void setup() {

  Serial.begin(115200);
  dac.begin(0x64);
  delay0 = (1000000 / freq0 - 1000000 / defaultFreq) / 4;
  Serial.flush();
}

void loop() {

  int tmp = analogRead(A0);
  if (tmp  - prev > r_slope && check == false) {
    maxi = 0;
    check = true;
  }
  if (tmp > maxi) {
    maxi = tmp;
  }

  if (maxi - tmp > r_slope) {

    if (check == true) {

      if (a0min < maxi && maxi < a0max) {
        sum[0] = 0;
        sum[1] = 0;
        count++;
      }
      else if (a1min < maxi && maxi < a1max) {
        sum[0] = 0;
        sum[1] = 1;
        count++;
      }
      else if (a2min < maxi && maxi < a2max) {
        sum[0] = 1;
        sum[1] = 0;
        count++;
      }
      else if (a3min < maxi && maxi < a3max) {
        sum[0] = 1;
        sum[1] = 1;
        count++;
      }
      //Serial.print(sum);
      if (count == 4) {
        c[i] = sum[1];
        c[i-1] = sum[0];
        i=i-2;
      }
      if (count == 5)
      {
        count = 0;
      }
      if(i == 0){
        for(int j=0;j<8;j++){
          keep |= c[j]<<j;
          Serial.print(c[j]);
        }
        Serial.print(keep,BIN);
        Serial.println();
        i = 7;
      }
    }
    
    check = false;
  }
 
  prev = tmp;
}
                                                                                    
