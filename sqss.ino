// Edited As Of: 2022-01-05
// API Call

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Arrows0nly04?";
const char* password = "@Looper04?";

String serverName = "https://flask-api-5ka3ttedna-df.a.run.app/sqss_data?";
String dbName = "client_template"; 
String sensorName = "client_sqss_L"; 
int sensorId = 1;
int readKey = 111;
int o2xx = 0;

void initWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.print("Connected to WiFi");
}

#include <Wire.h>

#include "SparkFun_SCD30_Arduino_Library.h"
#define scd_I2C_SDA 21
#define scd_I2C_SCL 22
TwoWire I2CSCD = TwoWire(0);
SCD30 airSensor;

#include "Adafruit_VEML7700.h"
#define veml_I2C_SDA 33
#define veml_I2C_SCL 32
TwoWire I2CVEML = TwoWire(1);
Adafruit_VEML7700 veml = Adafruit_VEML7700();


void setup() {
  Serial.begin(115200);
  
  Wire.begin();
  I2CSCD.begin(scd_I2C_SDA, scd_I2C_SCL);
  if (!airSensor.begin(&I2CSCD)) {
    Serial.println("SCD30 not detected");
    while (1)
      ;
  }  
  
  I2CVEML.begin(veml_I2C_SDA, veml_I2C_SCL);
  if (!veml.begin(&I2CVEML)) {
    Serial.println("Light Sensor not found");
    while (1);
  }

  initWiFi();

}
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection lost. Attempting to reconnect...");
    initWiFi();
  
  } else {
    HTTPClient http;
    float co2x = airSensor.getCO2();
    float temp = airSensor.getTemperature();
    float humi = airSensor.getHumidity();
    float luxx = veml.readLux();

    String serverPath = serverName
    + "s_db_name=" + dbName
    + "&s_sensor_name=" + sensorName
    + "&s_sqss_id=" + String(sensorId)
    + "&s_temp=" + String(temp)
    + "&s_humi=" + String(humi)
    + "&s_co2x=" + String(co2x)
    + "&s_o2xx=" + String(o2xx)
    + "&s_luxx=" + String(luxx)
    + "&read_key=" + String(readKey);
    Serial.println(serverPath);

    http.begin(serverPath.c_str()); //Specify the URL
    int httpCode = http.GET();      //Make the request

    if (httpCode > 0) { //Check for the returning code
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }
    delay(60000);
  }
}
