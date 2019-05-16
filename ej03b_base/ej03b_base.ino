#include <EEPROM.h>

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

- Guardar en la memoria no volatil, los valores registrados (cantidad de encendidos + tiempo acumulado de funcionamiento)
- Utilizar INPUT_PULLUP para las entradas (así evitamos la necesidad de resistencias pull up externas)
*/
#include <EEPROM.h>



#define PIN_BOTON                               7
#define CONFIGURAR_BOTON                        pinMode(PIN_BOTON, INPUT_PULLUP)
#define BOTON_PRESIONADO                        (digitalRead(PIN_BOTON)==LOW)

#define PIN_DETECTA_OSCURIDAD                   8
#define CONFIGURAR_DETECTA_OSCURIDAD            pinMode(PIN_DETECTA_OSCURIDAD, INPUT_PULLUP)
#define ESTA_OSCURO                             (digitalRead(PIN_DETECTA_OSCURIDAD)==LOW)

#define PIN_DETECTA_PRESENCIA                   9
#define CONFIGURAR_DETECTA_PRESENCIA            pinMode(PIN_DETECTA_PRESENCIA, INPUT_PULLUP)
#define DETECTA_PRESENCIA                       (digitalRead(PIN_DETECTA_PRESENCIA)==LOW)

#define PIN_REFLECTOR                           3
#define CONFIGURAR_REFLECTOR                    pinMode(PIN_REFLECTOR, OUTPUT)
#define APAGAR_REFLECTOR                        digitalWrite(PIN_REFLECTOR, LOW)
#define ENCENDER_REFLECTOR                      digitalWrite(PIN_REFLECTOR, HIGH)

#define PIN_LED_TEST                            13       //Build in Led
#define CONFIGURAR_LED_TEST                     pinMode(PIN_LED_TEST, OUTPUT)
#define APAGAR_LED_TEST                         digitalWrite(PIN_LED_TEST, LOW)
#define ENCENDER_LED_TEST                       digitalWrite(PIN_LED_TEST, HIGH)

#define ON                                      1
#define OFF                                     0

#define SALVAR_ENCENDIDOS 10
#define SALVAR_TIEMPO_MARCHA 11

unsigned char pulsos = 1;
unsigned long millis_pulso = 200, millis_off = 1000;
unsigned long tpo_marcha =0;

bool reflector = 0;  //el reflector se enciende/apaga según esta variable
bool encendido_manual=0;

unsigned int encendidos=0;
bool tx_temporizador=true;
bool tx_encendidos=true;
bool tx_tpo_marcha=false;
int temporizador=2000;
int tpo_energizado=30000;
bool tx_tpo_energizado=true;
bool tx_tiempo=false;
unsigned long tiempo=0;

byte eeEncendidos=0;
int dirEncendidos=0;
byte eeMarcha;
int dirTpoMarcha=1;

void setup(void)
	{
	CONFIGURAR_DETECTA_OSCURIDAD;
	CONFIGURAR_DETECTA_PRESENCIA;
	CONFIGURAR_REFLECTOR;
	CONFIGURAR_LED_TEST;
  CONFIGURAR_BOTON;
	APAGAR_REFLECTOR;
	APAGAR_LED_TEST;
	Serial.begin(9600);
  EEPROM.get(dirEncendidos, eeEncendidos);
  encendidos=eeEncendidos;
  EEPROM.get(dirTpoMarcha, eeMarcha);
  tpo_marcha=eeMarcha;
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
	static bool boton_presionado_ant = LOW;
  static unsigned long int momentoPulsado=0;
  unsigned long int tiempoMuerto = 50;
  if (BOTON_PRESIONADO == boton_presionado_ant)  return (false);
  if(millis()-momentoPulsado>tiempoMuerto){
  boton_presionado_ant = BOTON_PRESIONADO;
  momentoPulsado=millis();
  } else return false;
  return (BOTON_PRESIONADO);
	}
 
void InvertirEstadoReflector(void)  //Nacho
	{
	reflector = !reflector;
	encendido_manual = reflector;
	}

