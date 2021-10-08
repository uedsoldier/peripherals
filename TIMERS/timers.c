/**
  * @file timers.c
  * @brief  Librería para utilización de módulos temporizadores para microcontrolladores de 8 bits
  * @author Ing. José Roberto Parra Trewartha
*/

#include "timers.h"

#if  defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
     defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	 defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	 || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief Función de inicialización de timer 0. El timer 0 deberá activarse manualmente mediante T0CONbits.TMR0ON = 1;
  * @param param_config: (uint8_t) Definiciones para configuración del timer 0, incluidas en el archivo timers.h. 
  * Se reinician los registros del Timer 0 a su estado en POR, y se configura su fuente de reloj y preescala.	
  * @return (void)
*/
void timer0_init(uint8_t param_config) {
	T0CON = param_config;		//Establece parámetros de configuración sin encender el timer
	TMR0H = 0;
	TMR0L = 0;					//Reinicia registro de conteo del timer0
	INTCONbits.TMR0IF = 0;		//Limpia bandera de interrupción por desborde del timer0
}
#endif

#if  defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) || \
     defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	 defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	  || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer0
  * @param valor_timer: (uint16_t) Valor entero con que se carga el registro TMR0H:L
  * @return (void) 
*/
void set_timer0(uint16_t valor_timer)
{
	TMR0H = make8(valor_timer,1);
	TMR0L = make8(valor_timer,0);
}
#endif

#if  defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
     defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	 defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	  || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief  Función que devuelve el valor del registro de conteo del Timer0
  * @param (void)
  * @return (uint16_t) Valor actual del registro de conteo TMR0H:L
*/
uint16_t get_timer0()
{
	uint8_t tmr0l = TMR0L;		//Lectura de byte bajo para actualizar buffer de TMR0H 
	return make16(TMR0H,tmr0l);
}
#endif

#if defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) || defined (TMR_V4) || defined (TMR_V5)
/**
  * @brief Función de inicialización de timer 1. El timer 1 deberá activarse manualmente mediante T1CONbits.TMR1ON = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 1, incluidas en el archivo timers.h.
  * Se reinician los registros del Timer 1 a su estado en POR, y se configura su fuente de reloj, preescala y modo de 8/16 bits.	
  * @return (void)
*/
void timer1_init(uint8_t param_config)
{
	T1CON = param_config;		//Establece parámetros de configuración sin encender el timer
	TMR1H = 0;
	TMR1L = 0;					//Reinicia registro de conteo TMR1H:L
	TMR1IF = 0;					//Limpia bandera de interrupción por desborde del timer1
}

#elif defined (TMR_V6)  || defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)

/**
  * @brief Función de inicialización de timer 1. El timer 1 deberá activarse manualmente mediante T1CONbits.TMR1ON = 1; 
  * La función Gate control se deberá activar a su vez mediante T1GCONbits.TMR1GE = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 1, incluidas en el archivo timers.h.
  * Se reinician los registros del Timer 1 a su estado en POR, y se configura su fuente de reloj, preescala y modo de 8/16 bits.
  * @param gate_control: (uint8_t) Configuración de modalidad Gate Control.	
  * @return (void)
*/
void timer1_init(uint8_t param_config, uint8_t gate_control)
{
	T1GCON = gate_control;		//Configuración de Gate control. Se recomienda configurar esto antes que al Timer
	T1CON = param_config;		//Establece parámetros de configuración sin encender el timer
	TMR1H = 0;
	TMR1L = 0;						//Reinicia registro de conteo TMR1H:L
	TMR1IF = 0;				//Limpia bandera de interrupción por desborde del timer1
	TMR1GIF = 0;			//Limpia bandera de interrupción por Gate Timer1 
}
#endif

#if defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
    defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	 || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief  Función que devuelve el valor del registro de conteo del Timer1
  * @param (void)
  * @return (uint16_t) Valor actual del registro de conteo TMR1H:L
*/
uint16_t get_timer1()
{
	uint8_t tmr1l = TMR1L;		//Lectura de byte bajo para actualizar buffer de TMR1H 
	return make16(TMR1H,tmr1l);
}
#endif

