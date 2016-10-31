
#include <TimerOne.h>

#define waveRes  50
#define RSpeedPin  10
#define RDirPin  12
#define LSpeedPin  9
#define LDirPin  11
#define lineGrayscaleSensor  A0
#define musicGrayscaleSensor  A1
#define tempoButton  A8
#define REnocderA  16
#define REnocderB  17
#define LEnocderA  18
#define LEnocderB  19

int tempos[] = {0, 60, 80, 100, 120};
int tempoIndex = 0;

int LVoltStart[] = {255, 180, 167, 160, 156};
int RVoltStart[] = {0, 65, 73, 76, 81};

double fqcy[] = {0, 293.66, 329.63, 369.99,392.00,440.00,493.88,554.37,587.33};

int numNotes = 8;
int noteColourRanges[] = {170, 195, 225, 265, 305, 345, 380, 420};

char sinBinVals[waveRes] = {0};
int test = 0;
int note = 8;

int edge = 207;//DO NOT CHANGE (measured value for edge)

int noteTimerCount = 0;


int lastR = 1;
int lastL = 1;
double ticksRlast  = 0;
double ticksLlast  = 0;
double ticksR  = 0;
double ticksL  = 0;

int LVolts = 160;
int RVolts = 75;
double desiredSpeed = 0;//tempoPBMtoSpeedCMS(60); //cm/s
long milliLast = millis();


/*int melody1[] = {
6,5,4,5,6,6,6,5,5,5,6,6,7,6,5,4,5,6,6,6,6,6,5,5,4,3,2
};*/


void setup() {
  for (int i = 0; i < waveRes; i++) {
    sinBinVals[i] = (int) (127.5 + 127.5 * sin(PI * 2 * (double)i / waveRes) + 0.5); //+.5 so rounding occurs
  }

  Serial.begin(9600);

  pinMode(13, OUTPUT);
  // digitalWrite(13, HIGH);//so we know its on
  pinMode(tempoButton, INPUT);


  for (int i = 0; i < 8; i++) pinMode(i, OUTPUT);
  pinMode(RSpeedPin, OUTPUT);
  pinMode(RDirPin, OUTPUT);
  pinMode(LSpeedPin, OUTPUT);
  pinMode(LDirPin, OUTPUT);

  Timer1.initialize(1000);//time will be set in updateTimerWait
  Timer1.attachInterrupt(playNote);
  Timer1.start();
  //For testing sounds
  /*for (int i=0; i<=sizeof(melody1)/sizeof(int);i++)
  {
    note = melody1[i];
    updateTimerWait();
    delay(500);
  }*/
  updateTimerWait();

}

void updateTimerWait() {
  if (fqcy[note] == 0) {
    Timer1.stop();
    return;
  }
  Timer1.setPeriod( (1 / fqcy[note]) * 1000000 / waveRes);
  Timer1.start();
}

double tickIncrease(bool isRight, int pinA, int pinB) {
  int R_A = analogToDigitalRead(analogRead(pinA));
  int R_B = analogToDigitalRead(analogRead(pinB));
  int present = 0;
  if (R_A == 0 && R_B == 0) {
    present = (isRight) ? 1 : 4;
  } else if (R_A == 0 && R_B == 1) {
    present = (isRight) ? 2 : 3;
  } else if (R_A == 1 && R_B == 1) {
    present = (isRight) ? 3 : 2;
  } else if (R_A == 1 && R_B == 0) {
    present = (isRight) ? 4 : 1;
  }

  if (isRight) {
    if (lastR < present) {
      lastR = present;
      return (present - lastR) / 2.0;
    } else if (present < lastR) {
      lastR = present;
      return (present + 4 - lastR) / 2.0;
    }
  } else {
    if (lastL < present) {
      lastL = present;
      return (present - lastL) / 2.0;
    } else if (present < lastL) {
      lastL = present;
      return (present + 4 - lastL) / 2.0;
    }
  }
  return 0;
}


double tempoPBMtoSpeedCMS(int tempo) {
  return ((tempo * 5.0) / 60.0);
}

void loop() {
  updateNote();
  followLine();
  if (buttonPressed()) {
    iterateTempo();
  }
}


bool buttonPressed() {
  analogRead(tempoButton);
  if (analogRead(tempoButton) > 900)
    return true;
  return false;
}

