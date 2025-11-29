  /*
  * Código COMPLETO para Envío Continuo CSV de VALORES NORMALIZADOS (R,G,B,Claro,Etiqueta)
  * Sensor de color TCS3200. Optimizado para Excel/Data Streamer.
  */

  // === PINES DEL SENSOR TCS3200 ===
  #define S0 4 
  #define S1 5 
  #define S2 6 
  #define S3 7 
  #define OUT_TCS 8 

  // === CONFIGURACIÓN Y CALIBRACIÓN ===
  const int LECTURA_DELAY_MS = 100; // Retardo para estabilizar la lectura
  const char* ETIQUETA_FRUTA = "manzana"; // Etiqueta genérica
  const int CICLO_DELAY_MS = 4000; // Retardo entre lecturas

  // Valores MÍNIMOS (Blanco, Periodo más corto, mapeado a 255)
  const int R_MIN = 35;
  const int G_MIN = 35;
  const int B_MIN = 30;
  const int C_MIN = 15;

  // Valores MÁXIMOS (Negro, Periodo más largo, mapeado a 0)
  const int R_MAX = 935;
  const int G_MAX = 935;
  const int B_MAX = 745; 
  const int C_MAX = 290; 


  // =========================================================
  // === FUNCIONES AUXILIARES ===
  // Función para leer un color específico (retorna el Periodo crudo en µs)
  int leerColor(int s2_estado, int s3_estado) { 
  digitalWrite(S2, s2_estado);
  digitalWrite(S3, s3_estado);
  delay(LECTURA_DELAY_MS); 
  return pulseIn(OUT_TCS, LOW);
  }

  // Función que normaliza el Periodo (crudo) a un valor de Intensidad (0-255)
  int normalizar(int periodo, int min_periodo, int max_periodo) {
  // Limita el periodo dentro del rango de calibración
  periodo = constrain(periodo, min_periodo, max_periodo);
  
  // Mapeo inverso: periodo corto (MÁS luz) -> 255, periodo largo (MENOS luz) -> 0
  return map(periodo, min_periodo, max_periodo, 255, 0); 
  }
  // Función principal para la captura de datos y envío CSV
  void procesarLectura() { 
  // 1. Lectura de Periodos (Raw Data)
  int rojo_raw = leerColor(LOW, LOW);// Rojo
  int verde_raw = leerColor(HIGH, HIGH);// Verde
  int azul_raw = leerColor(LOW, HIGH);// Azul
  int claro_raw = leerColor(HIGH, LOW);// Claro

  // 2. Normalización de Datos (0-255)
  int rojo_norm = normalizar(rojo_raw, R_MIN, R_MAX);
  int verde_norm = normalizar(verde_raw, G_MIN, G_MAX);
  int azul_norm = normalizar(azul_raw, B_MIN, B_MAX);
  int claro_norm = normalizar(claro_raw, C_MIN, C_MAX); // Normaliza también el canal Claro
  
  // 3. Envío de Datos Normalizados (Formato CSV: R_N, G_N, B_N, Claro_N, Etiqueta)
  Serial.print(rojo_norm);
  Serial.print(",");
  Serial.print(verde_norm);
  Serial.print(",");
  Serial.print(azul_norm);
  Serial.print(",");
  Serial.print(claro_norm); // Enviamos el valor Claro Normalizado
  Serial.print(",");

  Serial.println(ETIQUETA_FRUTA); 
  }


  // =========================================================
  // === CONFIGURACIÓN Y BUCLE PRINCIPAL ===
  

  void setup() {
  // Configuración de pines de I/O
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT_TCS, INPUT);

  // Inicialización de la comunicación serial
  Serial.begin(9600); 
  delay(100); 

  // Configuración de la escala de frecuencia de salida (20%)
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  // --- ENCABEZADO CSV para Data Streamer ---
  // Las 4 columnas de color son las entradas de la  red 4-4-1
  Serial.println("Rojo_Norm,Verde_Norm,Azul_Norm,Claro_Norm,Etiqueta");

  
  Serial.println("==================================================");
  Serial.println("INICIO: Envío CSV Normalizado. Listo para Data Streamer.");
  Serial.println("==================================================");
  }

  void loop() {
  // El Arduino lee, normaliza y envía los datos
  procesarLectura();

  // Retardo entre ciclos
  delay(CICLO_DELAY_MS); 
  }