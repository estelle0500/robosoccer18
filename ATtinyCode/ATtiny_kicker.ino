#define CMD_PIN 1
#define CHARGE 0
#define DISCHARGE 4

int chargingTime = 5000;
bool needCharging = true;
bool isCharging = false;
unsigned long chargingStartTime = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(CMD_PIN, INPUT);
  pinMode(CHARGE,OUTPUT);
  pinMode(DISCHARGE,OUTPUT);
}

void loop() {
  if (isCharging)
    if (millis()-chargingStartTime > chargingTime)
    {
      isCharging = false;
      digitalWrite(CHARGE,LOW);
    }
  if (needCharging)
  {
    needCharging = false;
    isCharging = true;
    digitalWrite(CHARGE,HIGH);
    chargingStartTime = millis();
  }

  // as of now, just don't care whether the charging is completed
  if (digitalRead(CMD_PIN)==HIGH)
  {
    digitalWrite(CHARGE,LOW);
    delay(10);
    digitalWrite(DISCHARGE,HIGH);
    delay(1000);
    digitalWrite(DISCHARGE,LOW);
    needCharging = true;
    isCharging = false;
  }
  delay(10);
}
