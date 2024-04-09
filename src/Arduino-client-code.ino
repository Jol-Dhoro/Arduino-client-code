//The Sum of It All (Ed Sheeran docu name)
#include <Wire.h>
#include <DHT.h>
int relay = 12; // Plug the relay into Digital Pin 13
float calibration_value = 21.34 - 0.7;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
float ph_act;
#define DHTPIN 7       // DHT to digital pin 7
#define DHTTYPE DHT22  // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
int chk;
float hum;                  //Stores humidity value
float temp1;                //Stores temperature value
const int sensor_pin = A1;  //soil moisture to A1
const float MT = 9.00;      //threshold value for moisture %
const float HT = 23.00;     //threshold value for humidity
const float TT = 5.00;     //threshold value for temp
void setup() {
  Wire.begin();
  dht.begin();
  pinMode(relay, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  //temp and humidity starts
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp1 = dht.readTemperature();
  //pH calculation starts
  for (int i = 0; i < 10; i++) {
    buffer_arr[i] = analogRead(A0);  //ph on A0
    delay(30);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buffer_arr[i] > buffer_arr[j]) {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * 5.0 / 1024 / 6;
  ph_act = -5.70 * volt + calibration_value;
  //moisture percent starts
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage = (100 - ((sensor_analog / 1023.00) * 100));
  //every data being printed
  Serial.print("Moisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.println("%");
  Serial.print("pH Val: ");
  Serial.println(ph_act);
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp1);
  Serial.println(" Celsius");
  Serial.print("\n");
  delay(10000);  //Delay 10 sec. (will be changed to 1hr upon release)
  if((temp1<=TT)||(hum<=HT)||(moisture_percentage<=MT))
  {
      digitalWrite(relay, HIGH); // Turn the relay on
  }
  else{
    digitalWrite(relay, LOW); // Turn the relay Off
  }
}

// vim:ft=cpp
