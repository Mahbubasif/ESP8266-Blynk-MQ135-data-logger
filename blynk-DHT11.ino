#define BLYNK_PRINT Serial
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define MQ135pin A0
#define DHTTYPE DHT11
#define DHTPIN D3
float MQ135Value;

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <ThingSpeak.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char auth[] = "your auth token here";
char ssid[] = "your wifi ssid here";
char pass[] = "your wifi pass here";

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
void sendSensor()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  MQ135Value = analogRead(MQ135pin);

  if (isnan(humidity) || isnan(temperature) || isnan(MQ135Value))
  {
    Serial.println("Failed to read from DHT & MQ135 sensor!");
    return;
  }
  Blynk.virtualWrite(V5, humidity);
  Blynk.virtualWrite(V6, temperature);
  Blynk.virtualWrite(V4,MQ135Value);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}
 void loop() {
  delay(1000);
  display.clearDisplay();
  delay(500);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.print("Temperature:");
  display.println(dht.readTemperature());
  display.print("Humidity:");
  display.println(dht.readHumidity());
  display.println("AQI:");
  display.println(analogRead(MQ135pin));
  display.display();
  timer.setInterval(1000L, sendSensor);
  Blynk.run();
  timer.run();
 }
