/*
* Funciones para manejo de módulos CCP en modo PWM en microcontroladores PIC de 8 bits
* Autor: Ing. José Roberto Parra Trewartha
* Compilador: XC8
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "pconfig.h"
#include "funciones_auxiliares.h"
#include "timers.h"
#include "flash.h"
#include "pwm.h"

/*
* Inicialización de módulo CCP en modo PWM
* Parámetros: 
* param_config_timer: Parámetros de configuración del timer asociado
* periodo: Registro PRx de periodo del timer asociado (1,4,6,8,10 o 12)
* Retorno: 
* Vacío (void)
* Nota: tener cuidado con la elección previa de timer asociado si así se requiere
*/

/*
    PWM1
*/
#if  defined (PWM_V1) || defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V5) ||\
     defined (PWM_V6) || defined (PWM_V7) || defined (EPWM_V7)

void pwm1_init(uint8_t param_config_timer, uint8_t periodo)
{
    CCP1CON = CCP_PWM;                          //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    TRISCbits.TRISC2 = 0;                       //Pin PWM se configura como salida
    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
}

#elif  defined (PWM_V8) 
void pwm1_init(uint8_t param_config_timer, uint8_t periodo)
{
    CCP1CON = CCP_PWM;                             //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    TRISBbits.TRISB3 = 0;                       //Pin PWM se configura como salida
    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
}

#elif  defined (PWM_V4) || defined (PWM_V9) 
void pwm1_init(uint8_t param_config_timer, uint8_t periodo)
{
    CCP1CON = CCP_PWM;                          //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    TRISCbits.TRISC2 = 0;                       //Pin PWM se configura como salida
    if( T3CONbits.T3CCP2 && T3CONbits.T3CCP1 )
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }
    else
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
}

#elif   defined (PWM_V10) || defined (PWM_V12)
void pwm1_init(uint8_t param_config_timer, uint8_t periodo)
{
    CCP1CON = CCP_PWM;                          //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    if(PSTRCONbits.STRA)
    {
        #if defined PWM1_IO_V6
        TRISCbits.TRISC5 = 0;
        #else
        TRISCbits.TRISC2 = 0;
        #endif
    }
    if(PSTRCONbits.STRB)
    {
        #if defined PWM1_IO_V6
        TRISCbits.TRISC4 = 0;
        #else
        TRISBbits.TRISB2 = 0;  // P1B
        #endif
        ANSELHbits.ANS8 = 0;   // disable P1B analog
    }
    if(PSTRCONbits.STRC)
    {
       #if defined PWM1_IO_V6
       TRISCbits.TRISC3 = 0;
       #else
       TRISBbits.TRISB1 = 0;  // P1C
       #endif
       ANSELHbits.ANS10 = 0;  // disable P1C analog
    }
    if(PSTRCONbits.STRD)
    { 
       #if defined PWM1_IO_V6
       TRISCbits.TRISC2 = 0;
       #else
       TRISBbits.TRISB4 = 0;  // P1D
       #endif
       ANSELHbits.ANS11 = 0;  // disable P1D analog
    }
    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
}

#elif defined (PWM_V11)
void pwm1_init(uint8_t param_config_timer, uint8_t periodo)
{
    CCP1CON = CCP_PWM;                          //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    if(PSTRCONbits.STRA) TRISCbits.TRISC2 = 0;  // P1A
    if(PSTRCONbits.STRB) TRISDbits.TRISD5 = 0;  // P1B
    if(PSTRCONbits.STRC) TRISDbits.TRISD6 = 0;  // P1C
    if(PSTRCONbits.STRD) TRISDbits.TRISD7 = 0;  // P1D
    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
}

#elif defined (PWM_V13)
void pwm1_init(uint8_t param_config_timer, uint8_t periodo)
{
    CCP1CON = CCP_PWM;                          //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
}
#endif

/*
    PWM2
*/
#if defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V5) || defined (PWM_V6) || defined (PWM_V10) ||  defined (PWM_V11) 
void pwm2_init(uint8_t param_config_timer, uint8_t periodo)
{
    bool pmmode; /* Se establecerá en alto si el bit 0 Y el bit 1 o el bit 5 Y el bit 4 del registro __CONFIG3L están en alto*/
    bool ccp2mx; /* Se establecerá en alto si el bit 0 del registro __CONFIG3H está en alto*/
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;

    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    CCP2CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    if (flash_readByte(__CONFIG3H) & 0x01)
        ccp2mx= true;
    else
        ccp2mx = false;

    #if defined PWM_CONFIG3L_V1
    if ( ((flash_readByte(__CONFIG3L) & 0x03)) == 0x03)
        pmmode=true;
    else
        pmmode = false;
    #elif defined PWM_CONFIG3L_V2
    if ( ((flash_readByte(__CONFIG3L) & 0x30)) == 0x30)
        pmmode=true;
    else
        pmmode = false;
    #endif

    #if defined CCP2_V0
    TRISCbits.TRISC1 = 0;
    #elif defined PWM2_IO_V1
    if(ccp2mx)  
        TRISCbits.TRISC1 = 0;
    else        
        TRISBbits.TRISB3 = 0;
    #elif defined PWM2_IO_V7
    TRISBbits.TRISB3 = 0;
    #elif defined PWM2_IO_V8
    TRISEbits.TRISE7 = 0;
    #elif defined PWM2_IO_V2
    if(ccp2mx)
        TRISCbits.TRISC1 = 0;
    else        
        TRISEbits.TRISE7 = 0;
    #elif defined PWM2_IO_V4
    if(ccp2mx)  
        TRISCbits.TRISC1 = 0;
    else if(pmmode)     
        TRISEbits.TRISE7 = 0;       // Modo Microcontrolador
    else                
        TRISBbits.TRISB3 = 0;
    #endif

    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2

    #ifndef _OMNI_CODE_     
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm
    #endif
}

#elif  defined (PWM_V4) || defined (PWM_V9) 
void pwm2_init(uint8_t param_config_timer, uint8_t periodo)
{
    bool pmmode; /* Se establecerá en alto si el bit 0 Y el bit 1 o el bit 5 Y el bit 4 del registro __CONFIG3L están en alto*/
    bool ccp2mx; /* Se establecerá en alto si el bit 0 del registro __CONFIG3H está en alto*/
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;

    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    CCP2CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    if (flash_readByte(__CONFIG3H) & 0x01)
        ccp2mx= true;
    else
        ccp2mx = false;

    #if defined PWM_CONFIG3L_V1
    if ( ((flash_readByte(__CONFIG3L) & 0x03)) == 0x03)
        pmmode=true;
    else
        pmmode = false;
    #elif defined PWM_CONFIG3L_V2
    if ( ((flash_readByte(__CONFIG3L) & 0x30)) == 0x30)
        pmmode=true;
    else
        pmmode = false;
    #endif

    #if defined PWM2_IO_V1
    if(ccp2mx)  TRISCbits.TRISC1 = 0;
    else        TRISBbits.TRISB3 = 0;   
    #elif defined PWM2_IO_V7
    TRISBbits.TRISB3 = 0;
    #elif defined PWM2_IO_V8
    TRISEbits.TRISE7 = 0;
    #elif defined PWM2_IO_V2
    if(ccp2mx)  TRISCbits.TRISC1 = 0;
    else        TRISEbits.TRISE7 = 0;
    #elif defined PWM2_IO_V4
    if(ccp2mx)  TRISCbits.TRISC1 = 0;
    else if(pmmode)     TRISEbits.TRISE7 = 0;       // Modo microcontrolador
    else                TRISBbits.TRISB3 = 0;
    #else
    TRISCbits.TRISC1 = 0;
    #endif

    if (T3CONbits.T3CCP1 == 0)
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
    else
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }


    #ifndef _OMNI_CODE_     
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm
    #endif
}

#elif defined (PWM_V13)
void pwm2_init(uint8_t param_config_timer, uint8_t periodo)
{
    CCP2CON = CCP_PWM;                          //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
}

#elif defined (PWM_V14_2)
void pwm2_init ( uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP2CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= ((timer_source & 0x10)>>3); 

    PWM2_TRIS = 0;

    if( (CCPTMRS & 0x02))
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }  
    else
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
}

