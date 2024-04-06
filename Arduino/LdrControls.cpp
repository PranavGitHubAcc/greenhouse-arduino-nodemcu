#include <Arduino.h>

class LdrControls{
  uint8_t pin_number;

  public:
  int state;
  LdrControls(uint8_t _pin){
    pin_number = _pin;
  }
  void read();
  void display();
  void action();
};

inline void LdrControls::read(){
  state = digitalRead(pin_number);
};

inline void LdrControls::display(){
  Serial.print("LDR State: ");
  Serial.println(state);
}