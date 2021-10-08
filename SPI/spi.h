/*
    Librería de comunicación spi por hardware para microcontroladores PIC de 8 bits
    Autor: Ing. José Roberto Parra Trewartha
    Compilador: XC8
*/

#ifndef SPI_H
#define	SPI_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../pconfig.h"
#include "../../utils/utils.h"

//Macros para configuración de registro SSPSTAT
#define SPI_SAMPLE_END		0b10000000 	//Muestreo de datos de entrada al final de la salida de datos
#define SPI_SAMPLE_MIDDLE	0b00000000  //Muestreo de datos de entrada en la mitad de la salida de datos

/*
Modos de comunicacion SPI: (CPOL,CPHA) (Polaridad de reloj, Fase de reloj)
 * 
 * _______________________________________________________________________________
 * Modo |  Motorola  | Microchip |  Muestreo   |  Propagacion   | Estado inactivo |
 *      |  CPOL CPHA | CKP CKE   |  en flanco  |   en flanco    |   de reloj      |
 * _____|____________|___________|_____________|________________|_________________|
 *   0       0,0          0,1      ascendente      descendente         bajo
 *   1       0,1          0,0      descendente     ascendente          bajo
 *   2       1,0          1,1      descendente     ascendente          alto
 *   3       1,1          1,0      ascendente      descendente         alto
 * --------------------------------------------------------------------------------
 * 
 * Los modos más utilizados son el 0 (0,0 También conocido como MicroWire) 
 * y el 3 (1,1). Maestro y esclavo deben estar en el mismo modo para poder 
 * comunicarse adecuadamente.
*/
#define SPI_MODE_00	0 //Modo conocido tambien como MicroWire
#define SPI_MODE_01	1
#define SPI_MODE_10	2
#define SPI_MODE_11	3

//Constantes de rol para modo maestro
#define SPI_MASTER_CLK_DIV_4    0b00000000  //SPI modo maestro con reloj de oscilación = Fosc/4
#define SPI_MASTER_CLK_DIV_8    0b00001010  //SPI modo maestro con reloj de oscilación = Fosc/8 (No implementado en algunos modelos, verificar)    
#define SPI_MASTER_CLK_DIV_SSPADD 0b00001010  //SPI modo maestro con reloj de oscilación = FOSC/(4 * (SSPxADD+1))
#define SPI_MASTER_CLK_DIV_16   0b00000001  //SPI modo maestro con reloj de oscilación = Fosc/16
#define SPI_MASTER_CLK_DIV_64   0b00000010  //SPI modo maestro con reloj de oscilación = Fosc/64
#define SPI_MASTER_CLK_DIV_TMR2 0b00000011  //SPI modo maestro con reloj de oscilación = TMR2/2 (No implementado en algunos modelos, verificar)   

//Constantes de rol para modo esclavo
#define SPI_SLAVE_SS_ENABLED 	0b00000100  //SPI modo esclavo con SS habilitado
#define SPI_SLAVE_SS_DISABLED	0b00000101  //SPI modo esclavo con SS deshabilitado

/*
	Definición de prototipos de funciones
*/

#if defined (SPI_V1) || defined (SPI_V4)

void spi_init(uint8_t rol_spi,uint8_t modo, uint8_t fase_propagacion);	//Inicialización de módulo SSP en modo SPI
uint8_t spi_readByte(void);						//Lectura de 1 byte vía SPI
bool spi_writeByte(uint8_t dato_tx);		//Escritura de 1 byte vía SPI
uint8_t spi_xmit(uint8_t dato_tx);			//Escritura y lectura secuencial de 1 byte vía SPI

//Funciones prototipo para envío/recepción de datos de dos o más bytes
void spi_writeInt16(uint16_t dato);			//Escritura de dato entero de 16 bits
uint16_t spi_readInt16(uint16_t dato);		//Lectura de dato entero de 16 bits
void spi_writeInt24(uint24_t dato);			//Escritura de dato entero de 24 bits
uint24_t spi_readInt24(uint24_t dato);		//Lectura de dato entero de 24 bits
void spi_writeInt32(uint32_t dato);			//Escritura de dato entero de 32 bits
uint32_t spi_readInt32(uint32_t dato);		//Lectura de dato entero de 32 bits
void spi_writeFloat(float dato);			//Escritura de dato flotante de 32(24) bits
float spi_readFloat(void);					//Lectura de dato flotante de 32(24) bits

