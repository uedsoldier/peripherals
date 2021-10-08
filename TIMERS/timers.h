/**
  * @file timers.h
  * @brief  Librería para utilización de módulos temporizadores para microcontrolladores de 8 bits
  * @author Ing. José Roberto Parra Trewartha
*/
#ifndef TIMERS_H
#define TIMERS_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../pconfig.h"
#include "../../utils/utils.h"

/*
	Parámetros de configuración Timer0
*/
//Valores de preescala del Timer0

#define TIMER0_DIV_1   			0b1000 	//Preescala 1:1 (Sin preescala)
#define TIMER0_DIV_2 			0b0000  //Preescala 1:2
#define TIMER0_DIV_4 			0b0001  //Preescala 1:4
#define TIMER0_DIV_8 			0b0010  //Preescala 1:8
#define TIMER0_DIV_16 			0b0011  //Preescala 1:16
#define TIMER0_DIV_32 			0b0100  //Preescala 1:32
#define TIMER0_DIV_64 			0b0101  //Preescala 1:64
#define TIMER0_DIV_128      	0b0110  //Preescala 1:128
#define TIMER0_DIV_256 			0b0111  //Preescala 1:256

//Configuración de timer0 como registro de 8 o de 16 bits
#define TIMER0_8BIT				0b01000000 	
#define TIMER0_16BIT			0b00000000 	

//Selección de fuente interna (ciclos máquina) o externa (flancos de subida/bajada)
#define TIMER0_INTERNAL  		0b00000000	//Fuente de reloj es ciclo máquina Tcy = 4*TOSC
#if defined (TMR_V7_4)
#define TIMER0_INTOSC			0b00000000	//Fuente de incremento del timer0: Oscilador interno
#else
#define TIMER0_EXTERNAL		    0b00100000  //Fuente de incremento del timer0: Flanco externo en el pin T0CKI
#endif 
#define TIMER0_EXT_L_TO_H 		0b00000000  //Flanco de subida en el pin T0CKI
#define TIMER0_EXT_H_TO_L		0b00010000  //Flanco de bajada en el pin T0CKI

/*
	Prototipos de funciones
*/
void timer0_init(uint8_t param_config);
void set_timer0(uint16_t valor_timer);
uint16_t get_timer0(void);

/*
	Parámetros de configuración Timer1
*/

#if defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)

//Valores de preescala (1,2,4 u 8)
#define TIMER1_DIV_1			0b00000000  //Preescala 1:1 (Sin preescala)
#define TIMER1_DIV_2			0b00010000  //Preescala 1:2
#define TIMER1_DIV_4			0b00100000  //Preescala 1:4
#define TIMER1_DIV_8			0b00110000  //Preescala 1:8

//Configuración de oscilador externo/secundario o pulsos externos para utilización con timer
#if defined (TMR_V7_4)
#define TIMER1_INTOSC 	0b11000000  // Fuente de reloj es periodo de oscilador interno
#endif

#define TIMER1_EXTERNAL				0b10000000  //Fuente de reloj es un cristal externo o pulsos digitales con flanco de subida
#define TIMER1_INTERNAL_SYS_CLK 	0b01000000  //Fuente de reloj es reloj de sistema: TOSC (No utilizar si Timer1 se asocia a ECCP)
#define TIMER1_INTERNAL_INST_CLK 	0b00000000  //Fuente de reloj es reloj de instrucciones o ciclo máquina: Tcy = 4*TOSC
#define TIMER1_OSCEN_ON 	0b00001000 	//Circuito para oscilador activado
#define TIMER1_OSCEN_OFF 	0b00000000 	//Circuito para oscilador desactivado

//Configuración de funcionamiento sincronizado
#define TIMER1_SYNC				0b00000000		//Funcionamiento de reloj externo sincronizado
#define TIMER1_ASYNC			0b00000100      //Funcionamiento de reloj externo sin sincronizar

//Modo de lectura/escritura de registro timer1 como registro de 8 o de 16 bits
#define TIMER1_8BIT_RW          0b00000000  //Habilita lectura/escritura del registro TMR1H:L en dos operaciones de 8 bits
#define TIMER1_16BIT_RW         0b00000010  //Habilita lectura/escritura del registro TMR1H:L en una operación de 16 bits

