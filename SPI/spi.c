/*
    Librería de comunicación spi por hardware para microcontroladores PIC de 8 bits
    Autor: Ing. José Roberto Parra Trewartha
    Compilador: XC8
*/

#include "spi.h"

/*
    Función de inicialización de MSSP en modo SPI
    Parámetros:
    - rol_spi: Maestro o esclavo. Constantes definidas en spi.h
    - modo_spi: Modo de funcionamiento SPI según la lógica de los dispositivos a manejar, según se describe a continuación:
        Modos de comunicacion SPI: (CPOL,CPHA) (Polaridad de reloj, Fase de reloj)
         * _______________________________________________________________________________
         * Modo |  Motorola  | Microchip |  Muestreo   |  Propagacion   | Estado inactivo |
         *      |  CPOL CPHA | CKP CKE   |  en flanco  |   en flanco    |   de reloj      |
         * _____|____________|___________|_____________|________________|_________________|
         *   0       0,0          0,1      Ascendente      Descendente         Bajo
         *   1       0,1          0,0      Descendente     Ascendente          Bajo
         *   2       1,0          1,1      Descendente     Ascendente          Alto
         *   3       1,1          1,0      Ascendente      Descendente         Alto
         * --------------------------------------------------------------------------------
         * 
         * Los modos más utilizados son el 0 (0,0 También conocido como MicroWire) 
         * y el 3 (1,1). Maestro y esclavo deben estar en el mismo modo para poder 
         * comunicarse adecuadamente.
    - fase_propagacion: Definición de muestreo de datos de entrada, ya sea el final de los datos de salida o a la mitad (como en MicroWire)
    Retorno: vacío (void)
*/
#if defined (SPI_V1)
void spi_init(uint8_t rol_spi, uint8_t modo_spi, uint8_t fase_propagacion)
{
    SSPSTAT &= 0x3F;
    SSPCON1 = 0;    //Reinicio de registros asociados al módulo SSP
    SSPCON1 |= rol_spi;
    /*
    Configuración de modo SPI
    */
    //Selección de estado inactivo de reloj (idle). 1: Alto 0: Bajo
    SSPCON1bits.CKP = ((modo_spi == 2)||(modo_spi == 3))? 1:0;    
    //Selección de polaridad de reloj. 
    //Si CKP = 0 -> 1: Transmisión en flanco ascendente 0: Transmisión en flanco descendente
    //Si CKP = 1 -> 1: Transmisión en flanco descendente 0: Transmisión en flanco ascendente
    SSPSTATbits.CKE = ((modo_spi == 0)||(modo_spi == 2))? 1:0;
    SSPSTAT |= fase_propagacion;
    /*
    Configuración de pines SDO, SDI, SCK y !SS si fuera el caso.
    */
    //Pin !SS en modo esclavo
    switch( (rol_spi & 0x0F) ) //Se verifican los bits menos significativos, los cuales definen el modo maestro o esclavo
    {
        case 4:         //Modo esclavo con !SS habilitado
            #if defined SPI_IO_V1 
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo    
            TRISAbits.TRISA5 = 1;       // Pin !SS1 se configura como entrada
            #elif defined SPI_IO_V2 || defined SPI_IO_V4 
            TRISFbits.TRISF7 = 1;       // Pin !SS se configura como entrada
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V3
            TRISAbits.TRISA5 = 1;       // Pin !SS se configura como entrada
            TRISBbits.TRISB1 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V5 
            TRISCbits.TRISC6 = 1;       // Pin !SS se configura como entrada
            TRISCbits.TRISC5 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V6
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo    
            TRISAbits.TRISA5 = 1;       // Pin !SS1 se configura como entrada
            ANSEL &= 0b11101111;
            #elif defined SPI_IO_V9
            TRISBbits.TRISB6 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            TRISCbits.TRISC6 = 1;       // Pin !SS se configura como entrada
            #endif
            break;
        case 5:         //Modo esclavo con !SS deshabilitado
            #if defined (SPI_IO_V1) || defined (SPI_IO_V6) || defined (SPI_IO_V2) || defined (SPI_IO_V4)
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo    
            #elif defined SPI_IO_V3
            TRISBbits.TRISB1 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V5 
            TRISCbits.TRISC5 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V9
            TRISBbits.TRISB6 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo        
            #endif
            break;
        default:        //Modo maestro
            #if defined (SPI_IO_V1) || defined (SPI_IO_V6) || defined (SPI_IO_V2) || defined (SPI_IO_V4)
            TRISCbits.TRISC3 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro   
            #elif defined SPI_IO_V3
            TRISBbits.TRISB1 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro
            #elif defined SPI_IO_V5 
            TRISCbits.TRISC5 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro
            #elif defined SPI_IO_V9
            TRISBbits.TRISB6 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro       
            #endif
            break; 
    }
    #if defined SPI_IO_V3 
    TRISBbits.TRISB0 = 1;       // Pin SDI se configura como entrada 
    TRISCbits.TRISC7 = 0;       // Pin SDO se configura como salida
    #elif defined SPI_IO_V5
    TRISCbits.TRISC4 = 1;       // Pin SDI se configura como entrada
    TRISCbits.TRISC7 = 0;       // Pin SDO se configura como salida
    #elif defined SPI_IO_V9
    TRISCbits.TRISC7 = 0;       // Pin SDO se configura como salida
    TRISBbits.TRISB4 = 1;       // Pin SDI se configura como entrada         
    #else  
    TRISCbits.TRISC4 = 1;       // Pin SDI se configura como entrada
    TRISCbits.TRISC5 = 0;       // Pin SDO se configura como salida
    #endif
    SSPCON1bits.SSPEN=1;        //Habilita módulo MSSP
}

