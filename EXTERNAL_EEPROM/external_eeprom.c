/*
	Librería para manejo de memoria EEPROM extera mediante microcontroladores PIC de 8 bits

	Autor: Ing. José Roberto Parra Trewartha
	Compilador: XC8

*/

#include "external_eeprom.h"

/****************************************************************************************
*    Nombre de función:  external_eeprom_init                              				*
*    Valor de retorno:   Estado de bus si hay al menos una memoria serial en el bus		*
*    i2c: 0-Error 1-OK                      											*
*    Parámetros:                                                                		*
*    - tipo_memoria:  Tipo de memoria serial presente en el bus. Ej: 24LC512    		*
*    - n_dispositivos: Número de dispositivos en el bus.				        		*
*    Descripción: Inicialización de variables para utilizar memorias EEPROM externas 	*
*    Nota: La configuración del bis i2c debe realizarse para poder utilizar las eeprom  *
*	 seriales.
*/
external_eeprom_status_t external_eeprom_init(external_eeprom_t tipo_memoria, uint8_t n_dispositivos)
{
	bool bus_status = true;
	_deviceType = tipo_memoria;
	_nDevices = n_dispositivos;
	_pageSize = external_eeprom_tamano_pagina[tipo_memoria];    //Determinación de tamaño de página de escritura en bytes
	_deviceCapacity = external_eeprom_capacidad[tipo_memoria];  //Cálculo de capacidad individual de cada dispositivo en bytes
	_totalCapacity = (_nDevices * _deviceCapacity * 1024UL)/8;	//Cálculo de capacidad total del arreglo en bytes
	_maxAddress = _totalCapacity - 1;							//Máxima dirección de memoria
	_nAddrBytes = (_deviceCapacity > kibits_16)? 2:1;			//Cálculo de bytes de direccionamiento

	uint16_t kibits = _deviceCapacity;		//Cantidad de kibibits del arreglo de memorias


	/*
	Lectura de ACK por parte del bus i2c. Si hay al menos una memoria serial (con dirección 0, es decir A0,A1,A2 = 0), 
	el bus responderá con ACK=0 y la función regresará valor OK
	*/
	i2c_start();						//Condición START
	if (i2c_writeByte(EEPROM_EXTERNA_ADDRESS_WRITE) != 0)	//Envío de byte de control básico de escritura y lectura de ACK
		bus_status = false;					//En caso de colisión de datos en bus o NACK, caso que consiste en que no hay memorias seriales EEPROM
	i2c_stop();
	return bus_status;

}


/********************************************************************************
*    Nombre de función:  external_eeprom_writeByte                              *
*    Valor de retorno:   Estado de escritura: 0-Error 1-OK                      *
*    Parámetros:                                                                *
*    - dato:  Dato de 8 bits a escribir en la dirección deseada                 *
*    - address: Dirección de memoria en la que se desea escribir el dato        *
*    Descripción: Escritura de byte en memoria EEPROM externa                   *
********************************************************************************/
external_eeprom_status_t external_eeprom_writeByte(uint8_t dato, uint32_t addr)
{
	if(addr > _maxAddress)
		return EEPROM_EXTERNA_ADDR_ERR;		//Error de direccionamiento, se excede la máxima dirección posible

	int8_t ack = 1;	//Variable de acknowledge, para verifiación de escritura terminada

	//Obtención de bytes que conforman la dirección global
	addr_U = make8(addr,2);
	addr_H = make8(addr,1);
	addr_L = make8(addr,0);

	//Cálculo de byte de control dependiendo del valor addr_U	
	switch(_deviceType)
	{
		case MICROCHIP_24XX1025: 
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
			break;
		case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
			break; 
		default:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
	}

	i2c_start();						//Condición START
	i2c_writeByte(_eepromControlByte);	//Envío de byte de control
	if (_nAddrBytes == 2)
		i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
	i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
	i2c_writeByte(dato);				//Escritura de dato
	i2c_stop();							//Condición de STOP
	while( ack == 1)							//Lectura del bit "ACK" para asegurar escritura correcta
	{
		i2c_start();						//Condición START
		ack = i2c_writeByte(_eepromControlByte);	//Envío de byte de control
        if(ack == 1)
        {
            i2c_stop();
            __delay_us(5);
        }
	}
    i2c_stop();	
    return EEPROM_EXTERNA_OK;
}


