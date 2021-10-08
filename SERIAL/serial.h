/**
 * @file serial.h
 * @brief Librería para utilización de puerto serial por hardware para microcontroladores PIC de 8 bits
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/

#ifndef SERIAL_H
#define SERIAL_H

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../pconfig.h"
#include "../../utils/utils.h"

/*
	Definiciones del buffer de recepción serial implementado por software para USART. Comentar o no según necesidades del proyecto
*/
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
#define SERIAL_RX_BUFFER
//Si se cuenta con más de un módulo USART, y se desea implementar el buffer, usar la macro siguiente:
#else
#define SERIAL1_RX_BUFFER
//#define SERIAL2_RX_BUFFER
//#define SERIAL3_RX_BUFFER
//#define SERIAL4_RX_BUFFER
#endif


//
#ifdef SERIAL_RX_BUFFER
#define SERIAL_RX_BUFFER_SIZE 32 					//Tamaño del buffer de recepción serial(máximo 256)
uint8_t buffer_rx_serial[SERIAL_RX_BUFFER_SIZE]; 	//Declaración del buffer de recepción serial
#if (SERIAL_RX_BUFFER_SIZE>=256)
	#define SERIAL_RX_BUFFER_SIZE 256
#endif
volatile int16_t serial_queueFront;		//Índice de frente de buffer de recepción serial
volatile int16_t serial_queueEnd;		//Índice de fin de buffer de recepción serial
#endif


#ifdef SERIAL1_RX_BUFFER
#define SERIAL1_RX_BUFFER_SIZE 256 //Tamaño del buffer de recepción serial
uint8_t buffer_rx_serial1[SERIAL1_RX_BUFFER_SIZE]; //Declaración del buffer de recepción serial
#if (SERIAL1_RX_BUFFER_SIZE>=256)
	#define SERIAL1_RX_BUFFER_SIZE 256
#endif
volatile int16_t serial1_queueFront;		//Índice de frente de buffer de recepción serial
volatile int16_t serial1_queueEnd;		//Índice de fin de buffer de recepción serial
#endif

#ifdef SERIAL2_RX_BUFFER
#define SERIAL2_RX_BUFFER_SIZE 64 //Tamaño del buffer de recepción serial
uint8_t buffer_rx_serial2[SERIAL2_RX_BUFFER_SIZE]; //Declaración del buffer de recepción serial
#if (SERIAL2_RX_BUFFER_SIZE>=256)
	#define SERIAL2_RX_BUFFER_SIZE 256
#endif
volatile int16_t serial2_queueFront;		//Índice de frente de buffer de recepción serial
volatile int16_t serial2_queueEnd;		//Índice de fin de buffer de recepción serial
#endif

#ifdef SERIAL3_RX_BUFFER
#define SERIAL3_RX_BUFFER_SIZE 64 //Tamaño del buffer de recepción serial
uint8_t buffer_rx_serial3[SERIAL3_RX_BUFFER_SIZE]; //Declaración del buffer de recepción serial
#if (SERIAL3_RX_BUFFER_SIZE>=256)
	#define SERIAL3_RX_BUFFER_SIZE 256
#endif
volatile int16_t serial3_queueFront;		//Índice de frente de buffer de recepción serial
volatile int16_t serial3_queueEnd;		//Índice de fin de buffer de recepción serial
#endif

#ifdef SERIAL4_RX_BUFFER
#define SERIAL4_RX_BUFFER_SIZE 64 //Tamaño del buffer de recepción serial
uint8_t buffer_rx_serial4[SERIAL4_RX_BUFFER_SIZE]; //Declaración del buffer de recepción serial
#if (SERIAL4_RX_BUFFER_SIZE>=256)
	#define SERIAL4_RX_BUFFER_SIZE 256
#endif
volatile int16_t serial4_queueFront;		//Índice de frente de buffer de recepción serial
volatile int16_t serial4_queueEnd;		//Índice de fin de buffer de recepción serial
#endif


/*
	Definiciones para configuración de módulos USART en todas sus versiones. 
*/
//Definición de modo síncrono o asíncrono (el más común al emular terminal serial). Enmascarado con 0x80.
#define USART_SYNC_MODE 	0x80
#define USART_ASYNC_MODE	0x00

