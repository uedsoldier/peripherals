#include <stdint.h>
#include "../../peripherals/I2C/i2c.h"

/**
 * Funciones de manejo estructurado de datos provenientes de un bus I²C
 */

/**
 * @brief Definición de buffer de recepción I²C implementado por software. Comentar o no según necesidades del proyecto.
 */
#define I2C_RX_BUFFER
#define I2C1_RX_BUFFER
#define I2C2_RX_BUFFER

/**
 * @brief Definición de buffer de transmisión I²C implementado por software. Comentar o no según necesidades del proyecto.
 */
#define I2C_TX_BUFFER
#define I2C1_TX_BUFFER
#define I2C2_TX_BUFFER


/**
 * @brief Tamaño máximo del buffer de recepción I²C Por defecto, es igual a 128, pero puede adecuarse a las necesidades del usuario. Se recomienda también que sea potencia de 2
 */
#define I2C_RX_BUFFER_MAX_SIZE 128

/**
 * @brief Tamaño máximo del buffer de transmisión I²C Por defecto, es igual a 128, pero puede adecuarse a las necesidades del usuario. Se recomienda también que sea potencia de 2
 */
#define I2C_TX_BUFFER_MAX_SIZE 128

// Buffer RX (implementación de buffer circular)

#ifdef I2C_RX_BUFFER
#define I2C_RX_BUFFER_SIZE 32               	// Tamaño del buffer de recepción I²C (se recomienda un máximo de 128, por razones de cantidad de memoria RAM de los microcontroladores)
uint8_t buffer_rx_i2c[I2C_RX_BUFFER_SIZE];  	// Declaración del buffer de recepción I²C
#if (I2C_RX_BUFFER_SIZE >= I2C_RX_BUFFER_MAX_SIZE)
	#define I2C_RX_BUFFER_SIZE I2C_RX_BUFFER_MAX_SIZE
#endif
volatile int8_t i2c_rx_queueFront;		    	// Índice de frente de buffer de recepción I²C
volatile int8_t i2c_rx_queueEnd;		        	// Índice de fin de buffer de recepción I²C
#endif

#ifdef I2C1_RX_BUFFER
#define I2C1_RX_BUFFER_SIZE 32               	// Tamaño del buffer de recepción I²C (se recomienda un máximo de 128, por razones de cantidad de memoria RAM de los microcontroladores)
uint8_t buffer_rx_i2c1[I2C1_RX_BUFFER_SIZE];  	// Declaración del buffer de recepción I²C
#if (I2C1_RX_BUFFER_SIZE >= I2C_RX_BUFFER_MAX_SIZE)
	#define I2C1_RX_BUFFER_SIZE I2C_RX_BUFFER_MAX_SIZE
#endif
volatile int8_t i2c1_rx_queueFront;		    	// Índice de frente de buffer de recepción I²C
volatile int8_t i2c1_rx_queueEnd;		        	// Índice de fin de buffer de recepción I²C
#endif

#ifdef I2C2_RX_BUFFER
#define I2C2_RX_BUFFER_SIZE 32               	// Tamaño del buffer de recepción I²C (se recomienda un máximo de 128, por razones de cantidad de memoria RAM de los microcontroladores)
uint8_t buffer_rx_i2c1[I2C2_RX_BUFFER_SIZE];  	// Declaración del buffer de recepción I²C
#if (I2C2_RX_BUFFER_SIZE >= I2C_RX_BUFFER_MAX_SIZE)
	#define I2C2_RX_BUFFER_SIZE I2C_RX_BUFFER_MAX_SIZE
#endif
volatile int8_t i2c2_rx_queueFront;		    	// Índice de frente de buffer de recepción I²C
volatile int8_t i2c2_rx_queueEnd;		        	// Índice de fin de buffer de recepción I²C
#endif

// Buffer TX (implementación como arreglo convencional)

#ifdef I2C_TX_BUFFER
#define I2C_TX_BUFFER_SIZE 32               		// Tamaño del buffer de transmisión I²C (se recomienda un máximo de 128, por razones de cantidad de memoria RAM de los microcontroladores)
uint8_t buffer_tx_i2c[I2C_TX_BUFFER_SIZE];  		// Declaración del buffer de transmisión I²C
#if (I2C_TX_BUFFER_SIZE >= I2C_TX_BUFFER_MAX_SIZE)
	#define I2C_TX_BUFFER_SIZE I2C_TX_BUFFER_MAX_SIZE