/************************************************************************************************
*    Nombre de función:  external_eeprom_readByte                                                   *
*    Valor de retorno:   int8_t con el dato presente en la dirección de memoria EEPROM externa  *
*    Parámetros:                                                                                *
*    - address: Dirección de memoria EEPROM de la cual se desea leer el dato de 8 bits          *
*    Descripción: Lectura de byte en memoria EEPROM externa                                     *
************************************************************************************************/
uint8_t external_eeprom_readByte(uint32_t addr)
{
	//Obtención de bytes que conforman la dirección global
	addr_U = make8(addr,2);
	addr_H = make8(addr,1);
	addr_L = make8(addr,0);

	//Valor de retorno
	uint8_t retval;

	//Cálculo de byte de control dependiendo del valor addr_U	
	switch(_deviceType)
	{
		case MICROCHIP_24XX1025: 
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
			break;
		case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
			break; 
		default:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
	}

	i2c_start();						//Condición START
	i2c_writeByte(_eepromControlByte);	//Envío de byte de control
	if (_nAddrBytes == 2)
		i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
	i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
	i2c_restart();						//Condición RESTART
	i2c_writeByte(_eepromControlByte|EEPROM_EXTERNA_ADDRESS_READ);	//Envío de byte de control en modo lectura (R/!W = 1)
	retval = i2c_readByte(0);
	i2c_stop();

	return retval;
}

/*
* Escritura de variable de 16 bits en memoria eterna
* Parámetros: 
* dato: Dato de 16 bits a escribir en la dirección deseada
* address: Dirección de memoria en la que se desea escribir el dato, recordar que el dato ocupará dos posiciones de memoria
* Retorno: 
* Estado de escritura: 0-Error 1-OK 
*/
external_eeprom_status_t external_eeprom_writeInt16(uint16_t dato, uint32_t addr)
{
	if(addr >= _maxAddress)
		return EEPROM_EXTERNA_ADDR_ERR;		//Error de direccionamiento, se excede la máxima dirección posible

	int8_t ack = 1;	//Variable de acknowledge, para verifiación de escritura terminada
	uint8_t *p = (uint8_t *)&dato;	//Apuntador a variable entera de 16 bits
	//Obtención de bytes que conforman la dirección global
	addr_U = make8(addr,2);
	addr_H = make8(addr,1);
	addr_L = make8(addr,0);

	//Cálculo de byte de control dependiendo del valor addr_U	
	switch(_deviceType)
	{
		case MICROCHIP_24XX1025: 
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
			break;
		case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
			break; 
		default:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
	}

	i2c_start();						//Condición START
	i2c_writeByte(_eepromControlByte);	//Envío de byte de control
	if (_nAddrBytes == 2)
		i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
	i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
	for(uint8_t i=0; i!= sizeof(uint16_t);i++)
		i2c_writeByte(*(p++));			//Escritura de dato
	i2c_stop();							//Condición de STOP
	while( ack == 1)							//Lectura del bit "ACK" para asegurar escritura correcta
	{
		i2c_start();						//Condición START
		ack = i2c_writeByte(_eepromControlByte);	//Envío de byte de control
        if(ack == 1)
        {
            i2c_stop();
            __delay_us(5);
        }
	}
    i2c_stop();	
    return EEPROM_EXTERNA_OK;   //Sin errores
}

/*
* Lectura de variable de 16 bits en memoria externa
* Parámetros: 
* address: Dirección de memoria EEPROM de la cual se desea leer el dato de 16 bits. Recordar que se lee dato en esa posición y la siguiente.
* Retorno: 
* int16_t con el dato presente en la dirección de memoria EEPROM externa
*/
uint16_t external_eeprom_readInt16(uint32_t addr)
{
	//Obtención de bytes que conforman la dirección global
	addr_U = make8(addr,2);
	addr_H = make8(addr,1);
	addr_L = make8(addr,0);

	//Valor de retorno
	uint16_t retval;
    uint8_t *p = (uint8_t*)&retval; //Apuntador a valor de retorno

	//Cálculo de byte de control dependiendo del valor addr_U	
	switch(_deviceType)
	{
		case MICROCHIP_24XX1025: 
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
			break;
		case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
			break; 
		default:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
	}

	i2c_start();						//Condición START
	i2c_writeByte(_eepromControlByte);	//Envío de byte de control
	if (_nAddrBytes == 2)
		i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
	i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
	i2c_restart();						//Condición RESTART
	i2c_writeByte(_eepromControlByte|EEPROM_EXTERNA_ADDRESS_READ);	//Envío de byte de control en modo lectura (R/!W = 1)
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        if(i != (sizeof(uint16_t)-1))
            *(p++) = i2c_readByte(1);
        else
            *(p++) = i2c_readByte(0);
    }
	i2c_stop();

	return retval;
}


