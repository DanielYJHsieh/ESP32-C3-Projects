/*
 * ESP32-C3 Super Mini Blue LED Blink Example
 * 
 * This example demonstrates LED blinking on ESP32-C3 Super Mini
 * The board has a blue LED on GPIO8 (active LOW)
 * 
 * Hardware:
 * - ESP32-C3 Super Mini with Blue LED (CD2 LED B 0603) on GPIO8
 * - LED is active LOW (LOW = ON, HIGH = OFF)
 * 
 * Board: ESP32C3 Dev Module
 * Port: COM4
 */

// Define LED pin - GPIO8 is the built-in blue LED on ESP32-C3 Super Mini
#define LED_PIN 8

void setup() {
  // Add a small delay to ensure power stabilization
  delay(100);
  
  // Initialize the LED pin as an output FIRST (before Serial)
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Ensure LED is OFF initially
  
  // Initialize serial communication for debugging
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n\nESP32-C3 Super Mini Blue LED Blink Example Starting...");
  Serial.print("Blue LED connected to GPIO: ");
  Serial.println(LED_PIN);
  Serial.println("LED is active LOW (LOW=ON, HIGH=OFF)");
  
  // Flash LED twice to indicate successful boot
  for(int i = 0; i < 2; i++) {
    digitalWrite(LED_PIN, LOW);  // LED ON
    delay(100);
    digitalWrite(LED_PIN, HIGH); // LED OFF
    delay(100);
  }
  
  Serial.println("Setup complete! Starting blink loop...");
}

void loop() {
  // Turn the LED ON (active LOW, so use LOW)
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED: ON");
  delay(1000);
  
  // Turn the LED OFF (active LOW, so use HIGH)
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED: OFF");
  delay(1000);
}