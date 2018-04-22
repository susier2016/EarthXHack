//init
//sourcehelp: http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/
#include <pt.h>
#define timeThreshold 15
static struct pt pt1, pt2, pt3;
int ultrasonic_trigPin = 9, ultrasonic_echoPin = 10;
int ledPin = 11;
int tickEvent;
int runCount = 0;
double distanceA[2], durationA[5];

//static int runSensor(struct pt *pt, int interval);
//static int outputData(struct pt *pt, int interval);
//static int clearData(struct pt *pt, int interval);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(4800);
  //ultrasonic sensor and led setup
  pinMode(ultrasonic_trigPin, OUTPUT);
  pinMode(ultrasonic_echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  //init protothread vars
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  PT_INIT(&pt3);
}

void loop() {
  //clear dataArrays
  //clearData(&pt3, 250);
  //read data from sensor and get an avg
  runSensor(&pt1, 100);
  //output avg through serial
  outputData(&pt2, 200);
}

/**
   runSensor reads data from the ultrasonic sensor and accumulates and average to commit to com
   param: struct pt *pt, protothread object
   param: int interval time between runs
*/
static int runSensor(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  static int count = 0;
  double duration;
  double sum = 0, average;

  PT_BEGIN(pt);
  while (1) {
    //evaluate whether thread should execute
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();

    for (int i = 0; i < 5; i++) {
      //run ultrasonic
      digitalWrite(ultrasonic_trigPin, LOW);
      delayMicroseconds(10);
      digitalWrite(ultrasonic_trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(ultrasonic_trigPin, LOW);
      //get duration
      duration = pulseIn(ultrasonic_echoPin, HIGH);
    }
    //get avg distance reading
    //file into array
    durationA[count] = duration;
    count = (count + 1) % 5;
    //get sum of duration then avg
    for (int i = 0; i < 5; i++) {
      sum += durationA[i];
    }
    for (int i = 0; i < 5; i++) {
      durationA[i] = 0;
    }
    average = sum / 5;
    sum = 0;
    //conv into distance and file into array
    distanceA[runCount] = (average / 58) * 23 / 21;
    runCount = (runCount + 1) % 2;
  }
  PT_END(pt);
}
/**
   outputData takes committed average and prints to comm in some format (every 5 runSensor calls)
   param: struct pt *pt, protothread object
   param: int interval time between runs
*/
static int outputData(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;

  PT_BEGIN(pt);
  while (1) {
    //evaluate whether thread should execute
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();

    //check if there has been a big change in distance between new and old dist values
    int oldV = 0;//runCount;
    int newV = 1;//abs(runCount - 1) % 2;
    double distChange;
    static int changeCtr = 0;
    //if distance change reading is larger than 3cm
    //turn on light && send data to serial/wifi? else do otherwise
    /**Test output
    Serial.print("at dist[0]: ");
    Serial.print(distanceA[0]);
    Serial.print("at dist[1]: ");
    Serial.println(distanceA[1]);
    **/
    distChange = abs(distanceA[oldV] - distanceA[newV]);
    if (distChange > 0.18) {
      digitalWrite(ledPin, HIGH);
      Serial.print("=");
      Serial.print((double)timestamp/100, 4);
      Serial.println("s ON");   
      changeCtr = 0;
    }
    else { //(abs(distanceA[oldV] - distanceA[newV]) <= 3.00)
      //delay for some time to make sure object is asleep
      changeCtr ++;
      if(changeCtr > 5){
        digitalWrite(ledPin, LOW);
        if(changeCtr > 5 && changeCtr < 8){
          Serial.print("=");
          Serial.print((double)timestamp/100, 4);      
          Serial.println("s OFF");    
        }
      }
    }
    //Serial.print(" Distance Change: ");
    //Serial.println(distChange, 4);
  }
  PT_END(pt);
}

/**
   clearData reinitializes and empties all arrays
   param: struct pt *pt, protothread object
   param: int interval time between runs
*/
static int clearData(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;

  PT_BEGIN(pt);
  while (1) {
    //evaluate whether thread should execute
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();

    //clear arrays
    for (int i = 0; i < 5; i++) {
      durationA[i] = 0;
    }
    for (int i = 0; i < 2; i++) {
      distanceA[i] = 0;
    }
  }
  PT_END(pt);
}

