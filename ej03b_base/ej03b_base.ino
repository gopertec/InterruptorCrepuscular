/*
ITES - Instituto Tecnológico de Educación Superior
Tecnicatura Superior en Desarrollo de Software

Cátedra Microcontroladores II

Escrito Por: 

Fecha:

--------------------------------------------------

Ejercicio Nro. 3

Primer proyecto en GitHub

prueba por Ezequiel Errecalde
prueba por Enrique Sabetta
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

#define PIN_BOTON                               7
#define CONFIGURAR_BOTON                        pinMode(PIN_BOTON, INPUT)
#define BOTON_PRESIONADO                        (digitalRead(PIN_BOTON)==HIGH)

#define PIN_DETECTA_OSCURIDAD                   8
#define CONFIGURAR_DETECTA_OSCURIDAD            pinMode(PIN_DETECTA_OSCURIDAD, INPUT)
#define ESTA_OSCURO                             (digitalRead(PIN_DETECTA_OSCURIDAD)==HIGH)

#define PIN_DETECTA_PRESENCIA                   9
#define CONFIGURAR_DETECTA_PRESENCIA            pinMode(PIN_DETECTA_PRESENCIA, INPUT)
#define DETECTA_PRESENCIA                       (digitalRead(PIN_DETECTA_PRESENCIA)==HIGH)

#define PIN_REFLECTOR                           3
#define CONFIGURAR_REFLECTOR                    pinMode(PIN_REFLECTOR, OUTPUT)
#define APAGAR_REFLECTOR                        digitalWrite(PIN_REFLECTOR, LOW)
#define ENCENDER_REFLECTOR                      digitalWrite(PIN_REFLECTOR, HIGH)

#define PIN_LED_TEST                            10
#define CONFIGURAR_LED_TEST                     pinMode(PIN_LED_TEST, OUTPUT)
#define APAGAR_LED_TEST                         digitalWrite(PIN_LED_TEST, LOW)
#define ENCENDER_LED_TEST                       digitalWrite(PIN_LED_TEST, HIGH)

#define ON                                      1
#define OFF                                     0

unsigned char pulsos = 1;
unsigned long millis_pulso = 200, millis_off = 1000;
unsigned long tpo_marcha =0;

bool reflector = 0;	//el reflector se enciende/apaga según esta variable
bool encendido_manual=0;

unsigned int encendidos=0;
bool tx_temporizador=true;
bool tx_encendidos=true;
int temporizador=2000;

void setup(void)
{
	CONFIGURAR_DETECTA_OSCURIDAD;
	CONFIGURAR_DETECTA_PRESENCIA;
	CONFIGURAR_REFLECTOR;
	CONFIGURAR_LED_TEST;
	
	APAGAR_REFLECTOR;
	APAGAR_LED_TEST;
	Serial.begin(9600);
}

void LedTest(void)  //Matias y Enrique
  {
	static unsigned long millis_ini = 0;
	const unsigned long intervalo = 500;
	static int ledEstadoTest = LOW;
	if (millis() - millis_ini < intervalo) return;
	millis_ini = millis();
	ledEstadoTest = !ledEstadoTest;
	if (ledEstadoTest)  ENCENDER_LED_TEST;
		else        APAGAR_LED_TEST;		
  }

bool SePresionoBoton(void)
  {
  static bool estado_anterior = BOTON_PRESIONADO;
  if(BOTON_PRESIONADO==estado_anterior) return(false);
  estado_anterior = BOTON_PRESIONADO;
  return BOTON_PRESIONADO;
  }

void InvertirEstadoReflector(void)  //Nacho
  {
  reflector = !reflector;
  encendido_manual = reflector;
  }
  
void CtrlAutomaticoReflector(void) //Ezequiel
    {
    static bool detecta_presencia_ant=0;
    static unsigned long millis_inicial = 0;

    if (!ESTA_OSCURO)return;
    //...está oscuro

    //ALEJO: Esto está EXCELENTE para trabar el programa!!! Así nadie puede hacer más nada...
    ///...mientras detecta presencia!!!!
    //detectar el INSTANTE... es decir, el momento en que cambia de estado la señal DETECTA_PRESENCIA
    if (DETECTA_PRESENCIA != detecta_presencia_ant) ///////// mientras haya movimiento la luz va a estar prendida, cuando deja de detectar presencia empieza el conteo
        {
        detecta_presencia_ant=DETECTA_PRESENCIA;
        if(!DETECTA_PRESENCIA) return;
        // Si se quiere que en el momento que detecte presencia se active, cambiar while por if
        millis_inicial = millis();

        reflector = 1;
        }

    if(encendido_manual) return;

    if (!reflector) return;
    if (millis() - millis_inicial > temporizador)
        reflector = 0;
    }

/*
 Daniel Ranz
 Hola a todos! Permiso para aportar, moví las funciones  InvertirEstadoReflector() y SePresionoBoton() para arriba por que cuando se usan 
 funciones unas dentro de otras las que estan adentro de otra tienen que estar definidas anteriormente. A SePresionoBoton() la deje igual a la q vimos en clase. 
 El codigo esta chequeado y probado, funciona. Dejo el aporte como comentario para esperar la aprobacion de todos y por supuesto del profe.
 para probarlo hay que comentar la funcion CtrlAutomaticoReflector() q hizo el Eze y en el loop() borrar el if y reemplazar CtrlAutomaticoReflector() 
 por la controlAutoManual() que esta en este comentario abajo y descomentar todo lo que sigue:
 
 void modoAutomatico(void)
  {
  static unsigned long millis_ini=0;
  if(ESTA_OSCURO&DETECTA_PRESENCIA&!reflector)
    { 
    reflector=1;
    millis_ini=millis();
    }
  if(reflector)
    {
    if(millis()-millis_ini<temporizador)return;
    reflector=0;  
    } 
  }
 
void modoManual(void)
  { 
    InvertirEstadoReflector();
  }

void controlAutoManual(void)
  {
  if(SePresionoBoton()) 
    {
      modoManual();
      return;
    }
    else
    {   
      if(!encendido_manual)modoAutomatico();
      } 
    } 

*/
void ContadorDeEncendido(void)	//Matias
  {
	static bool estado_anterior=LOW;
	if(reflector==estado_anterior) return;
		estado_anterior=reflector;
	if(reflector)
	  {
		encendidos++;
		//   Serial.print("Encendidos: ");	  Serial.println(encendidos);
	  } 
    }

