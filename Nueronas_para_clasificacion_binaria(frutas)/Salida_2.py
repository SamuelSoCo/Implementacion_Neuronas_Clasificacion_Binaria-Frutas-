import serial
import time

PUERTO_SERIAL = "COM3"   # <-- CAMBIA AL TUYO
BAUDRATE = 9600

try:
    ser = serial.Serial(PUERTO_SERIAL, BAUDRATE, timeout=1)
    time.sleep(2)
    print("Python funcionando...\n")

    while True:
        linea = ser.readline().decode().strip()

        if linea == "":
            continue

        # Buscar SOLO la parte final: Clasificacion: xxxx
        if "Clasificacion:" in linea:
            partes = linea.split("Clasificacion:")
            fruta = partes[1].strip().lower()

            # Invertir la clasificación
            if fruta == "manzana":
                print("Python imprime: naranja\n")
            elif fruta == "naranja":
                print("Python imprime: manzana\n")
            else:
                print("Python: fruta no reconocida\n")

except serial.SerialException as e:
    print(f"\nERROR: No se pudo abrir o leer el puerto serial {PUERTO_SERIAL}.")
    print("Asegúrate de que el Arduino esté conectado y que el puerto sea correcto.")
    print(f"Detalle: {e}")

except KeyboardInterrupt:
    print("\nProceso detenido por el usuario.")

finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Conexión serial cerrada.")
