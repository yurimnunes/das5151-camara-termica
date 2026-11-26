// =====================================================================
// DAS5151 - Câmara térmica | Código de TESTE dos módulos
// ---------------------------------------------------------------------
// - Lê o DS18B20 (temperatura ambiente) no pino D2, sem travar o loop
// - Lê a saída do amplificador com LM324 (Pt100) no A0 e estima a temperatura
// - Controla o PWM do aquecedor no D3 pelo Serial Monitor
//
// Bibliotecas (Ferramentas > Gerenciar Bibliotecas):
//   - OneWire (Paul Stoffregen)
//   - DallasTemperature (Miles Burton)
//
// Serial Monitor: 9600 baud, "Nova linha".
//   Digite um número de 0 a 100 + Enter -> PWM em %
//   (limitado a PWM_MAX_PCT por segurança)
// =====================================================================

#include <OneWire.h>
#include <DallasTemperature.h>

// ---------------- Pinos ----------------
const int PINO_DS18B20 = 2;   // dados do DS18B20 (com pull-up 3,3 kΩ para 5 V)
const int PINO_PWM     = 3;   // D3 -> R1 -> LED do PC817
const int PINO_PT100   = A0;  // saída do LM324, pino 8 (após o filtro RC)

// ---------------- Segurança ----------------
const int   PWM_MAX_PCT   = 80;    // limite de PWM nos testes (%)
const float T_MAX_CAMARA  = 60.0;  // acima disso o aquecedor desliga (°C)
const float V_A0_MAX      = 3.4;   // LM324 saturado em cima (saída máx. ~3,5 V)
const float V_A0_MIN      = 0.15;  // saída presa embaixo: amplificador ou ponte em falha

// ---------------- Conversão do Pt100 ----------------
// Amplificador com LM324 (o INA125P queimou, 2026-11-26), G ≈ 119
// (Rf 2,2k, Rg 1k, Rd1 1k, Rd2 22k): 44,4 mV/°C, 1,110 V a 25 °C. Faixa útil: ~3 a ~76 °C.
// Valores teóricos: ajustar depois com a calibração em 2 pontos.
const float SENS_V_POR_C = 0.0444;
const float V_REF_25C    = 1.110;

OneWire oneWire(PINO_DS18B20);
DallasTemperature ds18b20(&oneWire);

int pwmPct = 0;
unsigned long ultimoPrint = 0;
float tAmbiente = NAN;

void aplicarPWM(int pct) {
  pct = constrain(pct, 0, PWM_MAX_PCT);
  pwmPct = pct;
  analogWrite(PINO_PWM, map(pct, 0, 100, 0, 255));
}

float lerTensaoA0() {
  // média de 16 leituras para reduzir ruído
  long soma = 0;
  for (int i = 0; i < 16; i++) soma += analogRead(PINO_PT100);
  return (soma / 16.0) * 5.0 / 1023.0;
}

void setup() {
  pinMode(PINO_PWM, OUTPUT);
  aplicarPWM(0);                       // começa com o aquecedor desligado
  Serial.begin(9600);

  ds18b20.begin();
  ds18b20.setResolution(12);           // 0,0625 °C
  ds18b20.setWaitForConversion(false); // não trava o loop (750 ms)
  ds18b20.requestTemperatures();

  Serial.println(F("Teste da camara termica"));
  Serial.print(F("DS18B20 encontrados: "));
  Serial.println(ds18b20.getDeviceCount());
  Serial.println(F("Digite 0 a 100 + Enter para o PWM (%)."));
  Serial.println(F("tempo_s\tPWM_%\tA0_V\tT_pt100_C\tT_amb_C"));
}

void loop() {
  // ---- comando de PWM pelo Serial ----
  if (Serial.available()) {
    int valor = Serial.parseInt();
    while (Serial.available()) Serial.read();  // limpa o resto da linha
    aplicarPWM(valor);
    Serial.print(F(">> PWM ajustado para "));
    Serial.print(pwmPct);
    Serial.println(F(" %"));
  }

  // ---- leitura e impressão a cada 1 s ----
  if (millis() - ultimoPrint >= 1000) {
    ultimoPrint = millis();

    // DS18B20: pega o resultado da conversão anterior e dispara a próxima
    float t = ds18b20.getTempCByIndex(0);
    if (t != DEVICE_DISCONNECTED_C) tAmbiente = t;
    ds18b20.requestTemperatures();

    float vA0 = lerTensaoA0();
    float tPt100 = 25.0 + (vA0 - V_REF_25C) / SENS_V_POR_C;

    // proteção: sobretemperatura ou leitura fora da faixa.
    // Saída presa embaixo também desliga: com o INA queimado a leitura ficava
    // perto de 0 V e a proteção por temperatura nunca atuaria.
    if (tPt100 > T_MAX_CAMARA || vA0 > V_A0_MAX || vA0 < V_A0_MIN) {
      if (pwmPct > 0) Serial.println(F("!! Protecao: aquecedor desligado"));
      aplicarPWM(0);
    }

    Serial.print(millis() / 1000);   Serial.print('\t');
    Serial.print(pwmPct);            Serial.print('\t');
    Serial.print(vA0, 3);            Serial.print('\t');
    Serial.print(tPt100, 2);         Serial.print('\t');
    if (isnan(tAmbiente)) Serial.println(F("erro"));
    else                  Serial.println(tAmbiente, 2);
  }
}
