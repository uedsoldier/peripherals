/**
  * @file compare.c
  * @brief Funciones para manejo de módulos CCP en modo output compare en microcontroladores PIC de 8 bits
  * @author Ing. José Roberto Parra Trewartha
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../pconfig.h"
#include "../../utils/utils.h"
#include "../TIMERS/timers.h"
#include "../FLASH/flash.h"
#include "compare.h"

/*
    Módulo CCP1
*/

#if defined (CC_V1) || defined (CC_V2) || defined (CC_V3) || defined (CC_V4) || defined (CC_V5) || defined (CC_V6)
/**
  * @brief Función de inicialización de módulo CCP1 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare1_init(uint8_t param_config) 
{

    CCP1CON = param_config;

    if( (CCP1CON != CCP_COMPARE_INT) && (CCP1CON != CCP_COMPARE_RESET_TIMER) ) //Modos en los que no se configura el pin CCP como salida
    {
        CM1_TRIS = 0;
    }
}
#elif defined (CC_V7)
/**
  * @brief Función de inicialización de módulo CCP1 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare1_init(uint8_t param_config) 
{

    CCP1CON = param_config;
}
#endif

#if defined (ECC_V5)
/**
  * @brief Función de inicialización de módulo ECCP1 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void ecompare1_init(uint8_t param_config) 
{

    ECCP1CON = param_config;

    if( ECCP1CON != CCP_COMPARE_INT)
    #if defined ECCP1_IO_V5
        TRISBbits.TRISB4 = 0;   
    #else
        TRISDbits.TRISD4 = 0;   
    #endif

}

#elif defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_2) || defined (ECC_V8_3) || defined (ECC_V8_4)\
    || defined (ECC_V8_5) || defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función de inicialización de módulo ECCP1 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void ecompare1_init(uint8_t param_config) 
{
    #if defined (ECC_V8_3) || defined (ECC_V8_4) || defined (ECC_V8_5)
    CCP1CON = param_config & 0x0F;
    #else
    ECCP1CON = param_config & 0x0F;
    #endif

    #if defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_3) || defined (ECC_V8_4)
    //Configurar fuente de temporización de CCP
    CCPTMRS0 &= 0xFC;
    CCPTMRS0 |= (( param_config & 0x70 )>>4);
    #elif defined (ECC_V9)|| defined (ECC_V9_1)  
    CCPTMRS0 &= 0xFC;
    CCPTMRS0 |= (( param_config & 0x30 )>>4);    
    #elif defined (ECC_V8_2) || defined (ECC_V8_5)
    //Configurar fuente de temporización de CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= (( param_config & 0x10)>>4); 
    #endif

    #ifndef CC4_IO_V2  
    #if defined (ECC_V8_3) || defined (ECC_V8_4)
    if(CCP1CON != CCP_COMPARE_INT)
    #else 
    if(ECCP1CON != CCP_COMPARE_INT)
    #endif

    #if defined (ECC_V8_3)
    RPINR14_15=0x02
    CM1_TRIS = 1;
    #else
    CM1_TRIS = 0; 
    #endif
    #endif  
}
#endif

/*
    Módulo CCP2
*/
#if defined (CC_V2 ) || defined ( CC_V3 ) || defined ( CC_V4 )
/**
  * @brief Función de inicialización de módulo CCP2 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare2_init(uint8_t param_config) 
{
    #define __CONFIG3L 0x300004
    #define __CONFIG3H 0x300005
    bool pmmode;
    bool ccp2mx;

    CCP2CON = param_config&0x0F; //  Configure capture
    if ((flash_readByte(__CONFIG3H)) & 0x01)
        ccp2mx = true;
    else
        ccp2mx = false;
   
	#if defined CC_CONFIG3L_V1
	    if (((flash_readByte(__CONFIG3L)) & 0x03) == 0x03)
	        pmmode = true;
	    else
	        pmmode = false;
	#elif defined CC_CONFIG3L_V2
	    if (((flash_readByte(__CONFIG3L)) & 0x30) == 0x30)
	        pmmode = true;
	    else
	        pmmode = false;
	#endif

	    if( CCP2CON != CCP_COMPARE_INT)
	    {
	        #if defined CC2_IO_V0   
	        TRISCbits.TRISC1 = 0;
	        #elif defined CC2_IO_V1 
	        TRISBbits.TRISB3 = 0;
	        #elif defined CC2_IO_V2 
	        TRISEbits.TRISE7 = 0;
	        #elif defined CC2_IO_V3
	        if(ccp2mx)
	            TRISCbits.TRISC1 = 0;
	        else
	            TRISBbits.TRISB3 = 0;
	        #elif defined CC2_IO_V4
	        if(ccp2mx)
	            TRISCbits.TRISC1 = 0;
	        else
	            TRISEbits.TRISE7 = 0;
	        #elif defined CC2_IO_V5
	        if(ccp2mx) 
	            TRISCbits.TRISC1 = 0;
	        else if(pmmode)
	            TRISEbits.TRISE7 = 0;
	        else
	            TRISBbits.TRISB3 = 0;
	        #endif  
	    }
}

#elif defined ( CC_V6 )
/**
  * @brief Función de inicialización de módulo CCP2 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare2_init(uint8_t param_config) 
{
    #define __CONFIG3L 0x300004
    #define __CONFIG3H 0x300005
    bool pmmode; 
    bool ccp2mx;

    CCP2CON = param_config & 0x0F; //  Configure capture

    if ((flash_readByte(__CONFIG3H)) & 0x01)
        ccp2mx = true;
    else
        ccp2mx = false;

    #if defined CC_CONFIG3L_V1
    if (((flash_readByte(__CONFIG3L)) & 0x03) == 0x03)
        pmmode = true;
    else
        pmmode = false;
    #elif defined CC_CONFIG3L_V2
    if (((flash_readByte(__CONFIG3L)) & 0x30) == 0x30)
        pmmode = true;
    else
        pmmode = false;
    #endif

    if( CCP2CON != CCP_COMPARE_INT)
    {
        #if defined CC2_IO_V0   
        TRISCbits.TRISC1 = 0;
        #elif defined CC2_IO_V1 
        TRISBbits.TRISB3 = 0;
        #elif defined CC2_IO_V2 
        TRISEbits.TRISE7 = 0;
        #elif defined CC2_IO_V3
        if(ccp2mx)
            TRISCbits.TRISC1 = 0;
        else
            TRISBbits.TRISB3 = 0;
        #elif defined CC2_IO_V4
        if(ccp2mx) 
            TRISCbits.TRISC1 = 0;
        else 
            TRISEbits.TRISE7 = 0;
        #elif defined CC2_IO_V5
        if(ccp2mx)
            TRISCbits.TRISC1 = 0;
        else if(pmmode)
            TRISEbits.TRISE7 = 0;
        else
            TRISBbits.TRISB3 = 0;
        #endif  
    }
}

#elif defined (CC_V7)
/**
  * @brief Función de inicialización de módulo CCP2 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare2_init(uint8_t param_config) 
{
    CCP2CON = param_config;
}

#elif defined ( CC_V8_2 ) || defined ( CC_V8_5 )
/**
  * @brief Función de inicialización de módulo CCP2 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare2_init(uint8_t param_config) 
{

    CCP2CON = param_config&0x0F; //  Configure capture
    //Configurar fuente de temporización de CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= (( param_config & 0x10 )>>3); 

    if( CCP2CON != CCP_COMPARE_INT)
    {
        CM2_TRIS=0;
    }

}
#elif defined ( CC_V8_5 )
/**
  * @brief Función de inicialización de módulo CCP2 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare2_init(uint8_t param_config) 
{

    CCP2CON = param_config&0x0F; //  Configure capture
    //Configurar fuente de temporización de CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= (( config & 0x10 )>>1); 
    
    if( CCP2CON != CCP_COMPARE_INT)
    {
        CM2_TRIS=0;
    }
}
#endif

#if defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_2) || defined (ECC_V8_3) || defined (ECC_V8_4)
/**
  * @brief Función de inicialización de módulo ECCP2 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void ecompare2_init(uint8_t param_config) 
{
    #if defined (ECC_V8_3) || defined (ECC_V8_4)
    CCP2CON = param_config & 0x0F;  // Configure capture
    #else
    ECCP2CON = param_config & 0x0F;  // Configure capture
    #endif

    #if defined (ECC_V8_2) 
    //configure timer source for CCP
    CCPTMRS &= 0xC7;
    CCPTMRS |= (( param_config & 0x70 )>>1);
    #else
    //configure timer source for CCP
    CCPTMRS0 &= 0xC7;
    CCPTMRS0 |= (( param_config & 0x70 )>>1);
    #endif 

    #ifndef CC4_IO_V2 
    #if defined (ECC_V8_3) || defined (ECC_V8_4)
    if( CCP2CON != CCP_COMPARE_INT)
    #else 
    if( ECCP2CON != CCP_COMPARE_INT)
    #endif

    #if defined (ECC_V8_3)
    RPINR16_17=0x07;
    CM2_TRIS = 1;
    #else 
    CM2_TRIS = 0;   
    #endif  
    #endif  
}

#elif defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función de inicialización de módulo ECCP2 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void ecompare2_init(uint8_t param_config) {
    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;

    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    ECCP2CON = param_config & 0x0F;  // Configure capture

    CCPTMRS0 &= 0xE7;
    CCPTMRS0 |= (( param_config & 0x30)>>1);

    if (flash_readByte(0x300005) & 0x01)
    {
        if( ECCP2CON != CCP_COMPARE_INT)
            TRISCbits.TRISC1 = 0;   
        else if ((flash_readByte(0x300005)) & 0x04)
        {
            if( ECCP2CON != CCP_COMPARE_INT)
                TRISBbits.TRISB3 = 0;   
        }
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
    Módulo CCP3
*/
#if defined (CC_V3 ) || defined ( CC_V4) || defined (CC_V8_2)
/**
  * @brief Función de inicialización de módulo CCP3 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare3_init(uint8_t param_config) 
{
    CCP3CON = param_config & 0x0F; //  Configure capture

    #if defined (CC_V8_2)
    //configure timer source for CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= (( param_config & 0x10)>>2); 
    #endif

    if( CCP3CON != CCP_COMPARE_INT)
        CM3_TRIS = 0;
}
#endif

#if defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_3) || defined (ECC_V8_4)
/**
  * @brief Función de inicialización de módulo ECCP3 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void ecompare3_init(uint8_t param_config) 
{
    CCP3CON = param_config & 0x0F;  // Configure capture

    //configure timer source for CCP
    CCPTMRS0 &= 0x3F;
    CCPTMRS0 |= (( param_config & 0x30 )<<2); 

    #ifndef CC4_IO_V2    
    if( CCP3CON != CCP_COMPARE_INT)
        #if defined (ECC_V8_3) || defined (ECC_V8_4)
        RPINR16_17=0x0B;
        CM3_TRIS = 1;
        #else
        CM3_TRIS = 0;   
        #endif  
    #endif  
}
#endif

#if  defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función de inicialización de módulo ECCP3 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void ecompare3_init(uint8_t param_config) {

    #ifndef _OMNI_CODE_
    uint8_t TBLPTR_U, TBLPTR_L;

    _asm
    movff TBLPTRU, TBLPTR_U
    movff TBLPTRL, TBLPTR_L
    _endasm
    #endif

    CCP3CON = param_config & 0x0F;  // Configure capture

    //configure timer source for CCP
    CCPTMRS0 &= 0x3F;
    CCPTMRS0 |= (( param_config & 0x30 )<<2); 

    if (flash_readByte(0x300005) & 0x01)
    {
        if( CCP3CON != CCP_COMPARE_INT)
            TRISBbits.TRISB5 = 0;   
        else if (flash_readByte(0x300005) & 0x04)
        {
            if( CCP3CON != CCP_COMPARE_INT)
                #if defined (ECC_V9)
                TRISEbits.TRISE0 = 0;
                #else
                TRISCbits.TRISC6 = 0;
                #endif
        }
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
    Módulo CCP4
*/
#if defined (CC_V4)
/**
  * @brief Función de inicialización de módulo CCP4 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare4_init(uint8_t param_config) 
{
    CCP4CON = param_config; //  Configure capture
    if( CCP4CON != CCP_COMPARE_INT)
        CM4_TRIS = 0;
}

#elif defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_2)\
|| defined (CC_V8_3) || defined (CC_V8_4) || defined (CC_V9)\
|| defined (CC_V9_1)
/**
  * @brief Función de inicialización de módulo CCP4 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare4_init(uint8_t param_config) 
{

    CCP4CON = param_config&0x0F; //  Configure capture

    #if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
    //configure timer source for CCP
    CCPTMRS1 &= 0xFC;
    CCPTMRS1 |= (( param_config & 0x30)>>4); 
    #elif defined (CC_V9)|| defined (CC_V9_1)  
    //configure timer source for CCP
    CCPTMRS1 &= 0xFC;
    CCPTMRS1 |= (( param_config & 0x30)>>4);   
    #elif defined (CC_V8_2)
    //configure timer source for CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= (( param_config & 0x10 )>>1); 
    #endif  

    if( CCP4CON != CCP_COMPARE_INT)
        #if defined (CC_V8_3) || defined (CC_V8_4)
        RPINR32_33=0x0B;
        CM4_TRIS = 1;     //TRIS direction as input
        #else
        CM4_TRIS = 1;     //TRIS direction as input
        #endif
}
#endif

/*
    Módulo CCP5
*/
#if defined (CC_V4)
/**
  * @brief Función de inicialización de módulo CCP5 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare5_init(uint8_t param_config) 
{
    CCP5CON = param_config;  // Configure capture

    if( CCP5CON != CCP_COMPARE_INT)
        TRISGbits.TRISG4 = 0;
}

#elif defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_2)\
|| defined (CC_V8_3) || defined (CC_V8_4) || defined (CC_V9)|| defined (CC_V9_1)
/**
  * @brief Función de inicialización de módulo CCP5 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare5_init(uint8_t param_config) 
{
    CCP5CON = param_config & 0x0F;  // Configure capture

    #if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
    //configure timer source for CCP
    CCPTMRS1 &= 0xFB;
    CCPTMRS1 |= (( param_config & 0x10 )>>2); 
    #elif  defined (CC_V9)|| defined (CC_V9_1)
    //configure timer source for CCP
    CCPTMRS1 &= 0xF3;
    CCPTMRS1 |= (( param_config & 0x30 )>>2);   
    #elif defined (CC_V8_2)
    //configure timer source for CCP
    CCPTMRS &= 0xEF;
    CCPTMRS |= (( param_config & 0x10 )); 
    #endif

    if( CCP5CON != CCP_COMPARE_INT)
    #if defined (CC_V8_3) || defined (CC_V8_4)
        RPINR32_33=0x0B;
    CM5_TRIS = 1;
    #else
    CM5_TRIS = 1;
    #endif
}
#endif

/*
    Módulo CCP6
*/
#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
/**
  * @brief Función de inicialización de módulo CCP6 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare6_init(uint8_t param_config) 
{
    CCP6CON = param_config & 0x0F;  // Configure capture

    //configure timer source for CCP
    CCPTMRS1 &= 0xEF;
    CCPTMRS1 |= ( param_config & 0x10 ); 

    if( CCP6CON != CCP_COMPARE_INT)
    #if defined (CC_V8_3) || defined (CC_V8_4)
        RPINR34_35=0x08;
    CM6_TRIS = 1;
    #else
    CM6_TRIS = 1;
    #endif
}
#endif

/*
    Módulo CCP7
*/
#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
/**
  * @brief Función de inicialización de módulo CCP7 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare7_init(uint8_t param_config) 
{
    CCP7CON = param_config&0x0F;  // Configure capture

    //configure timer source for CCP
    CCPTMRS1 &= 0x3F;
    CCPTMRS1 |= (( param_config & 0x30 )<<2); 

    if( CCP7CON != CCP_COMPARE_INT)
    #if defined (CC_V8_3) || defined (CC_V8_4)
        RPINR34_35=0x09;
    CM7_TRIS = 1;
    #else
    CM7_TRIS = 1;
    #endif
}
#endif

/*
    Módulo CCP8
*/
#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
/**
  * @brief Función de inicialización de módulo CCP8 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare8_init(uint8_t param_config) 
{
    CCP8CON = param_config&0x0F;  // Configure capture

    //configure timer source for CCP
    CCPTMRS2 &= 0xFC;
    CCPTMRS2 |= (( param_config & 0x30 )>>4);

    if( CCP8CON != CCP_COMPARE_INT)
    #if defined (CC_V8_3) || defined (CC_V8_4)
        RPINR36_37=0x08;
    CM8_TRIS = 1;
    #else
    CM8_TRIS = 1;
    #endif
}
#endif

/*
    Módulo CCP9
*/
#if defined (CC_V8) || defined (CC_V8_3) 
/**
  * @brief Función de inicialización de módulo CCP9 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare9_init(uint8_t param_config) 
{     
    CCP9CON = param_config & 0x0F;  // Configure capture

    //configure timer source for CCP
    CCPTMRS2 &= 0xFB;
    CCPTMRS2 |= (( param_config & 0x10 )>>2);

    if( CCP9CON != CCP_COMPARE_INT)
    #if defined (CC_V8_3) || defined (CC_V8_4)
        RPINR36_37=0x08;
    CM9_TRIS = 1;
    #else
    CM9_TRIS = 1;
    #endif
}
#endif

/*
    Módulo CCP10
*/
#if defined (CC_V8) || defined (CC_V8_3) 
/**
  * @brief Función de inicialización de módulo CCP10 en modo comparación
  * @param param_config: (uint8_t) Parámetros de configuración, para utilizar el módulo correspondiente como Output Compare en sus diferentes modalidades.
  * @return (void)
*/
void compare10_init(uint8_t param_config) 
{
    CCP10CON = param_config & 0x0F;     // Configure capture

    //configure timer source for CCP
    CCPTMRS2 &= 0xEF;
    CCPTMRS2 |= (param_config & 0x10);  

    if( CCP10CON != CCP_COMPARE_INT)
    #if defined (CC_V8_3) || defined (CC_V8_4)
        RPINR38_39=0x07;
    CM10_TRIS = 1;
    #else
    CM10_TRIS = 1;
    #endif
}
#endif