void RegistroAcumuladoDeMarcha(void)
  {
	static unsigned long tiempo_inicial=0;
	if(reflector==0) return;
	if (millis() - tiempo_inicial >= 1000)
	  {
    tpo_marcha ++;
    tiempo_inicial=millis();
	  }
  }



void ActualizaSalidas(void)
    {
    if(reflector)  ENCENDER_REFLECTOR;
    else      		APAGAR_REFLECTOR;
    }

void TransmisionPorSerie(void)
  {
  if(tx_temporizador)
    {
    tx_temporizador=0;
    Serial.print("El valor de la variable temporizador es: ");
    Serial.println(temporizador);
    }
  if(tx_encendidos)
    {
    tx_encendidos=0;
    Serial.print("El valor de la variable encendidos es: ");
    Serial.println(encendidos);
    }	
  }

void RecibirPorSerie(void)
  {
  if(!Serial.available())return;
  char dato=Serial.read();
  switch(dato)
    {
    case 'T': 
      tx_temporizador=1; 
      break;
    case 't': 
      {
      String cadena=Serial.readString();
      temporizador=cadena.toInt();
      break;
      }
      case 'E': 
      tx_encendidos=1; 
      break;
      case 'e': 
      {
      String str=Serial.readString();
      encendidos=str.toInt();
      break;
      }	      
      default: break;
    }
  }


void loop(void)
    {
    LedTest();
    ActualizaSalidas();
    RecibirPorSerie();
    TransmisionPorSerie();
    CtrlAutomaticoReflector();  //
    ContadorDeEncendido();  //Contar la cantidad de veces que se enciende
    RegistroAcumuladoDeMarcha();
    if (SePresionoBoton())
        InvertirEstadoReflector();
    }
