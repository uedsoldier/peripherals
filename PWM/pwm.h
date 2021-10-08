/*
Librería para manejo de módulos CCP en microcontroladores PIC de 8 bits. 
Autor: Ing. José Roberto Parra Trewartha
Compilador: XC8
*/

#ifndef __PWM_H
#define __PWM_H
/*
	Macro general de configuración PWM.
*/
#define CCP_PWM 					0x0C 	//Modo PWM

/*
	Macros para configuración de modo EPWM (sencillo, half-bridge o full-bridge), en microcontroladores con dicha característica.
	Utilizar la función pwmx_setOutput para definir funcionamiento en estos modos dentro de la aplicación
*/

#if  defined (PWM_V4) || defined (PWM_V5) || defined (PWM_V6) ||\
     defined (EPWM_V7) || defined (PWM_V8) || defined (PWM_V10) || \
	 defined (PWM_V11) || defined (PWM_V12) || defined (PWM_V13) ||\
	 defined (PWM_V14) || defined (PWM_V14_1) || defined (EPWM_V14)\
	 || defined (PWM_V14_3) || defined (PWM_V14_4) || defined (PWM_V14_5) || defined (EPWM_V14_3) ||\
	 defined (EPWM_V14_4) || defined (EPWM_V14_5) || defined (EPWM_V14_1) || defined (EPWM_V14_2) || \
	 defined (PWM_V14_2) || defined (PWM_V15_1) || defined (PWM_V15) \
	 || defined (EPWM_V15_1) || defined (EPWM_V15)

#define PWM_SINGLE_OUT              0x00	//Una sola salida. En algunos modelos solo es PxA, pero en otros modelos es posible utilizar PxB,PxC o PxD 
//por medio del registro PSTRxCON (Pulse steering control)
#define PWM_FULL_BRIDGE_FORWARD		0x40	//PxD modulada, PxA activa, PxB y PxC desactivados
#define PWM_HALF_BRIDGE				0x80	//PxA y PxB moduladas con control de banda muerta (dead band), PxC y PxD son pines de puerto
#define PWM_FULL_BRIDGE_REVERSE		0xC0	//PxB modulada, PxC activa, PxA y PxD desactivados


#endif

/* 
	Macros para definición de modo PWM. Se utilizan como segundo parámetro de las funciones pwmx_setOutput
*/
#define PWM_MODE_1     0b00001100 /* PxA,PxC active high, PxB,PxD active high */
#define PWM_MODE_2     0b00001101 /* PxA,PxC active high, PxB,PxD active low */
#define PWM_MODE_3     0b00001110 /* PxA,PxC active low, PxB,PxD active high */
#define PWM_MODE_4     0b00001111 /* PxA,PxC active low, PxB,PxD active low */

/*
	Macros correspondientes a direcciones de memoria de bits de configuración, en caso de que exista algún pin PWM multiplexado
*/
#define __CONFIG3L 0x300004
#define __CONFIG3H 0x300005

/*
	Macros para deteminar modo EPWM (sencillo, half-bridge o full-bridge)
*/
#define IS_DUAL_PWM(config) ((config) == PWM_HALF_BRIDGE)

#define IS_QUAD_PWM(config) ((config) == PWM_FULL_BRIDGE_FORWARD || \
                             (config) == PWM_FULL_BRIDGE_REVERSE)

#if defined (PWM_V14) || defined (PWM_V14_1) || defined (PWM_V14_3) || defined (PWM_V14_4) 


//***************** CCP source - TIMER selection ********************************************
//*************CCP4**************
#define CCP_4_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_4_SEL_TMR34		    0b00010000  	//CCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define CCP_4_SEL_TMR36		    0b00100000  	//CCP selects TIMER3 for Capture & Compare and TIMER6 for PWM

//*************CCP5**************
#define CCP_5_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_5_SEL_TMR54		    0b00010000  	//CCP selects TIMER5 for Capture & Compare and TIMER4 for PWM

//*************CCP6**************
#define CCP_6_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_6_SEL_TMR52		    0b00010000  	//CCP selects TIMER5 for Capture & Compare and TIMER2 for PWM

//*************CCP7**************
#define CCP_7_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_7_SEL_TMR54		    0b00010000  	//CCP selects TIMER5 for Capture & Compare and TIMER4 for PWM
#define CCP_7_SEL_TMR56		    0b00100000  	//CCP selects TIMER5 for Capture & Compare and TIMER6 for PWM
#define CCP_7_SEL_TMR58		    0b00110000  	//CCP selects TIMER5 for Capture & Compare and TIMER8 for PWM


