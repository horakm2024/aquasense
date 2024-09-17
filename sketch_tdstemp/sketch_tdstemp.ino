#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 13  // GPIO pin where DS18B20 is connected
#define TdsSensorPin 34  // GPIO pin where the TDS sensor is connected
#define VREF 3.3        // Analog reference voltage (Volt) of the ADC
#define SCOUNT 30       // Number of samples for averaging

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int analogBuffer[SCOUNT];     // Buffer to store analog readings
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;

float averageVoltage = 0;
float tdsValue = 0;
float temperature;  // Temperature from DS18B20 sensor

// Function to get the median value from an array
int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (int i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  
  // Sort the array
  for (int j = 0; j < iFilterLen - 1; j++) {
    for (int i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        int temp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = temp;
      }
    }
  }
  
  // Return the median
  if (iFilterLen % 2 == 1) {
    return bTab[(iFilterLen - 1) / 2];
  } else {
    return (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TdsSensorPin, INPUT);
  
  // Initialize DS18B20 temperature sensor
  sensors.begin();
}

void loop() {
  // Request temperature from DS18B20 sensor
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);  // Read temperature in Celsius

  // Read the analog value from the TDS sensor every 40 milliseconds
  static unsigned long lastAnalogReadTime = millis();
  if (millis() - lastAnalogReadTime > 40U) {
    lastAnalogReadTime = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);  // Store the analog reading
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT) {
      analogBufferIndex = 0;
    }
  }

  // Process and output every 800 milliseconds
  static unsigned long lastPrintTime = millis();
  if (millis() - lastPrintTime > 800U) {
    lastPrintTime = millis();

    // Copy buffer values for median filtering
    for (int i = 0; i < SCOUNT; i++) {
      analogBufferTemp[i] = analogBuffer[i];
    }

    // Calculate the average voltage using the median filtered value
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 4096.0;

    // Temperature compensation formula: adjust based on the sensor's specifications
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    float compensationVoltage = averageVoltage / compensationCoefficient;

    // Convert voltage value to TDS value
    tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
                - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;

    // Output TDS value and temperature
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    
    Serial.print("TDS Value: ");
    Serial.print(tdsValue, 0);
    Serial.println(" ppm");
  }
}