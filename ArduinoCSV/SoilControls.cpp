#include <Arduino.h>

class SoilControls {
  uint8_t pin_number;

  public:
  int analog_state;
  float moisture_percentage;
  SoilControls(uint8_t _pin) {
    pin_number = _pin;
  }
  void read();
  void display();
  void action();
};

inline void SoilControls::read() {
  analog_state = analogRead(pin_number);
  moisture_percentage = (100 - ((analog_state / 1023.00) * 100));
};

inline void SoilControls::display() {
  Serial.print("Moisture Percentage = ");
  Serial.println(moisture_percentage);
};

inline void SoilControls::action() {
}