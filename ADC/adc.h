/*
	Librería para utilización del módulo ADC en microcontroladores PIC de 8 bits
	Autor: Ing. José Roberto Parra Trewartha
	Compilador: XC8
*/
#ifndef ADC_H
#define ADC_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../pconfig.h"
#include "../../utils/utils.h"

//Definición de macros para función de lectura ADC
#define ADC_START_READ	0		//Inicia conversión, espera y devuelve resultado
#define ADC_START_ONLY	1		//Inicia conversión únicamente
#define ADC_READ_ONLY	2		//Únicamente devuelve resultado de la última conversión

#if	defined (ADC_V15) || defined (ADC_V15_1)
#define ADCON1H_VALUE                0x00
#define ADCON1L_VALUE                0x00
#define ADCON2H_VALUE                0x00
#define ADCON2L_VALUE                0x00
#define ADCON3H_VALUE                0x00
#define ADCON3L_VALUE                0x00
#define ADCON5H_VALUE                0x00
#define ADCON5L_VALUE                0x00
#define ADCHS0H_VALUE                0x00
#define ADCHS0L_VALUE                0x00
#define ADCSS1H_VALUE                0x00
#define ADCSS1L_VALUE                0x00
#define ADCSS0H_VALUE                0x00
#define ADCSS0L_VALUE                0x00
#endif

#if	defined (ADC_V1) || defined (ADC_V2) || defined (ADC_V3) || defined (ADC_V4) ||\
    defined (ADC_V5) || defined (ADC_V6) || defined (ADC_V8) || defined (ADC_V9) || defined (ADC_V9_1) ||\
	defined (ADC_V10) || defined (ADC_V11) || defined (ADC_V11_1) || defined (ADC_V12)\
	|| defined (ADC_V13) || defined (ADC_V13_1) || defined (ADC_V13_2) ||\
	defined (ADC_V13_3) || defined (ADC_V14) || defined (ADC_V14_1) || defined (ADC_V14_2) || defined (ADC_V14_3) 

//Definiciones para selección de reloj de conversión
#define ADC_FOSC_2       0b00000000 			//Reloj de conversión A/D es Fosc/2
#define ADC_FOSC_4       0b01000000 			//Reloj de conversión A/D es Fosc/4
#define ADC_FOSC_8       0b00010000 			//Reloj de conversión A/D es Fosc/8
#define ADC_FOSC_16      0b01010000 			//Reloj de conversión A/D es Fosc/16
#define ADC_FOSC_32      0b00100000 			//Reloj de conversión A/D es Fosc/32
#define ADC_FOSC_64      0b01100000 			//Reloj de conversión A/D es Fosc/64
#define ADC_FOSC_RC      0b01110000 			//Reloj de conversión A/D es Oscilador interno RC

//Definiciones para tiempo de adquisición Tacq 
#define ADC_0_TAD        0b00000000				//Tiempo de adquisición A/D es 0 TAD
#define ADC_2_TAD        0b00000010				//Tiempo de adquisición A/D es 2 TAD
#define ADC_4_TAD        0b00000100				//Tiempo de adquisición A/D es 4 TAD
#define ADC_6_TAD        0b00000110				//Tiempo de adquisición A/D es 6 TAD
#define ADC_8_TAD        0b00001000				//Tiempo de adquisición A/D es 8 TAD
#define ADC_12_TAD       0b00001010				//Tiempo de adquisición A/D es 12 TAD
#define ADC_16_TAD       0b00001100				//Tiempo de adquisición A/D es 16 TAD
#define ADC_20_TAD       0b00001110				//Tiempo de adquisición A/D es 20 TAD

#endif


#if defined (ADC_V2) || defined (ADC_V3) || defined (ADC_V4) || defined (ADC_V5) ||\
    defined (ADC_V6) || defined (ADC_V8) || defined (ADC_V9) || defined (ADC_V9_1) || defined (ADC_V11)|| defined (ADC_V11_1)||\
	defined (ADC_V12)
//Definiciones de configuración de voltaje de referencia para módulo ADC 
#define ADC_REF_VDD_VREFMINUS   	0b00000010 	// Referencias de voltaje para el ADC son VREF+ = VDD y VREF- = fuente externa en pin VREF-
#define ADC_REF_VREFPLUS_VREFMINUS  0b00000011 	// Referencias de voltaje para el ADC son VREF+ = fuente externa en pin VREF+ y VREF- = fuente externa en pin VREF-
#define ADC_REF_VREFPLUS_VSS		0b00000001 	// Referencias de voltaje para el ADC son VREF+ = fuente externa en pin VREF+ y VREF- = VSS
#define	ADC_REF_VDD_VSS  		    0b00000000 	// Referencias de voltaje para el ADC son VREF+ = VDD y VREF- = VSS
#endif

//Selección de formato de conversión A/D para todas las versiones
#define ADC_RIGHT_JUST   0b10000000 			//Resultado A/D justificado a la derecha
#define ADC_LEFT_JUST    0b00000000 			//Resultado A/D justificado a la izquierda

/**********************************************************************************************************************************/


#if defined (ADC_V1)

//*************** Control de configuración de puertos analógicos *******************************
#define ADC_8ANA_0REF    0b00000000				// VREF+=VDD,VREF-=VSS: todos los canales se configuran analógicos   (8 canales analógicos/0 referencias de voltaje)
#define ADC_7ANA_1REF    0b00000001				// AN3=VREF+:  todos los canales se configuran analógicos except AN3 (7 canales analógicos/1 referencias de voltaje)
#define ADC_5ANA_0REF    0b00000010 			// VREF+=VDD,VREF-=VSS: DIG-AN7,6,5 : ANG-AN4,3,2,1,0  (5 canales analógicos/0 referencias de voltaje)
#define ADC_4ANA_1REF    0b00000011 			// AN3=VREF+:  DIG- AN7,6,5 : ANG- AN4,2,1,0  (4 canales analógicos/1 referencias de voltaje)
#define ADC_3ANA_0REF    0b00000100 			// VREF+=VDD,VREF-=VSS: DIG- AN7,6,5,4,2 : ANG- AN3,1,0   (3 canales analógicos/0 referencias de voltaje)
#define ADC_2ANA_1REF    0b00000101 			// AN3=VREF+:  DIG- AN7,6,5,4,2 : ANG- AN1,0   (2 canales analógicos/1 referencias de voltaje)
#define ADC_0ANA_0REF    0b00000111 			// Todos los canales se configuran digitales  (0 canales analógicos/0 referencias de voltaje)
#define ADC_6ANA_2REF    0b00001000 			// AN3=VREF+,AN2=VREF-:  ANG- AN7,6,5,4,1,0  (6 canales analógicos/2 referencias de voltaje)
#define ADC_6ANA_0REF    0b00001001 			// VREF+=VDD,VREF-=VSS:  DIG- AN7,6 : ANG- AN5,4,3,2,1,0  (6 canales analógicos/0 referencias de voltaje)
#define ADC_5ANA_1REF    0b00001010 			// AN3=VREF+,VREF-=VSS:  DIG- AN7,6 : ANG- AN5,4,2,1,0  (5 canales analógicos/1 referencias de voltaje)
#define ADC_4ANA_2REF    0b00001011 			// AN3=VREF+ AN2=VREF-:   DIG- AN7,6 : ANG- AN5,4,1,0 (4 canales analógicos/2 referencias de voltaje)
#define ADC_3ANA_2REF    0b00001100 			// AN3=VREF+ AN2=VREF-:  DIG- AN7,6,5 : ANG- AN4,1,0  (3 canales analógicos/2 referencias de voltaje)
#define ADC_2ANA_2REF    0b00001101 			// AN3=VREF+ AN2=VREF-:   DIG- AN7,6,5,4 : ANG- AN1,0  (2 canales analógicos/2 referencias de voltaje)
#define ADC_1ANA_0REF    0b00001110 			// La única entrada analógica es AN0  (1 canales analógicos/0 referencias de voltaje)
#define ADC_1ANA_2REF    0b00001111 			// AN3=VREF+ AN2=VREF-:   DIG- AN7,6,5,4,1 : ANG- AN0   (1 canales analógicos/2 referencias de voltaje)

/**********************************************************************************************************************************/


#elif defined (ADC_V2) || defined (ADC_V5) || defined (ADC_V6) || defined (ADC_V12) 

//*************** Control de configuración de puertos analógicos *******************************
#define ADC_0ANA   0b00001111 					// Todos los canales se configuran digitales
#define ADC_1ANA   0b00001110 					// Canales analógicos: AN0  y los canales restantes se configuran digitales
#define ADC_2ANA   0b00001101 					// Canales analógicos: AN0->1 y los canales restantes se configuran digitales
#define ADC_3ANA   0b00001100 					// Canales analógicos: AN0->2 y los canales restantes se configuran digitales
#define ADC_4ANA   0b00001011 					// Canales analógicos: AN0->3 y los canales restantes se configuran digitales
#define ADC_5ANA   0b00001010 					// Canales analógicos: AN0->4 y los canales restantes se configuran digitales
#define ADC_6ANA   0b00001001 					// Canales analógicos: AN0->5 y los canales restantes se configuran digitales
#define ADC_7ANA   0b00001000 					// Canales analógicos: AN0->6 y los canales restantes se configuran digitales
#define ADC_8ANA   0b00000111 					// Canales analógicos: AN0->7 y los canales restantes se configuran digitales
#define ADC_9ANA   0b00000110 					// Canales analógicos: AN0->8 y los canales restantes se configuran digitales
#define ADC_10ANA  0b00000101 					// Canales analógicos: AN0->9 y los canales restantes se configuran digitales
#define ADC_11ANA  0b00000100 					// Canales analógicos: AN0->10 y los canales restantes se configuran digitales
#ifndef ADC_V12
#define ADC_12ANA  0b00000011 					// Canales analógicos: AN0->11 y los canales restantes se configuran digitales
#define ADC_13ANA  0b00000010 					// Canales analógicos: AN0->12 y los canales restantes se configuran digitales
#define ADC_14ANA  0b00000001 					// Canales analógicos: AN0->13 y los canales restantes se configuran digitales
#define ADC_15ANA  0b00000000 					// Todos los canales se configuran analógicos
#endif

/**********************************************************************************************************************************/
#elif defined (ADC_V3)
	//*************** Control de configuración de puertos analógicos *******************************
#define ADC_0ANA   0b11111111 					// Todos los canales se configuran digitales
#define ADC_1ANA   0b11111110	 				// Canales analógicos: AN0  y los canales restantes se configuran digitales
#define ADC_2ANA   0b11111100 					// Canales analógicos: AN0->1 y los canales restantes se configuran digitales
#define ADC_3ANA   0b11111000 					// Canales analógicos: AN0->2 y los canales restantes se configuran digitales
#define ADC_4ANA   0b11110000 					// Canales analógicos: AN0->3  y los canales restantes se configuran digitales
#define ADC_5ANA   0b11100000 					// Canales analógicos: AN0->4  y los canales restantes se configuran digitales
#define ADC_6ANA   0b11000000 					// Canales analógicos: AN0->5  y los canales restantes se configuran digitales
#define ADC_7ANA   0b10000000 					// Canales analógicos: AN0->6  y los canales restantes se configuran digitales

/**********************************************************************************************************************************/
#elif defined (ADC_V4)
//*************** Control de configuración de puertos analógicos *******************************
#define ADC_0ANA   0b00001111 					// Todos los canales se configuran digitales
#define ADC_1ANA   0b00001110	 				// Canales analógicos: AN0  y los canales restantes se configuran digitales
#define ADC_2ANA   0b00001100 					// Canales analógicos: AN0->1 y los canales restantes se configuran digitales
#define ADC_3ANA   0b00001000 					// Canales analógicos: AN0->2 y los canales restantes se configuran digitales
#define ADC_4ANA   0b00000000 					// Canales analógicos: AN0->3  y los canales restantes se configuran digitales

