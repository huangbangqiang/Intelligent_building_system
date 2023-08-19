#ifndef	_W5300_CONFIG_H_
#define	_W5300_CONFIG_H_

#include "wizchip_conf.h"
#ifdef __cplusplus
extern "C" {
#endif

#define ETHERNET_BUF_MAX_SIZE (1024 * 2)
#define SOCKET_DHCP 0
#define SOCKET_DNS 1
#define SOCKET_MQTT 2
#define DHCP_RETRY_COUNT 5
#define DNS_RETRY_COUNT 5

typedef struct MQTTCONNECTION
{
   char mqttHostUrl[1024];  
   int port;   
   char clientid[1024];   
   char username[1024];   
   char passwd[1024]; 
   uint8_t server_ip[4];
   char pubtopic[255];
   char subtopic[255];
}mqttconn;
void network_initialize(wiz_NetInfo net_info);
void print_network_information(wiz_NetInfo net_info);
void Reset_W5300(void);
void dhcp_assign(void);
void dhcp_init(void);
int do_dhcp(void);
void w5300_init(void);
void do_dns(void);
void dns_init(void);
void aliyun_init(void);
void keep_aliyun(void);
int send_aliyun(char* message);
// void do_cmd(void);
int json_decode(char *msg);
#ifdef __cplusplus
}
#endif

#endif