#if defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
    defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	 || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer1
  * @param valor_timer: (uint16_t) Valor entero con que se carga el registro TMR1H:L
  * @return (void) 
*/
void set_timer1(uint16_t valor_timer)
{
	TMR1H = make8(valor_timer,1);
	TMR1L = make8(valor_timer,0);
}
#endif

#if defined (TMR_V2) || defined (TMR_V3) || defined (TMR_V4) ||\
    defined (TMR_V5) || defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1)\
	|| defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief Función de inicialización de timer 2. El timer 2 deberá activarse manualmente mediante T2CONbits.TMR2ON = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 2, incluidas en el archivo timers.h. 
  * Se reinician los registros del Timer 2 a su estado en POR, y se configura su fuente de reloj, preescala y postescala.
  * @param period: (uint8_t) Valor de registro de periodo (PR2) del timer 2 	
  * @return (void)
*/
void timer2_init(uint8_t param_config, uint8_t period)
{
	T2CON = param_config;	//Establece parámetros de configuración sin encender el timer
	PR2 = period; 					//Establece registro de periodo del timer
	TMR2 = 0;						//Reinicia registro de conteo TMR2
	TMR2IF = 0;						//Limpia bandera de interrupción por desborde del timer2
}
#endif

#if defined (TMR_V2) || defined (TMR_V3) || defined (TMR_V4) || \
    defined (TMR_V5) || defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1)\
	|| defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief  Función que devuelve el valor del registro de conteo del Timer2
  * @param (void)
  * @return (uint8_t) Valor actual del registro de conteo TMR2
*/
uint8_t get_timer2()
{
	return TMR2;
}
#endif

#if defined (TMR_V2) || defined (TMR_V3) || defined (TMR_V4) || \
    defined (TMR_V5) || defined (TMR_V6)  || defined (TMR_V7) || defined (TMR_V7_1)\
	|| defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer2
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR2
  * @return (void) 
*/
void set_timer2(uint8_t valor_timer)
{
	TMR2 = valor_timer;
}

#endif

#if defined (TMR_V2) || defined (TMR_V4)
/**
  * @brief Función de inicialización de timer 3. El timer 3 deberá activarse manualmente mediante T3CONbits.TMR3ON = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 3, incluidas en el archivo timers.h.
  * Se reinician los registros del Timer 3 a su estado en POR, y se configura su fuente de reloj, preescala y modo de 8/16 bits.	
  * @return (void)
*/
void timer3_init(uint8_t param_config)
{
	T3CON |= ( param_config & 0xB7);		//Establece parámetros de configuración sin encender el timer e ignorando los bits de selección de fuente CCP
	TMR3H = 0;						
	TMR3L = 0; 							//Reinicia registro de conteo TMR3H:L
	TMR3IF = 0;				//Limpia bandera de interrupción por desborde del timer3
}
#endif

#if defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief Función de inicialización de timer 3. El timer 3 deberá activarse manualmente mediante T3CONbits.TMR3ON = 1; 
  * La función Gate control se deberá activar a su vez mediante T3GCONbits.TMR3GE = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 3, incluidas en el archivo timers.h.
  * Se reinician los registros del Timer 3 a su estado en POR, y se configura su fuente de reloj, preescala y modo de 8/16 bits.
  * @param gate_control: (uint8_t) Configuración de modalidad Gate Control.	
  * @return (void)
*/
void timer3_init(uint8_t param_config, uint8_t gate_control)
{
	T3GCON = gate_control;	//Configuración de Gate control. Se recomienda configurar esto antes que al Timer
	T3CON = param_config;	//Establece parámetros de configuración sin encender el timer
	TMR3H = 0;						
	TMR3L = 0; 				//Reinicia registro de conteo TMR3H:L
	TMR3IF = 0;				//Limpia bandera de interrupción por desborde del timer3
	TMR3GIF = 0;			//Limpia bandera de interrupción por Gate Timer3

}
#endif

#if defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
    defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	 || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief  Función que devuelve el valor del registro de conteo del Timer3
  * @param (void)
  * @return (uint16_t) Valor actual del registro de conteo TMR3H:L