/*
    Módulo CCP1
*/
#if defined (CC_V1) || defined (CC_V2) || defined (CC_V3) || defined (CC_V4) || defined (CC_V5) || defined (CC_V6) || defined (CC_V7)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo CCP1
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void compare1_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR1H = make8(period,1);
    CCPR1L = make8(period,0);
}
#endif

#if defined (ECC_V5)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo ECCP1
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void ecompare1_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    ECCPR1H = make8(period,1);
    ECCPR1L = make8(period,0);
}

#elif defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_2) || \
    defined (ECC_V8_3) || defined (ECC_V8_4) || defined (ECC_V8_5) || defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo ECCP1
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void ecompare1_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR1H = make8(period,1);
    CCPR1L = make8(period,0);
}
#endif

/*
    Módulo CCP2
*/
#if defined (CC_V2 ) || defined ( CC_V3 ) || defined ( CC_V4 ) || defined ( CC_V6 ) || defined (CC_V7) || defined ( CC_V8_2 ) || defined ( CC_V8_5 )
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo CCP2
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void compare2_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR2H = make8(period,1);
    CCPR2L = make8(period,0);
}
#endif

#if defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_2) || defined (ECC_V8_3) || defined (ECC_V8_4) || defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo ECCP2
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void ecompare2_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR2H = make8(period,1);
    CCPR2L = make8(period,0);
}
#endif

