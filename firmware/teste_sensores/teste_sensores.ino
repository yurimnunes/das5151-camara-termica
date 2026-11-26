// =====================================================================
// DAS5151 - Câmara térmica | TESTE DOS SENSORES (sem acionamento)
// ---------------------------------------------------------------------
// - Medição interna: saída do INA125P no A0 (após o filtro RC)
// - Medição ambiente: DS18B20 no D2, leitura sem travar o loop
// - NÃO aciona o aquecedor: D3 fica em LOW o tempo todo
//
// Bibliotecas: OneWire, DallasTemperature.
// Serial: 9600 baud. Uma linha por segundo:
//   tempo_s  A0_cont  ruido_cont  A0_V  Vab_mV  T_pt100_C  T_amb_C
//   A0_cont    = média de 64 leituras do A/D (0 a 1023)
//   ruido_cont = maior menos menor leitura dentro das 64 (pico a pico)
//   Vab_mV     = tensão da ponte estimada (A0_V / ganho do INA)
//   T_amb_C    = "erro" se o DS18B20 não responder
// =====================================================================

#include <OneWire.h>
#include <DallasTemperature.h>

const int PINO_DS18B20 = 2;
const int PINO_PWM     = 3;   // garante o aquecedor desligado se o acionamento estiver montado
const int PINO_PT100   = A0;

// Mesma reta do teste_camara (simulação): 68,7 mV/°C, 1,736 V a 25 °C.
// Ajustar depois com a calibração em 2 pontos.
const float SENS_V_POR_C = 0.0687;
const float V_REF_25C    = 1.736;
const float GANHO_INA    = 4.0 + 60000.0 / 330.0;  // RG = 330 ohm
const float V_ADC_REF    = 5.0;
const int   N_AMOSTRAS   = 64;

OneWire oneWire(PINO_DS18B20);
DallasTemperature ds18b20(&oneWire);

unsigned long ultimoPrint = 0;

void setup() {
  pinMode(PINO_PWM, OUTPUT);
  digitalWrite(PINO_PWM, LOW);
  Serial.begin(9600);

  ds18b20.begin();
  ds18b20.setResolution(12);           // 0,0625 °C
  ds18b20.setWaitForConversion(false); // não trava o loop (750 ms)
  ds18b20.requestTemperatures();

  Serial.println(F("Teste dos sensores (Pt100 + DS18B20), aquecedor desligado"));
  Serial.print(F("DS18B20 encontrados: "));
  Serial.println(ds18b20.getDeviceCount());
  Serial.println(F("tempo_s\tA0_cont\truido_cont\tA0_V\tVab_mV\tT_pt100_C\tT_amb_C"));
}

void loop() {
  if (millis() - ultimoPrint < 1000) return;
  ultimoPrint = millis();

  // DS18B20: pega a conversão anterior e dispara a próxima
  float tAmb = ds18b20.getTempCByIndex(0);
  ds18b20.requestTemperatures();

  long soma = 0;
  int minimo = 1023, maximo = 0;
  for (int i = 0; i < N_AMOSTRAS; i++) {
    int v = analogRead(PINO_PT100);
    soma += v;
    if (v < minimo) minimo = v;
    if (v > maximo) maximo = v;
  }
  float cont = soma / (float)N_AMOSTRAS;
  float vA0  = cont * V_ADC_REF / 1023.0;
  float vab  = vA0 / GANHO_INA * 1000.0;
  float t    = 25.0 + (vA0 - V_REF_25C) / SENS_V_POR_C;

  Serial.print(millis() / 1000);   Serial.print('\t');
  Serial.print(cont, 1);           Serial.print('\t');
  Serial.print(maximo - minimo);   Serial.print('\t');
  Serial.print(vA0, 3);            Serial.print('\t');
  Serial.print(vab, 2);            Serial.print('\t');
  Serial.print(t, 2);              Serial.print('\t');
  if (tAmb == DEVICE_DISCONNECTED_C) Serial.println(F("erro"));
  else                               Serial.println(tAmb, 2);
}
