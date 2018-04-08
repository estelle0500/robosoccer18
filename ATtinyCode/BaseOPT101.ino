int threshold = 300; // corresponding to about 1.5 V
void setup() {
  // put your setup code here, to run once:
  pinMode(A1,INPUT);
  pinMode(1,OUTPUT);
  digitalWrite(1,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(A1) > threshold)
  {
    digitalWrite(1,HIGH);
    // how long this thing is depends on the whether interrupt is used in the programme.
    delay(200);
    digitalWrite(1,LOW);
  }
}