/**********************************************************************************************************************************/
#elif defined (ADC_V9) || defined (ADC_V9_1) 
//*************** Control de configuración de puertos analógicos *******************************
#define ADC_0ANA      0b1111111111111111 		// Todos los canales se configuran digitales
#define ADC_1ANA      0b1111111111111110	 	// Canales analógicos: AN0 y los canales restantes se configuran digitales
#define ADC_2ANA      0b1111111111111100 		// Canales analógicos: AN0-AN1 y los canales restantes se configuran digitales
#define ADC_3ANA      0b1111111111111000 		// Canales analógicos: AN0-AN2 y los canales restantes se configuran digitales
#define ADC_4ANA      0b1111111111110000 		// Canales analógicos: AN0-AN3 y los canales restantes se configuran digitales
#define ADC_5ANA      0b1111111111100000		// Canales analógicos: AN0-AN4 y los canales restantes se configuran digitales
#define ADC_6ANA      0b1111111111000000	 	// Canales analógicos: AN0-AN5 y los canales restantes se configuran digitales
#if defined (ADC_V9)
#define ADC_7ANA      0b1111111110000000 		// Canales analógicos: AN0-AN6 y los canales restantes se configuran digitales
#endif
#define ADC_8ANA      0b1111111100000000 		// Canales analógicos: AN0-AN7 y los canales restantes se configuran digitales
#if defined (ADC_V9)
#define ADC_9ANA      0b1111111000000000 		// Canales analógicos: AN0-AN8 y los canales restantes se configuran digitales
#define ADC_10ANA     0b1111110000000000 		// Canales analógicos: AN0-AN9 y los canales restantes se configuran digitales
#endif
#define ADC_11ANA     0b1111100000000000	 	// Canales analógicos: AN0-AN10 y los canales restantes se configuran digitales
#define ADC_12ANA     0b1111000000000000 		// Canales analógicos: AN0-AN11 y los canales restantes se configuran digitales
#define ADC_13ANA     0b1110000000000000 		// Canales analógicos: AN0-AN12 y los canales restantes se configuran digitales
#define ADC_14ANA     0b1100000000000000	 	// Canales analógicos: AN0-AN13 y los canales restantes se configuran digitales
#define ADC_15ANA     0b1000000000000000 		// Canales analógicos: AN0-AN14 y los canales restantes se configuran digitales
#define ADC_16ANA     0b0000000000000000 		// Todos los canales se configuran analógicos

/**********************************************************************************************************************************/
#elif defined (ADC_V8) || defined (ADC_V10) 

//*************** Control de configuración de puertos analógicos *******************************
#define ADC_0ANA      0b0000000000000000 		// Todos los canales se configuran digitales
#define ADC_1ANA      0b0000000000000001	 	// Canales analógicos: AN0 y los canales restantes se configuran digitales
#define ADC_2ANA      0b0000000000000011 		// Canales analógicos: AN0-AN1 y los canales restantes se configuran digitales
#define ADC_3ANA      0b0000000000000111 		// Canales analógicos: AN0-AN2 y los canales restantes se configuran digitales
#define ADC_4ANA      0b0000000000001111 		// Canales analógicos: AN0-AN3 y los canales restantes se configuran digitales
#define ADC_5ANA      0b0000000000011111		// Canales analógicos: AN0-AN4 y los canales restantes se configuran digitales
#define ADC_6ANA      0b0000000000111111	 	// Canales analógicos: AN0-AN5 y los canales restantes se configuran digitales
#define ADC_7ANA      0b0000000001111111 		// Canales analógicos: AN0-AN6 y los canales restantes se configuran digitales
#define ADC_8ANA      0b0000000011111111 		// Canales analógicos: AN0-AN7 y los canales restantes se configuran digitales
#define ADC_9ANA      0b0000000111111111 		// Canales analógicos: AN0-AN8 y los canales restantes se configuran digitales
#define ADC_10ANA     0b0000001111111111 		// Canales analógicos: AN0-An9 y los canales restantes se configuran digitales
#define ADC_11ANA     0b0000011111111111	 	// Canales analógicos: AN0-AN10 y los canales restantes se configuran digitales
#define ADC_12ANA     0b0000111111111111 		// Canales analógicos: AN0-AN11 y los canales restantes se configuran digitales
#define ADC_13ANA     0b0001111111111111 		// Todos los canales se configuran analógicos

/**********************************************************************************************************************************/
#elif defined (ADC_V11) || defined (ADC_V11_1) 

//*************** Control de configuración de puertos analógicos *******************************
#define ADC_0ANA      0b0001111111111111 		// Todos los canales se configuran digitales
#define ADC_1ANA      0b0001111111111110	 	// Canales analógicos: AN0 y los canales restantes se configuran digitales
#define ADC_2ANA      0b0001111111111100 		// Canales analógicos: AN0-AN1 y los canales restantes se configuran digitales
#define ADC_3ANA      0b0001111111111000 		// Canales analógicos: AN0-AN2 y los canales restantes se configuran digitales
#define ADC_4ANA      0b0001111111110000 		// Canales analógicos: AN0-AN3 y los canales restantes se configuran digitales
#define ADC_5ANA      0b0001111111100000		// Canales analógicos: AN0-AN4 y los canales restantes se configuran digitales
#define ADC_6ANA      0b0001111111000000	 	// Canales analógicos: AN0-AN5 y los canales restantes se configuran digitales
#define ADC_7ANA      0b0001111110000000 		// Canales analógicos: AN0-AN6 y los canales restantes se configuran digitales
#define ADC_8ANA      0b0001111100000000 		// Canales analógicos: AN0-AN7 y los canales restantes se configuran digitales
#define ADC_9ANA      0b0001111000000000 		// Canales analógicos: AN0-AN8 y los canales restantes se configuran digitales
#define ADC_10ANA     0b0001110000000000 		// Canales analógicos: AN0-An9 y los canales restantes se configuran digitales
#define ADC_11ANA     0b0001100000000000	 	// Canales analógicos: AN0-AN10 y los canales restantes se configuran digitales
#define ADC_12ANA     0b0001000000000000 		// Canales analógicos: AN0-AN11 y los canales restantes se configuran digitales
#define ADC_13ANA     0b0000000000000000 		// Todos los canales se configuran analógicos
/**********************************************************************************************************************************/
#if defined (ADC_V11) || defined (ADC_V11_1) 
//************** Band Gap selection *******************************************
#define ADC_VBG_ON    0b1000000000000000      	//VBG output of Band Gap module is enabled
#define ADC_VBG_OFF   0b0000000000000000     	//VBG output of Band Gap module is disabled 
#endif

#endif

/**********************************************************************************************************************************/
#if defined (ADC_V7) || defined (ADC_V7_1)
//************** A/D conversion type *******************************************
#define	ADC_CONV_CONTINUOUS			0b00001000	//Auto-Conversion Continuous Loop Enabled
#define	ADC_CONV_SINGLE_SHOT		0b00000000	//Single shot mode enabled

//************* A/D conversion mode *******************************************
#define	ADC_MODE_MULTI_CH			0b00000100	//Multi-Channel mode enabled
#define	ADC_MODE_SINGLE_CH			0b00000000	//Single-Channel mode enabled

//************** A/D conversion mode sequence select *****************************
#define	ADC_CONV_SEQ_SEQM1			0b00000000	//Sequential Mode 1 (SEQM1); two muestras are taken in sequence
#define	ADC_CONV_SEQ_SEQM2			0b00000001	//Sequential Mode 2 (SEQM2); four muestras are taken in sequence
#define	ADC_CONV_SEQ_STNM1			0b00000010	//Simultaneous Mode 1 (STNM1); two muestras are taken simultaneously
#define	ADC_CONV_SEQ_STNM2			0b00000011	//Simultaneous Mode 2 (STNM2); two muestras are taken simultaneously
#define	ADC_CONV_SEQ_SCM1			0b00000000	//Single Channel Mode 1 (SCM1); Group A is taken and converted
#define	ADC_CONV_SEQ_SCM2			0b00000001	//Single Channel Mode 2 (SCM2); Group B is taken and converted
#define	ADC_CONV_SEQ_SCM3			0b00000010	//Single Channel Mode 3 (SCM3); Group C is taken and converted
#define	ADC_CONV_SEQ_SCM4			0b00000011	//Single Channel Mode 4 (SCM4); Group D is taken and converted

//************** A/D Vref selection *********************************************
#define	ADC_REF_VDD_VREFMINUS		0b10000000	//  VREF+ = VDD and VREF- = ext.source at VREF-
#define	ADC_REF_VREFPLUS_VSS		0b01000000	//  VREF+ = ext.source at VREF+ and VREF- = VSS
#define	ADC_REF_VREFPLUS_VREFMINUS	0b11000000	//  VREF+ = ext.source at VREF+ and VREF- = ext.source at VREF-
#define	ADC_REF_VDD_VSS				0b00000000	//  VREF+ = VDD and VREF- = VSS

//************** A/D FIFO buffer control ****************************************
#define	ADC_FIFO_EN					0b00100000	//FIFO Buffer Enable
#define	ADC_FIFO_DIS				0b00000000	//FIFO Buffer Deshabilita

//************** A/D Buffer depth interrupt control  *******************************
#define	INT_EACH_WR_BUF				0b00000000	//Interrupt is generated when each word is written to the buffer
#define	INT_2_4_WR_BUF				0b01000000	//Interrupt is generated when the 2nd and 4th words are written to the buffer
#define	INT_4_WR_BUF				0b10000000	//Interrupt is generated when the 4th word is written to the buffer

//************** A/D trigger source *********************************************
#define	ADC_TRIG_EXT_INT0			0b00000001	//External interrupt RC3/INT0 starts A/D sequence
#define	ADC_TRIG_TMR_5				0b00000010	//Timer5 starts A/D sequence
#define	ADC_TRIG_INP_CAP			0b00000100	//Input Capture 1 (IC1) starts A/D sequence
#define	ADC_TRIG_CCP2_COM			0b00001000	//CCP2 compare match starts A/D sequence	
#define	ADC_TRIG_PCPWM				0b00010000	//Power Control PWM module rising edge starts A/D sequence

//************** A/D Acquisition Time Selection *******************************
#define	ADC_0_TAD					0b00000000	//Tiempo de adquisición Tacq= 0 TAD
#define	ADC_2_TAD					0b00001000	//Tiempo de adquisición Tacq= 2 TAD
#define	ADC_4_TAD					0b00010000	//Tiempo de adquisición Tacq= 4 TAD
#define	ADC_6_TAD					0b00011000	//Tiempo de adquisición Tacq= 6 TAD
#define	ADC_8_TAD					0b00100000	//Tiempo de adquisición Tacq= 8 TAD
#define	ADC_10_TAD					0b00101000	//Tiempo de adquisición Tacq= 10 TAD
#define	ADC_12_TAD					0b00110000	//Tiempo de adquisición Tacq= 12 TAD
#define	ADC_16_TAD					0b00111000	//Tiempo de adquisición Tacq= 16 TAD
#define	ADC_20_TAD					0b01000000	//Tiempo de adquisición Tacq= 20 TAD
#define	ADC_24_TAD					0b01001000	//Tiempo de adquisición Tacq= 24 TAD
#define	ADC_28_TAD					0b01010000	//Tiempo de adquisición Tacq= 28 TAD
#define	ADC_32_TAD					0b01011000	//Tiempo de adquisición Tacq= 32 TAD
#define	ADC_36_TAD					0b01100000	//Tiempo de adquisición Tacq= 36 TAD
#define	ADC_40_TAD					0b01101000	//Tiempo de adquisición Tacq= 40 TAD
#define	ADC_48_TAD					0b01110000	//Tiempo de adquisición Tacq= 48 TAD
#define	ADC_64_TAD					0b01111000	//Tiempo de adquisición Tacq= 64 TAD


//*************** A/D Conversion Clock Selection *****************************
#define ADC_FOSC_2      		 	0b00000000 	//Reloj de conversión ADC =Fosc/2
#define ADC_FOSC_4      			0b00000100 	//Reloj de conversión ADC =Fosc/4
#define ADC_FRC_4        			0b00000011 	//Reloj de conversión ADC =FRC/4
#define ADC_FOSC_8       			0b00000001 	//Reloj de conversión ADC =Fosc/8
#define ADC_FOSC_16      			0b00000101 	//Reloj de conversión ADC =Fosc/16
#define ADC_FOSC_32      			0b00000010 	//Reloj de conversión ADC =Fosc/32
#define ADC_FOSC_64      			0b00000110 	//Reloj de conversión ADC =Fosc/64
#define ADC_FOSC_RC      			0b00000111 	//Reloj de conversión ADC =Internal RC OSC

#endif
/**********************************************************************************************************************************/
#if	defined (ADC_V1) || defined (ADC_V2) || defined (ADC_V3) || defined (ADC_V4) ||\
    defined (ADC_V5) || defined (ADC_V6) || defined (ADC_V8) || defined (ADC_V9) || defined (ADC_V9_1) ||\
	defined (ADC_V10) || defined (ADC_V11) || defined (ADC_V12) || defined (ADC_V11_1) 
//****************Selección de canales******************************************
#define ADC_CH0          0b00000000  			//Selecciona canal analógico #0
#define ADC_CH1          0b00001000  			//Selecciona canal analógico #1
#define ADC_CH2          0b00010000  			//Selecciona canal analógico #2
#define ADC_CH3          0b00011000  			//Selecciona canal analógico #3
#define ADC_CH4          0b00100000  			//Selecciona canal analógico #4
#define ADC_CH5          0b00101000  			//Selecciona canal analógico #5
#define ADC_CH6          0b00110000  			//Selecciona canal analógico #6
#define ADC_CH7          0b00111000  			//Selecciona canal analógico #7
#define ADC_CH8          0b01000000  			//Selecciona canal analógico #8
#define ADC_CH9          0b01001000  			//Selecciona canal analógico #9
#define ADC_CH10         0b01010000  			//Selecciona canal analógico #10
#define ADC_CH11         0b01011000  			//Selecciona canal analógico # 11
#define ADC_CH12         0b01100000  			//Selecciona canal analógico #12
#if defined (ADC_V11) || defined (ADC_V11_1) 
#define ADC_CH_CTMU      0b01101000  			// All analog inputs are off - CTMU
#define ADC_CH_VDDCORE   0b01110000  			// VDDCORESelect Channel
#define ADC_CH_VBG       0b01111000  			// Voltage Band gapSelect Channel
#elif defined (ADC_V10)
//#define ADC_CH_MASK		 (~FVR1)			//Mask ADCSelección de canalesbits
#else
#define ADC_CH13         0b01101000  			//Selecciona canal analógico #13
#define ADC_CH14         0b01110000  			//Selecciona canal analógico #14
#define ADC_CH15         0b01111000  			//Selecciona canal analógico #15
#endif

