#include <math.h> // Necesario para la función Sigmoide (expf)

// ------------------------------------------------------------------
// PINES Y CONFIGURACIÓN DEL HARDWARE (TCS3200)
// ------------------------------------------------------------------

// Pines del Sensor TCS3200
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define OUT_TCS 8 

// Constantes de tiempo
const int LECTURA_DELAY_MS = 100;
const int CICLO_DELAY_MS = 4000;

// ------------------------------------------------------------------
// CALIBRACIÓN Y RANGOS (Valores en microsegundos)


// Valores MÍNIMOS (Blanco, pulso corto)
const int R_MIN = 3;
const int G_MIN = 3;
const int B_MIN = 3;
const int C_MIN = 1;

// Valores MÁXIMOS (Negro, pulso largo)
const int R_MAX = 250;
const int G_MAX = 250;
const int B_MAX = 210; 
const int C_MAX = 80;

// ------------------------------------------------------------------
// PARÁMETROS DE LA RED NEURONAL (MLP 4-4-1) 

// W1: Pesos de entrada (4) a oculta (4). Matriz [4][4]
const float W1[4][4] = {{-0.82595450,0.30097684,-0.56078386,-0.73785502},
{-0.20391315,0.50615323,0.29051006,0.62281859},
{-0.12707019,-0.93947381,0.00817984,0.05740469},
{0.50265807,-0.01326199,0.25435102,0.27062473},
};

// B_oculta: Sesgos de la capa oculta (4 elementos)
const float B_oculta[4] =  {0.00000000,-2.06350255,-0.70413309,1.80208373};

// W2: Pesos de oculta (4) a salida (1). Vector [4]
const float W2[4] = {-1.07967234,-0.29689160,-0.76637059,0.10349683};

// B2: Sesgo de la capa de salida (1 elemento)
const float B2 =2.45807385;

// Umbral de decisión para la salida Sigmoide [0, 1]
const float UMBRAL_DECISION = 0.5; 

// ------------------------------------------------------------------
// === FUNCIONES AUXILIARES TCS3200 ===


int leerColor(int s2_estado, int s3_estado) { 
 digitalWrite(S2, s2_estado);
 digitalWrite(S3, s3_estado);
 delay(LECTURA_DELAY_MS); 
 return pulseIn(OUT_TCS, LOW);
}

int normalizar(int periodo, int min_periodo, int max_periodo) {
 periodo = constrain(periodo, min_periodo, max_periodo);
 return map(periodo, min_periodo, max_periodo, 255, 0); 
}

// ------------------------------------------------------------------
// FUNCIONES DE SOPORTE DE LA RED NEURONAL

// Activación ReLU (Capa Oculta)
float relu(float x) {
 return max(0.0, x);
}

// Activación Sigmoide (Capa de Salida)
float sigmoid_func(float x) {
// Utiliza expf para flotantes
 return 1.0 / (1.0 + expf(-x));
}

// Clasificación Binaria (Manzana=1, Naranja=0)
String clasificar(float z_out) {
// Si la probabilidad es 0.5 o mayor, es Manzana
 if (z_out >= UMBRAL_DECISION) {
  return "manzana"; 
 } 
// Si la probabilidad es menor que 0.5, es Naranja
 else {
  return "naranja"; 
 }
}

// ------------------------------------------------------------------
// FUNCIÓN PRINCIPAL DE INFERENCIA (Forward Propagation del MLP)


void procesarLectura() {
// 1. Lectura y NORMALIZACIÓN al rango [0, 255]
 int rojo_raw = leerColor(LOW, LOW); 
 int verde_raw = leerColor(HIGH, HIGH);
 int azul_raw = leerColor(LOW, HIGH);
 int claro_raw = leerColor(HIGH, LOW); 
 
 float entradas[4]; 
// Datos normalizados [0-255] - SIN ESTANDARIZACIÓN
 entradas[0] = (float)normalizar(rojo_raw, R_MIN, R_MAX);
 entradas[1] = (float)normalizar(verde_raw, G_MIN, G_MAX);
 entradas[2] = (float)normalizar(azul_raw, B_MIN, B_MAX);
 entradas[3] = (float)normalizar(claro_raw, C_MIN, C_MAX); 
// 3. CÁLCULO DE LA CAPA OCULTA (4 -> 4)
 float H[4] = {0.0, 0.0, 0.0, 0.0};
 
 for (int j = 0; j < 4; j++) { 
    float z_j = B_oculta[j]; 
 
// Suma ponderada
    for (int i = 0; i < 4; i++) { 
      z_j += W1[i][j] * entradas[i];
    }
    
    H[j] = relu(z_j); // Aplicación de ReLU
  } 

// 4. CÁLCULO DE LA CAPA DE SALIDA (4 -> 1)
  float Z_linear = B2;
 
  for (int j = 0; j < 4; j++) { 
    Z_linear += W2[j] * H[j];
  }
  
// APLICACIÓN CRÍTICA DE SIGMOIDE: La salida es una PROBABILIDAD (0 a 1)
  float Z_out = sigmoid_func(Z_linear); 
  
// 5. CLASIFICACIÓN Y ENVÍO POR SERIAL
  String resultado = clasificar(Z_out);

// Imprimimos la probabilidad para depuración
 Serial.print("Probabilidad (0=Naranja, 1=Manzana): "); Serial.print(Z_out, 4); 
 Serial.print(" -> Clasificacion: "); Serial.println(resultado);
}

// ------------------------------------------------------------------
// SETUP Y LOOP


void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT_TCS, INPUT);

  Serial.begin(9600); 
  delay(100); 

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  Serial.println("==================================================");
  Serial.println("INICIO: Clasificador Neuronal de Frutas (MLP 4-4-1)");
  Serial.println("ACTIVACION: Sigmoide (Salida es Probabilidad 0-1)");
  Serial.println("==================================================");
} 

void loop() {
  procesarLectura();
  delay(CICLO_DELAY_MS); 
}