#endif

#if defined (SPI_V4)
void spi_init(uint8_t rol_spi, uint8_t modo_spi, uint8_t fase_propagacion) {
    SSPSTAT =0;
    SSPCON = 0;    //Reinicio de registros asociados al módulo SSP
    SSPCON |= rol_spi;
    /*
    Configuración de modo SPI
    */
    //Selección de estado inactivo de reloj (idle). 1: Alto 0: Bajo
    SSPCONbits.CKP = ((modo_spi == 2)||(modo_spi == 3))? 1:0;    
    //Selección de polaridad de reloj. 
    //Si CKP = 0 -> 1: Transmisión en flanco ascendente 0: Transmisión en flanco descendente
    //Si CKP = 1 -> 1: Transmisión en flanco descendente 0: Transmisión en flanco ascendente
    SSPSTATbits.CKE = ((modo_spi == 0)||(modo_spi == 2))? 1:0;
    SSPSTAT |= fase_propagacion;
    /*
    Configuración de pines SDO, SDI, SCK y !SS si fuera el caso.
    */
    //Pin !SS en modo esclavo
    switch( (rol_spi & 0x0F) ) //Se verifican los bits menos significativos, los cuales definen el modo maestro o esclavo
    {
        case 4:         //Modo esclavo con !SS habilitado
            #if defined SPI_IO_V1 
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo    
            TRISAbits.TRISA5 = 1;       // Pin !SS1 se configura como entrada
            #elif defined SPI_IO_V2 || defined SPI_IO_V4 
            TRISFbits.TRISF7 = 1;       // Pin !SS se configura como entrada
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V3
            TRISAbits.TRISA5 = 1;       // Pin !SS se configura como entrada
            TRISBbits.TRISB1 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V5 
            TRISCbits.TRISC6 = 1;       // Pin !SS se configura como entrada
            TRISCbits.TRISC5 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V6
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo    
            TRISAbits.TRISA5 = 1;       // Pin !SS1 se configura como entrada
            ANSEL &= 0b11101111;
            #endif
        break;
        case 5:         //Modo esclavo con !SS deshabilitado
            #if defined (SPI_IO_V1) || defined (SPI_IO_V6) || defined (SPI_IO_V2) || defined (SPI_IO_V4)
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo    
            #elif defined SPI_IO_V3
            TRISBbits.TRISB1 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V5 
            TRISCbits.TRISC5 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #endif
        break;
        default:        //Modo maestro
            #if defined (SPI_IO_V1) || defined (SPI_IO_V6) || defined (SPI_IO_V2) || defined (SPI_IO_V4)
            TRISCbits.TRISC3 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro   
            #elif defined SPI_IO_V3
            TRISBbits.TRISB1 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro
            #elif defined SPI_IO_V5 
            TRISCbits.TRISC5 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro
            #endif
        break; 
    }
    #if defined SPI_IO_V3 
        TRISBbits.TRISB0 = 1;       // Pin SDI se configura como entrada  
        TRISCbits.TRISC7 = 0;       // Pin SDI se configura como salida
    #elif defined SPI_IO_V5
        TRISCbits.TRISC4 = 1;       // Pin SDI se configura como entrada
        TRISCbits.TRISC7 = 0;       // Pin SDI se configura como salida
    #else  
        TRISCbits.TRISC4 = 1;       // Pin SDI se configura como entrada
        TRISCbits.TRISC5 = 0;       // Pin SDI se configura como salida
    #endif
    SSPCONbits.SSPEN=1;        //Habilita módulo MSSP
}

#endif

#if defined (SPI_V2) || defined (SPI_V3)
//Microcontroladores con nombres de registros al estilo SSP1CON1 y SSP1STAT
void spi1_init(uint8_t rol_spi, uint8_t modo_spi, uint8_t fase_propagacion){
    SSP1STAT =0;
    SSP1CON1 = 0;    //Reinicio de registros asociados al módulo SSP
    SSP1CON1 |= rol_spi;
    /*
    Configuración de modo SPI
    */
    //Selección de estado inactivo de reloj (idle). 1: Alto 0: Bajo
    SSP1CON1bits.CKP = ((modo_spi == 2)||(modo_spi == 3))? 1:0;    
    //Selección de polaridad de reloj. 
    //Si CKP = 0 -> 1: Transmisión en flanco ascendente 0: Transmisión en flanco descendente
    //Si CKP = 1 -> 1: Transmisión en flanco descendente 0: Transmisión en flanco ascendente
    SSP1STATbits.CKE = ((modo_spi == 0)||(modo_spi == 2))? 1:0;
    SSP1STAT |= fase_propagacion;
    /*
    Configuración de pines SDO, SDI, SCK y !SS si fuera el caso.
    */
    //Pin !SS en modo esclavo
    switch( (rol_spi & 0x0F) ) //Se verifican los bits menos significativos, los cuales definen el modo maestro o esclavo
    {
        case 4:         //Modo esclavo con !SS habilitado
            #if defined SPI_IO_V1 
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo  
            TRISAbits.TRISA5 = 1;       // Pin !SS1 se configura como entrada
            #elif defined SPI_IO_V2 
            TRISFbits.TRISF7 = 1;       // Pin !SS1 se configura como entrada
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #endif
            break;
        case 5:         //Modo esclavo con !SS deshabilitado
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo  
            break;
        default:        //Modo maestro
            TRISCbits.TRISC3 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro
            break; 
    }
    TRISCbits.TRISC4 = 1;       // Pin SDI se configura como entrada
    TRISCbits.TRISC5 = 0;       // Pin SDO se configura como salida
    SSP1CON1bits.SSPEN=1;        //Habilita módulo MSSP
}

#endif 

