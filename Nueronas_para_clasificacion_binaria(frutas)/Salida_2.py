import serial
import time

puerto = "COM3"   # <-- CAMBIA AL TUYO
baudrate = 9600

ser = serial.Serial(puerto, baudrate, timeout=1)
time.sleep(2)

print("Python funcionando...\n")

while True:
    linea = ser.readline().decode().strip()

    if linea == "":
        continue

    # Buscar SOLO la parte final: Clasificacion: xxxx
    if "Clasificacion:" in linea:
        # Extraer la palabra después de "Clasificacion:"
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
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Conexión serial cerrada.")

    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
