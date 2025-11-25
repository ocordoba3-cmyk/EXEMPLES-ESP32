#define BUTTON_PIN 13  // GPIO21 pin connected to button
#define DEBOUNCE_TIME  50 // debounce time in milliseconds

// Variables for debounce logic
int lastSteadyState = LOW;
int lastFlickerableState = LOW;
int currentState;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool button() {
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

  // Retorna false si no hi ha canvi o el botó no està premut
  return false;
}

void loop() {
  if (button()) {
    Serial.println("Botó premut");
  }
}