#if defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
void spi1_init(uint8_t rol_spi, uint8_t modo_spi, uint8_t fase_propagacion) {
    SSP1STAT =0;
    SSP1CON1 = 0;    //Reinicio de registros asociados al módulo SSP
    SSP1CON1 |= rol_spi;
    /*
    Configuración de modo SPI
    */
    //Selección de estado inactivo de reloj (idle). 1: Alto 0: Bajo
    SSP1CON1bits.CKP = ((modo_spi == 2)||(modo_spi == 3))? 1:0;    
    //Selección de polaridad de reloj. 
    //Si CKP = 0 -> 1: Transmisión en flanco ascendente 0: Transmisión en flanco descendente
    //Si CKP = 1 -> 1: Transmisión en flanco descendente 0: Transmisión en flanco ascendente
    SSP1STATbits.CKE = ((modo_spi == 0)||(modo_spi == 2))? 1:0;
    SSP1STAT |= fase_propagacion;
    /*
    Configuración de pines SDO, SDI, SCK y !SS si fuera el caso.
    */
    //Pin !SS en modo esclavo
    switch( (rol_spi & 0x0F) ) { //Se verifican los bits menos significativos, los cuales definen el modo maestro o esclavo
        case 4:         //Modo esclavo con !SS habilitado
            #if defined SPI_IO_V15 
            TRISAbits.TRISA5 = 1;       // Pin !SS1 se configura como entrada
            TRISBbits.TRISB2 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo    
            #elif defined SPI_IO_V11 || defined (SPI_IO_V12) || defined (SPI_IO_V13)
            TRISAbits.TRISA5 = 1;       // Pin !SS1 se configura como entrada
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V14
            TRISAbits.TRISA5 = 1;       // // Pin !SS1 se configura como entrada
            TRISBbits.TRISB4 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V10
            TRISFbits.TRISF7 = 1;       // // Pin !SS1 se configura como entrada
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #endif
            break;
        case 5:         //Modo esclavo con !SS deshabilitado
            #if defined SPI_IO_V15 
            TRISBbits.TRISB2 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo    
            #elif defined SPI_IO_V11 || defined (SPI_IO_V12) || defined (SPI_IO_V13) || defined (SPI_IO_V10)
            TRISCbits.TRISC3 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V14
            TRISBbits.TRISB4 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #endif 
            break;
        default:        //Modo maestro
            #if defined SPI_IO_V15 
            TRISBbits.TRISB2 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro   
            #elif defined SPI_IO_V11 || defined (SPI_IO_V12) || defined (SPI_IO_V13) || defined (SPI_IO_V10)
            TRISCbits.TRISC3 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro
            #elif defined SPI_IO_V14
            TRISBbits.TRISB4 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro
            #endif
            break; 
    }
    #if defined SPI_IO_V15 
    TRISBbits.TRISB0 = 1;       // Pin SDI se configura como entrada
    TRISBbits.TRISB3 = 0;       // Pin SDO se configura como salida 
    #elif defined SPI_IO_V11 || defined (SPI_IO_V12) || defined (SPI_IO_V13) || defined (SPI_IO_V10)
    TRISCbits.TRISC4 = 1;       // Pin SDI se configura como entrada
    TRISCbits.TRISC5 = 0;       // Pin SDO se configura como salida
    #elif defined SPI_IO_V14
    TRISBbits.TRISB5 = 1;       // Pin SDI se configura como entrada
    TRISCbits.TRISC7 = 0;       // Pin SDO se configura como salida
    #endif

    SSP1CON1bits.SSPEN=1;        //Habilita módulo MSSP
}

#endif

#if defined (SPI_V3)
void spi2_init(uint8_t rol_spi, uint8_t modo_spi, uint8_t fase_propagacion) {
    SSP2STAT =0;
    SSP2CON1 = 0;    //Reinicio de registros asociados al módulo SSP
    SSP2CON1 |= rol_spi;
    /*
    Configuración de modo SPI
    */
    //Selección de estado inactivo de reloj (idle). 1: Alto 0: Bajo
    SSP2CON1bits.CKP = ((modo_spi == 2)||(modo_spi == 3))? 1:0;    
    //Selección de polaridad de reloj. 
    //Si CKP = 0 -> 1: Transmisión en flanco ascendente 0: Transmisión en flanco descendente
    //Si CKP = 1 -> 1: Transmisión en flanco descendente 0: Transmisión en flanco ascendente
    SSP2STATbits.CKE = ((modo_spi == 0)||(modo_spi == 2))? 1:0;
    SSP2STAT |= fase_propagacion;
    /*
    Configuración de pines SDO, SDI, SCK y !SS si fuera el caso.
    */
    //Pin !SS en modo esclavo
    switch( (rol_spi & 0x0F) ) //Se verifican los bits menos significativos, los cuales definen el modo maestro o esclavo
    {
        case 4:         //Modo esclavo con !SS habilitado
            #if defined SPI_IO_V8 
            TRISDbits.TRISD3 = 1;       // Pin !SS2 se configura como entrada
            TRISDbits.TRISD0 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo   
            #else
            TRISDbits.TRISD7 = 1;       // Pin !SS2 se configura como entrada
            TRISDbits.TRISD6 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #endif
            break;
        case 5:         //Modo esclavo con !SS deshabilitado
            #if defined SPI_IO_V8 
            TRISDbits.TRISD0 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo   
            #else
            TRISDbits.TRISD6 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #endif
            break;
        default:        //Modo maestro
            #if defined SPI_IO_V8 
            TRISDbits.TRISD0 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro   
            else
            TRISDbits.TRISD6 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro
            #endif
            break; 
    }
    #if defined SPI_IO_V8 
    TRISDbits.TRISD1 = 1;       // Pin SDI se configura como entrada
    TRISDbits.TRISD2 = 0;       // Pin SDO se configura como salida 
    #else
    TRISDbits.TRISD5 = 1;       // Pin SDI se configura como entrada
    TRISDbits.TRISD4 = 0;       // Pin SDO se configura como salida
    #endif

    SSP2CON1bits.SSPEN=1;        //Habilita módulo MSSP
}

