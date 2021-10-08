/*
    Librería de detección de causa de reinicio para microcontroladores PIC de 8 bits
    Autor: Ing. José Roberto Parra Trewartha
    Compilador: XC8
*/

#ifndef RESET_H
#define	RESET_H

#define WDT_ENABLED
#define STVR_ENABLED    /*Stack Over/ Under flow reset enable control */

#if defined(BOR_V1)
#define BOR_ENABLED
#endif

bool isMCLR(void);       /* MCLR reset?                   */
void status_reset(void);  /* Reset the POR and BOR bits    */
bool isPOR(void);        /* POR reset?                    */
bool isWU(void);         /* Wakeup during sleep?          */

#if defined(BOR_ENABLED)
bool isBOR(void);        /* BOR reset?                    */
#endif

#if defined(WDT_ENABLED)
bool isWDTTO(void);      /* WDT timeout during operation? */
bool isWDTWU(void);      /* WDT timeout during sleep?     */
#endif

#if defined (LVD_V1)
bool isLVD(void);        /*LVD-- Low voltage detect?      */
#endif




#endif /*RESET_H*/
