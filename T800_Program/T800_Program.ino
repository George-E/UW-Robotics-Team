void drive (int s, int dir);
void playNote ();
void colourToNote();
void followLine();
void convertToBin(int num, int a[]);

double fqcy[] = {0,261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
//float timePerStep[] = {0,7.5f,6.7f, 6.0f, 5.6f, 5.0f, 4.5f,4.0f,3.7f};//values for the timer (in microseconds)
int numNotes = 8;
int noteColourRanges[] = {170,195,225,265,305,345,900,900,900};
int noteTimerCount = 0;
IntervalTimer noteTimer;

//int timerCount = 0;
//bool countingUp = true;

int timerCount = 0;
bool countingUp = true;

int test = 0;

#define RSpeedPin  9
#define RDirPin  11
#define LSpeedPin  10
#define LDirPin  12
#define lineGrayscaleSensor  A0
#define musicGrayscaleSensor  A2
int Speed = 10;
int edge = 500;


int note = 0; //from C to C (where 0 is no note)

void setup() {
  
  Serial.begin(9600);
  noteTimer.begin(playNote,0.1f);

  for(int i = 0; i < 8; i++) pinMode(i,OUTPUT);
  

}

void loop() {
  followLine();
  updateNote();
}

// Plays a note in a set octave
void playNote (void) {//where is note var used, and does it know not to play anything if note == 0?
  noteTimerCount++;

  int bin[8] = {0};
  convertToBin((int)(127.5+127.5*sin(2*(3.141592654)*fqcy[note]*0.0000001*noteTimerCount) +0.5),bin); //added the plus 0.5 so that the cast to int will essentially follow traditional rounding standards 
  for(int i = 0; i < 8; i++){
    if(bin[i] == 1) digitalWrite(i,HIGH);
    else digitalWrite(i,LOW); 
  }


                        //the timer will tick every time per level, and every tick will increase the value of a counter variable
if (noteTimerCount == 10000000) noteTimerCount = 0; //after one second, restarts
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

void convertToBin(int num, int a[]) {
  for (int i = 7; i >= 0; i--) {
    a[i] = num % 2;
    num /= 2;
  }
}
/*void count(){
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

}*/

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



