#define PIR_SENSOR 2  
#define LED 13      

void setup() {
    pinMode(PIR_SENSOR, INPUT);
    pinMode(LED, OUTPUT);
    Serial.begin(9600);
    Serial.println("PIR Sensor System Initialized...");
}

void loop() {
    int motion = digitalRead(PIR_SENSOR);
    
    if (motion == HIGH) {
        digitalWrite(LED, HIGH); 
        Serial.println("Motion Detected! LED is ON");
        delay(1000); 
    } else {
        digitalWrite(LED, LOW); 
        Serial.println("No Motion. LED is OFF");
        delay(1000);  
    }
}
