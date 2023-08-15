#include <GyverPower.h>
#define millis() (millis() << (CLKPR & 0xF))
#define micros() (micros() << (CLKPR & 0xF))
#define delay(x) delay((x) >> (CLKPR & 0xf))
#define delayMicroseconds(x) delayMicroseconds((x) >> (CLKPR & 0xf))
#include "GyverTimer.h"
#define ENGINE_PIN 2
#define PHOTO_PIN A7
GTimer photoTime(MS);
GTimer waitFart(MS);
GTimer wePiss(MS);
uint32_t tmr;
byte fart=false;
int val;
void setup() {
  //Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(ENGINE_PIN, OUTPUT);
  photoTime.setInterval(500); //пол секунды prescaler_1 ставить 500
  power.autoCalibrate(); // автоматическая калибровка

  // отключение ненужной периферии
  power.hardwareDisable(PWR_SPI | PWR_I2C  ); // см раздел константы в GyverPower.h, разделяющий знак " | "

  // управление системной частотой
  power.setSystemPrescaler(PRESCALER_2); // см константы в GyverPower.h
  
  // настройка параметров сна
  power.setSleepMode(POWERDOWN_SLEEP); // если нужен другой режим сна, см константы в GyverPower.h (по умолчанию POWERDOWN_SLEEP)
}

void loop() {
  // put your main code here, to run repeatedly:
if (photoTime.isReady()){ 
val = analogRead (PHOTO_PIN);
//Serial.println(String(val));
//Serial.println(fart);
if (val>500) {
if (waitFart.isEnabled()!=true) waitFart.setInterval(240000); //4 минуты 
if (wePiss.isEnabled()!=true && fart==false) wePiss.setInterval(120000); // 2 минуты 
}
else
{
  if (fart==true){
  
  if (waitFart.isEnabled()!=true) {
    fart=false;
    power.sleep(SLEEP_8192MS); // спим ~ 8 секунд  
  }
  }
  else {
    if (wePiss.isEnabled()) wePiss.stop();
    if (waitFart.isEnabled()) waitFart.stop();
    power.sleep(SLEEP_8192MS); 
  }
  
}

}
if (waitFart.isReady()){
  
  digitalWrite(ENGINE_PIN,HIGH);
  delay(700);
  digitalWrite(ENGINE_PIN,LOW);
  waitFart.stop();  
}
if (wePiss.isReady()){
 fart=true;
 wePiss.stop(); 
}
}
