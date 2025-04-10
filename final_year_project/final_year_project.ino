#include <SoftwareSerial.h>
SoftwareSerial BTserial(0, 1);  // RX | TX

// DHT11 Sensor for temperature and humidity
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 2
int humid_limit;
int temp_limit;


// LDR to measure light falling on it
int ldr = A4;
int light_value;
int light_limit;


// Soil Moisture Sensor to check ammount of water in soil
int soil_moisture = A5;
int moisture_value;
int moisture_limit;


// Gas Sensors
// MQ135 Air Quality Sensor
int mq135 = A2;
int mq135_value;

// MQ7 carbon mono-oxide(CO)
int mq7 = A1;
int mq7_value;

int mq4 = A0;
int mq4_value;

void setup() {
  Serial.begin(9600);
  BTserial.begin(9600);

  pinMode(mq135, INPUT); 
  pinMode(mq4, INPUT);
  pinMode(mq7, INPUT);


}

void loop() {
  DHT.read(DHT11_PIN);
  light_value = (map(analogRead(ldr), 1024, 0, 25, 100));
  moisture_value = (map(analogRead(soil_moisture), 380, 950, 100, 0));
  mq135_value = analogRead(mq135);
  mq7_value = analogRead(mq7);
  mq4_value = analogRead(mq4);

  Serial.print(F("----------------------"));
    Serial.println();
    Serial.print(F("Humidity : "));
    Serial.print(DHT.humidity);
    Serial.println();
    Serial.print(F("Temperature : "));
    Serial.print(DHT.temperature);
    Serial.println();
    Serial.println(F("----------------------"));
    Serial.print(F("Sunlight : "));
    Serial.print(light_value);
    Serial.println();
    Serial.println(F("----------------------"));
    Serial.print(F("Soil Moisture : "));
    Serial.print(moisture_value);
    Serial.println();
    Serial.println(F("----------------------"));
    Serial.print(F("Air Quality : "));
    Serial.print(mq135_value);
    Serial.println();
    Serial.print(F("CO Level : "));
    Serial.print(mq7_value);
    Serial.println();
    Serial.print(F("CH4 Level : "));
    Serial.print(mq4_value);
    Serial.println();
    Serial.println(F("----------------------"));
    Serial.println();
    Serial.println();
}
