#include <WiFiNINA.h>
#include <PubSubClient.h>

const char* ssid = "task2.1P";
const char* password = "arjun123";

const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;

const char* waveTopic = "ES/Wave";
const char* patTopic = "ES/Pat";
const char* myName = "Arjun";

const int TRIG_PIN = 11;
const int ECHO_PIN = 10;
const int LED1_PIN = 6;
const int LED2_PIN = 5;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

unsigned long lastDetection = 0;
const unsigned long detectionDelay = 2000;

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) 
    return -1;
  return duration * 0.0343 / 2;
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (strcmp(topic, waveTopic) == 0) {
    Serial.println("WAVE detected - Lights ON");
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
  }

  if (strcmp(topic, patTopic) == 0) {
    Serial.println("PAT detected - Lights OFF");
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
  }
}
void connectWiFi() {
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("Wi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to Mqttx...");
    if (mqttClient.connect("ArjunArduino")) {
      Serial.println("connected!");
      mqttClient.subscribe(waveTopic);
      mqttClient.subscribe(patTopic);
    } else {
      Serial.print("failed, state=");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}
void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  connectWiFi();
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);
  connectMQTT();
  Serial.println("System ready!");
}
void loop() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();
  if (millis() - lastDetection >= detectionDelay) {
    lastDetection = millis();
    float distance = getDistance();
    if (distance == -1) {
      Serial.println("Distance: No echo");
    } else {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
      if (distance > 10 && distance <= 30) {
        Serial.println("Publishing WAVE");
        mqttClient.publish(waveTopic, myName);
      } 
      else if (distance > 0 && distance <= 10) {
        Serial.println("Publishing PAT");
        mqttClient.publish(patTopic, myName);
      }
    }
  }
}