#if defined (PWM_V14) || defined (PWM_V14_3)
//*************CCP8**************
#define CCP_8_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#if defined (PWM_V14)
#define CCP_8_SEL_TMR74		    0b00010000  	//CCP selects TIMER7 for Capture & Compare and TIMER4 for PWM
#define CCP_8_SEL_TMR76		    0b00100000  	//CCP selects TIMER7 for Capture & Compare and TIMER6 for PWM
#else
#define CCP_8_SEL_TMR54		    0b00010000  	//CCP selects TIMER5 for Capture & Compare and TIMER4 for PWM
#define CCP_8_SEL_TMR56		    0b00100000  	//CCP selects TIMER5 for Capture & Compare and TIMER6 for PWM
#endif
#endif


#if defined (PWM_V14_1) || defined (PWM_V14_4)
//*************CCP8**************
#define CCP_8_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_8_SEL_TMR14		    0b00010000  	//CCP selects TIMER1 for Capture & Compare and TIMER4 for PWM
#define CCP_8_SEL_TMR16		    0b00100000  	//CCP selects TIMER1 for Capture & Compare and TIMER6 for PWM

#endif

#if defined (PWM_V14) || defined (PWM_V14_3) 
//*************CCP9**************
#define CCP_9_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#if defined (PWM_V14_3)
#define CCP_9_SEL_TMR54		    0b00010000  	//CCP selects TIMER5 for Capture & Compare and TIMER4 for PWM
#else
#define CCP_9_SEL_TMR74		    0b00010000  	//CCP selects TIMER7 for Capture & Compare and TIMER4 for PWM
#endif


//*************CCP10**************
#define CCP_10_SEL_TMR12		 0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#if defined (PWM_V14_3)
#define CCP_10_SEL_TMR52		 0b00010000  	//CCP selects TIMER5 for Capture & Compare and TIMER4 for PWM
#else
#define CCP_10_SEL_TMR72		 0b00010000  	//CCP selects TIMER7 for Capture & Compare and TIMER2 for PWM
#endif

#endif
#endif

#if defined (EPWM_V14) || defined (EPWM_V14_1) || defined (EPWM_V14_3) || defined (EPWM_V14_4)
//***************** ECCP source - TIMER selection ********************************************
//*************ECCP1**************
#define ECCP_1_SEL_TMR12		    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_1_SEL_TMR34		    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_1_SEL_TMR36		    0b00100000  	//ECCP selects TIMER3 for Capture & Compare and TIMER6 for PWM
#define ECCP_1_SEL_TMR38		    0b00110000  	//ECCP selects TIMER3 for Capture & Compare and TIMER8 for PWM
#if defined (EPWM_V14)
#define ECCP_1_SEL_TMR310		    0b01000000  	//ECCP selects TIMER3 for Capture & Compare and TIMER10 for PWM
#define ECCP_1_SEL_TMR312		    0b01010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER12 for PWM
#endif

//*************ECCP2**************
#define ECCP_2_SEL_TMR12		    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_2_SEL_TMR34		    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_2_SEL_TMR36		    0b00100000  	//ECCP selects TIMER3 for Capture & Compare and TIMER6 for PWM
#define ECCP_2_SEL_TMR38		    0b00110000  	//ECCP selects TIMER3 for Capture & Compare and TIMER8 for PWM
#if defined (EPWM_V14)
#define ECCP_2_SEL_TMR310		    0b01000000  	//ECCP selects TIMER3 for Capture & Compare and TIMER10 for PWM
#endif

//*************ECCP3**************
#define ECCP_3_SEL_TMR12		    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_3_SEL_TMR34		    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_3_SEL_TMR36		    0b00100000  	//ECCP selects TIMER3 for Capture & Compare and TIMER6 for PWM
#define ECCP_3_SEL_TMR38		    0b00110000  	//ECCP selects TIMER3 for Capture & Compare and TIMER8 for PWM

#endif

#if defined (PWM_V14_5) || defined (EPWM_V14_5)
#define ECCP_1_SEL_TMR12	    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_1_SEL_TMR32	    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER2 for PWM

#define CCP_2_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_2_SEL_TMR32		    0b00010000  	//CCP selects TIMER3 for Capture & Compare and TIMER2 for PWM

#endif

#if defined (PWM_V14_2) || defined (EPWM_V14_2)
#define ECCP_1_SEL_TMR12	    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_1_SEL_TMR34	    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM

#define CCP_2_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_2_SEL_TMR34		    0b00010000  	//CCP selects TIMER3 for Capture & Compare and TIMER4 for PWM

#define CCP_3_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_3_SEL_TMR34		    0b00010000  	//CCP selects TIMER3 for Capture & Compare and TIMER4 for PWM

#define CCP_4_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_4_SEL_TMR34		    0b00010000  	//CCP selects TIMER3 for Capture & Compare and TIMER4 for PWM

#define CCP_5_SEL_TMR12		    0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_5_SEL_TMR34		    0b00010000  	//CCP selects TIMER5 for Capture & Compare and TIMER4 for PWM
#endif

#if defined (PWM_V15) || defined (EPWM_V15) || defined (PWM_V15_1) || defined (EPWM_V15_1)
#define ECCP_1_SEL_TMR12	    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_1_SEL_TMR34	    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_1_SEL_TMR56	    0b00100000  	//ECCP selects TIMER5 for Capture & Compare and TIMER6 for PWM

#define ECCP_2_SEL_TMR12	    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_2_SEL_TMR34	    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_2_SEL_TMR56	    0b00100000  	//ECCP selects TIMER5 for Capture & Compare and TIMER6 for PWM

#define ECCP_3_SEL_TMR12	    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_3_SEL_TMR34	    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_3_SEL_TMR56	    0b00100000  	//ECCP selects TIMER5 for Capture & Compare and TIMER6 for PWM

#define CCP_4_SEL_TMR12	    	0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_4_SEL_TMR34	    	0b00010000  	//CCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define CCP_4_SEL_TMR56	    	0b00100000  	//CCP selects TIMER5 for Capture & Compare and TIMER6 for PWM

#define CCP_5_SEL_TMR12	    	0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_5_SEL_TMR34	    	0b00010000  	//CCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define CCP_5_SEL_TMR56	    	0b00100000  	//CCP selects TIMER5 for Capture & Compare and TIMER6 for PWM
#endif


/*
	Prototipos de funciones para PWM 1
*/
void pwm1_init(uint8_t param_config_timer, uint8_t periodo);

void pwm1_setDuty(uint16_t duty);

#if  defined (PWM_V4) || defined (PWM_V5) || defined (PWM_V6) ||\
     defined (EPWM_V7) || defined (PWM_V8) || defined (PWM_V10) || \
	 defined (PWM_V11) || defined (PWM_V12)|| defined (PWM_V13)
void pwm1_setOutput(uint8_t output_config, uint8_t pwm_mode);
#endif

#if defined (PWM_V1) || defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V5) || defined (PWM_V6) || defined (PWM_V7) || defined (EPWM_V7) || defined (PWM_V9)\
	|| defined (PWM_V8) || defined (PWM_V10) || defined (PWM_V12) || defined (PWM_V11) || defined (PWM_V13)
void pwm1_close(void);
#endif

/*
	Prototipos de funciones para PWM 2
*/
#if defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V5) || defined (PWM_V6) || defined (PWM_V10) ||  defined (PWM_V11) ||\
	defined (PWM_V4) || defined (PWM_V9) || defined (PWM_V13)
void pwm2_init(uint8_t param_config_timer, uint8_t periodo);  
#elif defined (PWM_V14_2) || defined (PWM_V14_5)
void pwm2_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
#endif

#if defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V5) ||\
	defined (PWM_V6) || defined (PWM_V9) ||  defined (PWM_V10) ||  defined (PWM_V11)\
	|| defined (PWM_V13)  || defined (PWM_V14_2) || defined (PWM_V14_5)
void pwm2_setDuty(uint16_t duty);
#endif

#if defined (PWM_V4)
void pwm2_setOutput(uint8_t output_config, uint8_t pwm_mode);
#endif

#if defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V5)\
    || defined (PWM_V6) || defined (PWM_V9) ||  defined (PWM_V10) ||  defined (PWM_V11)\
    || defined (PWM_V13)  || defined (PWM_V14_2) || defined (PWM_V14_5)
void pwm2_close(void);
#endif

/*
	Prototipos de funciones para PWM 3
*/
#if defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V9) 
void pwm3_init(uint8_t param_config_timer, uint8_t periodo);  
#elif defined (PWM_V14_2)
void pwm3_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
#endif
#if defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V9) || defined (PWM_V14_2)
void pwm3_setDuty(uint16_t duty);
#endif
#if defined (PWM_V4)
void pwm3_setOutput(uint8_t output_config, uint8_t pwm_mode);
#endif
#if defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V9)  || defined (PWM_V14_2)
void pwm3_close(void);
#endif