/*
    Módulo CCP3
*/
#if defined (CC_V3 ) || defined ( CC_V4) || defined (CC_V8_2)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo CCP3
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void compare3_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR3H = make8(period,1);
    CCPR3L = make8(period,0);
}
#endif

#if defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_3) || defined (ECC_V8_4) || defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo ECCP3
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void ecompare3_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR3H = make8(period,1);
    CCPR3L = make8(period,0);
}
#endif

/*
    Módulo CCP4
*/
#if defined (CC_V4) || defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_2) || defined (CC_V8_3) || defined (CC_V8_4) || defined (CC_V9)\
    || defined (CC_V9_1)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo CCP4
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void compare4_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR4H = make8(period,1);
    CCPR4L = make8(period,0);
}
#endif

/*
    Módulo CCP5
*/
#if defined (CC_V4) || defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_2) || defined (CC_V8_3) || defined (CC_V8_4) || defined (CC_V9)\
    || defined (CC_V9_1)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo CCP5
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void compare5_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR5H = make8(period,1);
    CCPR5L = make8(period,0);
}
#endif

/*
    Módulo CCP6
*/
#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo CCP6
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void compare6_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR6H = make8(period,1);
    CCPR6L = make8(period,0);
}
#endif

/*
    Módulo CCP7
*/
#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo CCP7
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void compare7_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR7H = make8(period,1);
    CCPR7L = make8(period,0);
}
#endif

