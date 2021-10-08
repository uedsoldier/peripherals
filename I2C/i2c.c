/**
 * @file i2c.c
 * @brief Librería para utilización de módulo MSSP (i2c) por hardware para microcontroladores PIC de 8 bits.
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/

#include "i2c.h"


/*
    Funciones de configuración de modo i2c por hardware.
    Parámetros: 
    - opciones_sspcon: Configuración de modo maestro o esclavo ya sea de 7 o 10 bits de direccionamiento
    - opciones_slew_rate: Control de slew-rate para velocidad de 400 [kHz]
    - opciones_sspadd: 
        + En modo maestro, velocidad de reloj en [kHz]. Opciones comunes: 100, 400 y 1000
        + En modo esclavo, dirección deseada del dispositivo esclavo
*/
#if defined (I2C_V1)
void i2c_init(uint8_t opciones_sspcon, uint8_t opciones_slew_rate, uint16_t opciones_sspadd) {
    SSPSTAT &= 0x3F;        //Estado en power-on
    SSPCON1 = 0x00;         //Estado en power-on
    SSPCON2 = 0x00;         //Estado en power-on
    SSPCON1 |= opciones_sspcon;   //Selecciona modalidad I2C
    switch(opciones_sspcon) {
        case I2C_MASTER:case I2C_MASTER_FIRMWARE:   //En modo maestro...
            i2c_setSpeed(opciones_sspadd);
            break; 
        default:        //En modo esclavo...
            SSPADD = (uint8_t)opciones_sspadd;   //Simplemente establece dirección deseada de dispositivo esclavo
            break;
    }   
    SSPSTAT |= opciones_slew_rate;        //Configura control de slew-rate 
    //Configuración de pines SDA y SCL
    I2C_SCL_TRIS = 1;
    I2C_SDA_TRIS = 1;
    SSPCON1bits.SSPEN=1;   //Habilita el MSSP y usa los pines correspondintes como SDA y SCL. Habilita el hardware i2c 
}

#endif

#if defined (I2C_V4)
void i2c_init(uint8_t opciones_sspcon, uint8_t opciones_slew_rate, uint16_t opciones_sspadd) {
    SSPSTAT &= 0x3F;        //Estado en power-on
    SSPCON = 0x00;         //Estado en power-on
    SSPCON |= opciones_sspcon;   //Selecciona modalidad I2C
    switch(opciones_sspcon) {
        case I2C_MASTER:case I2C_MASTER_FIRMWARE:   //En modo maestro...
            i2c_setSpeed(opciones_sspadd);    
            break; 
        default:        //En modo esclavo...
            SSPADD = (uint8_t)opciones_sspadd;   //Simplemente establece dirección deseada de dispositivo esclavo
            break;
    }   
    SSPSTAT |= opciones_slew_rate;        //Configura control de slew-rate 
    //Configuración de pines SDA y SCL
    I2C_SCL_TRIS = 1;
    I2C_SDA_TRIS = 1;
    SSPCONbits.SSPEN=1;   //Habilita el MSSP y usa los pines correspondintes como SDA y SCL. Habilita el hardware i2c 
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) 
void i2c1_init(uint8_t opciones_sspcon, uint8_t opciones_slew_rate, uint16_t opciones_sspadd) {
    SSP1STAT &= 0x3F;        //Estado en power-on
    SSP1CON1 = 0x00;         //Estado en power-on
    SSP1CON2 = 0x00;         //Estado en power-on
    SSP1CON1 |= opciones_sspcon;   //Selecciona modalidad I2C
    switch(opciones_sspcon) {
        case I2C_MASTER:case I2C_MASTER_FIRMWARE:   //En modo maestro...
            i2c1_setSpeed(opciones_sspadd);
            break; 
        default:        //En modo esclavo...
            SSP1ADD = (uint8_t)opciones_sspadd;   //Simplemente establece dirección deseada de dispositivo esclavo
            break;
    }   
    SSP1STAT |= opciones_slew_rate;        //Configura control de slew-rate 
    //Configuración de pines SDA y SCL
    I2C1_SCL_TRIS = 1;
    I2C1_SDA_TRIS = 1;
    SSP1CON1bits.SSPEN=1;   //Habilita el MSSP y usa los pines correspondintes como SDA y SCL. Habilita el hardware i2c 
}