#endif

/**********************************************************************************************************************************/
#if defined (ADC_V10)
//****************Selección de canales******************************************
#define ADC_CH3          0b00011000  		//Selecciona canal analógico #3
#define ADC_CH4          0b00100000  		//Selecciona canal analógico #4
#define ADC_CH5          0b00101000  		//Selecciona canal analógico #5
#define ADC_CH6          0b00110000  		//Selecciona canal analógico #6
#define ADC_CH7          0b00111000  		//Selecciona canal analógico #7
#define ADC_CH8          0b01000000  		//Selecciona canal analógico #8
#define ADC_CH9          0b01001000  		//Selecciona canal analógico #9
#define ADC_CH10         0b01010000  		//Selecciona canal analógico #10
#define ADC_CH11         0b01011000  		//Selecciona canal analógico # 11
#define DAC1			 0b01110000  		// Convertidor digital-analógico (DAC)
#define FVR1			 0b01111000  		// Referencia de vonltaje fija (Fixed Voltage Reference)


//************** Configuración de referencia positiva de voltaje *************************
#define ADC_REF_VDD_VDD			0b00000000 	//  VREF+ = AVDD
#define ADC_REF_VDD_VREFPLUS	0b00000100 	//  VREF+ = referencia externa conectada a VREF+
#define ADC_REF_VDD_FVREF		0b00001000 	//  VREF+ = FVREF+

//************** Configuración de referencia negativa de voltaje *************************
#define ADC_REF_VDD_VSS		    0b00000000 	//  VREF- = AVSS
#define ADC_REF_VDD_VREFMINUS	0b00000001 	//  VREF- = referencia externa conectada a VREF-
#endif

#if defined (ADC_V12) 
#define ADC_TRIG_CTMU           0b10000000 	//Special trigger from the CTMU
#define ADC_TRIG_CCP2           0b00000000 	//Special trigger from CCP2
#endif
/**********************************************************************************************************************************/
#if defined (ADC_V13) || defined (ADC_V13_1) || defined (ADC_V13_2)\
	 || defined (ADC_V13_3)

//****************Selección de canales******************************************
#define ADC_CH0          0b00000000  			//Selecciona canal analógico #0
#define ADC_CH1          0b00000100  			//Selecciona canal analógico #1
#define ADC_CH2          0b00001000  			//Selecciona canal analógico #2
#define ADC_CH3          0b00001100  			//Selecciona canal analógico #3
#define ADC_CH4          0b00010000  			//Selecciona canal analógico #4
#if defined (ADC_V13) || defined (ADC_V13_1)  || defined (ADC_V13_3)
#define ADC_CH5          0b00010100  			//Selecciona canal analógico #5
#define ADC_CH6          0b00011000  			//Selecciona canal analógico #6
#define ADC_CH7          0b00011100  			//Selecciona canal analógico #7
#endif
#define ADC_CH8          0b00100000  			//Selecciona canal analógico #8
#define ADC_CH9          0b00100100  			//Selecciona canal analógico #9
#define ADC_CH10         0b00101000  			//Selecciona canal analógico #10
#if defined (ADC_V13) || defined (ADC_V13_1) 
#define ADC_CH11         0b00101100  			//Selecciona canal analógico # 11
#endif
#if defined (ADC_V13) || defined (ADC_V13_1) 
#if defined (ADC_V13)
#define ADC_CH12         0b00110000  			//Selecciona canal analógico #12
#define ADC_CH13         0b00110100  			//Selecciona canal analógico #13
#define ADC_CH14         0b00111000  			//Selecciona canal analógico #14
#define ADC_CH15         0b00111100  			//Selecciona canal analógico #15
#endif
#define ADC_CH16         0b01000000  			//Selecciona canal analógico #16
#define ADC_CH17         0b01000100  			//Selecciona canal analógico # 17
#define ADC_CH18         0b01001000  			//Selecciona canal analógico #18
#define ADC_CH19         0b01001100  			//Selecciona canal analógico #19
#if defined (ADC_V13)
#define ADC_CH20         0b01010000  			//Selecciona canal analógico #20
#define ADC_CH21         0b01010100  			//Selecciona canal analógico #21
#define ADC_CH22         0b01011000  			//Selecciona canal analógico #22
#define ADC_CH23         0b01011100  			//Selecciona canal analógico #23
#define ADC_CH28         0b01110000  			//Selecciona canal analógico #28 para CTMU
#define ADC_CH29         0b01110100  			//Selecciona canal analógico #29 para sensor de temperatura
#endif
#define ADC_CH30         0b01111000  			//Selecciona canal analógico #30
#define ADC_CH31         0b01111100  			//Selecciona canal analógico #31
#endif

//**************** Analog Negetive Channel Select *********************************
#define ADC_NEG_CH0          0b00000000  			//Selecciona al canal analógico negativo #0
#define ADC_NEG_CH1          0b00000001  			//Selecciona al canal analógico negativo #1
#define ADC_NEG_CH2          0b00000010  			//Selecciona al canal analógico negativo #2
#define ADC_NEG_CH3          0b00000011  			//Selecciona al canal analógico negativo #3
#define ADC_NEG_CH4          0b00000100  			//Selecciona al canal analógico negativo #4
#define ADC_NEG_CH5          0b00000101  			//Selecciona al canal analógico negativo #5
#define ADC_NEG_CH6          0b00000110  			//Selecciona al canal analógico negativo #6
#if defined (ADC_V13) || defined (ADC_V13_1)
#define ADC_NEG_CH7          0b00000111  			//Selecciona al canal analógico negativo #7
#endif

//************** Configuración de referencia de voltaje positiva *************************
#define ADC_REF_VDD_VDD			0b00000000 	//Referencia positiva para módulo ADC VREF+ = AVDD
#define ADC_REF_VDD_VREFPLUS	0b00010000 	//Referencia positiva para módulo ADC VREF+ = referencia externa conectada a VREF+
#define ADC_REF_VDD_INT_VREF_2	0b00100000 	//Referencia positiva para módulo ADC VREF+ = Referencia interna de 2.048V
#define ADC_REF_VDD_INT_VREF_4	0b00110000 	//Referencia positiva para módulo ADC VREF+ = Referencia interna de 4.096V
//************** Configuración de referencia de voltaje negativa *************************
#define ADC_REF_VDD_VSS		    0b00000000 	// ADC voltage negetive source VREF- = AVSS
#define ADC_REF_VDD_VREFMINUS	0b00001000 	// ADC voltage negetive source VREF- = referencia externa conectada a VREF-

/*Definición de registros de configuración de puertos ADC*/
#define ENABLE_AN0_DIG()              ANCON0bits.ANSEL0=0 /*Habilita AN0 en modo digital */
#define ENABLE_AN0_ANA()              ANCON0bits.ANSEL0=1 /*Habilita AN0 en modo analógico */
#define ENABLE_AN1_DIG()              ANCON0bits.ANSEL1=0 /*Habilita AN1 en modo digital */
#define ENABLE_AN1_ANA()              ANCON0bits.ANSEL1=1 /*Habilita AN1 en modo analógico */
#define ENABLE_AN2_DIG()              ANCON0bits.ANSEL2=0 /*Habilita AN2 en modo digital */
#define ENABLE_AN2_ANA()              ANCON0bits.ANSEL2=1 /*Habilita AN2 en modo analógico */
#define ENABLE_AN3_DIG()              ANCON0bits.ANSEL3=0 /*Habilita AN3 en modo digital */
#define ENABLE_AN3_ANA()              ANCON0bits.ANSEL3=1 /*Habilita AN3 en modo analógico */
#define ENABLE_AN4_DIG()              ANCON0bits.ANSEL4=0 /*Habilita AN4 en modo digital */
#define ENABLE_AN4_ANA()              ANCON0bits.ANSEL4=1 /*Habilita AN4 en modo analógico */
#if defined (ADC_V13) || defined (ADC_V13_1)  || defined (ADC_V13_3)
#define ENABLE_AN5_DIG()              ANCON0bits.ANSEL5=0 /*Habilita AN5 en modo digital */
#define ENABLE_AN5_ANA()              ANCON0bits.ANSEL5=1 /*Habilita AN5 en modo analógico */
#define ENABLE_AN6_DIG()              ANCON0bits.ANSEL6=0 /*Habilita AN6 en modo digital */
#define ENABLE_AN6_ANA()              ANCON0bits.ANSEL6=1 /*Habilita AN6 en modo analógico */
#define ENABLE_AN7_DIG()              ANCON0bits.ANSEL7=0 /*Habilita AN7 en modo digital */
#define ENABLE_AN7_ANA()              ANCON0bits.ANSEL7=1 /*Habilita AN7 en modo analógico */
#define ENABLE_ALL_ANA_0_7()          ANCON0=0xFF /*Habilita AN0-AN7 en modo analógico */
#define ENABLE_ALL_DIG_0_7()          ANCON0=0x00 /*Habilita AN0-AN7 en modo digital */
#endif
#define ENABLE_AN8_DIG()              ANCON1bits.ANSEL8=0 /*Habilita AN8 en modo digital */
#define ENABLE_AN8_ANA()              ANCON1bits.ANSEL8=1 /*Habilita AN8 en modo analógico */
#if defined (ADC_V13) || defined (ADC_V13_1)  || defined (ADC_V13_3)
#define ENABLE_AN9_DIG()              ANCON1bits.ANSEL9=0 /*Habilita AN9 en modo digital */
#define ENABLE_AN9_ANA()              ANCON1bits.ANSEL9=1 /*Habilita AN9 en modo analógico */
#define ENABLE_AN10_DIG()             ANCON1bits.ANSEL10=0 /*Habilita AN10 en modo digital */
#define ENABLE_AN10_ANA()             ANCON1bits.ANSEL10=1 /*Habilita AN10 en modo analógico */
#endif
#if defined (ADC_V13) || defined (ADC_V13_1) 
#define ENABLE_AN11_DIG()             ANCON1bits.ANSEL11=0 /*Habilita AN11 en modo digital */
#define ENABLE_AN11_ANA()             ANCON1bits.ANSEL11=1 /*Habilita AN11 en modo analógico */

#if defined (ADC_V13)
#define ENABLE_AN12_DIG()             ANCON1bits.ANSEL12=0 /*Habilita AN12 en modo digital */
#define ENABLE_AN12_ANA()             ANCON1bits.ANSEL12=1 /*Habilita AN12 en modo analógico */
#define ENABLE_AN13_DIG()             ANCON1bits.ANSEL13=0 /*Habilita AN13 en modo digital */
#define ENABLE_AN13_ANA()             ANCON1bits.ANSEL13=1 /*Habilita AN13 en modo analógico */
#define ENABLE_AN14_DIG()             ANCON1bits.ANSEL14=0 /*Habilita AN14 en modo digital */
#define ENABLE_AN14_ANA()             ANCON1bits.ANSEL14=1 /*Habilita AN14 en modo analógico */
#define ENABLE_AN15_DIG()             ANCON1bits.ANSEL15=0 /*Habilita AN15 en modo digital */
#define ENABLE_AN15_ANA()             ANCON1bits.ANSEL15=1 /*Habilita AN15 en modo analógico */
#endif
#define ENABLE_ALL_ANA_8_15()         ANCON1=0xFF /*Habilita AN8-AN15 en modo analógico */
#define ENABLE_ALL_DIG_8_15()         ANCON1=0x00 /*Habilita AN8-AN15 en modo digital */

