#include <FS.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <Servo.h>
#include <LED.h>

Servo servo;

LED led;

WiFiManager wifiManager;

#define servoPin 4
#define TRIGGER_PIN 13
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLo0T5Fxeq"
#define BLYNK_DEVICE_NAME "IoT DKP"
#define BLYNK_AUTH_TOKEN "CrYrjCa50t5tjFblVcdNcrgL3MTxO0Ca"

char blynk_token[34] = "";
bool shouldSaveConfig = false;

void saveConfigCallback () {
  Serial.println(F("Should save config"));
  shouldSaveConfig = true;
}

void initSPIFFS() {
  Serial.println(F("mounting FS..."));
  if (SPIFFS.begin()) {
    Serial.println(F("mounted file system"));
    if (SPIFFS.exists("/token.txt")) {
      Serial.println(F("reading config file"));
      File configFile = SPIFFS.open("/token.txt", "r");
      if (configFile) {
        Serial.print(F("opened config file"));
        while (configFile.available()) {
          String line = configFile.readStringUntil('\n');
          strcpy(blynk_token, line.c_str());
          Serial.println(F(" success"));
        }
      } else {
        Serial.println(F("failed to load token"));
      }
      configFile.close();
    }
  } else {
    Serial.println(F("failed to mount FS"));
  }
}

void saveToken() {
  Serial.println("saving config");
  File configFile = SPIFFS.open("/token.txt", "w");
  if (!configFile) {
    Serial.println("file creation failed");
  } else {
    Serial.println("File Created!");
    configFile.println(blynk_token);
  }
  configFile.close();
}

void initialize() {
  initSPIFFS();

  WiFiManagerParameter custom_blynk_token("blynk", "Token Blynk", blynk_token, 34);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.addParameter(&custom_blynk_token);

  if (!wifiManager.autoConnect("DKP C", "12345678")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  Serial.println("connected");
  strcpy(blynk_token, custom_blynk_token.getValue());

  if (shouldSaveConfig) {
    saveToken();
  }
}

void checkButton() {
  if (digitalRead(TRIGGER_PIN) == 1 ) {
    delay(50);
    if ( digitalRead(TRIGGER_PIN) == 1 ) {
      Serial.println("Button Pressed");
      delay(1000);
      if ( digitalRead(TRIGGER_PIN) == 1 ) {
        for (int i = 1; i <= 4; i++) {
          led.hijau(1);
          delay(80);
          led.hijau(0);
          delay(80);
          led.hijau(1);
          delay(80);
          led.hijau(0);
          delay(500);
        }
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wifiManager.resetSettings();
      }
    }
  }
  else {
    Serial.println("Connecting");
    Blynk.begin(blynk_token, WiFi.SSID().c_str(), WiFi.psk().c_str(), "blynk.cloud", 80);
    Serial.println("Connected");
  }
}

BLYNK_WRITE(V5) {
  if (param.asInt() == HIGH) {
    led.hijau(1);
  }
  else {
    led.hijau(0);
  }
}

BLYNK_WRITE(V4) {
  if (param.asInt() == LOW) {
    led.biru(1);
  }
  else {
    led.biru(0);
  }
}

BLYNK_WRITE(V3) {
  servo.write(param.asInt());
}

BLYNK_WRITE(V2) {
  if (param.asInt() == LOW) {
    led.buzzer(1);
  }
  else {
    led.buzzer(0);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  servo.attach(servoPin);
  pinMode(TRIGGER_PIN, INPUT);
  initialize();
  checkButton();
}

void loop() {
  Blynk.run();
}