
int sensorPin = 34; // GPIO pin for turbidity sensor

void setup() { 
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(sensorPin);  // Read turbidity sensor value
  Serial.print("Raw sensor value: ");
  Serial.println(sensorValue);  // Output the raw sensor value for debugging

  // Adjust the mapping (sensorValue mapped from 0-1900 to 0-190 NTU)
  // Adjust these values based on calibration data
  int turbidity = map(sensorValue, 0, 1900, 190, 0);  
  Serial.print("Turbidity (NTU): ");
  Serial.println(turbidity);  // Output turbidity value

  // Adjust thresholds based on calibration results
 ```````````````` if (turbidity <= 10) {
    Serial.println("Water status: Clear");
  } else if (turbidity > 10 && turbidity <= 30) {
    Serial.println("Water status: Fairly turbid");
  } else if (turbidity > 30 && turbidity <= 60) {
    Serial.println("Water status: Rather turbid");
  } else if (turbidity > 60 && turbidity <= 90) {
    Serial.println("Water status: Turbid");
  } else if (turbidity > 90) {
    Serial.println("Water status: Very turbid");
  }

  delay(1000);  // Delay for 1 second before next reading
}