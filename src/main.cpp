/******************************************************************************
 * Copyright 2022 Edwin Kestler
 * Licensed under MIT License. 
 * based on 2018 Google IoT CORE Library Example.
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
// This file contains static methods for API requests using Wifi / MQTT
#include <Arduino.h>
#include <Client.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTT.h>
#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>
#include <ciotc_config.h>  // Update this file with your configuration
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
//--------------------------------NeoPixel Config.-----------------------------

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN       19  // pin del pwm 23 esp32 --> 4-->esp8266
#define NUMPIXELS 60  // cantidad de Neopixeles
#define retardo 2     //delay de secuencia visual

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

byte led_W = 0;
byte degradacion = 0;
byte intensidad = 10;



//---------------------------------------------rutina de flaseo blanco-------------------------------------------
void flasheo() {
  led_W = 0;
  pixels.setBrightness(100);
  for (byte pos = 0; pos < NUMPIXELS; pos++) {

    pixels.setPixelColor(led_W, pixels.Color(150, 150, 150)); // Blanco

    pixels.show();   // Send the updated pixel colors to the hardware.
    led_W++;
    
  }
  for (byte pos = 0; pos < NUMPIXELS; pos += 4) {
    pixels.setPixelColor(pos, pixels.Color(150, 150, 0));//amarillo
    pixels.show();
  }

}

//---------------------------------------------rutina de flaseo de colores degradados-----------------------------
void flasheo_color(byte R, byte G, byte B, byte cuanto) {

  led_W = 0;
  pixels.setBrightness(100);//controlo el porcentaje de intensidad luminica
  for (byte pos = 0; pos < NUMPIXELS; pos++) {

    pixels.setPixelColor(led_W, pixels.Color(R, G, B)); //amarillo --> Blanco

    pixels.show();   // Send the updated pixel colors to the hardware.
    led_W++;
  }
  delay(cuanto);
  for (byte pos = (NUMPIXELS - 1); pos > 0; pos--) {
    pixels.setPixelColor(led_W, pixels.Color(0, 0, 0));

    pixels.show();
    led_W--;
  }
  delay(cuanto);
}

void rutina1_trueno(){
  flasheo_color(10, 0, 255, 5); //morado
  //----------------------------------------------vaiven--------------------------------------------------------
  intensidad = 10;
  led_W = 0;

  for (byte pos = 0; pos < NUMPIXELS; pos++) {
    pixels.setBrightness(intensidad);//controlo el porcentaje de intensidad luminica
    pixels.setPixelColor(led_W, pixels.Color(150, 150, 150)); //amarillo --> Blanco
    pixels.show();   // Send the updated pixel colors to the hardware.
    led_W++;
    intensidad += (100 / NUMPIXELS);
    delay(retardo);
  }
  for (byte pos = 0; pos < NUMPIXELS; pos += 4) {
    pixels.setPixelColor(pos, pixels.Color(150, 150, 0));
    pixels.show();
  }

  for (byte pos = (NUMPIXELS - 1); pos > 0; pos--) {
    pixels.setBrightness(intensidad);
    pixels.setPixelColor(led_W, pixels.Color(0, 0, 0));

    pixels.show();
    led_W--;
    intensidad -= (100 / NUMPIXELS);
    delay(retardo);
  }
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.show();
  delay(500);

  flasheo();
  flasheo_color(0, 0, 160, 2);//azul
  flasheo();
  flasheo_color(255, 0, 255, 2); //morado
  flasheo_color(160, 160, 160, 10);//blanco
  flasheo_color(230, 230, 250, 2); //morado
  flasheo_color(160, 160, 160, 10);
  flasheo_color(0, 0, 160, 2);//azul
  flasheo();
  flasheo_color(30, 30, 160, 3);//azul
  flasheo_color(0, 0, 0, 3);//azul
}
 
//----------------------MQTT. Config---------------------------------------------

// !!REPLACEME!!
// The MQTT callback function for commands and configuration updates
// Place your message handler code here.

void messageReceived(String &topic, String &payload){
  Serial.println("incoming: " + topic + " - " + payload);
  //-----------------------------------aqui ira el trueno------------------------------------------------------------
  
  //-creo un Json para decodificar los datos de recepción
    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      return;
    }
    //extraigo info del Json  
    String text = doc["text"];
    int id = doc["id"];
    bool stat = doc["status"];
    float value = doc["value"];
    
    Serial.print(F("El id es: "));
    Serial.println(id);
  
  if(id < 54){
    rutina1_trueno();
    Serial.println("ID esperado\n");
  }else{
    Serial.println("ID desconocido\n");
  }
  
  
}
///////////////////////////////

// Initialize WiFi and MQTT for this board
Client *netClient;
CloudIoTCoreDevice *device;
CloudIoTCoreMqtt *mqtt;
MQTTClient *mqttClient;
unsigned long iat = 0;
String jwt;

///////////////////////////////
// Helpers specific to this board
///////////////////////////////
String getDefaultSensor(){
  return "Wifi: " + String(WiFi.RSSI()) + "db";
}

String getJwt(){
  iat = time(nullptr);
  Serial.println("Refreshing JWT");
  jwt = device->createJWT(iat, jwt_exp_secs);
  return jwt;
}

void setupWifi(){
  Serial.println("Starting wifi");

  WiFi.mode(WIFI_STA);
  // WiFi.setSleep(false); // May help with disconnect? Seems to have been removed from WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
  }

  configTime(0, 0, ntp_primary, ntp_secondary);
  Serial.println("Waiting on time sync...");
  while (time(nullptr) < 1510644967){
    delay(10);
  }
}

void connectWifi(){
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
}

///////////////////////////////
// Orchestrates various methods from preceeding code.
///////////////////////////////
bool publishTelemetry(String data){
  return mqtt->publishTelemetry(data);
}

void connect(){
  connectWifi();
  mqtt->mqttConnect();
}

void setupCloudIoT(){
  device = new CloudIoTCoreDevice(
    project_id, location, registry_id, device_id,
    private_key);
  
  setupWifi();

  netClient = new WiFiClientSecure();
  ((WiFiClientSecure*)netClient)->setCACert(primary_ca);
  mqttClient = new MQTTClient(512);
  mqttClient->setOptions(180, true, 1000); // keepAlive, cleanSession, timeout
  mqtt = new CloudIoTCoreMqtt(mqttClient, netClient, device);
  mqtt->setUseLts(true);
  mqtt->startMQTT();
}
unsigned long timer;
unsigned long last_Telemetry_Millis = 0;
unsigned long telemetry_publish_interval = 30000;


void setup(){
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupCloudIoT();
  timer = millis();

  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  delay(10);
  pixels.clear(); // inicio apagado

}

void loop(){
  mqtt->loop();

  delay(10);  // <- fixes some issues with WiFi stability

  if (!mqttClient->connected()) {
    connect();
  }
  
  // TODO: replace with your code
  // publish a message roughly every second.
  if (millis() - last_Telemetry_Millis > telemetry_publish_interval) {
    last_Telemetry_Millis = millis();
    Serial.println(F("sending Telemetry data"));
    String Telemetry = ("la señal de wifi es:"+ getDefaultSensor());
    publishTelemetry(Telemetry);
  }
}