/*
* Escritura de variable de 24 bits en memoria eterna
* Parámetros: 
* dato: Dato de 24 bits a escribir en la dirección deseada
* address: Dirección de memoria en la que se desea escribir el dato, recordar que el dato ocupará tres posiciones de memoria
* Retorno: 
* Estado de escritura: 0-Error 1-OK 
*/
external_eeprom_status_t external_eeprom_writeInt24(uint24_t dato, uint32_t addr)
{
	if(addr+1 >= _maxAddress)
		return EEPROM_EXTERNA_ADDR_ERR;		//Error de direccionamiento, se excede la máxima dirección posible

	int8_t ack = 1;	//Variable de acknowledge, para verifiación de escritura terminada
	uint8_t *p = (uint8_t *)&dato;	//Apuntador a variable entera de 16 bits
	//Obtención de bytes que conforman la dirección global
	addr_U = make8(addr,2);
	addr_H = make8(addr,1);
	addr_L = make8(addr,0);

	//Cálculo de byte de control dependiendo del valor addr_U	
	switch(_deviceType)
	{
		case MICROCHIP_24XX1025: 
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
			break;
		case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
			break; 
		default:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
	}

	i2c_start();						//Condición START
	i2c_writeByte(_eepromControlByte);	//Envío de byte de control
	if (_nAddrBytes == 2)
		i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
	i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
	for(uint8_t i=0; i!= sizeof(uint24_t);i++)
		i2c_writeByte(*(p++));			//Escritura de dato
	i2c_stop();							//Condición de STOP
	while( ack == 1)							//Lectura del bit "ACK" para asegurar escritura correcta
	{
		i2c_start();						//Condición START
		ack = i2c_writeByte(_eepromControlByte);	//Envío de byte de control
        if(ack == 1)
        {
            i2c_stop();
            __delay_us(5);
        }
	}
    i2c_stop();	
    return EEPROM_EXTERNA_OK;   //Sin errores
}

/*
* Lectura de variable de 24 bits en memoria externa
* Parámetros: 
* address: Dirección de memoria EEPROM de la cual se desea leer el dato de 24 bits. 
* Retorno: 
* int24_t con el dato presente en la dirección de memoria EEPROM externa
*/
uint24_t external_eeprom_readInt24(uint32_t addr)
{
	//Obtención de bytes que conforman la dirección global
	addr_U = make8(addr,2);
	addr_H = make8(addr,1);
	addr_L = make8(addr,0);

	//Valor de retorno
	uint24_t retval;
    uint8_t *p = (uint8_t*)&retval; //Apuntador a valor de retorno

	//Cálculo de byte de control dependiendo del valor addr_U	
	switch(_deviceType)
	{
		case MICROCHIP_24XX1025: 
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
			break;
		case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
			break; 
		default:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
	}

	i2c_start();						//Condición START
	i2c_writeByte(_eepromControlByte);	//Envío de byte de control
	if (_nAddrBytes == 2)
		i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
	i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
	i2c_restart();						//Condición RESTART
	i2c_writeByte(_eepromControlByte|EEPROM_EXTERNA_ADDRESS_READ);	//Envío de byte de control en modo lectura (R/!W = 1)
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        if(i != (sizeof(uint24_t)-1))
            *(p++) = i2c_readByte(1);
        else
            *(p++) = i2c_readByte(0);
    }
	i2c_stop();

	return retval;
}

