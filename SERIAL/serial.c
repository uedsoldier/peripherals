/**
 * @file serial.c
 * @brief Librería para utilización de puerto serial por hardware para microcontroladores PIC de 8 bits
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/

#include "serial.h"

//USART1
// Solo cuentan con registro SPBRG, por lo que se utiliza solamente la opcion del bit BRGH si fuera necesario. No cuentan con registro BAUDCON, 
// por lo que no cuentan con generador de baud rate de 16 bits. A su vez solo cuentan con un módulo USART
#if defined (AUSART_V1)

/**
  * @brief Función de inicialización de módulo serial. La función reinicia los registros aspciados al módulo USART1 a su estado de RESET (POR).
  * Se configura modo síncrono/asíncrono, modo de 8/9 bits, modo esclavo/maestro (en modo síncrono), etc. Las definiciones de bits de configuración
  * se encuentran en el archivo serial.h
  * @param param_config: (uint8_t) Definiciones de bits para configuración de USART1
  * @param baud_rate: (uint32_t) Velocidad de comunicación en bits por segundo (bps). Algunos valores estándar: 110, 150, 300, 1200, 2400, 4800, 7200,
  * 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600
  * @param offset_calibracion: (int16_t) Número entero para calibración y corrección de registro SPBRG. Si no se desea hacer uso de esta característica,
  * mandar 0,
  * @return (void)
*/
void serial_init(uint8_t param_config, uint32_t baud_rate, int16_t offset_calibracion)
{
    int32_t spbrg_aux; //Variable auxiliar para cálculo de registro SPBRG con base en el valor de baud rate
    #ifdef SERIAL1_RX_BUFFER //inicialización de índices del buffer serial, si es que se utilizará
    serial1_queueEnd     = -1;
    serial1_queueFront   = -1;
    #endif
    TXSTA = 0;          
    RCSTA = 0;         //Reinicio de los registros de control de la EUSART a su valor default en caso de que se utilice algún bootloader
    SPBRGH = 0;
    SPBRG = 0;
    BAUDCONbits.BRG16 = 0;
    //Configuración de registros TXSTA y RCSTA
    if( param_config & 0x80 ) //Modo síncrono (1) o asíncrono (0)?
    {
        TXSTAbits.SYNC = 1;
        SPBRG = division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate); //En modo síncrono, esta es la manera de calcular el registro SPBRG
        if ( param_config & 0x40 ) //Modo síncrono maestro (1) o esclavo (0)?
            TXSTAbits.CSRC = 1;
    }
    else //Modo asíncrono
    {
        //Cálculo automático de registros para generación de baud rate
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-64*baud_rate,64*baud_rate);      //Se hace uso de esta macro que realiza redondeo para minimizar error de cálculo
        if(spbrg_aux>255)
        {
            spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-16*baud_rate,16*baud_rate);
            TXSTAbits.BRGH=1; //Para altos baud rates
        }
        if(spbrg_aux<256)
        {
            TXSTAbits.BRGH=0; //Para bajos baud rates
            SPBRG=(uint8_t)spbrg_aux;
        }
    }
    if( param_config & 0x20 ) //Modo de 9 bits(1) u 8 bits (0)?
    {
        TXSTAbits.TX9 = 1;
        RCSTAbits.RX9 = 1;
    }
    if( param_config & 0x08 ) //Transmisión contínua (1) o sencilla (0)?
        RCSTAbits.CREN = 1;
    if( param_config & 0x04 ) //Detección automática de dirección (1) o no (0)?
        RCSTAbits.ADDEN = 1;


    TXSTAbits.TXEN = 1;  // Habilita transmisor
    RCSTAbits.SPEN = 1;  // Habilita receptor
    TRISCbits.TRISC7 = 1;  //RX es RC7
    TRISCbits.TRISC6 = (TXSTAbits.SYNC && !TXSTAbits.CSRC)? 1:0; //Modo esclavo síncrono o TX es RC6   
 
}

//Cuentan con un solo módulo USART, pero con el registro SPBRGH. Poseen registro BAUDCON, permitiéndoles generar baud rate de 16 bits
#elif defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5) 

/**
 * @brief Función de inicialización de módulo serial. La función reinicia los registros aspciados al módulo USART1 a su estado de RESET (POR).
 * Se configura modo síncrono/asíncrono, modo de 8/9 bits, modo esclavo/maestro (en modo síncrono), etc. Las definiciones de bits de configuración
 * se encuentran en el archivo serial.h
 * @param param_config: (uint8_t) Definiciones de bits para configuración de USART1
 * @param baud_rate: (uint32_t) Velocidad de comunicación en bits por segundo (bps). Algunos valores estándar: 110, 150, 300, 1200, 2400, 4800, 7200,
 * 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600
 * @param offset_calibracion: (int16_t) Número entero para calibración y corrección de registro SPBRG. Si no se desea hacer uso de esta característica,
 * mandar 0.
 * @return (void)
*/
void serial_init(uint8_t param_config, uint32_t baud_rate, int16_t offset_calibracion)
{
    int32_t spbrg_aux; //Variable auxiliar para cálculo de registro SPBRG con base en el valor de baud rate
    #ifdef SERIAL_RX_BUFFER //inicialización de índices del buffer serial, si es que se utilizará
    serial_queueEnd     = -1;
    serial_queueFront   = -1;
    #endif
    TXSTA = 0;          
    RCSTA = 0;         //Reinicio de los registros de control de la EUSART a su valor default en caso de que se utilice algún bootloader
    SPBRGH = 0;
    SPBRG = 0;
    BAUDCONbits.BRG16 = 0;
    //Configuración de registros TXSTA y RCSTA
    if( param_config & 0x80 ) //Modo síncrono (1) o asíncrono (0)?
    {
        TXSTAbits.SYNC = 1;
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate); //En modo síncrono, esta es la manera de calcular el registro SPBRG
        SPBRG = make8(spbrg_aux,0);
        SPBRGH = make8(spbrg_aux,1);
        if ( param_config & 0x40 ) //Modo síncrono maestro (1) o esclavo (0)?
            TXSTAbits.CSRC = 1;
    }
    else //Modo asíncrono
    {
        //Cálculo automático de registros para generación de baud rate
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-64*baud_rate,64*baud_rate);      //Se hace uso de esta macro que realiza redondeo para minimizar error de cálculo
        if(spbrg_aux>255)
        {
            spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate);
            SPBRG = make8(spbrg_aux,0);
            SPBRGH = make8(spbrg_aux,1);
            TXSTAbits.BRGH=1; //Para altos baud rates
            BAUDCONbits.BRG16 = 1;  //Generador de baud rate de 16 bits. En caso de no cumplir con baud rate requerido, podrá probarse con este bit en cero 
            //y la división anterior con 16 en lugar de 4. En casi cualquier caso esto no será necesario, pues el error relativo porcentual con respecto al
            //baud rate deseado es menor con el generador de 16 bits.
        }
        if(spbrg_aux<256)
        {
            TXSTAbits.BRGH=0; //Para bajos baud rates
            SPBRGH = 0;
            SPBRG=(uint8_t)spbrg_aux;
        }
    }
    if( param_config & 0x20 ) //Modo de 9 bits(1) u 8 bits (0)?
    {
        TXSTAbits.TX9 = 1;
        RCSTAbits.RX9 = 1;
    }
    if( param_config & 0x08 ) //Transmisión contínua (1) o sencilla (0)?
        RCSTAbits.CREN = 1;
    if( param_config & 0x04 ) //Detección automática de dirección (1) o no (0)?
        RCSTAbits.ADDEN = 1;

    TXSTAbits.TXEN = 1;  // Habilita transmisor
    RCSTAbits.SPEN = 1;  // Habilita receptor
    TRISCbits.TRISC7 = 1;  //RX es RC7
    TRISCbits.TRISC6 = (TXSTAbits.SYNC && !TXSTAbits.CSRC)? 1:0; //Modo esclavo síncrono o TX es RC6   
 
}

#endif

//Solo cuentan con registro SPBRG, por lo que se utiliza solamente la opcion del bit BRGH si fuera necesario. No cuentan con registro BAUDCON, 
//por lo que no cuentan con generador de baud rate de 16 bits. Cuentan con dos módulos USART
#if defined (AUSART_V2) 
/**
 * @brief Función de inicialización de módulo serial. La función reinicia los registros aspciados al módulo USART1 a su estado de RESET (POR).
 * Se configura modo síncrono/asíncrono, modo de 8/9 bits, modo esclavo/maestro (en modo síncrono), etc. Las definiciones de bits de configuración
 * se encuentran en el archivo serial.h
 * @param param_config: (uint8_t) Definiciones de bits para configuración de USART1
 * @param baud_rate: (uint32_t) Velocidad de comunicación en bits por segundo (bps). Algunos valores estándar: 110, 150, 300, 1200, 2400, 4800, 7200,
 * 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600
 * @param offset_calibracion: (int16_t) Número entero para calibración y corrección de registro SPBRG. Si no se desea hacer uso de esta característica,
 * mandar 0.
 * @return (void)
*/
void serial1_init(uint8_t param_config, uint32_t baud_rate, int16_t offset_calibracion)
{
    int32_t spbrg_aux; //Variable auxiliar para cálculo de registro SPBRG con base en el valor de baud rate
    #ifdef SERIAL1_RX_BUFFER //inicialización de índices del buffer serial, si es que se utilizará
    serial1_queueEnd     = -1;
    serial1_queueFront   = -1;
    #endif
    TXSTA1 = 0;          
    RCSTA1 = 0;         //Reinicio de los registros de control de la EUSART a su valor default en caso de que se utilice algún bootloader
    SPBRGH1= 0;
    SPBRG1 = 0;
    BAUDCON1bits.BRG16 = 0;
    //Configuración de registros TXSTA y RCSTA
    if( param_config & 0x80 ) //Modo síncrono (1) o asíncrono (0)?
    {
        TXSTA1bits.SYNC = 1;
        SPBRG1 = division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate); //En modo síncrono, esta es la manera de calcular el registro SPBRG
        if ( param_config & 0x40 ) //Modo síncrono maestro (1) o esclavo (0)?
            TXSTA1bits.CSRC = 1;
    }
    else //Modo asíncrono
    {
        //Cálculo automático de registros para generación de baud rate
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-64*baud_rate,64*baud_rate);      //Se hace uso de esta macro que realiza redondeo para minimizar error de cálculo
        if(spbrg_aux>255)
        {
            spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-16*baud_rate,16*baud_rate);
            TXSTA1bits.BRGH=1; //Para altos baud rates
        }
        if(spbrg_aux<256)
        {
            TXSTA1bits.BRGH=0; //Para bajos baud rates
            SPBRG1=(uint8_t)spbrg_aux;
        }
    }
    if( param_config & 0x20 ) //Modo de 9 bits(1) u 8 bits (0)?
    {
        TXSTA1bits.TX9 = 1;
        RCSTA1bits.RX9 = 1;
    }
    if( param_config & 0x08 ) //Transmisión contínua (1) o sencilla (0)?
        RCSTA1bits.CREN = 1;
    if( param_config & 0x04 ) //Detección automática de dirección (1) o no (0)?
        RCSTA1bits.ADDEN = 1;

    TXSTA1bits.TXEN = 1;  // Habilita transmisor
    RCSTA1bits.SPEN = 1;  // Habilita receptor
    TRISCbits.TRISC7 = 1;  //RX es RC7
    TRISCbits.TRISC6 = (TXSTA1bits.SYNC && !TXSTA1bits.CSRC)? 1:0; //Modo esclavo síncrono o TX es RC6
}

//Cuentan con dos módulos USART y con los registros SPBRGH1 y 2. Poseen registro BAUDCONx, permitiéndoles generar baud rate de 16 bits
#elif  defined (EAUSART_V6) || defined (EAUSART_V7) || defined (EAUSART_V8) ||\
       defined (EAUSART_V9) || defined (EAUSART_V10)
