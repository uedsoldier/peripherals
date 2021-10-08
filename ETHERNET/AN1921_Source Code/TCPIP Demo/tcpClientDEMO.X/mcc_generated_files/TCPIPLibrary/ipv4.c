/**
  IPv4 implementation

  Company:
    Microchip Technology Inc.

  File Name:
    ipv4.c

  Summary:
    This is the implementation of IP version 4 stack

  Description:
    This source file provides the implementation of the API for the IP v4 stack.

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

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "network.h"
#include "ipv4.h"
#include "icmp.h"
#include "arpv4.h"
#include "tcpv4.h"
#include "tcpip_types.h"
#include "ethernet_driver.h"
#include "ip_database.h"

uint32_t remoteIpv4Address;
uint16_t ipv4StartPosition;
ipv4Header_t ipv4Header;
static void IPV4_SaveStartPosition(void);


/*
 *  Callback to TCP protocol to deliver the TCP packets
 */
extern void TCP_Recv(uint32_t, uint16_t);

void IPV4_Init(void)
{
    ipdb_init();
}

uint16_t IPV4_PseudoHeaderChecksum(uint16_t payloadLen)
{
    ipv4_pseudo_header_t tmp;
    uint8_t len;
    uint32_t cksm = 0;
    uint16_t *v;

    tmp.srcIpAddress  = ipv4Header.srcIpAddress;
    tmp.dstIpAddress  = ipv4Header.dstIpAddress;
    tmp.protocol      = ipv4Header.protocol;
    tmp.z             = 0;
    tmp.length        = payloadLen;

    len = sizeof(tmp);
    len = len >> 1;

    v = (uint16_t *) &tmp;

    while(len)
    {
        cksm += *v;
        len--;
        v++;
    }

    // wrap the checksum
    cksm = (cksm & 0x0FFFF) + (cksm>>16);

    // Return the resulting checksum
    return cksm;
}

error_msg IPV4_Packet(void)
{
    uint16_t cksm = 0;
    uint16_t length = 0;
    uint32_t addrTemp;
    char msg[40];
    uint8_t hdrLen;

    //calculate the IPv4 checksum
    cksm = ETH_RxComputeChecksum(sizeof(ipv4Header_t), 0);
    if (cksm != 0)
    {
        return IPV4_CHECKSUM_FAILS;
    }
    
    IPV4_SaveStartPosition();
    ETH_ReadBlock((char *)&ipv4Header, sizeof(ipv4Header_t));
    if(ipv4Header.version != 4)
    {
        return IP_WRONG_VERSION; // Incorrect version number
    }

    addrTemp = ipv4Header.dstIpAddress;
    ipv4Header.dstIpAddress = ntohl(addrTemp);
    addrTemp++;

    addrTemp = ipv4Header.srcIpAddress;
    ipv4Header.srcIpAddress = ntohl(addrTemp);

    if(ipv4Header.srcIpAddress == SPECIAL_IPV4_BROADCAST_ADDRESS)
        return DEST_IP_NOT_MATCHED;


    if(ipv4Header.dstIpAddress == ipdb_getAddress() || (ipv4Header.dstIpAddress == IPV4_ZERO_ADDRESS)||
        ((ipv4Header.dstIpAddress == SPECIAL_IPV4_BROADCAST_ADDRESS)
            ||((ipv4Header.dstIpAddress|CLASS_A_IPV4_BROADCAST_MASK == SPECIAL_IPV4_BROADCAST_ADDRESS && ((ipdb_getAddress()|CLASS_A_IPV4_REVERSE_BROADCAST_MASK)== ipv4Header.dstIpAddress)))
                ||((ipv4Header.dstIpAddress|CLASS_B_IPV4_BROADCAST_MASK == SPECIAL_IPV4_BROADCAST_ADDRESS && ((ipdb_getAddress()|CLASS_B_IPV4_REVERSE_BROADCAST_MASK)== ipv4Header.dstIpAddress)))
                    ||((ipv4Header.dstIpAddress|CLASS_C_IPV4_BROADCAST_MASK == SPECIAL_IPV4_BROADCAST_ADDRESS && ((ipdb_getAddress()|CLASS_C_IPV4_REVERSE_BROADCAST_MASK)== ipv4Header.dstIpAddress))))
            || (ipv4Header.dstIpAddress == ALL_HOST_MULTICAST_ADDRESS))
    {
        ipv4Header.length = ntohs(ipv4Header.length);

        hdrLen = (uint8_t)(ipv4Header.ihl << 2);

        if(ipv4Header.ihl < 5)
            return INCORRECT_IPV4_HLEN;

        if (ipv4Header.ihl > 5)
        {
            //Do not process the IPv4 Options field
            ETH_Dump((uint16_t)(hdrLen - sizeof(ipv4Header_t)));
            return IPV4_NO_OPTIONS;
        }
        
        switch((ipProtocolNumbers)ipv4Header.protocol)
        {
            case ICMP_TCPIP:
                {
                    // calculate and check the ICMP checksum
                    if((ipv4Header.dstIpAddress == IPV4_ZERO_ADDRESS))
                    {
                        return DEST_IP_NOT_MATCHED;
                    }
                    length = ipv4Header.length - hdrLen;
                    cksm = ETH_RxComputeChecksum(length, 0);

                    if (cksm == 0)
                    {
                        ICMP_Receive(&ipv4Header);
                    }
                    else
                    {
                        return ICMP_CHECKSUM_FAILS;
                    }
                }
                break;
            case TCP_TCPIP:
                // accept only uni cast TCP packets
                // check the TCP header checksum
                length = ipv4Header.length - hdrLen;
                cksm = IPV4_PseudoHeaderChecksum(length);
                cksm = ETH_RxComputeChecksum(length, cksm);

                // accept only packets with valid CRC Header
                if (cksm == 0 && (ipv4Header.dstIpAddress != SPECIAL_IPV4_BROADCAST_ADDRESS) && (ipv4Header.dstIpAddress != IPV4_ZERO_ADDRESS))                
                {
                    remoteIpv4Address = ipv4Header.srcIpAddress;
                    TCP_Recv(remoteIpv4Address, length);
                }
                break;
            default:
                ETH_Dump(ipv4Header.length);
                break;
        }
        return SUCCESS;
    }
    else
    {
        return DEST_IP_NOT_MATCHED;
    }
}