//Definiciones de rol en modo síncrono. Enmascarado con 0x40.
#define USART_SYNC_MASTER 	0x40
#define USART_SYNC_SLAVE	0x00

//Definiciones de cantidad de bits a transmitir/recibir. Enmascarado con 0x20.
#define USART_9_BIT			0x20
#define USART_8_BIT 		0x00


//Definiciones de alta velocidad del baud rate. Enmascarado con 0x10.
//#define USART_BRGH_HIGH	0x10
//#define USART_BRGH_LOW	0x00

//Definiciones de transmisión contínua o sencilla. Enmascarado con 0x08.
#define USART_CONT_RX		0x08
#define USART_SINGLE_RX		0x00

//Definición de estándar de comunicación 8N1
#define USART_8N1           (USART_ASYNC_MODE|USART_8_BIT|USART_CONT_RX)

//Definiciones de detección automática de dirección (Address detection). Enmascarado con 0x04.
#define USART_ADD_DETECT_ON		0x04
#define USART_ADD_DETECT_OFF	0x00

#if defined (EAUSART_V3 ) ||defined (EAUSART_V4 ) ||defined (EAUSART_V5 )||\
    defined (EAUSART_V6 ) ||defined (EAUSART_V7 ) ||defined (EAUSART_V8 ) ||\
	defined (EAUSART_V9 ) ||defined (EAUSART_V10 ) || defined (EAUSART_V11) \
	|| defined (EAUSART_V11_1) || defined (EAUSART_V12)
/*
	Definiciones para configuración de módulos EUSART para el registro BAUDCON. Para los microcontroladores que poseen Enhanced USART
*/
//Definición de estado lógico en espera (idle) de pin de reloj (modo síncrono) o de datos TX invertidos (modo asíncrono). Enmascarado con 0x80
#define EUSART_SYNC_IDLE_HIGH
#define EUSART_SYNC_IDLE_HIGH
#define EUSART_ASYNC_TX_NOT_INVERTED
#define EUSART_ASYNC_TX_INVERTED

//Definiciones de generador de baud rate de 16 bits. Enmascarado con 0x40.
//#define EUSART_BRG16
//#define EUSART_BRG8

//Definiciones para modalidad Wake-up, para modo asíncrono. Enmascarado con 0x20
#define EUSART_WAKEUP_ON
#define EUSART_WAKEUP_OFF

//Definiciones para auto detección de baud rate en modo asíncrono. Enmascarado con 0x10
#define EUSART_AUTO_BAUDRATE_ON
#define EUSART_AUTO_BAUDRATE_OFF

#endif
/*
	Definición de prototipos de funciones
*/

//Dispositivos con solo un módulo USART cuyos registros no contienen el sufijo "1"
#if defined (AUSART_V1) || defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
void serial_init(uint8_t param_config, uint32_t baud_rate,int16_t offset_calibracion); //Inicializa comunicación serial.
void serial_writeByte(uint8_t dato); //Envia un byte vía EUSART
uint8_t serial_readByte(void); 	//Lee un byte recibido por EUSART
void serial_puts(const char *cadena); //Rutina para mandar cadena de caracteres vía EUSART
void serial_gets(char *buffer, uint8_t len);	//Función para lectura de datos vía EUSART
void serial_writeLine(const char *cadena); //Mada cadena de caracteres precedida de CR y NL
void serial_writeBuffer(uint8_t *buffer, uint8_t length); //Rutina para mandar arreglo de bytes vía EUSART
void serial_writeInt16(uint16_t dato);
uint16_t serial_readInt16(void);
void serial_writeInt24(uint24_t dato);
uint24_t serial_readInt24(void);
void serial_writeInt32(uint32_t dato);
uint32_t serial_readInt32(void);
void serial_writeFloat(float dato);
float serial_readFloat(void);
void serial_write(void* datos, uint16_t len);
void serial_read(void* datos, uint16_t len);
void serial_interruptHandler(void); //Rutina a ejecutar en interrupción por recepción serial
uint16_t serial_dataAvailable(void); //Devuelve cantidad de datos en el buffer serial
int8_t serial_readByteBuffer(void);	//Devuelve el primer dato del buffer FIFO modificando índices de principio y fin
int8_t serial_firstByteReceived();	//Devuelve el primer dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
int8_t serial_lastByteReceived();	//Devuelve el último dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
void serial_readBuffer(uint8_t *buff,uint8_t len); //Lectura de 'len' elementos del buffer serial
void serial_flushBuffer(void); //Lee todos los datos del buffer, reiniciando sus indices
#if defined (EAUSART_V3) || defined (EAUSART_V4) || defined (EAUSART_V5)
void serial_baudcon(uint8_t param_config); //Configura registro BAUDCON
#endif


