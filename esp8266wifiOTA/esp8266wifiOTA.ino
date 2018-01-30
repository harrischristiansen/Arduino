#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Wire.h>

const char* ssid = "HC";
const char* password = "sunnysunny";
const char* host = "ESP-OTA";

#define led_pin BUILTIN_LED
#define blu_pin D4
#define beat 450
IPAddress ip(10, 3, 0, 85); //Node static IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(10, 3, 0, 254);
void setup() {
  Serial.begin(57600);

  pinMode(led_pin, OUTPUT);

  Serial.println("");
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("Retrying connection…");
    delay(5);
  }
  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
    analogWrite(led_pin, 0);
  });

  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
    for (int i = 0; i < 80; i++)
    {
      digitalWrite(led_pin, HIGH);
      delay(i * 2);
      digitalWrite(led_pin, LOW);
      delay(i * 2);
    }
    ESP.restart();
  });

  ArduinoOTA.onError([](ota_error_t error) {
    ESP.restart();
  });

  /* setup the OTA server */
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Now Online!");
}

void loop() {
  float power = WiFi.RSSI();
  analogWrite(blu_pin, power);
  ArduinoOTA.handle();
  heartbeat();
  delay(30);
}

void heartbeat() {
  digitalWrite(led_pin, HIGH);
  delay(beat);
  digitalWrite(led_pin, LOW);
  delay(beat);
}