#endif

#if defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2)
void i2c1_init(uint8_t opciones_sspcon, uint8_t opciones_slew_rate, uint16_t opciones_sspadd) {
    SSP1STAT &= 0x3F;        //Estado en power-on
    SSP1CON1 = 0x00;         //Estado en power-on
    SSP1CON2 = 0x00;         //Estado en power-on
    SSP1CON1 |= opciones_sspcon;   //Selecciona modalidad I2C
    switch(opciones_sspcon) {
        case I2C_MASTER:case I2C_MASTER_FIRMWARE:   //En modo maestro...
            i2c1_setSpeed(opciones_sspadd);
            break; 
        default:        //En modo esclavo...
            SSP1ADD = (uint8_t)opciones_sspadd;   //Simplemente establece dirección deseada de dispositivo esclavo
            break;
    }   
    SSP1STAT |= opciones_slew_rate;        //Configura control de slew-rate 
    I2C1_SCL_TRIS = 1;
    I2C1_SDA_TRIS = 1;
    SSP1CON1bits.SSPEN=1;   //Habilita el MSSP y usa los pines correspondintes como SDA y SCL. Habilita el hardware i2c 
    
}

#endif

#if defined (I2C_V3)
void i2c2_init(uint8_t opciones_sspcon, uint8_t opciones_slew_rate) {
    SSP2STAT &= 0x3F;        //Estado en power-on
    SSP2CON1 = 0x00;         //Estado en power-on
    SSP2CON2 = 0x00;         //Estado en power-on
    SSP2CON1 |= opciones_sspcon;   //Selecciona modalidad I2C
    switch(opciones_sspcon) {
        case I2C_MASTER:case I2C_MASTER_FIRMWARE:   //En modo maestro...
            i2c2_setSpeed(opciones_sspadd);
            break; 
        default:        //En modo esclavo...
            SSP2ADD = (uint8_t)opciones_sspadd;   //Simplemente establece dirección deseada de dispositivo esclavo
            break;
    }   
    SSP2STAT |= opciones_slew_rate;        //Configura control de slew-rate 
    //Configuración de pines SDA y SCL
    I2C2_SCL_TRIS = 1;
    I2C2_SDA_TRIS = 1;
    SSP2CON1bits.SSPEN=1;   //Habilita el MSSP y usa los pines correspondintes como SDA y SCL. Habilita el hardware i2c   
}
#endif

#if defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_init(uint8_t opciones_sspcon, uint8_t opciones_slew_rate, uint16_t opciones_sspadd) {
    SSP2STAT &= 0x3F;        //Estado en power-on
    SSP2CON1 = 0x00;         //Estado en power-on
    SSP2CON2 = 0x00;         //Estado en power-on
    SSP2CON1 |= opciones_sspcon;   //Selecciona modalidad I2C
    switch(opciones_sspcon) {
        case I2C_MASTER:case I2C_MASTER_FIRMWARE:   //En modo maestro...
            i2c2_setSpeed(opciones_sspadd);
            break; 
        default:        //En modo esclavo...
            SSP2ADD = (uint8_t)opciones_sspadd;   //Simplemente establece dirección deseada de dispositivo esclavo
            break;
    }   
    SSP2STAT |= opciones_slew_rate;        //Configura control de slew-rate 
    //Configuración de pines SDA y SCL
    I2C2_SCL_TRIS = 1;
    I2C2_SDA_TRIS = 1;
    SSP2CON1bits.SSPEN = 1;   //Habilita el MSSP y usa los pines correspondintes como SDA y SCL. Habilita el hardware i2c 
    
}
#endif