/*
    Módulo CCP8
*/
#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo CCP8
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void compare8_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR8H = make8(period,1);
    CCPR8L = make8(period,0);
}
#endif

/*
    Módulo CCP9
*/
#if defined (CC_V8) || defined (CC_V8_3) 
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo CCP9
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void compare9_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR9H = make8(period,1);
    CCPR9L = make8(period,0);
}
#endif

/*
    Módulo CCP10
*/
#if defined (CC_V8) || defined (CC_V8_3) 
/**
  * @brief Función de establecimiento de periodo de comparación para el módulo CCP10
  * @param period: (uint16_t) Variable de comparación. Se sugiere utilizar la hoja de cálculo correspondiente. Su valor depende de multiples parámetros,
  * como la configuración del temporizador asociado, la frecuencia del oscilador principal, etc.
  * @return (void)
*/
void compare10_setPeriod(uint16_t period)
{
    //Se configura la palabra CCPRH:CCPRL, formando una variable de 16 bits que contiene el periodo de comparación
    CCPR10H = make8(period,1);
    CCPR10L = make8(period,0);
}
#endif

/*
* Módulo CCP1
*/

#if defined (CC_V1) || defined (CC_V2) || defined (CC_V3) || defined (CC_V4) || defined (CC_V5) || defined (CC_V7) || defined (CC_V6)
/**
  * @brief Función para deshabilitar módulo Compare 1, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void compare1_disable()
{
    CCP1CON = 0;           //Deshabilita módulo CCP-Compare
}
#endif

#if defined (ECC_V5)
/**
  * @brief Función para deshabilitar módulo ECompare 1, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void ecompare1_disable()
{
    ECCP1CON = 0;           //Deshabilita módulo CCP-Compare
}
#elif defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_2)\
|| defined (ECC_V8_3) || defined (ECC_V8_4) || defined (ECC_V8_5) || defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función para deshabilitar módulo ECompare 1, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void ecompare1_disable()
{
    #if defined (ECC_V8_3) || defined (ECC_V8_4) || defined (ECC_V8_5)
    CCP1CON = 0;           //Deshabilita módulo CCP-Compare
    #else
    ECCP1CON = 0;           //Deshabilita módulo CCP-Compare
    #endif
}
#endif

/*
* Módulo CCP2
*/

