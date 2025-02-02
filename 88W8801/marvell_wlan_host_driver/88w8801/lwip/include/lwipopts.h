#ifndef _LWIP_LWIPOPTS_
#define _LWIP_LWIPOPTS_
#include "88w8801/88w8801.h"

// Disable critical section protection when allocating memory
#define SYS_LIGHTWEIGHT_PROT 0

// No operating system
#define NO_SYS 1

// For 32-bit CPUs, 4-byte alignment should be used
#define MEM_ALIGNMENT 4

// Memory heap array size
#define MEM_SIZE 0x4000

// Enable IPv4
#define LWIP_IPV4 1

// Enable DHCP Client
#define LWIP_DHCP 1

// Enable DNS
#define LWIP_DNS 1

// Enable TCP Selective Acknowledgement
#define LWIP_TCP_SACK_OUT 1

// TCP segment size
#define TCP_MSS 1460

// Enable Host Name Settings
#define LWIP_NETIF_HOSTNAME 1

// Disable Netconn
#define LWIP_NETCONN 0

// Disabling Sockets
#define LWIP_SOCKET 0

// Disable statistics collection
#define LWIP_STATS 0

// Enable DHCP Server
#define LWIP_DHCPD 1
#if !defined LWIP_DHCPD || defined __DOXYGEN__
#define LWIP_DHCPD 0
#else
#define LWIP_IP_ACCEPT_UDP_PORT(port) (port) == PP_NTOHS(LWIP_IANA_PORT_DHCP_SERVER)
#endif
#if !LWIP_IPV4
#undef LWIP_DHCPD
#define LWIP_DHCPD 0
#endif

// Configuring NAT
#if !defined LWIP_NAT || defined __DOXYGEN__
#define LWIP_NAT 0
#endif
#if !LWIP_IPV4
#undef LWIP_NAT
#define LWIP_NAT 0
#endif

// Adding relevant timeouts
#define MEMP_NUM_SYS_TIMEOUT (LWIP_NUM_SYS_TIMEOUT_INTERNAL + LWIP_NAT)
#endif