/*
    Función para generar condición START (S) en el bus i2c por hardware
    Retorno: vacío (void)
*/
#if defined (I2C_V1)
void i2c_start() {
    SSPCON2bits.SEN=1;          //Genera condición START
    while(SSPCON2bits.SEN){}    //Espera a terminar tal condición
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2)
void i2c1_start() {
    SSP1CON2bits.SEN=1;         //Genera condición START
    while(SSP1CON2bits.SEN){}   //Espera a terminar tal condición
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_start() {
    SSP2CON2bits.SEN=1;         //Genera condición START
    while(SSP2CON2bits.SEN){}   //Espera a terminar tal condición
}
#endif


/*
    Función para generar condición STOP (P) en el bus i2c por hardware
    Retorno: vacío (void)
*/
#if defined (I2C_V1)
void i2c_stop() {
    SSPCON2bits.PEN=1;          //Genera condición STOP
    while(SSPCON2bits.PEN){}    //Espera a terminar tal condición
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2)
void i2c1_stop() {
    SSP1CON2bits.PEN=1;          //Genera condición STOP
    while(SSP1CON2bits.PEN){}    //Espera a terminar tal condición
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_stop() {
    SSP2CON2bits.PEN=1;          //Genera condición STOP
    while(SSP2CON2bits.PEN){}    //Espera a terminar tal condición
}
#endif

/*
    Función para generar condición REPEATED START (Sr) en el bus i2c por hardware
    Retorno: vacío (void)
*/
#if defined (I2C_V1)
void i2c_restart() {
    SSPCON2bits.RSEN=1;          //Genera condición RESTART
    while(SSPCON2bits.RSEN){}    //Espera a terminar tal condición
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2)
void i2c1_restart() {
    SSP1CON2bits.RSEN=1;          //Genera condición RESTART
    while(SSP1CON2bits.RSEN){}    //Espera a terminar tal condición
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_restart() {
    SSP2CON2bits.RSEN=1;          //Genera condición RESTART
    while(SSP2CON2bits.RSEN){}    //Espera a terminar tal condición
}
#endif


/*
    Función para escribir un byte en el bus i2c por hardware
    Parámetros: 
    dato: Dato de 8 bits a trasmitir mediante el bus i2c por hardware
    Retorno: bit !ACK (acknowledge), 0 indica ACK, mientras 1 indica NACK
*/
#if defined (I2C_V1)

i2c_status_t i2c_writeByte(uint8_t dato) {
    SSPIF = 0;
    SSPBUF=dato;            //Manda datos al buffer del MSSP
    while(!SSPIF){}
    if ( SSPCON1bits.WCOL ) //Prueba si ocurrió colisión de datos a escribir (Write Colision)
        return I2C_WRITE_COLLISION;          //Si ésta última ocurrió,retornar con -1 (I2C_WRITE_COLLISION)
    else {
        if( ( (SSPCON1&0x0F) == 0x08 ) || ( (SSPCON1&0x0F) == 0x0B) ) { //Modo maestro...
            while(SSPSTATbits.BF);  //Esperar hasta que el buffer de transmisión se vacíe
            while(SSPSTATbits.R_W); //Esperar hasta que se acomplete el ciclo de transmisión 
            //Verificación de condición recibida: 1-NACK 0-ACK
            return (SSPCON2bits.ACKSTAT)? I2C_NACK:I2C_ACK;
        }
        else {                      //Modo esclavo
            SSPCON1bits.CKP=1;          //Soltar línea de reloj
            while ( !PIR1bits.SSPIF );  //Esperar hasta recibir noveno pulso de reloj   
            //Verificación de condición recibida: 1-NACK 0-ACK
            return ( ( !SSPSTATbits.R_W ) && ( !SSPSTATbits.BF ) )? I2C_NACK:I2C_ACK; 
        }
    }
}
#endif

#if defined (I2C_V4)
i2c_status_t i2c_writeByte(uint8_t dato) {
    SSPIF = 0;
    SSPBUF = dato;            //Manda datos al buffer del MSSP
    while(!SSPIF){}
    if ( SSPCONbits.WCOL ) //Prueba si ocurrió colisión de datos a escribir (Write Colision)
        return I2C_WRITE_COLLISION;          //Si ésta última ocurrió,retornar con -1 (I2C_WRITE_COLLISION)
    else {
        while(SSPSTATbits.BF);  //Esperar hasta que el buffer de transmisión se vacíe
        return 0; //ACK

    }

#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2) 
i2c_status_t i2c1_writeByte(uint8_t dato) {
    SSP1IF = 0;
    SSP1BUF = dato;            //Manda datos al buffer del MSSP
    while(!SSP1IF){}
    if ( SSP1CON1bits.WCOL ) //Prueba si ocurrió colisión de datos a escribir (Write Colision)
        return I2C_WRITE_COLLISION;          //Si ésta última ocurrió,retornar con -1 (I2C_WRITE_COLLISION)
    else {
        if( ( (SSP1CON1&0x0F) == 0x08 ) || ( (SSP1CON1&0x0F) == 0x0B) ) {  //Modo maestro...
            while(SSP1STATbits.BF){};  //Esperar hasta que el buffer de transmisión se vacíe
            while(SSP1STATbits.R_W){}; //Esperar hasta que se acomplete el ciclo de transmisión 
            //Verificación de condición recibida: 1-NACK 0-ACK
            return (SSP1CON2bits.ACKSTAT)? I2C_NACK:I2C_ACK;
        }
        else {                       //Modo esclavo
            SSP1CON1bits.CKP=1;          //Soltar línea de reloj
            while ( !PIR1bits.SSP1IF );  //Esperar hasta recibir noveno pulso de reloj   
            //Verificación de condición recibida: 1-NACK 0-ACK
            #if defined (I2C_V6_2)
            return ( ( !SSP1STATbits.R_NOT_W ) && ( !SSP1STATbits.BF ) )? I2C_NACK:I2C_ACK; 
            #else
            return ( ( !SSP1STATbits.R_W ) && ( !SSP1STATbits.BF ) )? I2C_NACK:I2C_ACK; 
            #endif
        }
    }
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
i2c_status_t i2c2_writeByte(uint8_t dato) {
    SSP2IF = 0;
    SSP2BUF = dato;            //Manda datos al buffer del MSSP
    while(!SSP2IF){}
    if ( SSP2CON1bits.WCOL ) //Prueba si ocurrió colisión de datos a escribir (Write Colision)
        return I2C_WRITE_COLLISION;          //Si ésta última ocurrió,retornar con -1 (I2C_WRITE_COLLISION)
    else {
        if( ( (SSP2CON1&0x0F) == 0x08 ) || ( (SSP2CON1&0x0F) == 0x0B) )  //Modo maestro...
        {
            while(SSP2STATbits.BF);  //Esperar hasta que el buffer de transmisión se vacíe
            while(SSP2STATbits.R_W); //Esperar hasta que se acomplete el ciclo de transmisión
            //Verificación de condición recibida: 1-NACK 0-ACK
            return (SSP2CON2bits.ACKSTAT)? I2C_NACK:I2C_ACK;
        }
        else                       //Modo esclavo
        {
            //SSP2CON1bits.CKP=1;          //Soltar línea de reloj
            // while ( !PIR1bits.SSP2IF );  //Esperar hasta recibir noveno pulso de reloj   
            //Verificación de condición recibida: 1-NACK 0-ACK
            #if defined (I2C_SFR_V1)
            return ( ( !SSP2STATbits.R_NOT_W ) && ( !SSP2STATbits.BF ) )? I2C_NACK:I2C_ACK; 
            #else
            return ( ( !SSP2STATbits.R_W ) && ( !SSP2STATbits.BF ) )? I2C_NACK:I2C_ACK; 
            #endif
        }
    }
}


#endif


/*
    Función para leer un byte del bus i2c por hardware
    Parámetros: 
    ack: Bit ACK a enviar a los dispositivos esclavos, según su lógica interna
    Retorno: Dato de 8 bits desde algún dispositivo esclavo
*/
#if defined (I2C_V1)
uint8_t i2c_readByte(uint8_t ack)
{
    uint8_t dato;
    SSPCON2bits.RCEN=1;             //Habilita recepción de datos en modo maestro 
    while ( !SSPSTATbits.BF );      //Espera hasta recibir byte
    dato = SSPBUF;                  //Lectura de buffer MSSP
    SSPCON2bits.ACKDT = (ack)? 0:1;   //Condición de ACK o NACK según necesidades de la aplicación
    SSPCON2bits.ACKEN=1;            //Genera dicha condición
    while(SSPCON2bits.ACKEN);       //Espera a que termine tal condición
    return dato;                    //Retorno de valor
}
#endif

#if defined (I2C_V4)
uint8_t i2c_readByte(uint8_t ack)
{
    //uint8_t dato;
    //SSPCON2bits.RCEN=1;             //Habilita recepción de datos en modo maestro 
    while ( !SSPSTATbits.BF );      //Espera hasta recibir byte
    //dato = SSPBUF;                  //Lectura de buffer MSSP
    //SSPCON2bits.ACKDT = (ack)? 0:1;   //Condición de ACK o NACK según necesidades de la aplicación
    //SSPCON2bits.ACKEN=1;            //Genera dicha condición
    //while(SSPCON2bits.ACKEN);       //Espera a que termine tal condición
    return SSPBUF;                    //Retorno de valor
}

#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2)
uint8_t i2c1_readByte(uint8_t ack) {
    uint8_t dato;
    SSP1CON2bits.RCEN=1;             //Habilita recepción de datos en modo maestro 
    while ( !SSP1STATbits.BF );      //Espera hasta recibir byte
    dato = SSP1BUF;                  //Lectura de buffer MSSP
    SSP1CON2bits.ACKDT = (ack)? 0:1;   //Condición de ACK o NACK según necesidades de la aplicación
    SSP1CON2bits.ACKEN=1;            //Genera dicha condición
    while(SSP1CON2bits.ACKEN);       //Espera a que termine tal condición
    return dato;                    //Retorno de valor
}

#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
uint8_t i2c2_readByte(uint8_t ack)
{
    uint8_t dato;
    SSP2CON2bits.RCEN=1;             //Habilita recepción de datos en modo maestro 
    while ( !SSP2STATbits.BF );      //Espera hasta recibir byte
    dato = SSP2BUF;                  //Lectura de buffer MSSP
    SSP2CON2bits.ACKDT = (ack)? 0:1;   //Condición de ACK o NACK según necesidades de la aplicación
    SSP2CON2bits.ACKEN=1;            //Genera dicha condición
    while(SSP2CON2bits.ACKEN);       //Espera a que termine tal condición
    return dato;                    //Retorno de valor
}
#endif

/*
    Función para escribir un dato entero de 2 bytes en el bus i2c por hardware
    Parámetros: 
    dato: Dato de 16 bits a trasmitir mediante el bus i2c por hardware
    Retorno: Vacío (void)
*/
#if defined (I2C_V1) || defined (I2C_V4)
void i2c_writeInt16(uint16_t dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(uint16_t);i++) {
        i2c_writeByte(*p++);
    }
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2) 
void i2c1_writeInt16(uint16_t dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(uint16_t);i++) {
        i2c1_writeByte(*p++);
    }
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_writeInt16(uint16_t dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(uint16_t);i++) {
        i2c2_writeByte(*p++);
    }
}
#endif

/*
    Función para escribir un dato entero de 3 bytes en el bus i2c por hardware
    Parámetros: 
    dato: Dato de 24 bits a trasmitir mediante el bus i2c por hardware
    Retorno: Vacío (void)
*/
#if defined (I2C_V1) || defined (I2C_V4)
void i2c_writeInt24(uint24_t dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(uint24_t);i++) {
        i2c_writeByte(*p++);
    }
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2) 
void i2c1_writeInt24(uint24_t dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(uint24_t);i++) {
        i2c1_writeByte(*p++);
    }
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_writeInt24(uint24_t dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(uint24_t);i++) {
        i2c2_writeByte(*p++);
    }
}
#endif

/*
    Función para escribir un dato entero de 4 bytes en el bus i2c por hardware
    Parámetros: 
    dato: Dato de 32 bits a trasmitir mediante el bus i2c por hardware
    Retorno: Vacío (void)
*/
#if defined (I2C_V1) || defined (I2C_V4)
void i2c_writeInt32(uint32_t dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(uint32_t);i++) {
        i2c_writeByte(*p++);
    }
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2) 
void i2c1_writeInt32(uint32_t dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(uint32_t);i++) {
        i2c1_writeByte(*p++);
    }
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_writeInt32(uint32_t dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(uint32_t);i++) {
        i2c2_writeByte(*p++);
    }
}
#endif