#if defined (TMR_V7_4) || defined (TMR_V7_5)
#define TIMER_GATE_SRC_Tx       0b00000001  //Timer(x+1) match PR(x+1)output
#define TIMER_GATE_SRC_CMP1     0b00000010  //Salida de comparador 1
#define TIMER_GATE_SRC_CMP2     0b00000011  //Salida de comparador 2

#else 
#define TIMER_GATE_SRC_T0       0b00000001  //Timer0 overflow output
#define TIMER_GATE_SRC_T2       0b00000010  //Timer2 match PR2  output
#endif

void timer1_init ( uint8_t param_config,  uint8_t gate_control);
uint16_t get_timer1 (void);
void set_timer1 ( uint16_t valor_timer);

//Todas las demás versiones
#elif defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) || defined (TMR_V4) || defined (TMR_V5)
void timer1_init ( uint8_t param_config);
uint16_t get_timer1 (void);
void set_timer1 ( uint16_t valor_timer);

//Valores de preescala (1,2,4 u 8)
#define TIMER1_DIV_1			0b00000000  //Preescala 1:1 (Sin preescala)
#define TIMER1_DIV_2			0b00010000  //Preescala 1:2
#define TIMER1_DIV_4			0b00100000  //Preescala 1:4
#define TIMER1_DIV_8			0b00110000  //Preescala 1:8

//Configuración de oscilador externo/secundario o pulsos externos para utilización con timer
#define TIMER1_EXTERNAL		0b00000010  //Fuente de reloj es un cristal externo o pulsos digitales con flanco de subida
#define TIMER1_INTERNAL		0b00000000  //Fuente de reloj es reloj de instrucciones o ciclo máquina (Tcy = 4*TOSC)
#define TIMER1_OSCEN_ON 	0b00001000 	//Circuito para oscilador activado
#define TIMER1_OSCEN_OFF 	0b00000000 	//Circuito para oscilador desactivado

//Configuración de funcionamiento sincronizado
#define TIMER1_SYNC				0b00000000		//Funcionamiento de reloj externo sincronizado
#define TIMER1_ASYNC			0b00000100      //Funcionamiento de reloj externo sin sincronizar

//Modo de lectura/escritura de registro timer1 como registro de 8 o de 16 bits
#define TIMER1_8BIT_RW          0b00000000  //Habilita lectura/escritura del registro TMR1H:L en dos operaciones de 8 bits
#define TIMER1_16BIT_RW         0b10000000  //Habilita lectura/escritura del registro TMR1H:L en una operación de 16 bits



#endif

/*
	Parámetros de configuración Timer2
*/

#if defined (TMR_V2) || defined (TMR_V3) || defined (TMR_V4) ||\
    defined (TMR_V5) || defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1) ||\
	defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)


//Valores de preescala 1,4,16 o 64. Este último valor solamente en algunos microcontroladores)
#define TIMER2_DIV_1				0b00000000   	//Preescala 1:1 (Sin preescala)
#define TIMER2_DIV_4				0b00000001   	//Preescala 1:4
#define TIMER2_DIV_16				0b00000010 		//Preescala 1:16
#define TIMER2_DIV_64				0b00000011 		//Preescala 1:64 (Ssi no está presente en el microcontrolador, se toma 1:16)

//Valores de postescala (del 1 al 16)
#define TIMER2_POST1			0b00000000	//Postescala 1:1
#define TIMER2_POST2			0b00001000  //Postescala 1:2
#define TIMER2_POST3			0b00010000  //Postescala 1:3
#define TIMER2_POST4			0b00011000  //Postescala 1:4
#define TIMER2_POST5			0b00100000  //Postescala 1:5
#define TIMER2_POST6			0b00101000  //Postescala 1:6
#define TIMER2_POST7			0b00110000  //Postescala 1:7
#define TIMER2_POST8			0b00111000  //Postescala 1:8
#define TIMER2_POST9			0b01000000  //Postescala 1:9
#define TIMER2_POST10			0b01001000  //Postescala 1:10
#define TIMER2_POST11			0b01010000  //Postescala 1:11
#define TIMER2_POST12			0b01011000  //Postescala 1:12
#define TIMER2_POST13			0b01100000  //Postescala 1:13
#define TIMER2_POST14			0b01101000  //Postescala 1:14
#define TIMER2_POST15			0b01110000  //Postescala 1:15
#define TIMER2_POST16			0b01111000  //Postescala 1:16