*/
uint16_t get_timer3()
{
	uint8_t tmr3l = TMR3L;		//Lectura de byte bajo para actualizar buffer de TMR3H
	return make16(TMR3H,tmr3l);
}
#endif

#if defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
    defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	 || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer3
  * @param valor_timer: (uint16_t) Valor entero con que se carga el registro TMR3H:L
  * @return (void) 
*/
void set_timer3(uint16_t valor_timer)
{
	TMR3H = make8(valor_timer,1);
	TMR3L = make8(valor_timer,0);
}
#endif

#if defined (TMR_V4)  || defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
 || defined (TMR_V7_3) || defined (TMR_V7_4)
/**
  * @brief Función de inicialización de timer 4. El timer 4 deberá activarse manualmente mediante T4CONbits.TMR4ON = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 4, incluidas en el archivo timers.h. 
  * Se reinician los registros del Timer 4 a su estado en POR, y se configura su fuente de reloj, preescala y postescala.
  * @param period: (uint8_t) Valor de registro de periodo (PR4) del timer 4 	
  * @return (void)
*/
void timer4_init(uint8_t param_config, uint8_t period)
{
	T4CON = param_config;	//Establece parámetros de configuración sin encender el timer
	PR4 = period; 			//Establece registro de periodo del timer
	TMR4 = 0;				//Reinicia registro de conteo TMR4
	TMR4IF = 0;				//Limpia bandera de interrupción por desborde del timer4
}

#endif

#if defined (TMR_V4) || defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1)\
 || defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer4
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR4
  * @return (void) 
*/
uint8_t get_timer4()
{
	return TMR4;
}
#endif

#if defined (TMR_V4) || defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1) ||\
	defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer4
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR4
  * @return (void) 
*/
void set_timer4(uint8_t valor_timer)
{
	TMR4 = valor_timer;
}

#endif

#if defined (TMR_V5)
/**
  * @brief Función de inicialización de timer 5. El timer 5 deberá activarse manualmente mediante T5CONbits.TMR5ON = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 5, incluidas en el archivo timers.h.
  * Se reinician los registros del Timer 5 a su estado en POR, y se configura su fuente de reloj, preescala y modo de 8/16 bits.
  * @param period: (uint16_t) Variable para definir el valor del registro de periodo (PR5H:L) del timer 5	
  * @return (void)
*/
void timer5_init(uint8_t param_config, uint16_t period)
{
	T5CON = param_config;		//Establece parámetros de configuración sin encender el timer
	TMR5H = 0:						
	TMR5L = 0; 				//Reinicia registro de conteo TMR5H:L
	PR5L = period;
	PR5H = period>>8;		//Establece registros de periodo (PR5H:L) del timer 5 
	TMR5IF = 0;				//Limpia bandera de interrupción por desborde del timer5
}
#endif 

#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_3) || defined (TMR_V7_4)
/**
  * @brief Función de inicialización de timer 5. El timer 5 deberá activarse manualmente mediante T5CONbits.TMR5ON = 1; 
  * La función Gate control se deberá activar a su vez mediante T5GCONbits.TMR5GE = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 5, incluidas en el archivo timers.h.
  * Se reinician los registros del Timer 5 a su estado en POR, y se configura su fuente de reloj, preescala y modo de 8/16 bits.
  * @param gate_control: (uint8_t) Configuración de modalidad Gate Control.	
  * @return (void)
*/
void timer5_init(uint8_t param_config, uint8_t gate_control)
{
	T5GCON = gate_control;		//Configuración de Gate control. Se recomienda configurar esto antes que al Timer
	T5CON = param_config;		//Establece parámetros de configuración sin encender el timer
	TMR5H = 0;						
	TMR5L = 0; 							//Reinicia registro de conteo TMR5H:L
	TMR5IF = 0;				//Limpia bandera de interrupción por desborde del timer5
	TMR5GIF = 0;			//Limpia bandera de interrupción por Gate Timer1 
}

#endif