/*
* Escritura de variable de 32 bits en memoria eterna
* Parámetros: 
* dato: Dato de 32 bits a escribir en la dirección deseada
* address: Dirección de memoria en la que se desea escribir el dato, recordar que el dato ocupará cuatro posiciones de memoria
* Retorno: 
* Estado de escritura: 0-Error 1-OK 
*/
external_eeprom_status_t external_eeprom_writeInt32(uint32_t dato, uint32_t addr)
{
	if(addr+2 >= _maxAddress)
		return EEPROM_EXTERNA_ADDR_ERR;		//Error de direccionamiento, se excede la máxima dirección posible

	int8_t ack = 1;	//Variable de acknowledge, para verifiación de escritura terminada
	uint8_t *p = (uint8_t *)&dato;	//Apuntador a variable entera de 16 bits
	//Obtención de bytes que conforman la dirección global
	addr_U = make8(addr,2);
	addr_H = make8(addr,1);
	addr_L = make8(addr,0);

	//Cálculo de byte de control dependiendo del valor addr_U	
	switch(_deviceType)
	{
		case MICROCHIP_24XX1025: 
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
			break;
		case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
			break; 
		default:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
	}

	i2c_start();						//Condición START
	i2c_writeByte(_eepromControlByte);	//Envío de byte de control
	if (_nAddrBytes == 2)
		i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
	i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
	for(uint8_t i=0; i!= sizeof(uint32_t);i++)
		i2c_writeByte(*(p++));			//Escritura de dato
	i2c_stop();							//Condición de STOP
	while( ack == 1)							//Lectura del bit "ACK" para asegurar escritura correcta
	{
		i2c_start();						//Condición START
		ack = i2c_writeByte(_eepromControlByte);	//Envío de byte de control
        if(ack == 1)
        {
            i2c_stop();
            __delay_us(5);
        }
	}
    i2c_stop();	
    return EEPROM_EXTERNA_OK;   //Sin errores
}

/*
* Lectura de variable de 32 bits en memoria externa
* Parámetros: 
* address: Dirección de memoria EEPROM de la cual se desea leer el dato de 32 bits. 
* Retorno: 
* int32_t con el dato presente en la dirección de memoria EEPROM externa
*/
uint32_t external_eeprom_readInt32(uint32_t addr)
{
	//Obtención de bytes que conforman la dirección global
	addr_U = make8(addr,2);
	addr_H = make8(addr,1);
	addr_L = make8(addr,0);

	//Valor de retorno
	uint32_t retval;
    uint8_t *p = (uint8_t*)&retval; //Apuntador a valor de retorno

	//Cálculo de byte de control dependiendo del valor addr_U	
	switch(_deviceType)
	{
		case MICROCHIP_24XX1025: 
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
			break;
		case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
			break; 
		default:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
	}

	i2c_start();						//Condición START
	i2c_writeByte(_eepromControlByte);	//Envío de byte de control
	if (_nAddrBytes == 2)
		i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
	i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
	i2c_restart();						//Condición RESTART
	i2c_writeByte(_eepromControlByte|EEPROM_EXTERNA_ADDRESS_READ);	//Envío de byte de control en modo lectura (R/!W = 1)
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        if(i != (sizeof(uint32_t)-1))
            *(p++) = i2c_readByte(1);
        else
            *(p++) = i2c_readByte(0);
    }
	i2c_stop();

	return retval;
}

/*
* Escritura de variable flotante en memoria eterna
* Parámetros: 
* dato: Dato de 32 bits a escribir en la dirección deseada
* address: Dirección de memoria en la que se desea escribir el dato, recordar que el dato ocupará tres o cuatro posiciones de memoria
* Retorno: 
* Estado de escritura: 0-Error 1-OK 
*/
external_eeprom_status_t external_eeprom_writeFloat(float dato, uint32_t addr)
{
	if(addr+sizeof(float)-2 >= _maxAddress)
		return EEPROM_EXTERNA_ADDR_ERR;		//Error de direccionamiento, se excede la máxima dirección posible

	int8_t ack = 1;	//Variable de acknowledge, para verifiación de escritura terminada
	uint8_t *p = (uint8_t *)&dato;	//Apuntador a variable entera de 16 bits
	//Obtención de bytes que conforman la dirección global
	addr_U = make8(addr,2);
	addr_H = make8(addr,1);
	addr_L = make8(addr,0);

	//Cálculo de byte de control dependiendo del valor addr_U	
	switch(_deviceType)
	{
		case MICROCHIP_24XX1025: 
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
			break;
		case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
			break; 
		default:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
	}

	i2c_start();						//Condición START
	i2c_writeByte(_eepromControlByte);	//Envío de byte de control
	if (_nAddrBytes == 2)
		i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
	i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
	for(uint8_t i=0; i!= sizeof(float);i++)
		i2c_writeByte(*(p++));			//Escritura de dato
	i2c_stop();							//Condición de STOP
	while( ack == 1)							//Lectura del bit "ACK" para asegurar escritura correcta
	{
		i2c_start();						//Condición START
		ack = i2c_writeByte(_eepromControlByte);	//Envío de byte de control
        if(ack == 1)
        {
            i2c_stop();
            __delay_us(5);
        }
	}
    i2c_stop();	
    return EEPROM_EXTERNA_OK;   //Sin errores
}