/*
	Prototipos de funciones
*/
void timer2_init(uint8_t param_config, uint8_t period);
uint8_t get_timer2(void);
void set_timer2(uint8_t valor_timer);

#endif

/*
	Parámetros de configuración Timer3
*/
#if defined(TMR_V2)  || defined(TMR_V4) //En estas versiones no se utiliza oscilador en Timer3

//Valores de preescala (1,2,4 u 8)
#define TIMER3_DIV_1			0b00000000  //Preescala 1:1 (Sin preescala)
#define TIMER3_DIV_2			0b00010000  //Preescala 1:2
#define TIMER3_DIV_4			0b00100000  //Preescala 1:4
#define TIMER3_DIV_8			0b00110000  //Preescala 1:8

//Configuración de fuente de reloj interna o externa para utilización con timer
#define TIMER3_EXTERNAL		0b00000010  //Fuente de reloj son pulsos digitales con flanco de subida
#define TIMER3_INTERNAL		0b00000000  //Fuente de reloj es reloj de instrucciones o ciclo máquina (Tcy = 4*TOSC)

//Configuración de funcionamiento sincronizado
#define TIMER3_SYNC				0b00000000		//Funcionamiento de reloj externo sincronizado
#define TIMER3_ASYNC			0b00000100      //Funcionamiento de reloj externo sin sincronizar

//Modo de lectura/escritura de registro timer1 como registro de 8 o de 16 bits
#define TIMER3_8BIT_RW          0b00000000  //Habilita lectura/escritura del registro TMR1H:L en dos operaciones de 8 bits
#define TIMER3_16BIT_RW         0b10000000  //Habilita lectura/escritura del registro TMR1H:L en una operación de 16 bits

/*
	Prototipos de funciones
*/

void timer3_init(uint8_t param_config);
uint16_t get_timer3(void);
void set_timer3(uint16_t valor_timer);


#elif defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1) ||\
defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)

//Valores de preescala (1,2,4 u 8)
#define TIMER3_DIV_1			0b00000000  //Preescala 1:1 (Sin preescala)
#define TIMER3_DIV_2			0b00010000  //Preescala 1:2
#define TIMER3_DIV_4			0b00100000  //Preescala 1:4
#define TIMER3_DIV_8			0b00110000  //Preescala 1:8

//Configuración de oscilador externo/secundario o pulsos externos para utilización con timer
#define TIMER3_EXTERNAL				0b10000000  //Fuente de reloj es un cristal externo o pulsos digitales con flanco de subida
#define TIMER3_INTERNAL_SYS_CLK 	0b01000000  //Fuente de reloj es reloj de sistema: TOSC (No utilizar si Timer3 se asocia a ECCP)
#define TIMER3_INTERNAL_INST_CLK 	0b00000000  //Fuente de reloj es reloj de instrucciones o ciclo máquina: Tcy = 4*TOSC
#define TIMER3_OSCEN_ON 	0b00001000 	//Circuito para oscilador secundario activado
#define TIMER3_OSCEN_OFF 	0b00000000 	//Circuito para oscilador secundario desactivado

//Configuración de funcionamiento sincronizado
#define TIMER3_SYNC				0b00000000		//Funcionamiento de reloj externo sincronizado
#define TIMER3_ASYNC			0b00000100      //Funcionamiento de reloj externo sin sincronizar

//Modo de lectura/escritura de registro timer1 como registro de 8 o de 16 bits
#define TIMER3_8BIT_RW          0b00000000  //Habilita lectura/escritura del registro TMR3H:L en dos operaciones de 8 bits
#define TIMER3_16BIT_RW         0b00000010  //Habilita lectura/escritura del registro TMR3H:L en una operación de 16 bits

