/*
ITES - Instituto Tecnológico de Educación Superior
Tecnicatura Superior en Desarrollo de Software

Cátedra Microcontroladores II

Escrito Por:

Fecha:

--------------------------------------------------

Ejercicio Nro. 3

Primer proyecto en GitHub

prueba por Nacho Maldonado


prueba por Matias Draeger
--------------------------------------------------
Objetivo:
INTERRUPTOR CREPUSCULAR CON SENSOR DE PRESENCIA

Condiciones:
•  PIN 3 = Reflector (K1) (1 = activado)
•  PIN 8 = Detector de luz (SW1) (1 = oscuro)
•  PIN 9 = Detector de presencia (SW2) (1 = detección)
•  PIN 10 = Destello de sistema funcionando (Utilizar este PIN como
verificación visual de que el sistema está funcionando)

NIVEL 1:
Si está oscuro (SW1 == 1) y se detectó presencia (SW2 == 1), encender el
reflector (K1 = 1) durante un tiempo determinado (fijo) y luego de dicho
tiempo apagarlo.

NIVEL 2:
- Si se presionar el BOTON_COMANDO_MANUAL, debe encender el reflector si estaba apagado, y apagarlo si estaba encendido.
Se debe apagar de manera temporizada únicamente si se encendió de manera automática.
*/

#define PIN_SW1									8
#define PIN_DETECTA_OSCURIDAD					PIN_SW1
#define CONFIGURAR_DETECTA_OSCURIDAD			pinMode(PIN_DETECTA_OSCURIDAD, INPUT)
#define ESTA_OSCURO								(digitalRead(PIN_DETECTA_OSCURIDAD)==HIGH)

#define PIN_DETECTA_PRESENCIA					9
#define CONFIGURAR_DETECTA_PRESENCIA			pinMode(PIN_DETECTA_PRESENCIA, INPUT)
#define DETECTA_PRESENCIA						(digitalRead(PIN_DETECTA_PRESENCIA)==HIGH)

#define PIN_REFLECTOR							3
#define CONFIGURAR_REFLECTOR					pinMode(PIN_REFLECTOR, OUTPUT)
#define APAGAR_REFLECTOR						digitalWrite(PIN_REFLECTOR, LOW)
#define ENCENDER_REFLECTOR						digitalWrite(PIN_REFLECTOR, HIGH)

#define PIN_LED_TEST							10
#define CONFIGURAR_LED_TEST						pinMode(PIN_LED_TEST, OUTPUT)
#define APAGAR_LED_TEST							digitalWrite(PIN_LED_TEST, LOW)
#define ENCENDER_LED_TEST						digitalWrite(PIN_LED_TEST, HIGH)

#define MS_REFLECTOR_ENCENDIDO					2000

#define ON 										1
#define OFF 									0

unsigned char pulsos = 1;
unsigned long millis_pulso = 200, millis_off = 1000;

bool reflector=0;

void setup(void)
    {
    CONFIGURAR_DETECTA_OSCURIDAD;
    CONFIGURAR_DETECTA_PRESENCIA;
    CONFIGURAR_REFLECTOR;
    CONFIGURAR_LED_TEST;

    APAGAR_REFLECTOR;
    APAGAR_LED_TEST;
    }

void LedTest(void)
    {

    }

void CtrlAutomaticoReflector(void)
    {

    }

void loop(void)
    {
    LedTest();
    CtrlAutomaticoReflector();
    }
