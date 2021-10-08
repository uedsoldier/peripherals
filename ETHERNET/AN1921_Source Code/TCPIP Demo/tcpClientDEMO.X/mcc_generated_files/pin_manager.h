/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC16F18346
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set Toggle_Led aliases
#define Toggle_Led_TRIS               TRISAbits.TRISA5
#define Toggle_Led_LAT                LATAbits.LATA5
#define Toggle_Led_PORT               PORTAbits.RA5
#define Toggle_Led_WPU                WPUAbits.WPUA5
#define Toggle_Led_OD                ODCONAbits.ODCA5
#define Toggle_Led_ANS                ANSELAbits.ANSA5
#define Toggle_Led_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define Toggle_Led_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define Toggle_Led_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define Toggle_Led_GetValue()           PORTAbits.RA5
#define Toggle_Led_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define Toggle_Led_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define Toggle_Led_SetPullup()      do { WPUAbits.WPUA5 = 1; } while(0)
#define Toggle_Led_ResetPullup()    do { WPUAbits.WPUA5 = 0; } while(0)
#define Toggle_Led_SetPushPull()    do { ODCONAbits.ODCA5 = 1; } while(0)
#define Toggle_Led_SetOpenDrain()   do { ODCONAbits.ODCA5 = 0; } while(0)
#define Toggle_Led_SetAnalogMode()  do { ANSELAbits.ANSA5 = 1; } while(0)
#define Toggle_Led_SetDigitalMode() do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set RB4 procedures
#define RB4_SetHigh()    do { LATBbits.LATB4 = 1; } while(0)
#define RB4_SetLow()   do { LATBbits.LATB4 = 0; } while(0)
#define RB4_Toggle()   do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RB4_GetValue()         PORTBbits.RB4
#define RB4_SetDigitalInput()   do { TRISBbits.TRISB4 = 1; } while(0)
#define RB4_SetDigitalOutput()  do { TRISBbits.TRISB4 = 0; } while(0)
#define RB4_SetPullup()     do { WPUBbits.WPUB4 = 1; } while(0)
#define RB4_ResetPullup()   do { WPUBbits.WPUB4 = 0; } while(0)
#define RB4_SetAnalogMode() do { ANSELBbits.ANSB4 = 1; } while(0)
#define RB4_SetDigitalMode()do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RB6 procedures
#define RB6_SetHigh()    do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()   do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()   do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()         PORTBbits.RB6
#define RB6_SetDigitalInput()   do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()  do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()     do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()   do { WPUBbits.WPUB6 = 0; } while(0)
#define RB6_SetAnalogMode() do { ANSELBbits.ANSB6 = 1; } while(0)
#define RB6_SetDigitalMode()do { ANSELBbits.ANSB6 = 0; } while(0)

// get/set Pot aliases
#define Pot_TRIS               TRISCbits.TRISC0
#define Pot_LAT                LATCbits.LATC0
#define Pot_PORT               PORTCbits.RC0
#define Pot_WPU                WPUCbits.WPUC0
#define Pot_OD                ODCONCbits.ODCC0
#define Pot_ANS                ANSELCbits.ANSC0
#define Pot_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define Pot_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define Pot_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define Pot_GetValue()           PORTCbits.RC0
#define Pot_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define Pot_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define Pot_SetPullup()      do { WPUCbits.WPUC0 = 1; } while(0)
#define Pot_ResetPullup()    do { WPUCbits.WPUC0 = 0; } while(0)
#define Pot_SetPushPull()    do { ODCONCbits.ODCC0 = 1; } while(0)
#define Pot_SetOpenDrain()   do { ODCONCbits.ODCC0 = 0; } while(0)
#define Pot_SetAnalogMode()  do { ANSELCbits.ANSC0 = 1; } while(0)
#define Pot_SetDigitalMode() do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()    do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()   do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()   do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()         PORTCbits.RC6
#define RC6_SetDigitalInput()   do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()  do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()     do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()   do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode() do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()    do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()   do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()   do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()         PORTCbits.RC7
#define RC7_SetDigitalInput()   do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()  do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()     do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()   do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode() do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/