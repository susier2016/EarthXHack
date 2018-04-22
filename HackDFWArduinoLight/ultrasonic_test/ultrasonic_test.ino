//sources: http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/
//test for ultrasonic sensor
int ultrasonic_trigPin = 9, ultrasonic_echoPin = 10;
int ledPin = 11;
double readVal = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ultrasonic_trigPin, OUTPUT);
  pinMode(ultrasonic_echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

int toggle = 0, count = 0, sum = 0, average;
double duration, distance;
//double lowRead = 370, highRead = 370;
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ultrasonic_trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(ultrasonic_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonic_trigPin, LOW);
  duration = pulseIn(ultrasonic_echoPin, HIGH);
  

  //distance = duration * 340 / 2 * 100; //centimeters
  
  sum += duration;
  if(count == 4){
    average = sum/5;
    //Serial.print("average duration signal: ");
    //Serial.print(average, 4);
    //Serial.println();
    sum = 0;
    distance = (average/58)* 23/21;
    //distance in cm
    
    if(distance<4)
      Serial.print("OOR");
    else{
      Serial.print("resultant distance: ");
      Serial.print(distance, 4);
      Serial.print(" cm");
    }
    Serial.println();
  }
  count = (count + 1)%5;

  //toggle LED
  if(toggle)
    digitalWrite(ledPin, LOW);
  else
    digitalWrite(ledPin, HIGH);
  toggle = (toggle + 1) % 2;
}