#define ENABLE_AN16_DIG()             ANCON2bits.ANSEL16=0 /*Habilita AN16 en modo digital */
#define ENABLE_AN16_ANA()             ANCON2bits.ANSEL16=1 /*Habilita AN16 en modo analógico */
#define ENABLE_AN17_DIG()             ANCON2bits.ANSEL17=0 /*Habilita AN17 en modo digital */
#define ENABLE_AN17_ANA()             ANCON2bits.ANSEL17=1 /*Habilita AN17 en modo analógico */
#define ENABLE_AN18_DIG()             ANCON2bits.ANSEL18=0 /*Habilita AN18 en modo digital */
#define ENABLE_AN18_ANA()             ANCON2bits.ANSEL18=1 /*Habilita AN18 en modo analógico */
#define ENABLE_AN19_DIG()             ANCON2bits.ANSEL19=0 /*Habilita AN19 en modo digital */
#define ENABLE_AN19_ANA()             ANCON2bits.ANSEL19=1 /*Habilita AN19 en modo analógico */
#if defined (ADC_V13)
#define ENABLE_AN20_DIG()             ANCON2bits.ANSEL20=0 /*Habilita AN20 en modo digital */
#define ENABLE_AN20_ANA()             ANCON2bits.ANSEL20=1 /*Habilita AN20 en modo analógico */
#define ENABLE_AN21_DIG()             ANCON2bits.ANSEL21=0 /*Habilita AN21 en modo digital */
#define ENABLE_AN21_ANA()             ANCON2bits.ANSEL21=1 /*Habilita AN21 en modo analógico */
#define ENABLE_AN22_DIG()             ANCON2bits.ANSEL22=0 /*Habilita AN22 en modo digital */
#define ENABLE_AN22_ANA()             ANCON2bits.ANSEL22=1 /*Habilita AN22 en modo analógico */
#define ENABLE_AN23_DIG()             ANCON2bits.ANSEL23=0 /*Habilita AN23 en modo digital */
#define ENABLE_AN23_ANA()             ANCON2bits.ANSEL23=1 /*Habilita AN23 en modo analógico */
#endif
#define ENABLE_ALL_ANA_16_23()        ANCON2=0xFF /*Habilita AN16-AN23 en modo analógico */
#define ENABLE_ALL_DIG_16_23()        ANCON2=0x00 /*Habilita AN16-AN23 en modo digital */
#endif

#endif  // End of version ADC_V13 & ADC_V13_1

/**********************************************************************************************************************************/

#if defined (ADC_V14) || defined (ADC_V14_1) || defined (ADC_V14_2) || defined (ADC_V14_3)

//*************** A/D Result Format Select ***********************************
#define ADC_RIGHT_JUST   0b10000000 			//Resultado de conversión justificado a la derecha
#define ADC_LEFT_JUST    0b00000000 			//Resultado de conversión justificado a la izquierda


//************** Configuración de referencia positiva de voltaje *************************
#define ADC_REF_VDD_VDD			0b00000000 	//Referencia positiva para módulo ADC VREF+ = AVDD
#define ADC_REF_VDD_VREFPLUS	0b00000100 	//Referencia positiva para módulo ADC VREF+ = referencia externa conectada a VREF+
#define ADC_REF_FVR_BUF			0b00001000 	//Referencia positiva para módulo ADC VREF+ = FVR BUF
//************** Negetive Voltage Reference Configuration *************************
#define ADC_REF_VDD_VSS		    0b00000000 	// ADC voltage negetive source VREF- = AVSS
#define ADC_REF_VDD_VREFMINUS	0b00001001 	// ADC voltage negetive source VREF- = referencia externa conectada a VREF-


//**************** Special Trigger Select bit *********************************
#define ADC_TRIG_CTMU           0b10000000 	//Special trigger from the CTMU
#if defined (ADC_V14) || defined (ADC_V14_1)
#define ADC_TRIG_CCP5           0b00000000 	//Special trigger from CCP5
#endif
#if defined (ADC_V14_2) || defined (ADC_V14_3)
#define ADC_TRIG_CCP2           0b00000000 	//Special trigger from CCP2 
#endif


#define ADC_CH0          0b00000000  			//Selecciona canal analógico #0
#define ADC_CH1          0b00000100  			//Selecciona canal analógico #1
#define ADC_CH2          0b00001000  			//Selecciona canal analógico #2
#define ADC_CH3          0b00001100  			//Selecciona canal analógico #3
#define ADC_CH4          0b00010000  			//Selecciona canal analógico #4
#if defined (ADC_V14) || defined (ADC_V14_2)
#define ADC_CH5          0b00010100  			//Selecciona canal analógico #5
#define ADC_CH6          0b00011000  			//Selecciona canal analógico #6
#define ADC_CH7          0b00011100  			//Selecciona canal analógico #7
#endif
#define ADC_CH8          0b00100000  			//Selecciona canal analógico #8
#define ADC_CH9          0b00100100  			//Selecciona canal analógico #9
#define ADC_CH10         0b00101000  			//Selecciona canal analógico #10
#define ADC_CH11         0b00101100  			//Selecciona canal analógico # 11
#define ADC_CH12         0b00110000  			//Selecciona canal analógico #12
#define ADC_CH13         0b00110100  			//Selecciona canal analógico #13
#define ADC_CH14         0b00111000  			//Selecciona canal analógico #14
#if defined (ADC_V14) || defined (ADC_V14_1)
#define ADC_CH15         0b00111100  			//Selecciona canal analógico #15
#define ADC_CH16         0b01000000  			//Selecciona canal analógico #16
#define ADC_CH17         0b01000100  			//Selecciona canal analógico #17
#endif
#if defined (ADC_V14) || defined (ADC_V14_1) || defined (ADC_V14_2) || defined (ADC_V14_3)
#define ADC_CH18         0b01001000  			//Selecciona canal analógico #18
#define ADC_CH19         0b01001100  			//Selecciona canal analógico #19
#endif
#if defined (ADC_V14) || defined (ADC_V14_1) || defined (ADC_V14_2)
#define ADC_CH20         0b01010000  			//Selecciona canal analógico #20
#endif
#if defined (ADC_V14) || defined (ADC_V14_2)
#define ADC_CH21         0b01010100  			//Selecciona canal analógico #21
#define ADC_CH22         0b01011000  			//Selecciona canal analógico #22
#define ADC_CH23         0b01011100  			//Selecciona canal analógico #23
#define ADC_CH24         0b01100000  			//Selecciona canal analógico #24
#define ADC_CH25         0b01100100  			//Selecciona canal analógico #25
#define ADC_CH26         0b01101000  			//Selecciona canal analógico #26
#define ADC_CH27         0b01101100  			//Selecciona canal analógico #27
#endif
#if defined (ADC_V14_2) || defined (ADC_V14_3)
#define ADC_CH_TEMP_DI   0b01110000  			//Selecciona canal analógico #28 para diodo de temperatura
#endif
#define ADC_CH_CTMU      0b01110100  			//Selecciona canal analógico #29 for CTMU
#define ADC_CH_DAC       0b01111000  			//Selecciona canal analógico #30 for DAC
#define ADC_CH_FRV       0b01111100  			//Selecciona canal analógico #31 for FRV

#endif

/**********************************************************************************************************************************/

#if	defined (ADC_V15) || defined (ADC_V15_1)

/***** ADCON1H register definition ****/
#define ADC_12BIT_MODE              0b00100000 /*12-bit A/D operation*/
#define ADC_10BIT_MODE              0b0000000 /*10-bit A/D operation*/

#define ADC_FORMAT_SIGN_FRACT       0b00000011 /* A/D data format signed fractional */
#define ADC_FORMAT_FRACT            0b00000010 /* A/D data format fractional */
#define ADC_FORMAT_SIGN_INT         0b00000001 /* A/D data format signed integer */
#define ADC_FORMAT_INTG             0b00000000 /* A/D data format integer */

/***** ADCON1L register definition ****/
#define ADC_TRIG_AUTO              	0b01110000 /*The SAMP bit is cleared after SAMC<4:0> number of TAD clocks following 
                                           the SAMP bit being set.No Extended Sample Time is present.*/										   
#define ADC_TRIG_CTMU                0b01000000 /* CTMU event ends sampling and starts conversion */
#define ADC_TRIG_TMR_5                0b00110000 /* Timer5 compare ends sampling and starts conversion */
#define ADC_TRIG_TMR_3                0b00100000 /* Timer3 compare ends sampling and starts conversion */
#define ADC_TRIG_INT0                0b00010000 /* Active transition on INT0 ends sampling and starts conversion */
#define ADC_TRIG_TIMER1                0b01010000 /* Timer1 compare ends sampling and starts conversion */
#define ADC_TRIG_CLEAR           	0b00000000 /*The SAMP bit must be cleared by software*/

#define ADC_AUTO_SAMPLING_ON        0b00000100 /* Sampling begins immediately after last conversion */
#define ADC_AUTO_SAMPLING_OFF       0b00000000 /* Sampling begins when SAMP bit is set */

#define ADC_SAMP_ON                 0b00000010 /* sample / hold amplifiers are sampling */
#define ADC_SAMP_OFF                0b00000000 /* sample / hold amplifiers are holding */

/***** ADCON2H control register definition ****/
#define ADC_REF_VDD_VREFPLUS       0b01000000 /*Converter Positive Voltage Reference - External VREF+*/
#define ADC_REF_VDD_VDD            0b00000000 /*Converter Positive Voltage Reference - AVDD*/

#define ADC_REF_VDD_VREFMINUS      0b00100000 /*Converter Negative Voltage Reference - External VREF- */
#define ADC_REF_VDD_VSS          0b00000000 /*Converter Negative Voltage Reference - AVSS */

#define ADC_BUF_REG_ENABLE          0b00001000 /* Conversion result is loaded into buffer location determined by the converted channel*/
#define ADC_BUF_REG_DISABLE         0b00000000 /* A/D result buffer is treated as a FIFO */

#define ADC_OFFSET_CAL_ON           0b00010000 /* Converts to get the Offset calibration value */
#define ADC_OFFSET_CAL_OFF          0b00000000 /* Coverts to get the actual input value */

#define ADC_SCAN_ON                 0b00000100 /* A/D Scan Input Selections for CH0 during SAMPLE A */
#define ADC_SCAN_OFF                0b00000000 /* A/D Do notScan Input Selections for CH0+ during SAMPLE A */

/***** ADCON2L control register definition ****/
#define ADC_INTR_EACH_CONV          0b00000000 /* Interrupción generada ante la conversión completa de  each sample */
#define ADC_INTR_2_CONV             0b00000100 /* Interrupción generada ante la conversión completa de  2 muestras */
#define ADC_INTR_3_CONV             0b00001000/* Interrupción generada ante la conversión completa de  3 muestras */
#define ADC_INTR_4_CONV             0b00001100 /* Interrupción generada ante la conversión completa de  4 muestras */
#define ADC_INTR_5_CONV             0b00010000 /* Interrupción generada ante la conversión completa de  5 muestras */
#define ADC_INTR_6_CONV             0b00010100 /* Interrupción generada ante la conversión completa de  6 muestras */
#define ADC_INTR_7_CONV             0b00011000 /* Interrupción generada ante la conversión completa de  7 muestras */
#define ADC_INTR_8_CONV             0b00011100 /* Interrupción generada ante la conversión completa de  8 muestras */
#define ADC_INTR_9_CONV             0b00100000 /* Interrupción generada ante la conversión completa de  9 muestras */
#define ADC_INTR_10_CONV            0b00100100 /* Interrupción generada ante la conversión completa de  10 muestras */
#define ADC_INTR_11_CONV            0b00101000 /* Interrupción generada ante la conversión completa de  11 muestras */
#define ADC_INTR_12_CONV            0b00101100 /* Interrupción generada ante la conversión completa de  12 muestras */
#define ADC_INTR_13_CONV            0b00110000 /* Interrupción generada ante la conversión completa de  13 muestras */
#define ADC_INTR_14_CONV            0b00110100 /* Interrupción generada ante la conversión completa de  14 muestras */
#define ADC_INTR_15_CONV            0b00111000 /* Interrupción generada ante la conversión completa de  15 muestras */
#define ADC_INTR_16_CONV            0b00111100 /* Interrupción generada ante la conversión completa de  16 muestras */
#define ADC_INTR_17_CONV            0b01000000 /* Interrupción generada ante la conversión completa de  17 muestras */
#define ADC_INTR_18_CONV            0b01000100 /* Interrupción generada ante la conversión completa de  18 muestras */
#define ADC_INTR_19_CONV            0b01001000 /* Interrupción generada ante la conversión completa de  19 muestras */
#define ADC_INTR_20_CONV            0b01001100 /* Interrupción generada ante la conversión completa de  20 muestras */
#define ADC_INTR_21_CONV            0b01010000 /* Interrupción generada ante la conversión completa de  21 muestras */
#define ADC_INTR_22_CONV            0b01010100 /* Interrupción generada ante la conversión completa de  22 muestras */
#define ADC_INTR_23_CONV            0b01011000 /* Interrupción generada ante la conversión completa de  23 muestras */
#define ADC_INTR_24_CONV            0b01011100 /* Interrupción generada ante la conversión completa de  24 muestras */
#define ADC_INTR_25_CONV            0b01100000 /* Interrupción generada ante la conversión completa de  25 muestras */
#define ADC_INTR_26_CONV            0b01100100 /* Interrupción generada ante la conversión completa de  26 muestras */
#define ADC_INTR_27_CONV            0b01101000 /* Interrupción generada ante la conversión completa de  27 muestras */
#define ADC_INTR_28_CONV            0b01101100 /* Interrupción generada ante la conversión completa de  28 muestras */
#define ADC_INTR_29_CONV            0b01110000 /* Interrupción generada ante la conversión completa de  29 muestras */
#define ADC_INTR_30_CONV            0b01110100 /* Interrupción generada ante la conversión completa de  30 muestras */
#define ADC_INTR_31_CONV            0b01111000 /* Interrupción generada ante la conversión completa de  31 muestras */
#define ADC_INTR_32_CONV            0b01111100 /* Interrupción generada ante la conversión completa de  32 muestras */

