#ifndef TCPIP_CONFIG_H
#define TCPIP_CONFIG_H

#include<stdint.h>
/**
  Section: Included Files
*/

/**
  Section: Macro Declarations
*/

/* Build the IPv4 Address*/
#define MAKE_IPV4_ADDRESS(a,b,c,d) ((uint32_t)(((uint32_t)a << 24) | ((uint32_t)b<<16) | ((uint32_t)c << 8) | (uint32_t)d))

/******************************** ARP Protocol Defines *********************************/
#define ARP_MAP_SIZE 8



/******************************** IP Protocol Defines ********************************/
#define IPv4_TTL            64u

/******************************** TCP Protocol Defines *********************************/
// Define the maximum segment size for the 
#define TCP_MAX_SEG_SIZE    1460u
#define TICK_SECOND 1u

// TCP Timeout and retransmit numbers
#define TCP_START_TIMEOUT_VAL           ((uint32_t)TICK_SECOND*2)  // Timeout to retransmit unacked data

#define TCP_MAX_RETRIES                 (5u)                // Maximum number of retransmission attempts
#define TCP_MAX_SYN_RETRIES             (3u)                // Smaller than all other retries to reduce SYN flood DoS duration

#define LOCAL_TCP_PORT_START_NUMBER     (1024u)             // define the lower port number to be used as a local port
#define LOCAL_TCP_PORT_END_NUMBER       (65535u)            // define the highest port number to be used as a local port


/******************************** TCP/IP stack debug Defines *********************************/

//#define ENABLE_TCP_DEBUG
//#define ENABLE_IP_DEBUG
//#define ENABLE_NET_DEBUG

#endif  /* TCPIP_CONFIG_H */