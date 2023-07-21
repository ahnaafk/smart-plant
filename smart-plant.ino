

//www.elegoo.com
//2018.10.25


#include <dht_nonblocking.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int lightPin = 0;
Servo motor;

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);



/*
 * Initialize the serial port.
 */
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Humidity (%)");
  motor.attach(3);
  motor.write(90);
}



/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment(float *temperature, float *humidity) {
  static unsigned long measurement_timestamp = millis();

  /* Measure once every four seconds. */
  if (millis() - measurement_timestamp > 3000ul) {
    if (dht_sensor.measure(temperature, humidity) == true) {
      measurement_timestamp = millis();
      return (true);
    }
  }
  return (false);
}

static bool restore_humidity(float humidity) {
  if (humidity < 60 ) {
    motor.write(0);
    delay(1000);
    motor.write(90);
    delay(1000);
  }
}

/*
 * Main program loop.
 */
void loop() {

  float temperature;
  float humidity;
  float light;
  light = analogRead(lightPin);
  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if (measure_environment(&temperature, &humidity) == true) {
    Serial.print(millis());
    Serial.print(" T = ");
    Serial.print(temperature, 1);
    Serial.print(" deg. C, H = ");
    Serial.print(humidity, 1);
    Serial.print("%, ");
    Serial.print("Light: ");
    Serial.println(light);
    restore_humidity(humidity);
  }
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(humidity);


}