//Definición de estructura de datos auxiliar para depuración y estatus del módulo USART en cuestión
union SERIAL{
	uint8_t byte_status;
	struct{
		unsigned RX_NINE:1;         // Bit de recepción #8 si el modo de 9 bits está habilitado
	    unsigned TX_NINE:1;         // Bit de transmisión #8 si el modo de 9 bits está habilitado
	    unsigned FRAME_ERROR:1;     // Framing Error de USART
	    unsigned OVERRUN_ERROR:1;   // Overrun Error para USART
		unsigned fill:4;
	};
};
union SERIAL serialStatus;

#endif

//Los siguientes dispositivos cuentan o bien con dos módulos USART (USART1 y USART2) o con uno solo cuyos registros van nombrados TXSTA1, RCSTA1 , etc.
#if defined (AUSART_V2) ||defined (EAUSART_V6) ||defined (EAUSART_V7) ||\
    defined (EAUSART_V8) ||defined (EAUSART_V9)  || defined (EAUSART_V10) ||\
	defined (EAUSART_V11) || defined (EAUSART_V11_1) || defined (EAUSART_V12)
void serial1_init(uint8_t param_config, uint32_t baud_rate,int16_t offset_calibracion); //Inicializa comunicación serial.
void serial1_writeByte(uint8_t dato); //Envia un byte vía EUSART
uint8_t serial1_readByte(void); 	//Lee un byte recibido por EUSART
void serial1_puts(const char *cadena); //Rutina para mandar cadena de caracteres vía EUSART
void serial1_gets(char *buffer, uint8_t len);	//Función para lectura de datos vía EUSART
void serial1_writeLine(const char *cadena); //Mada cadena de caracteres precedida de CR y NL
void serial1_writeBuffer(uint8_t *buffer, uint8_t length); //Rutina para mandar arreglo de bytes vía EUSART
void serial1_writeInt16(uint16_t dato);
uint16_t serial1_readInt16(void);
void serial1_writeInt24(uint24_t dato);
uint24_t serial1_readInt24(void);
void serial1_writeInt32(uint32_t dato);
uint32_t serial1_readInt32(void);
void serial1_writeFloat(float dato);
float serial1_readFloat(void);
void serial1_write(void* datos, uint16_t len);
void serial1_read(void* datos, uint16_t len);
void serial1_interruptHandler(void); //Rutina a ejecutar en interrupción por recepción serial
uint16_t serial1_dataAvailable(void); //Devuelve cantidad de datos en el buffer serial
int8_t serial1_readByteBuffer(void);	//Devuelve el primer dato del buffer FIFO modificando índices de principio y fin
int8_t serial1_firstByteReceived();	//Devuelve el primer dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
int8_t serial1_lastByteReceived();	//Devuelve el último dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
void serial1_readBuffer(uint8_t *buff,uint8_t len); //Lectura de 'len' elementos del buffer serial
void serial1_flushBuffer(void); //Lee todos los datos del buffer, reiniciando sus indices
#if defined (EAUSART_V6)|| defined (EAUSART_V7) || defined (EAUSART_V8) ||\
    defined (EAUSART_V9) || defined (EAUSART_V10) || defined (EAUSART_V11) || defined (EAUSART_V11_1) \
    || defined (EAUSART_V12)
void serial1_baudcon(uint8_t param_config); //Configura registro BAUDCON
#endif



//Definición de estructura de datos auxiliar para depuración y estatus del módulo USART en cuestión
union SERIAL1{
	uint8_t byte_status;
	struct{
		unsigned RX_NINE:1;         // Bit de recepción #8 si el modo de 9 bits está habilitado
	    unsigned TX_NINE:1;         // Bit de transmisión #8 si el modo de 9 bits está habilitado
	    unsigned FRAME_ERROR:1;     // Framing Error de USART
	    unsigned OVERRUN_ERROR:1;   // Overrun Error para USART
		unsigned fill:4;
	};
};
union SERIAL1 serial1Status;