/**
 * @brief Función de inicialización de módulo serial. La función reinicia los registros aspciados al módulo USART1 a su estado de RESET (POR).
 * Se configura modo síncrono/asíncrono, modo de 8/9 bits, modo esclavo/maestro (en modo síncrono), etc. Las definiciones de bits de configuración
 * se encuentran en el archivo serial.h
 * @param param_config: (uint8_t) Definiciones de bits para configuración de USART1
 * @param baud_rate: (uint32_t) Velocidad de comunicación en bits por segundo (bps). Algunos valores estándar: 110, 150, 300, 1200, 2400, 4800, 7200,
 * 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600
 * @param offset_calibracion: (int16_t) Número entero para calibración y corrección de registro SPBRG. Si no se desea hacer uso de esta característica,
 * mandar 0.
 * @return (void)
*/
void serial1_init(uint8_t param_config, uint32_t baud_rate, int16_t offset_calibracion)
{
    int32_t spbrg_aux; //Variable auxiliar para cálculo de registro SPBRG con base en el valor de baud rate
    #ifdef SERIAL1_RX_BUFFER //inicialización de índices del buffer serial, si es que se utilizará
    serial1_queueEnd     = -1;
    serial1_queueFront   = -1;
    #endif
    TXSTA1 = 0;          
    RCSTA1 = 0;         //Reinicio de los registros de control de la EUSART a su valor default en caso de que se utilice algún bootloader
    SPBRGH1 = 0;
    SPBRG1 = 0;
    BAUDCON1bits.BRG16 = 0;
    //Configuración de registros TXSTA y RCSTA
    if( param_config & 0x80 ) //Modo síncrono (1) o asíncrono (0)?
    {
        TXSTA1bits.SYNC = 1;
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate); //En modo síncrono, esta es la manera de calcular el registro SPBRG
        SPBRG1 = make8(spbrg_aux,0);
        SPBRGH1 = make8(spbrg_aux,1);
        if ( param_config & 0x40 ) //Modo síncrono maestro (1) o esclavo (0)?
            TXSTA1bits.CSRC = 1;
    }
    else //Modo asíncrono
    {
        //Cálculo automático de registros para generación de baud rate
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-64*baud_rate,64*baud_rate);      //Se hace uso de esta macro que realiza redondeo para minimizar error de cálculo
        if(spbrg_aux>255)
        {
            spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate);
            SPBRG1 = make8(spbrg_aux,0);
            SPBRGH1 = make8(spbrg_aux,1);
            TXSTA1bits.BRGH=1; //Para altos baud rates
            BAUDCON1bits.BRG16 = 1;  //Generador de baud rate de 16 bits. En caso de no cumplir con baud rate requerido, podrá probarse con este bit en cero 
            //y la división anterior con 16 en lugar de 4. En casi cualquier caso esto no será necesario, pues el error relativo porcentual con respecto al
            //baud rate deseado es menor con el generador de 16 bits.
        }
        if(spbrg_aux<256)
        {
            TXSTA1bits.BRGH=0; //Para bajos baud rates
            SPBRGH1 = 0;
            SPBRG1=(uint8_t)spbrg_aux;
        }
    }
    if( param_config & 0x20 ) //Modo de 9 bits(1) u 8 bits (0)?
    {
        TXSTA1bits.TX9 = 1;
        RCSTA1bits.RX9 = 1;
    }
    if( param_config & 0x08 ) //Transmisión contínua (1) o sencilla (0)?
        RCSTA1bits.CREN = 1;
    if( param_config & 0x04 ) //Detección automática de dirección (1) o no (0)?
        RCSTA1bits.ADDEN = 1;

    TXSTA1bits.TXEN = 1;    // Habilita transmisor
    RCSTA1bits.SPEN = 1;    // Habilita receptor
    TRISCbits.TRISC7 = 1;   //RX es RC7
    TRISCbits.TRISC6 = (TXSTA1bits.SYNC && !TXSTA1bits.CSRC)? 1:0; //Modo esclavo síncrono o TX es RC6
}

//Cuentan con dos módulos USART y con los registros SPBRGH1 y 2. Poseen registro BAUDCONx, permitiéndoles generar baud rate de 16 bits
#elif  defined (EAUSART_V11) || defined (EAUSART_V11_1) || defined (EAUSART_V12)
/**
 * @brief Función de inicialización de módulo serial. La función reinicia los registros aspciados al módulo USART1 a su estado de RESET (POR).
 * Se configura modo síncrono/asíncrono, modo de 8/9 bits, modo esclavo/maestro (en modo síncrono), etc. Las definiciones de bits de configuración
 * se encuentran en el archivo serial.h
 * @param param_config: (uint8_t) Definiciones de bits para configuración de USART1
 * @param baud_rate: (uint32_t) Velocidad de comunicación en bits por segundo (bps). Algunos valores estándar: 110, 150, 300, 1200, 2400, 4800, 7200,
 * 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600
 * @param offset_calibracion: (int16_t) Número entero para calibración y corrección de registro SPBRG. Si no se desea hacer uso de esta característica,
 * mandar 0.
 * @return (void)
*/
void serial1_init(uint8_t param_config, uint32_t baud_rate, int16_t offset_calibracion)
{
    int32_t spbrg_aux; //Variable auxiliar para cálculo de registro SPBRG con base en el valor de baud rate
    #ifdef SERIAL1_RX_BUFFER //inicialización de índices del buffer serial, si es que se utilizará
    serial1_queueEnd     = -1;
    serial1_queueFront   = -1;
    #endif
    TXSTA1 = 0;          
    RCSTA1 = 0;         //Reinicio de los registros de control de la EUSART a su valor default en caso de que se utilice algún bootloader
    SPBRGH1 = 0;
    SPBRG1 = 0;
    BAUDCON1bits.BRG16 = 0;
    //Configuración de registros TXSTA y RCSTA
    if( param_config & 0x80 ) //Modo síncrono (1) o asíncrono (0)?
    {
        TXSTA1bits.SYNC = 1;
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate); //En modo síncrono, esta es la manera de calcular el registro SPBRG
        SPBRG1 = make8(spbrg_aux,0);
        SPBRGH1 = make8(spbrg_aux,1);
        if ( param_config & 0x40 ) //Modo síncrono maestro (1) o esclavo (0)?
            TXSTA1bits.CSRC = 1;
    }
    else { //Modo asíncrono
        //Cálculo automático de registros para generación de baud rate
        uint32_t br = 64*baud_rate;
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-br,br);      //Se hace uso de esta macro que realiza redondeo para minimizar error de cálculo
        if(spbrg_aux > 255) {
            br = 4*baud_rate;
            spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-br,br);
            SPBRG1 = make8(spbrg_aux,0);
            SPBRGH1 = make8(spbrg_aux,1);
            TXSTA1bits.BRGH = 1; //Para altos baud rates
            BAUDCON1bits.BRG16 = 1;  //Generador de baud rate de 16 bits. En caso de no cumplir con baud rate requerido, podrá probarse con este bit en cero 
            //y la división anterior con 16 en lugar de 4. En casi cualquier caso esto no será necesario, pues el error relativo porcentual con respecto al
            //baud rate deseado es menor con el generador de 16 bits.
        }
        if(spbrg_aux < 256){
            TXSTA1bits.BRGH=0; //Para bajos baud rates
            SPBRGH1 = 0;
            SPBRG1=(uint8_t)spbrg_aux;
        }
    }
    if( param_config & 0x20 ) //Modo de 9 bits(1) u 8 bits (0)?
    {
        TXSTA1bits.TX9 = 1;
        RCSTA1bits.RX9 = 1;
    }
    if( param_config & 0x08 ) //Transmisión contínua (1) o sencilla (0)?
        RCSTA1bits.CREN = 1;
    if( param_config & 0x04 ) //Detección automática de dirección (1) o no (0)?
        RCSTA1bits.ADDEN = 1;

    TXSTA1bits.TXEN = 1;  // Habilita transmisor
    RCSTA1bits.SPEN = 1;  // Habilita receptor
    //IMPORTANTE: En estas versiones existe el módulo PPS (Peripheral Pin Select, el cual permite que RX y TX de EUSART1 puedan ubicarse en varios pines dedicados)
    #if defined(EAUSART_V11)
    TRISCbits.TRISC7 = 1;   //RX es RC7
    TRISCbits.TRISC6 = (TXSTA1bits.SYNC && !TXSTA1bits.CSRC)? 1:0; //Modo esclavo síncrono o TX es RC6
    #endif
}

#endif