#define ADC_ALT_BUF_ON              0b00000010 /* Buffer configured as 2 8-word buffers */
#define ADC_ALT_BUF_OFF             0b00000000 /* Buffer configured as 1 16-word buffer */

#define ADC_ALT_INPUT_ON            0b00000001 /* alternate between MUXA and MUXB */
#define ADC_ALT_INPUT_OFF           0b00000000 /* use MUXA only */

/**** ADCON3H register definition ****/
#define ADC_CONV_CLK_INTERNAL_RC    0b10000000 /* A/D Conversion Clock Source internal RC Clock */
#define ADC_CONV_CLK_SYSTEM         0b00000000 /* A/D Conversion Clock Source Clock derived from system clock */

#define ADC_EXT_SAMP_ON             0b01000000 /*A/D is still sampling after SAMP = 0*/
#define ADC_EXT_SAMP_OFF            0b00000000 /*A/D is finished sampling*/

#define ADC_CHARGE_PUMP_ENABLE      0b00100000 /*Charge pump for switches is enabled*/
#define ADC_CHARGE_PUMP_DISABLE     0b00000000 /*Charge pump for switches is enabled*/

//************** A/D Acquisition Time Selection *******************************
#define ADC_0_TAD		            0b00000000 /* Tiempo de adquisición A/D automático Tacq =  0 Tad */
#define ADC_1_TAD		            0b00000001 /* Tiempo de adquisición A/D automático Tacq =  1 Tad */
#define ADC_2_TAD           		0b00000010 /* Tiempo de adquisición A/D automático Tacq =  2 Tad */
#define ADC_3_TAD           		0b00000011 /* Tiempo de adquisición A/D automático Tacq =  3 Tad */
#define ADC_4_TAD           		0b00000100 /* Tiempo de adquisición A/D automático Tacq =  4 Tad */
#define ADC_5_TAD           		0b00000101 /* Tiempo de adquisición A/D automático Tacq =  5 Tad */
#define ADC_6_TAD           		0b00000110 /* Tiempo de adquisición A/D automático Tacq =  6 Tad */
#define ADC_7_TAD           		0b00000111 /* Tiempo de adquisición A/D automático Tacq =  7 Tad */
#define ADC_8_TAD           		0b00001000 /* Tiempo de adquisición A/D automático Tacq =  8 Tad */
#define ADC_9_TAD           		0b00001001 /* Tiempo de adquisición A/D automático Tacq =  9 Tad */
#define ADC_10_TAD           		0b00001010 /* Tiempo de adquisición A/D automático Tacq =  10 Tad */
#define ADC_11_TAD           		0b00001011 /* Tiempo de adquisición A/D automático Tacq =  11 Tad */
#define ADC_12_TAD           		0b00001100 /* Tiempo de adquisición A/D automático Tacq =  12 Tad */
#define ADC_13_TAD           		0b00001101 /* Tiempo de adquisición A/D automático Tacq =  13 Tad */
#define ADC_14_TAD           		0b00001110 /* Tiempo de adquisición A/D automático Tacq =  14 Tad */
#define ADC_15_TAD           		0b00001111 /* Tiempo de adquisición A/D automático Tacq =  15 Tad */
#define ADC_16_TAD           		0b00010000 /* Tiempo de adquisición A/D automático Tacq =  16 Tad */
#define ADC_17_TAD           		0b00010001 /* Tiempo de adquisición A/D automático Tacq =  17 Tad */
#define ADC_18_TAD           		0b00010010 /* Tiempo de adquisición A/D automático Tacq =  18 Tad */
#define ADC_19_TAD           		0b00010011 /* Tiempo de adquisición A/D automático Tacq =  19 Tad */
#define ADC_20_TAD           		0b00010100 /* Tiempo de adquisición A/D automático Tacq =  20 Tad */
#define ADC_21_TAD           		0b00010101 /* Tiempo de adquisición A/D automático Tacq =  21 Tad */
#define ADC_22_TAD           		0b00010110 /* Tiempo de adquisición A/D automático Tacq =  22 Tad */
#define ADC_23_TAD           		0b00010111 /* Tiempo de adquisición A/D automático Tacq =  23 Tad */
#define ADC_24_TAD           		0b00011000 /* Tiempo de adquisición A/D automático Tacq =  24 Tad */
#define ADC_25_TAD           		0b00011001 /* Tiempo de adquisición A/D automático Tacq =  25 Tad */
#define ADC_26_TAD           		0b00011010 /* Tiempo de adquisición A/D automático Tacq =  26 Tad */
#define ADC_27_TAD           		0b00011011 /* Tiempo de adquisición A/D automático Tacq =  27 Tad */
#define ADC_28_TAD           		0b00011100 /* Tiempo de adquisición A/D automático Tacq =  28 Tad */
#define ADC_29_TAD           		0b00011101 /* Tiempo de adquisición A/D automático Tacq =  29 Tad */
#define ADC_30_TAD           		0b00011110 /* Tiempo de adquisición A/D automático Tacq =  30 Tad */
#define ADC_31_TAD           		0b00011111 /* Tiempo de adquisición A/D automático Tacq =  31 Tad */

/**** ADCON3L register definition ****/
#define ADC_CONV_CLK_1Tcy          0b00000000 /* Reloj de conversión ADC Tadc= Tcy */
#define ADC_CONV_CLK_2Tcy          0b00000001 /* Reloj de conversión ADC Tadc= 2*Tcy */
#define ADC_CONV_CLK_3Tcy          0b00000010 /* Reloj de conversión ADC Tadc= 3*Tcy */
#define ADC_CONV_CLK_4Tcy          0b00000011 /* Reloj de conversión ADC Tadc= 4*Tcy */
#define ADC_CONV_CLK_5Tcy          0b00000100 /* Reloj de conversión ADC Tadc= 5*Tcy */
#define ADC_CONV_CLK_6Tcy          0b00000101 /* Reloj de conversión ADC Tadc= 6*Tcy */
#define ADC_CONV_CLK_7Tcy          0b00000110 /* Reloj de conversión ADC Tadc= 7*Tcy */
#define ADC_CONV_CLK_8Tcy          0b00000111 /* Reloj de conversión ADC Tadc= 8*Tcy */
#define ADC_CONV_CLK_9Tcy          0b00001000 /* Reloj de conversión ADC Tadc= 9*Tcy */
#define ADC_CONV_CLK_10Tcy         0b00001001 /* Reloj de conversión ADC Tadc= 10*Tcy */
#define ADC_CONV_CLK_11Tcy         0b00001010 /* Reloj de conversión ADC Tadc= 11*Tcy */
#define ADC_CONV_CLK_12Tcy         0b00001011 /* Reloj de conversión ADC Tadc= 12*Tcy */
#define ADC_CONV_CLK_13Tcy         0b00001100 /* Reloj de conversión ADC Tadc= 13*Tcy */
#define ADC_CONV_CLK_14Tcy         0b00001101 /* Reloj de conversión ADC Tadc= 14*Tcy */
#define ADC_CONV_CLK_15Tcy         0b00001110 /* Reloj de conversión ADC Tadc= 15*Tcy */
#define ADC_CONV_CLK_16Tcy         0b00001111 /* Reloj de conversión ADC Tadc= 16*Tcy */
#define ADC_CONV_CLK_17Tcy         0b00010000 /* Reloj de conversión ADC Tadc= 17*Tcy */
#define ADC_CONV_CLK_18Tcy         0b00010001 /* Reloj de conversión ADC Tadc= 18*Tcy */
#define ADC_CONV_CLK_19Tcy         0b00010010 /* Reloj de conversión ADC Tadc= 19*Tcy */
#define ADC_CONV_CLK_20Tcy         0b00010011 /* Reloj de conversión ADC Tadc= 20*Tcy */
#define ADC_CONV_CLK_21Tcy         0b00010100 /* Reloj de conversión ADC Tadc= 21*Tcy */
#define ADC_CONV_CLK_22Tcy         0b00010101 /* Reloj de conversión ADC Tadc= 22*Tcy */
#define ADC_CONV_CLK_23Tcy         0b00010110 /* Reloj de conversión ADC Tadc= 23*Tcy */
#define ADC_CONV_CLK_24Tcy         0b00010111 /* Reloj de conversión ADC Tadc= 24*Tcy */
#define ADC_CONV_CLK_25Tcy         0b00011000 /* Reloj de conversión ADC Tadc= 25*Tcy */
#define ADC_CONV_CLK_26Tcy         0b00011001 /* Reloj de conversión ADC Tadc= 26*Tcy */
#define ADC_CONV_CLK_27Tcy         0b00011010 /* Reloj de conversión ADC Tadc= 27*Tcy */
#define ADC_CONV_CLK_28Tcy         0b00011011 /* Reloj de conversión ADC Tadc= 28*Tcy */
#define ADC_CONV_CLK_29Tcy         0b00011100 /* Reloj de conversión ADC Tadc= 29*Tcy */
#define ADC_CONV_CLK_30Tcy         0b00011101 /* Reloj de conversión ADC Tadc= 40*Tcy */
#define ADC_CONV_CLK_31Tcy         0b00011110 /* Reloj de conversión ADC Tadc= 31*Tcy */
#define ADC_CONV_CLK_32Tcy         0b00011111 /* Reloj de conversión ADC Tadc= 32*Tcy */
#define ADC_CONV_CLK_33Tcy         0b00100000 /* Reloj de conversión ADC Tadc= 33*Tcy */
#define ADC_CONV_CLK_34Tcy         0b00100001 /* Reloj de conversión ADC Tadc= 34*Tcy */
#define ADC_CONV_CLK_35Tcy         0b00100010 /* Reloj de conversión ADC Tadc= 35*Tcy */
#define ADC_CONV_CLK_36Tcy         0b00100011 /* Reloj de conversión ADC Tadc= 36*Tcy */
#define ADC_CONV_CLK_37Tcy         0b00100100 /* Reloj de conversión ADC Tadc= 37*Tcy */
#define ADC_CONV_CLK_38Tcy         0b00100101 /* Reloj de conversión ADC Tadc= 38*Tcy */
#define ADC_CONV_CLK_39Tcy         0b00100110 /* Reloj de conversión ADC Tadc= 39*Tcy */
#define ADC_CONV_CLK_40Tcy         0b00100111 /* Reloj de conversión ADC Tadc= 40*Tcy */
#define ADC_CONV_CLK_41Tcy         0b00101000 /* Reloj de conversión ADC Tadc= 41*Tcy */
#define ADC_CONV_CLK_42Tcy         0b00101001 /* Reloj de conversión ADC Tadc= 42*Tcy */
#define ADC_CONV_CLK_43Tcy         0b00101010 /* Reloj de conversión ADC Tadc= 43*Tcy */
#define ADC_CONV_CLK_44Tcy         0b00101011 /* Reloj de conversión ADC Tadc= 44*Tcy */
#define ADC_CONV_CLK_45Tcy         0b00101100 /* Reloj de conversión ADC Tadc= 45*Tcy */
#define ADC_CONV_CLK_46Tcy         0b00101101 /* Reloj de conversión ADC Tadc= 46*Tcy */
#define ADC_CONV_CLK_47Tcy         0b00101110 /* Reloj de conversión ADC Tadc= 47*Tcy */
#define ADC_CONV_CLK_48Tcy         0b00101111 /* Reloj de conversión ADC Tadc= 48*Tcy */
#define ADC_CONV_CLK_49Tcy         0b00110000 /* Reloj de conversión ADC Tadc= 49*Tcy */
#define ADC_CONV_CLK_50Tcy         0b00110001 /* Reloj de conversión ADC Tadc= 50*Tcy */
#define ADC_CONV_CLK_51Tcy         0b00110010 /* Reloj de conversión ADC Tadc= 51*Tcy */
#define ADC_CONV_CLK_52Tcy         0b00110011 /* Reloj de conversión ADC Tadc= 52*Tcy */
#define ADC_CONV_CLK_53Tcy         0b00110100 /* Reloj de conversión ADC Tadc= 53*Tcy */
#define ADC_CONV_CLK_54Tcy         0b00110101 /* Reloj de conversión ADC Tadc= 54*Tcy */
#define ADC_CONV_CLK_55Tcy         0b00110110 /* Reloj de conversión ADC Tadc= 55*Tcy */
#define ADC_CONV_CLK_56Tcy         0b00110111 /* Reloj de conversión ADC Tadc= 56*Tcy */
#define ADC_CONV_CLK_57Tcy         0b00111000 /* Reloj de conversión ADC Tadc= 57*Tcy */
#define ADC_CONV_CLK_58Tcy         0b00111001 /* Reloj de conversión ADC Tadc= 58*Tcy */
#define ADC_CONV_CLK_59Tcy         0b00111010 /* Reloj de conversión ADC Tadc= 59*Tcy */
#define ADC_CONV_CLK_60Tcy         0b00111011 /* Reloj de conversión ADC Tadc= 60*Tcy */
#define ADC_CONV_CLK_61Tcy         0b00111100 /* Reloj de conversión ADC Tadc= 61*Tcy */
#define ADC_CONV_CLK_62Tcy         0b00111101 /* Reloj de conversión ADC Tadc= 62*Tcy */
#define ADC_CONV_CLK_63Tcy         0b00111110 /* Reloj de conversión ADC Tadc= 63*Tcy */
#define ADC_CONV_CLK_64Tcy         0b00111111 /* Reloj de conversión ADC Tadc= 64*Tcy */

