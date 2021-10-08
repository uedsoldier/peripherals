/*
    Librería para manejo de memoria EEPROM interna de microcontroladores PIC de 8 bits
    Autor: Ing. José Roberto Parra Trewartha
    Compilador: XC8
*/
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "funciones_auxiliares.h"
#include "pconfig.h"
#include "eeprom_interna.h"

#pragma warning disable 373
#pragma warning disable 520

/********************************************************************************
*    Nombre de función:  internal_eeprom_writeByte                                  *
*    Valor de retorno:   ninguno                                                *
*    Parámetros:                                                                *
*    - dato:  Dato de 8 bits a escribir en la dirección deseada                 *
*    - address: Dirección de memoria en la que se desea escribir el dato        *
*    Descripción: Escritura de byte en memoria EEPROM interna                   *
********************************************************************************/

void internal_eeprom_writeByte(uint8_t dato,tipo_address_eeprom_interna address)
{
    bool VALOR_GIE = 0;
    VALOR_GIE = INTCONbits.GIE; //Toma valor del bit de habilitación global de interrupciones
    INTCONbits.GIE=0;   //Deshabilita momentáneamente interrupciones
    EECON1bits.EEPGD=0; //Acceso a la memoria EEPROM de datos
    
    //EECON1bits.CFGS=0;  //Acceso a la memoria EEPROM. Agunos PIC no cuentan con este bit implementado
    clrbit(EECON1,6); //Alternativa a lo de arriba por la razón mencionada
    EECON1bits.WREN=1;  //Habilita escritura de datos
#if defined (EEP_V1) //EEPROM de 128 bytes
    EEADR=address & 0x7F);
#elif defined (EEP_V2) //EEPROM de 256 bytes
    EEADR=(address & 0xFF);
#elif defined (EEP_V3) //EEPROM de 1024 bytes
    EEADRH=( MAKE8(address,1) & 0x03 ); //Apunta a dirección alta de EEPROM interna (toma los dos bits más significativos)
    EEADR=MAKE8(address,0);  //Apunta dirección baja de EEPROM interna (toma los 8 bits menos significativos)
#endif
    EEDATA=dato;     //Carga dato en registro EEDATL
    EECON2=0x55;
    EECON2=0xAA;        //Secuencia requerida por el fabricante para escritura de datos
    EECON1bits.WR=1;    //Comienza escritura
    while(!EEIF) //Espera a que termine escritura
    {}
    EECON1bits.WREN=0;  //Deshabilita escritura de la EEPROM interna
    EEIF=0; //Limpia bandera de escritura terminada
    INTCONbits.GIE=VALOR_GIE;   //Rehabilita interrupciones como último paso si lo estaban anteriormente
}

/************************************************************************************************
*    Nombre de función:  internal_eeprom_readByte                                                   *
*    Valor de retorno:   int8_t con el dato presente en la dirección de memoria EEPROM interna  *
*    Parámetros:                                                                                *
*    - address: Dirección de memoria EEPROM de la cual se desea leer el dato de 8 bits          *
*    Descripción: Lectura de byte en memoria EEPROM interna                                     *
************************************************************************************************/

uint8_t internal_eeprom_readByte(tipo_address_eeprom_interna address)
{
#if defined (EEP_V3) //EEPROM de 1024 bytes
    EEADRH = ( MAKE8(address,1) & 0x03 ); //Apunta a dirección alta de EEPROM interna (toma los dos bits más significativos)
    EEADR = MAKE8(address,0);  //Apunta dirección baja de EEPROM interna (toma los 8 bits menos significativos)
#elif defined (EEP_V2) //EEPROM de 256 bytes
    EEADR=address;
#elif defined (EEP_V1) //EEPROM de 128 bytes
    EEADR=address;
#endif

    EECON1bits.EEPGD=0; //Acceso a la memoria EEPROM de datos
    //EECON1bits.CFGS=0;  //Acceso a la memoria EEPROM. Agunos PIC no cuentan con este bit implementado
    clrbit(EECON1,6); //Alternativa a lo de arriba por la razón mencionada
    EECON1bits.RD=1;  //Operación de lectura, que se realiza en un solo ciclo
    NOP();
    NOP();
    return EEDATA;           //Regresa dato leído
}

/*
    Funciones de lectura/escritura de enteros de 16 bits
*/

/*
* Escritura de variable de 16 bits en memoria interna
* Parámetros: 
* dato: Dato de 16 bits a escribir en la dirección deseada
* address: Dirección de memoria en la que se desea escribir el dato, recordar que el dato ocupará dos posiciones de memoria
* Retorno: 
* Vacío (void)
*/

void internal_eeprom_writeInt16(uint16_t dato,tipo_address_eeprom_interna address)
{
    tipo_address_eeprom_interna i;
    for(i=0;i!=sizeof(int16_t);i++)
    {
        internal_eeprom_writeByte(*((uint8_t *)&dato+i),address+i);
    }
}

/*
* Lectura de variable de 16 bits en memoria interna
* Parámetros: 
* address: Dirección de memoria EEPROM de la cual se desea leer el dato de 16 bits. Recordar que se lee dato en esa posición y la siguiente.
* Retorno: 
* int16_t con el dato presente en la dirección de memoria EEPROM interna
*/
uint16_t internal_eeprom_readInt16(tipo_address_eeprom_interna address)
{
    tipo_address_eeprom_interna i;
    int16_t retval;
    for(i=0;i!=sizeof(int16_t);i++)
    {
        *((uint8_t *)&retval+i)=internal_eeprom_readByte(address+i);
    }
    return retval;
}

