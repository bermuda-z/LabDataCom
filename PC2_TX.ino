/*start2(0-1) --01
  frame1(2) -- 0,1
  control2(3-4) --00 connect,01 PC1 to PC2,10 PC2 to PC1,11 ACK
  data6(circle4(12) angle2(3)) (5-10)
  CRC8(11-18)
  stop2(19-20) --10*/
#include <Servo.h>
Servo myservo;
#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

#define defaultFreq 1700 //DAC speed (Hz)
#define f0  386
#define f1  630
#define f2  1012
#define f3  1492

int delay0, delay1, delay2, delay3 , state;
const double S_DAC[4] = {sin(0), sin(1.5708), sin(3.14159), sin(4.71239)};
const float A[4] = {1.25, 2.50, 3.75, 5.00};

long startb    = 0x10000; //start
long stopb     = 0x2; //stop
long sendb     = startb | stopb ;
long frame0    = 0x0; //frame0
long frame1    = 0x8000; //frame1
long cont[]    = {0x0 , 0x2000 , 0x4000 , 0x6000}; //connect , 1 to 2 , 2 to 1 , ack
long angle[]   = {0x80, 0x100, 0x180};
long circle[3] = {0x0, 0x0, 0x0};
int cir1[] = {0, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0};
int cir2[] = {1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0};
int cir3[] = {1, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0};
long ack = 0x16002;
char indata;
int pos[] = {18, 69, 115};
char sizes[3];
int count = 0;
int tmp;
int x = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myservo.attach(9);
  dac.begin(0x64);

  delay0 = 10 * (1000000 / f0 - 588) / 4 - 1; // 500
  delay1 = (1 + (1000000 / f1 - 588) / 4) * 10; // 250
  delay2 = 10 * (1000000 / f2 - 588) / 4; // 100
  delay3 = 20 * (1000000 / f3 - 588) / 4; // 10
  //  Serial.print(delay0);
  //  Serial.print("  ");
  //  Serial.print(delay1);
  //  Serial.print("  ");
  //  Serial.flush();
  //  Serial.print(delay2);
  //  Serial.print("  ");
  //  Serial.println(delay3);
}


void loop() {
  
  myservo.write(pos[count]);
  sendb = startb | stopb;
  if (Serial.available() > 0) {
    //input from python 4,8,12 keep size
    indata = Serial.read();
    Serial.println(indata);
    sizes[count] = indata;      //match size angle

    //CRC
    //frame
    
    //circle + angle + CRC
    
    cirangcrc(count, indata);
    Serial.println(sendb , BIN);
    //split 20 bit send 2 bit
    sender();

    //รอ ACK PC1 if(ไม่ ACK){ split(); ส่งใหม่}
    count++;
  }

}

//1 0 11 0000 00 00000 10
//4 8 12
//1 0 10 0100 01 01100 10
//1 0 10 1000 10 11000 10
//1 0 10 1100 11 10100 10

//8 4 12
//1 0 10 1000 01 10010 10
//1 0 10 0100 10 00110 10
//1 0 10 1100 11 10100 10

//8 12 4
//1 0 10 1000 01 10010 10
//1 0 10 1100 10 00001 10
//1 0 10 0100 11 10011 10

//12 4 8
//1 0 10 1100 01 01011 10
//1 0 10 0100 10 00110 10
//1 0 10 1000 11 01101 10

//12 8 4
//1 0 10 1100 01 01011 10
//1 0 10 1000 10 11000 10
//1 0 10 0100 11 10011 10
double mapX(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void cirangcrc(int count , int indata) {     //ใส่มุม
  sendb |= cont[2];
  if (count == 0) {   
    if (indata == '4') { //PC1 to PC2
      cir1[4] = 0;
      cir1[5] = 1;
      circle[0] = crc(cir1, circle[0]);
      sendb |= circle[0];
    } else if (indata == '8') {
      cir2[4] = 0;
      cir2[5] = 1;
      circle[1] = crc(cir2, circle[1]);
      sendb |= circle[1];
    } else if (indata == 'a') {
      cir3[4] = 0;
      cir3[5] = 1;
      circle[2] = crc(cir3, circle[2]);
      sendb |= circle[2];
    }
  } else if (count == 1) {
    if (indata == '4') { //PC1 to PC2
      cir1[4] = 1;
      cir1[5] = 0;
      circle[0] = crc(cir1, circle[0]);
      sendb |= circle[0];
    } else if (indata == '8') {
      cir2[4] = 1;
      cir2[5] = 0;
      circle[1] = crc(cir2, circle[1]);
      sendb |= circle[1];
    } else if (indata == 'a') {
      cir3[4] = 1;
      cir3[5] = 0;
      circle[2] = crc(cir3, circle[2]);
      sendb |= circle[2];
    }
  } else if (count == 2) {
    if (indata == '4') { //PC1 to PC2
      cir1[4] = 1;
      cir1[5] = 1;
      circle[0] = crc(cir1, circle[0]);
      sendb |= circle[0];
    } else if (indata == '8') {
      cir2[4] = 1;
      cir2[5] = 1;
      circle[1] = crc(cir2, circle[1]);
      sendb |= circle[1];
    } else if (indata == 'a') {
      cir3[4] = 1;
      cir3[5] = 1;
      circle[2] = crc(cir3, circle[2]);
      sendb |= circle[2];
    }
  }
  //return sendb;

}

long crc(int indata[],long bits) {
  int last = 0;
  int Re[] = {0, 0, 0, 0, 0};
  for (int i = 0; i < 11; i++) {
    Re[0] = Re[1] ^ last;    //1
    Re[1] = Re[2] ;           //0
    Re[2] = Re[3] ^ last;    //1
    Re[3] = Re[4] ;           //0
    Re[4] = indata[i] ^ last;     //1
    last = Re[0];             //1
    //Serial.print(indata[i]);
  }
//   Serial.print(" ");
//  for (int i = 0; i < 5; i++)
//    Serial.print(Re[i]);
//  Serial.println();
  
  for (int i = 0; i < 6; i++) {
    bits |= indata[i] << 12-i ;
  }
  for (int i = 0; i < 5; i++) {
    bits |= Re[i] << 6-i;
  } 
  //Serial.println(bits,BIN); 
  return bits;
}
void sender(){
  for (int k = 0; k < 9; k++) { //00 01 10 11
      int tmp = sendb & 3;
      Serial.print("Dataframe =  ");
      Serial.print(sendb, BIN );
      Serial.print("   tmp =  ");

      Serial.print(tmp );

      if (tmp == 0) {
        for (int sl = 0; sl < 5; sl++) {
          for (int s = 0; s < 4; s++) {
            x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);
            dac.setVoltage(x, false);
            delayMicroseconds(delay0);
          }
        }
      } else if (tmp == 1) {
        for (int sl = 0; sl < 8; sl++) {
          for (int s = 0; s < 4; s++) {
            x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);
            dac.setVoltage(x, false);
            delayMicroseconds(delay1);
          }
        }
      } else if (tmp == 2) {
        for (int sl = 0; sl < 16; sl++) {
          for (int s = 0; s < 4; s++) {
            x = mapX(S_DAC[s] * A[3], -5, 5, 0, 4095);
            dac.setVoltage(x, false);
            delayMicroseconds(delay2);
          }
        }
      } else if (tmp == 3) {
        for (int sl = 0; sl < 32; sl++) {
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