#endif

//Los siguientes dispositivos cuentan efectivamente con un módulo USART2
#if defined (AUSART_V2 ) ||defined (EAUSART_V6 ) ||defined (EAUSART_V7 ) ||\
    defined (EAUSART_V8 ) ||defined (EAUSART_V9 ) || defined (EAUSART_V11) \
	|| defined (EAUSART_V12)
void serial2_init(uint8_t param_config, uint32_t baud_rate,int16_t offset_calibracion); //Inicializa comunicación serial.
void serial2_writeByte(uint8_t dato); //Envia un byte vía EUSART
uint8_t serial2_readByte(void); 	//Lee un byte recibido por EUSART
void serial2_puts(const char *cadena); //Rutina para mandar cadena de caracteres vía EUSART
void serial2_gets(char *buffer, uint8_t len);	//Función para lectura de datos vía EUSART
void serial2_writeLine(const char *cadena); //Mada cadena de caracteres precedida de CR y NL
void serial2_writeBuffer(uint8_t *buffer, uint8_t length); //Rutina para mandar arreglo de bytes vía EUSART
void serial2_writeInt16(uint16_t dato);
uint16_t serial2_readInt16(void);
void serial2_writeInt24(uint24_t dato);
uint24_t serial2_readInt24(void);
void serial2_writeInt32(uint32_t dato);
uint32_t serial2_readInt32(void);
void serial2_writeFloat(float dato);
float serial2_readFloat(void);
void serial2_write(void* datos, uint16_t len);
void serial2_read(void* datos, uint16_t len);
void serial2_interruptHandler(void); //Rutina a ejecutar en interrupción por recepción serial
uint16_t serial2_dataAvailable(void); //Devuelve cantidad de datos en el buffer serial
int8_t serial2_readByteBuffer(void);	//Devuelve el primer dato del buffer FIFO modificando índices de principio y fin
int8_t serial2_firstByteReceived();	//Devuelve el primer dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
int8_t serial2_lastByteReceived();	//Devuelve el último dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
void serial2_readBuffer(uint8_t *buff,uint8_t len); //Lectura de 'len' elementos del buffer serial
void serial2_flushBuffer(void); //Lee todos los datos del buffer, reiniciando sus indices
#if defined (EAUSART_V7) || defined (EAUSART_V8) || defined (EAUSART_V9) || defined (EAUSART_V11) || defined (EAUSART_V12)
void serial2_baudcon(uint8_t param_config); //Configura registro BAUDCON
#endif
//Definición de estructura de datos auxiliar para depuración y estatus del módulo USART en cuestión
union SERIAL2{
	uint8_t byte_status;
	struct{
		unsigned RX_NINE:1;         // Bit de recepción #8 si el modo de 9 bits está habilitado
	    unsigned TX_NINE:1;         // Bit de transmisión #8 si el modo de 9 bits está habilitado
	    unsigned FRAME_ERROR:1;     // Framing Error de USART
	    unsigned OVERRUN_ERROR:1;   // Overrun Error para USART
		unsigned fill:4;
	};
};
union SERIAL2 serial2Status;

#endif

