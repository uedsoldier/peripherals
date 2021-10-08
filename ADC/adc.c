/*
	Librería para utilización del módulo ADC en microcontroladores PIC de 8 bits
	Autor: Ing. José Roberto Parra Trewartha
	Compilador: XC8
*/
#include "adc.h"

/****************************************************************************************
* 	Nombre de función:  adc_init                                                  		*
* 	Valor de retorno:   void                                                      		*
* 	Parámetros:    varían según versión.												*
****************************************************************************************/
#if defined (ADC_V1)
void adc_init(uint8_t param_config, uint8_t param_config2) 
{
	/*
	Parámetros: 
	- param_config:		JUSTIFICACION | FUENTE_RELOJ | CONFIGURACION_CANALES_REFERENCIAS
	- param_config2: 	CANAL_ACTUAL_ADC
	*/
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = param_config; //Registro de control de justificación izquierda o derecha, reloj de conversión, configuración de pines analógicos/digitales
	// y referencias de voltaje
	ADCON0 = ((param_config<<2) & 0xC0)|(param_config2 & 0x38 );//Registro de control de reloj de conversión, selección de canal y habilitación de módulo ADC
	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}

#elif defined (ADC_V2)
void adc_init(uint8_t param_config, uint8_t param_config2)
{
	/*
		Parámetros: 
		- param_config:		JUSTIFICACION | FUENTE_RELOJ | CONFIGURACION_CANALES
		- param_config2: 	CANAL_ACTUAL_ADC | REFERENCIAS
	*/
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1
	ADCON2 = 0; //Reinicio de registro de control ADCON2

	ADCON0 = ( param_config2 >> 1) & 0x3C ;							//Registro de selección de canal y habilitación
  	ADCON1 = ((param_config2 << 4) & 0x30) | (param_config & 0x0F);	//Registro de control de referencias de voltaje y configuración de pines analógicos/digitales
  	ADCON2 = ( param_config & 0x80) | ((param_config >> 4) & 0x07);			//Registro de control de justificación y fuente de reloj A/D

	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}

#elif defined (ADC_V3)
void adc_init(uint8_t param_config, uint8_t param_config2, uint8_t port_config)
{
	/*
		Parámetros: 
		- param_config:		JUSTIFICACION | TIEMPO_ADQUISICION | FUENTE_RELOJ
		- param_config2: 	CANAL_ACTUAL_ADC | REFERENCIAS
		- port_config: 		Puertos del AN0 - AN6 configurados individualmente. Pueden utilizarse las macros ADC_XANA (0<= X <= 6) si así se desea
	*/    
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1
	ADCON2 = 0; //Reinicio de registro de control ADCON2

	ADCON0 = ( (param_config2 >> 1) & 0x3C ) | ((param_config2 << 6) & 0xC0 );   
    //Registro de control de referencias de voltaje, selección de canal y habilitación

	ADCON1 = port_config;
	//Registro de configuración de pines analógicos/digitales

	ADCON2 = ((param_config & 0x80 )|((param_config >> 4) & 0x07)) | ((param_config << 2) & 0x38);
	//Registro de control de justificación, tiempo de adquisición programable y fuente de reloj A/D

	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}


#elif defined (ADC_V4) || defined (ADC_V5) || defined (ADC_V6)
void adc_init(uint8_t param_config, uint8_t param_config2, uint8_t port_config)
{
	/*
	Parámetros: 
	- param_config:		JUSTIFICACION | TIEMPO_ADQUISICION | FUENTE_RELOJ 
	- param_config2: 	CANAL_ACTUAL_ADC | REFERENCIAS
	- port_config: 		Puertos del AN0 - AN10 configurados individualmente. Pueden utilizarse las macros ADC_XANA (0<= X <= 10) si así se desea
	*/    
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1
	ADCON2 = 0; //Reinicio de registro de control ADCON2

	ADCON0 = ( (param_config2 >> 1) & 0x3C );   
    //Registro de selección de canal y habilitación

	ADCON1 = ( port_config & 0x0F ) | ( (param_config2<<4) & 0x30 );
	//Registro de configuración de pines analógicos/digitales y de control de referencias de voltaje

	ADCON2 = ((param_config & 0x80 )|((param_config >> 4) & 0x07)) | ((param_config << 2) & 0x38);
	//Registro de control de justificación, tiempo de adquisición programable y fuente de reloj A/D

	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}

