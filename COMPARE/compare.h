/**
  * @file compare.h
  * @brief Funciones para manejo de módulos CCP en modo output compare en microcontroladores PIC de 8 bits
  * @author Ing. José Roberto Parra Trewartha
*/

#ifndef COMPARE_H
#define COMPARE_H

/*
    Macros que definen modos de funcionamiento del módulo CCP en modo Output Compare
*/

//Inicializa pin asociado en bajo, lo pone en alto cuando hay comparación exitosa y pone en alto bandera CCPxIF
#define CCP_COMPARE_SET_ON_MATCH	0x08
//Inicializa pin asociado en alto, lo pone en bajo cuando hay comparaciónn exitosa y pone en alto bandera CCPxIF	
#define CCP_COMPARE_CLR_ON_MATCH	0x09	
//Interrupción por software, pin asociado no cambia y pone en alto bandera CCPxIF
#define CCP_COMPARE_INT				0x0A
//Cambia de estado el pin asociado y pone en alto bandera CCPxIF	
#define CCP_COMPARE_INT_AND_TOGGLE  0x02	
//Desencadena evento especial, reinicia timer asociado y pone en alto bandera CCPxIF. En algunos modelos inicia conversión ADC.
#define CCP_COMPARE_RESET_TIMER		0x0B	

#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)

/*
    Macros de selección de temporizador para módulo Output Compare
*/

//*************CCP4**************

// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_4_SEL_TMR12         0b00000000      
// CCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define CCP_4_SEL_TMR34         0b00010000      
// CCP : TIMER3 -> Captura, Comparación TIMER6 -> PWM
#define CCP_4_SEL_TMR36         0b00100000      

//*************CCP5**************

// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_5_SEL_TMR12         0b00000000      
// CCP : TIMER5 -> Captura, Comparación TIMER4 -> PWM
#define CCP_5_SEL_TMR54         0b00010000      

//*************CCP6**************

// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_6_SEL_TMR12         0b00000000     
// CCP : TIMER5 -> Captura, Comparación TIMER2 -> PWM
#define CCP_6_SEL_TMR52         0b00010000      

//*************CCP7**************

// CCP : TIMER3 -> Captura, Comparación TIMER2 -> PWM
#define CCP_7_SEL_TMR12         0b00000000      
// CCP : TIMER5 -> Captura, Comparación TIMER4 -> PWM
#define CCP_7_SEL_TMR54         0b00010000      
// CCP : TIMER5 -> Captura, Comparación TIMER6 -> PWM
#define CCP_7_SEL_TMR56         0b00100000      
// CCP : TIMER5 -> Captura, Comparación TIMER8 -> PWM
#define CCP_7_SEL_TMR58         0b00110000      

#if defined (CC_V8) || defined (CC_V8_3)

//*************CCP8**************

// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_8_SEL_TMR12         0b00000000
#if defined (CC_V8)
// CCP : TIMER7 -> Captura, Comparación TIMER4 -> PWM
#define CCP_8_SEL_TMR74         0b00010000
// CCP : TIMER7 -> Captura, Comparación TIMER6 -> PWM
#define CCP_8_SEL_TMR76         0b00100000
#else
// CCP : TIMER5 -> Captura, Comparación TIMER4 -> PWM
#define CCP_8_SEL_TMR54         0b00010000
// CCP : TIMER5 -> Captura, Comparación TIMER6 -> PWM
#define CCP_8_SEL_TMR56         0b00100000
#endif
#endif

#if defined (CC_V8_1) || defined (CC_V8_4)

//*************CCP8**************

// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_8_SEL_TMR12         0b00000000  
// CCP : TIMER1 -> Captura, Comparación TIMER4 -> PWM
#define CCP_8_SEL_TMR14         0b00010000  
// CCP : TIMER1 -> Captura, Comparación TIMER6 -> PWM
#define CCP_8_SEL_TMR16         0b00100000  
#endif

#if defined (CC_V8)|| defined (CC_V8_3)