//Los siguientes dispositivos cuentan con 4 módulos USART
#if defined (EAUSART_V12)
//USART3
void serial3_inituint8_t param_config, uint32_t baud_rate,int16_t offset_calibracion); //Inicializa comunicación serial.
void serial3_writeByte(uint8_t dato); //Envia un byte vía EUSART
uint8_t serial3_readByte(void); 	//Lee un byte recibido por EUSART
void serial3_puts(const char *cadena); //Rutina para mandar cadena de caracteres vía EUSART
void serial3_gets(char *buffer, uint8_t len);	//Función para lectura de datos vía EUSART
void serial3_writeLine(const char *cadena); //Mada cadena de caracteres precedida de CR y NL
void serial3_writeBuffer(uint8_t *buffer, uint8_t length); //Rutina para mandar arreglo de bytes vía EUSART
void serial3_writeInt16(uint16_t dato);
uint16_t serial3_readInt16(void);
void serial3_writeInt24(uint24_t dato);
uint24_t serial3_readInt24(void);
void serial3_writeInt32(uint32_t dato);
uint32_t serial3_readInt32(void);
void serial3_writeFloat(float dato);
float serial3_readFloat(void);
void serial3_write(void* datos, uint16_t len);
void serial3_read(void* datos, uint16_t len);
void serial3_interruptHandler(void); //Rutina a ejecutar en interrupción por recepción serial
uint16_t serial3_dataAvailable(void); //Devuelve cantidad de datos en el buffer serial
int8_t serial3_readByteBuffer(void);	//Devuelve el primer dato del buffer FIFO modificando índices de principio y fin
int8_t serial3_firstByteReceived();	//Devuelve el primer dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
int8_t serial3_lastByteReceived();	//Devuelve el último dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
void serial3_readBuffer(uint8_t *buff,uint8_t3 len); //Lectura de 'len' elementos del buffer serial
void serial3_flushBuffer(void); //Lee todos los datos del buffer, reiniciando sus indices
void serial3_baudcon(uint8_t param_config); //Configura registro BAUDCON

//Definición de estructura de datos auxiliar para depuración y estatus del módulo USART en cuestión
union SERIAL3{
	uint8_t byte_status;
	struct{
		unsigned RX_NINE:1;         // Bit de recepción #8 si el modo de 9 bits está habilitado
	    unsigned TX_NINE:1;         // Bit de transmisión #8 si el modo de 9 bits está habilitado
	    unsigned FRAME_ERROR:1;     // Framing Error de USART
	    unsigned OVERRUN_ERROR:1;   // Overrun Error para USART
		unsigned fill:4;
	};
};
union SERIAL3 serial3Status;



//USART4
void serial4_inituint8_t param_config, uint32_t baud_rate,int16_t offset_calibracion); //Inicializa comunicación serial.
void serial4_writeByte(uint8_t dato); //Envia un byte vía EUSART
uint8_t serial4_readByte(void); 	//Lee un byte recibido por EUSART
void serial4_puts(const char *cadena); //Rutina para mandar cadena de caracteres vía EUSART
void serial4_gets(char *buffer, uint8_t len);	//Función para lectura de datos vía EUSART
void serial4_writeLine(const char *cadena); //Mada cadena de caracteres precedida de CR y NL
void serial4_writeBuffer(uint8_t *buffer, uint8_t length); //Rutina para mandar arreglo de bytes vía EUSART
void serial4_writeInt16(uint16_t dato);
uint16_t serial4_readInt16(void);
void serial4_writeInt24(uint24_t dato);
uint24_t serial4_readInt24(void);
void serial4_writeInt32(uint32_t dato);
uint32_t serial4_readInt32(void);
void serial4_writeFloat(float dato);
float serial4_readFloat(void);
void serial4_write(void* datos, uint16_t len);
void serial4_read(void* datos, uint16_t len);
void serial4_interruptHandler(void); //Rutina a ejecutar en interrupción por recepción serial
uint16_t serial4_dataAvailable(void); //Devuelve cantidad de datos en el buffer serial
int8_t serial4_readByteBuffer(void);	//Devuelve el primer dato del buffer FIFO modificando índices de principio y fin
int8_t serial4_firstByteReceived();	//Devuelve el primer dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
int8_t serial4_lastByteReceived();	//Devuelve el último dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
void serial4_readBuffer(uint8_t *buff,uint8_t4 len); //Lectura de 'len' elementos del buffer serial
void serial4_flushBuffer(void); //Lee todos los datos del buffer, reiniciando sus indices
void serial4_baudcon(uint8_t param_config); //Configura registro BAUDCON

//Definición de estructura de datos auxiliar para depuración y estatus del módulo USART en cuestión
union SERIAL4{
	uint8_t byte_status;
	struct{
		unsigned RX_NINE:1;         // Bit de recepción #8 si el modo de 9 bits está habilitado
	    unsigned TX_NINE:1;         // Bit de transmisión #8 si el modo de 9 bits está habilitado
	    unsigned FRAME_ERROR:1;     // Framing Error de USART
	    unsigned OVERRUN_ERROR:1;   // Overrun Error para USART
		unsigned fill:4;
	};
};
union SERIAL4 serial4Status;

#endif

#endif /*SERIAL_H*/