#if defined (CC_V2 ) || defined ( CC_V3 ) || defined ( CC_V4 ) || defined (CC_V7) || defined (CC_V8_5) || defined ( CC_V6 )  || defined (CC_V8_2)
/**
  * @brief Función para deshabilitar módulo Compare 2, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void compare2_disable()
{  
    CCP2CON = 0;           //Deshabilita módulo CCP-Compare
}
#endif

#if defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_3) || \
    defined (ECC_V8_4)|| defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función para deshabilitar módulo ECompare 2, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void ecompare2_disable()
{
    #if defined (ECC_V8_3) || defined (ECC_V8_4)
    CCP2CON = 0;           //Deshabilita módulo CCP-Compare
    #else
    ECCP2CON = 0;           //Deshabilita módulo CCP-Compare
    #endif
}
#endif

/*
* Módulo CCP3
*/
#if defined (CC_V3 ) || defined ( CC_V4) || defined (CC_V8_2)
/**
  * @brief Función para deshabilitar módulo Compare 3, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void compare3_disable()
{  
    CCP3CON = 0;           //Deshabilita módulo CCP-Compare
}
#endif

#if defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_3) || defined (ECC_V8_4)|| defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función para deshabilitar módulo ECompare 3, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void ecompare3_disable()
{
    #if defined (ECC_V8_3) || defined (ECC_V8_4)
    ECCP3CON = 0;           //Deshabilita módulo CCP-Compare
    #else
    CCP3CON = 0;           //Deshabilita módulo CCP-Compare
    #endif
}
#endif

/*
* Módulo CCP4
*/
#if defined (CC_V4) || defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_2) || defined (CC_V8_3) || defined (CC_V8_4) || defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función para deshabilitar módulo Compare 4, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void compare4_disable()
{  
    CCP4CON = 0;           //Deshabilita módulo CCP-Compare
}
#endif