void spi_enable(void);							//Habilitación de módulo SSP
void spi_disable(void);							//Deshabilitación de módulo 
void spi_setSpeed(uint16_t spi_speed_kHz);       // Función auxiliar para definir velocidad de comunicaciòn SPI mediante registro SSPxADD
#endif

#if defined (SPI_V2) || defined (SPI_V3) || defined(SPI_V5) || defined (SPI_V5_1) || defined (SPI_V5_2) || defined (SPI_V6)
void spi1_init(uint8_t rol_spi, uint8_t modo_spi, uint8_t fase_propagacion);	//Inicialización de módulo SSP en modo SPI
uint8_t spi1_readByte(void);						//Lectura de 1 byte vía SPI
bool spi1_writeByte(uint8_t dato_tx);		//Escritura de 1 byte vía SPI
uint8_t spi1_xmit(uint8_t dato_tx);			//Escritura y lectura secuencial de 1 byte vía SPI

//Funciones prototipo para envío/recepción de datos de dos o más bytes
void spi1_writeInt16(uint16_t dato);			//Escritura de dato entero de 16 bits
uint16_t spi1_readInt16(uint16_t dato);		//Lectura de dato entero de 16 bits
void spi1_writeInt24(uint24_t dato);			//Escritura de dato entero de 24 bits
uint24_t spi1_readInt24(uint24_t dato);		//Lectura de dato entero de 24 bits
void spi1_writeInt32(uint32_t dato);			//Escritura de dato entero de 32 bits
uint32_t spi1_readInt32(uint32_t dato);		//Lectura de dato entero de 32 bits
void spi1_writeFloat(float dato);			//Escritura de dato flotante de 32(24) bits
float spi1_readFloat(void);					//Lectura de dato flotante de 32(24) bits

void spi1_enable(void);							//Habilitación de módulo SSP
void spi1_disable(void);							//Deshabilitación de módulo 
void spi1_setSpeed(uint16_t spi_speed_kHz);     // Función auxiliar para definir velocidad de comunicaciòn SPI mediante registro SSPxADD
#endif

#if defined (SPI_V3) || defined (SPI_V5) || defined (SPI_V5_1) || defined (SPI_V6)

void spi2_init(uint8_t rol_spi, uint8_t modo_spi, uint8_t fase_propagacion);	//Inicialización de módulo SSP en modo SPI
uint8_t spi2_readByte(void);						//Lectura de 1 byte vía SPI
bool spi2_writeByte(uint8_t dato_tx);		//Escritura de 1 byte vía SPI
uint8_t spi2_xmit(uint8_t dato_tx);			//Escritura y lectura secuencial de 1 byte vía SPI

//Funciones prototipo para envío/recepción de datos de dos o más bytes

void spi2_writeInt16(uint16_t dato);			//Escritura de dato entero de 16 bits
uint16_t spi2_readInt16(uint16_t dato);		//Lectura de dato entero de 16 bits
void spi2_writeInt24(uint24_t dato);			//Escritura de dato entero de 24 bits
uint24_t spi2_readInt24(uint24_t dato);		//Lectura de dato entero de 24 bits
void spi2_writeInt32(uint32_t dato);			//Escritura de dato entero de 32 bits
uint32_t spi2_readInt32(uint32_t dato);		//Lectura de dato entero de 32 bits
void spi2_writeFloat(float dato);			//Escritura de dato flotante de 32(24) bits
float spi2_readFloat(void);					//Lectura de dato flotante de 32(24) bits

void spi2_enable(void);							//Habilitación de módulo SSP
void spi2_disable(void);							//Deshabilitación de módulo
void spi2_setSpeed(uint16_t spi_speed_kHz);      // Función auxiliar para definir velocidad de comunicaciòn SPI mediante registro SSPxADD

#endif

#endif	/* SPI_H */