/*
* Lectura de variable flotante en memoria externa
* Parámetros: 
* address: Dirección de memoria EEPROM de la cual se desea leer el dato flotante. 
* Retorno: 
* float con el dato presente en la dirección de memoria EEPROM externa
*/
float external_eeprom_readFloat(uint32_t addr)
{
	//Obtención de bytes que conforman la dirección global
	addr_U = make8(addr,2);
	addr_H = make8(addr,1);
	addr_L = make8(addr,0);

	//Valor de retorno
	float retval;
    uint8_t *p = (uint8_t*)&retval; //Apuntador a valor de retorno

	//Cálculo de byte de control dependiendo del valor addr_U	
	switch(_deviceType)
	{
		case MICROCHIP_24XX1025: 
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
			break;
		case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
			break; 
		default:
			_eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
	}

	i2c_start();						//Condición START
	i2c_writeByte(_eepromControlByte);	//Envío de byte de control
	if (_nAddrBytes == 2)
		i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
	i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
	i2c_restart();						//Condición RESTART
	i2c_writeByte(_eepromControlByte|EEPROM_EXTERNA_ADDRESS_READ);	//Envío de byte de control en modo lectura (R/!W = 1)
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        if(i != (sizeof(float)-1))
            *(p++) = i2c_readByte(1);
        else
            *(p++) = i2c_readByte(0);
    }
	i2c_stop();

	return retval;
}

/************************************************************************************
*    Nombre de función:  external_eeprom_writeBuffer                                *
*    Valor de retorno:   Estado de escritura: 0-Error 1-OK                          *
*    Parámetros:                                                                    *
*    - buffer:  Apuntador a buffer de datos a escribir en la memoria                *
*    - addr: Dirección de memoria a partir de la cual se desea escribir el buffer   *
*    - len: Cantidad de bytes a escribir en la memoria                              *
*    Descripción: Escritura de buffer de ´len´bytes en memoria EEPROM externa       *
************************************************************************************/
external_eeprom_status_t external_eeprom_writeBuffer(uint8_t *buffer, uint32_t addr, uint16_t len)
{
    if( addr+len-1 > _maxAddress)
		return EEPROM_EXTERNA_ADDR_ERR;		//Error de direccionamiento, se excede la máxima dirección posible
    
    int8_t ack = 1;	//Variable de acknowledge, para verifiación de escritura terminada
    uint16_t paginas_a_escribir=0;   //Número total de páginas a escribir
    uint32_t addr_aux = addr;       //Variable auxiliar de dirección
    //Cálculo de número de páginas
    while( len >= _pageSize)
    {
        len -= _pageSize;
        paginas_a_escribir++;
    }
    
    while(paginas_a_escribir != 0)
    {
        //Obtención de bytes que conforman la dirección global
        addr_U = make8(addr_aux,2);
        addr_H = make8(addr_aux,1);
        addr_L = make8(addr_aux,0);

        //Cálculo de byte de control dependiendo del valor addr_U	
        switch(_deviceType)
        {
            case MICROCHIP_24XX1025: 
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
                break;
            case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
                break; 
            default:
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
        }
        i2c_start();						//Condición START
        i2c_writeByte(_eepromControlByte);	//Envío de byte de control
        if (_nAddrBytes == 2)
            i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
        i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
        for(uint16_t i = 0; i != _pageSize; i++)
        {
            i2c_writeByte(*(buffer++));			//Escritura secuencial de 'len' datos
        }
        i2c_stop();							//Condición de STOP
        while( ack == 1)							//Lectura del bit "ACK" para asegurar escritura correcta
        {
            i2c_start();						//Condición START
            ack = i2c_writeByte(_eepromControlByte);	//Envío de byte de control
            if(ack == 1)
            {
                i2c_stop();
                __delay_us(5);
            }
        }
        i2c_stop();	
        paginas_a_escribir--;
        addr_aux += _pageSize;
        ack = 1;                //Bandera ack se devuelve a su estado inicial
    }

    //Envío de datos restantes
    if(len!=0)
    {
        //Obtención de bytes que conforman la dirección global
        addr_U = make8(addr_aux,2);
        addr_H = make8(addr_aux,1);
        addr_L = make8(addr_aux,0);

        //Cálculo de byte de control dependiendo del valor addr_U	
        switch(_deviceType)
        {
            case MICROCHIP_24XX1025: 
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
                break;
            case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
                break; 
            default:
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
        }
        i2c_start();						//Condición START
        i2c_writeByte(_eepromControlByte);	//Envío de byte de control
        if (_nAddrBytes == 2)
            i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
        i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
        for(uint16_t i=0; i!= len;i++)
        {
            i2c_writeByte(*(buffer++));			//Escritura secuencial de 'len' datos
        }
        i2c_stop();							//Condición de STOP
        ack = 1;                //Bandera ack se devuelve a su estado inicial
        while( ack == 1)							//Lectura del bit "ACK" para asegurar escritura correcta
        {
            i2c_start();						//Condición START
            ack = i2c_writeByte(_eepromControlByte);	//Envío de byte de control
            if(ack == 1)
            {
                i2c_stop();
                __delay_us(5);
            }
        }
        i2c_stop();	
    }
    return EEPROM_EXTERNA_OK;   //Sin errores
}