/*
	Prototipos de funciones
*/

void timer3_init(uint8_t param_config, uint8_t gate_control);
uint16_t get_timer3(void);
void set_timer3(uint16_t valor_timer);

#endif

/*
	Parámetros de configuración Timer4
*/

#if defined(TMR_V4) || defined(TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1)\
 ||defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4)

//Valores de preescala 1,4,16 o 64. Este último valor solamente en algunos microcontroladores)
#define TIMER4_DIV_1				0b00000000   	//Preescala 1:1 (Sin preescala)
#define TIMER4_DIV_4				0b00000001   	//Preescala 1:4
#define TIMER4_DIV_16				0b00000010 		//Preescala 1:16
#define TIMER4_DIV_64				0b00000011 		//Preescala 1:64 (Ssi no está presente en el microcontrolador, se toma 1:16)

//Valores de postescala (del 1 al 16)
#define TIMER4_POST1			0b00000000	//Postescala 1:1
#define TIMER4_POST2			0b00001000  //Postescala 1:2
#define TIMER4_POST3			0b00010000  //Postescala 1:3
#define TIMER4_POST4			0b00011000  //Postescala 1:4
#define TIMER4_POST5			0b00100000  //Postescala 1:5
#define TIMER4_POST6			0b00101000  //Postescala 1:6
#define TIMER4_POST7			0b00110000  //Postescala 1:7
#define TIMER4_POST8			0b00111000  //Postescala 1:8
#define TIMER4_POST9			0b01000000  //Postescala 1:9
#define TIMER4_POST10			0b01001000  //Postescala 1:10
#define TIMER4_POST11			0b01010000  //Postescala 1:11
#define TIMER4_POST12			0b01011000  //Postescala 1:12
#define TIMER4_POST13			0b01100000  //Postescala 1:13
#define TIMER4_POST14			0b01101000  //Postescala 1:14
#define TIMER4_POST15			0b01110000  //Postescala 1:15
#define TIMER4_POST16			0b01111000  //Postescala 1:16


/*
	Prototipos de funciones
*/
void timer4_init(uint8_t param_config, uint8_t period);
uint8_t get_timer4(void);
void set_timer4(uint8_t valor_timer);

#endif

/*
	Parámetros de configuración Timer5
*/
#if defined (TMR_V5)

#define TIMER5_SLEEP_ENABLE		0b10000000 	//Timer 5 habilitado durante modo SLEEP
#define TIMER5_SLEEP_DISABLE	0b00000000 	//Timer 5 deshabilitado durante modo SLEEP

#define TIMER5_SPECIAL_EVENT_ENABLE		0b00000000 	//Reset de Timer 5 por evento especial habilitado
#define TIMER5_SPECIAL_EVENT_DISABLE	0b01000000 	//Reset de Timer 5 por evento especial deshabilitado 

#define TIMER5_SINGLE_SHOT_ENABLE		0b00100000 	//Modo de single shot para Timer 5
#define TIMER5_CONTINUOUS_COUNT			0b00000000 	//Modo de conteo contínuo para Timer 5

//Valores de preescala (1,2,4 u 8)
#define TIMER5_DIV_1			0b00000000  //Preescala 1:1 (Sin preescala)
#define TIMER5_DIV_2			0b00001000  //Preescala 1:2
#define TIMER5_DIV_4			0b00010000  //Preescala 1:4
#define TIMER5_DIV_8			0b00011000  //Preescala 1:8

//Definición de fuente del reloj para el Timer 5
#define TIMER5_EXTERNAL 		0b00000010	//Fuente de reloj son pulsos digitales con flanco de subida(al parecer)
#define TIMER5_INTERNAL 		0b00000010	//Fuente de reloj es reloj de instrucciones o ciclo máquina: Tcy =4*TOSC

//Configuración de funcionamiento sincronizado
#define TIMER5_SYNC				0b00000000		//Funcionamiento de reloj externo sincronizado
#define TIMER5_ASYNC			0b00000100      //Funcionamiento de reloj externo sin sincronizar

