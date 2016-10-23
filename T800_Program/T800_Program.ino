void drive (int s, int dir);
void playNote ();
void colourToNote();
void followLine();
void convertToBin(int num, int a[]);

double fqcy[] = {0,261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
//float timePerStep[] = {0,7.5f,6.7f, 6.0f, 5.6f, 5.0f, 4.5f,4.0f,3.7f};//values for the timer (in microseconds)
int noteTimerCount = 0;
IntervalTimer noteTimer;
//int timerCount = 0;
//bool countingUp = true;
int note = 0; //from C to C (where 0 is no note)
void setup() {
  
  Serial.begin(9600);
  noteTimer.begin(playNote,0.1f);

  for(int i = 0; i < 8; i++) pinMode(i,OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:

}

void drive (int s, int dir) {
  //DRIVING CODE HERE
}

// Plays a note in a set octave
void playNote (void) {
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

