#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>


const char* ssid = "";
const char* password = "";


AsyncWebServer server(80);

void initWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("\nConectando ao WiFi");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWifi conectado!");
  Serial.print("IP do ESP32:");
  Serial.println(WiFi.localIP());
}


void initLittleFS(){
  if(!LittleFS.begin(true)){
    Serial.println("Erro ao montar LittleFS");
    while(true); // Se não inicar o LittleFS não inicia o sistema
  }
  Serial.println("LittleFS montado com sucesso");
}


void initWebServer(){

  //Rota principal
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  //Arquivos estáticos
  server.serveStatic("/", LittleFS, "/");

  // Receber informações do sistema
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"system\":\"ok\"}");
  });

  server.begin();
  Serial.println("Servidor iniciado");
}

void setup(){
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nInicializando sistema...");
  initLittleFS();
  initWifi();
  initWebServer();

  Serial.println("Sistema iniciado (SAFE)");
}

void loop() {
  // Vazio propositalmente
  // Toda lógica é assíncrona
}