#endif


#if defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
void spi2_init(uint8_t rol_spi, uint8_t modo_spi, uint8_t fase_propagacion) {
    SSP2STAT =0;
    SSP2CON1 = 0;    //Reinicio de registros asociados al módulo SSP
    SSP2CON1 |= rol_spi;
    /*
    Configuración de modo SPI
    */
    //Selección de estado inactivo de reloj (idle). 1: Alto 0: Bajo
    SSP2CON1bits.CKP = ((modo_spi == 2)||(modo_spi == 3))? 1:0;    
    //Selección de polaridad de reloj. 
    //Si CKP = 0 -> 1: Transmisión en flanco ascendente 0: Transmisión en flanco descendente
    //Si CKP = 1 -> 1: Transmisión en flanco descendente 0: Transmisión en flanco ascendente
    SSP2STATbits.CKE = ((modo_spi == 0)||(modo_spi == 2))? 1:0;
    SSP2STAT |= fase_propagacion;
    /*
    Configuración de pines SDO, SDI, SCK y !SS si fuera el caso.
    */
    //Pin !SS en modo esclavo
    switch( (rol_spi & 0x0F) ) //Se verifican los bits menos significativos, los cuales definen el modo maestro o esclavo
    {
        case 4:         //Modo esclavo con !SS habilitado
            #if defined SPI_IO_V11 
            TRISBbits.TRISB0 = 1;       // Pin !SS2 se configura como entrada
            TRISBbits.TRISB1 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo   
            #elif defined SPI_IO_V12
            TRISDbits.TRISD3 = 1;       // Pin !SS2 se configura como entrada
            TRISDbits.TRISD0 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V10
            TRISDbits.TRISD7 = 1;       // Pin !SS2 se configura como entrada
            TRISDbits.TRISD6 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #endif
        break;
        case 5:         //Modo esclavo con !SS deshabilitado
            #if defined SPI_IO_V11 
            TRISBbits.TRISB1 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo   
            #elif defined SPI_IO_V12
            TRISDbits.TRISD0 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #elif defined SPI_IO_V10
            TRISDbits.TRISD6 = 1;       // Pin SCK se configura como entrada al tratarse de modo esclavo
            #endif   
        break;
        default:        //Modo maestro
            #if defined SPI_IO_V11 
            TRISBbits.TRISB1 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro   
            #elif defined SPI_IO_V12
            TRISDbits.TRISD0 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro
            #elif defined SPI_IO_V10
            TRISDbits.TRISD6 = 0;       // Pin SCK se configura como salida al tratarse de modo maestro
            #endif   
        break; 
    }
    #if defined SPI_IO_V11 
    TRISBbits.TRISB2 = 1;       // Pin SDI se configura como entrada
    TRISBbits.TRISB3 = 0;       // Pin SDO se configura como salida 
    #elif defined SPI_IO_V12
    TRISDbits.TRISD1 = 1;       // Pin SDI se configura como entrada
    TRISDbits.TRISD4 = 0;       // Pin SDO se configura como salida
    #elif defined SPI_IO_V10
    TRISDbits.TRISD5 = 1;       // Pin SDI se configura como entrada
    TRISDbits.TRISD4 = 0;       // Pin SDO se configura como salida
    #endif

    SSP2CON1bits.SSPEN=1;        //Habilita módulo MSSP
}

#endif

/*
    Función de escritura de un dato vía SPI.
    Parámetros:
    Dato de 8 bits a excribir en el bus SPI
    Retorno: 
    - True si la escritura fue exitosa
    - False si hubo algún error por colisión de datos verificando el bit WCOL
*/
#if defined (SPI_V1)
bool spi_writeByte(uint8_t dato_tx) //Escritura de datos sin necesidad de obtener el valor recibido
{
    uint8_t dato_basura;
    dato_basura = SSPBUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    SSPIF = 0;
    SSPCON1bits.WCOL = 0;    //Limpia cualquier condición de colisión anterior
    SSPBUF=dato_tx;         //Carga registro con dato a enviar
    if(SSPCON1bits.WCOL)     //Hay colisión de datos?
        return false;       //Si, regresa con bandera de error
    else
        while(!SSPIF){}     //No, espera mientras se transmite el dato 
    return true;            //Regresa con bandera de éxito en la transmisión
}
#endif

