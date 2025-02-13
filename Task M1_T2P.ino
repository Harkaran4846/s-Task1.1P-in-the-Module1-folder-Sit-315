#define PIR_SENSOR 2  
#define LED 9       
volatile bool motionDetected = false; 

void motionISR() {
    motionDetected = true; 
}

void setup() {
    pinMode(PIR_SENSOR, INPUT);
    pinMode(LED, OUTPUT);
    Serial.begin(9600);
    Serial.println("PIR Sensor System Initialized...");
    attachInterrupt(digitalPinToInterrupt(PIR_SENSOR), motionISR, RISING); 
}

void loop() {
    if (motionDetected) {
        digitalWrite(LED, HIGH); 
        Serial.println("Motion Detected! LED is ON");
        delay(1000);
        digitalWrite(LED, LOW);
        Serial.println("LED is OFF");
        motionDetected = false; 
    }
}