error_msg IPv4_Start(uint32_t destAddress, ipProtocolNumbers protocol)
{
    error_msg ret = ERROR;
    // get the dest mac address
    const mac48Address_t *macAddress;
    uint32_t targetAddress;

    // Check if we have a valid IPadress and if it's different then 127.0.0.1
    if(((ipdb_getAddress() != 0) || (protocol == UDP_TCPIP))
     && (ipdb_getAddress() != 0x7F000001))
    {
        if(((destAddress == SPECIAL_IPV4_BROADCAST_ADDRESS)
             |((destAddress | CLASS_A_IPV4_BROADCAST_MASK) == SPECIAL_IPV4_BROADCAST_ADDRESS)
                |((destAddress | CLASS_B_IPV4_BROADCAST_MASK )== SPECIAL_IPV4_BROADCAST_ADDRESS)
                    |((destAddress | CLASS_C_IPV4_BROADCAST_MASK) == SPECIAL_IPV4_BROADCAST_ADDRESS))==0) // this is NOT a broadcast message
        {
            if( ((destAddress ^ ipdb_getAddress()) & ipdb_getSubNetMASK()) == 0)
            {
                targetAddress = destAddress;
            }
            else
            {
                targetAddress = ipdb_getRouter();
            }
            macAddress= ARPV4_Lookup(targetAddress);
            if(macAddress == 0)
            {
                ret = ARPV4_Request(targetAddress); // schedule an arp request
                return ret;
            }
        }
        else
        {
            macAddress = &broadcastMAC;
        }
        ret = ETH_WriteStart(macAddress, ETHERTYPE_IPV4);
        if(ret == SUCCESS)
        {
            ETH_Write16(0x4500); // VERSION, IHL, DSCP, ECN
            ETH_Write16(0); // total packet length
            ETH_Write32(0xAA554000); // My IPV4 magic Number..., FLAGS, Fragment Offset
            ETH_Write8(IPv4_TTL); // TTL
            ETH_Write8(protocol); // protocol
            ETH_Write16(0); // checksum. set to zero and overwrite with correct value
            ETH_Write32(ipdb_getAddress());
            ETH_Write32(destAddress);

            // fill the pseudo header for checksum calculation
            ipv4Header.srcIpAddress = ipdb_getAddress();
            ipv4Header.dstIpAddress = destAddress;
            ipv4Header.protocol = protocol;
        }
    }
    return ret;
}

error_msg IPV4_Send(uint16_t payloadLength)
{
    uint16_t totalLength;
    uint16_t cksm;
    error_msg ret;

    totalLength = 20 + payloadLength;
    totalLength = ntohs(totalLength);

    //Insert IPv4 Total Length
    ETH_Insert((char *)&totalLength, 2, sizeof(ethernetFrame_t) + offsetof(ipv4Header_t, length));

    cksm = ETH_TxComputeChecksum(sizeof(ethernetFrame_t),sizeof(ipv4Header_t),0);
    //Insert Ipv4 Header Checksum
    ETH_Insert((char *)&cksm, 2, sizeof(ethernetFrame_t) + offsetof(ipv4Header_t,headerCksm));
    ret = ETH_Send();

    return ret;
}

static void IPV4_SaveStartPosition(void)
{
    ipv4StartPosition = ETH_GetReadPtr();
}

uint16_t IPV4_GetStartPosition(void)
{    
    return ipv4StartPosition;
}


uint16_t IPV4_GetDatagramLength(void)
{
    return ((ipv4Header.length) - sizeof(ipv4Header_t));
}