#if defined (SPI_V4)
bool spi_writeByte(uint8_t dato_tx) //Escritura de datos sin necesidad de obtener el valor recibido
{
    uint8_t dato_basura;
    dato_basura = SSPBUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    SSPIF = 0;
    SSPCONbits.WCOL = 0;    //Limpia cualquier condición de colisión anterior
    SSPBUF=dato_tx;         //Carga registro con dato a enviar
    if(SSPCONbits.WCOL)     //Hay colisión de datos?
        return false;       //Si, regresa con bandera de error
    else
        while(!SSPIF){}     //No, espera mientras se transmite el dato 
    return true;            //Regresa con bandera de éxito en la transmisión
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
bool spi1_writeByte(uint8_t dato_tx) { //Escritura de datos sin necesidad de obtener el valor recibido
    uint8_t dato_basura;
    dato_basura = SSP1BUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    SSP1IF = 0;
    SSP1CON1bits.WCOL = 0;    //Limpia cualquier condición de colisión anterior
    SSP1BUF = dato_tx;         //Carga registro con dato a enviar
    if(SSP1CON1bits.WCOL)     //Hay colisión de datos?
        return false;       //Si, regresa con bandera de error
    else{
        while(!SSP1IF){}     //No, espera mientras se transmite el dato 
        SSP1IF = 0;
    }
    return true;            //Regresa con bandera de éxito en la transmisión
}
#endif

#if defined (SPI_V3) || defined (SPI_V5)
bool spi2_writeByte(uint8_t dato_tx) { //Escritura de datos sin necesidad de obtener el valor recibido
    uint8_t dato_basura;
    dato_basura = SSP2BUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    SSP2IF = 0;
    SSP2CON1bits.WCOL = 0;    //Limpia cualquier condición de colisión anterior
    SSP2BUF=dato_tx;         //Carga registro con dato a enviar
    if(SSP2CON1bits.WCOL)     //Hay colisión de datos?
        return false;       //Si, regresa con bandera de error
    else{
        while(!SSP2IF){}     //No, espera mientras se transmite el dato 
        SSP2IF = 0;
    }
    return true;            //Regresa con bandera de éxito en la transmisión
}

#elif defined (SPI_V5_1) || defined (SPI_V6)

bool spi2_writeByte(uint8_t dato_tx) //Escritura de datos sin necesidad de obtener el valor recibido
{
    uint8_t dato_basura;
    dato_basura = SSP2BUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
   SSP2STATbits.BF = 0;
    SSP2BUF=dato_tx;         //Carga registro con dato a enviar
    if(SSP2CON1bits.WCOL)     //Hay colisión de datos?
        return false;       //Si, regresa con bandera de error
    else
        while(!SSP2STATbits.BF){}     //No, espera mientras se transmite el dato 
    return true;            //Regresa con bandera de éxito en la transmisión
}

#endif

/*
    Función de lectura de un dato vía SPI.
    Parámetros:
    Vacío (void)
    Retorno: dato de 8 bits proveniente del bus SPI
*/
#if defined (SPI_V1) || defined (SPI_V4)

uint8_t spi_readByte()
{
    uint8_t dato_basura;
    dato_basura = SSPBUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    PIR1bits.SSPIF=0;           //Limpia bandera de interrucpión por actividad MSSP
    SSPBUF=0;                  //Carga dato para relizar transferencia
    while(!PIR1bits.SSPIF){}    //Espera mientras se recibe dato     
    return SSPBUF;             //Obtén valor leído y regrésalo
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)

uint8_t spi1_readByte()
{
    uint8_t dato_basura;
    dato_basura = SSP1BUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    PIR1bits.SSP1IF = 0;           //Limpia bandera de interrucpión por actividad MSSP
    SSP1BUF=0xFF;                  //Carga dato para relizar transferencia
    while(!PIR1bits.SSP1IF){}    //Espera mientras se recibe dato  
    PIR1bits.SSP1IF = 0;
    return SSP1BUF;             //Obtén valor leído y regrésalo
}

#endif

#if defined (SPI_V3) || defined (SPI_V5)
uint8_t spi2_readByte()
{
    uint8_t dato_basura;
    dato_basura = SSP2BUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    PIR3bits.SSP2IF=0;           //Limpia bandera de interrucpión por actividad MSSP
    SSP2BUF=0;                  //Carga dato para relizar transferencia
    while(!PIR3bits.SSP2IF){}    //Espera mientras se recibe dato     
    return SSP2BUF;             //Obtén valor leído y regrésalo
}
#endif

#if defined (SPI_V5_1) || defined (SPI_V6)
uint8_t spi2_readByte()
{
    uint8_t dato_basura;
    dato_basura = SSP2BUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    PIR2bits.SSP2IF=0;           //Limpia bandera de interrucpión por actividad MSSP
    SSP2BUF=0;                  //Carga dato para relizar transferencia
    while(!PIR2bits.SSP2IF){}    //Espera mientras se recibe dato     
    return SSP2BUF;             //Obtén valor leído y regrésalo
}
#endif

/*
    Función de escritura y lectura simultáneas de datos vía SPI.
    Parámetros:
    Dato de 8 bits a escribir en el bus SPI
    Retorno: Dato de 8 bits proveniente del bus SPI 
    Nota: no es posible la verificación de errores de colisión directamente mediante un valor de retorno, aunque se toman 
    precauciones para minimizar probabilidad de error.
*/
#if defined (SPI_V1)
uint8_t spi_xmit(uint8_t dato_tx) {
    uint8_t dato_auxiliar = SSPBUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    SSPCON1bits.WCOL = 0;       //Limpia cualquier condición de colisión anterior
    SSPBUF=dato_tx;             //Carga registro con dato a enviar
    while(!SSPSTATbits.BF){}    //Espera mientras se transmite el dato}
    return SSPBUF;              //Obtén valor leído y regrésalo
}
#elif defined (SPI_V4)
uint8_t spi_xmit(uint8_t dato_tx) {
    uint8_t dato_auxiliar = SSPBUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    SSPCONbits.WCOL = 0;       //Limpia cualquier condición de colisión anterior
    SSPBUF=dato_tx;             //Carga registro con dato a enviar
    while(!SSPSTATbits.BF){}    //Espera mientras se transmite el dato}
    return SSPBUF;              //Obtén valor leído y regrésalo
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
uint8_t spi1_xmit(uint8_t dato_tx) {
    uint8_t dato_auxiliar = SSP1BUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    SSP1CON1bits.WCOL = 0;       //Limpia cualquier condición de colisión anterior
    SSP1BUF = dato_tx;             //Carga registro con dato a enviar
    while(!SSP1STATbits.BF){}    //Espera mientras se transmite el dato
    return SSP1BUF;              //Obtén valor leído y regrésalo
}
#endif

#if defined (SPI_V3) || defined (SPI_V5)
uint8_t spi2_xmit(uint8_t dato_tx) {
    uint8_t dato_auxiliar = SSP2BUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    SSP2CON1bits.WCOL = 0;       //Limpia cualquier condición de colisión anterior
    SSP2BUF=dato_tx;             //Carga registro con dato a enviar
    while(!SSP2STATbits.BF){}    //Espera mientras se transmite el dato}
    return SSP2BUF;              //Obtén valor leído y regrésalo
}
#elif defined (SPI_V5_1) || defined (SPI_V6)
uint8_t spi2_xmit(uint8_t dato_tx) {
    uint8_t dato_auxiliar = SSP2BUF;   //Lectura de buffer para limpia bandera de BF (Buffer Full) por hardware automáticamente
    SSP2BUF=dato_tx;             //Carga registro con dato a enviar
    while(!SSP2STATbits.BF){}    //Espera mientras se transmite el dato}
    return SSP2BUF;              //Obtén valor leído y regrésalo
}
#endif

#if defined (SPI_V1) || defined (SPI_V4)
/**
 * @brief Función para escribir un dato entero de 2 bytes en el bus SPI por hardware
 * @param dato (uint16_t) Dato de 16 bits a trasmitir mediante el bus SPI por hardware
 * @return (void)
*/
void spi_writeInt16(uint16_t dato) {
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(uint16_t);i++)
    {
        spi_writeByte(*p++);
    }
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
/**
 * @brief Función para escribir un dato entero de 2 bytes en el bus SPI por hardware
 * @param dato (uint16_t) Dato de 16 bits a trasmitir mediante el bus SPI por hardware
 * @return (void)
*/
void spi1_writeInt16(uint16_t dato) {
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(uint16_t);i++) {
        spi1_writeByte(*p++);
    }
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
/**
 * @brief Función para escribir un dato entero de 2 bytes en el bus SPI por hardware
 * @param dato (uint16_t) Dato de 16 bits a trasmitir mediante el bus SPI por hardware
 * @return (void)
*/
void spi2_writeInt16(uint16_t dato) {
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(uint16_t);i++)
    {
        spi2_writeByte(*p++);
    }
}
#endif

#if defined (SPI_V1) || defined (SPI_V4)
/**
 * @brief Función para escribir un dato entero de 3 bytes en el bus SPI por hardware
 * @param dato (uint24_t) Dato de 24 bits a trasmitir mediante el bus SPI por hardware
 * @return (void)
*/
void spi_writeInt24(uint24_t dato) {
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(uint24_t);i++) {
        spi_writeByte(*p++);
    }
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
/**
 * @brief Función para escribir un dato entero de 3 bytes en el bus SPI por hardware
 * @param dato (uint24_t) Dato de 24 bits a trasmitir mediante el bus SPI por hardware
 * @return (void)
*/
void spi1_writeInt24(uint24_t dato) {
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(uint24_t);i++) {
        spi1_writeByte(*p++);
    }
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
/**
 * @brief Función para escribir un dato entero de 3 bytes en el bus SPI por hardware
 * @param dato (uint24_t) Dato de 24 bits a trasmitir mediante el bus SPI por hardware
 * @return (void)
*/
void spi2_writeInt24(uint24_t dato) {
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(uint24_t);i++) {
        spi2_writeByte(*p++);
    }
}
#endif