/**** ADCON5H register definition ****/
#define ADC_AUTO_SCAN_ENABLE        0b10000000 /*Auto-Scan is enabled*/
#define ADC_AUTO_SCAN_DISABLE       0b00000000 /*Auto-Scan is disabled*/

#define ADC_LOW_POWER_ENABLE        0b01000000 /*Low power enabled after scan*/
#define ADC_LOW_POWER_DISABLE       0b00000000 /*Full power enabled after scan*/

#define ADC_CTMU_ENABLE             0b00100000 /*CTMU is enabled when the ADC is enabled and active*/
#define ADC_CTMU_DISABLE            0b00000000 /*CTMU is not enabled by the ADC*/

#define ADC_BG_ENABLE               0b00010000 /*Band gap is enabled when the ADC is enabled and active*/
#define ADC_BG_DISABLE              0b00000000 /*Band gap is not enabled by the ADC*/

#define ADC_AUTO_INT_THRES_COMP    	0b00000000 /*Interrupt occurs after sampling is completed and valid compare has occured*/
#define ADC_AUTO_INT_COMP    		0b00000001/*Interrupt occurs after valid compare has occured*/
#define ADC_INT_THRES    			0b00000010 /*Interrupt occurs after sampling is completed*/
#define ADC_AUTO_SCAN	    		0b00000011 /*No Interrupt*/

/**** ADCON5L register definition ****/
#define ADC_WRITE_AUTO_COMPARE      0b00001000 /*conversion results not saved, but interrupts are generated when a valid match as defined by CM and ASINT bits occurs*/
#define ADC_WRITE_CONVERT_SAVE      0b00000100 /*conversion results saved to locations as determined by register bits when a match as defined by CM bits occurs*/
#define ADC_WRITE_LEGACY            0b00000000 /*conversion data saved to location determined by buffer register bits */

#define ADC_CMP_OUTSIDE_MODE        0b00000011 /*valid match occurs when conversion result is outside of the window*/
#define ADC_CMP_INSIDE_MODE         0b00000010 /*valid match occurs when conversion result is inside the window*/
#define ADC_CMP_GREATER_MODE        0b00000001 /*valid match occurs when result is greater than corresponding buffer register*/
#define ADC_CMP_LESS_MODE           0b00000000 /*valid match occurs when result is less than corresponding buffer register*/

/**** ADCHS0H register definition ****/
#define ADC_CH0_NEG_SAMPLEB_VREFN   0b00000000  /* CH0 negative input is VREF- */
#define ADC_CH0_NEG_SAMPLEB_AN1     0b01000000 /* CH0 negative input is AN1 */

#define ADC_CH0_POS_SAMPLEB_AN0     0b00000000 /* A/D CH0 pos i/p sel for SAMPLE B is AN0 */
#define ADC_CH0_POS_SAMPLEB_AN1     0b00000001 /* A/D CH0 pos i/p sel for SAMPLE B is AN1 */
#define ADC_CH0_POS_SAMPLEB_AN2     0b00000010 /* A/D CH0 pos i/p sel for SAMPLE B is AN2 */
#define ADC_CH0_POS_SAMPLEB_AN3     0b00000011 /* A/D CH0 pos i/p sel for SAMPLE B is AN3 */
#define ADC_CH0_POS_SAMPLEB_AN4     0b00000100 /* A/D CH0 pos i/p sel for SAMPLE B is AN4 */
#define ADC_CH0_POS_SAMPLEB_AN5     0b00000101 /* A/D CH0 pos i/p sel for SAMPLE B is AN5 */
#define ADC_CH0_POS_SAMPLEB_AN6     0b00000110 /* A/D CH0 pos i/p sel for SAMPLE B is AN6 */
#define ADC_CH0_POS_SAMPLEB_AN7     0b00000111 /* A/D CH0 pos i/p sel for SAMPLE B is AN7 */
#define ADC_CH0_POS_SAMPLEB_AN8     0b00001000 /* A/D CH0 pos i/p sel for SAMPLE B is AN8 */
#define ADC_CH0_POS_SAMPLEB_AN9     0b00001001 /* A/D CH0 pos i/p sel for SAMPLE B is AN9 */
#define ADC_CH0_POS_SAMPLEB_AN10    0b00001010 /* A/D CH0 pos i/p sel for SAMPLE B is AN9 */
#define ADC_CH0_POS_SAMPLEB_AN11    0b00001011 /* A/D CH0 pos i/p sel for SAMPLE B is AN11 */
#define ADC_CH0_POS_SAMPLEB_AN12    0b00001100 /* A/D CH0 pos i/p sel for SAMPLE B is AN12 */
#define ADC_CH0_POS_SAMPLEB_AN13    0b00001101 /* A/D CH0 pos i/p sel for SAMPLE B is AN13 */
#define ADC_CH0_POS_SAMPLEB_AN14    0b00001110 /* A/D CH0 pos i/p sel for SAMPLE B is AN14 */
#define ADC_CH0_POS_SAMPLEB_AN15   	0b00001111 /* A/D CH0 pos i/p sel for SAMPLE B is AN15 */
#if	defined (ADC_V15_1)
#define ADC_CH0_POS_SAMPLEB_AN16    0b00010000  /* A/D CH0 pos i/p sel for SAMPLE B is AN16 */
#define ADC_CH0_POS_SAMPLEB_AN17    0b00010001 /* A/D CH0 pos i/p sel for SAMPLE B is AN17 */
#define ADC_CH0_POS_SAMPLEB_AN18    0b00010010 /* A/D CH0 pos i/p sel for SAMPLE B is AN18 */
#define ADC_CH0_POS_SAMPLEB_AN19    0b00010011 /* A/D CH0 pos i/p sel for SAMPLE B is AN19 */
#define ADC_CH0_POS_SAMPLEB_AN20    0b00010100 /* A/D CH0 pos i/p sel for SAMPLE B is AN20 */
#define ADC_CH0_POS_SAMPLEB_AN21    0b00010101 /* A/D CH0 pos i/p sel for SAMPLE B is AN21 */
#define ADC_CH0_POS_SAMPLEB_AN22    0b00010110 /* A/D CH0 pos i/p sel for SAMPLE B is AN22 */
#define ADC_CH0_POS_SAMPLEB_AN23    0b00010111 /* A/D CH0 pos i/p sel for SAMPLE B is AN23 */
#endif
#define ADC_CH0_POS_SAMPLEB_CTMU_TEMP   0b00011000 /* No Channel used; all input floating; used for CTMU Temperature Sensor input*/
#define ADC_CH0_POS_SAMPLEB_CTMU     	0b00011001 /* No Channel used; all input floating; used for CTMU*/
#define ADC_CH0_POS_SAMPLEB_VBGDIV6  0b00011010 /* A/D CH0 pos i/p sel for SAMPLE B is band gap divided-by-six-reference (VBG/6) */
#define ADC_CH0_POS_SAMPLEB_VBGDIV2  0b00011011 /* A/D CH0 pos i/p sel for SAMPLE B is band gap divided-by-two-reference (VBG/2) */
#define ADC_CH0_POS_SAMPLEB_VBG		 0b00011100 /* A/D CH0 pos i/p sel for SAMPLE B is band gap reference (VBG) */
#define ADC_CH0_POS_SAMPLEB_AVSS     0b00011101 /* A/D CH0 pos i/p sel for SAMPLE B is AVSS */
#define ADC_CH0_POS_SAMPLEB_AVDD     0b00011110 /* A/D CH0 pos i/p sel for SAMPLE B is AVDD */
#define ADC_CH0_POS_SAMPLEB_VBAT     0b00011111 /* A/D CH0 pos i/p sel for SAMPLE B is VBAT */

/**** ADCHS0L register definition ****/
#define ADC_CH0_NEG_SAMPLEA_VREFN   0b00000000  /* CH0 negative input is VREF- */
#define ADC_CH0_NEG_SAMPLEA_AN1     0b01000000 /* CH0 negative input is AN1 */

#define ADC_CH0_POS_SAMPLEA_AN0     0b00000000 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN0 */
#define ADC_CH0_POS_SAMPLEA_AN1     0b00000001 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN1 */
#define ADC_CH0_POS_SAMPLEA_AN2     0b00000010 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN2 */
#define ADC_CH0_POS_SAMPLEA_AN3     0b00000011 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN3 */
#define ADC_CH0_POS_SAMPLEA_AN4     0b00000100 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN4 */
#define ADC_CH0_POS_SAMPLEA_AN5     0b00000101 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN5 */
#define ADC_CH0_POS_SAMPLEA_AN6     0b00000110 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN6 */
#define ADC_CH0_POS_SAMPLEA_AN7     0b00000111 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN7 */
#define ADC_CH0_POS_SAMPLEA_AN8     0b00001000 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN8 */
#define ADC_CH0_POS_SAMPLEA_AN9     0b00001001 /* A/D CH0 pos i/p sel for SAMPLE B is AN9 */
#define ADC_CH0_POS_SAMPLEA_AN10    0b00001010 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN10 */
#define ADC_CH0_POS_SAMPLEA_AN11    0b00001011 /* A/D CH0 pos i/p sel for SAMPLE B is AN11 */
#define ADC_CH0_POS_SAMPLEA_AN12    0b00001100 /* A/D CH0 pos i/p sel for SAMPLE B is AN12 */
#define ADC_CH0_POS_SAMPLEA_AN13    0b00001101 /* A/D CH0 pos i/p sel for SAMPLE B is AN13 */
#define ADC_CH0_POS_SAMPLEA_AN14    0b00001110 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN14 */
#define ADC_CH0_POS_SAMPLEA_AN15    0b00001111 /* A/D Chan 0 pos i/p sel for SAMPLE A is AN15 */
#if defined (ADC_V15_1) 
#define ADC_CH0_POS_SAMPLEA_AN16    0b00010000 /* A/D CH0 pos i/p sel for SAMPLE A is AN16 */
#define ADC_CH0_POS_SAMPLEA_AN17    0b00010001 /* A/D CH0 pos i/p sel for SAMPLE A is AN17 */
#define ADC_CH0_POS_SAMPLEA_AN18    0b00010010 /* A/D CH0 pos i/p sel for SAMPLE A is AN18 */
#define ADC_CH0_POS_SAMPLEA_AN19    0b00010011 /* A/D CH0 pos i/p sel for SAMPLE A is AN19 */
#define ADC_CH0_POS_SAMPLEA_AN20    0b00010100 /* A/D CH0 pos i/p sel for SAMPLE A is AN20 */
#define ADC_CH0_POS_SAMPLEA_AN21    0b00010101 /* A/D CH0 pos i/p sel for SAMPLE A is AN21 */
#define ADC_CH0_POS_SAMPLEA_AN22    0b00010110 /* A/D CH0 pos i/p sel for SAMPLE A is AN22 */
#define ADC_CH0_POS_SAMPLEA_AN23    0b00010111 /* A/D CH0 pos i/p sel for SAMPLE A is AN23 */
#endif
#define ADC_CH0_POS_SAMPLEA_CTMU_TEMP   0b00011000 /* No Channel used; all input floating; used for CTMU Temperature Sensor input*/
#define ADC_CH0_POS_SAMPLEA_CTMU     	0b00011001 /* No Channel used; all input floating; used for CTMU*/
#define ADC_CH0_POS_SAMPLEA_VBGDIV6  0b00011010 /* A/D CH0 pos i/p sel for SAMPLE B is band gap divided-by-six-reference (VBG/6) */
#define ADC_CH0_POS_SAMPLEA_VBGDIV2  0b00011011 /* A/D CH0 pos i/p sel for SAMPLE B is band gap divided-by-two-reference (VBG/2) */
#define ADC_CH0_POS_SAMPLEA_VBG		 0b00011100 /* A/D CH0 pos i/p sel for SAMPLE B is band gap reference (VBG) */
#define ADC_CH0_POS_SAMPLEA_AVSS     0b00011101 /* A/D CH0 pos i/p sel for SAMPLE A is AVSS */
#define ADC_CH0_POS_SAMPLEA_AVDD     0b00011110 /* A/D CH0 pos i/p sel for SAMPLE A is AVDD */
#define ADC_CH0_POS_SAMPLEA_VBAT     0b00011111 /* A/D CH0 pos i/p sel for SAMPLE A is VBAT */