#if defined (TMR_V5) || defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_3) || defined (TMR_V7_4)
/**
  * @brief  Función que devuelve el valor del registro de conteo del Timer5
  * @param (void)
  * @return (uint16_t) Valor actual del registro de conteo TMR5H:L
*/
uint16_t get_timer5()
{
	uint8_t tmr5l = TMR5L;		//Lectura de byte bajo para actualizar buffer de TMR5H
	return make16(TMR5H,tmr5l);
}
#endif

#if defined (TMR_V5) || defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_3) || defined (TMR_V7_4)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer5
  * @param valor_timer: (uint16_t) Valor entero con que se carga el registro TMR5H:L
  * @return (void) 
*/
void set_timer5(uint16_t valor_timer)
{
	TMR5H = make8(valor_timer,1);
	TMR5L = make8(valor_timer,0);
}
#endif

#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_3) || defined (TMR_V7_4)
/**
  * @brief Función de inicialización de timer 6. El timer 6 deberá activarse manualmente mediante T6CONbits.TMR6ON = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 6, incluidas en el archivo timers.h. 
  * Se reinician los registros del Timer 6 a su estado en POR, y se configura su fuente de reloj, preescala y postescala.
  * @param period: (uint8_t) Valor de registro de periodo (PR6) del timer 6 	
  * @return (void)
*/
void timer6_init(uint8_t param_config, uint8_t period)
{
	T6CON = param_config;	//Establece parámetros de configuración sin encender el timer
	PR6 = period; 					//Establece registro de periodo del timer
	TMR6 = 0;						//Reinicia registro de conteo TMR6
	TMR6IF = 0;						//Limpia bandera de interrupción por desborde del timer6
}

#endif

#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_3) || defined (TMR_V7_4)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer6
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR6
  * @return (void) 
*/
uint8_t get_timer6()
{
	return TMR6;
}
#endif

#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_3) || defined (TMR_V7_4)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer6
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR6
  * @return (void) 
*/
void set_timer6(uint8_t valor_timer)
{
	TMR6 = valor_timer;
}
#endif

#if defined (TMR_V7)
/**
  * @brief Función de inicialización de timer 7. El timer 7 deberá activarse manualmente mediante T7CONbits.TMR7ON = 1; 
  * La función Gate control se deberá activar a su vez mediante T7GCONbits.TMR7GE = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 7, incluidas en el archivo timers.h.
  * Se reinician los registros del Timer 7 a su estado en POR, y se configura su fuente de reloj, preescala y modo de 8/16 bits.
  * @param gate_control: (uint8_t) Configuración de modalidad Gate Control.	
  * @return (void)
*/
void timer7_init(uint8_t param_config, uint8_t gate_control)
{
	T7GCON = gate_control;	//Configuración de Gate control. Se recomienda configurar esto antes que al Timer
	T7CON = param_config;	//Establece parámetros de configuración sin encender el timer
	TMR7H = 0:						
	TMR7L = 0; 				//Reinicia registro de conteo TMR7H:L
	TMR7IF = 0;				//Limpia bandera de interrupción por desborde del timer7
	TMR7GIF = 0;			//Limpia bandera de interrupción por Gate Timer1 
}
#endif

#if defined (TMR_V7)
/**
  * @brief  Función que devuelve el valor del registro de conteo del Timer7
  * @param (void)
  * @return (uint16_t) Valor actual del registro de conteo TMR7H:L
*/
uint16_t get_timer7()
{
	uint8_t tmr7l = TMR7L;		//Lectura de byte bajo para actualizar buffer de TMR7H
	return make16(TMR7H,tmr7l);
}
#endif

#if defined (TMR_V7)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer7
  * @param valor_timer: (uint16_t) Valor entero con que se carga el registro TMR7H:L
  * @return (void) 
*/
void set_timer7(uint16_t valor_timer)
{
	TMR7H = make8(valor_timer,1);
	TMR7L = make8(valor_timer,0);
}
#endif

