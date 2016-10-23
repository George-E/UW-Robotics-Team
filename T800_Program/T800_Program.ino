void drive (int s, int dir);
void playNote ();
void colourToNote();
void followLine();
void convertToBin(int num, int a[]);

double fqcy[] = {0,261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

int noteTimerCount = 0;
IntervalTimer noteTimer;


int test = 0;
int note = 0;

#define RSpeedPin  11
#define RDirPin  8
#define LSpeedPin  10
#define LDirPin  12
#define lineGrayscaleSensor  0
#define musicGrayscaleSensor  2
int Speed = 10;
int edge = 500;


void setup() {
  
  Serial.begin(9600);
  noteTimer.begin(playNote,0.1f);

  for(int i = 0; i < 8; i++) pinMode(i,OUTPUT);
  

}

void loop() {
  followLine();
}

void drive (int s, int dir) {
  //DRIVING CODE HERE
}

// Plays a note in a set octave
void playNote (void) {
  noteTimerCount++;

  int bin[8] = {0};
  if(note != 0){
    convertToBin((int)(127.5+127.5*sin(2*(3.141592654)*fqcy[note]*0.0000001*noteTimerCount) +0.5),bin); //added the plus 0.5 so that the cast to int will essentially follow traditional rounding standards 
    for(int i = 0; i < 8; i++){
      if(bin[i] == 1) digitalWrite(i,HIGH);
      else digitalWrite(i,LOW); 
    }
  }

  else {
    for(int i = 0; i < 8; i++){
      digitalWrite(i,LOW);
    }
  }

                        //the timer will tick every time per level, and every tick will increase the value of a counter variable
if (noteTimerCount == 10000000) noteTimerCount = 0; //after one second, restarts
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


