/**
  * @file flash.c
  * @brief Librería para manejo de memoria flash de microcontroladores PIC de 8 bits
  * @author Ing. José Roberto Parra Trewartha
*/
#include "flash.h"

#pragma warning disable 373
#pragma warning disable 520

/**
  * @brief Función de escritura de byte en memoria flash 
  * @param address: (uint32_t) Dirección de memoria en la que se desea escribir el dato 
  * @param dato: (uint8_t) Dato de 8 bits a escribir en la dirección deseada
  * @return (void) 
*/
void flash_writeByte(uint32_t address, uint8_t dato)
{
    // PENDIENTE
}

/**
  * @brief Función de lectura de byte en memoria flash. 
  * @param address: (uint32_t) Dirección de memoria flash de la cual se desea leer el dato de 8 bits 
  * @return (uint8_t) Dato presente en la dirección de memoria flash indicada
*/
uint8_t flash_readByte(uint32_t address)
{
    flash_loadAddress(address);
    #ifdef __18CXX
    _asm TBLRD _endasm;
    return TABLAT;
    #else
    asm("\tTBLRD*");
    return TABLAT;
    #endif
}

