#include "stm32f1xx_hal.h"                  // Device header
#include "dhcp.h"
#include "dns.h"
#include "w5300_config.h"
#include <stdio.h>
#include "w5300.h"
#include "gpio.h"
#include "dns.h"
#include "mqtt_interface.h"
#include "MQTTClient.h"
#include <string.h>
#include "relay.h"
#include "bsp_flash.h"
#include "cJSON.h"

#define MQTT_SEND_BUFF_SIZE 2048
#define MQTT_RECV_BUFF_SIZE 2048

extern uint32_t send_time;

wiz_NetInfo net_info = {
    .mac = {0x00, 0x08, 0xaa, 0xcc, 0xff, 0xee},
    .ip = {192, 168, 1, 120},
    .sn = {255, 255, 255, 0},
    .gw = {192, 168, 1, 1},
    .dns = {8, 8, 8, 8},
    .dhcp = NETINFO_DHCP};

		
		
		

uint8_t dhcp_get_ip_flag = 0;
uint8_t dns_get_ip_flag = 0;
uint8_t dns_target_ip[4] = {
	0,
};
uint8_t ethernet_buf[ETHERNET_BUF_MAX_SIZE] = {
    0,
};
uint8_t retval = 0;
uint8_t dhcp_retry = 0;
uint8_t dns_retry = 0;
mqttconn aliyun = {
	.clientid = "ieojDDNl2aE.W5300|securemode=2,signmethod=hmacsha256,timestamp=1691025613168|",
	.username = "W5300&ieojDDNl2aE",
	.passwd = "a24bfc25b18076945fe1d9d6b6d2670eb2275db632d8aa804879af28e7d9bceb",
	.mqttHostUrl = "iot-06z00dbroeg8dx3.mqtt.iothub.aliyuncs.com",
	.port = 1883,
	.server_ip = {0,0,0,0},
	.pubtopic = "/sys/ieojDDNl2aE/W5300/thing/event/property/post",
	.subtopic = "/sys/ieojDDNl2aE/W5300/thing/service/property/set"
};
uint8_t mqtt_send_buff[MQTT_SEND_BUFF_SIZE] = {0};
uint8_t mqtt_recv_buff[MQTT_RECV_BUFF_SIZE] = {0};
MQTTClient c = {0};
Network n = {0};
int connOK;
char *cmd;
char recv_flag = 0;

void network_initialize(wiz_NetInfo net_info)
{
    ctlnetwork(CN_SET_NETINFO, (void *)&net_info);
}

void print_network_information(wiz_NetInfo net_info)
{
    uint8_t tmp_str[8] = {
        0,
    };

    ctlnetwork(CN_GET_NETINFO, (void *)&net_info);
    ctlwizchip(CW_GET_ID, (void *)tmp_str);

    if (net_info.dhcp == NETINFO_DHCP)
    {
        printf("====================================================================================================\r\n");
        printf(" %s network configuration : DHCP\r\n", (char *)tmp_str);
    }
    else
    {
        printf("====================================================================================================\r\n");
        printf(" %s network configuration : static\r\n", (char *)tmp_str);
    }

    printf(" MAC         : %02X:%02X:%02X:%02X:%02X:%02X\r\n", net_info.mac[0], net_info.mac[1], net_info.mac[2], net_info.mac[3], net_info.mac[4], net_info.mac[5]);
    printf(" IP          : %d.%d.%d.%d\r\n", net_info.ip[0], net_info.ip[1], net_info.ip[2], net_info.ip[3]);
    printf(" Subnet Mask : %d.%d.%d.%d\r\n", net_info.sn[0], net_info.sn[1], net_info.sn[2], net_info.sn[3]);
    printf(" Gateway     : %d.%d.%d.%d\r\n", net_info.gw[0], net_info.gw[1], net_info.gw[2], net_info.gw[3]);
    printf(" DNS         : %d.%d.%d.%d\r\n", net_info.dns[0], net_info.dns[1], net_info.dns[2], net_info.dns[3]);
    printf("====================================================================================================\r\n");
}

