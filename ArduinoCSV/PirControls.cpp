#include <Arduino.h>

class PirControls {
  uint8_t pin_number;
  public:
  int state;
  PirControls(uint8_t _pin) {
    pin_number = _pin;
  }
  void read();
  void display();
  void action();
};

inline void PirControls::read() {
  state = digitalRead(pin_number);
};

inline void PirControls::display() {
  Serial.print("PIR State: ");
  Serial.println(state);
}