//*************CCP9**************

// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_9_SEL_TMR12         0b00000000 
#if defined (CC_V8_3)
// CCP : TIMER5 -> Captura, Comparación TIMER4 -> PWM
#define CCP_9_SEL_TMR54         0b00010000 
#else
// CCP : TIMER7 -> Captura, Comparación TIMER4 -> PWM
#define CCP_9_SEL_TMR74         0b00010000 
#endif

//*************CCP10**************

// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_10_SEL_TMR12         0b00000000
#if defined (CC_V8_3)
// CCP : TIMER5 -> Captura, Comparación TIMER2 -> PWM
#define CCP_10_SEL_TMR52         0b00010000
#else
// CCP : TIMER7 -> Captura, Comparación TIMER2 -> PWM
#define CCP_10_SEL_TMR72         0b00010000
#endif
#endif

#endif  

#if defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_3)|| defined (ECC_V8_4)

// ECCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define ECCP_1_SEL_TMR12            0b00000000 
// ECCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define ECCP_1_SEL_TMR34            0b00010000 
// ECCP : TIMER3 -> Captura, Comparación TIMER6 -> PWM
#define ECCP_1_SEL_TMR36            0b00100000 
// ECCP : TIMER3 -> Captura, Comparación TIMER8 -> PWM
#define ECCP_1_SEL_TMR38            0b00110000 
#if defined (ECC_V8)
// ECCP : TIMER3 -> Captura, Comparación TIMER10 -> PWM
#define ECCP_1_SEL_TMR310           0b01000000 
// ECCP : TIMER3 -> Captura, Comparación TIMER12 -> PWM
#define ECCP_1_SEL_TMR312           0b01010000 
#endif

//*************ECCP2**************

// ECCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define ECCP_2_SEL_TMR12            0b00000000 
// ECCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define ECCP_2_SEL_TMR34            0b00010000 
// ECCP : TIMER3 -> Captura, Comparación TIMER6 -> PWM
#define ECCP_2_SEL_TMR36            0b00100000 
// ECCP : TIMER3 -> Captura, Comparación TIMER8 -> PWM
#define ECCP_2_SEL_TMR38            0b00110000 
#if defined (ECC_V8)
// ECCP : TIMER3 -> Captura, Comparación TIMER10 -> PWM
#define ECCP_2_SEL_TMR310           0b01000000 
#endif

//*************ECCP3**************

// ECCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define ECCP_3_SEL_TMR12            0b00000000 
// ECCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define ECCP_3_SEL_TMR34            0b00010000 
// ECCP : TIMER3 -> Captura, Comparación TIMER6 -> PWM
#define ECCP_3_SEL_TMR36            0b00100000 
// ECCP : TIMER3 -> Captura, Comparación TIMER8 -> PWM
#define ECCP_3_SEL_TMR38            0b00110000 

#endif

#if defined (CC_V8_5) || defined (ECC_V8_5)
// ECCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define ECCP_1_SEL_TMR12        0b00000000 
// ECCP : TIMER3 -> Captura, Comparación TIMER2 -> PWM
#define ECCP_1_SEL_TMR32        0b00010000 

// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_2_SEL_TMR12         0b00000000 
// CCP : TIMER3 -> Captura, Comparación TIMER2 -> PWM
#define CCP_2_SEL_TMR32         0b00010000 
#endif

#if defined (CC_V8_2) || defined (ECC_V8_2)
// ECCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define ECCP_1_SEL_TMR12        0b00000000 
// ECCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define ECCP_1_SEL_TMR34        0b00010000 

// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_2_SEL_TMR12         0b00000000 
// CCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define CCP_2_SEL_TMR34         0b00010000 
// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_3_SEL_TMR12         0b00000000 
// CCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define CCP_3_SEL_TMR34         0b00010000 
// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_4_SEL_TMR12         0b00000000 
// CCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define CCP_4_SEL_TMR34         0b00010000 
// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_5_SEL_TMR12         0b00000000 
// CCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define CCP_5_SEL_TMR34         0b00010000 

