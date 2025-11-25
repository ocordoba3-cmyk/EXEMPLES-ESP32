/*
 Blink amb temps configurables
 Fa parpellejar un LED connectat al pin 23 amb temps d’encesa i apagat configurables.
*/

// Constant per al pin del LED
const int LED_PIN = 23;

// Variables configurables per al temps en ms
int tempsEncesa = 500;   // mig segon
int tempsApagat = 2000;  // dos segons

void setup() {
  pinMode(LED_PIN, OUTPUT); // Configurem el pin com a sortida
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // Encenem el LED
  delay(tempsEncesa);            // Esperem el temps d’encesa

  digitalWrite(LED_PIN, LOW);    // Apaga el LED
  delay(tempsApagat);            // Espera el temps d’apagat
}