#if defined (SPI_V1) || defined (SPI_V4)
/**
 * @brief Función para escribir un dato entero de 4 bytes en el bus SPI por hardware
 * @param dato (uint32_t) Dato de 32 bits a trasmitir mediante el bus SPI por hardware
 * @return (void)
*/
void spi_writeInt32(uint32_t dato) {
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(uint32_t);i++) {
        spi_writeByte(*p++);
    }
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
/**
 * @brief Función para escribir un dato entero de 4 bytes en el bus SPI por hardware
 * @param dato (uint32_t) Dato de 32 bits a trasmitir mediante el bus SPI por hardware
 * @return (void)
*/
void spi1_writeInt32(uint32_t dato) {
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(uint32_t);i++) {
        spi1_writeByte(*p++);
    }
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
/**
 * @brief Función para escribir un dato entero de 4 bytes en el bus SPI por hardware
 * @param dato (uint32_t) Dato de 32 bits a trasmitir mediante el bus SPI por hardware
 * @return (void)
*/
void spi2_writeInt32(uint32_t dato) {
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(uint32_t);i++) {
        spi2_writeByte(*p++);
    }
}
#endif

/*
    Función para escribir un dato flotante de 4 bytes (o 3) en el bus SPI por hardware
    Parámetros: 
    dato: Dato flotante de 32(24) bits a trasmitir mediante el bus SPI por hardware
    Retorno: Vacío (void)
*/
#if defined (SPI_V1) || defined (SPI_V4)
void spi_writeFloat(float dato)
{
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(float);i++)
    {
        spi_writeByte(*p++);
    }
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
void spi1_writeFloat(float dato)
{
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(float);i++)
    {
        spi1_writeByte(*p++);
    }
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
void spi2_writeFloat(float dato)
{
    uint8_t * p = (uint8_t *)&dato;
    uint8_t i;
    for(i=0;i!=sizeof(float);i++)
    {
        spi2_writeByte(*p++);
    }
}
#endif

