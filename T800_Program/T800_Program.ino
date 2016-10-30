
#include <TimerOne.h>
double fqcy[] = {0,261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
//<<<<<<< HEAD
//float timePerStep[] = {0,7.5f,6.7f, 6.0f, 5.6f, 5.0f, 4.5f,4.0f,3.7f};//values for the timer (in microseconds)
int numNotes = 8;
int noteColourRanges[] = {170,195,225,265,305,345,900,900,900};
//=======

//>>>>>>> origin/master


int sinVals[250] = {0};
int test = 0;
int note = 3;

#define RSpeedPin  9
#define RDirPin  11
#define LSpeedPin  10
#define LDirPin  12
#define lineGrayscaleSensor  A0
#define musicGrayscaleSensor  A1
int Speed = 10;
int edge = 500;

int noteTimerCount = 0;
//IntervalTimer noteTimer;

void setup() {
  for (int i=0; i<250;i++) {
    sinVals[i] = (int) (127.5+127.5*sin(3.141592654*2*(double)i/250) +0.5);//+.5 so rounding occurs
    }
  Serial.begin(9600);

    pinMode(13,OUTPUT);
  digitalWrite(13, LOW);//so we know its on
  
  for(int i = 0; i < 8; i++) pinMode(i,OUTPUT);
  
  //noteTimer.begin(playNote,0.1f);
  Timer1.initialize(40);
  Timer1.attachInterrupt(playNote);
 Timer1.start();
  
}


void loop() {
  //followLine();
  /*int bin[8] = {0};
  for (int n=0; n<256;n++) {
    convertToBin(n,bin);
    for(int i = 0; i < 8; i++){
     // Serial.print(bin[i]);
      if(bin[8-1-i] == 1) digitalWrite(i,HIGH);//added -1 b/c we want 0-7 not 1-8
      else digitalWrite(i,LOW); 
    }
    delayMicroseconds(10);
  }
  for (int n=255; n>=0;n--) {
    convertToBin(n,bin);
    for(int i = 0; i < 8; i++){
     // Serial.print(bin[i]);
      if(bin[8-1-i] == 1) digitalWrite(i,HIGH);//added -1 b/c we want 0-7 not 1-8
      else digitalWrite(i,LOW); 
    }
    delayMicroseconds(10);
  }*/
}


// Plays a note in a set octave
void playNote (void) {//where is note var used, and does it know not to play anything if note == 0?
  
  int bin[8] = {0};
  if(note != 0){
    //int num = (int)(127.5+127.5*sin(2*(3.141592654)*fqcy[note]*0.0000001*noteTimerCount) +0.5);
    int num = sinVals[noteTimerCount];
    Serial.println(sinVals[noteTimerCount]);
    convertToBin(num,bin); //added the plus 0.5 so that the cast to int will essentially follow traditional rounding standards 
    for(int i = 0; i < 8; i++){
     // Serial.print(bin[i]);
      if(bin[7-i] == 1) digitalWrite(i,HIGH);//changed 8 to 7 b/c we want 0-7 not 1-8
      else digitalWrite(i,LOW); 
    }
    //Serial.println();
  }

  else {
    for(int i = 0; i < 8; i++){
      digitalWrite(i,LOW);
    }
  }
noteTimerCount++;
                        //the timer will tick every time per level, and every tick will increase the value of a counter variable
if (noteTimerCount == 250) noteTimerCount = 0; //after one second, restarts
}

void colourToNote () {
  //code here
}

void followLine() {
  //assumed black is higher, have to check that
  //values will have to be try tested of course
  int lineReading = readGrayscaleSensor(lineGrayscaleSensor);
  int difference = lineReading- edge;
  int leftMultiplier =0, rightMultiplier = 0;
  if (difference > 0) {
    leftMultiplier -= abs(difference)/100.0;
  } else {
    rightMultiplier -= abs(difference)/100.0;
  }
  forward(leftMultiplier,rightMultiplier);
}

//<<<<<<< HEAD
void updateNote() {
  //should add some logic that weeds out outlier data (ie rapid changes in note)
  int musicReading = readGrayscaleSensor(musicGrayscaleSensor);
  //Serial.println(musicReading);
  int tempNote = 0, index =0 ;
  while (musicReading > noteColourRanges[index]) {
    tempNote++;
    index++;
    if (index == numNotes) {
      tempNote = 9; //undefined if below lowest note range start or higher than highest note range end
      break;
    }
  }
  note = tempNote;
  Serial.println(note);
}

//=======
//>>>>>>> origin/master
void convertToBin(int num, int a[]) {
  for (int i = 7; i >= 0; i--) {
    a[i] = num % 2;
    num /= 2;
  }
}


int readGrayscaleSensor(int pin) {
  return analogRead(pin);
}

void stop() {
  digitalWrite( RSpeedPin, LOW );
  digitalWrite( RDirPin, LOW );
  digitalWrite( LSpeedPin, LOW );
  digitalWrite( LDirPin, LOW );
}


void forward(double leftMultiplier, double rightMultiplier)
{
  digitalWrite(RDirPin, HIGH);
  analogWrite(RSpeedPin, Speed*rightMultiplier);
  digitalWrite(LDirPin, HIGH);
  analogWrite(LSpeedPin, Speed*leftMultiplier);
}

void backward(double leftMultiplier, double rightMultiplier)
{
  digitalWrite(RDirPin, LOW);
  analogWrite(RSpeedPin, Speed*rightMultiplier);
  digitalWrite(LDirPin, LOW);
  analogWrite(LSpeedPin, Speed*leftMultiplier);
}