#endif

#if defined (CC_V9) || defined (ECC_V9) || defined (CC_V9_1) || defined (ECC_V9_1)
// ECCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define ECCP_1_SEL_TMR12        0b00000000 
// ECCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define ECCP_1_SEL_TMR34        0b00010000 
// ECCP : TIMER5 -> Captura, Comparación TIMER6 -> PWM
#define ECCP_1_SEL_TMR56        0b00100000 
// ECCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define ECCP_2_SEL_TMR12        0b00000000 
// ECCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define ECCP_2_SEL_TMR34        0b00010000 
// ECCP : TIMER5 -> Captura, Comparación TIMER6 -> PWM
#define ECCP_2_SEL_TMR56        0b00100000 
// ECCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define ECCP_3_SEL_TMR12        0b00000000 
// ECCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define ECCP_3_SEL_TMR34        0b00010000 
// ECCP : TIMER5 -> Captura, Comparación TIMER6 -> PWM
#define ECCP_3_SEL_TMR56        0b00100000 
// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_4_SEL_TMR12         0b00000000 
// CCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define CCP_4_SEL_TMR34         0b00010000 
// CCP : TIMER5 -> Captura, Comparación TIMER6 -> PWM
#define CCP_4_SEL_TMR56         0b00100000 
// CCP : TIMER1 -> Captura, Comparación TIMER2 -> PWM
#define CCP_5_SEL_TMR12         0b00000000 
// CCP : TIMER3 -> Captura, Comparación TIMER4 -> PWM
#define CCP_5_SEL_TMR34         0b00010000 
// CCP : TIMER5 -> Captura, Comparación TIMER6 -> PWM
#define CCP_5_SEL_TMR56         0b00100000 

#endif

/*
    Prototipos de funciones
*/

#if defined (CC_V1) || defined (CC_V2) || defined (CC_V3) || defined (CC_V4) || defined (CC_V5) || defined (CC_V6) || defined (CC_V7)
void compare1_init(uint8_t param_config);
void compare1_setPeriod(uint16_t period);

#endif


#if defined (CC_V2 ) || defined ( CC_V3 ) || defined ( CC_V4 ) ||\
    defined (CC_V6) || defined (CC_V7) || defined (CC_V8_2) || defined (CC_V8_5)
void compare2_init(uint8_t param_config);
void compare2_setPeriod(uint16_t period);
#endif

#if defined (CC_V3 ) || defined (CC_V4) || defined (CC_V8_2)
void compare3_init(uint8_t param_config);
void compare3_setPeriod(uint16_t period);
#endif

#if defined (CC_V4) || defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_2)\
    || defined (CC_V8_3)|| defined (CC_V8_4) || defined (CC_V9) || defined (CC_V9_1)
void compare4_init(uint8_t param_config);
void compare4_setPeriod(uint16_t period);

void compare5_init(uint8_t param_config);
void compare5_setPeriod(uint16_t period);
#endif

#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3)|| defined (CC_V8_4)
void compare6_init(uint8_t param_config);
void compare6_setPeriod(uint16_t period);

void compare7_init(uint8_t param_config);
void compare7_setPeriod(uint16_t period);

void compare8_init(uint8_t param_config);
void compare8_setPeriod(uint16_t period);

#if defined (CC_V8) || defined (CC_V8_3)
void compare9_init(uint8_t param_config);
void compare9_setPeriod(uint16_t period);

void compare10_init(uint8_t param_config);
void compare10_setPeriod(uint16_t period);
#endif

#endif

#if defined (ECC_V5) || defined (ECC_V8) || defined (ECC_V8_1) ||\
 defined (ECC_V8_2) || defined (ECC_V8_3)|| defined (ECC_V8_4) || defined (ECC_V8_5) || \
 defined (ECC_V9) || defined (ECC_V9_1)
void ecompare1_init(uint8_t param_config);
void ecompare1_setPeriod(uint16_t period);
#endif

