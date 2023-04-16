#include <Arduino.h>
#include <WiFi.h>

#include <WebServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>

#include <INTERVAL.h>
#include <MqttController.hpp>
#include <MqttCallback.hpp>

#include <OTAFunctions.h>

#include <WiFiManager.h> 

#include <pins.h>
#include <html.h>

WiFiManager wifiManager;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

char status[64] = "off";
char ws_message[1024];

void notifyClients() {
  snprintf(ws_message, sizeof(ws_message)-1, "{\"id\": \"%s\", \"value\": \"%s\"}", "status", status);
  ws.textAll(ws_message);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    const char *data_char = (const char*)data; // This is not 0-terminated!

    // Disable everything
    digitalWrite(PIN_MOTOR_FORWARD_LEFT, LOW);
    digitalWrite(PIN_MOTOR_FORWARD_RIGHT, LOW);
    digitalWrite(PIN_MOTOR_BACKWARD_LEFT, LOW);
    digitalWrite(PIN_MOTOR_BACKWARD_RIGHT, LOW);

    if (strncmp(data_char, "forw", 4) == 0) {
      if (strncmp(&data_char[5], "on", 2) == 0) {
        digitalWrite(PIN_MOTOR_FORWARD_LEFT, HIGH);
        digitalWrite(PIN_MOTOR_FORWARD_RIGHT, HIGH);
        strncpy(status, "forward", sizeof(status)-1);
      } else {
        strncpy(status, "off", sizeof(status)-1);
      }
    } else if (strncmp(data_char, "back", 4) == 0) {
      if (strncmp(&data_char[5], "on", 2) == 0) {
        digitalWrite(PIN_MOTOR_BACKWARD_LEFT, HIGH);
        digitalWrite(PIN_MOTOR_BACKWARD_RIGHT, HIGH);
        strncpy(status, "backward", sizeof(status)-1);
      } else {
        strncpy(status, "off", sizeof(status)-1);
      }
    } else if (strncmp(data_char, "left", 4) == 0) {
      if (strncmp(&data_char[5], "on", 2) == 0) {
        digitalWrite(PIN_MOTOR_BACKWARD_LEFT, HIGH);
        digitalWrite(PIN_MOTOR_FORWARD_RIGHT, HIGH);
        strncpy(status, "left", sizeof(status)-1);
      } else {
        strncpy(status, "off", sizeof(status)-1);
      }
    } else if (strncmp(data_char, "righ", 4) == 0) {
      if (strncmp(&data_char[5], "on", 2) == 0) {
        digitalWrite(PIN_MOTOR_FORWARD_LEFT, HIGH);
        digitalWrite(PIN_MOTOR_BACKWARD_RIGHT, HIGH);
        strncpy(status, "right", sizeof(status)-1);
      } else {
        strncpy(status, "off", sizeof(status)-1);
      }
    } else if (strncmp(data_char, "beep", 4) == 0) {
      if (strncmp(&data_char[5], "on", 2) == 0) {
        digitalWrite(PIN_BUZZ, HIGH);
        strncpy(status, "beep", sizeof(status)-1);
      } else {
        digitalWrite(PIN_BUZZ, LOW);
        strncpy(status, "off", sizeof(status)-1);
      }
    } else {
      strncpy(status, "off", sizeof(status)-1);
    }
    notifyClients();
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    return status;
  }
  return "";
}

void setup(){
  init_pins();
  Serial.begin(115200);
  Serial.println("Starting network");
  wifiManager.autoConnect("ClESPotoni", "clespotoni");
  
  startOTA("clespotoni", OTAPW);
  
  while(millis()<10000) {
    ArduinoOTA.handle();
    INTERVAL(1000,millis()) {
      Serial.print(".");
    }
  }

  Serial.println(WiFi.localIP());

  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.begin();
}

void loop() {
  ws.cleanupClients();
  ArduinoOTA.handle();
}