/*
    Función para leer un dato entero de 2 bytes desde el bus SPI por hardware
    Parámetros: 
    dato: Dato de 16 bits a transmitir para generar pulsos de reloj adecuados. Es posible que sea cero si el protocolo en la aplicación no es importante. 
    Retorno: Dato de 16 bits a recibir mediante el bus SPI por hardware
*/
#if defined (SPI_V1) || defined (SPI_V4)
uint16_t spi_readInt16(uint16_t dato_tx)
{
    uint16_t dato_leido;
    uint8_t i;
    uint8_t * p_lectura;                             //Apuntador a variable entera de 16 bits
    uint8_t * p_transmision;                         //Apuntador a la dirección del dato a transmitir
    p_lectura = (uint8_t *)&dato_leido;         //Asigna dirección de valor de retorno de tipo entero de 16 bits
    p_transmision = (uint8_t *)&dato_tx;           //Asigna dirección de dato a transmitir a su apuntador
    for(i=0;i!=sizeof(uint16_t);i++)
    {
        *(p_lectura++) = spi_xmit(*(p_transmision++));      //Transmisión/recepción simultánea de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
uint16_t spi1_readInt16(uint16_t dato_tx)
{
    uint16_t dato_leido;
    uint8_t i;
    uint8_t * p_lectura;                             //Apuntador a variable entera de 16 bits
    uint8_t * p_transmision;                         //Apuntador a la dirección del dato a transmitir
    p_lectura = (uint8_t *)&dato_leido;            //Asigna dirección de valor de retorno de tipo entero de 16 bits
    p_transmision = (uint8_t *)&dato_tx;           //Asigna dirección de dato a transmitir a su apuntador
    for(i=0;i!=sizeof(uint16_t);i++)
    {
        *(p_lectura++) = spi1_xmit(*(p_transmision++));      //Transmisión/recepción simultánea de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
uint16_t spi2_readInt16(uint16_t dato_tx)
{
    uint16_t dato_leido;
    uint8_t i;
    uint8_t * p_lectura;                             //Apuntador a variable entera de 16 bits
    uint8_t * p_transmision;                         //Apuntador a la dirección del dato a transmitir
    p_lectura = (uint8_t *)&dato_leido;            //Asigna dirección de valor de retorno de tipo entero de 16 bits
    p_transmision = (uint8_t *)&dato_tx;           //Asigna dirección de dato a transmitir a su apuntador
    for(i=0;i!=sizeof(uint16_t);i++)
    {
        *(p_lectura++) = spi2_xmit(*(p_transmision++));      //Transmisión/recepción simultánea de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído
}
#endif


/*
    Función para leer un dato entero de 3 bytes desde el bus SPI por hardware
    Parámetros: 
    dato: Dato de 24 bits a transmitir para generar pulsos de reloj adecuados. Es posible que sea cero si el protocolo en la aplicación no es importante. 
    Retorno: Dato de 24 bits a recibir mediante el bus SPI por hardware
*/
#if defined (SPI_V1) || defined (SPI_V4)
uint24_t spi_readInt24(uint24_t dato_tx)
{
    uint24_t dato_leido;
    uint8_t i;
    uint8_t * p_lectura;                             //Apuntador a variable entera de 24 bits
    uint8_t * p_transmision;                         //Apuntador a la dirección del dato a transmitir
    p_lectura = (uint8_t *)&dato_leido;            //Asigna dirección de valor de retorno de tipo entero de 24 bits
    p_transmision = (uint8_t *)&dato_tx;            //Asigna dirección de dato a transmitir a su apuntador
    for(i=0;i!=sizeof(uint24_t);i++)
    {
        *(p_lectura++) = spi_xmit(*(p_transmision++));      //Transmisión/recepción simultánea de datos
    }
    return dato_leido;    //Retorno de valor de 24 bits leído
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
uint24_t spi1_readInt24(uint24_t dato_tx)
{
    uint24_t dato_leido;
    uint8_t i;
    uint8_t * p_lectura;                             //Apuntador a variable entera de 24 bits
    uint8_t * p_transmision;                         //Apuntador a la dirección del dato a transmitir
    p_lectura = (uint8_t *)&dato_leido;            //Asigna dirección de valor de retorno de tipo entero de 24 bits
    p_transmision = (uint8_t *)&dato_tx;           //Asigna dirección de dato a transmitir a su apuntador
    for(i=0;i!=sizeof(uint24_t);i++)
    {
        *(p_lectura++) = spi1_xmit(*(p_transmision++));      //Transmisión/recepción simultánea de datos
    }
    return dato_leido;      //Retorno de valor de 24 bits leído
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
uint24_t spi2_readInt24(uint24_t dato_tx)
{
    uint24_t dato_leido;
    uint8_t i;
    uint8_t * p_lectura;                             //Apuntador a variable entera de 24 bits
    uint8_t * p_transmision;                         //Apuntador a la dirección del dato a transmitir
    p_lectura = (uint8_t *)&dato_leido;            //Asigna dirección de valor de retorno de tipo entero de 24 bits
    p_transmision = (uint8_t *)&dato_tx;           //Asigna dirección de dato a transmitir a su apuntador
    for(i=0;i!=sizeof(uint24_t);i++)
    {
        *(p_lectura++) = spi2_xmit(*(p_transmision++));      //Transmisión/recepción simultánea de datos
    }
    return dato_leido;          //Retorno de valor de 24 bits leído
}
#endif


/*
    Función para leer un dato entero de 4 bytes desde el bus SPI por hardware
    Parámetros: 
    dato: Dato de 32 bits a transmitir para generar pulsos de reloj adecuados. Es posible que sea cero si el protocolo en la aplicación no es importante. 
    Retorno: Dato de 32 bits a recibir mediante el bus SPI por hardware
*/
#if defined (SPI_V1) || defined (SPI_V4)
uint32_t spi_readInt32(uint32_t dato_tx)
{
    uint32_t dato_leido;
    uint8_t i;
    uint8_t * p_lectura;                             //Apuntador a variable entera de 32 bits
    uint8_t * p_transmision;                         //Apuntador a la dirección del dato a transmitir
    p_lectura = (uint8_t *)&dato_leido;            //Asigna dirección de valor de retorno de tipo entero de 32 bits
    p_transmision = (uint8_t *)&dato_tx;           //Asigna dirección de dato a transmitir a su apuntador
    for(i=0;i!=sizeof(uint32_t);i++)
    {
        *(p_lectura++) = spi_xmit(*(p_transmision++));      //Transmisión/recepción simultánea de datos
    }
    return dato_leido;              //Retorno de valor de 32 bits leído
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
uint32_t spi1_readInt32(uint32_t dato_tx)
{
    uint32_t dato_leido;
    uint8_t i;
    uint8_t * p_lectura;                             //Apuntador a variable entera de 32 bits
    uint8_t * p_transmision;                         //Apuntador a la dirección del dato a transmitir
    p_lectura = (uint8_t *)&dato_leido;            //Asigna dirección de valor de retorno de tipo entero de 32 bits
    p_transmision = (uint8_t *)&dato_tx;           //Asigna dirección de dato a transmitir a su apuntador
    for(i=0;i!=sizeof(uint32_t);i++)
    {
        *(p_lectura++) = spi1_xmit(*(p_transmision++));      //Transmisión/recepción simultánea de datos
    }
    return dato_leido;                          //Retorno de valor de 32 bits leído
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
uint32_t spi2_readInt32(uint32_t dato_tx)
{
    uint32_t dato_leido;
    uint8_t i;
    uint8_t * p_lectura;                             //Apuntador a variable entera de 32 bits
    uint8_t * p_transmision;                         //Apuntador a la dirección del dato a transmitir
    p_lectura = (uint8_t *)&dato_leido;            //Asigna dirección de valor de retorno de tipo entero de 32 bits
    p_transmision = (uint8_t *)&dato_tx;           //Asigna dirección de dato a transmitir a su apuntador
    for(i=0;i!=sizeof(uint32_t);i++)
    {
        *(p_lectura++) = spi2_xmit(*(p_transmision++));      //Transmisión/recepción simultánea de datos
    }
    return dato_leido;                                  //Retorno de valor de 32 bits leído
}
#endif

/*
    Función para leer un dato flotante de 4 bytes (o 3) desde el bus SPI por hardware
    Parámetros: 
    Ninguno 
    Retorno: Dato flotante de 32(24) bits a recibir mediante el bus SPI por hardware
*/

#if defined (SPI_V1) || defined (SPI_V4)
float spi_readFloat()
{
    float dato;
    uint8_t i;
    uint8_t * p; //Apuntador a variable float
    p = (uint8_t*)&dato; //Asigna dirección de valor de retorno de tipo flotante
    for(i=0;i!=sizeof(float);i++)
    {
        *p++ = spi_xmit(0);
    }
    return dato;            //Retorno de valor flotante leído
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
float spi1_readFloat()
{
    float dato;
    uint8_t i;
    uint8_t * p; //Apuntador a variable float
    p = (uint8_t*)&dato; //Asigna dirección de valor de retorno de tipo flotante
    for(i=0;i!=sizeof(float);i++)
    {
        *p++ = spi1_xmit(0);
    }
    return dato;                //Retorno de valor flotante leído
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
float spi2_readFloat()
{
    float dato;
    uint8_t i;
    uint8_t * p; //Apuntador a variable float
    p = (uint8_t*)&dato; //Asigna dirección de valor de retorno de tipo flotante
    for(i=0;i!=sizeof(float);i++)
    {
        *p++ = spi2_xmit(0);
    }
    return dato;                    //Retorno de valor flotante leído
}
#endif


/*
    Funciones para habilitar/deshabilitar el módulo MSSP SPI con fines de ahorro de energía
*/
#if defined (SPI_V1)
void spi_enable()
{
    SSPCON1bits.SSPEN=1;
}
#endif
#if defined (SPI_V4)
void spi_enable()
{
    SSPCONbits.SSPEN=1;
}
#endif


#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
void spi1_enable()
{
    SSP1CON1bits.SSPEN=1;
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
void spi2_enable()
{
    SSP2CON1bits.SSPEN=1;
}
#endif


#if defined (SPI_V1)
void spi_disable()
{
    SSPCON1bits.SSPEN=0;
}
#endif
#if defined (SPI_V4)
void spi_disable()
{
    SSPCONbits.SSPEN=0;
}
#endif


#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
void spi1_disable()
{
    SSP1CON1bits.SSPEN=0;
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
void spi2_disable()
{
    SSP2CON1bits.SSPEN=0;
}
#endif

#if defined (SPI_V1) || defined (SPI_V4)
void spi_setSpeed(uint16_t spi_speed_kHz) {
    SSPADD = (uint8_t)(division_entera_sin_signo(_XTAL_FREQ,4*(uint32_t)(spi_speed_kHz)*1000UL)-1); //Calcula registro SSPADD con base en baud_rate_requerido en [kHz]
}
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
void spi1_setSpeed(uint16_t spi_speed_kHz) {
    SSP1ADD = (uint8_t)(division_entera_sin_signo(_XTAL_FREQ,4*(uint32_t)(spi_speed_kHz)*1000UL)-1); //Calcula registro SSP1ADD con base en baud_rate_requerido en [kHz]
}
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)
void spi2_setSpeed(uint16_t spi_speed_kHz) {
    SSP2ADD = (uint8_t)(division_entera_sin_signo(_XTAL_FREQ,4*(uint32_t)(spi_speed_kHz)*1000UL)-1); //Calcula registro SSP2ADD con base en baud_rate_requerido en [kHz]
}
#endif
