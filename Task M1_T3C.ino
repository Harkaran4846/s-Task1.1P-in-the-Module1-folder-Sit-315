#define PIR_PIN 2         
#define TRIG_PIN 6       
#define ECHO_PIN 7        
#define LED_PIN 9         

void setup() {
    pinMode(PIR_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
}

void loop() {
   
    if (digitalRead(PIR_PIN) == HIGH) {
        Serial.println("Motion Detected by PIR!");
    } else {
        Serial.println("No Motion Detected");
    }

    
    int distance = getUltrasonicDistance();
    if (distance > 0 && distance <= 100) { 
        Serial.print("Interrupt Triggered: Object detected by Ultrasonic Sensor!");
        Serial.print(distance);
        Serial.println(" cm");
        blinkLED();
    }

    delay(500);
}

int getUltrasonicDistance() {
    long duration;
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}

void blinkLED() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(300);
        digitalWrite(LED_PIN, LOW);
        delay(300);
    }
}