/*
	Prototipos de funciones
*/
void timer5_init(uint8_t param_config, uint16_t period);
uint16_t get_timer5(void);
void set_timer5(uint16_t valor_timer);

#elif defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_3) || defined (TMR_V7_4)

//Valores de preescala (1,2,4 u 8)
#define TIMER5_DIV_1			0b00000000  //Preescala 1:1 (Sin preescala)
#define TIMER5_DIV_2			0b00010000  //Preescala 1:2
#define TIMER5_DIV_4			0b00100000  //Preescala 1:4
#define TIMER5_DIV_8			0b00110000  //Preescala 1:8

//Configuración de oscilador externo/secundario o pulsos externos para utilización con timer
#if defined (TMR_V7_4)
#define TIMER5_INTOSC 	0b11000000  // //Fuente de reloj es periodo de oscilador interno
#endif
#define TIMER5_EXTERNAL				0b10000000  //Fuente de reloj es un cristal externo o pulsos digitales con flanco de subida
#define TIMER5_INTERNAL_SYS_CLK 	0b01000000  //Fuente de reloj es reloj de sistema: TOSC (No utilizar si Timer1 se asocia a ECCP)
#define TIMER5_INTERNAL_INST_CLK 	0b00000000  //Fuente de reloj es reloj de instrucciones o ciclo máquina: Tcy = 4*TOSC
#define TIMER5_OSCEN_ON 	0b00001000 	//Circuito para oscilador activado
#define TIMER5_OSCEN_OFF 	0b00000000 	//Circuito para oscilador desactivado

//Configuración de funcionamiento sincronizado
#define TIMER5_SYNC				0b00000000		//Funcionamiento de reloj externo sincronizado
#define TIMER5_ASYNC			0b00000100      //Funcionamiento de reloj externo sin sincronizar

//Modo de lectura/escritura de registro timer1 como registro de 8 o de 16 bits
#define TIMER5_8BIT_RW          0b00000000  //Habilita lectura/escritura del registro TMR1H:L en dos operaciones de 8 bits
#define TIMER5_16BIT_RW         0b00000010  //Habilita lectura/escritura del registro TMR1H:L en una operación de 16 bits

/*
	Prototipos de funciones
*/
void timer5_init(uint8_t param_config, uint8_t gate_control);
uint16_t get_timer5(void);
void set_timer5(uint16_t valor_timer);

#endif

/*
	Parámetros de configuración Timer6
*/
#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_3) || defined (TMR_V7_4)

//Valores de preescala 1,4,16 o 64. Este último valor solamente en algunos microcontroladores)
#define TIMER6_DIV_1				0b00000000   	//Preescala 1:1 (Sin preescala)
#define TIMER6_DIV_4				0b00000001   	//Preescala 1:4
#define TIMER6_DIV_16				0b00000010 		//Preescala 1:16
#define TIMER6_DIV_64				0b00000011 		//Preescala 1:64 (Ssi no está presente en el microcontrolador, se toma 1:16)

//Valores de postescala (del 1 al 16)
#define TIMER6_POST1			0b00000000	//Postescala 1:1
#define TIMER6_POST2			0b00001000  //Postescala 1:2
#define TIMER6_POST3			0b00010000  //Postescala 1:3
#define TIMER6_POST4			0b00011000  //Postescala 1:4
#define TIMER6_POST5			0b00100000  //Postescala 1:5
#define TIMER6_POST6			0b00101000  //Postescala 1:6
#define TIMER6_POST7			0b00110000  //Postescala 1:7
#define TIMER6_POST8			0b00111000  //Postescala 1:8
#define TIMER6_POST9			0b01000000  //Postescala 1:9
#define TIMER6_POST10			0b01001000  //Postescala 1:10
#define TIMER6_POST11			0b01010000  //Postescala 1:11
#define TIMER6_POST12			0b01011000  //Postescala 1:12
#define TIMER6_POST13			0b01100000  //Postescala 1:13
#define TIMER6_POST14			0b01101000  //Postescala 1:14
#define TIMER6_POST15			0b01110000  //Postescala 1:15
#define TIMER6_POST16			0b01111000  //Postescala 1:16