#elif defined (ADC_V8)
void adc_init(uint8_t param_config, uint8_t param_config2, uint16_t port_config)
{
	/*
	Parámetros: 
	- param_config:		JUSTIFICACION | TIEMPO_ADQUISICION | FUENTE_RELOJ 
	- param_config2: 	CANAL_ACTUAL_ADC | REFERENCIAS
	- port_config: 		Puertos del AN0 - AN10 configurados individualmente. Pueden utilizarse las macros ADC_XANA (0<= X <= 10) si así se desea
	*/    
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1
	ADCON2 = 0; //Reinicio de registro de control ADCON2

	ADCON0 = ( (param_config2 >> 1) & 0x3C );   
    //Registro de selección de canal y habilitación

	ADCON1 |= ( (param_config2<<4) & 0x30 );
	//Registro de control de referencias de voltaje

	ADCON2 = ((param_config & 0x80 )|((param_config >> 4) & 0x07)) | ((param_config << 2) & 0x38);
	//Registro de control de justificación, tiempo de adquisición programable y fuente de reloj A/D

	ANSEL = port_config;
    ANSELH = (port_config >> 8);
	//Registros de configuración de pines analógicos/digitales

	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}

#elif defined (ADC_V7)
void adc_init(uint8_t param_config, uint8_t param_config2, uint8_t param_config3)
{
	/*
	Parámetros: 
	- param_config:		CONTROL_CONTINUOUS_LOOP | TIPO_CONV | MULTI/SINGLE_CHANNEL | MODO_SECUENCIAL
	- param_config2: 	REFERENCIAS | CONTROL_BUFFER | CONTROL_TRIGGERS
	- param_config3: 	JUSTIFICACION | TIEMPO_ADQUISICION | FUENTE_RELOJ
	*/    
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1
	ADCON2 = 0; //Reinicio de registro de control ADCON2
	ADCON3 = 0; //Reinicio de registro de control ADCON3

	ADCON0 = (param_config<<2) & 0x3C;
	//Registro de control de modo de conversión, single/multi channel, secuencia de conversión y habilitación 
	ADCON1 = (param_config2 & 0xC0) | ((param_config2 >>1) & 0x10);
	//Registro de control de referencias de voltaje y control de buffer FIFO para el ADC.
	ADCON2 = param_config3;
	//Registro de control de justificación, tiempo de adquisición programable y fuente de reloj A/D
	ADCON3 = (param_config2 & 0x1F) | (param_config & 0xC0);
	//Registro control de triggers para el módulo ADC y de control de modo contínuo de conversión (continuous loop)
	ADCHS = 0;
	//Registro de selección de canales analógicos
	ANSEL0 = 0;
	ANSEL1 = 0;
	//Registros de configuración de pines analógicos/digitales

	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}

#elif defined (ADC_V7_1)
void adc_init(uint8_t param_config, uint8_t param_config2, uint8_t param_config3)
{
	/*
	Parámetros: 
	- param_config:		CONTROL_CONTINUOUS_LOOP | TIPO_CONV | MULTI/SINGLE_CHANNEL | MODO_SECUENCIAL
	- param_config2: 	REFERENCIAS | CONTROL_BUFFER | CONTROL_TRIGGERS
	- param_config3: 	JUSTIFICACION | TIEMPO_ADQUISICION | FUENTE_RELOJ
	*/    
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1
	ADCON2 = 0; //Reinicio de registro de control ADCON2
	ADCON3 = 0; //Reinicio de registro de control ADCON3

	ADCON0 = (param_config<<2) & 0x3C;
	//Registro de control de modo de conversión, single/multi channel, secuencia de conversión y habilitación 
	ADCON1 = (param_config2 & 0xC0) | ((param_config2 >>1) & 0x10);
	//Registro de control de referencias de voltaje y control de buffer FIFO para el ADC.
	ADCON2 = param_config3;
	//Registro de control de justificación, tiempo de adquisición programable y fuente de reloj A/D
	ADCON3 = (param_config2 & 0x1F) | (param_config & 0xC0);
	//Registro control de triggers para el módulo ADC y de control de modo contínuo de conversión (continuous loop)
	ADCHS = 0;
	//Registro de selección de canales analógicos
	ANSEL0 = 0;
	//Registro de configuración de pines analógicos/digitales

	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}

