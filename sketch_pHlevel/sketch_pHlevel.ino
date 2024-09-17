#define pH_PIN 34  // Analog pin to which pH sensor is connected

float voltage, pH_value;

void setup() {
  Serial.begin(115200);  // Start serial communication
  pinMode(pH_PIN, INPUT);  // Set pH pin as input
}

void loop() {
  int sensorValue = analogRead(pH_PIN);  
  voltage = sensorValue * (3.3 / 4095.0);  // Convert sensor value to voltage
  
  // Adjust the calibration formula
  // Assuming that pH = 7 at around 2.5V (neutral water)
  // We can fine-tune this equation based on your actual readings:
  pH_value = 3.6 * voltage + 2.1;  // Adjust this multiplier and offset as needed

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" V, pH Value: ");
  Serial.println(pH_value);

// Check pH range and print appropriate message
if (pH_value >= 6.0 && pH_value <= 8.5) {
    Serial.println("The water is more natural.");
} else if (pH_value < 6.0) {
    Serial.println("The water is acidic.");
} else if (pH_value > 8.5) {
    Serial.println("The water is more alkaline.");
}

delay(1000);  // Wait for 1 second before next reading
}