#if defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_3)|| \
    defined (ECC_V8_4)|| defined (ECC_V9) || defined (ECC_V9_1)
void ecompare2_init(uint8_t param_config);
void ecompare2_setPeriod(uint16_t period);

void ecompare3_init(uint8_t param_config);
void ecompare3_setPeriod(uint16_t period);
#endif

/*
    Macros de definición de E/S para módulo CCP
*/

#if defined CC1_IO_V1   
#define CM1_TRIS    TRISBbits.TRISB3 
#elif defined CC1_IO_V2
#define CM1_TRIS    TRISCbits.TRISC5 
#elif defined CC8_IO_V1
#define CM1_TRIS    TRISBbits.TRISB4
#elif defined CC8_IO_V2
#define CM1_TRIS    TRISDbits.TRISD4
#elif defined CC9_IO_V1
#define CM1_TRIS    TRISCbits.TRISC2
#else
#define CM1_TRIS    TRISCbits.TRISC2 
#endif

#if defined CC8_IO_V1
#define CM2_TRIS    TRISCbits.TRISC2
#elif defined CC8_IO_V2
#define CM2_TRIS    TRISCbits.TRISC2
#elif defined CC9_IO_V1
#define CM2_TRIS    TRISBbits.TRISB3
#else
#define CM2_TRIS    TRISEbits.TRISE7
#endif

#if defined CC3_IO_V1   
#define CM3_TRIS    TRISDbits.TRISD1 
#elif defined CC8_IO_V1
#define CM3_TRIS    TRISCbits.TRISC6
#elif defined CC8_IO_V2
#define CM3_TRIS    TRISCbits.TRISC6
#elif defined CC9_IO_V1
#define CM3_TRIS    TRISBbits.TRISB5
#else   
#define CM3_TRIS    TRISGbits.TRISG0 
#endif

#if defined CC4_IO_V1
#define CM4_TRIS    TRISDbits.TRISD2
#elif defined CC4_IO_V2
#define CM4_TRIS    TRISBbits.TRISB4
#elif defined CC8_IO_V1
#define CM4_TRIS    TRISCbits.TRISC7
#elif defined CC8_IO_V2
#define CM4_TRIS    TRISCbits.TRISC7
#elif defined CC9_IO_V1
#define CM4_TRIS    TRISBbits.TRISB0
#else
#define CM4_TRIS    TRISGbits.TRISG3
#endif

#if defined CC4_IO_V2   
#define CM5_TRIS    TRISBbits.TRISB5
#elif defined CC8_IO_V1
#define CM5_TRIS    TRISBbits.TRISB5
#elif defined CC8_IO_V2
#define CM5_TRIS    TRISBbits.TRISB5
#elif defined CC9_IO_V1
#define CM5_TRIS    TRISAbits.TRISA4
#else   
#define CM5_TRIS    TRISGbits.TRISG4
#endif

#if defined CC4_IO_V2   
#define CM6_TRIS    TRISBbits.TRISB6
#else   
#define CM6_TRIS    TRISEbits.TRISE6
#endif

#if defined CC4_IO_V2   
#define CM7_TRIS    TRISBbits.TRISB7
#else   
#define CM7_TRIS    TRISEbits.TRISE5
#endif

#if defined CC4_IO_V2   
#define CM8_TRIS    TRISCbits.TRISC1
#else   
#define CM8_TRIS    TRISEbits.TRISE4
#endif

#if defined CC4_IO_V2   
#define CM9_TRIS    TRISCbits.TRISC6
#else   
#define CM9_TRIS    TRISEbits.TRISE3
#endif

#if defined CC4_IO_V2   
#define CM10_TRIS    TRISCbits.TRISC7
#else   
#define CM10_TRIS   TRISEbits.TRISE2
#endif

#if defined  ECCP1_IO_V5 
#define ECCP1CON CCP1CON
#define ECCP1IF  CCP1IF
#define ECCP1IE  CCP1IE
#endif


#endif /*COMPARE_H*/