/**** ADCSS0L control register definition ****/
#define ADC_SCAN_AN0      			0b00000001 /*Habilita Input Scan AN0 */
#define ADC_SKIP_SCAN_AN0      		0b00000000 /* Deshabilita Input Scan AN0 */
#define ADC_SCAN_AN1      			0b00000010 /*Habilita Input Scan AN1 */
#define ADC_SKIP_SCAN_AN1      		0b00000000 /* Deshabilita Input Scan AN1 */
#define ADC_SCAN_AN2      			0b00000100 /*Habilita Input Scan AN2 */
#define ADC_SKIP_SCAN_AN2      		0b00000000 /* Deshabilita Input Scan AN2 */
#define ADC_SCAN_AN3      			0b00001000 /*Habilita Input Scan AN3 */
#define ADC_SKIP_SCAN_AN3      		0b00000000 /* Deshabilita Input Scan AN3 */
#define ADC_SCAN_AN4      			0b00010000 /*Habilita Input Scan AN4 */
#define ADC_SKIP_SCAN_AN4      		0b00000000 /* Deshabilita Input Scan AN4 */
#define ADC_SCAN_AN5      			0b00100000 /*Habilita Input Scan AN5 */
#define ADC_SKIP_SCAN_AN5      		0b00000000 /* Deshabilita Input Scan AN5 */
#define ADC_SCAN_AN6      			0b01000000 /*Habilita Input Scan AN6 */
#define ADC_SKIP_SCAN_AN6     		0b00000000 /* Deshabilita Input Scan AN6 */
#define ADC_SCAN_AN7      			0b10000000 /*Habilita Input Scan AN7 */
#define ADC_SKIP_SCAN_AN7      		0b00000000 /* Deshabilita Input Scan AN7 */
#define ENABLE_ALL_INPUT_SCAN       0b11111111 /*Habilita Input Scan AN0-AN15 */
#define DISABLE_ALL_INPUT_SCAN      0b00000000 /*Deshabilita Input Scan AN0-AN15 */

/**** ADCSS0H control register definition ****/
#define ADC_SCAN_AN8      			0b00000001 /*Habilita Input Scan AN8 */
#define ADC_SKIP_SCAN_AN8      		0b00000000 /* Deshabilita Input Scan AN8 */
#define ADC_SCAN_AN9      		    0b00000010 /*Habilita Input Scan AN9 */
#define ADC_SKIP_SCAN_AN9      		0b00000000 /* Deshabilita Input Scan AN9 */
#define ADC_SCAN_AN10     			0b00000100 /*Habilita Input Scan AN10 */
#define ADC_SKIP_SCAN_AN10     		0b00000000 /* Deshabilita Input Scan AN10 */
#define ADC_SCAN_AN11     			0b00001000 /*Habilita Input Scan AN11 */
#define ADC_SKIP_SCAN_AN11     		0b00000000 /* Deshabilita Input Scan AN11 */
#define ADC_SCAN_AN12     			0b00010000 /*Habilita Input Scan AN12 */
#define ADC_SKIP_SCAN_AN12          0b00000000 /* Deshabilita Input Scan AN12 */
#define ADC_SCAN_AN13    			0b00100000 /*Habilita Input Scan AN13 */
#define ADC_SKIP_SCAN_AN13     		0b00000000 /* Deshabilita Input Scan AN13 */
#define ADC_SCAN_AN14    			0b01000000 /*Habilita Input Scan AN14 */
#define ADC_SKIP_SCAN_AN14     		0b00000000 /* Deshabilita Input Scan AN14 */
#define ADC_SCAN_AN15    			0b10000000 /*Habilita Input Scan AN15 */
#define ADC_SKIP_SCAN_AN15     		0b00000000 /* Deshabilita Input Scan AN15 */
#define ENABLE_ALL_INPUT_SCAN       0b11111111 /*Habilita Input Scan AN0-AN15 */
#define DISABLE_ALL_INPUT_SCAN      0b00000000 /*Deshabilita Input Scan AN0-AN15 */

/**** ADCSS1L register definition ****/
#define ADC_SCAN_AN16     			0b00000001 /*Habilita Input Scan AN16 */
#define ADC_SKIP_SCAN_AN16    		0b00000000 /* Deshabilita Input Scan AN16 */
#define ADC_SCAN_AN17     			0b00000010 /*Habilita Input Scan AN17 */
#define ADC_SKIP_SCAN_AN17    		0b00000000 /* Deshabilita Input Scan AN17 */
#define ADC_SCAN_AN18     			0b00000100 /*Habilita Input Scan AN18 */
#define ADC_SKIP_SCAN_AN18    		0b00000000 /* Deshabilita Input Scan AN18 */
#define ADC_SCAN_AN19     			0b00001000 /*Habilita Input Scan AN19 */
#define ADC_SKIP_SCAN_AN19    		0b00000000 /* Deshabilita Input Scan AN19 */
#define ADC_SCAN_AN20     			0b00010000 /*Habilita Input Scan AN20 */
#define ADC_SKIP_SCAN_AN20   		0b00000000 /* Deshabilita Input Scan AN20 */
#define ADC_SCAN_AN21     			0b00100000 /*Habilita Input Scan AN21 */
#define ADC_SKIP_SCAN_AN21    		0b00000000 /* Deshabilita Input Scan AN21 */
#define ADC_SCAN_AN22     			0b01000000 /*Habilita Input Scan AN22 */
#define ADC_SKIP_SCAN_AN22    		0b00000000 /* Deshabilita Input Scan AN22 */
#define ADC_SCAN_AN23     			0b10000000 /*Habilita Input Scan AN23 */
#define ADC_SKIP_SCAN_AN23    		0b00000000 /* Deshabilita Input Scan AN23 */
#define ENABLE_ALL_INPUT_SCAN       0b11111111 /*Habilita Input Scan AN0-AN15 */
#define DISABLE_ALL_INPUT_SCAN      0b00000000 /*Deshabilita Input Scan AN0-AN15 */

/**** ADCSS1H register definition ****/
#define ADC_SCAN_AN24     			0b00000001 /*Habilita Input Scan AN24 */
#define ADC_SKIP_SCAN_AN24    		0b00000000 /* Deshabilita Input Scan AN24 */
#define ADC_SCAN_AN25     			0b00000010 /*Habilita Input Scan AN25 */
#define ADC_SKIP_SCAN_AN25    		0b00000000 /* Deshabilita Input Scan AN25 */
#define ADC_SCAN_AN26     			0b00000100 /*Habilita Input Scan AN26 */
#define ADC_SKIP_SCAN_AN26    		0b00000000 /* Deshabilita Input Scan AN26 */
#define ADC_SCAN_AN27     			0b00001000 /*Habilita Input Scan AN27 */
#define ADC_SKIP_SCAN_AN27    		0b00000000/* Deshabilita Input Scan AN27 */
#define ADC_SCAN_AN28     			0b00010000 /*Habilita Input Scan AN28 */
#define ADC_SKIP_SCAN_AN28    		0b00000000 /* Deshabilita Input Scan AN28 */
#define ADC_SCAN_AN29     			0b00100000 /*Habilita Input Scan AN29 */
#define ADC_SKIP_SCAN_AN29    		0b00000000/* Deshabilita Input Scan AN29 */
#define ADC_SCAN_AN30     			0b01000000 /*Habilita Input Scan AN30 */
#define ADC_SKIP_SCAN_AN30    		0b00000000/* Deshabilita Input Scan AN30 */
#define ENABLE_ALL_INPUT_SCAN       0b11111111 /*Habilita Input Scan AN0-AN15 */
#define DISABLE_ALL_INPUT_SCAN      0b00000000 /*Deshabilita Input Scan AN0-AN15 */

/*****ADCTMUEN0L register definition***********/
#define ADC_CTMU_AN0     			0b00000001 /*CTMU enabled and connected to AN0 */
#define ADC_SKIP_CTMU_AN0    		0b00000000 /*CTMU not connected to AN0*/
#define ADC_CTMU_AN1     			0b00000010 /*CTMU enabled and connected to AN1 */
#define ADC_SKIP_CTMU_AN1    		0b00000000 /*CTMU not connected to AN1*/
#define ADC_CTMU_AN2     			0b00000100 /*CTMU enabled and connected to AN2*/
#define ADC_SKIP_CTMU_AN2    		0b00000000 /*CTMU not connected to AN2*/
#define ADC_CTMU_AN3     			0b00001000 /*CTMU enabled and connected to AN3 */
#define ADC_SKIP_CTMU_AN3    		0b00000000 /*CTMU not connected to AN3*/
#define ADC_CTMU_AN4     			0b00010000 /*CTMU enabled and connected to AN4*/
#define ADC_SKIP_CTMU_AN4    		0b00000000 /*CTMU not connected to AN4*/
#define ADC_CTMU_AN5     			0b00100000 /*CTMU enabled and connected to AN5 */
#define ADC_SKIP_CTMU_AN5    		0b00000000 /*CTMU not connected to AN5*/
#define ADC_CTMU_AN6     			0b01000000 /*CTMU enabled and connected to AN6*/
#define ADC_SKIP_CTMU_AN6    		0b00000000 /*CTMU not connected to AN6*/
#define ADC_CTMU_AN7     			0b10000000 /*CTMU enabled and connected to AN7 */
#define ADC_SKIP_CTMU_AN7    		0b00000000 /*CTMU not connected to AN7*/

/*****ADCTMUEN0H register definition***********/
#define ADC_CTMU_AN8     			0b00000001 /*CTMU enabled and connected to AN8 */
#define ADC_SKIP_CTMU_AN8    		0b00000000 /*CTMU not connected to AN8*/
#define ADC_CTMU_AN9     			0b00000010 /*CTMU enabled and connected to AN9*/
#define ADC_SKIP_CTMU_AN9    		0b00000000 /*CTMU not connected to AN9*/
#define ADC_CTMU_AN10     			0b00000100 /*CTMU enabled and connected to AN10*/
#define ADC_SKIP_CTMU_AN10    		0b00000000 /*CTMU not connected to AN10*/
#define ADC_CTMU_AN11     			0b00001000 /*CTMU enabled and connected to AN11*/
#define ADC_SKIP_CTMU_AN11    		0b00000000 /*CTMU not connected to AN11*/
#define ADC_CTMU_AN12     			0b00010000 /*CTMU enabled and connected to AN12*/
#define ADC_SKIP_CTMU_AN12    		0b00000000 /*CTMU not connected to AN12*/
#define ADC_CTMU_AN13     			0b00100000 /*CTMU enabled and connected to AN13*/
#define ADC_SKIP_CTMU_AN13    		0b00000000 /*CTMU not connected to AN13*/
#define ADC_CTMU_AN14     			0b01000000 /*CTMU enabled and connected to AN14*/
#define ADC_SKIP_CTMU_AN14    		0b00000000 /*CTMU not connected to AN14*/
#define ADC_CTMU_AN15     			0b10000000 /*CTMU enabled and connected to AN15*/
#define ADC_SKIP_CTMU_AN15    		0b00000000 /*CTMU not connected to AN15*/

/*****ADCTMUEN1L register definition***********/
#define ADC_CTMU_AN16     			0b00000001 /*CTMU enabled and connected to AN16 */
#define ADC_SKIP_CTMU_AN16    		0b00000000 /*CTMU not connected to AN16*/
#define ADC_CTMU_AN17     			0b00000010 /*CTMU enabled and connected to AN17 */
#define ADC_SKIP_CTMU_AN17    		0b00000000 /*CTMU not connected to AN17*/
#define ADC_CTMU_AN18     			0b00000100 /*CTMU enabled and connected to AN18 */
#define ADC_SKIP_CTMU_AN18    		0b00000000 /*CTMU not connected to AN18*/
#define ADC_CTMU_AN19     			0b00001000 /*CTMU enabled and connected to AN19 */
#define ADC_SKIP_CTMU_AN19    		0b00000000 /*CTMU not connected to AN19*/
#define ADC_CTMU_AN20     			0b00010000 /*CTMU enabled and connected to AN20 */
#define ADC_SKIP_CTMU_AN20    		0b00000000 /*CTMU not connected to AN20*/
#define ADC_CTMU_AN21     			0b00100000 /*CTMU enabled and connected to AN21 */
#define ADC_SKIP_CTMU_AN21    		0b00000000 /*CTMU not connected to AN21*/
#define ADC_CTMU_AN22     			0b01000000  /*CTMU enabled and connected to AN22 */
#define ADC_SKIP_CTMU_AN22    		0b00000000 /*CTMU not connected to AN22*/
#define ADC_CTMU_AN23    			0b10000000 /*CTMU enabled and connected to AN23 */
#define ADC_SKIP_CTMU_AN23   		0b00000000 /*CTMU not connected to AN23*/

