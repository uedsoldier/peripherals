/**
 * @file i2c.h
 * @brief Librería para utilización de módulo MSSP (I²C) por hardware para microcontroladores PIC de 8 bits.
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/

#ifndef I2C_H
#define	I2C_H


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../pconfig.h"
#include "../../utils/utils.h"

/**
  * @brief Enumeración de estados de comunicación I²C
*/
typedef enum i2c_status_t {
	I2C_WRITE_COLLISION		= -1,	// Error de colisión WCOL
	I2C_ACK  				= 0,	// Acknowledge
	I2C_NACK 				= 1,	// No Acknowledge
	I2C_TIMEOUT				= 2		// Timeout en la comunicación I²C
} i2c_status_t;

/**
 * @brief Variables de velocidad del bus I²C, expresada en [kHz]
 */
#if defined (I2C_V1) || defined (I2C_V4)
static uint16_t i2c_speed;
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2)
static uint16_t i2c1_speed;
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
static uint16_t i2c2_speed;
#endif


//Definición de pines
#if defined (I2C_IO_V1 ) 
#define I2C_SCL_TRIS	TRISCbits.TRISC3
#define I2C_SDA_TRIS	TRISCbits.TRISC4
#endif

#if defined (I2C_IO_V2)
#define I2C_SCL_TRIS	TRISCbits.TRISC5
#define I2C_SDA_TRIS	TRISCbits.TRISC4
#endif

#if defined (I2C_IO_V3)
#define I2C_SCL_TRIS	TRISBbits.TRISB1
#define I2C_SDA_TRIS	TRISBbits.TRISB0
#endif

#if defined (I2C_IO_V4)
#define I2C_SCL_TRIS	TRISBbits.TRISB6
#define I2C_SDA_TRIS	TRISBbits.TRISB4

#endif

#if defined (I2C_IO_V5)
#define I2C1_SCL_TRIS	TRISCbits.TRISC3
#define I2C1_SDA_TRIS	TRISCbits.TRISC4
#define I2C2_SCL_TRIS	TRISBbits.TRISB1
#define I2C2_SDA_TRIS	TRISBbits.TRISB2
#endif

#if defined (I2C_IO_V6)
#define I2C2_SCL_TRIS	TRISDbits.TRISD0
#define I2C2_SDA_TRIS	TRISDbits.TRISD1
#endif

#if defined (I2C_IO_V7)
#define I2C2_SCL_TRIS	TRISDbits.TRISD6
#define I2C2_SDA_TRIS	TRISDbits.TRISD5
#endif

/**
 * Constantes de opciones para registro SSPCON
*/
#define   I2C_SLAVE_7   			0b00000110     	// Modo I²C esclavo, con direccionamiento de 7 bits
#define   I2C_SLAVE_10  			0b00000111    	// Modo I²C esclavo, con direccionamiento de 10 bits
#define   I2C_MASTER    			0b00001000     	// Modo I²C maestro
#define   I2C_MASTER_FIRMWARE		0b00001011		// Modo I²C maestro controlado por firmware (slave idle)
#define   I2C_SLAVE_7_STSP_INT 		0b00001110		// Modo I²C esclavo, con direccionamiento de 7 bits con interrupciones de bits START y STOP habilitadas
#define   I2C_SLAVE_10_STSP_INT 	0b00001111		// Modo I²C esclavo, con direccionamiento de 10 bits con interrupciones de bits START y STOP habilitadas

/**
 * Constantes de opciones para slew_rate
*/
#define   I2C_SLEW_OFF  			0b10000000  	// Slew rate deshabilitado, para modo de 100[kHz]
#define   I2C_SLEW_ON   			0b00000000  	// Slew rate habilitado, para modo de 400[kHz]

/**
 * Prototipos de funciones
*/
#if defined (I2C_V1) || defined (I2C_V4)
void i2c_init(uint8_t opciones_sspcon, uint8_t opciones_slew_rate, uint16_t opciones_sspadd);
void i2c_start(void);
void i2c_stop(void);
void i2c_restart(void);
i2c_status_t i2c_writeByte(uint8_t dato);
uint8_t i2c_readByte(uint8_t ack);
void i2c_enable(void);
void i2c_disable(void);
void i2c_writeInt16(uint16_t dato);
void i2c_writeInt24(uint24_t dato);
void i2c_writeInt32(uint32_t dato);
void i2c_writeFloat(float dato);
void i2c_setSpeed(uint16_t speed);
uint16_t i2c_getSpeed(void);
#endif

#if defined (I2C_V2) || defined (I2C_V3) || defined (I2C_V5) || defined (I2C_V6) || defined (I2C_V6_1) || defined (I2C_V6_2)
void i2c1_init(uint8_t opciones_sspcon, uint8_t opciones_slew_rate, uint16_t opciones_sspadd);
void i2c1_start(void);
void i2c1_stop(void);
void i2c1_restart(void);
i2c_status_t i2c1_writeByte(uint8_t dato);
uint8_t i2c1_readByte(uint8_t ack);
void i2c1_enable(void);
void i2c1_disable(void);
void i2c1_writeInt16(uint16_t dato);
void i2c1_writeInt24(uint24_t dato);
void i2c1_writeInt32(uint32_t dato);
void i2c1_writeFloat(float dato);
void i2c1_setSpeed(uint16_t speed);
uint16_t i2c1_getSpeed(void);
#endif

#if defined (I2C_V3) || defined (I2C_V6) || defined (I2C_V6_1)
void i2c2_init(uint8_t opciones_sspcon, uint8_t opciones_slew_rate, uint16_t opciones_sspadd);
void i2c2_start(void);
void i2c2_stop(void);
void i2c2_restart(void);
i2c_status_t i2c2_writeByte(uint8_t dato);
uint8_t i2c2_readByte(uint8_t ack);
void i2c2_enable(void);
void i2c2_disable(void);
void i2c2_writeInt16(uint16_t dato);
void i2c2_writeInt24(uint24_t dato);
void i2c2_writeInt32(uint32_t dato);
void i2c2_writeFloat(float dato);
void i2c2_setSpeed(uint16_t speed);
uint16_t i2c2_getSpeed(void);
#endif

#endif	/* I2C_H */