#elif defined (PWM_V14_5)
void pwm2_init ( uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP2CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= ((timer_source & 0x01)>>1); 

    PWM2_TRIS = 0;

    if( (CCPTMRS & 0x08))
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
}
#endif

/*
    PWM3
*/
#if defined (PWM_V3) 
void pwm3_init(uint8_t param_config_timer, uint8_t periodo);
{
    CCP3CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
}

#elif defined (PWM_V4) 
void pwm3_init(uint8_t param_config_timer, uint8_t periodo);
{
    CCP3CON= CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    if (T3CONbits.T3CCP2 == 0 && T3CONbits.T3CCP1 == 0)
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
    else
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }
}

#elif defined (PWM_V9) 
void pwm3_init(uint8_t param_config_timer, uint8_t periodo);
{
    CCP3CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    TRISGbits.TRISG0=0;

    if (T3CONbits.T3CCP2 == 0 && T3CONbits.T3CCP1 == 0)
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
    else
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }
}
#elif defined (PWM_V14_2)
void pwm3_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source); 
{
    CCP3CON= CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= ((timer_source & 0x10)>>2); 

    PWM3_TRIS = 0;

    if( (CCPTMRS & 0x04))
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }  
    else
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
 
}
#endif


/*
    PWM4
*/
#if defined (PWM_V4) || defined (PWM_V9)  
void pwm4_init(uint8_t param_config_timer, uint8_t periodo)
{
    CCP4CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    #if defined PWM4_IO_V1
    TRISDbits.TRISD2=0;    //configure pin portd,2 an output
    #else
    TRISGbits.TRISG3=0;    //configure pin portg,3 an output
    #endif 

    if (T3CONbits.T3CCP2 == 0 && T3CONbits.T3CCP1 == 0)
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
    else
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }
}  
#elif defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm4_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP4CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS1 &= 0xFC;
    CCPTMRS1 |= ((timer_source & 0x30)>>4);   

    PWM4_TRIS = 0;

    uint8_t CCPTMRS1_aux = CCPTMRS1 & 0x03;

    switch(CCPTMRS1_aux)
    {
        case 0x00:
            timer2_init(param_config_timer,periodo);    //Configuración de timer 2
            T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
            break;
        case 0x01:
            timer4_init(param_config_timer,periodo);    //Configuración de timer 4
            T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
            break;
        case 0x02:
            timer6_init(param_config_timer,periodo);    //Configuración de timer 6
            T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6
            break;
        default: break;
    }    
}
#elif defined (PWM_V14_2)
void pwm4_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP4CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= ((timer_source & 0x10)>>1); 

    PWM4_TRIS = 0;

    if( (CCPTMRS & 0x08))
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }  
    else
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
}
#elif defined (PWM_V15) || defined (PWM_V15_1)
void pwm4_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP4CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS1 &= 0xFC;
    CCPTMRS1 |= ((timer_source & 0x30)>>4);    

    PWM4_TRIS = 0;


    uint8_t CCPTMRS1_aux = CCPTMRS1 & 0x03;

    switch(CCPTMRS1_aux)
    {
        case 0x00:
            timer2_init(param_config_timer,periodo);    //Configuración de timer 2
            T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
            break;
        case 0x01:
            timer4_init(param_config_timer,periodo);    //Configuración de timer 4
            T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
            break;
        case 0x02:
            timer6_init(param_config_timer,periodo);    //Configuración de timer 6
            T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6
            break;
        default: break;
    }  
}
#endif

/*
    PWM5
*/
#if defined (PWM_V4) || defined (PWM_V9)
void pwm5_init(uint8_t param_config_timer, uint8_t periodo)
{
    CCP5CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    if (T3CONbits.T3CCP2 == 0 && T3CONbits.T3CCP1 == 0)
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
    else
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }
    TRISGbits.TRISG4=0;    //configure pin portg,4 an output

}
#elif defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm5_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP5CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS1 &= 0xFB;
    CCPTMRS1 |= (( timer_source & 0x10)>>2);   

    PWM5_TRIS = 0;

    uint8_t CCPTMRS1_aux = CCPTMRS1 & 0x04;

    if( CCPTMRS1_aux == 0x00)
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
    else if( CCPTMRS1_aux == 0x04)
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }
}
#elif defined (PWM_V14_2)
void pwm5_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP5CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= (( timer_source & 0x10));  

    PWM5_TRIS = 0;

    if( (CCPTMRS&0x10))
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }
    else
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
}
#elif defined (PWM_V15) ||defined (PWM_V15_1)

void pwm5_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP5CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS1 &= 0xF3;
    CCPTMRS1 |= ((timer_source & 0x30)>>2); 

    PWM5_TRIS = 0;

    uint8_t CCPTMRS1_aux = CCPTMRS1 & 0x0C;

    switch(CCPTMRS1_aux)
    {  
        case 0x00:
            timer2_init(param_config_timer,periodo);    //Configuración de timer 2
            T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
            break;
        case 0x04:
            timer4_init(param_config_timer,periodo);    //Configuración de timer 4
            T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
            break;
        case 0x08:
            timer6_init(param_config_timer,periodo);    //Configuración de timer 6
            T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6
            break;
        default: break;

    }
}
#endif

/*
    PWM6
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm6_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;
    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    CCP6CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS1 &= 0xEF;
    CCPTMRS1 |= ( timer_source & 0x10);  

    #if defined (PWM_V14_IO_V1)  || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1)
    if (flash_readByte(__CONFIG3H) & 0x02)
        TRISEbits.TRISE6 = 0;
    #ifndef PWM_V15_2_IO_V1
    else
        TRISHbits.TRISH7 = 0;
    #endif     
    #else
    PWM6_TRIS = 0;
    #endif

    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
} 
#endif

/*
    PWM7
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm7_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;
    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    CCP7CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS1 &= 0x3F;
    CCPTMRS1 |= (( timer_source & 0x30)<<2); 

    #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1)
    if (flash_readByte(__CONFIG3H) & 0x02)
        TRISEbits.TRISE5 = 0;
    #ifndef PWM_V15_2_IO_V1
    else
        TRISHbits.TRISH6 = 0;
    #endif     
    #else
    PWM7_TRIS = 0;
    #endif

    uint8_t CCPTMRS1_aux = CCPTMRS1 & 0xC0;

    switch(CCPTMRS1_aux)
    {
        case 0x00:
            timer2_init(param_config_timer,periodo);    //Configuración de timer 2
            T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
            break;
        case 0x40:
            timer4_init(param_config_timer,periodo);    //Configuración de timer 4
            T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
        break;
        case 0x80:
            timer6_init(param_config_timer,periodo);    //Configuración de timer 6
            T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6
        break;
        case 0xC0:
            timer8_init(param_config_timer,periodo);    //Configuración de timer 8
            T8CONbits.TMR8ON = 1;                       //Habilitación manual de timer 8
        break;
        default: break;
    }    
}
#endif

/*
    PWM8
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm8_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;
    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    CCP8CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS2 &= 0xFC;
    CCPTMRS2 |= (( timer_source & 0x30)>>4);

    #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1)
    if (flash_readByte(__CONFIG3H) & 0x02)
        TRISEbits.TRISE4 = 0;
    #ifndef PWM_V15_2_IO_V1
    else
        TRISHbits.TRISH5 = 0;
    #endif     
    #else
    PWM8_TRIS = 0;
    #endif

    uint8_t CCPTMRS2_aux = CCPTMRS2 & 0x03;

    switch(CCPTMRS2_aux)
    {
        case 0x00:
            timer2_init(param_config_timer,periodo);    //Configuración de timer 2
            T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
            break;
        case 0x01:
            timer4_init(param_config_timer,periodo);    //Configuración de timer 4
            T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
        break;
        case 0x02:
            timer6_init(param_config_timer,periodo);    //Configuración de timer 6
            T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6
        break;
        default: break;
    }    
}
#endif

/*
    PWM9
*/
#if defined (PWM_V14) || defined (PWM_V14_3)
void pwm9_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;

    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    CCP9CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS2 &= 0xFB;
    CCPTMRS2 |= (( timer_source & 0x10)>>2);  

    #if defined (PWM_V14_IO_V1)  || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1)
    if (flash_readByte(__CONFIG3H) & 0x02)
        TRISEbits.TRISE3 = 0;
    #ifndef PWM_V15_2_IO_V1
    else
        TRISHbits.TRISH4 = 0;
    #endif 
    #else
    PWM9_TRIS = 0;
    #endif
    uint8_t CCPTMRS2_aux = CCPTMRS2 & 0x04;
    if( CCPTMRS2_aux == 0x00)
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }

    else if( CCPTMRS2_aux == 0x04)
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }
}
#endif

