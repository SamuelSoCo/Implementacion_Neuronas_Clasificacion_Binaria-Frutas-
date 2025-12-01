# PROGRAMA PARA GRAFICAR UNA RED NEURONAL CON PESOS DEFINIDOS USANDO NETWORKX

# 1. IMPORTACIÓN DE LIBRERÍAS
import pandas as pd
import numpy as np 
import networkx as nx 
import matplotlib.pyplot as plt

# 2. DEFINICIÓN DE LOS PARÁMETROS DE LA RED (TUS DATOS)
# W1: Pesos de entrada (E) a oculta (H). Matriz [4][4]
W1 = np.array([
    [-0.82595450, 0.30097684, -0.56078386, -0.73785502],
    [-0.20391315, 0.50615323, 0.29051006, 0.62281859],
    [-0.12707019, -0.93947381, 0.00817984, 0.05740469],
    [ 0.50265807, -0.01326199, 0.25435102, 0.27062473],
])

# W2: Pesos de oculta (H) a salida (S). Vector [4]
W2 = np.array([-1.07967234, -0.29689160, -0.76637059, 0.10349683])

# Sesgos (Bias) - Se usarán para las etiquetas de texto
B_oculta = np.array([0.00000000, -2.06350255, -0.70413309, 1.80208373])
B2 = 2.45807385

# 3. CONVERSIÓN DE MATRICES DE PESOS A DATAFRAME DE ARISTAS
origenes = []
destinos = []
pesos = []

# A. Conexiones W1 (Entrada -> Oculta)
for i in range(4): # 4 neuronas de entrada (E1 a E4)
    for j in range(4): # 4 neuronas ocultas (H1 a H4)
        origenes.append(f'E{i+1}')
        destinos.append(f'H{j+1}')
        pesos.append(W1[i][j]) 

# B. Conexiones W2 (Oculta -> Salida)
for i in range(4): # 4 neuronas ocultas (H1 a H4)
    origenes.append(f'H{i+1}')
    destinos.append('S1')
    pesos.append(W2[i])

# Crear el DataFrame final
df_pesos = pd.DataFrame({'Origen': origenes, 'Destino': destinos, 'Peso': pesos})

# 4. CREACIÓN DEL GRAFO
NEURONAL_GRAPH = nx.from_pandas_edgelist(
    df_pesos,
    source='Origen',
    target='Destino',
    edge_attr='Peso',
    create_using=nx.DiGraph # Usamos DiGraph para la dirección del flujo
)

# 5. ASIGNACIÓN DEL ATRIBUTO DE CAPA (SOLUCIÓN AL ERROR)
# La función multipartite_layout requiere que la información de la capa sea un atributo de nodo.
particion = {'E': 0, 'H': 1, 'S': 2} # Mapeo de prefijo a número de capa
capa_atributos = {}

for nodo in NEURONAL_GRAPH.nodes():
    # Asigna el número de capa (0, 1, o 2) al atributo 'layer' del nodo
    capa_atributos[nodo] = particion.get(nodo[0], 0) 

nx.set_node_attributes(NEURONAL_GRAPH, capa_atributos, "layer")

# 6. VISUALIZACIÓN DEL GRAFO
plt.figure(figsize=(14, 9))

# A. Usar multipartite_layout con el atributo 'layer'
pos = nx.multipartite_layout(NEURONAL_GRAPH, subset_key="layer") 

# B. Definir colores de nodos por capa:
colores = {'E': '#4CAF50', 'H': '#2196F3', 'S': '#FFC107'} # Verde, Azul, Amarillo
colores_nodos = [colores[nodo[0]] for nodo in NEURONAL_GRAPH.nodes()]

# C. Dibujar el grafo
nx.draw_networkx_nodes(NEURONAL_GRAPH, pos, node_color=colores_nodos, node_size=1600, alpha=0.9)
nx.draw_networkx_edges(NEURONAL_GRAPH, pos, edge_color='gray', width=1.0, arrowsize=20)
nx.draw_networkx_labels(NEURONAL_GRAPH, pos, font_size=11, font_weight='bold')

# D. Etiquetas de los Pesos (en las aristas)
etiquetas_formateadas = {
    (u, v): f"{p:.2f}" for u, v, p in NEURONAL_GRAPH.edges(data='Peso')
}
nx.draw_networkx_edge_labels(NEURONAL_GRAPH, pos, edge_labels=etiquetas_formateadas, font_size=8, label_pos=0.3)

# E. Añadir etiquetas para el BIAS (etiquetas de texto)
bias_labels = {}
# Bias de la capa oculta (H1 a H4)
for i, bias_val in enumerate(B_oculta):
    node_name = f'H{i+1}'
    bias_labels[node_name] = f"Bias: {bias_val:.2f}"

# Bias de la capa de salida (S1)
bias_labels['S1'] = f"Bias: {B2:.2f}"

# Dibujar las etiquetas de Bias
for node, label in bias_labels.items():
    if node in pos:
        x, y = pos[node]
        plt.text(x + 0.15, y, label, 
                fontsize=9, 
                color='black', 
                bbox=dict(facecolor='white', alpha=0.7, edgecolor='none', boxstyle='round,pad=0.3'))


plt.title("Estructura y Pesos de la Red Neuronal MLP 4x4x1")
plt.axis('off') # Ocultar los ejes
plt.show()