void Reset_W5300(void)
{
	HAL_GPIO_WritePin(RST_GPIO_Port,RST_Pin,GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(RST_GPIO_Port,RST_Pin,GPIO_PIN_SET);
	HAL_Delay(1600);
}

void w5300_init(void)
{
	Reset_W5300();
	if(WIZCHIP_READ(VERSIONR)!= 0x5300)
	{
		printf("ACCESS ERR:VERSION != 0x04,read value = 0x%02x\r\n",WIZCHIP_READ(VERSIONR));
		while(1)
		{
			HAL_Delay(100);
		}
	}
	while(1)
	{
		if(HAL_GPIO_ReadPin(LINK_STATUS_GPIO_Port,LINK_STATUS_Pin) == 0)
		{
			printf("PHY link connected\r\n");
			break;
		}
		else
		{
			printf("PHY link down\r\n");
			HAL_Delay(1000);
		}
	}
}

void dhcp_conflict(void)
{
    printf("CONFLICT IP from DHCP\r\n");
    while (1)
        ;
}

void dhcp_assign(void)
{
    getIPfromDHCP(net_info.ip);
    getGWfromDHCP(net_info.gw);
    getSNfromDHCP(net_info.sn);
    getDNSfromDHCP(net_info.dns);

    net_info.dhcp = NETINFO_DHCP;

    network_initialize(net_info);
    print_network_information(net_info);
    printf("DHCP LEASED TIME:%d Sec.\r\n", getDHCPLeasetime());
}
void dhcp_init(void)
{
	if(net_info.dhcp == NETINFO_DHCP){
		setSHAR(net_info.mac);
    printf("DHCP client running \r\n");
    DHCP_init(SOCKET_DHCP, ethernet_buf);
    reg_dhcp_cbfunc(dhcp_assign, dhcp_assign, dhcp_conflict);
	}
	else{
		network_initialize(net_info);
		print_network_information(net_info);
	}
}

int do_dhcp(void)
{
    
    if (net_info.dhcp == NETINFO_DHCP)
    {
        retval = DHCP_run();
        if (retval == DHCP_IP_LEASED)
        {
            if (dhcp_get_ip_flag == 0)
            {
                printf("DHCP success\r\n");
                dhcp_get_ip_flag = 1;
								return 1;
            }
        }
        else if (retval == DHCP_FAILED)
        {
            dhcp_get_ip_flag = 0;
            dhcp_retry++;
            if (dhcp_retry <= DHCP_RETRY_COUNT)
            {
                printf(" DHCP timeout occurred and retry %d \r\n", dhcp_retry);
            }
        }

        if (dhcp_retry > DHCP_RETRY_COUNT)
        {
            printf(" DHCP failed \r\nset default netinfo");
            DHCP_stop();
            return -1;
        }
    }
		return 0;
}

void dns_init(void)
{
	DNS_init(SOCKET_DNS,ethernet_buf);
}
void do_dns(void)
{
	if((dns_get_ip_flag == 0) && (retval == DHCP_IP_LEASED))
	{
		while(1)
		{
			if(DNS_run(net_info.dns,(uint8_t*)aliyun.mqttHostUrl,aliyun.server_ip) > 0)
			{
				printf("DNS success\r\n");
				printf("Target domain:%s\r\n",aliyun.mqttHostUrl);
				printf("IP of target domain:%d.%d.%d.%d\r\n",aliyun.server_ip[0],aliyun.server_ip[1],aliyun.server_ip[2],aliyun.server_ip[3]);
				dns_get_ip_flag = 1;
				aliyun_init();
				break;
			}
			else
			{
				dns_retry ++;
				if(dns_retry <= DNS_RETRY_COUNT)
				{
					printf("DNS timeout occurred and retry %d\r\n",dns_retry);
				}
			}
			if(dns_retry > DNS_RETRY_COUNT)
			{
				printf("DNS failed\r\n");
				while(1)
					;
			}
			HAL_Delay(1000);
		}
	}
}

void messageArrived(MessageData* md)
{
	char testbuffer[2048];
	MQTTMessage* message = md->message;

	if (0)
	{
		memcpy(testbuffer,(char*)message->payload,(int)message->payloadlen);
		testbuffer[message->payloadlen + 1] = '\n';
		printf("%s\r\n",testbuffer);
	}

	if (0)
	{
		printf("%.*s", (int)message->payloadlen, (char*)message->payload);
	}
	else
	{	
		printf("%.*s%s",(int)message->payloadlen, (char*)message->payload, "\r\n");
	}
	json_decode((char*)message->payload);
}

void aliyun_init(void)
{
	if((dns_get_ip_flag == 1) && (retval == DHCP_IP_LEASED))
	{
		int ret;
		
		MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
		NewNetwork(&n, SOCKET_MQTT);
		ConnectNetwork(&n, aliyun.server_ip, 1883);
		MQTTClientInit(&c, &n, 1000, mqtt_send_buff, MQTT_SEND_BUFF_SIZE, mqtt_recv_buff, MQTT_RECV_BUFF_SIZE);
		data.willFlag = 0;
		data.MQTTVersion = 3;
		data.clientID.cstring = aliyun.clientid;
		data.username.cstring = aliyun.username;
		data.password.cstring = aliyun.passwd;
		data.keepAliveInterval = 30;
		data.cleansession = 1;
		connOK = MQTTConnect(&c, &data);
		printf("Connected:%s\r\n", connOK==0?"success":"failed");
		printf("Subscribing to %s\r\n",aliyun.subtopic);
		ret = MQTTSubscribe(&c, aliyun.subtopic, QOS1, messageArrived);
		printf("Subscribed:%s\r\n", ret==0?"success":"failed");
		if(connOK != 0)
		{
			Reset_W5300();
		}
		send_time = 0;
	}
}

void keep_aliyun(void)
{
	if((dns_get_ip_flag == 1) && (retval == DHCP_IP_LEASED) && !connOK)
		MQTTYield(&c, 30);
}
int send_aliyun(char* message)
{
	int ret;
if((dns_get_ip_flag == 1) && (retval == DHCP_IP_LEASED) && !connOK)
	{	
		int send_retry_count = 0;
		MQTTMessage pubmessage={
				.qos = QOS1,
				.retained = 0,
				.dup = 0,
				.id = 0,
			};
		pubmessage.payload = message;
		pubmessage.payloadlen = strlen(message);
			ret = MQTTPublish(&c,aliyun.pubtopic,&pubmessage);
		while(ret != 0)
		{

			ret = MQTTPublish(&c,aliyun.pubtopic,&pubmessage);
			send_retry_count++;
			if(send_retry_count > 3)
			{
				aliyun_init();
				break;
			}
			HAL_Delay(1000);
		}
	}
	return ret;
}

int json_decode(char *msg)
{
	cJSON* jsondata = NULL;
	cJSON* params = NULL;
	cJSON* Relay = NULL;
	cJSON* Warning_value = NULL;
	char *Relay_name[] = {"Relay1","Relay2","Relay3","Relay4","Relay5","Relay6"};
	char *Warning_name[] = {"MQ4_1_Warning_value","MQ4_2_Warning_value","MQ4_3_Warning_value"};
	char **temp = NULL;
	char tempstr[256];
	jsondata = cJSON_Parse(msg);
	if(jsondata == NULL)
	{
		printf("parse fail.\n");
		return -1;
	}
	params = cJSON_GetObjectItem(jsondata,"params");
	if(strstr(msg,"Relay"))
	{
		temp = Relay_name;
		for (size_t i = 0; i < 6; i++)
		{
			Relay = cJSON_GetObjectItem(params,*temp);
			if(Relay == NULL)
				temp++;
			else
			{
				if(!strcmp(Relay->valuestring,"1"))
				{
					if(!strcmp(*temp,"Relay1"))
						relay_on(RELAY1_Pin);
					else if(!strcmp(*temp,"Relay2"))
						relay_on(RELAY2_Pin);
					else if(!strcmp(*temp,"Relay3"))
						relay_on(RELAY3_Pin);
					else if(!strcmp(*temp,"Relay4"))
						relay_on(RELAY4_Pin);
					else if(!strcmp(*temp,"Relay5"))
						relay_on(RELAY5_Pin);
					else if(!strcmp(*temp,"Relay6"))
						relay_on(RELAY6_Pin);
					break;
				}
				if(!strcmp(Relay->valuestring,"0"))
				{
					if(!strcmp(*temp,"Relay1"))
						relay_off(RELAY1_Pin);
					else if(!strcmp(*temp,"Relay2"))
						relay_off(RELAY2_Pin);
					else if(!strcmp(*temp,"Relay3"))
						relay_off(RELAY3_Pin);
					else if(!strcmp(*temp,"Relay4"))
						relay_off(RELAY4_Pin);
					else if(!strcmp(*temp,"Relay5"))
						relay_off(RELAY5_Pin);
					else if(!strcmp(*temp,"Relay6"))
						relay_off(RELAY6_Pin);
					break;
				}
			}
			
		}
	}
	if(strstr(msg,"Warning_value"))
	{
		temp = Warning_name;
		uint16_t tempnumber;
		for (size_t i = 0; i < 6; i++)
		{
			Warning_value = cJSON_GetObjectItem(params,*temp);
			if(Warning_value == NULL)
				temp++;
			else
			{
				tempnumber = Warning_value->valueint;
				if(!strcmp(*temp,"MQ4_1_Warning_value"))
				{
					Flash_PageErase(MQ4_1_ADDR);
					STMFLASH_Write(MQ4_1_ADDR,&tempnumber,2);
					sprintf(tempstr,"{id:1,params:{MQ4_1_Warning_value:%d},version:\"1.0\",method:\"thing.event.property.post\"}",Warning_value->valueint);
					send_aliyun(tempstr);
				}
				else if(!strcmp(*temp,"MQ4_2_Warning_value"))
				{
					Flash_PageErase(MQ4_2_ADDR);
					STMFLASH_Write(MQ4_2_ADDR,&tempnumber,2);
					sprintf(tempstr,"{id:1,params:{MQ4_2_Warning_value:%d},version:\"1.0\",method:\"thing.event.property.post\"}",Warning_value->valueint);
					send_aliyun(tempstr);
				}
				else if(!strcmp(*temp,"MQ4_3_Warning_value"))
				{
					Flash_PageErase(MQ4_3_ADDR);
					STMFLASH_Write(MQ4_3_ADDR,&tempnumber,2);
					sprintf(tempstr,"{id:1,params:{MQ4_3_Warning_value:%d},version:\"1.0\",method:\"thing.event.property.post\"}",Warning_value->valueint);
					send_aliyun(tempstr);
				}
				break;
			}
		}
	}
	cJSON_Delete(jsondata);
	return 0;
}
