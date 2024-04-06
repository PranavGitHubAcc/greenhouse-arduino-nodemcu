#include <dht.h>
#include <Arduino.h>

class Dht11Controls : public dht {
  // read11(pinNumber) function is defined inside dht lib
  // .humidity and .temperature are public variables defined inside dht lib
  uint8_t pinNumber;

  public:
  Dht11Controls(uint8_t _pin) {
    pinNumber = _pin;
  };

  void read();
  void display();
  void action();
};

inline void Dht11Controls::read() {
  this->read11(pinNumber);
};

inline void Dht11Controls::display() {
  Serial.print("Temperature = ");
  Serial.println(this->temperature);
  Serial.print("Humidity = ");
  Serial.println(this->humidity);
};

inline void Dht11Controls::action() {
  return;
};