#endif
volatile uint16_t i2c_tx_bufferIndex;		    	// Índice de buffer de transmisión I²C
volatile uint16_t i2c_tx_bufferLength;		        // Contador de fin de buffer de transmisión I²C
#endif

#ifdef I2C1_TX_BUFFER
#define I2C1_TX_BUFFER_SIZE 32               		// Tamaño del buffer de transmisión I²C (se recomienda un máximo de 128, por razones de cantidad de memoria RAM de los microcontroladores)
uint8_t buffer_tx_i2c1[I2C1_TX_BUFFER_SIZE];  		// Declaración del buffer de transmisión I²C
#if (I2C1_TX_BUFFER_SIZE >= I2C_TX_BUFFER_MAX_SIZE)
	#define I2C1_TX_BUFFER_SIZE I2C_TX_BUFFER_MAX_SIZE
#endif
volatile uint16_t i2c1_tx_bufferIndex;		    	// Índice de buffer de transmisión I²C
volatile uint16_t i2c1_tx_bufferLength;		        // Contador de fin de buffer de transmisión I²C
#endif

#ifdef I2C2_TX_BUFFER
#define I2C2_TX_BUFFER_SIZE 32               		// Tamaño del buffer de transmisión I²C (se recomienda un máximo de 128, por razones de cantidad de memoria RAM de los microcontroladores)
uint8_t buffer_tx_i2c1[I2C2_TX_BUFFER_SIZE];  		// Declaración del buffer de transmisión I²C
#if (I2C2_TX_BUFFER_SIZE >= I2C_TX_BUFFER_MAX_SIZE)
	#define I2C2_TX_BUFFER_SIZE I2C_TX_BUFFER_MAX_SIZE
#endif
volatile uint16_t i2c2_tx_bufferIndex;		    	// Índice de buffer de transmisión I²C
volatile uint16_t i2c2_tx_bufferLength;		        // Contador de fin de buffer de transmisión I²C
#endif


i2c_status_t i2c_write(void *datos, uint16_t len);
void i2c_read(void *datos, uint16_t len);
void i2c_interruptHandler();						// Función a ejecutar en interrupción por recepción I²C	
uint8_t i2c_rx_dataAvailable();						// Devuelve cantidad de datos en el buffer de recepción I²C		
uint8_t i2c_rx_readByteBuffer();					// Devuelve el primer dato del buffer FIFO modificando índices de principio y fin
uint8_t i2c_rx_firstByteReceived();					// Devuelve el primer dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
uint8_t i2c_rx_lastByteReceived();					// Devuelve el último dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
void i2c_rx_readBuffer(int8_t *buff,uint8_t len); 	// Lectura de 'len' elementos del buffer de recepción I²C	
void i2c_rx_flushBuffer(void); 						// Reinicia índices de buffer, para indicar que éste se encuentra vacío


/////////////////////////////////////////////

/**
  * @brief Función para escribir un dato de cualquier tipo por I²C	
  * @param datos: (void*) Dato de cualquier tipo a trasmitir mediante I²C
  * @param len: (uint16_t) Cantidad de bytes a transmitir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (i2c_status_t) Estado de escritura
*/
i2c_status_t i2c_write(void* datos, uint16_t len) {
	i2c_status_t status;
    uint8_t* _datos = (uint8_t *)datos;
    while(len--)
        status = i2c_writeByte(*_datos++);
	return status;
}

/**
  * @brief Función para leer un dato de cualquier tipo por I²C
  * @param datos: (void*) Dato de cualquier tipo a recibir mediante polling de su bandera de interrupción
  * @param len: (uint16_t) Cantidad de bytes a recibir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void i2c_read(void* datos, uint16_t len) {
    uint8_t* _datos = (uint8_t*)datos;           //Apuntador a variable
    while(len--) {
        while(!SSPIF);                  //En espera de recepción de datos
        *(_datos++) = i2c_readByte();      //Recepción de datos
    }
}

/**
 * @brief  Función para utilizarse si se desea implementar un buffer por software para el módulo I²C.
 * Se deberá incluir en la rutina de interrupción por recepción exitosa en el módulo MSSP (verificando estado alto de la bandera SSPIF)  
 * @param (void) 
 * @return (void)
*/
void i2c_interruptHandler() {
    int8_t dato=i2c_readByte(); //Lee dato recibido
    if(((i2c_rx_queueEnd==I2C_RX_BUFFER_SIZE-1) && i2c_rx_queueFront==0 ) || ((i2c_rx_queueEnd+1)==i2c_rx_queueFront)) {
        //Si buffer lleno, índice en la primera posición
		i2c_rx_queueFront++;
        if(i2c_rx_queueFront == I2C_RX_BUFFER_SIZE)
			i2c_rx_queueFront=0;
    }

    if(i2c_rx_queueEnd == I2C_RX_BUFFER_SIZE-1)
        i2c_rx_queueEnd=0;
    else
        i2c_rx_queueEnd++;
    buffer_rx_i2c[i2c_rx_queueEnd]=dato;
    if(i2c_rx_queueFront==-1) 
        i2c_rx_queueFront=0;
}

