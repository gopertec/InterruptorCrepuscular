# InterruptorCrepuscular marca ITES - modelo SR-2019
Controlador para reflector activada por fotocélula

Cuando el control detecta que está oscuro, y detecta movimiento, activa la salida para encender un reflector.
Mediante un pulsador, se podrá comandar de manera manual invirtiendo el estado que tenga la salida al momento de presionarse el pulsador.
Si se encendió manualmente, no se deberá apagar con el temporizador que se activa con el sensor de movimiento.

Se registra el tiempo acumulado de encendido (al momento de apagarse, se actuliza en la EEPROM)
Se registra el tiempo desde que el sistema se energizó.
Si se presionan dos veces el pulsador (con una espacio de tiempo inferior de un segundo), se activa la salida manualmente con el tiempo del temporizador, independientemente de la detección del sensor de movimiento. Por defecto, temporizar el apagado (15 seg).
El tiempo del temporizador se almacena en la memoria no volatil.

### NOTIFICACIONES por puerto serie
- Al instante de encender. Informar que se encendió
- Al instante de apagar. Informar que se apagó + la cantidad acumulada de encendidos + el tiempo acumulado de funcionamiento
- Cada 30 segundos, informar el tiempo que lleva energizado

### COMANDOS por puerto serie
- Ajuste del tiempo del temporizador
- Lectura  del tiempo del temporizador
- Ajuste del tiempo entre los que se informa el tiempo que lleva energizado
- Lectura del tiempo de informe de energizado
- Reseteo del registro de marcha acumulada
- Lectura del registro de marcha acumulada
- Reseteo del registro de encendidos acumulados
- Lectura del registro de encendidos acumulados

### Recomendaciones:
- El código agregado se debe probar antes de subirlo al proyecto.
- Los colaboradores no deberán agregar variables globales.
- El código debe ser tal que no trabe el funcionamiento, es decir, debe permitir la ejecución del resto de las funciones.