/*
	Prototipos de funciones para PWM 4
*/
#if defined (PWM_V4) || defined (PWM_V9)
void pwm4_init(uint8_t param_config_timer, uint8_t periodo); 
#elif defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4) || defined (PWM_V14_2) || defined (PWM_V15) || defined (PWM_V15_1)
void pwm4_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
#endif
#if defined (PWM_V4) || defined (PWM_V9) || defined (PWM_V14) || defined (PWM_V14_1)\
	 || defined (PWM_V14_2) ||defined (PWM_V14_3) || defined (PWM_V14_4) \
	 || defined (PWM_V15) ||defined (PWM_V15_1)
void pwm4_setDuty(uint16_t duty);
#endif
#if defined (PWM_V4) || defined (PWM_V9) || defined (PWM_V14) || defined (PWM_V14_1)\
	 || defined (PWM_V14_2) ||defined (PWM_V14_3) || defined (PWM_V14_4) ||\
	 defined (PWM_V15) ||defined (PWM_V15_1)
void pwm4_close(void);
#endif


/*
	Prototipos de funciones para PWM 5
*/
#if defined (PWM_V4) || defined (PWM_V9)
void pwm5_init(uint8_t param_config_timer, uint8_t periodo); 
#elif defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4) || defined (PWM_V14_2) || defined (PWM_V15) ||defined (PWM_V15_1)
void pwm5_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
#endif
#if defined (PWM_V4) || defined (PWM_V9) || defined (PWM_V14) || defined (PWM_V14_1)\
	 || defined (PWM_V14_2) ||defined (PWM_V14_3) || defined (PWM_V14_4) || defined (PWM_V15) ||defined (PWM_V15_1)
void pwm5_setDuty(uint16_t duty);
void pwm5_close(void);
#endif

/*
	Prototipos de funciones para PWM 6
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm6_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
void pwm6_setDuty(uint16_t duty);
void pwm6_close(void);
#endif

/*
	Prototipos de funciones para PWM 7
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm7_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
void pwm7_setDuty(uint16_t duty);
void pwm7_close(void);
#endif

/*
	Prototipos de funciones para PWM 8
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm8_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
void pwm8_setDuty(uint16_t duty);
void pwm8_close(void);
#endif

/*
	Prototipos de funciones para PWM 9
*/
#if defined (PWM_V14) || defined (PWM_V14_3)
void pwm9_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
void pwm9_setDuty(uint16_t duty);
void pwm9_close(void);
#endif

/*
	Prototipos de funciones para PWM 10
*/
#if defined (PWM_V14) ||defined (PWM_V14_3) 
void pwm10_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
void pwm10_setDuty(uint16_t duty);
void pwm10_close(void);
#endif


/*
	Prototipos de funciones para EPWM 1
*/

#if defined (EPWM_V7)
void epwm1_init(uint8_t param_config_timer, uint8_t periodo);
#elif defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4) ||\
	defined (EPWM_V14_2) || defined (EPWM_V14_5) || defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm1_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
#endif

#if defined (EPWM_V7) || defined (EPWM_V14) || defined (EPWM_V14_1) || defined (EPWM_V14_2) \
	||defined (EPWM_V14_3) || defined (EPWM_V14_4) || defined (EPWM_V14_5) || defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm1_setDuty(uint16_t duty);
#endif

#if defined (EPWM_V7) || defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4) ||\
	defined (EPWM_V14_2) || defined (EPWM_V14_5) || defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm1_setOutput(uint8_t output_config, uint8_t pwm_mode);
#endif

#if defined (EPWM_V7) || defined (EPWM_V14) || defined (EPWM_V14_1) || defined (EPWM_V14_2) || defined (EPWM_V15) ||defined (EPWM_V15_1) ||\
	defined (EPWM_V14_3) || defined (EPWM_V14_4) || defined (EPWM_V14_5)
void epwm1_close(void);
#endif 


/*
	Prototipos de funciones para EPWM 2
*/
#if defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4) || defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm2_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
#endif

#if defined (EPWM_V14) || defined (EPWM_V14_1)||defined (EPWM_V14_3) || \
	defined (EPWM_V14_4)|| defined (EPWM_V15) ||defined (EPWM_V15_1)
void epwm2_setDuty(uint16_t duty);
#endif

#if defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4) || defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm2_setOutput(uint8_t output_config, uint8_t pwm_mode);
#endif