//USART2
//El módulo EUSART 2 en estos dispositivos no cuenta con registro BAUDCON
#if defined (AUSART_V2) || defined (EAUSART_V6)
/**
 * @brief Función de inicialización de módulo serial. La función reinicia los registros aspciados al módulo USART2 a su estado de RESET (POR).
 * Se configura modo síncrono/asíncrono, modo de 8/9 bits, modo esclavo/maestro (en modo síncrono), etc. Las definiciones de bits de configuración
 * se encuentran en el archivo serial.h
 * @param param_config: (uint8_t) Definiciones de bits para configuración de USART2
 * @param baud_rate: (uint32_t) Velocidad de comunicación en bits por segundo (bps). Algunos valores estándar: 110, 150, 300, 1200, 2400, 4800, 7200,
 * 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600
 * @param offset_calibracion: (int16_t) Número entero para calibración y corrección de registro SPBRG. Si no se desea hacer uso de esta característica,
 * mandar 0.
 * @return (void)
*/
void serial2_init(uint8_t param_config, uint32_t baud_rate, int16_t offset_calibracion)
{
    int32_t spbrg_aux; //Variable auxiliar para cálculo de registro SPBRG con base en el valor de baud rate
    #ifdef SERIAL2_RX_BUFFER //inicialización de índices del buffer serial, si es que se utilizará
    serial2_queueEnd     = -1;
    serial2_queueFront   = -1;
    #endif
    TXSTA2 = 0;          
    RCSTA2 = 0;         //Reinicio de los registros de control de la EUSART a su valor default en caso de que se utilice algún bootloader
    SPBRGH2 = 0;
    SPBRG2 = 0;
    BAUDCON2bits.BRG16 = 0;
    //Configuración de registros TXSTA y RCSTA
    if( param_config & 0x80 ) //Modo síncrono (1) o asíncrono (0)?
    {
        TXSTA2bits.SYNC = 1;
        SPBRG2 = division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate); //En modo síncrono, esta es la manera de calcular el registro SPBRG
        if ( param_config & 0x40 ) //Modo síncrono maestro (1) o esclavo (0)?
            TXSTA2bits.CSRC = 1;
    }
    else //Modo asíncrono
    {
        //Cálculo automático de registros para generación de baud rate
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-64*baud_rate,64*baud_rate);      //Se hace uso de esta macro que realiza redondeo para minimizar error de cálculo
        if(spbrg_aux>255)
        {
            spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-16*baud_rate,16*baud_rate);
            TXSTA2bits.BRGH=1; //Para altos baud rates
        }
        if(spbrg_aux<256)
        {
            TXSTA2bits.BRGH=0; //Para bajos baud rates
            SPBRG1=(uint8_t)spbrg_aux;
        }
    }
    if( param_config & 0x20 ) //Modo de 9 bits(1) u 8 bits (0)?
    {
        TXSTA2bits.TX9 = 1;
        RCSTA2bits.RX9 = 1;
    }
    if( param_config & 0x08 ) //Transmisión contínua (1) o sencilla (0)?
        RCSTA2bits.CREN = 1;
    if( param_config & 0x04 ) //Detección automática de dirección (1) o no (0)?
        RCSTA2bits.ADDEN = 1;

    TXSTA2bits.TXEN = 1;  // Habilita transmisor
    RCSTA2bits.SPEN = 1;  // Habilita receptor
    TRISGbits.TRISG2 = 1;  //RX es RG2
    TRISGbits.TRISG1 = (TXSTA1bits.SYNC && !TXSTA1bits.CSRC)? 1:0; //Modo esclavo síncrono o TX es RG1
}
#elif defined (EAUSART_V7) || defined (EAUSART_V8) || defined (EAUSART_V9)
/**
 * @brief Función de inicialización de módulo serial. La función reinicia los registros aspciados al módulo USART2 a su estado de RESET (POR).
 * Se configura modo síncrono/asíncrono, modo de 8/9 bits, modo esclavo/maestro (en modo síncrono), etc. Las definiciones de bits de configuración
 * se encuentran en el archivo serial.h
 * @param param_config: (uint8_t) Definiciones de bits para configuración de USART2
 * @param baud_rate: (uint32_t) Velocidad de comunicación en bits por segundo (bps). Algunos valores estándar: 110, 150, 300, 1200, 2400, 4800, 7200,
 * 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600
 * @param offset_calibracion: (int16_t) Número entero para calibración y corrección de registro SPBRG. Si no se desea hacer uso de esta característica,
 * mandar 0.
 * @return (void)
*/
void serial2_init(uint8_t param_config, uint32_t baud_rate, int16_t offset_calibracion)
{
    int32_t spbrg_aux; //Variable auxiliar para cálculo de registro SPBRG con base en el valor de baud rate
    #ifdef SERIAL2_RX_BUFFER //inicialización de índices del buffer serial, si es que se utilizará
    serial2_queueEnd     = -1;
    serial2_queueFront   = -1;
    #endif
    TXSTA2 = 0;          
    RCSTA2 = 0;         //Reinicio de los registros de control de la EUSART a su valor default en caso de que se utilice algún bootloader
    SPBRGH2 = 0;
    SPBRG2 = 0;
    BAUDCON2bits.BRG16 = 0;
    //Configuración de registros TXSTA y RCSTA
    if( param_config & 0x80 ) //Modo síncrono (1) o asíncrono (0)?
    {
        TXSTA2bits.SYNC = 1;
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate); //En modo síncrono, esta es la manera de calcular el registro SPBRG
        SPBRG2 = make8(spbrg_aux,0);
        SPBRGH2 = make8(spbrg_aux,1);
        if ( param_config & 0x40 ) //Modo síncrono maestro (1) o esclavo (0)?
            TXSTA2bits.CSRC = 1;
    }
    else //Modo asíncrono
    {
        //Cálculo automático de registros para generación de baud rate
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-64*baud_rate,64*baud_rate);      //Se hace uso de esta macro que realiza redondeo para minimizar error de cálculo
        if(spbrg_aux>255)
        {
            spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate);
            SPBRG2 = make8(spbrg_aux,0);
            SPBRGH2 = make8(spbrg_aux,1);
            TXSTA2bits.BRGH=1; //Para altos baud rates
            BAUDCON2bits.BRG16 = 1;  //Generador de baud rate de 16 bits. En caso de no cumplir con baud rate requerido, podrá probarse con este bit en cero 
            //y la división anterior con 16 en lugar de 4. En casi cualquier caso esto no será necesario, pues el error relativo porcentual con respecto al
            //baud rate deseado es menor con el generador de 16 bits.
        }
        if(spbrg_aux<256)
        {
            TXSTA2bits.BRGH=0; //Para bajos baud rates
            SPBRGH2 = 0;
            SPBRG2=(uint8_t)spbrg_aux;
        }
    }
    if( param_config & 0x20 ) //Modo de 9 bits(1) u 8 bits (0)?
    {
        TXSTA2bits.TX9 = 1;
        RCSTA2bits.RX9 = 1;
    }
    if( param_config & 0x08 ) //Transmisión contínua (1) o sencilla (0)?
        RCSTA2bits.CREN = 1;
    if( param_config & 0x04 ) //Detección automática de dirección (1) o no (0)?
        RCSTA2bits.ADDEN = 1;

    TXSTA2bits.TXEN = 1;    // Habilita transmisor
    RCSTA2bits.SPEN = 1;    // Habilita receptor
    TRISGbits.TRISG2 = 1;  //RX es RG2
    TRISGbits.TRISG1 = (TXSTA1bits.SYNC && !TXSTA1bits.CSRC)? 1:0; //Modo esclavo síncrono o TX es RG1
}
#elif defined (EAUSART_V11) || defined (EAUSART_V12)
/**
 * @brief Función de inicialización de módulo serial. La función reinicia los registros aspciados al módulo USART2 a su estado de RESET (POR).
 * Se configura modo síncrono/asíncrono, modo de 8/9 bits, modo esclavo/maestro (en modo síncrono), etc. Las definiciones de bits de configuración
 * se encuentran en el archivo serial.h
 * @param param_config: (uint8_t) Definiciones de bits para configuración de USART2
 * @param baud_rate: (uint32_t) Velocidad de comunicación en bits por segundo (bps). Algunos valores estándar: 110, 150, 300, 1200, 2400, 4800, 7200,
 * 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600
 * @param offset_calibracion: (int16_t) Número entero para calibración y corrección de registro SPBRG. Si no se desea hacer uso de esta característica,
 * mandar 0.
 * @return (void)
*/
void serial2_init(uint8_t param_config, uint32_t baud_rate, int16_t offset_calibracion)
{
    int32_t spbrg_aux; //Variable auxiliar para cálculo de registro SPBRG con base en el valor de baud rate
    #ifdef SERIAL2_RX_BUFFER //inicialización de índices del buffer serial, si es que se utilizará
    serial2_queueEnd     = -1;
    serial2_queueFront   = -1;
    #endif
    TXSTA2 = 0;          
    RCSTA2 = 0;         //Reinicio de los registros de control de la EUSART a su valor default en caso de que se utilice algún bootloader
    SPBRGH2 = 0;
    SPBRG2 = 0;
    BAUDCON2bits.BRG16 = 0;
    //Configuración de registros TXSTA y RCSTA
    if( param_config & 0x80 ) //Modo síncrono (1) o asíncrono (0)?
    {
        TXSTA2bits.SYNC = 1;
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate); //En modo síncrono, esta es la manera de calcular el registro SPBRG
        SPBRG2 = make8(spbrg_aux,0);
        SPBRGH2 = make8(spbrg_aux,1);
        if ( param_config & 0x40 ) //Modo síncrono maestro (1) o esclavo (0)?
            TXSTA2bits.CSRC = 1;
    }
    else //Modo asíncrono
    {
        //Cálculo automático de registros para generación de baud rate
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-64*baud_rate,64*baud_rate);      //Se hace uso de esta macro que realiza redondeo para minimizar error de cálculo
        if(spbrg_aux>255)
        {
            spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate);
            SPBRG2 = make8(spbrg_aux,0);
            SPBRGH2 = make8(spbrg_aux,1);
            TXSTA2bits.BRGH=1; //Para altos baud rates
            BAUDCON2bits.BRG16 = 1;  //Generador de baud rate de 16 bits. En caso de no cumplir con baud rate requerido, podrá probarse con este bit en cero 
            //y la división anterior con 16 en lugar de 4. En casi cualquier caso esto no será necesario, pues el error relativo porcentual con respecto al
            //baud rate deseado es menor con el generador de 16 bits.
        }
        if(spbrg_aux<256)
        {
            TXSTA2bits.BRGH=0; //Para bajos baud rates
            SPBRGH2 = 0;
            SPBRG2=(uint8_t)spbrg_aux;
        }
    }
    if( param_config & 0x20 ) //Modo de 9 bits(1) u 8 bits (0)?
    {
        TXSTA2bits.TX9 = 1;
        RCSTA2bits.RX9 = 1;
    }
    if( param_config & 0x08 ) //Transmisión contínua (1) o sencilla (0)?
        RCSTA2bits.CREN = 1;
    if( param_config & 0x04 ) //Detección automática de dirección (1) o no (0)?
        RCSTA2bits.ADDEN = 1;

    TXSTA2bits.TXEN = 1;  // Habilita transmisor
    RCSTA2bits.SPEN = 1;  // Habilita receptor
    //IMPORTANTE: En estas versiones existe el módulo PPS (Peripheral Pin Select, el cual permite que RX y TX de EUSART2 puedan ubicarse en varios pines dedicados)
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
 * @brief Función de inicialización de módulo serial. La función reinicia los registros aspciados al módulo USART3 a su estado de RESET (POR).
 * Se configura modo síncrono/asíncrono, modo de 8/9 bits, modo esclavo/maestro (en modo síncrono), etc. Las definiciones de bits de configuración
 * se encuentran en el archivo serial.h
 * @param param_config: (uint8_t) Definiciones de bits para configuración de USART3
 * @param baud_rate: (uint32_t) Velocidad de comunicación en bits por segundo (bps). Algunos valores estándar: 110, 150, 300, 1200, 2400, 4800, 7200,
 * 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600
 * @param offset_calibracion: (int16_t) Número entero para calibración y corrección de registro SPBRG. Si no se desea hacer uso de esta característica,
 * mandar 0.
 * @return (void)
*/
void serial3_init(uint8_t param_config, uint32_t baud_rate, int16_t offset_calibracion)
{
    int32_t spbrg_aux; //Variable auxiliar para cálculo de registro SPBRG con base en el valor de baud rate
    #ifdef SERIAL2_RX_BUFFER //inicialización de índices del buffer serial, si es que se utilizará
    serial3_queueEnd     = -1;
    serial3_queueFront   = -1;
    #endif
    TXSTA3 = 0;          
    RCSTA3 = 0;         //Reinicio de los registros de control de la EUSART a su valor default en caso de que se utilice algún bootloader
    SPBRGH3 = 0;
    SPBRG3 = 0;
    BAUDCON3bits.BRG16 = 0;
    //Configuración de registros TXSTA y RCSTA
    if( param_config & 0x80 ) //Modo síncrono (1) o asíncrono (0)?
    {
        TXSTA3bits.SYNC = 1;
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate); //En modo síncrono, esta es la manera de calcular el registro SPBRG
        SPBRG3 = make8(spbrg_aux,0);
        SPBRGH3 = make8(spbrg_aux,1);
        if ( param_config & 0x40 ) //Modo síncrono maestro (1) o esclavo (0)?
            TXSTA3bits.CSRC = 1;
    }
    else //Modo asíncrono
    {
        //Cálculo automático de registros para generación de baud rate
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-64*baud_rate,64*baud_rate);      //Se hace uso de esta macro que realiza redondeo para minimizar error de cálculo
        if(spbrg_aux>255)
        {
            spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate);
            SPBRG3 = make8(spbrg_aux,0);
            SPBRGH3 = make8(spbrg_aux,1);
            TXSTA3bits.BRGH=1; //Para altos baud rates
            BAUDCON3bits.BRG16 = 1;  //Generador de baud rate de 16 bits. En caso de no cumplir con baud rate requerido, podrá probarse con este bit en cero 
            //y la división anterior con 16 en lugar de 4. En casi cualquier caso esto no será necesario, pues el error relativo porcentual con respecto al
            //baud rate deseado es menor con el generador de 16 bits.
        }
        if(spbrg_aux<256)
        {
            TXSTA3bits.BRGH=0; //Para bajos baud rates
            SPBRGH3 = 0;
            SPBRG3=(uint8_t)spbrg_aux;
        }
    }
    if( param_config & 0x20 ) //Modo de 9 bits(1) u 8 bits (0)?
    {
        TXSTA3bits.TX9 = 1;
        RCSTA3bits.RX9 = 1;
    }
    if( param_config & 0x08 ) //Transmisión contínua (1) o sencilla (0)?
        RCSTA3bits.CREN = 1;
    if( param_config & 0x04 ) //Detección automática de dirección (1) o no (0)?
        RCSTA3bits.ADDEN = 1;

    TXSTA3bits.TXEN = 1;  // Habilita transmisor
    RCSTA3bits.SPEN = 1;  // Habilita receptor
    //IMPORTANTE: En estas versiones existe el módulo PPS (Peripheral Pin Select, el cual permite que RX y TX de EUSART3 puedan ubicarse en varios pines dedicados)
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
 * @brief Función de inicialización de módulo serial. La función reinicia los registros aspciados al módulo USART4 a su estado de RESET (POR).
 * Se configura modo síncrono/asíncrono, modo de 8/9 bits, modo esclavo/maestro (en modo síncrono), etc. Las definiciones de bits de configuración
 * se encuentran en el archivo serial.h
 * @param param_config: (uint8_t) Definiciones de bits para configuración de USART4
 * @param baud_rate: (uint32_t) Velocidad de comunicación en bits por segundo (bps). Algunos valores estándar: 110, 150, 300, 1200, 2400, 4800, 7200,
 * 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600
 * @param offset_calibracion: (int16_t) Número entero para calibración y corrección de registro SPBRG. Si no se desea hacer uso de esta característica,
 * mandar 0.
 * @return (void)
*/
void serial4_init(uint8_t param_config, uint32_t baud_rate, int16_t offset_calibracion)
{
    int32_t spbrg_aux; //Variable auxiliar para cálculo de registro SPBRG con base en el valor de baud rate
    #ifdef SERIAL2_RX_BUFFER //inicialización de índices del buffer serial, si es que se utilizará
    serial4_queueEnd     = -1;
    serial4_queueFront   = -1;
    #endif
    TXSTA4 = 0;          
    RCSTA4 = 0;         //Reinicio de los registros de control de la EUSART a su valor default en caso de que se utilice algún bootloader
    SPBRGH4 = 0;
    SPBRG4 = 0;
    BAUDCON4bits.BRG16 = 0;
    //Configuración de registros TXSTA y RCSTA
    if( param_config & 0x80 ) //Modo síncrono (1) o asíncrono (0)?
    {
        TXSTA4bits.SYNC = 1;
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate); //En modo síncrono, esta es la manera de calcular el registro SPBRG
        SPBRG4 = make8(spbrg_aux,0);
        SPBRGH4 = make8(spbrg_aux,1);
        if ( param_config & 0x40 ) //Modo síncrono maestro (1) o esclavo (0)?
            TXSTA4bits.CSRC = 1;
    }
    else //Modo asíncrono
    {
        //Cálculo automático de registros para generación de baud rate
        spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-64*baud_rate,64*baud_rate);      //Se hace uso de esta macro que realiza redondeo para minimizar error de cálculo
        if(spbrg_aux>255)
        {
            spbrg_aux = offset_calibracion + (int32_t)division_entera_sin_signo(_XTAL_FREQ-4*baud_rate,4*baud_rate);
            SPBRG4 = make8(spbrg_aux,0);
            SPBRGH4 = make8(spbrg_aux,1);
            TXSTA4bits.BRGH=1; //Para altos baud rates
            BAUDCON4bits.BRG16 = 1;  //Generador de baud rate de 16 bits. En caso de no cumplir con baud rate requerido, podrá probarse con este bit en cero 
            //y la división anterior con 16 en lugar de 4. En casi cualquier caso esto no será necesario, pues el error relativo porcentual con respecto al
            //baud rate deseado es menor con el generador de 16 bits.
        }
        if(spbrg_aux<256)
        {
            TXSTA4bits.BRGH=0; //Para bajos baud rates
            SPBRGH4 = 0;
            SPBRG4=(uint8_t)spbrg_aux;
        }
    }
    if( param_config & 0x20 ) //Modo de 9 bits(1) u 8 bits (0)?
    {
        TXSTA4bits.TX9 = 1;
        RCSTA4bits.RX9 = 1;
    }
    if( param_config & 0x08 ) //Transmisión contínua (1) o sencilla (0)?
        RCSTA4bits.CREN = 1;
    if( param_config & 0x04 ) //Detección automática de dirección (1) o no (0)?
        RCSTA4bits.ADDEN = 1;

    TXSTA4bits.TXEN = 1;  // Habilita transmisor
    RCSTA4bits.SPEN = 1;  // Habilita receptor
    //IMPORTANTE: En estas versiones existe el módulo PPS (Peripheral Pin Select, el cual permite que RX y TX de EUSART4 puedan ubicarse en varios pines dedicados)
}