/*
    PWM10
*/
#if defined (PWM_V14) ||defined (PWM_V14_3) 
void pwm10_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{  
    CCP10CON = CCP_PWM;    //Inicialización del registro CCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para CCP
    CCPTMRS2 &= 0xEF;
    CCPTMRS2 |= ( timer_source & 0x10); 

    PWM10_TRIS = 0;

    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
}
#endif

/*
* Inicialización de módulo ECCP en modo PWM
* Parámetros: 
* param_config_timer: Parámetros de configuración del timer asociado
* periodo: Registro PRx de periodo del timer asociado (1,4,6,8,10 o 12)
* timer_source: Parámetro de timer asociado
* Retorno: 
* Vacío (void)
*/

/*
    EPWM1
*/
#if defined (EPWM_V7)
void epwm1_init(uint8_t param_config_timer, uint8_t periodo)
{
    ECCP1CON = CCP_PWM;                          //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
}

#elif defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4)
void epwm1_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    #if defined (EPWM_V14_3) || defined (EPWM_V14_4)
    CCP1CON= CCP_PWM;   //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    #else
    ECCP1CON= CCP_PWM;   //Inicialización del registro ECCPCON correspondiente para modo PWM sencillo
    #endif

    //Configuración de timer fuente para ECCP
    CCPTMRS0 &= 0xFC;
    CCPTMRS0 |= ((timer_source & 0x18)>>3);  
      
    if( (CCPTMRS0&0x07)==0x00)
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
      
    else if( (CCPTMRS0&0x07)==0x01)
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }
      
    else if( (CCPTMRS0&0x07)==0x02)
    {
        timer6_init(param_config_timer,periodo);    //Configuración de timer 6
        T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6
    }
     
    else if( (CCPTMRS0&0x07)==0x03)
    {
        timer8_init(param_config_timer,periodo);    //Configuración de timer 8
        T8CONbits.TMR8ON = 1;                       //Habilitación manual de timer 8
    }
      
    #if defined (EPWM_V14)
    else if( (CCPTMRS0&0x07)==0x04)
    {
        timer10_init(param_config_timer,periodo);    //Configuración de timer 10
        T10CONbits.TMR10ON = 1;                      //Habilitación manual de timer 10
    }  
    else if( (CCPTMRS0&0x07)==0x05)
    {
        timer12_init(param_config_timer,periodo);   //Configuración de timer 12
        T12CONbits.TMR12ON = 1;                     //Habilitación manual de timer 12
    }
    #endif  

}

#elif defined (EPWM_V14_2)
void epwm1_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    ECCP1CON= CCP_PWM;   //Inicialización del registro ECCPCON correspondiente para modo PWM sencillo
    //Configuración de timer fuente para ECCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= ((timer_source & 0x10)>>4); 

    if( (CCPTMRS & 0x01))
    {
        timer4_init(param_config_timer,periodo);    //Configuración de timer 4
        T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
    }
    else
    {
        timer2_init(param_config_timer,periodo);    //Configuración de timer 2
        T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
    }
}

#elif defined (EPWM_V14_5)
void epwm1_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP1CON= CCP_PWM;   //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    //Configuración de timer fuente para ECCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= ((timer_source & 0x10)>>4); 
    timer2_init(param_config_timer,periodo);    //Configuración de timer 2
    T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
}

#elif defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm1_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP1CON= CCP_PWM;   //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    //Configuración de timer fuente para ECCP
    CCPTMRS0 &= 0xFC;
    CCPTMRS0 |= ((timer_source & 0x30)>>4);  

    uint8_t CCPTMRS0_aux = CCPTMRS0 & 0x03;

    switch(CCPTMRS0_aux)
    {
        case 0x00:
            timer2_init(param_config_timer,periodo);    //Configuración de timer 2
            T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
            break;
        case 0x01:
            timer4_init(param_config_timer,periodo);    //Configuración de timer 4
            T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
            break;
        case 0x02:
            timer6_init(param_config_timer,periodo);    //Configuración de timer 6
            T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6
            break;
        default: break;
    }
}
#endif

/*
    EPWM2
*/
#if defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4)
void epwm2_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    #if defined (EPWM_V14_3) || defined (EPWM_V14_4)
    CCP2CON = CCP_PWM;   //Inicialización del registro CCPCON correspondiente para modo PWM sencillo
    #else
    ECCP2CON = CCP_PWM;   //Inicialización del registro ECCPCON correspondiente para modo PWM sencillo
    #endif

    //Configuración de timer fuente para ECCP
    CCPTMRS0 &= 0xC7;
    CCPTMRS0 |= ((timer_source & 0x70)>>1);

    uint8_t CCPTMRS0_aux = CCPTMRS0 & 0x38;

    switch(CCPTMRS0_aux)
    {
        case 0x00:
            timer2_init(param_config_timer,periodo);    //Configuración de timer 2
            T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
            break;
        case 0x08:
            timer4_init(param_config_timer,periodo);    //Configuración de timer 4
            T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
            break;
        case 0x10:
            timer6_init(param_config_timer,periodo);    //Configuración de timer 6
            T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6 
            break;
        case 0x18:
            timer8_init(param_config_timer,periodo);    //Configuración de timer 8
            T8CONbits.TMR8ON = 1;                       //Habilitación manual de timer 8
            break;
        #if defined (EPWM_V14)
        case 0x20:
            timer10_init(param_config_timer,periodo);    //Configuración de timer 10
            T10CONbits.TMR10ON = 1;                       //Habilitación manual de timer 10
            break;
        #endif
        default: break; 
    } 
}
#elif defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm2_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    ECCP2CON = CCP_PWM;   //Inicialización del registro ECCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para ECCP
    CCPTMRS0 &= 0xE7;
    CCPTMRS0 |= ((timer_source & 0x30)>>1);
    uint8_t CCPTMRS0_aux = CCPTMRS0 & 0x18;

    switch(CCPTMRS0_aux)
    {
        case 0x00:
            timer2_init(param_config_timer,periodo);    //Configuración de timer 2
            T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
            break;
        case 0x08:
            timer4_init(param_config_timer,periodo);    //Configuración de timer 4
            T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
            break;
        case 0x10:
            timer6_init(param_config_timer,periodo);    //Configuración de timer 6
            T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6
            break;
        default: break;
    }
}
#endif

/*
    EPWM3
*/
#if defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4)
void epwm3_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP3CON = CCP_PWM;   //Inicialización del registro ECCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para ECCP
    CCPTMRS0 &= 0x3F;
    CCPTMRS0 |= ((timer_source & 0x30)<<2); 

    uint8_t CCPTMRS0_aux = CCPTMRS0 & 0xC0;

    switch(CCPTMRS0_aux)
    {
        case 0x00:
            timer2_init(param_config_timer,periodo);    //Configuración de timer 2
            T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
            break;
        case 0x40:
            timer4_init(param_config_timer,periodo);    //Configuración de timer 4
            T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
            break;
        case 0x80:
            timer6_init(param_config_timer,periodo);    //Configuración de timer 6
            T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6
            break;
        case 0xC0:
            timer8_init(param_config_timer,periodo);    //Configuración de timer 8
            T8CONbits.TMR8ON = 1;                       //Habilitación manual de timer 8
            break;
        default: break;
    }
}
#elif defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm3_init(uint8_t param_config_timer, uint8_t periodo, uint8_t timer_source)
{
    CCP3CON = CCP_PWM;   //Inicialización del registro ECCPCON correspondiente para modo PWM sencillo

    //Configuración de timer fuente para ECCP
    CCPTMRS0 &= 0x3F;
    CCPTMRS0 |= ((timer_source & 0x30)<<2); 

    uint8_t CCPTMRS0_aux = CCPTMRS0 & 0xC0;

    switch(CCPTMRS0_aux)
    {
        case 0x00:
            timer2_init(param_config_timer,periodo);    //Configuración de timer 2
            T2CONbits.TMR2ON = 1;                       //Habilitación manual de timer 2
            break;
        case 0x40:
            timer4_init(param_config_timer,periodo);    //Configuración de timer 4
            T4CONbits.TMR4ON = 1;                       //Habilitación manual de timer 4
            break;
        case 0x80:
            timer6_init(param_config_timer,periodo);    //Configuración de timer 6
            T6CONbits.TMR6ON = 1;                       //Habilitación manual de timer 6
            break;
        default: break;
    }
}
#endif

