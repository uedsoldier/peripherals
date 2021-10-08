/**
  Network layer implementation
	
  Company:
    Microchip Technology Inc.

  File Name:
    network.c

  Summary:
    Network layer handling.

  Description:
    This file provides the network layer implementation for TCP/IP stack.

 */

/*

©  [2015] Microchip Technology Inc. and its subsidiaries.  You may use this software 
and any derivatives exclusively with Microchip products. 
  
THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS, 
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF 
NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS 
INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE 
IN ANY APPLICATION. 

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL 
OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED 
TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY 
OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S 
TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED 
THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS. 

*/

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "network.h"
#include "tcpip_types.h"
#include "arpv4.h"
#include "ipv4.h"
#include "tcpv4.h"
#include "rtcc.h"
#include "ethernet_driver.h"
#include "ip_database.h"

time_t arpTimer;

void Network_Init(void)
{
    ETH_Init();
    ARPV4_Init();
    IPV4_Init();
    TCP_Init();
    rtcc_init();
    Network_WaitForLink();  
    timersInit();
}

void timersInit()
{
    time(&arpTimer);
    arpTimer += 10;  
}

void Network_WaitForLink(void)
{
    while(!ETH_CheckLinkUp()); 
}

void Network_Manage(void)
{
    time_t now;
    static time_t nowPv = 0;

    ETH_EventHandler();
    Network_Read(); // handle any packets that have arrived...

    // manage any outstanding timeouts
    time(&now);
    if(now >= arpTimer)
    {
        ARPV4_Update();
        arpTimer += 10;
    }
    if(now > nowPv) // at least 1 second has elapsed
    {
        // is defined as a minimum of 1 seconds in RFC973
        TCP_Update();  // handle timeouts

    }
    nowPv = now;
}

void Network_Read(void)
{
    ethernetFrame_t header;
    char debug_str[80];

    if(ETH_packetReady())
    {
        ETH_NextPacketUpdate();
        ETH_ReadBlock((char *)&header, sizeof(header));
        header.id.type = ntohs(header.id.type); // reverse the type field
        switch (header.id.type)
        {
            case ETHERTYPE_VLAN:
                break;
            case ETHERTYPE_ARP:
                ARPV4_Packet();
                break;
            case ETHERTYPE_IPV4:
                IPV4_Packet();
                break;
            case ETHERTYPE_IPV6:
                break;
            default:
                break;
        }        
        ETH_Flush();
    }
}