/*
    Función para escribir un dato flotante de 4 bytes (o 3) en el bus i2c por hardware
    Parámetros: 
    dato: Dato flotante de 32(24) bits a trasmitir mediante el bus i2c por hardware
    Retorno: Vacío (void)
*/
#if defined (I2C_V1) || defined (I2C_V4)
void i2c_writeFloat(float dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(float);i++) {
        i2c_writeByte(*p++);
    }
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2) 
void i2c1_writeFloat(float dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(float);i++) {
        i2c1_writeByte(*p++);
    }
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_writeFloat(float dato) {
    uint8_t i;
    uint8_t * p = (uint8_t *)&dato;
    for(i=0;i!=sizeof(float);i++) {
        i2c2_writeByte(*p++);
    }
}
#endif

/*
    Funciones para habilitar/deshabilitar el módulo con fines de ahorro de energía
*/
#if defined (I2C_V1)
void i2c_enable() {
    SSPCON1bits.SSPEN=1;
}
#endif

#if defined (I2C_V4)
void i2c_enable() {
    SSPCONbits.SSPEN=1;
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2)
void i2c1_enable() {
    SSP1CON1bits.SSPEN=1;
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_enable() {
    SSP2CON1bits.SSPEN=1;
}
#endif

#if defined (I2C_V1)
void i2c_disable() {
    SSPCON1bits.SSPEN=0;
}
#endif

#if defined (I2C_V4)
void i2c_disable() {
    SSPCONbits.SSPEN=0;
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2)
void i2c1_disable() {
    SSP1CON1bits.SSPEN=0;
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_disable() {
    SSP2CON1bits.SSPEN=0;
}
#endif

#if defined (I2C_V1) || defined (I2C_V4)
void i2c_setSpeed(uint16_t speed){
    SSPADD = (uint8_t)(division_entera_sin_signo(_XTAL_FREQ,4*(uint32_t)(speed)*1000UL)-1); //Calcula registro SSPxADD con base en baud_rate_requerido en [kHz]
    i2c_speed = speed;
}

uint16_t i2c_getSpeed(void){
    return i2c_speed;
}
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2)
void i2c1_setSpeed(uint16_t speed){
    SSP1ADD = (uint8_t)(division_entera_sin_signo(_XTAL_FREQ,4*(uint32_t)(speed)*1000UL)-1); //Calcula registro SSPxADD con base en baud_rate_requerido en [kHz]
    i2c1_speed = speed;
}

uint16_t i2c1_getSpeed(void){
    return i2c1_speed;
}
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_setSpeed(uint16_t speed){
    SSP2ADD = (uint8_t)(division_entera_sin_signo(_XTAL_FREQ,4*(uint32_t)(speed)*1000UL)-1); //Calcula registro SSPxADD con base en baud_rate_requerido en [kHz]
    i2c2_speed = speed;
}

uint16_t i2c2_getSpeed(void){
    return i2c2_speed;
}
#endif



