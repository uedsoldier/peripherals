/**
  * @file flash.h
  * @brief Librería para manejo de memoria flash de microcontroladores PIC de 8 bits
  * @author Ing. José Roberto Parra Trewartha
*/
#ifndef FLASH_H
#define	FLASH_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../pconfig.h"
#include "../../utils/utils.h"

#if defined (FLASH_V1_1)

#define FLASH_WRITE_BLOCK 	8
#define FLASH_ERASE_BLOCK 	64

#elif defined (FLASH_V1_2)

#define FLASH_WRITE_BLOCK 	32
#define FLASH_ERASE_BLOCK 	64

#elif defined (FLASH_V1_3)

#define FLASH_WRITE_BLOCK 	16
#define FLASH_ERASE_BLOCK 	64

#elif defined (FLASH_V1_4)

#define FLASH_WRITE_BLOCK 	64
#define FLASH_ERASE_BLOCK 	64

#elif defined (FLASH_V1_5) || defined (FLASH_V2_1)

#define FLASH_WRITE_BLOCK 	64
#define FLASH_ERASE_BLOCK 	1024

#elif defined (FLASH_V1_6)

#define FLASH_WRITE_BLOCK 	128
#define FLASH_ERASE_BLOCK 	128

#endif

//Macro para cargar en los apuntadores de tabla (table pointers) la dirección flash deseada, un dato de 24 bits
#define flash_loadAddress(address)	TBLPTRU = make8(address,2); TBLPTRH = make8(address,1); TBLPTRL = make8(address,0)

/** Macro para leer un dato desde la memoria flash mediante la precarga de los apuntadores de tabla (table pointers)
  * desde la dirección flash deseada, mediante la macro anterior.
*/
#ifdef __18CXX
#define flash_tableRead(data)						_asm TBLRD _endasm; data=TABLAT
#else
#define flash_tableRead(data)						asm("\tTBLRD*"); data=TABLAT
#endif

/**
  * Prototipos de funciones
*/
uint8_t flash_readByte(uint32_t address);


#endif	/* FLASH_H */

