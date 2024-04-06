#include <Arduino.h>

class Mq135Controls {
  uint8_t pin_number;
  const float RESPONSE_FACTOR = 1.0; // Adjust this value based on your calibration
  const float BASELINE_VOLTAGE = 0.0; // Baseline voltage in Volts

  public:
  int analog_value;
  float moisture_percentage;
  float voltage, co2ppm;

  Mq135Controls(uint8_t _pin) {
    pin_number = _pin;
  }
  void read();
  void display();
  void action();
};

inline void Mq135Controls::read() {
  analog_value = analogRead(this->pin_number);
  moisture_percentage = (100 - ((analog_value / 1023.00) * 100));

  // Convert analog reading to CO2 concentration
  voltage = (analog_value * 5.0)/1023.0; // Assuming a 5V supply voltage
  co2ppm = (voltage - BASELINE_VOLTAGE) / RESPONSE_FACTOR;
};

inline void Mq135Controls::display() {
  Serial.print("Analog Value = ");
  Serial.println(analog_value);
  Serial.print("CO2 Voltage = ");
  Serial.println(voltage);
  Serial.print("PPM = ");
  Serial.println(co2ppm);
};

inline void Mq135Controls::action() {
}