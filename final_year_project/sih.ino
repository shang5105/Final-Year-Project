#include <SoftwareSerial.h>
SoftwareSerial BTserial(0, 1);  // RX | TX


// L298N Motor Pins
#define EnA 3
#define In1 4


// LED Pins
#define ledW A7  // White LED
#define ledG 13  // Green LED


// Keypad Declaration
#include <Keypad.h>
#include <Wire.h>
const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 12, 11, 10, 9 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 8, 7, 6, 5 };  //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


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


// Joystick
const int X_pin = A4;
const int Y_pin = A5;
int x_center , y_center;


// lcd1 Initialisation
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);

int page_counter = 0;
unsigned long previousMillis = 0;
unsigned long previousMillisLCD = 0;
unsigned long interval = 2000;
unsigned long intervalLCD = 10000;

int list_counter;

char key;

int flag_crop_selected;


///////////////////////////////////////////////////////////////////////////////////////////  Void Setup Begins Here  ///////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  BTserial.begin(9600);

  pinMode(EnA, OUTPUT);
  pinMode(In1, OUTPUT);

  pinMode(mq135, INPUT); 
  pinMode(mq4, INPUT);
  pinMode(mq7, INPUT);

  lcd1.init();
  lcd1.clear();
  lcd1.backlight();
  lcd2.init();
  lcd2.clear();
  lcd2.backlight();

  lcd1.setCursor(0, 0);
  lcd1.print(F("Plant"));
  lcd1.setCursor(0, 1);
  lcd1.print(F("Monitor System"));
  delay(3000);

  list_counter = 0;
  flag_crop_selected = 0;

  lcd2.setCursor(0, 0);
  lcd2.print(F("C to view crops"));
  lcd2.setCursor(0, 1);
  lcd2.print(F("0-9 to select"));

  x_center = analogRead(A4);
  y_center = analogRead(A5);
}


///////////////////////////////////////////////////////////////////////////////////////////  Void Loop Begins Here  ///////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  DHT.read(DHT11_PIN);
  light_value = (map(analogRead(ldr), 1024, 0, 25, 100));
  moisture_value = (map(analogRead(soil_moisture), 380, 950, 100, 0));
  mq135_value = analogRead(mq135);
  mq7_value = analogRead(mq7);
  mq4_value = analogRead(mq4);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {  //If interval is reached, return to home page 1
    previousMillis = currentMillis;
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

  keypadd();
  lcdMain();
}



void keypadd() {
  key = keypad.getKey();
  if (key == 'C' || (analogRead(X_pin) > (x_center+200))) {
    list_crop(list_counter);
    delay(300);
  }


  if (key == 'D' || (analogRead(X_pin) < (x_center-200))) {
    list_counter++;
    if (list_counter > 5) {
      list_counter = 0;
    }
    list_crop(list_counter);
    delay(300);
  }


  if (analogRead(Y_pin) > (y_center+200) && list_counter == 0){
    rice();
    flag_crop_selected = 1;
    delay(300);
  }
  if (analogRead(Y_pin) < (y_center-200) && list_counter == 1){
    wheat();
    flag_crop_selected = 1;
    delay(300);
  }
  if (analogRead(Y_pin) > (y_center+200) && list_counter == 1){
    millets();
    flag_crop_selected = 1;
    delay(300);
  }
  if (analogRead(Y_pin) < (y_center-200) && list_counter == 2){
    cotton();
    flag_crop_selected = 1;
    delay(300);
  }
  if (analogRead(Y_pin) > (y_center+200) && list_counter == 2){
    pulses();
    flag_crop_selected = 1;
    delay(300);
  }
  if (analogRead(Y_pin) < (y_center-200) && list_counter == 3){
    tea();
    flag_crop_selected = 1;
    delay(300);
  }
  if (analogRead(Y_pin) > (y_center+200) && list_counter == 3){
    coffee();
    flag_crop_selected = 1;
    delay(300);
  }
  if (analogRead(Y_pin) < (y_center-200) && list_counter == 4){
    groundnut();
    flag_crop_selected = 1;
    delay(300);
  }
  if (analogRead(Y_pin) > (y_center+200) && list_counter == 4){
    sugarcane();
    flag_crop_selected = 1;
    delay(300);
  }
  if (analogRead(Y_pin) < (y_center-200) && list_counter == 5){
    spices();
    flag_crop_selected = 1;
    delay(300);
  }



  if (key == '0') {
    rice();
    flag_crop_selected = 1;
  }
  if (key == '1') {
    wheat();
    flag_crop_selected = 1;
  }
  if (key == '2') {
    millets();
    flag_crop_selected = 1;
  }
  if (key == '3') {
    cotton();
    flag_crop_selected = 1;
  }
  if (key == '4') {
    pulses();
    flag_crop_selected = 1;
  }
  if (key == '5') {
    tea();
    flag_crop_selected = 1;
  }
  if (key == '6') {
    coffee();
    flag_crop_selected = 1;
  }
  if (key == '7') {
    groundnut();
    flag_crop_selected = 1;
  }
  if (key == '8') {
    sugarcane();
    flag_crop_selected = 1;
  }
  if (key == '9') {
    spices();
    flag_crop_selected = 1;
  }
}




