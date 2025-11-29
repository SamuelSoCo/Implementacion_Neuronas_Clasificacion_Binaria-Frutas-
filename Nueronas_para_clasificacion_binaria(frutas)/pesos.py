import pandas as pd
from sklearn.model_selection import train_test_split
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
import numpy as np

# ====================================================================
# 1. CONFIGURACIÓN
# ====================================================================

NOMBRE_ARCHIVO_CSV = 'C:\\Arduino_uno\\Neuronas_Frutas\\final1.csv'
EPOCAS =1000 # Aumentamos las épocas para mejor convergencia con Sigmoid
TAMANO_LOTE = 5 

# ====================================================================
# 2. CARGA Y PRE-PROCESAMIENTO
# ====================================================================

try:
    df = pd.read_csv(NOMBRE_ARCHIVO_CSV)
except FileNotFoundError:
    print(f"ERROR: No se encontró el archivo {NOMBRE_ARCHIVO_CSV}. Verifica la ruta.")
    exit()

X = df[['Rojo_Norm', 'Verde_Norm', 'Azul_Norm', 'Claro_Norm']].values
y = df['Fruta'].values # y es la etiqueta numérica (0 para Naranja, 1 para Manzana)

# Aquí las etiquetas quedan como 0 y 1 (binario).
y_train, y_test = y[:int(len(y)*0.8)], y[int(len(y)*0.8):]
X_train, X_test = X[:int(len(X)*0.8)], X[int(len(X)*0.8):]

print(f"Total de muestras para entrenamiento: {X_train.shape[0]}. Etiquetas binarias [0, 1].")


# ====================================================================
# 3. CONSTRUCCIÓN Y ENTRENAMIENTO DEL MODELO (Keras 4-4-1 con Sigmoid)
# ====================================================================

model = Sequential()
# Capa Oculta: 4 entradas -> 4 neuronas (usando ReLU)
model.add(Dense(4, activation='relu', input_shape=(4,)))
# Capa de Salida: 4 neuronas -> 1 neurona (usando SIGMOID)
model.add(Dense(1, activation='sigmoid')) # Salida entre 0 y 1

# loss='binary_crossentropy' es la mejor función de pérdida para salida Sigmoid (0/1).
model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

print("\nIniciando entrenamiento con Sigmoid...")
model.fit(
    X_train, 
    y_train, 
    epochs=EPOCAS, 
    batch_size=TAMANO_LOTE, 
    validation_data=(X_test, y_test),
    verbose=0
)

# ====================================================================
# 4. EXTRACCIÓN DE PESOS Y SESGOS
# ====================================================================

print("\nEntrenamiento finalizado.")
loss, accuracy = model.evaluate(X_test, y_test, verbose=0)
print(f"Precisión del Modelo en Datos de Prueba: {accuracy*100:.2f}%")

# Obtener los pesos (W1, B1) de la primera capa y (W2, B2) de la capa de salida
W1, B1 = model.layers[0].get_weights()
W2, B2 = model.layers[1].get_weights()

def format_array(arr):
    return ','.join([f'{val:.8f}' for val in arr.flatten()])

print("\n" + "="*70)
print("###  PARÁMETROS C++ para Arduino (4-4-1 Sigmoid) ###")
print("="*70)

# 1. Capa de Entrada a Oculta (W1)
print("\n// W1: Pesos de entrada (4) a oculta (4). Matriz [4][4]")
print("const float W1[4][4] = {")
for i in range(4):
    print(f"\t{{{format_array(W1[i])}}},")
print("};")
print("\n// B_oculta: Sesgos de la capa oculta (4 elementos)")
print(f"const float B_oculta[4] = {{{format_array(B1)}}};")

# 2. Capa Oculta a Salida (W2)
print("\n// W2: Pesos de oculta (4) a salida (1). Vector [4]")
print(f"const float W2[4] = {{{format_array(W2)}}};")
print("\n// B2: Sesgo de la capa de salida (1 elemento)")
print(f"const float B2 = {B2[0]:.8f};")

print("\n==========================================================")
print(" El código de Arduino  debe de usar la función sigmoid().")