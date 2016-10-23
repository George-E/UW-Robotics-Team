void drive (int s, int dir);
void playNote ();
void colourToNote();
void followLine();
void convertToBin(int num, int a[]);

double fqcy[] = {0,261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
IntervalTimer noteTimer;
int timerCount = 0;
bool countingUp = true;
int test = 0;
int note = 0;
void setup() {
  Serial.begin(9600);
  noteTimer.begin(playNote,0.5);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void drive (int s, int dir) {
  //DRIVING CODE HERE
}

// Plays a note in a set octave
void playNote (void) {
  /*Takes an int value from 0 - 7 and outputs that note based on the frequency */

  double period = 1.0 / fqcy[note];
  double timePerLevel = period / (2*255); //sets the tixk time (in seconds) as the period, divided by the amount of steps required in the period
  countingUp = true;
  timerCount = 0;
  noteTimer.begin(count, 100000); //multiplied by 1000000 as timer takes values in miliseconds not seconds

                        //the timer will tick every time per level, and every tick will increase the value of a counter variable

}

void colourToNote () {
  //code here
}

void followLine() {

}

void convertToBin(int num, int a[]) {
  for (int i = 7; i >= 0; i--) {
    a[i] = num % 2;
    num /= 2;
  }
}
void count(){
  if (timerCount == 255) countingUp = false;
  if(timerCount == 0) countingUp = true;
  if (countingUp) timerCount +=1;
  if (!countingUp) timerCount -=1;

  int bin[8] = {0};
  convertToBin(timerCount, bin);
  for(int i = 0; i < 8; i++){
    Serial.print(bin[i]);
  }
  Serial.print(" ");
  Serial.print(timerCount);
  Serial.print("\n");
}

