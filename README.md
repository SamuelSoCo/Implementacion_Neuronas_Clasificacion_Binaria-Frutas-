feat: Implementación de Red Neuronal para Clasificación de Frutas (Manzana/Naranja)

Este commit documenta y presenta el manual de implementación de un sistema de clasificación binaria (manzana/naranja) utilizando una Red Neuronal Artificial (RNA) simple. La implementación se centra en la adquisición de datos de color mediante hardware Arduino y el entrenamiento/optimización de la red en Python.

**Funcionalidades Clave Añadidas:**

* **Adquisición de Datos Crudos:** Uso de Arduino Uno [cite: 12] y el sensor de color **TCS3200** [cite: 13, 18] para obtener lecturas RGB/Clear (Rojo, Verde, Azul, Luminosidad) de las frutas[cite: 13, 42].
    * Se incluye el código `RAW_DATA` para la calibración y obtención de valores mínimos y máximos[cite: 30, 44].
    * El sensor opera en el modo de **escala de frecuencia del 20%** para estabilidad[cite: 27, 39].
* **Preparación y Normalización de Datos:** Se implementa una función de **normalización min-max invertida** en Arduino (`Normalisacion_Raw_Data`)[cite: 46, 70, 76].
    * La normalización invierte el periodo crudo (tiempo de pulso) a un valor de intensidad en el rango **0-255** (periodo corto $\rightarrow$ alta intensidad)[cite: 50, 68, 80].
    * Los datos normalizados se envían en formato **CSV** con etiquetas (`manzana`/'1', `naranja`/'0') para el entrenamiento, utilizando **Data Streamer** de Excel[cite: 46, 47, 62].
* **Estructura de la Red Neuronal (Documentación):** La red neuronal a entrenar en Python es de tipo **4-4-1** (4 entradas (RGB + Claro) $\rightarrow$ 4 neuronas ocultas $\rightarrow$ 1 salida)[cite: 92].
* **Hardware y Entorno:** Se detallan los componentes (`Arduino Uno`, `TCS3200`) y las herramientas de software (`Python`, librerías `NumPy` y `scikit-learn`) para el entrenamiento y obtención de Pesos (W) y Bias (B) óptimos[cite: 12, 13, 14, 9].
