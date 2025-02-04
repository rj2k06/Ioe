// Pin Definitions for 4-digit 7-segment display
byte D1 = 2;  // Digit 1
byte D2 = 3;  // Digit 2
byte D3 = 4;  // Digit 3
byte D4 = 5;  // Digit 4

byte A = 6;   // Segment A
byte B = 7;   // Segment B
byte C = 8;   // Segment C
byte D = 9;   // Segment D
byte E = 10;  // Segment E
byte F = 11;  // Segment F
byte G = 12;  // Segment G


const int segmentPins[] = {A, B, C, D, E, F, G};  // Segment pins
const int digitPins[] = {D1, D2, D3, D4};  // Digit control pins

// For common cathode 7-segment display
uint8_t digitON = LOW;  // Active low for common cathode
uint8_t digitOFF = HIGH; // Inactive high

uint8_t segmentON = HIGH; // Active high for segments
uint8_t segmentOFF = LOW; // Inactive low

// Moisture sensor and pump relay pins
const int moistureSensorPin = A0;  // Analog pin for the moisture sensor
const int relayPin = 13;           // Digital pin for the relay control (use digital pin on Arduino)

void setup() {
  // Initialize segment pins as output
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Initialize digit control pins as output
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
  }

  // Initialize relay pin as output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Start with relay off (pump off)

  // Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read moisture value from sensor (0 to 1023)
  int moistureValue = analogRead(moistureSensorPin);
  Serial.print("Moisture Level: ");
  Serial.println(moistureValue);

  // Check moisture level and control pump
  bool isDry = (moistureValue > 250); // Adjust threshold as needed
  
  if (isDry) {
    // Soil is dry; turn on the pump
    digitalWrite(relayPin, LOW);  // LOW to activate the relay for common relay modules
    displayData("LOW");           // Display "LOW" for dry condition
  } else {
    // Soil is wet; turn off the pump
    digitalWrite(relayPin, HIGH); // HIGH to deactivate the relay
    displayData("HIGH");          // Display "HIGH" for wet condition
  }

  delay(1000); // Delay before next moisture reading
}

// Function to display text on the 4-digit 7-segment display
void displayData(String text) {
  for (int i = 0; i < 100; i++) {  // Repeat to make display appear stable
    if (text == "HIGH") {
      displayDigit(1, 0);  // Display "H"
      displayDigit(2, 1);  // Display "I"
      displayDigit(3, 2);  // Display "G"
      displayDigit(1, 3);  // Display "H"
    } else if (text == "LOW") {
      displayDigit(0, 0);  // Display "L"
      displayDigit(4, 1);  // Display "O"
      displayDigit(5, 2);  // Display "W"
    }
  }
}

// Function to display a single character on the 7-segment display
void displayDigit(int charCode, int digitPos) {
  const byte segmentPatterns[6] = {
    0b00001110,  // L: 
    0b00110111,  // H: 
    0b00000110,  // I: 
    0b01011110,  // G: 
    0b01111110,  // O
    0b00000000,   // 
  };

  // Set the segments for the given character
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], bitRead(segmentPatterns[charCode], 6 - i) == 1 ? segmentON : segmentOFF);
  }

  
  for (int i = 0; i < 4; i++) {
    digitalWrite(digitPins[i], (i == digitPos) ? digitON : digitOFF);
  }

  delay(5);  // Slight delay for stability
}
