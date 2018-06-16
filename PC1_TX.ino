/*start2(0-1) --01
  frame1(2) -- 0,1
  control2(3-4) --00 connect,01 PC1 to PC2,10 PC2 to PC1,11 ACK
  data6(circle4(12) angle2(3)) (5-10)
  CRC8(11-18)
  stop2(19-20) --10*/
#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

#define defaultFreq 1700 //DAC speed (Hz)
#define f0  386
#define f1  630
#define f2  1012
#define f3  1592

int delay0, delay1, delay2, delay3 , state;
const double S_DAC[4] = {sin(0), sin(1.5708), sin(3.14159), sin(4.71239)};
const float A[4] = {1.25, 2.50, 3.75, 5.00};


long startb    = 0x80000; //start
long stopb     = 0x2; //stop
long sendb     = startb | stopb ;
long frame0    = 0x0; //frame0
long frame1    = 0x40000; //frame1
long cont[]    = {0x0 , 0x10000 , 0x20000 , 0x30000}; //connect , 1 to 2 , 2 to 1 , ack
long angle[]   = {0x400, 0x800, 0xC00};
long circle[3] = {0x0, 0x0, 0x0};
int cir1[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int cir2[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int cir3[] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dac.begin(0x64);

  delay0 = (1000000 / f0 - 588) / 4; // 500
  delay1 = (1000000 / f1 - 588) / 4; // 250
  delay2 = (1000000 / f2 - 588) / 4; // 100
  delay3 = (1000000 / f3 - 588) / 4; // 10

  //  Serial.print(delay0);
  //  Serial.print("  ");
  //  Serial.print(delay1);
  //  Serial.print("  ");
  //  Serial.flush();
  //  Serial.print(delay2);
  //  Serial.print("  ");
  //  Serial.println(delay3);
  encode();
}
String indata;
int x = 0;
void loop() {
  int i = 0 , frame = 0;
  if (Serial.available() > 0) {
    indata = Serial.readString();
    Serial.println(indata);
    if (indata == "connect") {     
    } else if (indata == "small") { //PC1 to PC2
      sendb |= cont[1] | circle[0] ;
    } else if (indata == "medium") {
      sendb |= cont[1] | circle[1] ;
    } else if (indata == "large") {
      sendb |= cont[1] | circle[2] ;
    }
    //split 2 bit to send
    for (int k = 0; k < 10; k++) { //00 01 10 11
      int tmp = sendb & 3;
      Serial.print("Dataframe =  ");
      Serial.print(sendb, BIN );
      Serial.print("   tmp =  ");

      Serial.print(tmp );

      switch (tmp) {
        case 0 : for (int sl = 0; sl < 5; sl++) {
            for (int s = 0; s < 4; s++) {
              x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);
              dac.setVoltage(x, false);
              delayMicroseconds(delay0);
            }
          }
        case 1 : for (int sl = 0; sl < 8; sl++) {
            for (int s = 0; s < 4; s++) {
              x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);
              dac.setVoltage(x, false);
              delayMicroseconds(delay1);
            }
          }
        case 2 : for (int sl = 0; sl < 5 * 2 + 2; sl++) {
            for (int s = 0; s < 4; s++) {
              x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);
              dac.setVoltage(x, false);
              delayMicroseconds(delay2);
            }
          }
        case 3 : for (int sl = 0; sl < 5 * 3 + 2; sl++) {
            for (int s = 0; s < 4; s++) {
              x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);
              dac.setVoltage(x, false);
              delayMicroseconds(delay3);
            }
          }
      }
      sendb >>= 2;
      Serial.println();

    }
  }
}

double mapX(double x, double in_min, double in_max, double out_min, double out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long crc(int indata[], long bits) {
  int last = 0;
  int Re[] = {0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 14; i++) {
    Re[0] = Re[1] ^ last;    //1
    Re[1] = Re[2] ^ last;    //1
    Re[2] = Re[3] ;           //0
    Re[3] = Re[4] ^ last;    //1
    Re[4] = Re[5] ;           //0
    Re[5] = Re[6] ^ last;    //1
    Re[6] = Re[7] ;           //0
    Re[7] = indata[i] ^ last;     //1
    last = Re[0];
  }

  for (int i = 0; i < 6; i++) {
    bits |= indata[i] << 13 - i ;
  }
  for (int i = 0; i < 8; i++) {
    bits |= Re[i] << 7 - i;
  }
  //Serial.println(bits,BIN);
  return bits;
}

void encode() {
  //CRC
  //010000 01010010
  //100000 10100100
  //110000 11110110
  circle[0] = crc(cir1, circle[0]) << 2;  
  circle[1] = crc(cir2, circle[1]) << 2;  
  circle[2] = crc(cir3, circle[2]) << 2;
}