#elif defined (ADC_V9) || defined (ADC_V9_1)
void adc_init(uint8_t param_config, uint8_t param_config2, uint16_t port_config)
{
	/*
	Parámetros: 
	- param_config:		JUSTIFICACION | TIEMPO_ADQUISICION | FUENTE_RELOJ  
	- param_config2: 	CANAL_ACTUAL_ADC | REFERENCIAS
	- port_config: 		Puertos del AN0 - AN15 configurados individualmente. Pueden utilizarse las macros ADC_XANA (0<= X <= 15) si así se desea
	*/    
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1

	ADCON0 = ((param_config2 >> 1) & 0x3C) | ((param_config2 << 6) & 0xC0);
    //Registro de control de referencias de voltaje, de selección de canal y habilitación

	ADCON1 = (config & 0x80) | ((config >> 4) & 0x07) | ((config << 2) & 0x38); 
	//Registro de control de justificación, tiempo de adquisición programable, fuente de reloj A/D y calibración.

	ANSEL = port_config;
    ANSELH = (port_config >> 8);
	//Registros de configuración de pines analógicos/digitales

	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}

#elif defined (ADC_V10)
void adc_init(uint8_t param_config, uint8_t param_config2, uint8_t param_config3 ,uint16_t port_config)
{
	/*
	Parámetros: 
	- param_config:		JUSTIFICACION | TIEMPO_ADQUISICION | FUENTE_RELOJ  
	- param_config2:	CANAL_ACTUAL_ADC
	- param_config3:	REFERENCIAS
	- port_config: 		Puertos del AN0 - AN15 configurados individualmente. Pueden utilizarse las macros ADC_XANA (0<= X <= 15) si así se desea
	*/    
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1
	ADCON2 = 0; //Reinicio de registro de control ADCON2

	ADCON0 = (param_config2 >> 1) & 0x3C; 
    //Registro de selección de canal y habilitación.

	ADCON1 = (param_config3 & 0x0C) | (param_config3 & 0x03);
	//Registro de control de referencias de voltaje.

	ADCON2 = (param_config & 0x80) | ((param_config >> 4) & 0x07) | ((param_config << 2) & 0x38);
	//Registro de control de justificación, tiempo de adquisición programable y fuente de reloj A/D.

	ANSEL = port_config;
    ANSELH = (port_config >> 8);
	//Registros de configuración de pines analógicos/digitales

	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}

#elif defined (ADC_V11)
/* 
	Función presente en plib. Aún no hay documentación (datasheets)
*/
void adc_init( uint8_t config, uint8_t config2, unsigned int  config3)
{    
    /*ADCON0 = 0;
    ADCON1 = 0;

    ADCON0 = ((config2 >> 1) & 0b00111100) | //channel selection
             ((config2 << 6) & 0b11000000);	//VREF			 
    ADCON1 = (config & 0b10000000) | //Result Format 
             ((config >> 4) & 0b00000111) | //Conversion clock select
             ((config << 2) & 0b00111000); //acquisition time
			 
    ANCON0 = config3; // port configuration
    ANCON1 = config3 >> 8;  // VBG Selection and port configuration

	if( config2 & 0b10000000 )
    {
      PIR1bits.ADIF = 0;
      PIE1bits.ADIE = 1;
      INTCONbits.PEIE = 1;
    }
    ADCON0bits.ADON = 1;
    */
}

#elif defined (ADC_V11_1)
void adc_init(uint8_t param_config, uint8_t param_config2, uint8_t param_config3 ,uint16_t port_config)
{
	/*
	Parámetros: 
	- param_config:		JUSTIFICACION | TIEMPO_ADQUISICION | FUENTE_RELOJ  
	- param_config2:	CANAL_ACTUAL_ADC | REFERENCIAS
	- param_config3:	CONTROL_TRIGGER
	- port_config: 		Puertos del AN0 - AN15 configurados individualmente. Pueden utilizarse las macros ADC_XANA (0<= X <= 15) si así se desea
	*/    
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1

	ADCON0 = ((param_config2 >> 1) & 0x3C) | ((param_config2 << 6) & 0xC0);	
    //Registro de control de referencias de voltaje, selección de canal y habilitación.

	ADCON1 = param_config & 0x80) | ((param_config >> 4) & 0x07) | ((param_config << 2) & 0x38);
	//Registro de control de justificación, tiempo de adquisición programable y fuente de reloj A/D.

	ANCON0 = port_config; 
    ANCON1 = port_config >> 8;
	//Registros de configuración de pines analógicos/digitales y de Voltage Band Gap (VGP)
    ADCTRIG = ( param_config3 & 0xC0)>>6;
    //Registro control de triggers para el módulo ADC
	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}

