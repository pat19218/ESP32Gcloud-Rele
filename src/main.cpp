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
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#define rele1 19          //pin del primer rele
//----------------------MQTT. Config---------------------------------------------

// !!REPLACEME!!
// The MQTT callback function for commands and configuration updates
// Place your message handler code here.

void messageReceived(String &topic, String &payload){
  Serial.println("incoming: " + topic + " - " + payload);
  //-----------------------------------Reinicio rele-----------------------------------------------------
  
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
    digitalWrite(rele1, 0);
    delay(1500);
    digitalWrite(rele1, 1);
    Serial.println("Reinicio router\n");
  }else{
    Serial.println("Todo trabaja bien\n");
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
byte refresh=0;


void setup(){
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupCloudIoT();
  timer = millis();
  pinMode(rele1, OUTPUT);
  digitalWrite(rele1, 1);
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
    String Telemetry = ("señal ESP_VPN:"+ getDefaultSensor());
    publishTelemetry(Telemetry);
    refresh++;
  }

  if ( (refresh-2) == 0) { //Para cada 10 mins --> refresh-20
    refresh = 0;
    Serial.println(F("Envieando dato de esp conectado "));
    String Telemetry = ("Dispositivo en conexion ESP_VPN");
    publishTelemetry(Telemetry);
  }
  
}