/**
 * @brief Función que devuelve la cantidad de datos presente en el buffer de recepción I²C
 * @param (void) 
 * @return (uint8_t) Cantidad de bytes presentes en el buffer por software asociado a I²C 
*/
uint8_t i2c_dataAvailable() {
    if(i2c_rx_queueFront == -1) 
        return 0;
    if(i2c_rx_queueFront < i2c_rx_queueEnd)
        return( i2c_rx_queueEnd - i2c_rx_queueFront + 1);
    else if( i2c_rx_queueFront > i2c_rx_queueEnd)
        return (I2C_RX_BUFFER_SIZE-i2c_rx_queueFront + i2c_rx_queueEnd+1);
    else
        return 1;
}

/**
 * @brief Función que devuelve el primer dato presente en el buffer de recepción I²C modificando los índices de la cola; es decir, de la cola saldrá el primer elemento (FIFO).
 * Es responsabilidad del usuario verificar si hay datos disponibles en el buffer, de lo contrario, se devolverá 0xFF
 * @param (void) 
 * @return (int8_t) primer dato de la cola o buffer circular
*/
uint8_t i2c_rx_readByteBuffer() {
    uint8_t dato;
    //Verificar si la cola está vacía
    if(i2c_rx_queueFront == -1)
    	return 0xFF;           //Devuelve entonces 0xFF

    dato = buffer_rx_i2c[i2c_rx_queueFront];

    if(i2c_rx_queueFront == i2c_rx_queueEnd) {
        //Si solo hay un dato, la cola se encuentra vacía ahora
        i2c_rx_queueFront=-1;
        i2c_rx_queueEnd=-1;
    }
    else {
        i2c_rx_queueFront++;
        if(i2c_rx_queueFront == I2C_RX_BUFFER_SIZE)
            i2c_rx_queueFront = 0;
    }
    return dato;
}

/**
 * @brief Función que devuelve el primer dato presente en el buffer de recepción I²C sin modificar índices de la cola.
 * Es solo una función de consulta del primer dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos, devolviendo 0xFF.
 * @param (void) 
 * @return (uint8_t) primer dato de la cola o buffer circular
*/
uint8_t i2c_rx_firstByteReceived() {
    return buffer_rx_i2c[i2c_rx_queueFront];
}

/**
 * @brief Función que devuelve el último dato presente en el buffer de recepción I²C sin modificar índices de la cola.
 * Es solo una función de consulta del último dato presente. Es responsabilidad del usuario verificar que el buffer tenga datos, o arriesgarse a obtener datos erróneos, devolviendo 0xFF.
 * @param (void) 
 * @return (uint8_t) último dato de la cola o buffer circular
*/
int8_t i2c_rx_lastByteReceived() {
    uint8_t dato;
    dato = (i2c_rx_queueEnd == 0xFF)? -1:buffer_rx_i2c[i2c_rx_queueEnd];   
    //Verificar si la cola está vacía, en caso contrario devuelve último valor de la cola
    return dato;
}

/**
 * @brief Función para lectura de una cantidad de elementos definida desde el buffer circular de recepción I²C 
 * implementado por software, para ponerlos en otro buffer (arreglo)  
 * @param buff (uint8_t *) Apuntador al arreglo en el cual se desean copiar los datos.
 * @param len (uint8_t) cantidad de elementos a copiar de un buffer a otro 
 * @return (void)
*/
void i2c_rx_readBuffer(uint8_t *buff,uint8_t len) {
    uint8_t i;
    for( i = 0; i != len; i++) {
        buff[i] = i2c_rx_readByteBuffer();
    }
}

/**
 * @brief Función para limpiar buffer de recepción I²C reiniciando los valores de los índices del buffer circular
 * @param (void)
 * @return (void)
*/
void i2c_rx_flushBuffer() {
    i2c_rx_queueEnd     = -1;
    i2c_rx_queueFront   = -1;
}
