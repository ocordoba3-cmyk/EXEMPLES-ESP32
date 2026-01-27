// LIBRERIAS NECESARIAS
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// CONFIGURACION DEL WIFI
const char* ssid = "ESP32";
const char* password = "pruebaesp32";

// TOKEN DEL BOT
String BOTtoken = "";

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// VARIABLES DE CONTROL
unsigned long lastTimeBotRan = 0;
const int botDelay = 1000;

// LED interno
const int ledPin = 2;

// FUNCION PARA PROCESAR MENSAJES
void handleNewMessages(int numNewMessages) {

  for (int i = 0; i < numNewMessages; i++) {

    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    if (text == "/start") {
      bot.sendMessage(chat_id,
        " ESP32 BOT ACTIVO\n\n"
        "Comandos disponibles:\n"
        "/ping - Responde pong\n"
        "/ip - Muestra la IP del ESP32\n"
        "/led_on - Enciende LED\n"
        "/led_off - Apaga LED\n"
        "/foto - Hace una foto");
    }

    else if (text == "/ping") {
      bot.sendMessage(chat_id, "pong");
    }

    else if (text == "/ip") {
      bot.sendMessage(chat_id, "IP del ESP32: " + WiFi.localIP().toString());
    }

    else if (text == "/led_on") {
      digitalWrite(ledPin, HIGH);
      bot.sendMessage(chat_id, "LED encendido");
    }

    else if (text == "/led_off") {
      digitalWrite(ledPin, LOW);
      bot.sendMessage(chat_id, "LED apagado");
    }

    // ---------------- COMANDO /FOTO ----------------
    else if (text == "/foto") {

      camera_fb_t *hola = esp_camera_fb_get();
      if (!hola) {
        bot.sendMessage(chat_id, "Error al hacer la foto");
        return;
      }

      WiFiClientSecure clientTCP;
      clientTCP.setInsecure();

      if (!clientTCP.connect("api.telegram.org", 443)) {
        bot.sendMessage(chat_id, "Error conectando a Telegram");
        esp_camera_fb_return(hola);
        return;
      }

      String head = "--X\r\n"
                    "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n" +
                    chat_id +
                    "\r\n--X\r\n"
                    "Content-Disposition: form-data; name=\"photo\"; filename=\"cam.jpg\"\r\n"
                    "Content-Type: image/jpeg\r\n\r\n";

      String tail = "\r\n--X--\r\n";

      clientTCP.println("POST /bot" + BOTtoken + "/sendPhoto HTTP/1.1");
      clientTCP.println("Host: api.telegram.org");
      clientTCP.println("Content-Type: multipart/form-data; boundary=X");
      clientTCP.println("Content-Length: " + String(head.length() + hola->len + tail.length()));
      clientTCP.println();

      clientTCP.print(head);
      clientTCP.write(hola->buf, hola->len);
      clientTCP.print(tail);

      esp_camera_fb_return(hola);

      bot.sendMessage(chat_id, "Foto enviada");
    }
  }
}

// SETUP
void setup() {
  Serial.begin(115200);

  // Inicializar cámara
  camera_config_t config = {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sscb_sda = SIOD_GPIO_NUM,
    .pin_sscb_scl = SIOC_GPIO_NUM,
    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,
    .pin_xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_VGA,
    .jpeg_quality = 10,
    .fb_count = 1
  };

  esp_camera_init(&config);

  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  client.setInsecure();
}

// LOOP
void loop() {

  if (millis() - lastTimeBotRan > botDelay) {

    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    if (numNewMessages > 0) {
      handleNewMessages(numNewMessages);
    }

    lastTimeBotRan = millis();
  }
}
