void drive (int s, int dir);
void playNote ();
void colourToNote();
void followLine();
void convertToBin(int num, int a[]);

double fqcy[] = {0,261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
float timePerStep[] = {0,7.5f,6.7f, 6.0f, 5.6f, 5.0f, 4.5f,4.0f,3.7f};//values for the timer (in microseconds)
int noteTimerCount = 0;
IntervalTimer noteTimer;
int timerCount = 0;
bool countingUp = true;
<<<<<<< HEAD
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

=======
int note = 0; //from C to C (where 0 is no note)
>>>>>>> origin/master
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

  if((noteTimerCount*10)%((int)(timePerStep[note]*10)) == 0) count(); //once the timer is a whole myltiple of the amount of time per step, increases the voltage by 1


                        //the timer will tick every time per level, and every tick will increase the value of a counter variable
if (noteTimerCount == 3123540) noteTimerCount = 0; //3123540 is the LCM of all of the time per step variables so this is simply so that notTimerCount wont overflow and skip a repetition
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
void count(){
  if (timerCount == 255) countingUp = false;
  else if(timerCount == 0) countingUp = true;
  if (countingUp) timerCount +=1;
  else if (!countingUp) timerCount -=1;

  int bin[8] = {0};
  convertToBin(timerCount, bin); //converts the value of timercount to  binary
  for(int i = 0; i < 8; i++){ //each element in the array from 0-7 corresponds to a didital output pin on the teensy. If the bit is on, the teensy outputs a voltage on that bit, wich, wth the R2R bridge, results in a wave form as the bits count up and down from 255
    //pin 0 corresponds with the last bit of binary and pin 8 with the first bit
    if(bin[i] == 1)digitalWrite(i,HIGH);
    else digitalWrite(i,LOW);
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