#if defined (EPWM_V14) || defined (EPWM_V14_1)|| defined (EPWM_V15) ||defined (EPWM_V15_1) || defined (EPWM_V14_3) || defined (EPWM_V14_4)
void epwm2_close(void);
#endif

/*
	Prototipos de funciones para EPWM 3
*/
#if defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4) || defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm3_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source);
#endif
#if defined (EPWM_V14) || defined (EPWM_V14_1)||defined (EPWM_V14_3) || \
	defined (EPWM_V14_4)|| defined (EPWM_V15) ||defined (EPWM_V15_1)
void epwm3_setDuty(uint16_t duty);
#endif
#if defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4) || defined (EPWM_V15_1) || defined (EPWM_V15) 
void epwm3_setOutput(uint8_t output_config, uint8_t pwm_mode);
#endif
#if defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) ||\
 defined (EPWM_V14_4)|| defined (EPWM_V15) ||defined (EPWM_V15_1)
void epwm3_close(void);
#endif

#if defined CC1_IO_V1	
#define PWM1_TRIS	TRISBbits.TRISB3 
#elif defined CC1_IO_V2
#define PWM1_TRIS	TRISCbits.TRISC5 
#elif defined CC8_IO_V1
#define PWM1_TRIS    TRISBbits.TRISB4
#elif defined CC8_IO_V2
#define PWM1_TRIS    TRISDbits.TRISD4
#elif defined CC9_IO_V1
#define PWM1_TRIS    TRISCbits.TRISC2
#else
#define	PWM1_TRIS	TRISCbits.TRISC2 
#endif

#if defined CC8_IO_V1
#define PWM2_TRIS    TRISCbits.TRISC2
#elif defined CC8_IO_V2
#define PWM2_TRIS    TRISCbits.TRISC2
#elif defined CC9_IO_V1
#define PWM2_TRIS    TRISBbits.TRISB3
#else
#define PWM2_TRIS    TRISEbits.TRISE7
#endif

#if defined CC3_IO_V1	
#define PWM3_TRIS	TRISDbits.TRISD1 
#elif defined CC8_IO_V1
#define PWM3_TRIS    TRISCbits.TRISC6
#elif defined CC8_IO_V2
#define PWM3_TRIS    TRISCbits.TRISC6
#elif defined CC9_IO_V1
#define PWM3_TRIS    TRISBbits.TRISB5
#else	
#define PWM3_TRIS	TRISGbits.TRISG0 
#endif

#if defined CC4_IO_V1
#define PWM4_TRIS    TRISDbits.TRISD2
#elif defined CC4_IO_V2
#define PWM4_TRIS    TRISBbits.TRISB4
#elif defined CC8_IO_V1
#define PWM4_TRIS    TRISCbits.TRISC7
#elif defined CC8_IO_V2
#define PWM4_TRIS    TRISCbits.TRISC7
#elif defined CC9_IO_V1
#define PWM4_TRIS    TRISBbits.TRISB0
#else
#define PWM4_TRIS    TRISGbits.TRISG3
#endif

#if defined CC4_IO_V2	
#define PWM5_TRIS    TRISBbits.TRISB5
#elif defined CC8_IO_V1
#define PWM5_TRIS    TRISBbits.TRISB5
#elif defined CC8_IO_V2
#define PWM5_TRIS    TRISBbits.TRISB5
#elif defined CC9_IO_V1
#define PWM5_TRIS    TRISAbits.TRISA4
#else	
#define PWM5_TRIS    TRISGbits.TRISG4
#endif

#if defined CC4_IO_V2	
#define PWM6_TRIS    TRISBbits.TRISB6
#else	
#define PWM6_TRIS    TRISEbits.TRISE6
#endif

#if defined CC4_IO_V2	
#define PWM7_TRIS    TRISBbits.TRISB7
#else	
#define PWM7_TRIS    TRISEbits.TRISE5
#endif

#if defined CC4_IO_V2	
#define PWM8_TRIS    TRISCbits.TRISC1
#else	
#define PWM8_TRIS    TRISEbits.TRISE4
#endif

#if defined CC4_IO_V2	
#define PWM9_TRIS    TRISCbits.TRISC6
#else	
#define PWM9_TRIS    TRISEbits.TRISE3
#endif

#if defined CC4_IO_V2	
#define PWM10_TRIS    TRISCbits.TRISC7
#else	
#define PWM10_TRIS   TRISEbits.TRISE2
#endif

#endif