void lcdMain() {
  if (flag_crop_selected == 1) {
    switch (page_counter) {
      case 0:
        {
          if (DHT.humidity == 0 || DHT.temperature == 0 || DHT.humidity == 255 || DHT.temperature == 255) {
            lcd1.clear();
            lcd1.setCursor(0, 0);  // set the cursor on the first row and column
            lcd1.print(F("DHT Sensor"));
            lcd1.setCursor(0, 1);  //set the cursor on the second row and first column
            lcd1.print(F("Not Initialize"));
            delay(300);
          }
          else {
            lcd1.clear();
            lcd1.setCursor(0, 0);  // set the cursor on the first row and column
            lcd1.print(F("Humid = "));
            lcd1.print(DHT.humidity);  //print the humidity
            lcd1.print(F(" %"));
            lcd1.setCursor(0, 1);  //set the cursor on the second row and first column
            lcd1.print(F("Temp = "));
            lcd1.print(DHT.temperature);  //print the temperature
            lcd1.print(F(" C"));
            delay(300);
          }
        }
        break;

      case 1:
        {
          lcd1.clear();
          lcd1.setCursor(0, 0);
          lcd1.print(F("Light = "));
          lcd1.print(light_value);
          lcd1.print(F(" %"));
          lcd1.setCursor(0, 1);
          lcd1.print(F("Moisture = "));
          lcd1.print(moisture_value);
          lcd1.print(F(" %"));
          delay(300);
        }
        break;

      case 2:
        {
          lcd1.clear();
          if (light_value < light_limit) {
            lcd1.setCursor(0, 0);
            lcd1.print(F("Low Sunlight"));
          }
          if (light_value >= light_limit) {
            lcd1.setCursor(0, 0);
            lcd1.print(F("Ample Sunlight"));
          }
          if (moisture_value < moisture_limit) {
            lcd1.setCursor(0, 1);
            lcd1.print(F("Low Water"));
          }
          if (moisture_value >= moisture_limit) {
            lcd1.setCursor(0, 1);
            lcd1.print(F("Ample Water"));
          }
          delay(300);
        }
        break;

      case 3:
        {
          lcd1.clear();
          if (DHT.humidity > humid_limit) {
            lcd1.setCursor(0, 0);
            lcd1.print(F("High Humidity"));
          }
          if (DHT.humidity < humid_limit) {
            lcd1.setCursor(0, 0);
            lcd1.print(F("Normal Humidity"));
          }
          if (DHT.temperature > temp_limit) {
            lcd1.setCursor(0, 1);
            lcd1.print(F("High Temprature"));
          }
          if (DHT.temperature < temp_limit) {
            lcd1.setCursor(0, 1);
            lcd1.print(F("Normal Temprature"));
          }
          delay(300);
        }
        break;
    }

    unsigned long currentMillisLCD = millis();
    if (currentMillisLCD - previousMillisLCD > intervalLCD) {  //If interval is reached, return to home page 1
      previousMillisLCD = currentMillisLCD;
      page_counter++;
      if (page_counter > 3) {
        page_counter = 0;
      }
      lcd1.clear();
    }
  }
}




void list_crop(int list_counter) {
  switch (list_counter) {
    case 0:
      {
        lcd2.setCursor(0, 0);
        lcd2.print("                ");
        lcd2.setCursor(0, 1);
        lcd2.print("                ");
        lcd2.setCursor(0, 0);
        lcd2.print("D to next page");
        lcd2.setCursor(0, 1);
        lcd2.print("0. Rice");
      }
      break;

    case 1:
      {
        lcd2.setCursor(0, 0);
        lcd2.print("                ");
        lcd2.setCursor(0, 1);
        lcd2.print("                ");
        lcd2.setCursor(0, 0);
        lcd2.print("1. Wheat");
        lcd2.setCursor(0, 1);
        lcd2.print("2. Millets");
      }
      break;

    case 2:
      {
        lcd2.setCursor(0, 0);
        lcd2.print("                ");
        lcd2.setCursor(0, 1);
        lcd2.print("                ");
        lcd2.setCursor(0, 0);
        lcd2.print("3. Cotton");
        lcd2.setCursor(0, 1);
        lcd2.print("4. Pulses");
      }
      break;

    case 3:
      {
        lcd2.setCursor(0, 0);
        lcd2.print("                ");
        lcd2.setCursor(0, 1);
        lcd2.print("                ");
        lcd2.setCursor(0, 0);
        lcd2.print("5. Tea");
        lcd2.setCursor(0, 1);
        lcd2.print("6. Coffee");
      }
      break;

    case 4:
      {
        lcd2.setCursor(0, 0);
        lcd2.print("                ");
        lcd2.setCursor(0, 1);
        lcd2.print("                ");
        lcd2.setCursor(0, 0);
        lcd2.print("7. Groundnut");
        lcd2.setCursor(0, 1);
        lcd2.print("8. Sugarcane");
      }
      break;

    case 5:
      {
        lcd2.setCursor(0, 0);
        lcd2.print("                ");
        lcd2.setCursor(0, 1);
        lcd2.print("                ");
        lcd2.setCursor(0, 0);
        lcd2.print("9. Spices");
        lcd2.setCursor(0, 1);
        lcd2.print("Select by 0-9");
      }
      break;
  }
}