/*
    Funciones de lectura/escritura de enteros de 24 bits
*/

/*
* Escritura de variable de 24 bits en memoria interna
* Parámetros: 
* dato: Dato de 24 bits a escribir en la dirección deseada
* address: Dirección de memoria en la que se desea escribir el dato, recordar que el dato ocupará tres posiciones de memoria
* Retorno: 
* Vacío (void)
*/

void internal_eeprom_writeInt24(uint24_t dato,tipo_address_eeprom_interna address)
{
    tipo_address_eeprom_interna i;
    for(i=0;i!=sizeof(uint24_t);i++)
    {
        internal_eeprom_writeByte(*((uint8_t *)&dato+i),address+i);
    }
}

/*
* Lectura de variable de 24 bits en memoria interna
* Parámetros: 
* address: Dirección de memoria EEPROM de la cual se desea leer el dato de 24 bits.
* Retorno: 
* int24_t con el dato presente en la dirección de memoria EEPROM interna
*/
uint24_t internal_eeprom_readInt24(tipo_address_eeprom_interna address)
{
    tipo_address_eeprom_interna i;
    uint24_t retval;
    for(i=0;i!=sizeof(uint24_t);i++)
    {
        *((uint8_t *)&retval+i)=internal_eeprom_readByte(address+i);
    }
    return retval;
}

/*
    Funciones de lectura/escritura de enteros de 32 bits
*/

/*
* Escritura de variable de 32 bits en memoria interna
* Parámetros: 
* dato: Dato de 32 bits a escribir en la dirección deseada
* address: Dirección de memoria en la que se desea escribir el dato, recordar que el dato ocupará cuatro posiciones de memoria
* Retorno: 
* Vacío (void)
*/
void internal_eeprom_writeInt32(uint32_t dato,tipo_address_eeprom_interna address)
{
    tipo_address_eeprom_interna i;
    for(i=0;i!=sizeof(uint32_t);i++)
    {
        internal_eeprom_writeByte(*((uint8_t *)&dato+i),address+i);
    }
}

/*
* Lectura de variable de 32 bits en memoria interna
* Parámetros: 
* address: Dirección de memoria EEPROM de la cual se desea leer el dato de 16 bits. Recordar que se lee dato en esa posición y las 3 siguientes.
* Retorno: 
* int32_t con el dato presente en la dirección de memoria EEPROM interna
*/
uint32_t internal_eeprom_readInt32(tipo_address_eeprom_interna address)
{
    tipo_address_eeprom_interna i;
    uint32_t retval;
    for(i=0;i!=sizeof(uint32_t);i++)
    {
        *((uint8_t *)&retval+i)=internal_eeprom_readByte(address+i);
    }
    return retval;
}


/*
    Funciones de lectura/escritura de datos flotantes de 32 bits
*/

void internal_eeprom_writeFloat(float dato,tipo_address_eeprom_interna address)
{
    tipo_address_eeprom_interna i;
     for(i=0;i!=sizeof(float);i++)
    {
         internal_eeprom_writeByte(*((int8_t *)&dato+i),address+i);
    }
}
float internal_eeprom_readFloat(tipo_address_eeprom_interna address)
{
    tipo_address_eeprom_interna i;
    float retval;
    for(i=0;i!=sizeof(float);i++)
    {
        *((int8_t *)&retval+i)=internal_eeprom_readByte(address+i);
    }
    return retval;
}

/*
    Funciones de lectura/escritura de bits
*/


/*
* Escritura de un bit solamente en memoria interna
* Parámetros: 
* dato: Bit a escribir en la dirección deseada (true o false)
* posicion: Posición de memoria dentro del registro en el que se almacenará el bit. (0..7)
* address: Dirección de memoria en la que se desea escribir el bit.
* Retorno: 
* Vacío (void)
*/
void internal_eeprom_writeBit(bool dato, tipo_address_eeprom_interna address, uint8_t posicion )
{
    int8_t byte_auxiliar;
    byte_auxiliar = internal_eeprom_readByte(address);  //Lectura de byte en la posición de memoria
    setbit(byte_auxiliar,posicion);                 //Pone en alto el bit en ceustión, pero en la variable auxiliar
    internal_eeprom_writeByte(byte_auxiliar,address);   //Escribe la variable modificada en el registro que debe quedar

}

/*
* Lectura de bit en memoria interna
* Parámetros: 
* address: Dirección de memoria EEPROM de la cual se desea leer el bit
* posicion: Posición de memoria dentro del registro de la que se leerá su estado (0..7)
* Retorno: 
* bool con el estado del bit presente en la dirección de memoria EEPROM interna, en la posición indicada
*/
bool internal_eeprom_readBit(tipo_address_eeprom_interna address, uint8_t posicion)
{
    int8_t byte_auxiliar;
    byte_auxiliar = internal_eeprom_readByte(address);  //Lectura inicial de dato
    return testbit(byte_auxiliar,posicion);         //Devuelve estado del bit ubicado en la posición deseada

}
