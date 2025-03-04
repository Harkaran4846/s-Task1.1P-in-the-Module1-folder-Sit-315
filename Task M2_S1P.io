const byte LED_PIN = 13;
const byte POT_PIN = A4;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
      
  Serial.begin(9600);
  
  double timerFrequency = readPotentiometer();
  startTimer(timerFrequency);
}

void loop() {
  double timerFrequency = readPotentiometer();
  startTimer(timerFrequency);
  delay(500); 
}

double readPotentiometer() {
  int potValue = analogRead(POT_PIN);
  double minFreq = 0.1;
  double maxFreq = 5.0;  
  return minFreq + (potValue / 1023.0) * (maxFreq - minFreq);
}

void startTimer(double timerFrequency) {
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;

  long timerClock = 16000000;  
  int prescaler = 1024;
  
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);

  OCR1A = (timerClock / (prescaler * timerFrequency)) - 1;

  TIMSK1 |= (1 << OCIE1A);

  interrupts();
}

ISR(TIMER1_COMPA_vect) {
   digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