/*
* Inicialización de salidas digitales CCP para módulo PWM
* Parámetros: 
* output_config: Parámetros de configuración de salidas PWM
* output_mode: Parámetros de configuración de modo PWM
* Retorno: 
* Vacío (void)
*/

/*
    PWM1
*/
#if defined (PWM_V1) || defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V4) ||\
    defined (PWM_V5) || defined (PWM_V6) || defined (PWM_V7) || defined (EPWM_V7) ||\
    defined (PWM_V9) ||  defined (PWM_V10) ||  defined (PWM_V11) ||  defined (PWM_V12) 
void pwm1_setOutput(uint8_t output_config, uint8_t output_mode)
{
    bool eccpmx;    //Bit de multiplexación de módulo CCP1, ubicado en __CONFIG3H
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;
    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;
    CCP1CON = (CCP1CON | 0xC0) & output_config_aux;  //Bits de configuración de salida Enhanced PWM
    CCP1CON = (CCP1CON | 0x0F) & output_mode_aux;    //Bits de configuración de selección de modo CCP
    //Comprobación de bit de configuración de multiplexación de módulo CCP1
    
    
    
    #if defined (PWM_CONFIG3L_V1) || defined (PWM_CONFIG3L_V2)
        #if defined (PWM1_IO_V4)
    if ( flash_readByte(__CONFIG3H) & 0x02)
        eccpmx = true;
    else
        eccpmx = false;
        #endif
    #endif

    #if defined PWM1_IO_V1  
    if (output_config == PWM_SINGLE_OUT)         //Modo PWM sencillo 
    {
            TRISCbits.TRISC2 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
            TRISCbits.TRISC2 = 0;
            TRISDbits.TRISD5 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
            TRISCbits.TRISC2 = 0;
            TRISDbits.TRISD5 = 0;
            TRISDbits.TRISD6 = 0;
            TRISDbits.TRISD7 = 0;
    }
    #elif defined PWM1_IO_V2 
    if (output_config == PWM_SINGLE_OUT)
    {
            TRISCbits.TRISC2 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
            TRISCbits.TRISC2 = 0;
            TRISEbits.TRISE6 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
            TRISCbits.TRISC2 = 0;
            TRISEbits.TRISE6 = 0;
            TRISEbits.TRISE5 = 0;
            TRISGbits.TRISG4 = 0;
    }
    #elif defined PWM1_IO_V3 
    if (output_config == PWM_SINGLE_OUT)
    {
            TRISCbits.TRISC2 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
            TRISCbits.TRISC2 = 0;
            TRISDbits.TRISD0 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
            TRISCbits.TRISC2 = 0;
            TRISDbits.TRISD0 = 0;
            TRISEbits.TRISE5 = 0;
            TRISGbits.TRISG4 = 0;
    }
    
    #elif defined PWM1_IO_V4
    if (output_config == PWM_SINGLE_OUT)
    {
            TRISCbits.TRISC2 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
            TRISCbits.TRISC2 = 0;
        if(eccpmx)
            TRISEbits.TRISE6 = 0;
        else
             TRISHbits.TRISH7 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
            TRISCbits.TRISC2 = 0;
        if(eccpmx)  {TRISEbits.TRISE6 = 0;TRISEbits.TRISE5 = 0;}
        else        {TRISHbits.TRISH7 = 0;TRISHbits.TRISH6 = 0;}
            TRISGbits.TRISG4 = 0;
    }
    
    #elif defined PWM1_IO_V6
    if (output_config == PWM_SINGLE_OUT)
    {
            TRISCbits.TRISC5 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
            TRISCbits.TRISC5 = 0;
            TRISCbits.TRISC4 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
            TRISCbits.TRISC5 = 0;
            TRISCbits.TRISC4 = 0;
            TRISCbits.TRISC3 = 0;
            TRISCbits.TRISC2 = 0;

    }   

    #endif
    #ifndef _OMNI_CODE_
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm
    #endif
}