#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_4)
/**
  * @brief Función de inicialización de timer 8. El timer 8 deberá activarse manualmente mediante T8CONbits.TMR8ON = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 8, incluidas en el archivo timers.h. 
  * Se reinician los registros del Timer 8 a su estado en POR, y se configura su fuente de reloj, preescala y postescala.
  * @param period: (uint8_t) Valor de registro de periodo (PR8) del timer 8 	
  * @return (void)
*/
void timer8_init(uint8_t param_config, uint8_t period)
{
	T8CON = param_config;	//Establece parámetros de configuración sin encender el timer
	PR8 = period; 			//Establece registro de periodo del timer
	TMR8 = 0;				//Reinicia registro de conteo TMR8
	TMR8IF = 0;				//Limpia bandera de interrupción por desborde del timer8
}

#endif

#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_4)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer8
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR8
  * @return (void) 
*/
uint8_t get_timer8()
{
	return TMR8;
}
#endif

#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_4)
/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer8
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR8
  * @return (void) 
*/
void set_timer8(uint8_t valor_timer)
{
	TMR8 = valor_timer;
}
#endif

#if defined (TMR_V7)
/**
  * @brief Función de inicialización de timer 10. El timer 10 deberá activarse manualmente mediante T10CONbits.TMR10ON = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 10, incluidas en el archivo timers.h. 
  * Se reinician los registros del Timer 10 a su estado en POR, y se configura su fuente de reloj, preescala y postescala.
  * @param period: (uint8_t) Valor de registro de periodo (PR10) del timer 10 	
  * @return (void)
*/
void timer10_init(uint8_t param_config, uint8_t period)
{
	T10CON = param_config;	//Establece parámetros de configuración sin encender el timer
	PR10 = period; 			//Establece registro de periodo del timer
	TMR10 = 0;				//Reinicia registro de conteo TMR10
	TMR10IF = 0;			//Limpia bandera de interrupción por desborde del timer10
}

/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer10
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR10
  * @return (void) 
*/
uint8_t get_timer10()
{
	return TMR10;
}

/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer10
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR10
  * @return (void) 
*/
void set_timer10(uint8_t valor_timer)
{
	TMR10 = valor_timer;
}
#endif

#if defined (TMR_V7)
/**
  * @brief Función de inicialización de timer 12. El timer 12 deberá activarse manualmente mediante T12CONbits.TMR12ON = 1;
  * @param param_config: (uint8_t)  Definiciones para configuración del timer 12, incluidas en el archivo timers.h. 
  * Se reinician los registros del Timer 12 a su estado en POR, y se configura su fuente de reloj, preescala y postescala.
  * @param period: (uint8_t) Valor de registro de periodo (PR12) del timer 12 	
  * @return (void)
*/
void timer12_init(uint8_t param_config, uint8_t period)
{
	T12CON = param_config;	//Establece parámetros de configuración sin encender el timer
	PR12 = period; 					//Establece registro de periodo del timer
	TMR12 = 0;						//Reinicia registro de conteo TMR12
	TMR12IF = 0;						//Limpia bandera de interrupción por desborde del timer12
}

/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer12
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR12
  * @return (void) 
*/
uint8_t get_timer12()
{
	return TMR12;
}

/**
  * @brief Función que establece/precarga el valor del registro de conteo del Timer12
  * @param valor_timer: (uint8_t) Valor entero con que se carga el registro TMR12
  * @return (void) 
*/
void set_timer12(uint8_t valor_timer)
{
	TMR12 = valor_timer;
}
#endif

#if defined (TMR_V4) || defined (TMR_V2) 
/**
  * @brief Función que establece la fuente de reloj (timer) para cada módulo CCP, modificando el valor del registro T3CON.
  * @param param_config: (uint8_t) Valor definido en timers.h para configurar fuente de reloj de los CCP.
  * @return (void) 
*/
void setTimerCCPsource(uint8_t param_config)
{
	T3CON &= 0xB7;	
  	T3CON |= (param_config & 0x48); 			// Timer to CCPx enable 
}
#elif defined (TMR_V6)
/**
  * @brief Función que establece la fuente de reloj (timer) para cada módulo CCP, modificando el valor del registro TCLKCON.
  * @param param_config: (uint8_t) Valor definido en timers.h para configurar fuente de reloj de los CCP.
  * @return (void) 
*/
void setTimerCCPsource(uint8_t param_config)
{
	TCLKCON &= 0xFC;
 	TCLKCON |= (param_config & 0x03);
}
#endif



