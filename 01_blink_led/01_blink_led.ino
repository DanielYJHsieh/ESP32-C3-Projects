/*
 * ESP32-C3 Super Mini Blue LED Blink Example with RAM Monitoring
 * 
 * This example demonstrates LED blinking on ESP32-C3 Super Mini
 * with RAM usage monitoring displayed on UART
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

// Function to display RAM usage
void printMemoryInfo() {
  Serial.println("========== RAM Usage ==========");
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  
  Serial.print("Total Heap: ");
  Serial.print(ESP.getHeapSize());
  Serial.println(" bytes");
  
  Serial.print("Used Heap: ");
  Serial.print(ESP.getHeapSize() - ESP.getFreeHeap());
  Serial.println(" bytes");
  
  Serial.print("Heap Usage: ");
  Serial.print(100.0 * (ESP.getHeapSize() - ESP.getFreeHeap()) / ESP.getHeapSize(), 2);
  Serial.println(" %");
  
  Serial.print("Min Free Heap (since boot): ");
  Serial.print(ESP.getMinFreeHeap());
  Serial.println(" bytes");
  
  Serial.println("===============================");
}

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
  
  Serial.println("Setup complete! Starting blink loop...\n");
  
  // Display initial RAM usage
  printMemoryInfo();
  Serial.println();
}

void loop() {
  // Turn the LED ON (active LOW, so use LOW)
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED: ON");
  
  // Display RAM usage
  printMemoryInfo();
  Serial.println();
  
  delay(1000);
  
  // Turn the LED OFF (active LOW, so use HIGH)
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED: OFF");
  
  delay(1000);
}