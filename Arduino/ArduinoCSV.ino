#include "Dht11Controls.cpp"
#include "PirControls.cpp"
#include "LdrControls.cpp"
#include "SoilControls.cpp"
#include "Mq135Controls.cpp"

// INPUT definitions
#define LDRPIN A5
#define MQ135PIN A2
#define SOILPIN A0
#define PIRPIN 2
#define DHT11PIN 8

Dht11Controls dht11(DHT11PIN);
LdrControls ldr(LDRPIN);
PirControls pir(PIRPIN);
SoilControls soil(SOILPIN);
Mq135Controls mq135(MQ135PIN);

void setup() {
  pinMode(PIRPIN, INPUT);
  pinMode(SOILPIN, INPUT);
  pinMode(LDRPIN, INPUT);
  pinMode(MQ135PIN, INPUT);
  pinMode(DHT11PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // ---- START ----- Reading and displaying sensor data --------
  ldr.read();
  mq135.read();
  soil.read();
  pir.read();
  dht11.read();

  // ---- END ----- Reading and displaying sensor data --------

  // --- START ----------- Send data to NodeMCU -----------------
  int humidity = dht11.humidity;  
  int temperature = dht11.temperature;
  int pir_state =  pir.state;
  int ldr_state = ldr.state;
  int co2_analog = mq135.analog_value;
  float soil_moisture = soil.moisture_percentage;

  if (ldr_state == 0) {
    ldr_state = 1;
  } else {
    ldr_state = 0;
  }

  Serial.print(humidity);
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(pir_state);
  Serial.print(",");
  Serial.print(ldr_state);
  Serial.print(",");
  Serial.print(co2_analog);
  Serial.print(",");
  Serial.println(soil_moisture);
  // ----- END --------- Send data to NodeMCU -----------------

  delay(1000);
}
