/*
	Librería para manejo de memoria EEPROM interna de microcontroladores PIC de 8 bits
	Autor: Ing. José Roberto Parra Trewartha
	Compilador: XC8

*/
#ifndef EEPROM_INTERNA_H
#define	EEPROM_INTERNA_H


/*
    Macro que define tamaño de EEPROM: 256, 1024 bytes. Se cambia automáticamente
    según microcontrolador a utilizar
*/

#if defined(EEP_V1)||defined(EEP_V2)
#define tipo_address_eeprom_interna uint8_t

#elif defined(EEP_V3) 
#define tipo_address_eeprom_interna uint16_t
#endif

/*
	Prototipos de funciones básicas
*/
#if defined (EEP_V1) || defined (EEP_V2) || defined (EEP_V3)
/*
	Funciones básicas de lectura y escritura de bytes
*/
void internal_eeprom_writeByte(uint8_t dato,tipo_address_eeprom_interna address);
uint8_t internal_eeprom_readByte(tipo_address_eeprom_interna address);

/*
	Funciones para manejo de variables de 2 o más bytes
*/

void internal_eeprom_writeInt16(uint16_t dato,tipo_address_eeprom_interna address);
uint16_t internal_eeprom_readInt16(tipo_address_eeprom_interna address);

void internal_eeprom_writeInt24(uint24_t dato,tipo_address_eeprom_interna address);
uint24_t internal_eeprom_readInt24(tipo_address_eeprom_interna address);

void internal_eeprom_writeInt32(uint32_t dato,tipo_address_eeprom_interna address);
uint32_t internal_eeprom_readInt32(tipo_address_eeprom_interna address);

void internal_eeprom_writeFloat(float dato,tipo_address_eeprom_interna address);
float internal_eeprom_readFloat(tipo_address_eeprom_interna address);

/*
	Funciones de lectura/escritura de bits individuales
*/
void internal_eeprom_writeBit(bool dato, tipo_address_eeprom_interna address,uint8_t posicion);
bool internal_eeprom_readBit(tipo_address_eeprom_interna address, uint8_t posicion);
#endif


#endif	/* EEPROM_INTERNA_H */