void CtrlAutomaticoReflector(void) //Ezequiel
	{
	/*
	ALEJO--> Acomodar.
	Ahora, si está encendido el reflector y deja de detectar oscuridad,
	el reflector queda encendido.
	*/
	static bool detecta_presencia_ant=0;
	static unsigned long millis_inicial = 0;
	if (!ESTA_OSCURO)return; //...está oscuro
	if (DETECTA_PRESENCIA != detecta_presencia_ant) ///////// mientras haya movimiento la luz va a estar prendida, cuando deja de detectar presencia empieza el conteo
		{
		detecta_presencia_ant=DETECTA_PRESENCIA;
		  if(!DETECTA_PRESENCIA) return;               // Si se quiere que en el momento que detecte presencia se active, cambiar while por if
		  reflector = 1;
		  millis_inicial = millis();
		}
	if(encendido_manual) return;
	if (!reflector) return;
	if (millis() - millis_inicial > temporizador)
		reflector = 0;
	}
void Guardar2(int dato){ //RANZ
  if(dato==SALVAR_ENCENDIDOS) EEPROM.write(0,encendidos);
  else if(dato==SALVAR_TIEMPO_MARCHA) EEPROM.write(1,tpo_marcha);
  else return;
  } 


void ContadorDeEncendido(void)  //Matias
	{
	static bool estado_anterior=LOW;
	if(reflector==estado_anterior) return;
	estado_anterior=reflector;
	if(reflector)
		{
		encendidos++;
    Guardar(dirEncendidos, encendidos);
   /*eeEncendidos=encendidos;
   EEPROM.update(dirEncendidos, eeEncendidos);*/
		}
	}

void RegistroAcumuladoDeMarcha(void) //ahi modifique la funcion, ahora respeta el tiempo que esta prendido //Ezequiel
	{
	static unsigned long tiempo_inicial=0;
	if(reflector==0){
	  Guardar(dirTpoMarcha, tpo_marcha);
	  return;
	}
	if (millis() - tiempo_inicial > 1000)
		{
		tpo_marcha ++;
		tiempo_inicial=millis();
		}
	}

void TiempoEnergizado(void)
	{
	static unsigned long tiempo_inicial=0;
	tiempo=(millis()/1000);
	if(millis() - tiempo_inicial<tpo_energizado) return;
	tiempo_inicial=millis();
	tx_tiempo=true;
	
   /* if(tiempo=30){
	  tx_tpo_energizado=true;
	}*/
	}


void ActualizaSalidas(void)
	{
	if(reflector)  ENCENDER_REFLECTOR;
	else          APAGAR_REFLECTOR;
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
	if(tx_tpo_marcha)
		{
		tx_tpo_marcha=0;
		Serial.print("El valor de la variable tpo_marcha es: ");
		Serial.println(tpo_marcha);
		}
	if(tx_tpo_energizado)
		{
		 tx_tpo_energizado=0;
		 Serial.print("El valor de la variable tpo_energizado es: ");
		 Serial.println(tpo_energizado);
		}
	if(tx_tiempo)
		{
		  tx_tiempo=0;
		  Serial.print("El programa lleva energizado: ");
		  Serial.print(tiempo);
		  Serial.println(" segundos");
		}
	}

void RecibirPorSerie(void)
	{
	if(!Serial.available())return;
	char dato=Serial.read();
	switch(dato)
		{
		case 'T':
			tx_temporizador=true;
			break;
		case 't':
			{
			String cadena=Serial.readString();
			if(cadena.length())
				temporizador=cadena.toInt();
			tx_temporizador=true;
			break;
			}
		case 'E':
			tx_encendidos=1;
			break;
		case 'e':
			{
			String str=Serial.readString();
			encendidos=str.toInt();
			tx_encendidos=1;
			break;
			}
		case 'M':
			tx_tpo_marcha=1;
			break;
		case 'm':
			tpo_marcha=0;
			tx_tpo_marcha=1;
			break;
		case 'p':
			tx_tpo_energizado=true;
			break;
		case 'P':
			{
			String cadena=Serial.readString();
			if(cadena.length())
				tpo_energizado=cadena.toInt();
			tx_tpo_energizado=true;
			break;
			}
		default:
			break;
		}
	}

void Guardar(int direccion, byte dato)
  {
    EEPROM.update(direccion, dato);
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
	TiempoEnergizado();
	if (SePresionoBoton())
		InvertirEstadoReflector();
	}