void iterateTempo() {
  stop();
  tempoIndex++;
  tempoIndex %= 5;
  LVolts = LVoltStart[tempoIndex];
  RVolts = RVoltStart[tempoIndex];
  desiredSpeed = tempoPBMtoSpeedCMS(tempos[tempoIndex]);
  if (tempoIndex==0) {
    delay(2000);
    return;
  }
  for (int i=0; i<tempos[tempoIndex]/10; i++) {
    digitalWrite(13, HIGH);
    delay(300);
    digitalWrite(13, LOW);
    delay(200);
  }
}


double ticksToDist(double ticks) {
  return 2 * PI * 3.25 * ticks / 1920.0;
}
int analogToDigitalRead(int an) {
  return an > 500;
}

// Plays a note in a set octave
void playNote (void) {//where is note var used, and does it know not to play anything if note == 0?
  if (tempoIndex != 0) {
    char num = sinBinVals[noteTimerCount];
    //Serial.println(sinVals[noteTimerCount]);
    for (int i = 0; i < 8; i++) {
      if ((num >> i) & 0b1) digitalWrite(i, HIGH); //changed 8 to 7 b/c we want 0-7 not 1-8
      else digitalWrite(i, LOW);
    }
  }
  noteTimerCount++;
  //the timer will tick every time per level, and every tick will increase the value of a counter variable
  if (noteTimerCount == waveRes) noteTimerCount = 0; //after one second, restarts
}

void updateNote() {
  //should add some logic that weeds out outlier data (ie rapid changes in note)
  int musicReading = readGrayscaleSensor(musicGrayscaleSensor);
  Serial.print(musicReading);
  int tempNote = 0, index = 0 ;
  while (musicReading > noteColourRanges[index]) {
    tempNote++;
    index++;
    if (index == numNotes) {
      break;
    }
  }
  Serial.print("\t");
  Serial.println(note);
  if (tempNote != note) {
    note = tempNote;
    updateTimerWait();
  }
}

int readGrayscaleSensor(int pin) {
  return analogRead(pin);
}


void followLine() {
  if (tempoIndex==0) {
    stop();
    return;
  }
  int lineReading = readGrayscaleSensor(lineGrayscaleSensor);
  //Serial.println(lineReading);
  int difference = lineReading - edge;
  double leftMultiplier = 1, rightMultiplier = 1;
  if (difference > 5) {
    rightMultiplier -= abs(difference) / 100.0;
    //Serial.println("slightlyLEFT");
  } else if (difference < -5) {
    leftMultiplier += abs(difference) / 100.0;
    //Serial.println("slightlyRIGHT");
  } else {
    updateSpeed();
  }
  if(leftMultiplier > 1.2) leftMultiplier = 1.2;
  if(rightMultiplier < 0.8) rightMultiplier = 0.8;
  //Serial.println(abs(difference) / 100.0);
  forward(leftMultiplier, rightMultiplier);
}

void forward(double leftMultiplier, double rightMultiplier)
{
  digitalWrite(RDirPin, LOW);
  analogWrite(RSpeedPin, RVolts * rightMultiplier);
  digitalWrite(LDirPin, HIGH);
  analogWrite(LSpeedPin, LVolts * leftMultiplier);
}

void stop()
{
  
  digitalWrite(RDirPin, LOW);
  digitalWrite(LDirPin, LOW);
  digitalWrite(RSpeedPin, LOW);
  digitalWrite(LSpeedPin, LOW);
}

void updateSpeed() {
  ticksR += tickIncrease(true, REnocderA, REnocderB);
  ticksL += tickIncrease(false, LEnocderA, LEnocderB);
  
  long milliNow = millis();

  if (milliNow - milliLast > 100) {
    double LSpeedNow = (ticksToDist(ticksLlast) - ticksToDist(ticksL)) / ((milliLast - milliNow) / 1000.0);
    double RSpeedNow = (ticksToDist(ticksRlast) - ticksToDist(ticksR)) / ((milliLast - milliNow) / 1000.0);

   /* Serial.print(LVolts);
    Serial.print("\t");
    Serial.print(LSpeedNow);
    Serial.print("\t");
    Serial.print(RVolts);
    Serial.print("\t");
    Serial.println(RSpeedNow);*/

    if (desiredSpeed - LSpeedNow > 0.2)
      LVolts -= 1;
    else if (desiredSpeed - LSpeedNow < -0.2)
      LVolts += 1;

    if (desiredSpeed - RSpeedNow > 0.2)
      RVolts += 1;
    else if (desiredSpeed - RSpeedNow < -0.2)
      RVolts -= 1;

    if (RVolts > 255)
      RVolts = 255;
    if (LVolts < 0)
      LVolts = 0;

    ticksLlast = ticksL;
    ticksRlast = ticksR;
    milliLast = milliNow;
  }

}

