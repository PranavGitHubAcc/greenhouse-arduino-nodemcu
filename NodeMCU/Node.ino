#include <Servo.h>
#include "thingProperties.h"

#define FANRELAYPIN D1
#define PUMPRELAYPIN D2
#define SERVOPIN D8

int count = 0;
int humidity = 0;
int temperature = 0;
int pir_state = 0;
int ldr_state = 0;
int co2_analog = 0;
float soil_moisture = 0.0;
int day_start = millis();
int ldr_state_previous = 0;
int light_prev;
int sun_time = 0;

Servo servo;

void serialFlush() {
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}

void closeShutters(Servo servo) {
  servo.write(0);
}

void openShutters(Servo servo) {
  servo.write(85);
}

void setup() {
  pinMode(FANRELAYPIN, OUTPUT);
  pinMode(PUMPRELAYPIN, OUTPUT);
  servo.attach(SERVOPIN, 500, 2400);
  digitalWrite(FANRELAYPIN, HIGH);
  digitalWrite(PUMPRELAYPIN, HIGH);
  Serial.begin(9600);
  delay(1500);

  initProperties();                                   // Defined in thingProperties.h
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  // Connect to Arduino IoT Cloud
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  if (Serial.available() > 0) {

    // ArduinoCloud.update();
    String data = Serial.readStringUntil('\n');  // Read data until a newline character
    int commaIndex = data.indexOf(',');

    if (commaIndex != -1) {
      humidity = data.substring(0, commaIndex).toInt();

      data = data.substring(commaIndex + 1);
      commaIndex = data.indexOf(',');
      temperature = data.substring(0, commaIndex).toInt();

      data = data.substring(commaIndex + 1);
      commaIndex = data.indexOf(',');
      pir_state = data.substring(0, commaIndex).toInt();

      data = data.substring(commaIndex + 1);
      commaIndex = data.indexOf(',');
      ldr_state = data.substring(0, commaIndex).toInt();
      if (ldr_state == 0) {
        ldr_state = 1;
      } else {
        ldr_state = 0;
      }

      data = data.substring(commaIndex + 1);
      commaIndex = data.indexOf(',');
      co2_analog = data.substring(0, commaIndex).toInt();

      data = data.substring(commaIndex + 1);
      soil_moisture = data.toFloat();
    }
    // ------- START ACTION code --------------
    if (ldr_state == 1 && ldr_state_previous == 0) {
      light_prev = millis();
      ldr_state_previous = 1;
    }

    else if (ldr_state == 0 && ldr_state_previous == 1) {
      sun_time += millis() - light_prev;
      ldr_state_previous = 0;
    }

    else if (ldr_state == 1 && ldr_state_previous == 1) {
      sun_time += millis() - light_prev;
      light_prev = millis();
      ldr_state_previous = 0;
    }

    if ((millis() - day_start) > 86400000) {
      day_start = millis();
      sun_time = 0;
    }

    if (pir_state == 1) {
      Serial.println("Motion detected.");
    }

    if (temperature > 35) {
      digitalWrite(FANRELAYPIN, LOW);
      delay(3000);
      digitalWrite(FANRELAYPIN, HIGH);
    }

    if (humidity > 50) {
      digitalWrite(FANRELAYPIN, LOW);
      delay(3000);
      digitalWrite(FANRELAYPIN, HIGH);
    }

    if (soil_moisture < 20) {
      digitalWrite(PUMPRELAYPIN, LOW);
      //iot_water_pump = true;
      delay(3000);
      digitalWrite(PUMPRELAYPIN, HIGH);
      //iot_water_pump = false;
    }

    delay(1000);
    // ------- END ACTION code --------------

    // ------- START PRINTING ------------
    Serial.println("------- Sensor Data -------");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");

    Serial.print("PIR State: ");
    Serial.println(pir_state);

    Serial.print("LDR State: ");
    Serial.println(ldr_state);

    Serial.print("CO2 Analog Value: ");
    Serial.println(co2_analog);

    Serial.print("Soil Moisture: ");
    Serial.println(soil_moisture);
    Serial.println();

    Serial.println("------ System Information ------- ");
    Serial.print("Day started: ");
    Serial.print((millis() - day_start) / 1000);
    Serial.println(" seconds ago");

    Serial.print("Sunlight: ");
    Serial.print(sun_time / 1000);
    Serial.println(" seconds");
    Serial.println();
    // ---------- END PRINTING -----------

    serialFlush();  // Flush Input Buffer

    // ------- START IOT VARIABLE UPDATES ---------
    iot_co2 = co2_analog;
    iot_soil_moisture = soil_moisture;
    iot_humidity = humidity;
    iot_temperature = temperature;
    iot_water_pump = false;
    // ------- END IOT VARIABLE UPDATES ---------
  }
}

void onIotWaterPumpChange() {
  if (iot_water_pump == true) {
    digitalWrite(PUMPRELAYPIN, LOW);
    delay(3000);
    digitalWrite(PUMPRELAYPIN, HIGH);
    iot_water_pump = false;
  }
}