#elif defined (PWM_V8) 
void pwm1_setOutput(uint8_t output_config, uint8_t output_mode)
{
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;
    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;
    CCP1CON = (CCP1CON | 0xC0) & output_config_aux;  //Bits de configuración de salida Enhanced PWM
    CCP1CON = (CCP1CON | 0x0F) & output_mode_aux;    //Bits de configuración de selección de modo CCP
    if (output_config == PWM_SINGLE_OUT)
    {
        TRISBbits.TRISB3 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {
        TRISBbits.TRISB3 = 0;
        TRISBbits.TRISB2 = 0;   
    }
    else if (IS_QUAD_PWM(output_config))
    {
        TRISBbits.TRISB3 = 0;
        TRISBbits.TRISB2 = 0;
        TRISBbits.TRISB6 = 0;
        TRISBbits.TRISB7 = 0;   
    }   
    #ifndef _OMNI_CODE_ 
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm 
    #endif
}

#elif defined (PWM_V13)
void pwm1_setOutput(uint8_t output_config, uint8_t output_mode)
{
    //uint8_t output_config_aux = output_config;
    //uint8_t output_mode_aux = output_mode;
    output_config |= 0x3F;
    output_mode |= 0xFC;
    CCP1CON = (CCP1CON | 0xC0) & output_config;  //Bits de configuración de salida Enhanced PWM
    CCP1CON = (CCP1CON | 0x0F) & output_mode;    //Bits de configuración de selección de modo CCP
}
#endif


/*
    PWM2
*/
#if defined (PWM_V4)
void pwm2_setOutput(uint8_t output_config, uint8_t output_mode)
{
    uint8_t pmmode; //Se establecerá en alto si el bit 0 Y el bit 1 del registro __CONFIG3L están en alto
    uint8_t ccp2mx; //Se establecerá en alto si el bit 0 del registro __CONFIG3H está en alto
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;
    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    CCP2CON = (CCP2CON | 0xC0) & output_config;
    CCP2CON = (CCP2CON | 0x0F) & output_mode;

    #if defined (PWM_CONFIG3L_V1) || defined (PWM_CONFIG3L_V2)
    if ( flash_readByte(__CONFIG3H) & 0x01)
        ccp2mx = true;
    else
        ccp2mx = false;
    #endif

    #if defined PWM_CONFIG3L_V1
    if ((flash_readByte(__CONFIG3L) & 0x03) == 0x03)
        pmmode = true;
    else
        pmmode = false;
    #elif defined PWM_CONFIG3L_V2
    if ((flash_readByte(__CONFIG3L) & 0x30) == 0x30)
        pmmode = true;
    else
        pmmode = false;
    #endif

    #if defined PWM2_IO_V5

    if (SINGLE_OUT == output_config)
    {
        if(ccp2mx)  
            TRISCbits.TRISC1 = 0;
        else        
            TRISEbits.TRISE7 = 0; 
    }
    else if (IS_DUAL_PWM(output_config))
    {
        if(ccp2mx)  
            TRISCbits.TRISC1 = 0;
        else        
            TRISEbits.TRISE7 = 0;
        TRISEbits.TRISE2 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
        if(ccp2mx)  
            TRISCbits.TRISC1 = 0;
        else 
           TRISEbits.TRISE7 = 0;
        TRISEbits.TRISE2 = 0;
        TRISEbits.TRISE1 = 0;
        TRISEbits.TRISE0 = 0;
    }   
    #elif defined PWM2_IO_V3
    if (SINGLE_OUT == output_config)
    {
        TRISCbits.TRISC1 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
        TRISCbits.TRISC1 = 0;
        TRISEbits.TRISE2 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
        TRISCbits.TRISC1 = 0;
        TRISEbits.TRISE2 = 0;
        TRISEbits.TRISE1 = 0;
        TRISEbits.TRISE0 = 0;
    }
    #elif defined PWM2_IO_V6
    if (SINGLE_OUT == output_config)
    {
        if(ccp2mx)          
            TRISCbits.TRISC1 = 0;
        else if(pmmode)     
            TRISEbits.TRISE7 = 0; 
        else                
            TRISBbits.TRISB3 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {
        if(ccp2mx)          
            TRISCbits.TRISC1 = 0;
        else if(pmmode)     
            TRISEbits.TRISE7 = 0; 
        else                
            TRISBbits.TRISB3 = 0;
        TRISEbits.TRISE2 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
        if(ccp2mx)          
            TRISCbits.TRISC1 = 0;
        else if(pmmode)     
            TRISEbits.TRISE7 = 0; 
        else                
            TRISBbits.TRISB3 = 0;
        TRISEbits.TRISE2 = 0;
        TRISEbits.TRISE1 = 0;
        TRISEbits.TRISE0 = 0;
    }   
    #endif

    #ifndef _OMNI_CODE_
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm
    #endif    
}
#endif

/*
    PWM3
*/
#if defined (PWM_V4)
void pwm3_setOutput(uint8_t output_config, uint8_t output_mode)
{
    bool eccpmx;    //Bit de multiplexación de módulo ccp1, ubicado en __CONFIG3H
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;
    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    CCP3CON = (CCP3CON | 0xC0) & output_config;  //Bits de configuración de salida PWM
    CCP3CON = (CCP3CON | 0x0F) & output_mode;    //Bits de configuración de selección de modo CCP

    //Comprobación de bit de configuración de multiplexación de módulo CCP3
    #if defined (PWM1_IO_V4)
    if (flash_readByte(__CONFIG3H) & 0x02)
        eccpmx = true;
    else
        eccpmx = false;
    #endif
    
    #if defined PWM3_IO_V1
    if (SINGLE_OUT == output_config)
    {
        TRISDbits.TRISD1 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
        TRISDbits.TRISD1 = 0;
        TRISEbits.TRISE4 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
        TRISDbits.TRISD1 = 0;
        TRISEbits.TRISE4 = 0;
        TRISEbits.TRISE3 = 0;
        TRISDbits.TRISD2 = 0;
    }
    #elif defined PWM3_IO_V2
    if (SINGLE_OUT == output_config)
    {
        TRISGbits.TRISG0 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
        TRISGbits.TRISG0 = 0;
        TRISEbits.TRISE4 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
        TRISGbits.TRISG0 = 0;
        TRISEbits.TRISE4 = 0;
        TRISEbits.TRISE3 = 0;
        TRISGbits.TRISG3 = 0;
    }
    #elif defined PWM3_IO_V3
    if (SINGLE_OUT == output_config)
    {
        TRISGbits.TRISG0 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
        TRISGbits.TRISG0 = 0;
        if(eccpmx)  
            TRISEbits.TRISE4 = 0;
        else        
            TRISHbits.TRISH5 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
        TRISGbits.TRISG0 = 0;
        if(eccpmx)  
        {
            TRISEbits.TRISE4 = 0;
            TRISEbits.TRISE3 = 0;
        }
        else        
        {
            TRISHbits.TRISH5 = 0;
            TRISHbits.TRISH4 = 0;
        }
        TRISGbits.TRISG3 = 0;
    }
    #endif
    
    #ifndef _OMNI_CODE_
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm
    #endif
}
#endif


/*
    EPWM1
*/
#if defined (EPWM_V7)
void epwm1_setOutput(uint8_t output_config, uint8_t output_mode)
{
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;
    ECCP1CON = (ECCP1CON | 0xC0 ) & output_config_aux;   //Bits de configuración de salida Enhanced PWM
    ECCP1CON = (ECCP1CON | 0x0F ) & output_mode_aux;     //Bits de configuración de selección de modo CCP
    if(output_config == PWM_SINGLE_OUT)
    {
        TRISDbits.TRISD4 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {
        TRISDbits.TRISD4 = 0;
        TRISDbits.TRISD5 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
        TRISDbits.TRISD4 = 0;
        TRISDbits.TRISD5 = 0;
        TRISDbits.TRISD6 = 0;
        TRISDbits.TRISD7 = 0;
    }
}

#elif defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4)
void epwm1_setOutput(uint8_t output_config, uint8_t output_mode)
{
    bool eccpmx = 0;    //Bit de multiplexación de módulo ccp1, ubicado en __CONFIG3H
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;
    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif
            
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;   //Comprobación de bit de configuración de multiplexación de módulo CCP1
            
    #if defined (EPWM_V14_3) || defined (EPWM_V14_4)
    CCP1CON = (CCP1CON | 0xC0) & output_config_aux;  //Bits de configuración de salida Enhanced PWM
    CCP1CON = (CCP1CON | 0x0F) & output_mode_aux;    //Bits de configuración de selección de modo CCP
    #else
    ECCP1CON = (ECCP1CON | 0xC0 ) & output_config_aux;   //Bits de configuración de salida Enhanced PWM
    ECCP1CON = (ECCP1CON | 0x0F ) & output_mode_aux;     //Bits de configuración de selección de modo CCP
    #endif
    
    #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1) 
        #if defined (PWM1_IO_V4)
    if ( flash_readByte(__CONFIG3H) & 0x02)
        eccpmx = true;
    else
        eccpmx = false;
        #endif
    #endif
    if ( output_config == PWM_SINGLE_OUT)
    {
        TRISCbits.TRISC2 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
        TRISCbits.TRISC2 = 0;
    #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1)      
        if(eccpmx)
            TRISEbits.TRISE6 = 0;
        else
            TRISHbits.TRISH7 = 0;
    #elif defined (PWM_V15_2_IO_V1)
        if(eccpmx)
            TRISEbits.TRISE6 = 0;
    #elif defined (PWM_V14_IO_V2) 
            TRISEbits.TRISE6 = 0; 
    #endif          
    }
    else if (IS_QUAD_PWM(output_config))
    {
        TRISCbits.TRISC2 = 0;
    #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1)      
        if(eccpmx)  {TRISEbits.TRISE6 = 0;TRISEbits.TRISE5 = 0;}
        else        {TRISHbits.TRISH7 = 0;TRISHbits.TRISH6 = 0;}
        TRISGbits.TRISG4 = 0;
    #elif defined (PWM_V15_2_IO_V1)
        if(eccpmx)
            TRISEbits.TRISE6 = 0;
            TRISEbits.TRISE5 = 0;
            TRISGbits.TRISG4 = 0;       
    #elif defined (PWM_V14_IO_V2) 
            TRISEbits.TRISE6 = 0;
            TRISEbits.TRISE5 = 0;
            TRISGbits.TRISG4 = 0;
    #endif          
    }

    #ifndef _OMNI_CODE_
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm
    #endif
}

#elif defined (EPWM_V14_2)
void epwm1_setOutput(uint8_t output_config, uint8_t output_mode)
{
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;
    ECCP1CON = (ECCP1CON | 0xC0 ) & output_config_aux;   //Bits de configuración de salida Enhanced PWM
    ECCP1CON = (ECCP1CON | 0x0F ) & output_mode_aux;     //Bits de configuración de selección de modo CCP
    #if defined(PWM14_2_IO_V2)
    if (output_config == PWM_SINGLE_OUT)
    {
        TRISDbits.TRISD4 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {
        TRISDbits.TRISD4 = 0;
        TRISDbits.TRISD5 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
        TRISDbits.TRISD4 = 0;
        TRISDbits.TRISD5 = 0;
        TRISDbits.TRISD6 = 0;
        TRISDbits.TRISD7 = 0;
    }
    #elif defined(PWM14_2_IO_V1)
    if (output_config == PWM_SINGLE_OUT)
    {
        TRISBbits.TRISB4 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {
        TRISBbits.TRISB4 = 0;
        TRISBbits.TRISB1 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
        TRISBbits.TRISB4 = 0;
        TRISBbits.TRISB1 = 0;
        TRISBbits.TRISB2 = 0;
        TRISBbits.TRISB3 = 0;
    }
    #endif  
}

#elif defined (EPWM_V14_5)
void epwm1_setOutput(uint8_t output_config, uint8_t output_mode)
{
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;
    CCP1CON = (CCP1CON | 0xC0 ) & output_config_aux;   //Bits de configuración de salida Enhanced PWM
    CCP1CON = (CCP1CON | 0x0F ) & output_mode_aux;     //Bits de configuración de selección de modo CCP
    #if defined(PWM14_5_IO_V1) || defined(PWM14_5_IO_V2)
    if (SINGLE_OUT == output_config)
    {
        TRISCbits.TRISC2 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {
        TRISCbits.TRISC2 = 0;
        #if defined(PWM14_5_IO_V1)
        TRISDbits.TRISD5 = 0;
        #elif defined(PWM14_5_IO_V2)
        TRISBbits.TRISB2 = 0;
        #endif
    }
    else if (IS_QUAD_PWM(output_config))
    {
        TRISCbits.TRISC2 = 0;
        #if defined(PWM14_5_IO_V1)
        TRISDbits.TRISD5 = 0;
        TRISDbits.TRISD6 = 0;
        TRISDbits.TRISD7 = 0;
        #elif defined(PWM14_5_IO_V2)
        TRISBbits.TRISB2 = 0;
        TRISBbits.TRISB1 = 0;
        TRISBbits.TRISB4 = 0;
        #endif
    }
    #endif
}

#elif defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm1_setOutput(uint8_t output_config, uint8_t output_mode)
{
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;
    ECCP1CON = (ECCP1CON | 0xC0 ) & output_config_aux;   //Bits de configuración de salida Enhanced PWM
    ECCP1CON = (ECCP1CON | 0x0F ) & output_mode_aux;     //Bits de configuración de selección de modo CCP
    if (SINGLE_OUT == output_config)
    {
            TRISCbits.TRISC2 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {
        TRISCbits.TRISC2 = 0;
        TRISBbits.TRISB2 = 0;
    }
    else if (IS_QUAD_PWM(output_config))
    {
       TRISBbits.TRISB4 = 0;
       TRISBbits.TRISB1 = 0;
       TRISCbits.TRISC2 = 0;
       TRISBbits.TRISB2 = 0;
    }
}
#endif

/*
    EPWM2
*/
#if defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4)
void epwm2_setOutput(uint8_t output_config, uint8_t output_mode)
{
    bool eccpmx = false;    //Se establecerá en alto si el bit 1 del registro __CONFIG3H está en alto                
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;
    
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;

    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    /* set P1M1 and P1M0 */
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;

    #if defined (EPWM_V14_3) || defined (EPWM_V14_4)
    CCP2CON = (CCP2CON | 0xC0) & output_config_aux;
    /* set CCP1M3, CCP1M2, CCP1M1, CCP1M0 */
    CCP2CON = (CCP2CON | 0x0F) & output_mode_aux;
    #else
    ECCP2CON = (ECCP2CON | 0xC0) & output_config_aux;
    /* set CCP1M3, CCP1M2, CCP1M1, CCP1M0 */
    ECCP2CON = (ECCP2CON | 0x0F) & output_mode_aux;
    #endif


    #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1)
    if (flash_readByte(__CONFIG3H) & 0x01 )
        eccpmx = true;
    else
        eccpmx = false;
    #endif
    //--------------------------------------

    if (PWM_SINGLE_OUT == output_config)
    {
    #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1)         
        if(eccpmx)
            TRISCbits.TRISC1 = 0;
        else
            TRISEbits.TRISE7 = 0;
    #elif defined (PWM_V14_IO_V2)
        TRISEbits.TRISE7 = 0; 
    #endif      
    }
    else if (IS_DUAL_PWM(output_config))
    {               
        #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1)             
        if(eccpmx)
            TRISCbits.TRISC1 = 0;
        else
            TRISEbits.TRISE7 = 0;

        TRISEbits.TRISE2 = 0;
        #elif defined (PWM_V14_IO_V2)
        TRISEbits.TRISE7 = 0; 
        TRISEbits.TRISE2 = 0;       
        #endif      


    }
    else if (IS_QUAD_PWM(output_config))
    {
        #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1)         
        if(eccpmx)
            TRISCbits.TRISC1 = 0;
        else
            TRISEbits.TRISE7 = 0;
        TRISEbits.TRISE2 = 0;       
        TRISEbits.TRISE1 = 0;
        TRISEbits.TRISE0 = 0;            
        #elif defined (PWM_V14_IO_V2)
        TRISEbits.TRISE7 = 0;
        TRISEbits.TRISE2 = 0;       
        TRISEbits.TRISE1 = 0;
        TRISEbits.TRISE0 = 0;           
        #endif      

    }

    #ifndef _OMNI_CODE_
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm
    #endif
}

#elif defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm2_setOutput(uint8_t output_config, uint8_t output_mode)
{
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;
    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;

    /* set P1M1 and P1M0 */
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;

    ECCP2CON = (ECCP2CON | 0xC0) & output_config_aux;
    /* set CCP1M3, CCP1M2, CCP1M1, CCP1M0 */
    ECCP2CON = (ECCP2CON | 0x0F) & output_mode_aux;

    uint8_t lectura_CONFIG3H = (flash_readByte(__CONFIG3H) & 0x01);
    if (SINGLE_OUT == output_config)
    {
        if ( lectura_CONFIG3H)
        {
            TRISCbits.TRISC1= 0;
        }
        else
        {
            TRISBbits.TRISB3 = 0;       
        }

    }
    else if (IS_DUAL_PWM(output_config))
    {               

    if ( lectura_CONFIG3H )
    {
        TRISCbits.TRISC1= 0;
        TRISCbits.TRISC0 = 0;       
    }
    else
    {
        TRISBbits.TRISB5 = 0;
        TRISBbits.TRISB3 = 0;       
    }       
    }

    else if (IS_QUAD_PWM(output_config))
    {
        #if defined (EPWM_V15)  
        if ( lectura_CONFIG3H )
        {
            TRISCbits.TRISC1= 0;
            TRISCbits.TRISC0 = 0;       
        }
        else
        {
            TRISBbits.TRISB5 = 0;
            TRISBbits.TRISB3 = 0;       
        }
        TRISDbits.TRISD3 = 0;
        TRISDbits.TRISD4 = 0;   
        #endif      
    }

    #ifndef _OMNI_CODE_
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm 
    #endif
}
#endif

/*
    EPWM3
*/
#if defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) || defined (EPWM_V14_4)
void epwm3_setOutput(uint8_t output_config, uint8_t output_mode)
{
    bool eccpmx = false;    //Se establecerá en alto si el bit 1 del registro __CONFIG3H está en alto                
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;
    
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;

    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    /* set P1M1 and P1M0 */
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;

    CCP3CON = (CCP3CON | 0xC0) & output_config_aux;  //Bits de configuración de salida Enhanced PWM
    CCP3CON = (CCP3CON | 0x0F) & output_mode_aux;    //Bits de configuración de selección de modo CCP

    #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1) 
    if (flash_readByte(__CONFIG3H) & 0x02 )
        eccpmx = true;
    else
        eccpmx = false;
    #endif

    #if defined (PWM_V14_IO_V1) || defined (PWM_V14_IO_V2) || defined (PWM_V15_IO_V1) || defined (PWM_V15_2_IO_V1)  
    if (PWM_SINGLE_OUT == output_config)
    {
        TRISGbits.TRISG0 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
        TRISGbits.TRISG0 = 0;
    #if defined (PWM_V14_IO_V1)	|| defined (PWM_V15_IO_V1)          
        if(eccpmx)
            TRISEbits.TRISE4 = 0;
        else
             TRISHbits.TRISH5 = 0;
    #elif defined (PWM_V15_2_IO_V1)
        if(eccpmx)
            TRISEbits.TRISE4 = 0;
    #elif defined (PWM_V14_IO_V2)
            TRISEbits.TRISE4 = 0; 
    #endif          
    }
    else if (IS_QUAD_PWM(output_config))
    {
            TRISGbits.TRISG0 = 0;
        #if defined (PWM_V14_IO_V1) || defined (PWM_V15_IO_V1)          
        if(eccpmx)  
        {
            TRISEbits.TRISE4 = 0;
            TRISEbits.TRISE3 = 0;
        }
        else        
        {
            TRISHbits.TRISH5 = 0;
            TRISHbits.TRISH4 = 0;
        }
        TRISGbits.TRISG3 = 0;       
        #elif defined (PWM_V15_2_IO_V1)
        if(eccpmx)
            TRISEbits.TRISE4 = 0;
            TRISEbits.TRISE3 = 0;
            TRISGbits.TRISG3 = 0;               
        #elif defined (PWM_V14_IO_V2)
            TRISEbits.TRISE4 = 0;
            TRISEbits.TRISE3 = 0;
            TRISGbits.TRISG3 = 0;           
        #endif
    }
    #endif

    #ifndef _OMNI_CODE_
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm
    #endif
}
#elif defined (EPWM_V15_1)
void epwm3_setOutput(uint8_t output_config, uint8_t output_mode)
{
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;
    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;

    /* set P1M1 and P1M0 */
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;

    CCP3CON = (ECCP2CON | 0xC0) & output_config_aux;
    /* set CCP1M3, CCP1M2, CCP1M1, CCP1M0 */
    CCP3CON = (ECCP2CON | 0x0F) & output_mode_aux;

    uint8_t lectura_CONFIG3H = (flash_readByte(__CONFIG3H) & 0x01);

    if (SINGLE_OUT == output_config)
    {
        if ( lectura_CONFIG3H )
            TRISBbits.TRISB5 = 0;
        else
            TRISCbits.TRISC6 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
        if ( lectura_CONFIG3H )
            TRISBbits.TRISB5 = 0;
        else
            TRISCbits.TRISC6 = 0;
            TRISCbits.TRISC7 = 0; 
    }

    #ifndef _OMNI_CODE_
    _asm
    movff TBLPTR_U, TBLPTRU
    movff TBLPTR_L, TBLPTRL
    _endasm 
    #endif
}
#elif defined (EPWM_V15) 
void epwm3_setOutput(uint8_t output_config, uint8_t output_mode)
{
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;

    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif
    uint8_t output_config_aux = output_config;
    uint8_t output_mode_aux = output_mode;

    /* set P1M1 and P1M0 */
    output_config_aux |= 0x3F;
    output_mode_aux |= 0xFC;

    CCP3CON = (ECCP2CON | 0xC0) & output_config_aux;
    /* set CCP1M3, CCP1M2, CCP1M1, CCP1M0 */
    CCP3CON = (ECCP2CON | 0x0F) & output_mode_aux;

    uint8_t lectura_CONFIG3H = (flash_readByte(__CONFIG3H) & 0x01);

    if (SINGLE_OUT == output_config)
    {
        if (lectura_CONFIG3H)
            TRISBbits.TRISB5 = 0;
        else
            TRISEbits.TRISE0 = 0;
    }
    else if (IS_DUAL_PWM(output_config))
    {               
        if (lectura_CONFIG3H)
            TRISBbits.TRISB5 = 0;
        else
            TRISEbits.TRISE0 = 0;
        TRISEbits.TRISE1 = 0; 
    }
}
#endif

/*
* Función para establecimiento de ciclo de trabajo de PWM
* Parámetros: 
* duty: Variable de ciclo de trabajo. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
* como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
* Retorno: 
* Vacío (void)
*/

/*
    PWM1
*/
#if defined (PWM_V1) || defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V5) || defined (PWM_V6) || defined (PWM_V7) || defined (EPWM_V7) || defined (PWM_V8)  || defined (PWM_V9) ||  defined (PWM_V10) ||  defined (PWM_V11) || defined (PWM_V13) || defined (PWM_V12)
void pwm1_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR1L=(uint8_t)(duty>>2);
    CCP1CON|= (uint8_t)((duty & 0x0003)<< 4);
}
#endif

/*
    PWM2
*/
#if defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V5) ||\
    defined (PWM_V6) || defined (PWM_V9) ||  defined (PWM_V10) ||  defined (PWM_V11)\
    || defined (PWM_V13)  || defined (PWM_V14_2) || defined (PWM_V14_5)
void pwm2_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR2L=(uint8_t)(duty>>2);
    CCP2CON|= (uint8_t)((duty & 0x0003)<< 4);
}
#endif

/*
    PWM3
*/
#if defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V9) || defined (PWM_V14_2)
void pwm3_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR3L=(uint8_t)(duty>>2);
    CCP3CON|= (uint8_t)((duty & 0x0003)<< 4);
}
#endif

/*
    PWM4
*/
#if defined (PWM_V4) || defined (PWM_V9) || defined (PWM_V14) || defined (PWM_V14_1)\
     || defined (PWM_V14_2) ||defined (PWM_V14_3) || defined (PWM_V14_4) \
     || defined (PWM_V15) ||defined (PWM_V15_1)
void pwm4_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR4L=(uint8_t)(duty>>2);
    CCP4CON|= (uint8_t)((duty & 0x0003)<< 4);  
}
#endif

/*
    PWM5
*/
#if defined (PWM_V4) || defined (PWM_V9) || defined (PWM_V14) || defined (PWM_V14_1)\
     || defined (PWM_V14_2) ||defined (PWM_V14_3) || defined (PWM_V14_4) || defined (PWM_V15) ||defined (PWM_V15_1)
void pwm5_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR5L=(uint8_t)(duty>>2);
    CCP5CON|= (uint8_t)((duty & 0x0003)<< 4);   
}
#endif


/*
    PWM6
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm6_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR6L=(uint8_t)(duty>>2);
    CCP6CON|= (uint8_t)((duty & 0x0003)<< 4);   
}
#endif

/*
    PWM7
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm7_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR7L=(uint8_t)(duty>>2);
    CCP7CON|= (uint8_t)((duty & 0x0003)<< 4);   
}
#endif

/*
    PWM8
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm8_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR8L=(uint8_t)(duty>>2);
    CCP8CON|= (uint8_t)((duty & 0x0003)<< 4);   
}
#endif

/*
    PWM9
*/
#if defined (PWM_V14) || defined (PWM_V14_3)
void pwm9_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR9L=(uint8_t)(duty>>2);
    CCP9CON|= (uint8_t)((duty & 0x0003)<< 4);   
}
#endif

/*
    PWM10
*/
#if defined (PWM_V14) ||defined (PWM_V14_3)
void pwm10_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR10L=(uint8_t)(duty>>2);
    CCP10CON|= (uint8_t)((duty & 0x0003)<< 4);   
}
#endif

