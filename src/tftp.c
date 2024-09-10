#include "lwip/sockets.h"
#include "lwip/inet.h"

#include "xil_printf.h"
#include "stdlib.h"
#include "string.h"
#include "boot_config.h"



#define TFTP_PORT 69
#define TFTP_E_PORT 1200
#define RRQ 1
#define WRQ 2
#define DATA 3
#define ACK 4
#define ERROR 5
#define TFTP_MODE "octet"
#define TFTP_MAX_BLOCK_LEN 512

extern void tftp_boot(uint64_t addr);

/*
 *
 */

void start_tftp(ip4_addr_t server, const char *file)
{
	struct sockaddr_in sa;
	int sd;
	uint8_t buffer[1024], *ptr;
	int len, recvlen, bytes, count;

	memset(buffer, 0, 1024);
	sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	memset(&sa, 0, sizeof(sa));
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(TFTP_E_PORT);
	sa.sin_family = AF_INET;
	bind(sd, (struct sockaddr*)&sa, sizeof(sa));
	sa.sin_port = htons(TFTP_PORT);
	sa.sin_addr.s_addr = server.addr;
	xil_printf("requesting file %s from %s\r\n", file, inet_ntoa(server));
	ptr = buffer;
	*(uint16_t*)ptr = htons(1);
	ptr += 2;
	strcpy(ptr, file);
	ptr += strlen(file)+1;
	strcpy(ptr, TFTP_MODE);
	ptr += strlen(TFTP_MODE)+1;
	len = ptr - buffer;
	sendto(sd, buffer, len, 0, (struct sockaddr*)&sa, sizeof(sa));
	recvlen = 516;
	count = 0;
	bytes = 0;
	ptr = (uint8_t*)BOOT_ADDRESS;
	xil_printf("----------\r\nStoring Data at Boot Address 0x%x\r\nreceiving DATA...\r\n", BOOT_ADDRESS);
	//while(recvlen == 516)
	while(recvlen == 516)
	{
		len = sizeof(sa);
		memset(&sa, 0, len);
		recvlen = recvfrom(sd, buffer, 1024, 0, (struct sockaddr*)&sa, &len);
		if(ntohs(*(uint16_t*)buffer) == DATA)
		{
			//xil_printf("Received data %d %s\n", recvlen, buffer[4]);
			xil_printf("Received data %d\n", recvlen);
			if(recvlen - 4 > 0)
			{
				memcpy(ptr, &buffer[4], recvlen-4);
				ptr += recvlen -4;
			}
			bytes += recvlen;
			count++;
			if(count % 10 == 0)
				xil_printf("#%s", count % 300 == 0 ? "\r\n" : "");
			*(uint16_t*)buffer = htons(ACK);
			sendto(sd, buffer, 4, 0, (struct sockaddr*)&sa, sizeof(sa));
		}
	}
	xil_printf("\r\n\r\nreceived %dkB in total\r\n", bytes/1000);
	xil_printf("----------\r\nBooting Application from Address 0x%x\r\n\r\n", BOOT_ADDRESS);
	xil_printf("----------\r\nExiting start_tftp()\r\n\r\n");

	//tftp_boot(BOOT_ADDRESS);

}
