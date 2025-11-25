#define BUTTON_PIN 13       // GPIO13 connectat al botó
#define DEBOUNCE_TIME 50    // temps de debounce en ms

// Variables per al debounce
int lastSteadyState = HIGH;
int lastFlickerableState = HIGH;
int currentState;
unsigned long lastDebounceTime = 0;

// Comptador de pulsacions
int counter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Botó amb resistència pull-up interna
}

// Funció que encapsula la lectura del botó amb debounce
bool buttonPressed() {
  currentState = digitalRead(BUTTON_PIN);

  if (currentState != lastFlickerableState) {
    lastDebounceTime = millis();
    lastFlickerableState = currentState;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
    if (lastSteadyState != currentState) {
      lastSteadyState = currentState;
      // Retorna true si el botó està premut (LOW per INPUT_PULLUP)
      return (currentState == LOW);
    }
  }
  return false;
}

void loop() {
  if (buttonPressed()) {
    counter++;
    Serial.print("Botó premut! Comptador = ");
    Serial.println(counter);
  }
}