/*
    EPWM1
*/
#if defined (EPWM_V7) 
void epwm1_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    ECCPR1L=(uint8_t)(duty>>2);
    ECCP1CON|= (uint8_t)((duty & 0x0003)<< 4);
}

#elif defined (EPWM_V14) || defined (EPWM_V14_1) || defined (EPWM_V14_2) \
    ||defined (EPWM_V14_3) || defined (EPWM_V14_4) || defined (EPWM_V14_5) || defined (EPWM_V15) || defined (EPWM_V15_1)
void epwm1_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR1L=(uint8_t)(duty>>2);

    #if defined (EPWM_V14_3) || defined (EPWM_V14_4) || defined (EPWM_V14_5) 
    CCP1CON|= (uint8_t)((duty & 0x0003)<< 4);
    #else
    ECCP1CON|= (uint8_t)((duty & 0x0003)<< 4);
    #endif 
}
#endif

/*
    EPWM2
*/
#if defined (EPWM_V14) || defined (EPWM_V14_1)||defined (EPWM_V14_3) || \
    defined (EPWM_V14_4)|| defined (EPWM_V15) ||defined (EPWM_V15_1)
void epwm2_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR2L=(uint8_t)(duty>>2);

    #if defined (EPWM_V14_3) || defined (EPWM_V14_4)
    CCP2CON|= (uint8_t)((duty & 0x0003)<< 4);
    #else
    ECCP2CON|= (uint8_t)((duty & 0x0003)<< 4);
    #endif 
}    
#endif

