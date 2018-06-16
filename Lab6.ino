//Lab6 4 sampling
int zeta[] = {0,90,180,270};
float s[] = {sin(0),sin(90),sin(180),sin(270)};
uint16_t pwmDuty[] = {127,255,127,0};
#define pin_number 3
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pin_number,OUTPUT);
  for(int i=0;i<4;i++){
  Serial.print(zeta[i]);
  Serial.print(" ");
  Serial.print(s[i]);
  Serial.print(" ");
  Serial.print(pwmDuty[i]);
  Serial.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<4;i++){
  analogWrite(pin_number,pwmDuty[i]);
  delayMicroseconds(400);
  }
  }
