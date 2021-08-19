#include "index.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

const uint8_t LED_Pin = 2;

const char* ssid = "HIPAA";
const char* password = "123456789";

boolean LED_Status;

AsyncWebServer server(80);
WebSocketsServer websocket(81);

IPAddress local_IP(192, 168, 137, 77);
IPAddress gateway(10, 32, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

void setup() {
  Serial.begin(115200);
  pinMode(LED_Pin, OUTPUT);
  digitalWrite(LED_Pin, LOW);
  WiFi_Init();
}

void loop() {
  websocket.loop();
}

void WiFi_Init() {
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    Serial.println("STA Failed to configure.");
  else Serial.println("STA Succeeded to config.");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.println(WiFi.localIP());

  server.on("/", [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });
  server.onNotFound(notFound);
  server.begin();
  websocket.begin();
  websocket.onEvent(webSocketEvent);

}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Page Not found");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type)
  {
    case WStype_DISCONNECTED: Serial.printf("[%u] Disconnected!\n", num); break;
    case WStype_CONNECTED: {
        IPAddress ip = websocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        if (LED_Status)websocket.sendTXT(num, "LED ON");
        else websocket.sendTXT(num, "LED OFF");
      }
      break;
    case WStype_TEXT: {
        Serial.printf("[%u] get Text: %s\n", num, payload);
        String message = String((char*)( payload));
        if (message == "LED ON") {
          digitalWrite(LED_Pin, HIGH);
          LED_Status = true;
        }
        else {
          digitalWrite(LED_Pin, LOW);
          LED_Status = false;
        }
        websocket.broadcastTXT(message);
      }
      break;
  }
}