/*
    EPWM3
*/
#if defined (EPWM_V14) || defined (EPWM_V14_1)||defined (EPWM_V14_3) || \
    defined (EPWM_V14_4)|| defined (EPWM_V15) ||defined (EPWM_V15_1)
void epwm3_setDuty(uint16_t duty)
{
    //Se configura la palabra CCPRL:CCPCON<5:4>, formando una variable de 10 bits que contiene el ciclo de trabajo
    CCPR3L=(uint8_t)(duty>>2);
    CCP3CON|= (uint8_t)((duty & 0x0003)<< 4);
}
#endif


/*
* Función para desactivar PWM
* Parámetros: 
* Ninguno
* Retorno: 
* Vacío (void)
*/

/*
    PWM1
*/
#if defined (PWM_V1) || defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V5) || defined (PWM_V6) || defined (PWM_V7) || defined (EPWM_V7) || defined (PWM_V9)
void pwm1_close()
{
    CCP1CON=0;                    // Desactiva PWM
    // TRISCbits.TRISC2=1;     //NO es necesario poner en triestado el puerto y deshabilitar la salida pwm en él. Esto fue comentado para mantener la consistencia con respecto a otras funciones para desactivar pwm
}

#elif  defined (PWM_V8)
void pwm1_close()
{
    CCP1CON=0;                    // Desactiva PWM
    // TRISBbits.TRISB3=1; //NO es necesario poner en triestado el puerto y deshabilitar la salida pwm en él. Esto fue comentado para mantener la consistencia con respecto a otras funciones para desactivar pwm
}

