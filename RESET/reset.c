/*
    Librería de detección de causa de reinicio para microcontroladores PIC de 8 bits
    Autor: Ing. José Roberto Parra Trewartha
    Compilador: XC8
*/
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "funciones_auxiliares.h"
#include "pconfig.h"
#include "reset.h"

/*
	Función de detección de BOR (Brown Out Reset)
*/
bool isBOR()
{
	if(!RCONbits.NOT_BOR)
	{
		if(!RCONbits.NOT_POR)
			return false;
		return true;
	}
	return false;
}
/*
	Función de POR (Power-On Reset)
*/
bool isPOR()
{
	if(!RCONbits.NOT_POR)           // If POR cleared
    {
        if(RCONbits.NOT_PD)     // If PD set
        {
           	// If BOR is enabled
			#if defined(BOR_ENABLED)
            if(!RCONbits.NOT_BOR)   // If BOR cleared
            {
                // if BOR and WDT are enabled
				#if defined(WDT_ENABLED)
                if(!RCONbits.NOT_TO)    // If TO cleared
                    return false;// Return FALSE
				#endif
                return true;       // Else return TRUE
            }
            return false;            // Else return FALSE
                // BOR is disabled
			#else 
            // If WDT is enabled
			#if defined(WDT_ENABLED)
            if(!RCONbits.NOT_TO)    // If TO cleared
                return false;    // Return FALSE
            return true;               // Else return TRUE
			#else
            // Otherwise BOR and WDT are disabled
            return true;               // Else return TRUE
			#endif
			#endif
        }
        return false;                    // Else return FALSE
    }
    return false;
}
/*
	Función de detección de reset por MCLR
*/
bool isMCLR()
{
    if(RCONbits.NOT_POR)                    // If POR is set
    {
        if(RCONbits.NOT_PD)             // If NOT_PD is set
        {
            // If BOR is enabled
            #if defined(BOR_ENABLED)
            if(RCONbits.NOT_BOR)    // If BOR is set
            {
                // If BOR and WDT are enabled
                #if defined(WDT_ENABLED)
                if(!RCONbits.NOT_TO)    // If TO is cleared
                    return false;// Return FALSE
                #endif
                if(!RCONbits.NOT_RI)    // If RI is cleared
                    return false;// Return FALSE
                #if defined(STVR_ENABLED)
                #if defined (STK_SFR_V1)
                if(STKPTRbits.STKFUL)    // If STKFUL is set
                #else
                if(STKPTRbits.STKOVF)    // If STKOVF is set
                #endif
                    return false;// Return FALSE
                if(STKPTRbits.STKUNF)    // If STKUNF is set
                    return false;// Return FALSE
                #endif
                return true;       // Else return TRUE
            }
            return false;            // Else return FALSE
            // BOR is disabled
            #else
            // If WDT is enabled
            #if defined(WDT_ENABLED)
            if(!RCONbits.NOT_TO)    // If TO is cleared
                return false;    // Return FALSE
            if(!RCONbits.NOT_RI)    // If RI is cleared
                return false;// Return FALSE
            #if defined(STVR_ENABLED)
            if(STKPTRbits.STKOVF)    // If STKOVF is set
                return false;// Return FALSE

                        if(STKPTRbits.STKUNF)    // If STKUNF is set
                                return false;// Return FALSE
            #endif
                         return true;               // Else return TRUE
            // BOR and WDT are disabled
            #else
            if(!RCONbits.NOT_RI)    // If RI is cleared
                return false;// Return FALSE

            #if defined(STVR_ENABLED)
            if(STKPTRbits.STKOVF)    // If STKOVF is set
                return false;// Return FALSE
            if(STKPTRbits.STKUNF)    // If STKUNF is set
                return false;// Return FALSE
            #endif
            return true;               // Else return TRUE
            #endif
            #endif
        }
        return false;                    // Else return FALSE
    }
    return false;                            // Else return FALSE
}

#if defined (LVD_V1)
/*
	Función de detección de reset por LVD (Low-Voltage Detect)
*/
bool isLVD()
{
   return PIR2bits.LVDIF ? true:false;
}
#endif


#if defined(WDT_ENABLED)
/*
	Función de detección de reset por WDT (WatchDog Timer)
*/
bool isWDTTO()
{
    if(RCONbits.NOT_POR)                            // if POR is set
    {
        if(RCONbits.NOT_PD)                     // if NOT_PD is set
        {
            // If BOR is enabled
			#if defined(BOR_ENABLED)
            if(RCONbits.NOT_BOR)            // if BOR is set
            {
                if(RCONbits.NOT_TO)     // if TO is set
                    return false;    // Return FALSE
                return true;               // Else return TRUE
            }
            return false;                    // Else return FALSE
            // BOR is disabled
			#else
            if(RCONbits.NOT_TO)             // if TO is set
                return false;            // Return FALSE
            return true;                       // Else return TRUE
			#endif
        }
        return false;                            // Else return FALSE
    }
    return false;                                    // Else return FALSE
}
#endif

/*
	Función de detección de Wake-up
*/
bool isWU()
{
    if(RCONbits.NOT_POR)                    // If POR is set
    {
        if(!RCONbits.NOT_PD)            // If PD is cleared
        {
            // If BOR is enabled
			#if defined(BOR_ENABLED)
            if(RCONbits.NOT_BOR)    // If BOR is set
            {
                // If WDT is enabled
				#if defined(WDT_ENABLED)
                if(!RCONbits.NOT_TO)    // If TO is cleared
                    return false;// Return FALSE
				#endif
                return true;       // Else return TRUE
            }
            return false;            // Else return FALSE
        	// BOR is disabled
			#else 
        	// If WDT is enabled
			#if defined(WDT_ENABLED)
            if(!RCONbits.NOT_TO)    // If TO is cleared
                return false;    // Return FALSE
            return true;               // Else return TRUE
			#else
            return true;               // Else return TRUE
			#endif
			#endif
        }
        return false;                    // Else return FALSE
    }
    return false;                            // Else return FALSE
}

/*
	Función de reinicio de los bits POR y BOR
*/
void status_reset(void)
{
        RCONbits.NOT_POR = 1;                   // Reset POR bit
        // If BOR enabled
		#if defined(BOR_ENABLED)
        RCONbits.NOT_BOR = 1;                   // Reset BOR bit
		#endif
}