/*
	Prototipos de funciones
*/
void timer6_init(uint8_t param_config, uint8_t period);
uint8_t get_timer6(void);
void set_timer6(uint8_t valor_timer);

#endif

/*
	Parámetros de configuración Timer7
*/
#if defined (TMR_V7)

//Valores de preescala (1,2,4 u 8)
#define TIMER7_DIV_1			0b00000000  //Preescala 1:1 (Sin preescala)
#define TIMER7_DIV_2			0b00010000  //Preescala 1:2
#define TIMER7_DIV_4			0b00100000  //Preescala 1:4
#define TIMER7_DIV_8			0b00110000  //Preescala 1:8

//Configuración de oscilador externo/secundario o pulsos externos para utilización con timer
#define TIMER7_EXTERNAL				0b10000000  //Fuente de reloj es un cristal externo o pulsos digitales con flanco de subida
#define TIMER7_INTERNAL_SYS_CLK 	0b01000000  //Fuente de reloj es reloj de sistema: TOSC (No utilizar si Timer1 se asocia a ECCP)
#define TIMER7_INTERNAL_INST_CLK 	0b00000000  //Fuente de reloj es reloj de instrucciones o ciclo máquina: Tcy = 4*TOSC
#define TIMER7_OSCEN_ON 	0b00001000 	//Circuito para oscilador activado
#define TIMER7_OSCEN_OFF 	0b00000000 	//Circuito para oscilador desactivado

//Configuración de funcionamiento sincronizado
#define TIMER7_SYNC				0b00000000		//Funcionamiento de reloj externo sincronizado
#define TIMER7_ASYNC			0b00000100      //Funcionamiento de reloj externo sin sincronizar

//Modo de lectura/escritura de registro timer1 como registro de 8 o de 16 bits
#define TIMER7_8BIT_RW          0b00000000  //Habilita lectura/escritura del registro TMR1H:L en dos operaciones de 8 bits
#define TIMER7_16BIT_RW         0b00000010  //Habilita lectura/escritura del registro TMR1H:L en una operación de 16 bits

/*
	Prototipos de funciones
*/
void timer7_init(uint8_t param_config, uint8_t gate_control);
uint16_t get_timer7(void);
void set_timer7(uint16_t valor_timer);

#endif

/*
	Parámetros de configuración Timer8
*/

#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_4)

//Valores de preescala 1,4,16 o 64. Este último valor solamente en algunos microcontroladores)
#define TIMER8_DIV_1				0b00000000   	//Preescala 1:1 (Sin preescala)
#define TIMER8_DIV_4				0b00000001   	//Preescala 1:4
#define TIMER8_DIV_16				0b00000010 		//Preescala 1:16
#define TIMER8_DIV_64				0b00000011 		//Preescala 1:64 (Ssi no está presente en el microcontrolador, se toma 1:16)

//Valores de postescala (del 1 al 16)
#define TIMER8_POST1			0b00000000	//Postescala 1:1
#define TIMER8_POST2			0b00001000  //Postescala 1:2
#define TIMER8_POST3			0b00010000  //Postescala 1:3
#define TIMER8_POST4			0b00011000  //Postescala 1:4
#define TIMER8_POST5			0b00100000  //Postescala 1:5
#define TIMER8_POST6			0b00101000  //Postescala 1:6
#define TIMER8_POST7			0b00110000  //Postescala 1:7
#define TIMER8_POST8			0b00111000  //Postescala 1:8
#define TIMER8_POST9			0b01000000  //Postescala 1:9
#define TIMER8_POST10			0b01001000  //Postescala 1:10
#define TIMER8_POST11			0b01010000  //Postescala 1:11
#define TIMER8_POST12			0b01011000  //Postescala 1:12
#define TIMER8_POST13			0b01100000  //Postescala 1:13
#define TIMER8_POST14			0b01101000  //Postescala 1:14
#define TIMER8_POST15			0b01110000  //Postescala 1:15
#define TIMER8_POST16			0b01111000  //Postescala 1:16