/*
* Módulo CCP5
*/
#if defined (CC_V4) || defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_2) || defined (CC_V8_3) || defined (CC_V8_4) || defined (ECC_V9)|| defined (ECC_V9_1)
/**
  * @brief Función para deshabilitar módulo Compare 5, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void compare5_disable()
{  
    CCP5CON = 0;           //Deshabilita módulo CCP-Compare
}
#endif

/*
* Módulo CCP6
*/
#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
/**
  * @brief Función para deshabilitar módulo Compare 6, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void compare6_disable()
{  
    CCP6CON = 0;           //Deshabilita módulo CCP-Compare
}
#endif

/*
* Módulo CCP7
*/
#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
/**
  * @brief Función para deshabilitar módulo Compare 7, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void compare7_disable()
{  
    CCP7CON = 0;           //Deshabilita módulo CCP-Compare
}
#endif

/*
* Módulo CCP8
*/
#if defined (CC_V8) || defined (CC_V8_1) || defined (CC_V8_3) || defined (CC_V8_4)
/**
  * @brief Función para deshabilitar módulo Compare 8, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void compare8_disable()
{  
    CCP8CON = 0;           //Deshabilita módulo CCP-Compare
}
#endif

/*
* Módulo CCP9
*/
#if defined (CC_V8) || defined (CC_V8_3)
/**
  * @brief Función para deshabilitar módulo Compare 9, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void compare9_disable()
{  
    CCP9CON = 0;           //Deshabilita módulo CCP-Compare
}
#endif

/*
* Módulo CCP10
*/
#if defined (CC_V8) || defined (CC_V8_3) 
/**
  * @brief Función para deshabilitar módulo Compare 10, así podrá utilizarse el pin asociado como E/S de propósito general
  * o bien para ahorro de energía
  * @param (void)
  * @return (void)
*/
void compare10_disable()
{  
    CCP10CON = 0;           //Deshabilita módulo CCP-Compare
}
#endif