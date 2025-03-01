#include <DHT.h>
#include <PinChangeInterrupt.h>

#define PIR_PIN 2        
#define TRIG_PIN 6      
#define ECHO_PIN 7       
#define LED_PIN 9       
#define DHT_PIN 4        
#define DHT_TYPE DHT22   

DHT dht(DHT_PIN, DHT_TYPE);

volatile bool readDHTFlag = false;

void setup() {
    pinMode(PIR_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
    dht.begin();

    // Attach Pin Change Interrupt for Ultrasonic Echo
    attachPCINT(digitalPinToPCINT(ECHO_PIN), ultrasonicISR, CHANGE);

    // Configure Timer1 to trigger every 15 seconds
    cli(); // Disable interrupts
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // Prescaler 1024
    OCR1A = 46874;  // (16MHz / (1024 * (1/15Hz))) - 1 = 46874 (15 sec interval)
    TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
    sei(); // Enable interrupts
}

void loop() {
    // **PIR Sensor Continuously Checking Motion**
    if (digitalRead(PIR_PIN) == HIGH) {
        Serial.println("Motion Detected by PIR!");
    } else {
        Serial.println("No Motion Detected");
    }

    // **Ultrasonic Sensor Condition**
    int distance = getUltrasonicDistance();
    if (distance > 0 && distance <= 50) {
        Serial.println("Interrupt Triggered: Object detected by Ultrasonic Sensor!");
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }

    // **DHT22 Read Every 15 Seconds**
    if (readDHTFlag) {
        readDHTSensor();
        blinkLED();
        readDHTFlag = false;
    }

    delay(100);
}

// **Ultrasonic Distance Calculation**
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

// **LED Blinking Function for DHT22 Event**
void blinkLED() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(300);
        digitalWrite(LED_PIN, LOW);
        delay(300);
    }
}

// **Ultrasonic Interrupt Service Routine**
void ultrasonicISR() {
    int distance = getUltrasonicDistance();
    if (distance > 0 && distance <= 50) {
        Serial.println("Interrupt Triggered: Object detected by Ultrasonic Sensor!");
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
}

// **Timer Interrupt Service Routine (fires every 15 sec)**
ISR(TIMER1_COMPA_vect) {
    readDHTFlag = true;
}

// **Function to Read DHT22 Sensor**
void readDHTSensor() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
}
