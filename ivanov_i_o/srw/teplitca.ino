#define PIN_LIGHT_SENSOR A0
#define PIN_LIGHT 6
#define PIN_HUMIDITY_SENSOR A1
#define PIN_WATER_PUMP 5 
#define PIN_DHT_SENSOR 2
#define PIN_VEN_HEAT 4
#define PIN_VEN 7
#define ON 1
#define OFF 0


#include <TroykaDHT.h>
DHT dht_sensor(PIN_DHT_SENSOR, DHT21);


int PROGRAMM_CHECK_TIME = 1;


struct Climate {
  int norm_luminosity;
  double min_air_temp; 
  double max_air_temp; 
  double min_air_humidity; 
  double max_air_humidity; 
  int norm_soil_humidity;
  int watering_time;
  int ven_time;
}; 


struct Sensors {
  int hours;
  int minutes;
  int seconds;
  int luminosity;
  double air_temp;
  double air_humidity;
  double soil_humidity;
}; 


struct State {
  long long int ven_time;
  long long int watering_time;
  long long int last_watering;
  bool regular_ven;
  bool light;
  bool ven;
  bool pump;
  bool heat;
}; 

State state;
Climate clim;
Sensors sens;


void setup()
{
  Serial.begin(9600);
  dht_sensor.begin();
  pinMode(PIN_LIGHT, OUTPUT);
  pinMode(PIN_WATER_PUMP, OUTPUT);
  pinMode(PIN_DHT_SENSOR, INPUT);
  pinMode(PIN_VEN_HEAT, OUTPUT);
  pinMode(PIN_VEN, OUTPUT);
}


void plant()
{
  clim.norm_luminosity = 700;
  clim.min_air_temp = 18;
  clim.max_air_temp = 30;
  clim.norm_soil_humidity = 75;
  clim.min_air_humidity = 50;
  clim.max_air_humidity = 60;
  clim.watering_time = 10000;
  clim.ven_time = 60000;
}


void set_time()
{
  sens.seconds = millis() / 1000;

  if (sens.seconds == 60) {
    sens.minutes += 1;
    sens.seconds = 0;
  }

  if (sens.minutes == 60) {
    sens.hours += 1;
    sens.minutes = 0;
  }

  if (sens.hours == 24) {
    sens.hours = 0;
  } 
}


double soil_humidity_convert(int value)
{
  double persents;
  persents = (double)((1023 - value) * 100 / 1024);
  return persents;
}


void get_sensors()
{
  dht_sensor.read();

  sens.soil_humidity = soil_humidity_convert(analogRead(PIN_HUMIDITY_SENSOR));
  sens.luminosity = analogRead(PIN_LIGHT_SENSOR);
  sens.air_temp = dht_sensor.getTemperatureC();
  sens.air_humidity = dht_sensor.getHumidity();
}


void ventilation()  
{
  state.ven_time += PROGRAMM_CHECK_TIME * 1000;

  if (sens.minutes % 4 == 0) { 
    state.regular_ven = ON;
    state.ven_time = 0;
  } else {
    if (state.ven_time > clim.ven_time) {
      state.regular_ven = OFF;
    } else {
      state.regular_ven = ON;
    }
  }
}


void air_temp()
{
  if (sens.air_temp >= clim.min_air_temp && sens.air_temp <= clim.max_air_temp) {
    state.ven = OFF;
    state.heat = OFF;
  }

  if (sens.air_temp < clim.min_air_temp) {
    state.ven = ON;
    state.heat = ON;
  }

  if (sens.air_temp > clim.max_air_temp) {
    state.ven = ON;
    state.heat = OFF;
  }
}


void air_humidity()
{
  if (sens.air_humidity >= clim.min_air_humidity && sens.air_humidity <= clim.max_air_humidity) {
    state.ven = OFF;
    state.pump = OFF;
  }

  if (sens.air_humidity < clim.min_air_humidity) {
    state.ven = OFF;
    state.pump = ON;
  }

  if (sens.air_humidity > clim.max_air_humidity) {
    state.ven = ON;
    state.pump = OFF;
  }
}


void soil_humidity()
{
  if (sens.soil_humidity < clim.norm_soil_humidity) {
    state.pump = ON;
  } else {
    state.pump = OFF;
  }
}


void light()
{
  if (sens.luminosity >= clim.norm_luminosity) {
    state.light = OFF;
  } else {
    state.light = ON;
  }
}


void day_night()
{
  if (sens.hours < 6 || sens.hours > 22){
    state.regular_ven = ON;
    state.light = ON;
  }
}


void do_light()
{
  digitalWrite(PIN_LIGHT, state.light);
}


void do_heat()
{
  if (state.heat == ON) {
    digitalWrite(PIN_VEN, ON);
    digitalWrite(PIN_VEN_HEAT, ON);
  } else {
    digitalWrite(PIN_VEN_HEAT, OFF);
  }
}


void do_vent()
{
  if (state.regular_ven == ON){
    digitalWrite(PIN_VEN, ON);
  } else if (state.ven == ON) {
    digitalWrite(PIN_VEN, ON);
  } else {
    digitalWrite(PIN_VEN, OFF);
  }
}


void do_pump()
{ 
  if (state.last_watering > 300000) {

    if (state.pump == ON) {
      digitalWrite(PIN_WATER_PUMP, state.pump);
      state.watering_time += PROGRAMM_CHECK_TIME * 1000;

      if (state.watering_time > clim.watering_time) {
        digitalWrite(PIN_WATER_PUMP, OFF);
        state.watering_time = 0;
        state.last_watering = 0;
      } 
    } else {
      digitalWrite(PIN_WATER_PUMP, state.pump);
    }
  }
  state.last_watering += PROGRAMM_CHECK_TIME * 1000;
}


void periodic_check()
{
  get_sensors();

  air_temp();
  air_humidity();
  ventilation();
  soil_humidity();
  light();

  day_night();

  do_light();
  do_vent();
  do_heat();
  do_pump();
}


void loop()
{
  plant();
  set_time();

  if (sens.seconds % PROGRAMM_CHECK_TIME == 0) { 
    delay(800);
    periodic_check();
  }
}