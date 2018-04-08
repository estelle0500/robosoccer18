// Needs to be calibrated
#define THRE1 420 // the one on the left
#define THRE2 420 // the one on the right

void setup() {
  // put your setup code here, to run once:
  //pinMode(A0,INPUT);
  pinMode(A3,INPUT);
  pinMode(A2,INPUT);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
}

void loop() {
  // checking the one on the left
  if (analogRead(A3)>THRE1 || analogRead(A2)>THRE2)
  {
    digitalWrite(2,HIGH);
    delay(200);
    digitalWrite(2, LOW);
  }
}
