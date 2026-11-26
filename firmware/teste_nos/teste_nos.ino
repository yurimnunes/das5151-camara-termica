// =====================================================================
// DAS5151 - Câmara térmica | ARDUINO COMO VOLTÍMETRO da medição interna
// ---------------------------------------------------------------------
// Diagnóstico sem multímetro: ligar jumpers dos nós até as entradas
// analógicas livres e comparar com os valores esperados (cerca de 25 °C):
//   A0 = saída do filtro (já ligado)      ~1,736 V
//   A1 = nó T (topo da ponte)             ~2,121 V
//   A2 = nó F (base da ponte)             ~1,919 V
//   A3 = nó a (VIN+, pino 6 do INA)       ~2,025 V
//   A4 = nó b (VIN-, pino 7 do INA)       ~2,016 V  (um pouco abaixo de a)
//   A5 = VO (pino 10 do INA)              ~1,736 V
// Entrada sem jumper flutua: ignorar essa coluna.
// D3 fica em LOW (aquecedor desligado). Serial 9600 baud, 1 linha/s.
// a_b_mV = A3 - A4: só o sinal (positivo) é confiável, resolução ~5 mV.
// =====================================================================

const int PINO_PWM = 3;
const int N_AMOSTRAS = 64;
const float V_ADC_REF = 5.0;

unsigned long ultimoPrint = 0;

float lerV(int pino) {
  analogRead(pino);  // descarta a 1a leitura após trocar de canal no multiplexador
  long soma = 0;
  for (int i = 0; i < N_AMOSTRAS; i++) soma += analogRead(pino);
  return (soma / (float)N_AMOSTRAS) * V_ADC_REF / 1023.0;
}

void setup() {
  pinMode(PINO_PWM, OUTPUT);
  digitalWrite(PINO_PWM, LOW);
  Serial.begin(9600);
  Serial.println(F("Arduino como voltimetro da medicao interna"));
  Serial.println(F("tempo_s\tA0_filtro\tA1_T\tA2_F\tA3_a\tA4_b\tA5_VO\ta_b_mV"));
}

void loop() {
  if (millis() - ultimoPrint < 1000) return;
  ultimoPrint = millis();

  float v[6];
  for (int i = 0; i < 6; i++) v[i] = lerV(A0 + i);

  Serial.print(millis() / 1000);
  for (int i = 0; i < 6; i++) { Serial.print('\t'); Serial.print(v[i], 3); }
  Serial.print('\t');
  Serial.println((v[3] - v[4]) * 1000.0, 1);
}