/********************************************************************************
*    Nombre de función:  external_eeprom_readBuffer                             *
*    Valor de retorno:   Estado de lectura: 0-Error 1-OK                        *
*    Parámetros:                                                                *
*    - buffer:  Apuntador a buffer de datos a leer desde la memoria             *
*    - addr: Dirección de memoria a partir de la cual se desea leer el buffer   *
*    - len: Cantidad de bytes a leer desde la memoria                           *
*    Descripción: Lectura de buffer de ´len´bytes desde memoria EEPROM externa  *
********************************************************************************/
external_eeprom_status_t external_eeprom_readBuffer(uint8_t *buffer, uint32_t addr, uint16_t len)
{
    if( addr+len-1 > _maxAddress)
		return EEPROM_EXTERNA_ADDR_ERR;		//Error de direccionamiento, se excede la máxima dirección posible
    //Obtención de bytes que conforman la dirección global
    addr_U = make8(addr,2);
    addr_H = make8(addr,1);
    addr_L = make8(addr,0);

    //Cálculo de byte de control dependiendo del valor addr_U	
    switch(_deviceType)
    {
        case MICROCHIP_24XX1025: 
            _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
            break;
        case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
            _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
            break; 
        default:
            _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
    }
    i2c_start();						//Condición START
    i2c_writeByte(_eepromControlByte);	//Envío de byte de control
    if (_nAddrBytes == 2)
        i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
    i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
    i2c_restart();						//Condición RESTART
	i2c_writeByte(_eepromControlByte|EEPROM_EXTERNA_ADDRESS_READ);	//Envío de byte de control en modo lectura (R/!W = 1)
    for(uint16_t i=0;i!=len;i++)
    {
        if(i != (len-1))
            *(buffer++) = i2c_readByte(1);
        else
            *(buffer++) = i2c_readByte(0);
    }
	i2c_stop();
    return EEPROM_EXTERNA_OK;   //Sin errores
}

