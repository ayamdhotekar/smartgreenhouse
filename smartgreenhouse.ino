#include<ctype.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TxxxxxxxxP"
#define BLYNK_TEMPLATE_NAME "smartgreenhouse"
#define BLYNK_AUTH_TOKEN "qIxxxxxxxxxxxxxxxx_I"

#define SOILPIN 14
#define FANRELAY 26
#define LIGHTRELAY1 25
#define LIGHTRELAY2 33
#define PUMPRELAY 32
#define LDR 27
#define DHTPIN 13 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

char ssid[] = "Kxxh";
char pass[] = "9xxxxxxxx8";
float temperature ; 
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(SOILPIN, INPUT);
  pinMode(LDR, INPUT);
  pinMode(FANRELAY, OUTPUT);
  digitalWrite(FANRELAY, HIGH);

  pinMode(LIGHTRELAY1, OUTPUT);
  digitalWrite(LIGHTRELAY1, HIGH);

  pinMode(LIGHTRELAY2, OUTPUT);
  digitalWrite(LIGHTRELAY2, HIGH);

  pinMode(PUMPRELAY, OUTPUT);
  digitalWrite(PUMPRELAY, HIGH);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

}

void loop() {
  delay(5000);
  float newTemperature = dht.readTemperature()  ;
  if( !isnan(newTemperature) ) {temperature = newTemperature ; }
  
  float humidity = dht.readHumidity();
  int soilSensor = digitalRead(SOILPIN);
  int LDRval = digitalRead(LDR);

 
  Blynk.run();

  Blynk.virtualWrite(V0, temperature );
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, soilSensor);
  Blynk.virtualWrite(V3, LDRval );
  bool fanStatus = true ; 
  if (temperature > 33) {
    fanStatus = false ; 
  } 
  digitalWrite(FANRELAY, fanStatus);


  bool lightStatus = true ; 
  if (LDRval) {
    lightStatus= false ; 
  } 
  digitalWrite(LIGHTRELAY1, lightStatus);
  digitalWrite(LIGHTRELAY2, lightStatus);


  bool pumpStatus = true ; 
  if (soilSensor) {
    pumpStatus= false ; 
  } 
  digitalWrite(PUMPRELAY, pumpStatus);
 


  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" degree Celcius");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("Soil Moisture Sensor: ");
  if (soilSensor) {
    Serial.println("Insufficient Water");
  } else {
    Serial.println("Sufficient Water");
  }

  Serial.print("Light Intensity: ");
  if (LDRval) {
    Serial.println("Insufficient Light");
  } else {
    Serial.println("Sufficient Light");
  }

}
