// =====================================================================
// DAS5151 - Câmara térmica | TESTE SÓ DA MEDIÇÃO INTERNA (ponte + INA125P)
// ---------------------------------------------------------------------
// - Lê a saída do INA125P no A0 (após o filtro RC) e estima a temperatura
// - NÃO lê o DS18B20 e NÃO aciona o aquecedor (D3 fica em LOW)
//
// Serial: 9600 baud. Uma linha por segundo:
//   tempo_s  A0_cont  ruido_cont  A0_V  Vab_mV  T_pt100_C
//   A0_cont    = média de 64 leituras do A/D (0 a 1023)
//   ruido_cont = maior menos menor leitura dentro das 64 (pico a pico)
//   Vab_mV     = tensão da ponte estimada (A0_V / ganho do INA)
// =====================================================================

const int PINO_PWM   = 3;    // garante o aquecedor desligado se o acionamento estiver montado
const int PINO_PT100 = A0;

// Mesma reta do teste_camara (simulação): 68,7 mV/°C, 1,736 V a 25 °C.
// Ajustar depois com a calibração em 2 pontos.
const float SENS_V_POR_C = 0.0687;
const float V_REF_25C    = 1.736;
const float GANHO_INA    = 4.0 + 60000.0 / 330.0;  // RG = 330 ohm
const float V_ADC_REF    = 5.0;
const int   N_AMOSTRAS   = 64;

unsigned long ultimoPrint = 0;

void setup() {
  pinMode(PINO_PWM, OUTPUT);
  digitalWrite(PINO_PWM, LOW);
  Serial.begin(9600);
  Serial.println(F("Teste da ponte (medicao interna)"));
  Serial.println(F("tempo_s\tA0_cont\truido_cont\tA0_V\tVab_mV\tT_pt100_C"));
}

void loop() {
  if (millis() - ultimoPrint < 1000) return;
  ultimoPrint = millis();

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
  Serial.println(t, 2);
}