#elif defined (ADC_V12)
void adc_init(uint8_t param_config, uint8_t param_config2, uint8_t param_config3)
{
	/*
	Parámetros: 
	- param_config:		JUSTIFICACION | TIEMPO_ADQUISICION | FUENTE_RELOJ	 
	- param_config2: 	REFERENCIAS | CANAL_ACTUAL_ADC 
	- param_config3: 	CONTROL_TRIGGERS | 	Selección de canales analógicos del AN0 al AN11 
	*/    
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1
	ADCON2 = 0; //Reinicio de registro de control ADCON2

	ADCON0 = (param_config2 >> 1) & 0x3C;
	//Registro de control de calibración, selección de canal y habilitación. La función de calibración se lleva a cabo con una macro. 
	ADCON1 = (param_config3  & 0x8F) | ((param_config2 << 4) & 0x30); 
	//Registro de selección de triggers, control de referencias de voltaje y de configuración de pines analógicos/digitales
	ADCON2 = (param_config & 0x80) | ((param_config >> 4) & 0x07) |	((param_config << 2) & 0x38);
	//Registro de control de justificación, tiempo de adquisición programable y fuente de reloj A/D
	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}


#elif defined (ADC_V13) || defined (ADC_V13_1) || defined (ADC_V13_2)\
	|| defined (ADC_V13_3) || defined (ADC_V14) || defined (ADC_V14_1) || defined (ADC_V14_2) || defined (ADC_V14_3)
void adc_init(uint8_t param_config, uint8_t param_config2, uint8_t param_config3)
{
	/*
	Parámetros: 
	- param_config:		JUSTIFICACION | TIEMPO_ADQUISICION | FUENTE_RELOJ	 
	- param_config2: 	CANAL_ACTUAL_ADC 
	- param_config3: 	CONTROL_TRIGGERS | REFERENCIAS | CANAL_NEGATIVO_ACTUAL
	*/    
	ADCON0 = 0;	//Reinicio de registro de control ADCON0
	ADCON1 = 0; //Reinicio de registro de control ADCON1
	ADCON2 = 0; //Reinicio de registro de control ADCON2

	ADCON0 = (param_config2) & 0x7C;
	//Registro de selección de canal y habilitación.
	ADCON1 = param_config3;
	//Registro de selección de triggers, control de referencias de voltaje y de configuración de canales negativos para mediciones diferenciales
	ADCON2 = (param_config & 0x80) | ((param_config >> 4) & 0x07) |	((param_config << 2) & 0x38);
	//Registro de control de justificación, tiempo de adquisición programable y fuente de reloj A/D
	ADCON0bits.ADON = 1;	//Habilitación de módulo ADC
}


#elif defined (ADC_V15) || defined (ADC_V15_1)
void adc_init( uint8_t param_config, uint8_t param_config2, uint8_t param_config3, uint8_t param_config4, uint8_t param_config5, uint8_t param_config6, uint8_t param_config7, uint8_t param_config8)
{
    ADCON1L = 0;
	ADCON1H = 0;
    ADCON2L = 0;
	ADCON2H = 0;
	ADCON3L = 0;
    ADCON3H = 0;
	ADCON5L = 0;
    ADCON5H = 0;

    ADCON1H = param_config;	// Mode and O/P format
	ADCON1L = (param_config2) | 0b00000010; //Trigger  & Sample enable bit 
			 
    ADCON2H = param_config3; // Vref+/- , OFFCAL mode & BUFREGEN
	ADCON2L = param_config4; // Interrupt sample ,Alt i/p mode
	
	ADCON3L = (param_config5) | 0b11111111;	//  CONV CLK
	ADCON3H = (param_config6) | 0b00001111;  // Auto Sample Time select bits
	
	ADCON5L = param_config7; // Write & Compare mode bits
    ADCON5H = param_config8; // Auto-Scan bits
    ADCON1Hbits.ADON = 1;
}

#endif

