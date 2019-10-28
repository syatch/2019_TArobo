#include <Wire.h>
#include <stdlib.h>
#include "ti2c.h"
#include "kinjo_stp_motor_driver.h"
#include "param.h"

uint8_t addr = 0x33;
KinjoStMotorDriver m1 = KinjoStMotorDriver(addr);

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  int i;
  for (i = 0 ; i < 200 ; i++)
  {
    double angle = params[i][2];
    int degree = angle / M_PI * 180;
    while (degree < 0)
      degree += 360;
    degree =  (int)degree % 360;
    m1.rotate_degree(degree);
    Serial.println(degree);
    delay(100);
  }
}
