//CRC
int data[] = {1, 1, 0, 0,  0, 0,  0, 0, 0, 0, 0}; //data
long d = 0x0;
//divisor   110101
//remainder 11001
//normal   00000
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  crc(data,d);
}
void crc(int indata[],long bits) {
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
  Serial.println(bits,BIN); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