/****************************************************************************************
* 	Nombre de función:  adc_read                                                  		*
* 	Valor de retorno:   int16 con el valor de conversión del módulo ADC            		*
* 	Parámetros:    varían según versión, pero por lo general no toma parámetros.		*
****************************************************************************************/
#if defined (ADC_V1) || defined (ADC_V2) || defined (ADC_V3) ||\
    defined (ADC_V4) || defined (ADC_V5) || defined (ADC_V6) ||\
    defined (ADC_V7) || defined (ADC_V7_1) || defined (ADC_V8) ||\
    defined (ADC_V9) || defined (ADC_V9_1) || defined (ADC_V10) || defined (ADC_V11) ||\
	defined (ADC_V12) || defined (ADC_V13) || defined (ADC_V13_1)\
	|| defined (ADC_V11_1) || defined (ADC_V13_2) || defined (ADC_V13_3)\
	|| defined (ADC_V14) || defined (ADC_V14_1) || defined (ADC_V14_2) || defined (ADC_V14_3)

uint16_t adc_read(uint8_t modo_lectura_adc)
{
	switch(modo_lectura_adc)
	{
		case ADC_START_READ:
			#if defined (ADC_V9) || defined (ADC_V9_1)
			WDTCONbits.DEVCFG = 0;
			#endif
			ADCON0bits.GO = 1;
			while(ADCON0bits.GO){}
			return make16(ADRESH,ADRESL);
		case ADC_START_ONLY:
			#if defined (ADC_V9) || defined (ADC_V9_1)
			WDTCONbits.DEVCFG = 0;
			#endif
			ADCON0bits.GO = 1;
			return 0;
		case ADC_READ_ONLY:
			return make16(ADRESH,ADRESL);
        default: return 0; break;
	}
}

#elif	defined (ADC_V15) || defined (ADC_V15_1)
//indice_buffer - Es el número del buffer ADC a leer. Número del 0 al 15. 
uint16_t adc_read(uint8_t indice_buffer, uint8_t modo_lectura_adc)
{
	switch(modo_lectura_adc)
	{
		case ADC_START_READ:
			ADCON1Lbits.SAMP = 0; /* clear SAMP to start conversion*/
			while(!ADCON1Lbits.DONE){}
			return(*(&ADCBUF0+indice_buffer));
		case ADC_START_ONLY:
			ADCON1Lbits.SAMP = 0; /* clear SAMP to start conversion*/
			return 0;
		case ADC_READ_ONLY:
			return(*(&ADCBUF0+indice_buffer));
	}   
}
#endif

/************************************************************************************************
* 	Nombre de función:  adc_setChannel                                            				*
* 	Valor de retorno:   void           															*
* 	Parámetros:    		Número de canal ADC a establecer para las conversiones subsecuentes 	*
************************************************************************************************/
#if defined (ADC_V1)
void adc_setChannel(uint8_t canal)
{
	ADCON0 = (canal & 0x38) | (ADCON0 & 0xC7);
}

#elif defined (ADC_V2) || defined (ADC_V3) || defined (ADC_V4) ||\
      defined (ADC_V5) || defined (ADC_V6) || defined (ADC_V8)|| \
	  defined (ADC_V10)|| defined (ADC_V11) || defined (ADC_V12) || defined (ADC_V11_1)	
void adc_setChannel(uint8_t canal)
{
	ADCON0 = ((canal >> 1) & 0x3C) | (ADCON0 & 0xC3);
}

#elif defined (ADC_V9) || defined (ADC_V9_1)
void adc_setChannel(uint8_t canal)
{
	WDTCONbits.DEVCFG = 0;  
  	ADCON0 = ((canal >> 1) & 0x3C) | (ADCON0 & 0xC3);
}

#elif defined (ADC_V13) || defined (ADC_V13_1) || defined (ADC_V13_2)\
	|| defined (ADC_V13_3) || defined (ADC_V14) || defined (ADC_V14_1) || defined (ADC_V14_2) || defined (ADC_V14_3)
void adc_setChannel(uint8_t canal)
{
	ADCON0 = (ADCON0 & 0x83) | (canal & 0x7C);
}

#elif defined (ADC_V15) || defined (ADC_V15_1) 
void adc_setChannel(uint8_t canalA, uint8_t canalB)
{
	ADCHS0L = canalA;//(ADCHS0L & 0b00000000)|((canalA) & 0b11111111); //Para mí, no tiene sentido realizar estas operaciones que dan exactamente lo mismo
  	ADCHS0H = canalB;//(ADCHS0H & 0b00000000)|((canalB) & 0b11111111);
}

#endif