/************************************************************************************
*    Nombre de función:  external_eeprom_write                                      *
*    Valor de retorno:   Estado de escritura: 0-Error 1-OK                          *
*    Parámetros:                                                                    *
*    - datos:  Apuntador  datos a escribir en la memoria (incluso estructuras)      *
*    - addr: Dirección de memoria a partir de la cual se desea escribir el buffer   *
*    - len: Cantidad de bytes a escribir en la memoria                              *
*    Descripción: Escritura de ´len´bytes en memoria EEPROM externa                 *
************************************************************************************/
external_eeprom_status_t external_eeprom_write(void *datos, uint32_t addr, uint16_t len)
{
    if( addr+len-1 > _maxAddress)
		return EEPROM_EXTERNA_ADDR_ERR;		//Error de direccionamiento, se excede la máxima dirección posible
    
    uint8_t *p = (uint8_t*)datos;   //Apuntador a datos
    int8_t ack = 1;	//Variable de acknowledge, para verifiación de escritura terminada
    uint16_t paginas_a_escribir=0;   //Número total de páginas a escribir
    uint32_t addr_aux = addr;       //Variable auxiliar de dirección
    //Cálculo de número de páginas
    while( len >= _pageSize)
    {
        len -= _pageSize;
        paginas_a_escribir++;
    }
    
    while(paginas_a_escribir != 0)
    {
        //Obtención de bytes que conforman la dirección global
        addr_U = make8(addr_aux,2);
        addr_H = make8(addr_aux,1);
        addr_L = make8(addr_aux,0);

        //Cálculo de byte de control dependiendo del valor addr_U	
        switch(_deviceType)
        {
            case MICROCHIP_24XX1025: 
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
                break;
            case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
                break; 
            default:
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
        }
        i2c_start();						//Condición START
        i2c_writeByte(_eepromControlByte);	//Envío de byte de control
        if (_nAddrBytes == 2)
            i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
        i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
        for(uint16_t i = 0; i != _pageSize; i++)
        {
            i2c_writeByte(*(p++));			//Escritura secuencial de 'len' datos
        }
        i2c_stop();							//Condición de STOP
        while( ack == 1)							//Lectura del bit "ACK" para asegurar escritura correcta
        {
            i2c_start();						//Condición START
            ack = i2c_writeByte(_eepromControlByte);	//Envío de byte de control
            if(ack == 1)
            {
                i2c_stop();
                __delay_us(5);
            }
        }
        i2c_stop();	
        paginas_a_escribir--;
        addr_aux += _pageSize;
        ack = 1;                //Bandera ack se devuelve a su estado inicial
    }

    //Envío de datos restantes
    if(len!=0)
    {
        //Obtención de bytes que conforman la dirección global
        addr_U = make8(addr_aux,2);
        addr_H = make8(addr_aux,1);
        addr_L = make8(addr_aux,0);

        //Cálculo de byte de control dependiendo del valor addr_U	
        switch(_deviceType)
        {
            case MICROCHIP_24XX1025: 
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
                break;
            case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
                break; 
            default:
                _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
        }
        i2c_start();						//Condición START
        i2c_writeByte(_eepromControlByte);	//Envío de byte de control
        if (_nAddrBytes == 2)
            i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
        i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
        for(uint16_t i=0; i!= len;i++)
        {
            i2c_writeByte(*(p++));			//Escritura secuencial de 'len' datos
        }
        i2c_stop();							//Condición de STOP
        ack = 1;                //Bandera ack se devuelve a su estado inicial
        while( ack == 1)							//Lectura del bit "ACK" para asegurar escritura correcta
        {
            i2c_start();						//Condición START
            ack = i2c_writeByte(_eepromControlByte);	//Envío de byte de control
            if(ack == 1)
            {
                i2c_stop();
                __delay_us(5);
            }
        }
        i2c_stop();	
    }
    return EEPROM_EXTERNA_OK;   //Sin errores
}