#elif  defined (PWM_V10) || defined (PWM_V12)
//TODO add code
void pwm1_close()
{
    CCP1CON=0;                    // Desactiva PWM
    // #if defined PWM1_IO_V6     //NO es necesario poner en triestado el puerto y deshabilitar la salida pwm en él. Esto fue comentado para mantener la consistencia con respecto a otras funciones para desactivar pwm
    // if(PSTRCONbits.STRA == 1) TRISCbits.TRISC5 = 1;  // P1A
    // if(PSTRCONbits.STRB == 1) TRISCbits.TRISC4 = 1;  // P1B
    // if(PSTRCONbits.STRC == 1) TRISCbits.TRISC3 = 1;  // P1C
    // if(PSTRCONbits.STRD == 1) TRISCbits.TRISC2 = 1;  // P1D        
    // #else
    // if(PSTRCONbits.STRA == 1) TRISCbits.TRISC2 = 1;  // P1A
    // if(PSTRCONbits.STRB == 1) TRISBbits.TRISB2 = 1;  // P1B
    // if(PSTRCONbits.STRC == 1) TRISBbits.TRISB1 = 1;  // P1C
    // if(PSTRCONbits.STRD == 1) TRISBbits.TRISB4 = 1;  // P1D
    // #endif
}
#elif  defined (PWM_V11)
//TODO add code
void pwm1_close()
{
    CCP1CON = 0;                     // Desactiva PWM
    //NO es necesario poner en triestado el puerto y deshabilitar la salida pwm en él. Esto fue comentado para mantener la consistencia con respecto a otras funciones para desactivar pwm
    // if(PSTRCONbits.STRA == 1) TRISCbits.TRISC2 = 1;  // P1A
    // if(PSTRCONbits.STRB == 1) TRISDbits.TRISD5 = 1;  // P1B
    // if(PSTRCONbits.STRC == 1) TRISDbits.TRISD6 = 1;  // P1C
    // if(PSTRCONbits.STRD == 1) TRISDbits.TRISD7 = 1;  // P1D
}

#elif defined (PWM_V13)
void pwm1_close()
{
    //
    CCP1CON = 0;                      // Desactiva PWM
}
#endif

/*
    PWM2
*/
#if defined (PWM_V2) || defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V5)\
    || defined (PWM_V6) || defined (PWM_V9) ||  defined (PWM_V10) ||  defined (PWM_V11)\
    || defined (PWM_V13)  || defined (PWM_V14_2) || defined (PWM_V14_5)
void pwm2_close()
{
    CCP2CON = 0;          // Desactiva PWM2
}
#endif

/*
    PWM3
*/
#if defined (PWM_V3) || defined (PWM_V4) || defined (PWM_V9)  || defined (PWM_V14_2)
void pwm3_close()
{
    CCP3CON = 0;            // Desactiva PWM3
}
#endif

/*
    PWM4
*/
#if defined (PWM_V4) || defined (PWM_V9) || defined (PWM_V14) || defined (PWM_V14_1)\
     || defined (PWM_V14_2) ||defined (PWM_V14_3) || defined (PWM_V14_4) ||\
     defined (PWM_V15) ||defined (PWM_V15_1)
void pwm4_close()
{
    CCP4CON = 0;            // Desactiva PWM4
}
#endif

/*
    PWM5
*/
#if defined (PWM_V4) || defined (PWM_V9) || defined (PWM_V14) || defined (PWM_V14_1) || defined (PWM_V14_2)||defined (PWM_V14_3) || \
    defined (PWM_V14_4)|| defined (PWM_V15) ||defined (PWM_V15_1)
void pwm5_close()
{
    CCP5CON = 0;            // Desactiva PWM5   
}
#endif

/*
    PWM6
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm6_close()
{
    CCP6CON = 0;            // Desactiva PWM6     
}
#endif

/*
    PWM7
*/
#if defined (PWM_V14) || defined (PWM_V14_1) || defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm7_close()
{
    CCP7CON = 0;            // Desactiva PWM7
}
#endif

/*
    PWM8
*/
#if defined (PWM_V14) || defined (PWM_V14_1) ||defined (PWM_V14_3) || defined (PWM_V14_4)
void pwm8_close()
{
    CCP8CON = 0;            // Desactiva PWM8
}
#endif

/*
    PWM9
*/
#if defined (PWM_V14) || defined (PWM_V14_3)
void pwm9_close()
{
    CCP9CON = 0;            // Desactiva PWM9
}
#endif

/*
    PWM10
*/
#if defined (PWM_V14) ||defined (PWM_V14_3)
void pwm10_close()
{
    CCP10CON = 0;            // Desactiva PWM10
}
#endif

/*
    EPWM1
*/
#if defined (EPWM_V7) || defined (EPWM_V14) || defined (EPWM_V14_1)\
    || defined (EPWM_V14_2) || defined (EPWM_V15) ||defined (EPWM_V15_1) 
void epwm1_close()
{  
   ECCP1CON = 0;           // Desactiva PWM
}

#elif defined (EPWM_V14_3) || defined (EPWM_V14_4) || defined (EPWM_V14_5) 
void epwm1_close()
{   
   CCP1CON = 0;           // Desactiva PWM
}
#endif

/*
    EPWM2
*/
#if defined (EPWM_V14) || defined (EPWM_V14_1)|| defined (EPWM_V15) ||defined (EPWM_V15_1)
void epwm2_close()
{
    ECCP2CON = 0;           // Desactiva EPWM2
}
#elif defined (EPWM_V14_3) || defined (EPWM_V14_4)
void epwm2_close()
{
    CCP2CON = 0;           // Desactiva EPWM2
}
#endif

/*
    EPWM3
*/
#if defined (EPWM_V14) || defined (EPWM_V14_1) ||defined (EPWM_V14_3) ||\
 defined (EPWM_V14_4)|| defined (EPWM_V15) ||defined (EPWM_V15_1)
void epwm3_close()
{
    CCP3CON = 0;           // Desactiva EPWM3
}
#endif