/*
	Prototipos de funciones
*/
void timer8_init(uint8_t param_config, uint8_t period);
uint8_t get_timer8(void);
void set_timer8(uint8_t valor_timer);

#endif

/*
	Parámetros de configuración Timer10
*/

#if defined (TMR_V7)

//Valores de preescala 1,4,16 o 64. Este último valor solamente en algunos microcontroladores)
#define TIMER10_DIV_1				0b00000000   	//Preescala 1:1 (Sin preescala)
#define TIMER10_DIV_4				0b00000001   	//Preescala 1:4
#define TIMER10_DIV_16				0b00000010 		//Preescala 1:16
#define TIMER10_DIV_64				0b00000011 		//Preescala 1:64 (Ssi no está presente en el microcontrolador, se toma 1:16)

//Valores de postescala (del 1 al 16)
#define TIMER10_POST1			0b00000000	//Postescala 1:1
#define TIMER10_POST2			0b00001000  //Postescala 1:2
#define TIMER10_POST3			0b00010000  //Postescala 1:3
#define TIMER10_POST4			0b00011000  //Postescala 1:4
#define TIMER10_POST5			0b00100000  //Postescala 1:5
#define TIMER10_POST6			0b00101000  //Postescala 1:6
#define TIMER10_POST7			0b00110000  //Postescala 1:7
#define TIMER10_POST8			0b00111000  //Postescala 1:8
#define TIMER10_POST9			0b01000000  //Postescala 1:9
#define TIMER10_POST10			0b01001000  //Postescala 1:10
#define TIMER10_POST11			0b01010000  //Postescala 1:11
#define TIMER10_POST12			0b01011000  //Postescala 1:12
#define TIMER10_POST13			0b01100000  //Postescala 1:13
#define TIMER10_POST14			0b01101000  //Postescala 1:14
#define TIMER10_POST15			0b01110000  //Postescala 1:15
#define TIMER10_POST16			0b01111000  //Postescala 1:16

/*
	Prototipos de funciones
*/
void timer10_init(uint8_t param_config, uint8_t period);
uint8_t get_timer10(void);
void set_timer10(uint8_t valor_timer);

#endif


/*
	Parámetros de configuración Timer12
*/

#if defined (TMR_V7)

//Valores de preescala 1,4,16 o 64. Este último valor solamente en algunos microcontroladores)
#define TIMER12_DIV_1			0b00000000   	//Preescala 1:1 (Sin preescala)
#define TIMER12_DIV_4			0b00000001   	//Preescala 1:4
#define TIMER12_DIV_16			0b00000010 		//Preescala 1:16
#define TIMER12_DIV_64			0b00000011 		//Preescala 1:64 (Ssi no está presente en el microcontrolador, se toma 1:16)

//Valores de postescala (del 1 al 16)
#define TIMER12_POST1			0b00000000	//Postescala 1:1
#define TIMER12_POST2			0b00001000  //Postescala 1:2
#define TIMER12_POST3			0b00010000  //Postescala 1:3
#define TIMER12_POST4			0b00011000  //Postescala 1:4
#define TIMER12_POST5			0b00100000  //Postescala 1:5
#define TIMER12_POST6			0b00101000  //Postescala 1:6
#define TIMER12_POST7			0b00110000  //Postescala 1:7
#define TIMER12_POST8			0b00111000  //Postescala 1:8
#define TIMER12_POST9			0b01000000  //Postescala 1:9
#define TIMER12_POST10			0b01001000  //Postescala 1:10
#define TIMER12_POST11			0b01010000  //Postescala 1:11
#define TIMER12_POST12			0b01011000  //Postescala 1:12
#define TIMER12_POST13			0b01100000  //Postescala 1:13
#define TIMER12_POST14			0b01101000  //Postescala 1:14
#define TIMER12_POST15			0b01110000  //Postescala 1:15
#define TIMER12_POST16			0b01111000  //Postescala 1:16

#endif
/*
	Prototipos de funciones
*/
void timer12_init(uint8_t param_config, uint8_t period);
uint8_t get_timer12(void);
void set_timer12(uint8_t valor_timer);