#endif  

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)

/**
  * @brief Función que transmite un byte vía USART 
  * @param dato: (uint8_t) byte a transmitir  
  * @return Ninguno: (void) 
*/
void serial_writeByte(uint8_t dato)
{
    if(TXSTAbits.TX9)  //Modo de 9 bits?
    {
        TXSTAbits.TX9D = (serialStatus.TX_NINE)? 1:0; //Establece bit según bandera correspondiente
    }
    //El bit TXSTAbits.TX9D funge como un bit transmitido adicional si tal característica es habilitada
    TXREG=dato;     //Mueve dato al buffer de transmisión
    while(!TXSTAbits.TRMT){} //Espera a que termine el envío
}
#endif

#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V10) ||\
    defined (EAUSART_V11) || defined (EAUSART_V11_1) || defined (EAUSART_V12)
/**
  * @brief Función que transmite un byte vía USART1 
  * @param dato: (uint8_t) byte a transmitir  
  * @return Ninguno: (void) 
*/
void serial1_writeByte(uint8_t dato) {
    if(TXSTA1bits.TX9){  //Modo de 9 bits?
        TXSTA1bits.TX9D = (serial1Status.TX_NINE)? 1:0; //Establece bit según bandera correspondiente
    }
    //El bit TXSTA1bits.TX9D funge como un bit transmitido adicional si tal característica es habilitada
    TXREG1 = dato;     //Mueve dato al buffer de transmisión
    while(!TXSTA1bits.TRMT){} //Espera a que termine el envío
}

#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11)\
    || defined (EAUSART_V12)
/**
  * @brief Función que transmite un byte vía USART2 
  * @param dato: (uint8_t) byte a transmitir  
  * @return Ninguno: (void) 
*/
void serial2_writeByte(uint8_t dato)
{
    if(TXSTA2bits.TX9)  //Modo de 9 bits?
    {
        TXSTA2bits.TX9D = (serial2Status.TX_NINE)? 1:0; //Establece bit según bandera correspondiente
    }
    //El bit TXSTA2bits.TX9D funge como un bit transmitido adicional si tal característica es habilitada
    TXREG2=dato;     //Mueve dato al buffer de transmisión
    while(!TXSTA2bits.TRMT){} //Espera a que termine el envío
} 
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función que transmite un byte vía USART3 
  * @param dato: (uint8_t) byte a transmitir  
  * @return Ninguno: (void) 
*/
void serial3_writeByte(uint8_t dato)
{
    if(TXSTA3bits.TX9)  //Modo de 9 bits?
    {
        TXSTA3bits.TX9D = (serial3Status.TX_NINE)? 1:0; //Establece bit según bandera correspondiente
    }
    //El bit TXSTA3bits.TX9D funge como un bit transmitido adicional si tal característica es habilitada
    TXREG3=dato;     //Mueve dato al buffer de transmisión
    while(!TXSTA2bits.TRMT){} //Espera a que termine el envío
} 
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función que transmite un byte vía USART4 
  * @param dato: (uint8_t) byte a transmitir  
  * @return Ninguno: (void) 
*/
void serial4_writeByte(uint8_t dato)
{
    if(TXSTA4bits.TX9)  //Modo de 9 bits?
    {
        TXSTA4bits.TX9D = (serial4Status.TX_NINE)? 1:0; //Establece bit según bandera correspondiente
    }
    //El bit TXSTA4bits.TX9D funge como un bit transmitido adicional si tal característica es habilitada
    TXREG4=dato;     //Mueve dato al buffer de transmisión
    while(!TXSTA4bits.TRMT){} //Espera a que termine el envío
} 
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función para recibir un byte vía USART. Esta función debe utilizarse si hubo un evento de 
  * recepción (RCIF activa), por medio de polling o interrupción.
  * @param (void)  
  * @return (uint8_t) Dato de 8 bits que ha sido recibido mediante EUSART
*/
uint8_t serial_readByte()  
{   
    serialStatus.byte_status &= 0xF2;   //Limpia banderas de estado anteriores
    if(RCSTAbits.RX9)                   //Si se está en modo de 9 bits...
    {
        serialStatus.RX_NINE = (RCSTAbits.RX9D)? 1:0; //Lee el valor de dicho bitde acuerdo con el bit RX9D
    }

    if(RCSTAbits.FERR)                  //Si ocurrió un framing error...
        serialStatus.FRAME_ERROR = 1;   //Índicalo en la bandera correspondiente

    if(RCSTAbits.OERR)                  //Si ocurrió un overrun error...
        serialStatus.OVERRUN_ERROR = 1; //Índicalo en la bandera correspondiente
    return RCREG;                       //Regresa valor recibido
}
#endif

#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V10) ||\
    defined (EAUSART_V11) || defined (EAUSART_V11_1) || defined (EAUSART_V12)
/**
  * @brief Función para recibir un byte vía USART1. Esta función debe utilizarse si hubo un evento de 
  * recepción (RC1IF activa), por medio de polling o interrupción.
  * @param (void) 
  * @return (uint8_t) Dato de 8 bits que ha sido recibido mediante EUSART
*/
uint8_t serial1_readByte()  
{   
    serial1Status.byte_status &= 0xF2;   //Limpia banderas de estado anteriores
    if(RCSTA1bits.RX9)                   //Si se está en modo de 9 bits...
    {
        serial1Status.RX_NINE = (RCSTA1bits.RX9D)? 1:0; //Lee el valor de dicho bitde acuerdo con el bit RX9D
    }

    if(RCSTA1bits.FERR)                  //Si ocurrió un framing error...
        serial1Status.FRAME_ERROR = 1;   //Índicalo en la bandera correspondiente

    if(RCSTA1bits.OERR)                  //Si ocurrió un overrun error...
        serial1Status.OVERRUN_ERROR = 1; //Índicalo en la bandera correspondiente
    return RCREG1;                       //Regresa valor recibido
}

#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11)\
    || defined (EAUSART_V12)
