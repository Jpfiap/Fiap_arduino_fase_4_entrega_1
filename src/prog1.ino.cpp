# 1 "C:\\Users\\Jp\\AppData\\Local\\Temp\\tmpvy3c_ccr"
#include <Arduino.h>
# 1 "C:/Users/Jp/Desktop/FIAO Farmtech solution fase 4/Fiap_arduino_fase_3_entrega_1-main/Fiap_arduino_fase_3_entrega_1-main/src/prog1.ino"
#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define DHTPIN 23
#define DHTTYPE DHT22
#define BUTTON_P 22
#define BUTTON_K 21
#define LDR_PIN 34
#define RELAY_PIN 13
#define LCD_SDA 21
#define LCD_SCL 22


struct SensorData {
    bool fosforoOK;
    bool potassioOK;
    float ph;
    float umidade;
} sensorData;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);


const uint32_t INTERVALO_LEITURA = 3000;
uint32_t ultimaLeitura = 0;
void setup();
void lerSensores();
void atualizarDisplay();
void loop();
#line 31 "C:/Users/Jp/Desktop/FIAO Farmtech solution fase 4/Fiap_arduino_fase_3_entrega_1-main/Fiap_arduino_fase_3_entrega_1-main/src/prog1.ino"
void setup() {
    Serial.begin(115200);

    Wire.begin(LCD_SDA, LCD_SCL);
    lcd.init();
    lcd.backlight();

    pinMode(BUTTON_P, INPUT_PULLUP);
    pinMode(BUTTON_K, INPUT_PULLUP);
    pinMode(LDR_PIN, INPUT);
    pinMode(RELAY_PIN, OUTPUT);

    dht.begin();
    digitalWrite(RELAY_PIN, LOW);

    Serial.println("Sistema de Irrigação - Milho");
    lcd.print("Sistema Irrigacao");
}


void lerSensores() {
    sensorData.fosforoOK = digitalRead(BUTTON_P) == LOW;
    sensorData.potassioOK = digitalRead(BUTTON_K) == LOW;

    int ldrValue = analogRead(LDR_PIN);
    sensorData.ph = map(ldrValue, 0, 4095, 0, 140) / 10.0;
    sensorData.umidade = dht.readHumidity();
}


void atualizarDisplay() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("U:");
    lcd.print(sensorData.umidade, 1);
    lcd.print("% pH:");
    lcd.print(sensorData.ph, 1);

    lcd.setCursor(0, 1);
    lcd.print("P:");
    lcd.print(sensorData.fosforoOK ? "OK" : "NO");
    lcd.print(" K:");
    lcd.print(sensorData.potassioOK ? "OK" : "NO");
}

void loop() {
    uint32_t tempoAtual = millis();

    if (tempoAtual - ultimaLeitura >= INTERVALO_LEITURA) {
        lerSensores();

        bool phOK = (sensorData.ph >= 5.5 && sensorData.ph <= 7.0);
        bool umidadeOK = !isnan(sensorData.umidade) &&
                         sensorData.umidade >= 40.0 &&
                         sensorData.umidade <= 70.0;


        Serial.print("Umidade:");
        Serial.print(sensorData.umidade);
        Serial.print(",pH:");
        Serial.print(sensorData.ph);
        Serial.print(",Fosforo:");
        Serial.print(sensorData.fosforoOK ? 100 : 0);
        Serial.print(",Potassio:");
        Serial.println(sensorData.potassioOK ? 100 : 0);


        if (sensorData.umidade >= 70.0) {
            digitalWrite(RELAY_PIN, LOW);
        } else if (!sensorData.fosforoOK || !sensorData.potassioOK ||
                   !phOK || sensorData.umidade < 40.0) {
            digitalWrite(RELAY_PIN, HIGH);
        } else {
            digitalWrite(RELAY_PIN, LOW);
        }

        atualizarDisplay();
        ultimaLeitura = tempoAtual;
    }
}