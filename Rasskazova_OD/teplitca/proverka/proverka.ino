#include <dht.h>

#define PIN_LIGHT_SENSOR A0  // датчик освещенности
#define PIN_LIGHT 6  // светодиодная лента
#define PIN_HUMIDITY_SENSOR A1  // датчик влажности почвы 0 - влажно 1023 - сухо
#define PIN_WATER_PUMP 5  // водяной насос
#define PIN_DHT_SENSOR 2  // датчик температуры и влажности воздуха
#define PIN_VEN_HEAT 4  // нагреватель вентилятора 
#define PIN_VEN 7  // вентилятор

#define ON 1  // включение цифровых выходов
#define OFF 0  // выключение цифровых выходов
#define DAY 1  // день
#define NIGHT 0  // ночь
#define FIX 1  // нуждается в принятии мер
#define OK 0  // не нуждается в принятии мер

#include <TroykaDHT.h>
DHT dht_sensor(PIN_DHT_SENSOR, DHT21);

struct Sensors  {
  int hours;
  int minutes;
  int luminosity; 
  double air_temp; 
  double air_humidity; 
  int ground_humidity;
}; 

Sensors sens;

void get_sensors()  // снятие данных с датчиком
{
  dht_sensor.read();
  sens.ground_humidity = analogRead(PIN_HUMIDITY_SENSOR);
  sens.luminosity = analogRead(PIN_LIGHT_SENSOR);
  sens.air_temp = dht_sensor.getTemperatureC();
  sens.air_humidity = dht_sensor.getHumidity();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht_sensor.begin();
  pinMode(PIN_LIGHT, OUTPUT);
  pinMode(PIN_WATER_PUMP, OUTPUT);
  pinMode(PIN_DHT_SENSOR, INPUT);
  pinMode(PIN_VEN_HEAT, OUTPUT);
  pinMode(PIN_VEN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
   delay(1500);

  get_sensors();

   Serial.print("Часы: ");
   Serial.println(sens.hours);
   Serial.print("Минуты: ");
   Serial.println(sens.hours);
   Serial.print("Освященность: ");
   Serial.println(sens.luminosity);
   Serial.print("Темп возд:");
   Serial.println(sens.air_temp);
   Serial.print("Влаж возд:");
   Serial.println(sens.air_humidity);
   Serial.print("Влаж почв:");
   Serial.println(sens.ground_humidity);
}
