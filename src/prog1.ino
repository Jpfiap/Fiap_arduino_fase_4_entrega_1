#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 23
#define DHTTYPE DHT22

#define BUTTON_P 22      // Fósforo
#define BUTTON_K 21      // Potássio
#define LDR_PIN 34       // Sensor de pH simulado
#define RELAY_PIN 13     // Relé e LED

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_P, INPUT_PULLUP);
  pinMode(BUTTON_K, INPUT_PULLUP);
  pinMode(LDR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  dht.begin();
  digitalWrite(RELAY_PIN, LOW); // Desligado inicialmente

  Serial.println("Sistema de Irrigação - Milho");
}

void loop() {
  // Leitura dos sensores
  bool fosforoOK = digitalRead(BUTTON_P) == LOW;  // Presença: pressionado
  bool potassioOK = digitalRead(BUTTON_K) == LOW; // Presença: pressionado
  int ldrValue = analogRead(LDR_PIN);  // Simula pH
  float umidade = dht.readHumidity();

  // Mapeia LDR (0-4095) para pH 0 a 14
  float ph = map(ldrValue, 0, 4095, 0, 140) / 10.0;
  bool phOK = (ph >= 5.5 && ph <= 7.0);  // Faixa ideal para milho

  // Verifica se umidade está na faixa desejada (40% a 70%)
  bool umidadeOK = !isnan(umidade) && umidade >= 40.0 && umidade <= 70.0;

  // Exibe leituras no monitor serial
  Serial.println("=== Leitura Atual ===");
  Serial.print("Fósforo OK: "); Serial.println(fosforoOK ? "Sim" : "Não");
  Serial.print("Potássio OK: "); Serial.println(potassioOK ? "Sim" : "Não");
  Serial.print("pH: "); Serial.print(ph); Serial.print(" ("); Serial.print(phOK ? "OK" : "Fora da faixa"); Serial.println(")");
  Serial.print("Umidade: "); Serial.print(umidade); Serial.print("% ("); Serial.print(umidadeOK ? "OK" : "Fora da faixa"); Serial.println(")");
  Serial.println("=====================");

  // Lógica de controle da irrigação
  if (umidade >= 70.0) {
    digitalWrite(RELAY_PIN, LOW);   // Desativa bomba se umidade >= 70%
    Serial.println("🟢 Irrigação Inativa (Umidade >= 70%)");
  } else if (!fosforoOK || !potassioOK || !phOK || umidade < 40.0) {
    digitalWrite(RELAY_PIN, HIGH);  // Ativa bomba se há deficiência
    Serial.println("🔴 Irrigação Ativa (Deficiência detectada)");
  } else {
    digitalWrite(RELAY_PIN, LOW);   // Desativa bomba se tudo OK
    Serial.println("🟢 Irrigação Inativa (Tudo OK)");
  }

  delay(3000);  // Leitura a cada 3 segundos
}