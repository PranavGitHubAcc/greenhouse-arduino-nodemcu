#include <Servo.h>
#include <Arduino.h>

class ServoControls : public Servo {
  // write(int position) is defined in Servo
  public:

  void action();
  void test();
};

inline void ServoControls::action() {
  const int CLOSE_SHUTTER = 180;
  this->write(CLOSE_SHUTTER);
};

inline void ServoControls::test() {
  for (int i = 0; i<180;i+=30) {
    this->write(i);
    delay(100);
  }
  for (int i = 180; i>0;i-=30) {
    this->write(i);
    delay(100);
  }
  Serial.println("Servo Test Done.");
}
