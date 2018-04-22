//photoresistor calibration
int lightPin = A0;
int ledPin = 11;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

double r = 0;
void loop() {
  // put your main code here, to run repeatedly:
  r = analogRead(lightPin);
  Serial.println(r/2);
  //analogWrite(ledPin, analogRead(lightPin));
  delay(10);
}