/*****ADCTMUEN1H register definition***********/
#define ADC_CTMU_AN24    			0b00000001 /*CTMU enabled and connected to AN24 */
#define ADC_SKIP_CTMU_AN24   		0b00000000 /*CTMU not connected to AN24*/
#define ADC_CTMU_AN25    			0b00000010 /*CTMU enabled and connected to AN25 */
#define ADC_SKIP_CTMU_AN25   		0b00000000 /*CTMU not connected to AN25*/
#define ADC_CTMU_AN26     			0b00000100 /*CTMU enabled and connected to AN26 */
#define ADC_SKIP_CTMU_AN26    		0b00000000 /*CTMU not connected to AN26*/
#define ADC_CTMU_AN27     			0b00001000 /*CTMU enabled and connected to AN27 */
#define ADC_SKIP_CTMU_AN27    		0b00000000 /*CTMU not connected to AN27*/
#define ADC_CTMU_AN28     			0b00010000 /*CTMU enabled and connected to AN28 */
#define ADC_SKIP_CTMU_AN28    		0b00000000 /*CTMU not connected to AN28*/
#define ADC_CTMU_AN29     			0b00100000 /*CTMU enabled and connected to AN29 */
#define ADC_SKIP_CTMU_AN29    		0b00000000 /*CTMU not connected to AN29*/
#define ADC_CTMU_AN30     			0b01000000 /*CTMU enabled and connected to AN30 */
#define ADC_SKIP_CTMU_AN30    		0b00000000 /*CTMU not connected to AN30*/
/**********************************************************************************************************************************/

/*ADC Port Configuration register definition*/
#define ENABLE_AN0_DIG()              ANCON1bits.ANSEL0=1 /*Habilita AN0 en modo digital */
#define ENABLE_AN0_ANA()              ANCON1bits.ANSEL0=0 /*Habilita AN0 en modo analógico */
#define ENABLE_AN1_DIG()              ANCON1bits.ANSEL1=1 /*Habilita AN1 en modo digital */
#define ENABLE_AN1_ANA()              ANCON1bits.ANSEL1=0 /*Habilita AN1 en modo analógico */
#define ENABLE_AN2_DIG()              ANCON1bits.ANSEL2=1 /*Habilita AN2 en modo digital */
#define ENABLE_AN2_ANA()              ANCON1bits.ANSEL2=0 /*Habilita AN2 en modo analógico */
#define ENABLE_AN3_DIG()              ANCON1bits.ANSEL3=1 /*Habilita AN3 en modo digital */
#define ENABLE_AN3_ANA()              ANCON1bits.ANSEL3=0 /*Habilita AN3 en modo analógico */
#define ENABLE_AN4_DIG()              ANCON1bits.ANSEL4=1 /*Habilita AN4 en modo digital */
#define ENABLE_AN4_ANA()              ANCON1bits.ANSEL4=0 /*Habilita AN4 en modo analógico */
#define ENABLE_AN5_DIG()              ANCON1bits.ANSEL5=1 /*Habilita AN5 en modo digital */
#define ENABLE_AN5_ANA()              ANCON1bits.ANSEL5=0 /*Habilita AN5 en modo analógico */
#define ENABLE_AN6_DIG()              ANCON1bits.ANSEL6=1 /*Habilita AN6 en modo digital */
#define ENABLE_AN6_ANA()              ANCON1bits.ANSEL6=0 /*Habilita AN6 en modo analógico */
#define ENABLE_AN7_DIG()              ANCON1bits.ANSEL7=1 /*Habilita AN7 en modo digital */
#define ENABLE_AN7_ANA()              ANCON1bits.ANSEL7=0 /*Habilita AN7 en modo analógico */
#define ENABLE_ALL_ANA_0_7()          ANCON1=0x00 /*Habilita AN0-AN7 en modo analógico */
#define ENABLE_ALL_DIG_0_7()          ANCON1=0xFF /*Habilita AN0-AN7 en modo digital */

#define ENABLE_AN8_DIG()              ANCON1bits.ANSEL8=1 /*Habilita AN8 en modo digital */
#define ENABLE_AN8_ANA()              ANCON1bits.ANSEL8=0 /*Habilita AN8 en modo analógico */
#define ENABLE_AN9_DIG()              ANCON1bits.ANSEL9=1 /*Habilita AN9 en modo digital */
#define ENABLE_AN9_ANA()              ANCON1bits.ANSEL9=0 /*Habilita AN9 en modo analógico */
#define ENABLE_AN10_DIG()             ANCON1bits.ANSEL10=1 /*Habilita AN10 en modo digital */
#define ENABLE_AN10_ANA()             ANCON1bits.ANSEL10=0 /*Habilita AN10 en modo analógico */
#define ENABLE_AN11_DIG()             ANCON1bits.ANSEL11=1 /*Habilita AN11 en modo digital */
#define ENABLE_AN11_ANA()             ANCON1bits.ANSEL11=0 /*Habilita AN11 en modo analógico */
#define ENABLE_AN12_DIG()             ANCON1bits.ANSEL12=1 /*Habilita AN12 en modo digital */
#define ENABLE_AN12_ANA()             ANCON1bits.ANSEL12=0 /*Habilita AN12 en modo analógico */
#define ENABLE_AN13_DIG()             ANCON1bits.ANSEL13=1 /*Habilita AN13 en modo digital */
#define ENABLE_AN13_ANA()             ANCON1bits.ANSEL13=0 /*Habilita AN13 en modo analógico */
#define ENABLE_AN14_DIG()             ANCON1bits.ANSEL14=1 /*Habilita AN14 en modo digital */
#define ENABLE_AN14_ANA()             ANCON1bits.ANSEL14=0 /*Habilita AN14 en modo analógico */
#define ENABLE_AN15_DIG()             ANCON1bits.ANSEL15=1 /*Habilita AN15 en modo digital */
#define ENABLE_AN15_ANA()             ANCON1bits.ANSEL15=0 /*Habilita AN15 en modo analógico */
#define ENABLE_ALL_ANA_8_15()         ANCON1=0x00 /*Habilita AN8-AN15 en modo analógico */
#define ENABLE_ALL_DIG_8_15()         ANCON1=0xFF /*Habilita AN8-AN15 en modo digital */

#define ENABLE_AN16_DIG()             ANCON2bits.ANSEL16=1 /*Habilita AN16 en modo digital */
#define ENABLE_AN16_ANA()             ANCON2bits.ANSEL16=0 /*Habilita AN16 en modo analógico */
#define ENABLE_AN17_DIG()             ANCON2bits.ANSEL17=1 /*Habilita AN17 en modo digital */
#define ENABLE_AN17_ANA()             ANCON2bits.ANSEL17=0 /*Habilita AN17 en modo analógico */
#define ENABLE_AN18_DIG()             ANCON2bits.ANSEL18=1 /*Habilita AN18 en modo digital */
#define ENABLE_AN18_ANA()             ANCON2bits.ANSEL18=0 /*Habilita AN18 en modo analógico */
#define ENABLE_AN19_DIG()             ANCON2bits.ANSEL19=1 /*Habilita AN19 en modo digital */
#define ENABLE_AN19_ANA()             ANCON2bits.ANSEL19=0 /*Habilita AN19 en modo analógico */
#define ENABLE_AN20_DIG()             ANCON2bits.ANSEL20=1 /*Habilita AN20 en modo digital */
#define ENABLE_AN20_ANA()             ANCON2bits.ANSEL20=0 /*Habilita AN20 en modo analógico */
#define ENABLE_AN21_DIG()             ANCON2bits.ANSEL21=1 /*Habilita AN21 en modo digital */
#define ENABLE_AN21_ANA()             ANCON2bits.ANSEL21=0 /*Habilita AN21 en modo analógico */
#define ENABLE_AN22_DIG()             ANCON2bits.ANSEL22=1 /*Habilita AN22 en modo digital */
#define ENABLE_AN22_ANA()             ANCON2bits.ANSEL22=0 /*Habilita AN22 en modo analógico */
#define ENABLE_AN23_DIG()             ANCON2bits.ANSEL23=1 /*Habilita AN23 en modo digital */
#define ENABLE_AN23_ANA()             ANCON2bits.ANSEL23=0 /*Habilita AN23 en modo analógico */
#define ENABLE_ALL_ANA_16_23()        ANCON2=0x00 /*Habilita AN16-AN23 en modo analógico */
#define ENABLE_ALL_DIG_16_23()        ANCON2=0xFF /*Habilita AN16-AN23 en modo digital */

#endif

/*Macros for backward compatibility*/ 
#if defined (ADC_V8)

#define ADC_CHAN0   0b1111111111111110  // AN0
#define ADC_CHAN1   0b1111111111111101  // AN1
#define ADC_CHAN2   0b1111111111111011  // AN2
#define ADC_CHAN3   0b1111111111110111  // AN3
#define ADC_CHAN4   0b1111111111101111  // AN4
#define ADC_CHAN5   0b1111111111011111  // AN5
#define ADC_CHAN6   0b1111111110111111  // AN6
#define ADC_CHAN7   0b1111111101111111  // AN7
#define ADC_CHAN8   0b1111111011111111  // AN8
#define ADC_CHAN9   0b1111110111111111  // AN9
#define ADC_CHAN10  0b1111101111111111  // AN10
#define ADC_CHAN11  0b1111011111111111  // AN11
#define ADC_CHAN12  0b1110111111111111  // AN12
#define ADC_CHAN13  0b1101111111111111  // AN13
#define ADC_CHAN14  0b1011111111111111  // AN14
#define ADC_CHAN15  0b0111111111111111  // AN15

#endif

#if defined ( ADC_V2 ) || defined (ADC_V3) || defined (ADC_V4 ) ||\
    defined ( ADC_V5 ) || defined ( ADC_V6 ) || defined (ADC_V7) ||\
	defined (ADC_V7_1) || defined ( ADC_V8 ) || defined (ADC_V9) || defined (ADC_V9_1) ||\
	defined (ADC_V11) || defined (ADC_V12)

#define ADC_VREFPLUS_VDD	ADC_REF_VDD_VREFMINUS      	// VREF+ = AVDD
#define ADC_VREFPLUS_EXT	ADC_REF_VREFPLUS_VREFMINUS 	// VREF+ = Externa
#define ADC_VREFMINUS_VSS	ADC_REF_VREFPLUS_VSS       	// VREF- = AVSS
#define ADC_VREFMINUS_EXT	ADC_REF_VREFPLUS_VREFMINUS 	// VREF- = Externa

#elif defined (ADC_V10) || defined (ADC_V15) || defined (ADC_V15_1)
	
#define ADC_VREFPLUS_VDD	ADC_REF_VDD_VDD      		// VREF+ = AVDD
#define ADC_VREFPLUS_EXT	ADC_REF_VDD_VREFPLUS 		// VREF+ = Externa
#define ADC_VREFMINUS_VSS	ADC_REF_VDD_VSS       		// VREF- = AVSS
#define ADC_VREFMINUS_EXT	ADC_REF_VDD_VREFMINUS 		// VREF- = Externa

#endif

/*
	Definición de prototipos de funciones:
	- adc_init
	- adc_read
	- adc_setChannel
*/

#if defined (ADC_V1) || defined (ADC_V2)

void adc_init ( uint8_t param_config , uint8_t param_config2 );

#elif defined (ADC_V3) || defined (ADC_V4) || defined (ADC_V5) || defined (ADC_V6) ||\
      defined (ADC_V7) || defined (ADC_V7_1)|| defined (ADC_V12) || defined (ADC_V13)\
	  || defined (ADC_V13_1) || defined (ADC_V13_2) || defined (ADC_V13_3) || \
	  defined (ADC_V14) || defined (ADC_V14_1) || defined (ADC_V14_2) || defined (ADC_V14_3)

void adc_init ( uint8_t param_config , uint8_t param_config2 , uint8_t port_config );

#elif defined (ADC_V8) || defined (ADC_V9) || defined (ADC_V9_1) 

void adc_init ( uint8_t param_config , uint8_t param_config2 , uint16_t port_config );

#elif defined (ADC_V10) || defined (ADC_V11_1) 

void adc_init ( uint8_t param_config , uint8_t param_config2 , uint8_t param_config3 , uint16_t port_config);
			  
#elif defined (ADC_V11)

void adc_init ( uint8_t param_config , uint8_t param_config2 , uint16_t param_config3);
			   
#elif	defined (ADC_V15) || defined (ADC_V15_1)	

void adc_init( uint8_t param_config, uint8_t param_config2,	
uint8_t param_config3, uint8_t param_config4,
uint8_t param_config5, uint8_t param_config6 uint8_t param_config7, uint8_t param_config8);
		   
#endif

#if	defined (ADC_V15) || defined (ADC_V15_1)	

uint16_t adc_read(uint8_t indice_buffer, uint8_t modo_lectura_adc);
				  
#else

uint16_t adc_read(uint8_t modo_lectura_adc);

#endif

#if defined (ADC_V1) || defined (ADC_V2) || defined (ADC_V3) ||\
    defined (ADC_V4) || defined ( ADC_V5 ) || defined ( ADC_V6 )||\
	defined ( ADC_V8 ) || defined ( ADC_V9) || defined (ADC_V9_1) || defined ( ADC_V10)||\
	defined ( ADC_V11) || defined (ADC_V12) || defined (ADC_V13) || \
	defined (ADC_V13_1) || defined ( ADC_V11_1) || defined (ADC_V13_2)\
	|| defined (ADC_V13_3) || defined (ADC_V14) || defined (ADC_V14_1) || defined (ADC_V14_2) || defined (ADC_V14_3)

void adc_setChannel(uint8_t canal);

#elif	defined (ADC_V15) || defined (ADC_V15_1)	

void adc_setChannel ( uint8_t canalA , uint8_t canalB);

#endif

#endif /* ADC_H */