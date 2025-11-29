/*
 * Código de Calibración: Obtención de Valores Crudos (Pulsos)
 * Sensor TCS3200 con pines S0=4, S1=5, S2=6, S3=7, OUT=8.
 */

// === PINES DEL SENSOR TCS3200 ===
#define S0 4 
#define S1 5
#define S2 6
#define S3 7
#define OUT_TCS 8 

// Variables para almacenar los valores de pulso
long R_pulso, G_pulso, B_pulso, C_pulso; 

// Retardo para estabilización
const int LECTURA_DELAY_MS = 100; 


// === FUNCIONES ===

// Función para seleccionar el filtro y leer el pulso
int leerColor(int s2_estado, int s3_estado) { 
  digitalWrite(S2, s2_estado);
  digitalWrite(S3, s3_estado);
  delay(LECTURA_DELAY_MS);
  return pulseIn(OUT_TCS, LOW); 
}


void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT_TCS, INPUT);

  Serial.begin(9600);
  Serial.println("===============================================");
  Serial.println("LECTURA DE COLORES TCS3200");
  Serial.println("R = Rojo | G = Verde | B = Azul | C = Claro");
  Serial.println("===============================================");

  // Escala 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  delay(100);
}


void loop() {

  // 1. Rojo (S2=LOW, S3=LOW)
  R_pulso = leerColor(LOW, LOW);

  // 2. Verde (S2=HIGH, S3=HIGH)
  G_pulso = leerColor(HIGH, HIGH);

  // 3. Azul (S2=LOW, HIGH)
  B_pulso = leerColor(LOW, HIGH);

  // 4. Claro (S2=HIGH, LOW)
  C_pulso = leerColor(HIGH, LOW);

  // Imprimir valores crudos
  Serial.print("R: "); Serial.print(R_pulso);
  Serial.print(" | G: "); Serial.print(G_pulso);
  Serial.print(" | B: "); Serial.print(B_pulso);
  Serial.print(" | C (Claro): "); Serial.println(C_pulso);

  delay(500);
}