/**
  * @brief Función para recibir un byte vía USART2. Esta función debe utilizarse si hubo un evento de 
  * recepción (RC2IF activa), por medio de polling o interrupción.
  * @param (void) 
  * @return (uint8_t) Dato de 8 bits que ha sido recibido mediante EUSART2
*/
uint8_t serial2_readByte()  
{   
    serial2Status.byte_status &= 0xF2;   //Limpia banderas de estado anteriores
    if(RCSTA2bits.RX9)                   //Si se está en modo de 9 bits...
    {
        serial2Status.RX_NINE = (RCSTA2bits.RX9D)? 1:0; //Lee el valor de dicho bitde acuerdo con el bit RX9D
    }

    if(RCSTA2bits.FERR)                  //Si ocurrió un framing error...
        serial2Status.FRAME_ERROR = 1;   //Índicalo en la bandera correspondiente

    if(RCSTA2bits.OERR)                  //Si ocurrió un overrun error...
        serial2Status.OVERRUN_ERROR = 1; //Índicalo en la bandera correspondiente
    return RCREG2;                       //Regresa valor recibido
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para recibir un byte vía USART3. Esta función debe utilizarse si hubo un evento de 
  * recepción (RC3IF activa), por medio de polling o interrupción.
  * @param (void) 
  * @return (uint8_t) Dato de 8 bits que ha sido recibido mediante EUSART3
*/
uint8_t serial3_readByte()  
{   
    serial3Status.byte_status &= 0xF2;   //Limpia banderas de estado anteriores
    if(RCSTA3bits.RX9)                   //Si se está en modo de 9 bits...
    {
        serial3Status.RX_NINE = (RCSTA3bits.RX9D)? 1:0; //Lee el valor de dicho bitde acuerdo con el bit RX9D
    }

    if(RCSTA3bits.FERR)                  //Si ocurrió un framing error...
        serial3Status.FRAME_ERROR = 1;   //Índicalo en la bandera correspondiente

    if(RCSTA3bits.OERR)                  //Si ocurrió un overrun error...
        serial3Status.OVERRUN_ERROR = 1; //Índicalo en la bandera correspondiente
    return RCREG3;                       //Regresa valor recibido
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para recibir un byte vía USART4. Esta función debe utilizarse si hubo un evento de 
  * recepción (RC4IF activa), por medio de polling o interrupción.
  * @param (void) 
  * @return (uint8_t) Dato de 8 bits que ha sido recibido mediante EUSART4
*/
uint8_t serial4_readByte()  
{   
    serial4Status.byte_status &= 0xF2;   //Limpia banderas de estado anteriores
    if(RCSTA4bits.RX9)                   //Si se está en modo de 9 bits...
    {
        serial4Status.RX_NINE = (RCSTA4bits.RX9D)? 1:0; //Lee el valor de dicho bitde acuerdo con el bit RX9D
    }

    if(RCSTA4bits.FERR)                  //Si ocurrió un framing error...
        serial4Status.FRAME_ERROR = 1;   //Índicalo en la bandera correspondiente

    if(RCSTA4bits.OERR)                  //Si ocurrió un overrun error...
        serial4Status.OVERRUN_ERROR = 1; //Índicalo en la bandera correspondiente
    return RCREG4;                       //Regresa valor recibido
}
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función de escritura de una cadena de caracteres mediante EUSART
  * @param dato: (const char*) Apuntador a la cadena de caracteres que se desea transmitir  
  * @return (void)
*/
void serial_puts(const char *cadena)
{
    while(*cadena) //Mientras el valor de la cadena no sea \0, escribir caracter en la posición del apuntador
    {
        serial_writeByte(*cadena++); //Escribe bytes secuencialmente hasta terminar con la cadena
    }
}

#endif

#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V10) ||\
    defined (EAUSART_V11) || defined (EAUSART_V11_1) || defined (EAUSART_V12)
/**
  * @brief Función de escritura de una cadena de caracteres mediante EUSART1
  * @param dato: (const char*) Apuntador a la cadena de caracteres que se desea transmitir  
  * @return (void)
*/
void serial1_puts(const char *cadena)
{
    while(*cadena) //Mientras el valor de la cadena no sea \0, escribir caracter en la posición del apuntador
    {
        serial1_writeByte(*cadena++); //Escribe bytes secuencialmente hasta terminar con la cadena
    }
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11)\
    || defined (EAUSART_V12)
/**
  * @brief Función de escritura de una cadena de caracteres mediante EUSART2
  * @param dato: (const char*) Apuntador a la cadena de caracteres que se desea transmitir  
  * @return (void)
*/
void serial2_puts(const char *cadena)
{
    while(*cadena) //Mientras el valor de la cadena no sea \0, escribir caracter en la posición del apuntador
    {
        serial2_writeByte(*cadena++); //Escribe bytes secuencialmente hasta terminar con la cadena
    }
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función de escritura de una cadena de caracteres mediante EUSART3
  * @param dato: (const char*) Apuntador a la cadena de caracteres que se desea transmitir  
  * @return (void)
*/
void serial3_puts(const char *cadena)
{
    while(*cadena) //Mientras el valor de la cadena no sea \0, escribir caracter en la posición del apuntador
    {
        serial3_writeByte(*cadena++); //Escribe bytes secuencialmente hasta terminar con la cadena
    }
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función de escritura de una cadena de caracteres mediante EUSART4
  * @param dato: (const char*) Apuntador a la cadena de caracteres que se desea transmitir  
  * @return (void)
*/
void serial4_puts(const char *cadena)
{
    while(*cadena) //Mientras el valor de la cadena no sea \0, escribir caracter en la posición del apuntador
    {
        serial4_writeByte(*cadena++); //Escribe bytes secuencialmente hasta terminar con la cadena
    }
}

#endif
      
//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
 * @brief Función para lectura de una cantidad de elementos definida directamente desde el módulo EUSART,
 * para ponerlos en un buffer (arreglo)  
 * @param buffer: (char*) Apuntador al arreglo en el cual se desean copiar los datos.  
 * @param len: (uint16_t) Cantidad de elementos a copiar de un buffer a otro
 * @return (void)
*/
void serial_gets(char *buffer, uint8_t len)
{
    uint8_t i;    //Contador de longitud de buffer
    for( i=0 ; i!=len ; ++i)
    {
        while(!RCIF);                  //En espera de recepción de datos
        *buffer  = serial_readByte();    //Obtención de caracter del buffer serial, se almacena en el arreglo de caracteres
        buffer++; //se incremento de apuntador al arreglo
    }
}
#else
/**
 * @brief Función para lectura de una cantidad de elementos definida directamente desde el módulo EUSART1,
 * para ponerlos en un buffer (arreglo)  
 * @param buffer: (char*) Apuntador al arreglo en el cual se desean copiar los datos.  
 * @param len: (uint16_t) Cantidad de elementos a copiar de un buffer a otro
 * @return (void)
*/
void serial1_gets(char *buffer, uint8_t len)
{
    uint8_t i;    //Contador de longitud de buffer
    for( i=0 ; i!=len ; ++i)
    {
        while(!RC1IF);                  //En espera de recepción de datos
        *buffer  = serial1_readByte();    //Obtención de caracter del buffer serial, se almacena en el arreglo de caracteres
        buffer++; //se incremento de apuntador al arreglo
    }
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
 * @brief Función para lectura de una cantidad de elementos definida directamente desde el módulo EUSART2,
 * para ponerlos en un buffer (arreglo)  
 * @param buffer: (char*) Apuntador al arreglo en el cual se desean copiar los datos.  
 * @param len: (uint16_t) Cantidad de elementos a copiar de un buffer a otro
 * @return (void)
*/
void serial2_gets(char *buffer, uint8_t len)
{
    uint8_t i;    //Contador de longitud de buffer
    for( i=0 ; i!=len ; ++i)
    {
        while(!RC2IF);                  //En espera de recepción de datos
        *buffer  = serial2_readByte();    //Obtención de caracter del buffer serial, se almacena en el arreglo de caracteres
        buffer++; //se incremento de apuntador al arreglo
    }
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
 * @brief Función para lectura de una cantidad de elementos definida directamente desde el módulo EUSART3,
 * para ponerlos en un buffer (arreglo)  
 * @param buffer: (char*) Apuntador al arreglo en el cual se desean copiar los datos.  
 * @param len: (uint16_t) Cantidad de elementos a copiar de un buffer a otro
 * @return (void)
*/
void serial3_gets(char *buffer, uint8_t len)
{
    uint8_t i;    //Contador de longitud de buffer
    for( i=0 ; i!=len ; ++i)
    {
        while(!RC3IF);                  //En espera de recepción de datos
        *buffer  = serial3_readByte();    //Obtención de caracter del buffer serial, se almacena en el arreglo de caracteres
        buffer++; //se incremento de apuntador al arreglo
    }
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
 * @brief Función para lectura de una cantidad de elementos definida directamente desde el módulo EUSART4,
 * para ponerlos en un buffer (arreglo)  
 * @param buffer: (char*) Apuntador al arreglo en el cual se desean copiar los datos.  
 * @param len: (uint16_t) Cantidad de elementos a copiar de un buffer a otro
 * @return (void)
*/
void serial4_gets(char *buffer, uint8_t len)
{
    uint8_t i;    //Contador de longitud de buffer
    for( i=0 ; i!=len ; ++i)
    {
        while(!RC4IF);                  //En espera de recepción de datos
        *buffer  = serial4_readByte();    //Obtención de caracter del buffer serial, se almacena en el arreglo de caracteres
        buffer++; //se incremento de apuntador al arreglo
    }
}  
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
 * @brief Función de escritura de una cadena de caracteres, con retorno de carro (CR) y nueva línea (NL) mediante EUSART  
 * @param cadena: (const char*) Apuntador a la cadena de caracteres que se desea transmitir.  
 * @return (void)
*/
void serial_writeLine(const char *cadena)
{
    serial_puts(cadena);    //Imprime cadena normalmente
    serial_writeByte('\r'); //Retorno de carro CR
    serial_writeByte('\n'); //Nueva línea NL
}
#else
/**
 * @brief Función de escritura de una cadena de caracteres, con retorno de carro (CR) y nueva línea (NL) mediante EUSART1  
 * @param cadena: (const char*) Apuntador a la cadena de caracteres que se desea transmitir.  
 * @return (void)
*/
void serial1_writeLine(const char *cadena)
{
    serial1_puts(cadena);    //Imprime cadena normalmente
    serial1_writeByte('\r'); //Retorno de carro CR
    serial1_writeByte('\n'); //Nueva línea NL
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
 * @brief Función de escritura de una cadena de caracteres, con retorno de carro (CR) y nueva línea (NL) mediante EUSART2  
 * @param cadena: (const char*) Apuntador a la cadena de caracteres que se desea transmitir.  
 * @return (void)
*/
void serial2_writeLine(const char *cadena)
{
    serial2_puts(cadena);    //Imprime cadena normalmente
    serial2_writeByte('\r'); //Retorno de carro CR
    serial2_writeByte('\n'); //Nueva línea NL
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
 * @brief Función de escritura de una cadena de caracteres, con retorno de carro (CR) y nueva línea (NL) mediante EUSART3  
 * @param cadena: (const char*) Apuntador a la cadena de caracteres que se desea transmitir.  
 * @return (void)
*/
void serial3_writeLine(const char *cadena)
{
    serial3_puts(cadena);    //Imprime cadena normalmente
    serial3_writeByte('\r'); //Retorno de carro CR
    serial3_writeByte('\n'); //Nueva línea NL
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
 * @brief Función de escritura de una cadena de caracteres, con retorno de carro (CR) y nueva línea (NL) mediante EUSART4  
 * @param cadena: (const char*) Apuntador a la cadena de caracteres que se desea transmitir.  
 * @return (void)
*/
void serial4_writeLine(const char *cadena)
{
    serial4_puts(cadena);    //Imprime cadena normalmente
    serial4_writeByte('\r'); //Retorno de carro CR
    serial4_writeByte('\n'); //Nueva línea NL
} 
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
 * @brief Función de escritura de un arreglo de bytes mediante EUSART  
 * @param buffer: (uint8_t*) Apuntador al arreglo de datos que se desea transmitir.  
 * @param length: (uint16_t) Cantidad de datos del arreglo que se desean transmitir
 * @return (void)
*/
void serial_writeBuffer(uint8_t *buffer, uint8_t length)
{
    for(uint8_t i=0;i!=length;i++)
    {
        serial_writeByte(*buffer); //Envío secuencial de datos del buffer
        buffer++;
    }
}
#else
/**
 * @brief Función de escritura de un arreglo de bytes mediante EUSART1  
 * @param buffer: (uint8_t*) Apuntador al arreglo de datos que se desea transmitir.  
 * @param length: (uint16_t) Cantidad de datos del arreglo que se desean transmitir
 * @return (void)
*/
void serial1_writeBuffer(uint8_t *buffer, uint8_t length)
{
    for(uint8_t i=0;i!=length;i++)
    {
        serial1_writeByte(*buffer); //Envío secuencial de datos del buffer
        buffer++;
    }
}
#endif
//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
 * @brief Función de escritura de un arreglo de bytes mediante EUSART2  
 * @param buffer: (uint8_t*) Apuntador al arreglo de datos que se desea transmitir.  
 * @param length: (uint16_t) Cantidad de datos del arreglo que se desean transmitir
 * @return (void)
*/
void serial2_writeBuffer(uint8_t *buffer, uint8_t length)
{
    for(uint8_t i=0;i!=length;i++)
    {
        serial2_writeByte(*buffer); //Envío secuencial de datos del buffer
        buffer++;
    }
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
 * @brief Función de escritura de un arreglo de bytes mediante EUSART3  
 * @param buffer: (uint8_t*) Apuntador al arreglo de datos que se desea transmitir.  
 * @param length: (uint16_t) Cantidad de datos del arreglo que se desean transmitir
 * @return (void)
*/
void serial3_writeBuffer(uint8_t *buffer, uint8_t length)
{
    for(uint8_t i=0;i!=length;i++)
    {
        serial3_writeByte(*buffer); //Envío secuencial de datos del buffer
        buffer++;
    }
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
 * @brief Función de escritura de un arreglo de bytes mediante EUSART4  
 * @param buffer: (uint8_t*) Apuntador al arreglo de datos que se desea transmitir.  
 * @param length: (uint16_t) Cantidad de datos del arreglo que se desean transmitir
 * @return (void)
*/
void serial4_writeBuffer(uint8_t *buffer, uint8_t length)
{
    for(uint8_t i=0;i!=length;i++)
    {
        serial4_writeByte(*buffer); //Envío secuencial de datos del buffer
        buffer++;
    }
}
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función para escribir un dato entero de 2 bytes por medio de EUSART
  * @param dato: (uint16_t) Dato de 16 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial_writeInt16(uint16_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        serial_writeByte(*((uint8_t *)&dato+i));
    }
}
#else
/**
  * @brief Función para escribir un dato entero de 2 bytes por medio de EUSART1
  * @param dato: (uint16_t) Dato de 16 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial1_writeInt16(uint16_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        serial1_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato entero de 2 bytes por medio de EUSART2
  * @param dato: (uint16_t) Dato de 16 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial2_writeInt16(uint16_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        serial2_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato entero de 2 bytes por medio de EUSART3
  * @param dato: (uint16_t) Dato de 16 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial3_writeInt16(uint16_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        serial3_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato entero de 2 bytes por medio de EUSART4
  * @param dato: (uint16_t) Dato de 16 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial4_writeInt16(uint16_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        serial4_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función para leer un dato entero de 2 bytes por medio de EUSART. Polling del bit RCIF
  * @param (void) 
  * @return (uint16_t)  Dato de 16 bits que ha sido recibido mediante EUSART
*/
uint16_t serial_readInt16()
{
    uint16_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 16 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 16 bits
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        while(!RCIF);                  //En espera de recepción de datos
        *(p_lectura++) = serial_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído 
}
#else
/**
  * @brief Función para leer un dato entero de 2 bytes por medio de EUSART1. Polling del bit RC1IF
  * @param (void) 
  * @return (uint16_t)  Dato de 16 bits que ha sido recibido mediante EUSART1
*/
uint16_t serial1_readInt16()
{
    uint16_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 16 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 16 bits
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        while(!RC1IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial1_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído
    
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
  * @brief Función para leer un dato entero de 2 bytes por medio de EUSART2. Polling del bit RC2IF
  * @param (void) 
  * @return (uint16_t)  Dato de 16 bits que ha sido recibido mediante EUSART2
*/
uint16_t serial2_readInt16()
{
    uint16_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 16 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 16 bits
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        while(!RC2IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial2_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para leer un dato entero de 2 bytes por medio de EUSART3. Polling del bit RC3IF
  * @param (void) 
  * @return (uint16_t)  Dato de 16 bits que ha sido recibido mediante EUSART3
*/
uint16_t serial3_readInt16()
{
    uint16_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 16 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 16 bits
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        while(!RC3IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial3_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para leer un dato entero de 2 bytes por medio de EUSART4. Polling del bit RC4IF
  * @param (void) 
  * @return (uint16_t)  Dato de 16 bits que ha sido recibido mediante EUSART4
*/
uint16_t serial4_readInt16()
{
    uint16_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 16 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 16 bits
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        while(!RC4IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial4_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído
}
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función para escribir un dato entero de 3 bytes por medio de EUSART
  * @param dato: (uint24_t) Dato de 24 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial_writeInt24(uint24_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        serial_writeByte(*((uint8_t *)&dato+i));
    }
}
#else
/**
  * @brief Función para escribir un dato entero de 3 bytes por medio de EUSART1
  * @param dato: (uint24_t) Dato de 24 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial1_writeInt24(uint24_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        serial1_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato entero de 3 bytes por medio de EUSART2
  * @param dato: (uint24_t) Dato de 24 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial2_writeInt24(uint24_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        serial2_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato entero de 3 bytes por medio de EUSART3
  * @param dato: (uint24_t) Dato de 24 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial3_writeInt24(uint24_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        serial3_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato entero de 3 bytes por medio de EUSART4
  * @param dato: (uint24_t) Dato de 24 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial4_writeInt24(uint24_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        serial4_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función para leer un dato entero de 3 bytes por medio de EUSART. Polling del bit RCIF
  * @param (void) 
  * @return (uint24_t)  Dato de 24 bits que ha sido recibido mediante EUSART
*/
uint24_t serial_readInt24()
{
    uint24_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 24 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 24 bits
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        while(!RCIF);                  //En espera de recepción de datos
        *(p_lectura++) = serial_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 24 bits leído 
}
#else
/**
  * @brief Función para leer un dato entero de 3 bytes por medio de EUSART1. Polling del bit RC1IF
  * @param (void) 
  * @return (uint24_t)  Dato de 24 bits que ha sido recibido mediante EUSART1
*/
uint24_t serial1_readInt24()
{
    uint24_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 24 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 24 bits
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        while(!RC1IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial1_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 24 bits leído
    
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
  * @brief Función para leer un dato entero de 3 bytes por medio de EUSART2. Polling del bit RC2IF
  * @param (void) 
  * @return (uint24_t)  Dato de 24 bits que ha sido recibido mediante EUSART2
*/
uint24_t serial2_readInt24()
{
    uint24_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 24 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 24 bits
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        while(!RC2IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial2_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 24 bits leído
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para leer un dato entero de 3 bytes por medio de EUSART3. Polling del bit RC3IF
  * @param (void) 
  * @return (uint24_t)  Dato de 24 bits que ha sido recibido mediante EUSART3
*/
uint24_t serial3_readInt24()
{
    uint24_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 24 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 24 bits
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        while(!RC3IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial3_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 24 bits leído
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para leer un dato entero de 3 bytes por medio de EUSART4. Polling del bit RC4IF
  * @param (void) 
  * @return (uint24_t)  Dato de 24 bits que ha sido recibido mediante EUSART4
*/
uint24_t serial4_readInt24()
{
    uint24_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 24 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 24 bits
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        while(!RCIF);                  //En espera de recepción de datos
        *(p_lectura++) = serial4_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 24 bits leído
}
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función para escribir un dato entero de 4 bytes por medio de EUSART
  * @param dato: (uint32t) Dato de 32 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial_writeInt32(uint32_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        serial_writeByte(*((uint8_t *)&dato+i));
    }
}
#else
/**
  * @brief Función para escribir un dato entero de 4 bytes por medio de EUSART1
  * @param dato: (uint32t) Dato de 32 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial1_writeInt32(uint32_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        serial1_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato entero de 4 bytes por medio de EUSART2
  * @param dato: (uint32t) Dato de 32 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial2_writeInt32(uint32_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        serial2_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato entero de 4 bytes por medio de EUSART3
  * @param dato: (uint32t) Dato de 32 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial3_writeInt32(uint32_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        serial3_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato entero de 4 bytes por medio de EUSART4
  * @param dato: (uint32t) Dato de 32 bits a transmitir  
  * @return Ninguno: (void) 
*/
void serial4_writeInt32(uint32_t dato)
{
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        serial4_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función para leer un dato entero de 4 bytes por medio de EUSART. Polling del bit RCIF
  * @param (void) 
  * @return (uint32_t)  Dato de 32 bits que ha sido recibido mediante EUSART
*/
uint32_t serial_readInt32()
{
    uint32_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 32 bits
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        while(!RCIF);                  //En espera de recepción de datos
        *(p_lectura++) = serial_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32 bits leído 
}
#else
/**
  * @brief Función para leer un dato entero de 4 bytes por medio de EUSART1. Polling del bit RC1IF
  * @param (void) 
  * @return (uint32_t)  Dato de 32 bits que ha sido recibido mediante EUSART1
*/
uint32_t serial1_readInt32()
{
    uint32_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 32 bits
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        while(!RC1IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial1_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32 bits leído
    
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
  * @brief Función para leer un dato entero de 4 bytes por medio de EUSART2. Polling del bit RC2IF
  * @param (void) 
  * @return (uint32_t)  Dato de 32 bits que ha sido recibido mediante EUSART2
*/
uint32_t serial2_readInt32()
{
    uint32_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 32 bits
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        while(!RC2IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial2_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32 bits leído
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para leer un dato entero de 4 bytes por medio de EUSART3. Polling del bit RC3IF
  * @param (void) 
  * @return (uint32_t)  Dato de 32 bits que ha sido recibido mediante EUSART3
*/
uint32_t serial3_readInt32()
{
    uint32_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 32 bits
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        while(!RC3IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial3_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32 bits leído
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para leer un dato entero de 4 bytes por medio de EUSART4. Polling del bit RC4IF
  * @param (void) 
  * @return (uint32_t)  Dato de 32 bits que ha sido recibido mediante EUSART4
*/
uint32_t serial4_readInt32()
{
    uint32_t dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable entera de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 32 bits
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        while(!RC4IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial4_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32 bits leído
}
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función para escribir un dato en punto flotante por medio de EUSART
  * @param dato: (float) Dato en punto flotante a transmitir  
  * @return Ninguno: (void) 
*/
void serial_writeFloat(float dato)
{
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        serial_writeByte(*((uint8_t *)&dato+i));
    }
}
#else
/**
  * @brief Función para escribir un dato en punto flotante por medio de EUSART1
  * @param dato: (float) Dato en punto flotante a transmitir  
  * @return Ninguno: (void) 
*/
void serial1_writeFloat(float dato)
{
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        serial1_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato en punto flotante por medio de EUSART2
  * @param dato: (float) Dato en punto flotante a transmitir  
  * @return Ninguno: (void) 
*/
void serial2_writeFloat(float dato)
{
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        serial2_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato en punto flotante por medio de EUSART3
  * @param dato: (float) Dato en punto flotante a transmitir  
  * @return Ninguno: (void) 
*/
void serial3_writeFloat(float dato)
{
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        serial3_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato en punto flotante por medio de EUSART4
  * @param dato: (float) Dato en punto flotante a transmitir  
  * @return Ninguno: (void) 
*/
void serial4_writeFloat(float dato)
{
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        serial4_writeByte(*((uint8_t *)&dato+i));
    }
}
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)

/**
  * @brief Función para escribir un dato de cualquier tipo por medio de USART
  * @param datos: (void*) Dato de cualquier tipo a trasmitir mediante USART
  * @param len: (uint16_t) Cantidad de bytes a transmitir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial_write(void* datos, uint16_t len)
{
    uint8_t* _datos = (uint8_t *)datos;
    while(len--)
        serial_writeByte(*_datos++);
}
#endif

#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V10) ||\
    defined (EAUSART_V11) || defined (EAUSART_V11_1) || defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato de cualquier tipo por medio de USART1
  * @param datos: (void*) Dato de cualquier tipo a trasmitir mediante USART1
  * @param len: (uint16_t) Cantidad de bytes a transmitir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial1_write(void* datos, uint16_t len)
{
    uint8_t* _datos = (uint8_t *)datos;
    while(len--)
        serial1_writeByte(*_datos++);
}

#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11)\
    || defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato de cualquier tipo por medio de USART2
  * @param datos: (void*) Dato de cualquier tipo a trasmitir mediante USART2
  * @param len: (uint16_t) Cantidad de bytes a transmitir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial2_write(void* datos, uint16_t len)
{
    uint8_t* _datos = (uint8_t *)datos;
    while(len--)
        serial2_writeByte(*_datos++);
} 
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato de cualquier tipo por medio de USART3
  * @param datos: (void*) Dato de cualquier tipo a trasmitir mediante USART3
  * @param len: (uint16_t) Cantidad de bytes a transmitir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial3_write(void* datos, uint16_t len)
{
    uint8_t* _datos = (uint8_t *)datos;
    while(len--)
        serial3_writeByte(*_datos++);
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para escribir un dato de cualquier tipo por medio de USART4
  * @param datos: (void*) Dato de cualquier tipo a trasmitir mediante USART4
  * @param len: (uint16_t) Cantidad de bytes a transmitir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial4_write(void* datos, uint16_t len)
{
    uint8_t* _datos = (uint8_t *)datos;
    while(len--)
        serial4_writeByte(*_datos++);
}
#endif



//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función para leer un dato en punto flotante por medio de EUSART. Polling del bit RCIF
  * @param (void) 
  * @return (float)  Dato en punto flotante que ha sido recibido mediante EUSART
*/
float serial_readFloat()
{
    float dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable flotante de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo flotante de 32(24) bits
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        while(!RCIF);                  //En espera de recepción de datos
        *(p_lectura++) = serial_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32(24) bits leído 
}
#else
/**
  * @brief Función para leer un dato en punto flotante por medio de EUSART1. Polling del bit RC1IF
  * @param (void) 
  * @return (float)  Dato en punto flotante que ha sido recibido mediante EUSART1
*/
float serial1_readFloat()
{
    float dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable flotante de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo flotante de 32(24) bits
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        while(!RC1IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial1_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32(24) bits leído 
    
}
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
/**
  * @brief Función para leer un dato en punto flotante por medio de EUSART2. Polling del bit RC2IF
  * @param (void) 
  * @return (float)  Dato en punto flotante que ha sido recibido mediante EUSART2
*/
float serial2_readFloat()
{
    float dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable flotante de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo flotante de 32(24) bits
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        while(!RC2IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial2_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32(24) bits leído 
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para leer un dato en punto flotante por medio de EUSART3. Polling del bit RC3IF
  * @param (void) 
  * @return (float)  Dato en punto flotante que ha sido recibido mediante EUSART3
*/
float serial3_readFloat()
{
    float dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable flotante de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo flotante de 32(24) bits
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        while(!RC3IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial3_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32(24) bits leído 
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para leer un dato en punto flotante por medio de EUSART4. Polling del bit RC4IF
  * @param (void) 
  * @return (float)  Dato en punto flotante que ha sido recibido mediante EUSART4
*/
float serial4_readFloat()
{
    float dato_leido;
    uint8_t  * p_lectura;                    //Apuntador a variable flotante de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo flotante de 32(24) bits
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        while(!RC4IF);                  //En espera de recepción de datos
        *(p_lectura++) = serial4_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32(24) bits leído 
}
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
  * @brief Función para leer un dato de cualquier tipo por medio de USART
  * @param datos: (void*) Dato de cualquier tipo a recibir mediante USART
  * @param len: (uint16_t) Cantidad de bytes a recibir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial_read(void* datos, uint16_t len)
{
    uint8_t* _datos = (uint8_t*)datos;           //Apuntador a variable
    while(len--)
    {
        while(!RCIF);                  //En espera de recepción de datos
        *(_datos++) = serial_readByte();      //Recepción de datos
    }
}
#endif

#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V10) ||\
    defined (EAUSART_V11) || defined (EAUSART_V11_1) || defined (EAUSART_V12)
/**
  * @brief Función para leer un dato de cualquier tipo por medio de USART1
  * @param datos: (void*) Dato de cualquier tipo a recibir mediante USART1
  * @param len: (uint16_t) Cantidad de bytes a recibir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial1_read(void* datos, uint16_t len)
{
    uint8_t* _datos = (uint8_t*)datos;           //Apuntador a variable
    while(len--)
    {
        while(!RC1IF);                  //En espera de recepción de datos
        *(_datos++) = serial1_readByte();      //Recepción de datos
    }
}

#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11)\
    || defined (EAUSART_V12)
/**
  * @brief Función para leer un dato de cualquier tipo por medio de USART2
  * @param datos: (void*) Dato de cualquier tipo a recibir mediante USART2
  * @param len: (uint16_t) Cantidad de bytes a recibir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial2_read(void* datos, uint16_t len)
{
    uint8_t* _datos = (uint8_t*)datos;           //Apuntador a variable
    while(len--)
    {
        while(!RC2IF);                  //En espera de recepción de datos
        *(_datos++) = serial2_readByte();      //Recepción de datos
    }
}
#endif

//USART3
#if defined (EAUSART_V12)
/**
  * @brief Función para leer un dato de cualquier tipo por medio de USART3
  * @param datos: (void*) Dato de cualquier tipo a recibir mediante USART3
  * @param len: (uint16_t) Cantidad de bytes a recibir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial3_read(void* datos, uint16_t len)
{
    uint8_t* _datos = (uint8_t*)datos;           //Apuntador a variable
    while(len--)
    {
        while(!RC3IF);                  //En espera de recepción de datos
        *(_datos++) = serial3_readByte();      //Recepción de datos
    }
}
#endif

//USART4
#if defined (EAUSART_V12)
/**
  * @brief Función para leer un dato de cualquier tipo por medio de USART4
  * @param datos: (void*) Dato de cualquier tipo a recibir mediante USART4
  * @param len: (uint16_t) Cantidad de bytes a recibir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial4_read(void* datos, uint16_t len)
{
    uint8_t* _datos = (uint8_t*)datos;           //Apuntador a variable
    while(len--)
    {
        while(!RC4IF);                  //En espera de recepción de datos
        *(_datos++) = serial4_readByte();      //Recepción de datos
    }
}
#endif



//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
#ifdef SERIAL_RX_BUFFER
/**
 * @brief  Función para utilizarse si se desea implementar un buffer por software para el puerto serie.
 * Se deberá incluir en la rutina de interrupción por recepción exitosa en el módulo EUSART (verificando estado alto de la bandera RCIF)  
 * @param (void) 
 * @return (void)
*/
void serial_interruptHandler()
{
    uint8_t dato = serial_readByte(); //Lee dato recibido
    if(((serial_queueEnd==SERIAL_RX_BUFFER_SIZE-1) && serial_queueFront==0 ) || ((serial_queueEnd+1)==serial_queueFront))
    {
        //Si buffer lleno, índice en la primera posición
        serial_queueFront++;
        if(serial_queueFront==SERIAL_RX_BUFFER_SIZE) 
            serial_queueFront=0;
    }

    if(serial_queueEnd==SERIAL_RX_BUFFER_SIZE-1)
        serial_queueEnd=0;
    else
        serial_queueEnd++;
    buffer_rx_serial[serial_queueEnd]=dato;
    if(serial_queueFront==-1) 
        serial_queueFront=0;
}
#endif
#else
#ifdef SERIAL1_RX_BUFFER
/**
 * @brief  Función para utilizarse si se desea implementar un buffer por software para el puerto serie.
 * Se deberá incluir en la rutina de interrupción por recepción exitosa en el módulo EUSART1 (verificando estado alto de la bandera RC1IF)  
 * @param (void) 
 * @return (void)
*/
void serial1_interruptHandler() {
    uint8_t dato=serial1_readByte(); //Lee dato recibido
    if(((serial1_queueEnd==SERIAL1_RX_BUFFER_SIZE-1) && serial1_queueFront==0 ) || ((serial1_queueEnd+1)==serial1_queueFront))
    {
        //Si buffer lleno, índice en la primera posición
       serial1_queueFront++;
        if(serial1_queueFront==SERIAL1_RX_BUFFER_SIZE) 
            serial1_queueFront=0;
    }

    if(serial1_queueEnd==SERIAL1_RX_BUFFER_SIZE-1)
        serial1_queueEnd=0;
    else
        serial1_queueEnd++;
    buffer_rx_serial1[serial1_queueEnd]=dato;
    if(serial1_queueFront==-1) 
        serial1_queueFront=0;
}
#endif
#endif
//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
#ifdef SERIAL2_RX_BUFFER
/**
 * @brief  Función para utilizarse si se desea implementar un buffer por software para el puerto serie.
 * Se deberá incluir en la rutina de interrupción por recepción exitosa en el módulo EUSART2 (verificando estado alto de la bandera RC2IF)  
 * @param (void) 
 * @return (void)
*/
void serial2_interruptHandler()
{
    uint8_t dato=serial2_readByte(); //Lee dato recibido
    if(((serial2_queueEnd==SERIAL1_RX_BUFFER_SIZE-1) && serial2_queueFront==0 ) || ((serial2_queueEnd+1)==serial2_queueFront))
    {
        //Si buffer lleno, índice en la primera posición
       serial2_queueFront++;
        if(serial2_queueFront==SERIAL1_RX_BUFFER_SIZE) 
            serial2_queueFront=0;
    }

    if(serial2_queueEnd==SERIAL1_RX_BUFFER_SIZE-1)
        serial2_queueEnd=0;
    else
        serial2_queueEnd++;
    buffer_rx_serial2[serial2_queueEnd]=dato;
    if(serial2_queueFront==-1) 
        serial2_queueFront=0;
}
#endif
#endif

//USART3
#if defined (EAUSART_V12)
#ifdef SERIAL3_RX_BUFFER
/**
 * @brief  Función para utilizarse si se desea implementar un buffer por software para el puerto serie.
 * Se deberá incluir en la rutina de interrupción por recepción exitosa en el módulo EUSART3 (verificando estado alto de la bandera RC3IF)  
 * @param (void) 
 * @return (void)
*/
void serial3_interruptHandler()
{
    uint8_t dato = serial3_readByte(); //Lee dato recibido
    if(((serial3_queueEnd==SERIAL1_RX_BUFFER_SIZE-1) && serial3_queueFront==0 ) || ((serial3_queueEnd+1)==serial3_queueFront))
    {
        //Si buffer lleno, índice en la primera posición
       serial3_queueFront++;
        if(serial3_queueFront==SERIAL1_RX_BUFFER_SIZE) 
            serial3_queueFront=0;
    }

    if(serial3_queueEnd==SERIAL1_RX_BUFFER_SIZE-1)
        serial3_queueEnd=0;
    else
        serial3_queueEnd++;
    buffer_rx_serial3[serial3_queueEnd]=dato;
    if(serial3_queueFront==-1) 
        serial3_queueFront=0;
}
#endif
#endif

//USART4
#if defined (EAUSART_V12)
#ifdef SERIAL4_RX_BUFFER
/**
 * @brief  Función para utilizarse si se desea implementar un buffer por software para el puerto serie.
 * Se deberá incluir en la rutina de interrupción por recepción exitosa en el módulo EUSART4 (verificando estado alto de la bandera RC4IF)  
 * @param (void) 
 * @return (void)
*/
void serial4_interruptHandler()
{
    uint8_t dato = serial4_readByte(); //Lee dato recibido
    if(((serial4_queueEnd==SERIAL1_RX_BUFFER_SIZE-1) && serial4_queueFront==0 ) || ((serial4_queueEnd+1)==serial4_queueFront))
    {
        //Si buffer lleno, índice en la primera posición
       serial4_queueFront++;
        if(serial4_queueFront==SERIAL1_RX_BUFFER_SIZE) 
            serial4_queueFront=0;
    }

    if(serial4_queueEnd==SERIAL1_RX_BUFFER_SIZE-1)
        serial4_queueEnd=0;
    else
        serial4_queueEnd++;
    buffer_rx_serial4[serial4_queueEnd]=dato;
    if(serial4_queueFront==-1) 
        serial4_queueFront=0;
}
#endif
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
#ifdef SERIAL_RX_BUFFER
/**
 * @brief Función que devuelve la cantidad de datos presente en el buffer SERIAL
 * @param (void) 
 * @return (uint16_t) Cantidad de bytes presentes en el buffer por software asociado a EUSART 
*/
uint16_t serial_dataAvailable()
{
    if(serial_queueFront==-1) 
        return 0;
    if(serial_queueFront<serial_queueEnd)
        return(serial_queueEnd-serial_queueFront+1);
    else if(serial_queueFront>serial_queueEnd)
        return (SERIAL_RX_BUFFER_SIZE-serial_queueFront+serial_queueEnd+1);
    else
        return 1;
}
#endif
#else
#ifdef SERIAL1_RX_BUFFER
/**
 * @brief Función que devuelve la cantidad de datos presente en el buffer SERIAL1
 * @param (void) 
 * @return (uint8_t) Cantidad de bytes presentes en el buffer por software asociado a EUSART1
*/
uint16_t serial1_dataAvailable()
{
    if(serial1_queueFront==-1) 
        return 0;
    if(serial1_queueFront<serial1_queueEnd)
        return(serial1_queueEnd-serial1_queueFront+1);
    else if(serial1_queueFront>serial1_queueEnd)
        return (SERIAL1_RX_BUFFER_SIZE-serial1_queueFront+serial1_queueEnd+1);
    else
        return 1;
}
#endif
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
#ifdef SERIAL2_RX_BUFFER
/**
 * @brief Función que devuelve la cantidad de datos presente en el buffer SERIAL2
 * @param (void) 
 * @return (uint8_t) Cantidad de bytes presentes en el buffer por software asociado a EUSART2
*/
uint16_t serial2_dataAvailable()
{
    if(serial2_queueFront==-1) 
        return 0;
    if(serial2_queueFront<serial2_queueEnd)
        return(serial2_queueEnd-serial2_queueFront+1);
    else if(serial2_queueFront>serial2_queueEnd)
        return (SERIAL2_RX_BUFFER_SIZE-serial2_queueFront+serial2_queueEnd+1);
    else
        return 1;
}
#endif
#endif

//USART3
#if defined (EAUSART_V12)
#ifdef SERIAL3_RX_BUFFER
/**
 * @brief Función que devuelve la cantidad de datos presente en el buffer SERIAL3
 * @param (void) 
 * @return (uint8_t) Cantidad de bytes presentes en el buffer por software asociado a EUSART3
*/
uint16_t serial3_dataAvailable()
{
    if(serial3_queueFront==-1) 
        return 0;
    if(serial3_queueFront<serial3_queueEnd)
        return(serial3_queueEnd-serial3_queueFront+1);
    else if(serial3_queueFront>serial3_queueEnd)
        return (SERIAL3_RX_BUFFER_SIZE-serial3_queueFront+serial3_queueEnd+1);
    else
        return 1;
}
#endif
#endif

//USART4
#if defined (EAUSART_V12)
#ifdef SERIAL4_RX_BUFFER
/**
 * @brief Función que devuelve la cantidad de datos presente en el buffer SERIAL4
 * @param (void) 
 * @return (uint8_t) Cantidad de bytes presentes en el buffer por software asociado a EUSART4
*/
uint16_t serial4_dataAvailable()
{
    if(serial4_queueFront==-1) 
        return 0;
    if(serial4_queueFront<serial4_queueEnd)
        return(serial4_queueEnd-serial4_queueFront+1);
    else if(serial4_queueFront>serial4_queueEnd)
        return (SERIAL4_RX_BUFFER_SIZE-serial4_queueFront+serial4_queueEnd+1);
    else
        return 1;
}
#endif
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
#ifdef SERIAL_RX_BUFFER
/**
 * @brief Función que devuelve el primer dato presente en el buffer EUSART modificando los índices de la cola.
 * Es decir, de la cola saldrá el primer elemento (FIFO)
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
int8_t serial_readByteBuffer() {
    uint8_t dato;
    //Verificar si la cola está vacía
    if(serial_queueFront==-1)
    return -1;           //Devuelve entonces -1

    dato=buffer_rx_serial[serial_queueFront];

    if(serial_queueFront==serial_queueEnd)
    {
        //Si solo hay un dato, la cola se encuentra vacía ahora
        serial_queueFront=-1;
        serial_queueEnd=-1;
    }
    else
    {
        serial_queueFront++;
        if(serial_queueFront==SERIAL_RX_BUFFER_SIZE)
            serial_queueFront=0;
    }
    return dato;
}
#endif
#else
#ifdef SERIAL1_RX_BUFFER
/**
 * @brief Función que devuelve el primer dato presente en el buffer EUSART1 modificando los índices de la cola.
 * Es decir, de la cola saldrá el primer elemento (FIFO)
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
int8_t serial1_readByteBuffer() {
    uint8_t dato;
    //Verificar si la cola está vacía
    if(serial1_queueFront==-1)
    return -1;           //Devuelve entonces -1

    dato=buffer_rx_serial1[serial1_queueFront];

    if(serial1_queueFront==serial1_queueEnd)
    {
        //Si solo hay un dato, la cola se encuentra vacía ahora
        serial1_queueFront=-1;
        serial1_queueEnd=-1;
    }
    else
    {
        serial1_queueFront++;
        if(serial1_queueFront==SERIAL1_RX_BUFFER_SIZE)
            serial1_queueFront=0;
    }

    return dato;
}
#endif
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
#ifdef SERIAL2_RX_BUFFER
/**
 * @brief Función que devuelve el primer dato presente en el buffer EUSART2 modificando los índices de la cola.
 * Es decir, de la cola saldrá el primer elemento (FIFO)
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
int8_t serial2_readByteBuffer() {
    uint8_t dato;
    //Verificar si la cola está vacía
    if(serial2_queueFront==-1)
    return -1;           //Devuelve entonces -1

    dato=buffer_rx_serial2[serial2_queueFront];

    if(serial2_queueFront==serial2_queueEnd)
    {
        //Si solo hay un dato, la cola se encuentra vacía ahora
        serial2_queueFront=-1;
        serial2_queueEnd=-1;
    }
    else
    {
        serial2_queueFront++;
        if(serial2_queueFront==SERIAL2_RX_BUFFER_SIZE)
            serial1_queueFront=0;
    }

    return dato;
}
#endif
#endif

//USART3
#if defined (EAUSART_V12)
#ifdef SERIAL3_RX_BUFFER
/**
 * @brief Función que devuelve el primer dato presente en el buffer EUSART3 modificando los índices de la cola.
 * Es decir, de la cola saldrá el primer elemento (FIFO)
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
int8_t serial3_readByteBuffer() {
    uint8_t dato;
    //Verificar si la cola está vacía
    if(serial3_queueFront==-1)
    return -1;           //Devuelve entonces -1

    dato=buffer_rx_serial3[serial3_queueFront];

    if(serial3_queueFront==serial3_queueEnd)
    {
        //Si solo hay un dato, la cola se encuentra vacía ahora
        serial3_queueFront=-1;
        serial3_queueEnd=-1;
    }
    else
    {
        serial3_queueFront++;
        if(serial3_queueFront==SERIAL3_RX_BUFFER_SIZE)
            serial3_queueFront=0;
    }

    return dato;
}
#endif
#endif

//USART4
#if defined (EAUSART_V12)
#ifdef SERIAL4_RX_BUFFER
/**
 * @brief Función que devuelve el primer dato presente en el buffer EUSART4 modificando los índices de la cola.
 * Es decir, de la cola saldrá el primer elemento (FIFO)
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
int8_t serial4_readByteBuffer() {
    uint8_t dato;
    //Verificar si la cola está vacía
    if(serial4_queueFront==-1)
    return -1;           //Devuelve entonces -1

    dato=buffer_rx_serial4[serial4_queueFront];

    if(serial4_queueFront==serial4_queueEnd)
    {
        //Si solo hay un dato, la cola se encuentra vacía ahora
        serial4_queueFront=-1;
        serial4_queueEnd=-1;
    }
    else
    {
        serial4_queueFront++;
        if(serial4_queueFront==SERIAL4_RX_BUFFER_SIZE)
            serial4_queueFront=0;
    }

    return dato;
}
#endif
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
#ifdef SERIAL_RX_BUFFER
/**
 * @brief Función que devuelve el primer dato presente en el buffer EUSART sin modificar índices de la cola.
 * Es solo una función de consulta del primer dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos.
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
int8_t serial_firstByteReceived() {
    return buffer_rx_serial[serial_queueFront];
}
#endif
#else
#ifdef SERIAL1_RX_BUFFER
/**
 * @brief Función que devuelve el primer dato presente en el buffer EUSART1 sin modificar índices de la cola.
 * Es solo una función de consulta del primer dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos.
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
int8_t serial1_firstByteReceived() {
    return buffer_rx_serial1[serial1_queueFront];
}
#endif
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
#ifdef SERIAL2_RX_BUFFER
/**
 * @brief Función que devuelve el primer dato presente en el buffer EUSART2 sin modificar índices de la cola.
 * Es solo una función de consulta del primer dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos.
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
int8_t serial2_firstByteReceived() {
    return buffer_rx_serial2[serial2_queueFront];
}
#endif
#endif

//USART3
#if defined (EAUSART_V12)
#ifdef SERIAL3_RX_BUFFER
/**
 * @brief Función que devuelve el primer dato presente en el buffer EUSART3 sin modificar índices de la cola.
 * Es solo una función de consulta del primer dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos.
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
int8_t serial3_firstByteReceived() {
    return buffer_rx_serial3[serial3_queueFront];
}
#endif
#endif

//USART4
#if defined (EAUSART_V12)
#ifdef SERIAL4_RX_BUFFER
/**
 * @brief Función que devuelve el primer dato presente en el buffer EUSART4 sin modificar índices de la cola.
 * Es solo una función de consulta del primer dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos.
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
int8_t serial4_firstByteReceived() {
    return buffer_rx_serial4[serial4_queueFront];
}
#endif
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
#ifdef SERIAL_RX_BUFFER
/**
 * @brief Función que devuelve el último dato presente en el buffer EUSART sin modificar índices de la cola.
 * Es solo una función de consulta del último dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos.
 * @param (void) 
 * @return (uint8_t) último dato de la cola o buffer circular
*/
int8_t serial_lastByteReceived() {
    int8_t dato;
    dato = (serial_queueEnd==-1)? -1:buffer_rx_serial[serial_queueEnd];   
    //Verificar si la cola está vacía, en caso contrario devuelve último valor de la cola
    return dato;
}
#endif
#else
#ifdef SERIAL1_RX_BUFFER
/**
 * @brief Función que devuelve el último dato presente en el buffer EUSART sin modificar índices de la cola.
 * Es solo una función de consulta del último dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos.
 * @param (void) 
 * @return (uint8_t) último dato de la cola o buffer circular
*/
int8_t serial1_lastByteReceived() {
    int8_t dato;
    dato = (serial1_queueEnd==-1)? -1:buffer_rx_serial1[serial1_queueEnd];   
    //Verificar si la cola está vacía, en caso contrario devuelve último valor de la cola
    return dato;
}
#endif
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
#ifdef SERIAL2_RX_BUFFER
/**
 * @brief Función que devuelve el último dato presente en el buffer EUSART sin modificar índices de la cola.
 * Es solo una función de consulta del último dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos.
 * @param (void) 
 * @return (uint8_t) último dato de la cola o buffer circular
*/
int8_t serial2_lastByteReceived() {
    int8_t dato;
    dato = (serial2_queueEnd==-1)? -1:buffer_rx_serial2[serial2_queueEnd];   
    //Verificar si la cola está vacía, en caso contrario devuelve último valor de la cola
    return dato;
}
#endif
#endif

//USART3
#if defined (EAUSART_V12)
#ifdef SERIAL3_RX_BUFFER
/**
 * @brief Función que devuelve el último dato presente en el buffer EUSART sin modificar índices de la cola.
 * Es solo una función de consulta del último dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos.
 * @param (void) 
 * @return (uint8_t) último dato de la cola o buffer circular
*/
int8_t serial3_lastByteReceived() {
    int8_t dato;
    dato = (serial3_queueEnd==-1)? -1:buffer_rx_serial3[serial3_queueEnd];   
    //Verificar si la cola está vacía, en caso contrario devuelve último valor de la cola
    return dato;
}
#endif
#endif

//USART4
#if defined (EAUSART_V12)
#ifdef SERIAL4_RX_BUFFER
/**
 * @brief Función que devuelve el último dato presente en el buffer EUSART sin modificar índices de la cola.
 * Es solo una función de consulta del último dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos.
 * @param (void) 
 * @return (uint8_t) último dato de la cola o buffer circular
*/
int8_t serial4_lastByteReceived() {
    int8_t dato;
    dato = (serial4_queueEnd==-1)? -1:buffer_rx_serial4[serial4_queueEnd];   
    //Verificar si la cola está vacía, en caso contrario devuelve último valor de la cola
    return dato;
}
#endif
#endif




//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
#ifdef SERIAL_RX_BUFFER
/**
 * @brief Función para lectura de una cantidad de elementos definida desde el buffer serial circular EUSART
 * implementado por software, para ponerlos en otro buffer (arreglo)  
 * @param buff (int8_t *) Apuntador al arreglo en el cual se desean copiar los datos.
 * @param len (uint8_t) cantidad de elementos a copiar de un buffer a otro 
 * @return (void)
*/
void serial_readBuffer(uint8_t *buff,uint8_t len) {
    for(uint8_t i=0;i!=len;i++) {
        buff[i]=serial_readByteBuffer();
    }
}
#endif
#else
#ifdef SERIAL1_RX_BUFFER
/**
 * @brief Función para lectura de una cantidad de elementos definida desde el buffer serial circular EUSART1
 * implementado por software, para ponerlos en otro buffer (arreglo)  
 * @param buff (int8_t *) Apuntador al arreglo en el cual se desean copiar los datos.
 * @param len (uint8_t) cantidad de elementos a copiar de un buffer a otro 
 * @return (void)
*/
void serial1_readBuffer(uint8_t *buff,uint8_t len) {
    for(uint8_t i=0;i!=len;i++) {
        buff[i]=serial1_readByteBuffer();
    }
}
#endif
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
#ifdef SERIAL2_RX_BUFFER
/**
 * @brief Función para lectura de una cantidad de elementos definida desde el buffer serial circular EUSART2
 * implementado por software, para ponerlos en otro buffer (arreglo)  
 * @param buff (int8_t *) Apuntador al arreglo en el cual se desean copiar los datos.
 * @param len (uint8_t) cantidad de elementos a copiar de un buffer a otro 
 * @return (void)
*/
void serial2_readBuffer(uint8_t *buff,uint8_t len) {
    for(uint8_t i=0;i!=len;i++) {
        buff[i]=serial2_readByteBuffer();
    }
}
#endif
#endif

//USART3
#if defined (EAUSART_V12)
#ifdef SERIAL3_RX_BUFFER
/**
 * @brief Función para lectura de una cantidad de elementos definida desde el buffer serial circular EUSART3
 * implementado por software, para ponerlos en otro buffer (arreglo)  
 * @param buff (int8_t *) Apuntador al arreglo en el cual se desean copiar los datos.
 * @param len (uint8_t) cantidad de elementos a copiar de un buffer a otro 
 * @return (void)
*/
void serial3_readBuffer(uint8_t *buff,uint8_t3 len) {
    for(uint8_t i=0;i!=len;i++) {
        buff[i]=serial3_readByteBuffer();
    }
}
#endif
#endif

//USART4
#if defined (EAUSART_V12)
#ifdef SERIAL4_RX_BUFFER
/**
 * @brief Función para lectura de una cantidad de elementos definida desde el buffer serial circular EUSART4
 * implementado por software, para ponerlos en otro buffer (arreglo)  
 * @param buff (int8_t *) Apuntador al arreglo en el cual se desean copiar los datos.
 * @param len (uint8_t) cantidad de elementos a copiar de un buffer a otro 
 * @return (void)
*/
void serial4_readBuffer(uint8_t *buff,uint8_t4 len) {
    for(uint8_t i=0;i!=len;i++) {
        buff[i]=serial4_readByteBuffer();
    }
}
#endif
#endif

//USART1
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
#ifdef SERIAL_RX_BUFFER
/**
 * @brief Función para limpiar buffer serial reiniciando los valores de los índices del buffer EUSART
 * @param (void)
 * @return (void)
*/
void serial_flushBuffer() {
    serial_queueEnd     = -1;
    serial_queueFront   = -1;
}
#endif
#else
#ifdef SERIAL1_RX_BUFFER
/**
 * @brief Función para limpiar buffer serial reiniciando los valores de los índices del buffer EUSART1
 * @param (void)
 * @return (void)
*/
void serial1_flushBuffer() {
    serial1_queueEnd     = -1;
    serial1_queueFront   = -1;
}
#endif
#endif

//USART2
#if defined (AUSART_V2) || defined (EAUSART_V6)|| defined (EAUSART_V7) ||\
    defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) \
    || defined (EAUSART_V12)
#ifdef SERIAL2_RX_BUFFER
/**
 * @brief Función para limpiar buffer serial reiniciando los valores de los índices del buffer EUSART2
 * @param (void)
 * @return (void)
*/
void serial2_flushBuffer() {
    serial2_queueEnd     = -1;
    serial2_queueFront   = -1;
}
#endif
#endif

//USART3
#if defined (EAUSART_V12)
#ifdef SERIAL3_RX_BUFFER
/**
 * @brief Función para limpiar buffer serial reiniciando los valores de los índices del buffer EUSART3
 * @param (void)
 * @return (void)
*/
void serial3_flushBuffer() {
    serial3_queueEnd     = -1;
    serial3_queueFront   = -1;
}
#endif
#endif

//USART4
#if defined (EAUSART_V12)
#ifdef SERIAL4_RX_BUFFER
/**
 * @brief Función para limpiar buffer serial reiniciando los valores de los índices del buffer EUSART4
 * @param (void)
 * @return (void)
*/
void serial4_flushBuffer() {
    serial4_queueEnd     = -1;
    serial4_queueFront   = -1;
}
#endif
#endif

#if defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
/**
 * @brief Función para configurar características del registro BAUDCON si se encuentra presente
 * @param param_config: (uint8_t) Parámetros para configurar el registro BAUDCON
 * @return (void)
*/
void serial_baudcon(uint8_t param_config) {
    BAUDCON = param_config;
}
#endif

#if defined (EAUSART_V6)|| defined (EAUSART_V7) || defined (EAUSART_V8) ||\
    defined (EAUSART_V9) || defined (EAUSART_V10) || defined (EAUSART_V11) || defined (EAUSART_V11_1) \
    || defined (EAUSART_V12)
/**
 * @brief Función para configurar características del registro BAUDCON1 si se encuentra presente
 * @param param_config: (uint8_t) Parámetros para configurar el registro BAUDCON1
 * @return (void)
*/
void serial1_baudcon(uint8_t param_config) {
    BAUDCON1 = param_config;
}
#endif

#if defined (EAUSART_V7) || defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) || defined (EAUSART_V12)
/**
 * @brief Función para configurar características del registro BAUDCON2 si se encuentra presente
 * @param param_config: (uint8_t) Parámetros para configurar el registro BAUDCON2
 * @return (void)
*/
void serial2_baudcon(uint8_t param_config) {
    BAUDCON2 = param_config;
}
#endif

#if defined (EAUSART_V12)
/**
 * @brief Función para configurar características del registro BAUDCON3 si se encuentra presente
 * @param param_config: (uint8_t) Parámetros para configurar el registro BAUDCON3
 * @return (void)
*/
void serial3_baudcon(uint8_t param_config) {
    BAUDCON3 = param_config;
}
#endif

#if defined (EAUSART_V12)
/**
 * @brief Función para configurar características del registro BAUDCON4 si se encuentra presente
 * @param param_config: (uint8_t) Parámetros para configurar el registro BAUDCON4
 * @return (void)
*/
void serial4_baudcon(uint8_t param_config) {
    BAUDCON4 = param_config;
}
#endif