/********************************************************************************
*    Nombre de función:  external_eeprom_read                                   *
*    Valor de retorno:   Estado de lectura: 0-Error 1-OK                        *
*    Parámetros:                                                                *
*    - buffer:  Apuntador a datos a leer desde la memoria (incluso estructuras) *
*    - addr: Dirección de memoria a partir de la cual se desea leer             *
*    - len: Cantidad de bytes a leer desde la memoria                           *
*    Descripción: Lectura de 'len' bytes desde memoria EEPROM externa           *
********************************************************************************/
external_eeprom_status_t external_eeprom_read(void *datos, uint32_t addr, uint16_t len)
{
    if( addr+len-1 > _maxAddress)
		return EEPROM_EXTERNA_ADDR_ERR;		//Error de direccionamiento, se excede la máxima dirección posible
    uint8_t *p = (uint8_t*)datos;   //Apuntador a salida de datos
    //Obtención de bytes que conforman la dirección global
    addr_U = make8(addr,2);
    addr_H = make8(addr,1);
    addr_L = make8(addr,0);

    //Cálculo de byte de control dependiendo del valor addr_U	
    switch(_deviceType)
    {
        case MICROCHIP_24XX1025: 
            _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U & 0x06) | ((addr_U & 0x01) << 3);	//Byte de control en modo escritura (R/!W = 0)
            break;
        case MICROCHIP_24XX1026: case ATMEL_AT24CM02:
            _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE | (addr_U << 1);
            break; 
        default:
            _eepromControlByte = EEPROM_EXTERNA_ADDRESS_WRITE;
    }
    i2c_start();						//Condición START
    i2c_writeByte(_eepromControlByte);	//Envío de byte de control
    if (_nAddrBytes == 2)
        i2c_writeByte(addr_H);			//Envío de byte alto de dirección		
    i2c_writeByte(addr_L);				//Envío de byte bajo de dirección
    i2c_restart();						//Condición RESTART
	i2c_writeByte(_eepromControlByte|EEPROM_EXTERNA_ADDRESS_READ);	//Envío de byte de control en modo lectura (R/!W = 1)
    for(uint16_t i=0;i!=len;i++)
    {
        if(i != (len-1))
            *(p++) = i2c_readByte(1);
        else
            *(p++) = i2c_readByte(0);
    }
	i2c_stop();
    return EEPROM_EXTERNA_OK;   //Sin errores
    
}

/********************************************************************************
*    Nombre de función:  external_eeprom_getDeviceCapacity                      *
*    Valor de retorno:   uint32_t                                               *     
*    Parámetros: Ninguno                                                        *                           *
*    Descripción: Obtención de capacidad de memoria de dispositivo individual.  *
*    Utilizada con fines de depuración.                                         *
********************************************************************************/
uint32_t external_eeprom_getDeviceCapacity(){
    return _deviceCapacity;
}

/********************************************************************************
*    Nombre de función:  external_eeprom_getTotalCapacity                       *
*    Valor de retorno:   uint32_t                                               *     
*    Parámetros: Ninguno                                                        *                           *
*    Descripción: Obtención de capacidad de memoria de arreglo de memorias.     *
*    Utilizada con fines de depuración.                                         *
********************************************************************************/
uint32_t external_eeprom_getTotalCapacity()
{
    return _totalCapacity;
}

/********************************************************************************
*    Nombre de función:  external_eeprom_getMaxAddress                          *
*    Valor de retorno:   uint32_t                                               *     
*    Parámetros: Ninguno                                                        *                           *
*    Descripción: Obtención de dirección máxima en arreglo de memorias.         *
*    Utilizada con fines de depuración.                                         *
********************************************************************************/
uint32_t external_eeprom_getMaxAddress()
{
    return _maxAddress;
}

/********************************************************************************
*    Nombre de función:  external_eeprom_getPageSize                            *
*    Valor de retorno:   uint32_t                                               *     
*    Parámetros: Ninguno                                                        *                           *
*    Descripción: Obtención de tamaño de página en bytes, dependiendo del tipo  *
*    de dispositivo. Utilizada con fines de depuración.                         *               *
********************************************************************************/
uint16_t external_eeprom_getPageSize()
{
    return _pageSize;
}

/********************************************************************************
*    Nombre de función:  external_eeprom_getDeviceName                            *
*    Valor de retorno:   Apuntador a cadena de caracteres                       *                                               *     
*    Parámetros: Ninguno                                                        *                           *
*    Descripción: Obtención de nombre de dispositivo.                           *
*    Utilizada con fines de depuración.                         *               *
********************************************************************************/
const char* external_eeprom_getDeviceName()
{
    switch(_deviceType)
    {
        case MICROCHIP_24XX16B: return "MICROCHIP_24XX16B";
        case MICROCHIP_24XX32A: return "MICROCHIP_24XX32A";
        case MICROCHIP_24XX64:  return "MICROCHIP_24XX64";
        case MICROCHIP_24XX128: return "MICROCHIP_24XX128";
        case MICROCHIP_24XX256: return "MICROCHIP_24XX256";
        case MICROCHIP_24XX512: return "MICROCHIP_24XX512";
        case MICROCHIP_24XX1025:    return "MICROCHIP_24XX1025";
        case MICROCHIP_24XX1026:    return "MICROCHIP_24XX1026";
        case ATMEL_AT24CM02:    return "ATMEL_AT24CM02";
        default: break;
    }
}