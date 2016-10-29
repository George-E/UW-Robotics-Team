int tempoIndex = 4;
int tempos[] = {0, 60, 80, 100, 120};

void setup() {
  Serial.begin(9600);
  pinMode(A8, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println(analogRead(A8));
  if (buttonPressed()) {
    iterateTempo();
  }
}

bool buttonPressed() {
  analogRead(A8);
  if (analogRead(A8) > 900)
    return true;
  return false;
}

void iterateTempo() {
  tempoIndex++;
  tempoIndex %= 5;
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

