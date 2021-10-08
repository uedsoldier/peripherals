/*
	Librería para manejo de memoria EEPROM extera mediante microcontroladores PIC de 8 bits

	Autor: Ing. José Roberto Parra Trewartha
	Compilador: XC8

*/
#ifndef EXTERNAL_EEPROM_H
#define	EXTERNAL_EEPROM_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../utils/utils.h"
#include "../../pconfig.h"
#include "../../peripherals/I2C/i2c.h"
#include "../../emulated_protocols/I2C_SW/i2c_sw.h"

typedef enum EXTERNAL_EEPROM_METODO_COM_t
{
	EXTERNAL_EEPROM_I2C = 0,
	EXTERNAL_EEPROM_SPI = 1
	
} EXTERNAL_EEPROM_METODO_COM_t;

/*
    Macros para definición de módulo de comunicación con el que se utilizarán las memorias EEPROM. 
    Cambiar según necesidades de la aplicación
*/
#define EXTERNAL_EEPROM_METODO_COM		EXTERNAL_EEPROM_I2C		//EXTERNAL_EEPROM_I2C o EXTERNAL_EEPROM_SPI
#define EXTERNAL_EEPROM_METODO_NUM		1						// 1 o 2


/**/
typedef enum external_eeprom_t {
	MICROCHIP_24XX16B  	= 0,		//Memoria: 16	Tamaño de página: 16
	MICROCHIP_24XX32A  	= 1,		//Memoria: 32	Tamaño de página: 32
	MICROCHIP_24XX64   	= 2,		//Memoria: 64	Tamaño de página: 32
	MICROCHIP_24XX128  	= 3,		//Memoria: 128  Tamaño de página: 64
	MICROCHIP_24XX256  	= 4,		//Memoria: 256  Tamaño de página: 64
	MICROCHIP_24XX512  	= 5,		//Memoria: 512	Tamaño de página: 128
	MICROCHIP_24XX1025 	= 6,		//Memoria: 1024	Tamaño de página: 128
	MICROCHIP_24XX1026 	= 7,		//Memoria: 1024	Tamaño de página: 128
	ATMEL_AT24CM02	   	= 8			//Memoria: 2048	Tamaño de página: 256
	//Agregar
} external_eeprom_t;



/*
	Enumeración para tamaño de EEPROM en kibibits. Las memorias EEPROM seriales se designan generalmente en kibibits.
*/
typedef enum external_eeprom_size_t {
    kibits_2 = 2,
    kibits_4 = 4,
    kibits_8 = 8,
    kibits_16 = 16,
    kibits_32 = 32,
    kibits_64 = 64,
    kibits_128 = 128,
    kibits_256 = 256,
    kibits_512 = 512,
    kibits_1024 = 1024,
    kibits_2048 = 2048
} external_eeprom_size_t;

const uint16_t external_eeprom_capacidad[] = {16,32,64,128,256,512,1024,1024,2048};
const uint16_t external_eeprom_tamano_pagina[] = {16,32,32,64,64,128,128,128,256};

/*
	Enumeración para códigos de estado devueltos por ...
*/
typedef enum external_eeprom_status_t {
	EXTERNAL_EEPROM_ADDR_ERR = 0,
	EXTERNAL_EEPROM_OK		= 1,
    EXTERNAL_EEPROM_UNKNOWN_ERROR = 2
	//
} external_eeprom_status_t;

/*
	Macro de dirección i2c (byte de control) de memorias seriales eeprom
*/
#define EXTERNAL_EEPROM_ADDRESS_WRITE	0xA0
#define EXTERNAL_EEPROM_ADDRESS_READ  	0xA1

/*
	Prototipos de funciones
*/
//Función de inicialización
external_eeprom_status_t external_eeprom_init(external_eeprom_t tipo_memoria, uint8_t n_dispositivos);

//Funciones de lectura/escritura de enteros de 8 bits
external_eeprom_status_t external_eeprom_writeByte(uint8_t dato, uint32_t addr);
uint8_t external_eeprom_readByte(uint32_t addr);

//Funciones de lectura/escritura de enteros de 16 bits
external_eeprom_status_t external_eeprom_writeInt16(uint16_t dato, uint32_t addr);
uint16_t external_eeprom_readInt16(uint32_t addr);

//Funciones de lectura/escritura de enteros de 24 bits
external_eeprom_status_t external_eeprom_writeInt24(uint24_t dato, uint32_t addr);
uint24_t external_eeprom_readInt24(uint32_t addr);

//Funciones de lectura/escritura de enteros de 32 bits
external_eeprom_status_t external_eeprom_writeInt32(uint32_t dato, uint32_t addr);
uint32_t external_eeprom_readInt32(uint32_t addr);

//Funciones de lectura/escritura de datos flotantes
external_eeprom_status_t external_eeprom_writeFloat(float dato, uint32_t addr);
float external_eeprom_readFloat(uint32_t addr);

//Funciones de lectura/escritura de arreglos de datos
external_eeprom_status_t external_eeprom_writeBuffer(uint8_t *buffer, uint32_t addr, uint16_t len);
external_eeprom_status_t external_eeprom_readBuffer(uint8_t *buffer, uint32_t addr, uint16_t len);

//Funciones de lectura/escritura de datos en general
external_eeprom_status_t external_eeprom_write(void *datos, uint32_t addr, uint16_t len);
external_eeprom_status_t external_eeprom_read(void *datos, uint32_t addr, uint16_t len);


uint32_t external_eeprom_getDeviceCapacity();
uint32_t external_eeprom_getTotalCapacity();
uint32_t external_eeprom_getMaxAddress();
uint16_t external_eeprom_getPageSize();
const char* external_eeprom_getDeviceName();

/*
	Variables internas
*/
static uint8_t _eepromControlByte;	//Byte de control de la memoria eeprom
static uint16_t _deviceCapacity;	//Capacidad de una memoria, en kilobits
static uint8_t _nDevices;		//Cantidad de memmorias en el bus
static uint16_t _pageSize;		//Tamaño de página en bytes
static uint16_t _nAddrBytes;    //Número de bytes de dirección (1 o 2)
static uint32_t _totalCapacity;	//Capacidad del conjunto de memorias en el bus, en bytes
static uint32_t _maxAddress;	//Última dirección del arrelo de memorias
static external_eeprom_t _deviceType;	//Tipo de memoria utilizada
static uint8_t addr_U,addr_H,addr_L;    //Variables de direccionamiento

#endif /*EXTERNAL_EEPROM_H*/