void rice() {
  humid_limit = 50;
  temp_limit = 23;
  moisture_limit = 95;
  light_limit = 70;
  lcd2.setCursor(0, 0);
  lcd2.print("                ");
  lcd2.setCursor(0, 1);
  lcd2.print("                ");
  lcd2.setCursor(0, 0);
  lcd2.print("Selected Crop -:");
  lcd2.setCursor(0, 1);
  lcd2.print("Rice");
}

void wheat() {
  humid_limit = 55;
  temp_limit = 25;
  moisture_limit = 20;
  light_limit = 70;
  lcd2.setCursor(0, 0);
  lcd2.print("                ");
  lcd2.setCursor(0, 1);
  lcd2.print("                ");
  lcd2.setCursor(0, 0);
  lcd2.print("Selected Crop -:");
  lcd2.setCursor(0, 1);
  lcd2.print("Wheat");
}

void millets() {
  humid_limit = 40;
  temp_limit = 30;
  moisture_limit = 23;
  light_limit = 70;
  lcd2.setCursor(0, 0);
  lcd2.print("                ");
  lcd2.setCursor(0, 1);
  lcd2.print("                ");
  lcd2.setCursor(0, 0);
  lcd2.print("Selected Crop -:");
  lcd2.setCursor(0, 1);
  lcd2.print("Millets");
}

void cotton() {
  humid_limit = 70;
  temp_limit = 17;
  moisture_limit = 40;
  light_limit = 70;
  lcd2.setCursor(0, 0);
  lcd2.print("                ");
  lcd2.setCursor(0, 1);
  lcd2.print("                ");
  lcd2.setCursor(0, 0);
  lcd2.print("Selected Crop -:");
  lcd2.setCursor(0, 1);
  lcd2.print("Cotton");
}

void pulses() {
  humid_limit = 40;
  temp_limit = 23;
  moisture_limit = 30;
  light_limit = 70;
  lcd2.setCursor(0, 0);
  lcd2.print("                ");
  lcd2.setCursor(0, 1);
  lcd2.print("                ");
  lcd2.setCursor(0, 0);
  lcd2.print("Selected Crop -:");
  lcd2.setCursor(0, 1);
  lcd2.print("Pulses");
}

void tea() {
  humid_limit = 80;
  temp_limit = 20;
  moisture_limit = 85;
  light_limit = 70;
  lcd2.setCursor(0, 0);
  lcd2.print("                ");
  lcd2.setCursor(0, 1);
  lcd2.print("                ");
  lcd2.setCursor(0, 0);
  lcd2.print("Selected Crop -:");
  lcd2.setCursor(0, 1);
  lcd2.print("Tea");
}

void coffee() {
  humid_limit = 65;
  temp_limit = 20;
  moisture_limit = 75;
  light_limit = 40;
  lcd2.setCursor(0, 0);
  lcd2.print("                ");
  lcd2.setCursor(0, 1);
  lcd2.print("                ");
  lcd2.setCursor(0, 0);
  lcd2.print("Selected Crop -:");
  lcd2.setCursor(0, 1);
  lcd2.print("Coffee");
}

void groundnut() {
  humid_limit = 65;
  temp_limit = 30;
  moisture_limit = 23;
  light_limit = 90;
  lcd2.setCursor(0, 0);
  lcd2.print("                ");
  lcd2.setCursor(0, 1);
  lcd2.print("                ");
  lcd2.setCursor(0, 0);
  lcd2.print("Selected Crop -:");
  lcd2.setCursor(0, 1);
  lcd2.print("Groundnut");
}

void sugarcane() {
  humid_limit = 80;
  temp_limit = 25;
  moisture_limit = 70;
  light_limit = 90;
  lcd2.setCursor(0, 0);
  lcd2.print("                ");
  lcd2.setCursor(0, 1);
  lcd2.print("                ");
  lcd2.setCursor(0, 0);
  lcd2.print("Selected Crop -:");
  lcd2.setCursor(0, 1);
  lcd2.print("Sugarcane");
}

void spices() {
  humid_limit = 50;
  temp_limit = 23;
  moisture_limit = 10;
  light_limit = 70;
  lcd2.setCursor(0, 0);
  lcd2.print("                ");
  lcd2.setCursor(0, 1);
  lcd2.print("                ");
  lcd2.setCursor(0, 0);
  lcd2.print("Selected Crop -:");
  lcd2.setCursor(0, 1);
  lcd2.print("Spices");
}