/*
	Macros de configuración de gate control para Timers 1,3,5 y 7, en caso de que presenten dicha característica
*/
#if defined (TMR_V6)|| defined (TMR_V7) ||defined (TMR_V7_1) ||defined (TMR_V7_2) ||defined (TMR_V7_3) ||defined (TMR_V7_4) ||defined (TMR_V7_5)

#define TIMER_GATE_ON 	        0b10000000  //El conteo del timer se controla mediante la función Timerx gate
#define TIMER_GATE_OFF 	        0b00000000  //El timer siempre se encuentra en conteo


#define TIMER_GATE_POL_HI     	0b01000000  //Gate activo en alto
#define TIMER_GATE_POL_LO   	0b00000000  //Gate activo en bajo


#define TIMER_GATE_TOGGLE_ON 	0b00100000  //Modo Gate Toggle es habilitado
#define TIMER_GATE_TOGGLE_OFF 	0b00000000  //Modo Gate Toggle es deshabilitado

#define TIMER_GATE_1SHOT_ON     0b00010000  //Gate one shot es habilitado
#define TIMER_GATE_1SHOT_OFF    0b00000000  //Gate one shot es deshabilitado

#define TIMER_GATE_SRC_T1GPIN   0b00000000  //Gate de timer es el pin destinado a ello

#if defined (TMR_V7_4) ||defined (TMR_V7_5)
#define TIMER_GATE_SRC_Tx       0b00000001  //Cuando el registro TMR(x+1) es igual a PR(x+1), se activa gate del timer x
#define TIMER_GATE_SRC_CMP1     0b00000010  //La salida del comparador 1 es la fuente del gate
#define TIMER_GATE_SRC_CMP2     0b00000011  //La salida del comparador 2 es la fuente del gate
#else 
#define TIMER_GATE_SRC_T0       0b00000001  //Desborde de Timer0 es la fuente del gate
#define TIMER_GATE_SRC_T2       0b00000010  //Cuando el registro TMR2 es igual a PR2, se activa gate
#endif

#endif

/*
	Macros para configuración de fuente de reloj de módulos CCP
*/
#if defined (TMR_V4)

#define T34_SOURCE_CCP   		  	0b01001000		// Timer3 y Timer4 son fuentes de reloj para CCP1-CCP5
#define T12_CCP12_T34_CCP345		0b01000000		// Timer1 y Timer2 son fuentes de reloj para CCP1 y CCP2, mientras Timer3 y Timer4 son fuentes de reloj para CCP3-CCP5
#define T12_CCP1_T34_CCP2345		0b00001000		// Timer1 y Timer2 son fuentes de reloj para CCP1 y Timer3 y Timer4 son fuentes de reloj para CCP2-CCP5
#define T12_SOURCE_CCP   		  	0b00000000		// Timer1 y Timer2 son fuentes de reloj para CCP1-CCP5

#elif defined (TMR_V2)
#define T3_SOURCE_CCP        	0b01001000			// Timer3 es la fuente de reloj para Input Capture/Output Compare para ambos CCP
#define T1_CCP1_T3_CCP2       	0b00001000			// Timer1 es la fuente de reloj para Input Capture/Output Compare para CCP1 mientras Timer3 para CCP2
#define T1_SOURCE_CCP        	0b00000000			// Timer1 es la fuente de reloj para Input Capture/Output Compare para ambos CCP

#elif defined (TMR_V6)
#define T34_SOURCE_CCP12        0b00000011		// Timer3 y Timer4 son fuentes de reloj para CCP1 y CCP2 
#define T12_CCP1_T34_CCP2       0b00000001		// Timer1 y Timer2 son fuentes de reloj para CCP1, mientras Timer3 y Timer4 son fuentes de reloj para CCP2 
#define T12_SOURCE_CCP        	0b00000000		// Timer1 y Timer2 son fuentes de reloj para CCP1 y CCP2

#endif

/*
	Prototipo de función para selección de fuente de timer para módulos CCP
*/
#if defined (TMR_V4) || defined (TMR_V2) || defined (TMR_V6)

void setTimerCCPsource(uint8_t param_config);

#endif

#endif /* TIMERS_H */

