//  LIBRERÍAS NECESARIAS
#include <WiFi.h>                 // Conectamos el ESP32 al WiFi
#include <WiFiClientSecure.h>     // Crea un cliente HTTPS seguro.
#include <UniversalTelegramBot.h> // Crea el token del bot i el cliente https en el object “bot”
#include <ArduinoJson.h>          // Para procesar mensajes

//  CONFIGURACIÓN DEL WIFI 

const char* ssid = "ESP32";        // Nombre del WiFi 
const char* password = "pruebaesp32"; // Contraseña del WiFi

//  TOKEN DEL BOT DE TELEGRAM
String BOTtoken = "";

WiFiClientSecure client;          // Cliente seguro para HTTPS
UniversalTelegramBot bot(BOTtoken, client); // Bot de Telegram

//  VARIABLES DE CONTROL
unsigned long lastTimeBotRan = 0; // Guarda la ultima vez que se revisaron los mensajes
const int botDelay = 1000;        // Revisa telegram cada 1 segundo

// LED interno del ESP32 por ejemplo el 2
const int ledPin = 2;

//  FUNCIÓN PARA PROCESAR MENSAJES
void handleNewMessages(int numNewMessages) {

// Bucle para revisar cada mensaje 
 for (int i = 0; i < numNewMessages; i++) {

    String chat_id = bot.messages[i].chat_id; // ID del chat donde se ha recibido el mensaje, sirve para responder
    String text = bot.messages[i].text;       // Texto recibido por el usuario

    // si el comando /start, el bot se presenta i te dice la lista de comandos disponibles
    if (text == "/start") {
      bot.sendMessage(chat_id,
        " ESP32 BOT ACTIVO\n\n"
        "Comandos disponibles:\n"
        "/ping - Responde pong\n"
        "/ip - Muestra la IP del ESP32\n"
        "/led_on - Enciende LED\n"
        "/led_off - Apaga LED");
    }

    // Comando /ping
    if (text == "/ping") {
      bot.sendMessage(chat_id, "pong");
    }

    // --- Comando /ip ---
    if (text == "/ip") {
      bot.sendMessage(chat_id, "IP del ESP32: " + WiFi.localIP().toString());
    }

    // --- Encender LED ---
    if (text == "/led_on") {
      digitalWrite(ledPin, HIGH);
      bot.sendMessage(chat_id, "💡 LED encendido");
    }

    if (text == "/led_off") {
      digitalWrite(ledPin, LOW);
      bot.sendMessage(chat_id, "💡 LED apagado");
    }
 }
}

//  SETUP 
void setup() {
  Serial.begin(115200);  
  // Inicia el puerto serie para ver mensajes en el monitor serie.

  pinMode(ledPin, OUTPUT);
  // Configura el pin del LED como salida para poder encenderlo y apagarlo.

  WiFi.begin(ssid, password);
  // Inicia la conexión WiFi con el nombre y contraseña que pusiste.

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Espera hasta que el ESP32 se conecte al WiFi.
  // Mientras no esté conectado, imprime puntos en el monitor serie.

  client.setInsecure();
  // Permite conexiones HTTPS sin validar el certificado 

//  LOOP QUE REVISA MENSAJES DE TELEGRAM
void loop() {
  if (millis() - lastTimeBotRan > botDelay) {
    // Comprueba si ha pasado 1 segundo desde la última vez que se revisaron mensajes.

    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    // Pide a Telegram si hay mensajes nuevos desde el último ID recibido.

    if (numNewMessages > 0) {
      handleNewMessages(numNewMessages);
    }
    // Si hay mensajes nuevos, los procesa con la función handleNewMessages().

    lastTimeBotRan = millis();
    // Actualiza el tiempo para volver a esperar 1 segundo.
  }
}



