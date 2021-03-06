#include "stdafx.h" //3,4

#define PROTO_TCP 6
#define PROTO_UDP 17
#define PROTO_ARP 2
#define PROTO_ICMP 1
#define ICMPHDR_LEN 8
#define RES_MAC { 0x2c, 0x21, 0x72, 0x93, 0xdf, 0x00 }
#define MID_IN_MAC { 0xfc, 0xaa, 0x14, 0x96, 0x8b, 0x54 }
#define MID_OUT_MAC { 0xd8, 0xfc, 0x93, 0x46, 0x58, 0x70 } //(SRC)REQ_IP/MAC -> MID_OUT_IP/MAC
//#define MID_OUT_IP "192.168.32.57" //ME
#define REQ_MAC { 0x00, 0x26, 0x66, 0x89, 0xbe, 0x1d }									//(DST)MID_OUT_IP/MAC -> REQ_IP/MAC
//#define REQ_IP "192.168.31.2" //BOB_MIL

#define	ETHER_ADDR_LEN		6
#define ETH_P_IP 0x0800

char MID_OUT_IP[24];
char REQ_IP[24];
//#define MID_IN_IP "192.168.31.160" //test

/*
* Structure of a 10Mb/s Ethernet header.
*/
typedef struct	ether_header {
	u_char	ether_dhost[ETHER_ADDR_LEN];
	u_char	ether_shost[ETHER_ADDR_LEN];
	u_short	ether_type;
}ethhdr;

/*
* Structure of a 48-bit Ethernet address.
*/
struct	ether_addr {
	u_char octet[ETHER_ADDR_LEN];
};

/* 4 bytes IP address */
typedef struct ip_address{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header{
	u_char  ihl : 4;
	u_char  ver : 4;        // ver (4 bits) + Internet header length (4 bits)
	u_char  tos;            // Type of service 
	u_short tlen;           // Total length 
	u_short identification; // Identification
	u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
	u_char  ttl;            // Time to live
	u_char  proto;          // Protocol
	u_short crc;            // Header checksum
//	ip_address  saddr;      // Source address
//	ip_address  daddr;      // Destination address
	u_long saddr;
	u_long daddr;
	u_int   op_pad;         // Option + Padding
}iphdr;

/* UDP header*/
typedef struct udp_header{
	u_short sport;          // Source port
	u_short dport;          // Destination port
	u_short len;            // Datagram length
	u_short crc;            // Checksum
}udphdr;

typedef struct {
	uint16_t src_port;
	uint16_t dst_port;
	uint32_t seq;
	uint32_t ack;
	uint8_t  data_offset : 4;  // 4 bits
	uint8_t  not_used : 4;
	uint8_t  flags;
	uint16_t window_size;
	uint16_t checksum;
	uint16_t urgent_p;
} tcp_header_t, tcphdr;

typedef struct
{
	u_long saddr;
	u_long daddr;
	uint8_t reversed;
	uint8_t proto;
	uint16_t tulen;
}pseudo_header;

u_short checksum(unsigned short *   data, int length)
{
	register int                nleft = length;
	register unsigned short     *   w = data;
	register int                sum = 0;
	unsigned short              answer = 0;

	while (nleft > 1)
	{
		sum += *w++;
		
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(unsigned char *)(&answer) = *(unsigned char *)w;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = (~sum & 0xffff);

	return answer;
}

u_short checksum_tu(unsigned short *   data, int length)
{
	register int                nleft = length;
	register unsigned short     *   w = data;
	register int                sum = 0;
	unsigned short              answer = 0;

	while (nleft > 1)
	{
		sum += htons(*w++);

		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(unsigned char *)(&answer) = *(unsigned char *)w;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = (~sum & 0xffff);

	return answer;
}


void req_handling(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
	pcap_t* res_handle = (pcap_t*)args;

	char*ptr = (char*)buffer;
	ethhdr*ethptr = (ethhdr*)ptr;
	iphdr*ipptr;
	tcphdr*tcpptr;
	udphdr*udpptr;
	char*data;
	char url[] = "";
	char temp;


	if (ethptr->ether_type == htons(ETH_P_IP))
	{
		ipptr = (iphdr*)(ptr + sizeof(ethhdr));
	}
	else
	{
		//	printf("IPv6\n");
		return;
	}

	//printf("PROTO : %d\n", ipptr->proto);
	switch (ipptr->proto)
	{
	case PROTO_TCP:
		tcpptr = (tcphdr*)(ptr + sizeof(ethhdr) + ipptr->ihl * 4);
		data = ptr + sizeof(ethhdr)+ipptr->ihl * 4 + tcpptr->data_offset * 4;
		for (int i = 0; i < header->caplen; i++)
		{
			if (buffer[i] == 0x48 && buffer[i + 1] == 0x6f && buffer[i + 2] == 0x73 && buffer[i + 3] == 0x74 && buffer[i + 4] == 0x3a)
			{
				for (int j = i + 5;; j++)
				{
					sprintf(url, "%s%c", url,buffer[j]);
					printf("%s\n", url);
					if (buffer[j] == 0x0d && buffer[j + 1] == 0x0a)
					{
						break;
					}
				}
			}
		}
		break;
	case PROTO_UDP:
		udpptr = (udphdr*)(ptr + sizeof(ethhdr)+ipptr->ihl * 4);
		data = ptr + sizeof(ethhdr)+ipptr->ihl * 4 + udpptr->len;
		break;
	case PROTO_ICMP:
//		printf("len : %d bytes\n", header->len);
		data = ptr + sizeof(ethhdr)+ipptr->ihl * 4 + ICMPHDR_LEN;
		break;
	case PROTO_ARP:
		break;
	default:
		printf("exception\n");
		printf("type : 0x%x\n", ipptr->proto);
	}

	//todo
	//filtering

	//printf("saddr : %u %u\n", ipptr->saddr, inet_addr(REQ_IP));
	if (ipptr->saddr == inet_addr(REQ_IP))
	{ //request packet
		u_char* temp;

		temp = ethptr->ether_shost;
		u_char src_mac_array[6] = MID_OUT_MAC;
		for (int i = 0; i<ETHER_ADDR_LEN; i++)
			temp[i] = src_mac_array[i]; //src change

		temp = ethptr->ether_dhost;
		u_char dst_mac_array[6] = RES_MAC;
		for (int i = 0; i<ETHER_ADDR_LEN; i++)
			temp[i] = dst_mac_array[i]; //dst change

		//printf("src mac : ");
		//for (int i = 0; i < ETHER_ADDR_LEN; i++)
		//	printf("%02x:", src_mac_array[i]);
		//printf("\n");
		//printf("src pkt : ");
		//for (int i = 0; i < ETHER_ADDR_LEN; i++)
		//	printf("%02x:", ethptr->ether_shost[i]);
		//printf("\n");
		//printf("dst mac : ");
		//for (int i = 0; i < ETHER_ADDR_LEN; i++)
		//	printf("%02x:", dst_mac_array[i]);
		//printf("\n");
		//printf("dst pkt : ");
		//for (int i = 0; i < ETHER_ADDR_LEN; i++)
		//	printf("%02x:", ethptr->ether_dhost[i]);
		//printf("\n");
		
		ipptr->saddr = inet_addr(MID_OUT_IP);
		//printf("0x%x\n", ipptr->crc);
		ipptr->crc = 0;
		ipptr->crc = checksum((u_short*)ipptr, ipptr->ihl * 4);
		//printf("0x%x\n", ipptr->crc);



		if (ipptr->proto == PROTO_TCP || ipptr->proto == PROTO_UDP)
		{
			//tcp checksum
			char psh[65536];
			pseudo_header*pshptr = (pseudo_header*)psh;
			pshptr->saddr = ipptr->saddr;
			pshptr->daddr = ipptr->daddr;
			pshptr->reversed = 0;
			pshptr->proto = ipptr->proto;
			if (ipptr->proto == PROTO_TCP)
			{
				//printf("*** tcp ***\n");
				pshptr->tulen = ((htons(ipptr->tlen) - (ipptr->ihl * 4))) >> 8 | ((htons(ipptr->tlen) - (ipptr->ihl * 4))) << 8;
				tcpptr->checksum = 0;
				memcpy(psh + sizeof(pseudo_header), tcpptr, htons(pshptr->tulen));
				//printf("calc checksum : 0x%x\n", tcpptr->checksum = htons(checksum_tu((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen))));
				//tcpptr->checksum = htons(checksum_tu((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen)));
				tcpptr->checksum = checksum((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen));
				//printf("calc csum : 0x%x\n", tcpptr->checksum);
			}
			else
			{
				//printf("*** udp ***\n");
				pshptr->tulen = udpptr->len;
				udpptr->crc = 0;
				memcpy(psh + sizeof(pseudo_header), udpptr, htons(pshptr->tulen));
				//printf("calc checksum : 0x%x\n", udpptr->crc = htons(checksum_tu((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen))));
				//udpptr->crc = htons(checksum_tu((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen)));
				udpptr->crc = checksum((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen));
				//printf("calc csum : 0x%x\n", tcpptr->checksum);
			}
		}

		/* Send down the packet */
		if (pcap_sendpacket(res_handle, buffer, header->len /* size */) != 0)
		{
			fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(res_handle));
			return;
		}
	}
}

void res_handling(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
	pcap_t* req_handle = (pcap_t*)args;

	char*ptr = (char*)buffer;
	ethhdr*ethptr = (ethhdr*)ptr;
	iphdr*ipptr;
	tcphdr*tcpptr;
	udphdr*udpptr;
	char*data;



	if (ethptr->ether_type == htons(ETH_P_IP))
	{
		ipptr = (iphdr*)(ptr + sizeof(ethhdr));
	}
	else
	{
		//	printf("IPv6\n");
		return;
	}

	//printf("PROTO : %d\n", ipptr->proto);
	switch (ipptr->proto)
	{
	case PROTO_TCP:
		tcpptr = (tcphdr*)(ptr + sizeof(ethhdr)+ipptr->ihl * 4);
		data = ptr + sizeof(ethhdr)+ipptr->ihl * 4 + tcpptr->data_offset * 4;
		break;
	case PROTO_UDP:
		udpptr = (udphdr*)(ptr + sizeof(ethhdr)+ipptr->ihl * 4);
		data = ptr + sizeof(ethhdr)+ipptr->ihl * 4 + udpptr->len;
		break;
	case PROTO_ICMP:
		//		printf("len : %d bytes\n", header->len);
		data = ptr + sizeof(ethhdr)+ipptr->ihl * 4 + ICMPHDR_LEN;
		break;
	case PROTO_ARP:
		break;
	default:
		printf("exception\n");
		printf("type : 0x%x\n", ipptr->proto);
	}

	//printf("saddr : %u %u\n", ipptr->saddr, inet_addr(REQ_IP));
	if (ipptr->daddr == inet_addr(MID_OUT_IP))
	{ //request packet
		u_char* temp;

		temp = ethptr->ether_shost;
		u_char src_mac_array[6] = MID_IN_MAC;
		for (int i = 0; i<ETHER_ADDR_LEN; i++)
			temp[i] = src_mac_array[i]; //src change

		temp = ethptr->ether_dhost;
		u_char dst_mac_array[6] = REQ_MAC;
		for (int i = 0; i<ETHER_ADDR_LEN; i++)
			temp[i] = dst_mac_array[i]; //dst change

		//printf("src mac : ");
		//for (int i = 0; i < ETHER_ADDR_LEN; i++)
		//	printf("%02x:", src_mac_array[i]);
		//printf("\n");
		//printf("src pkt : ");
		//for (int i = 0; i < ETHER_ADDR_LEN; i++)
		//	printf("%02x:", ethptr->ether_shost[i]);
		//printf("\n");
		//printf("dst mac : ");
		//for (int i = 0; i < ETHER_ADDR_LEN; i++)
		//	printf("%02x:", dst_mac_array[i]);
		//printf("\n");
		//printf("dst pkt : ");
		//for (int i = 0; i < ETHER_ADDR_LEN; i++)
		//	printf("%02x:", ethptr->ether_dhost[i]);
		//printf("\n");

		//ipptr->saddr = inet_addr(MID_IN_IP);//test
		ipptr->daddr = inet_addr(REQ_IP);
		//printf("0x%x\n", ipptr->crc);
		ipptr->crc = 0;
		ipptr->crc = checksum((u_short*)ipptr, ipptr->ihl * 4);
		//printf("0x%x\n", ipptr->crc);



		if (ipptr->proto == PROTO_TCP || ipptr->proto == PROTO_UDP)
		{
			//tcp checksum
			char psh[65536];
			pseudo_header*pshptr = (pseudo_header*)psh;
			pshptr->saddr = ipptr->saddr;
			pshptr->daddr = ipptr->daddr;
			pshptr->reversed = 0;
			pshptr->proto = ipptr->proto;
			if (ipptr->proto == PROTO_TCP)
			{
				//printf("*** tcp ***\n");
				pshptr->tulen = ((htons(ipptr->tlen) - (ipptr->ihl * 4))) >> 8 | ((htons(ipptr->tlen) - (ipptr->ihl * 4))) << 8;
				tcpptr->checksum = 0;
				memcpy(psh + sizeof(pseudo_header), tcpptr, htons(pshptr->tulen));
				//printf("calc checksum : 0x%x\n", tcpptr->checksum = htons(checksum_tu((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen))));
				//tcpptr->checksum = htons(checksum_tu((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen)));
				tcpptr->checksum = checksum((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen));
				//printf("calc csum : 0x%x\n", tcpptr->checksum);
			}
			else
			{
				//printf("*** udp ***\n");
				pshptr->tulen = udpptr->len;
				udpptr->crc = 0;
				memcpy(psh + sizeof(pseudo_header), udpptr, htons(pshptr->tulen));
				//printf("calc checksum : 0x%x\n", udpptr->crc = htons(checksum_tu((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen))));
				//udpptr->crc = htons(checksum_tu((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen)));
				udpptr->crc = checksum((u_short*)psh, sizeof(pseudo_header)+htons(pshptr->tulen));
				//printf("calc csum : 0x%x\n", tcpptr->checksum);
			}
		}

		/* Send down the packet */
		if (pcap_sendpacket(req_handle, buffer, header->len /* size */) != 0)
		{
			fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(req_handle));
			return;
		}
	}
}

//Thread Routine
DWORD CallerThread(LPVOID lpdwThreadParam)
{ //res_handling func caller
	struct handlezip* hdzip = (struct handlezip*)lpdwThreadParam;
	pcap_loop(hdzip->res_handle, -1, res_handling, (u_char*)hdzip->req_handle);
	return 0;
}

int main(int argc, char **argv)
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	int i = 0;
	int j;
	pcap_t *adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];

	pcap_if_t *alldevsp, *device;
	pcap_t *req_handle, *res_handle;

	char *devname, devs[100][100];
	int count = 1, n;


	/* Retrieve the device list on the local machine */
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	/* Print the list */
	for (d = alldevs; d; d = d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	j = i;
	if (i == 0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}

	printf("Enter the interface number (1-%d):", i);
	scanf_s("%d", &inum, sizeof(inum));

	if (inum < 1 || inum > i)
	{
		printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
	for (d = alldevs, i = 0; i< inum - 1; d = d->next, i++);

	/* Open the device */
	if ((req_handle = pcap_open(d->name,          // name of the device
		65536,            // portion of the packet to capture
		// 65536 guarantees that the whole packet will be captured on all the link layers
		PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
		-1,             // read timeout
		NULL,             // authentication on the remote machine
		errbuf            // error buffer
		)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	//////////////////////////////////////////////////
	printf("Enter the interface number (1-%d):", j);
	scanf_s("%d", &inum, sizeof(inum));

	if (inum < 1 || inum > j)
	{
		printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
	for (d = alldevs, j = 0; j< inum - 1; d = d->next, j++);

	/* Open the device */
	if ((res_handle = pcap_open(d->name,          // name of the device
		65536,            // portion of the packet to capture
		// 65536 guarantees that the whole packet will be captured on all the link layers
		PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
		-1,             // read timeout
		NULL,             // authentication on the remote machine
		errbuf            // error buffer
		)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	//printf("\nlistening on %s...\n", d->description);

	/* At this point, we don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);

	printf("MID_OUT_IP : ");
	scanf_s("%s", MID_OUT_IP, 24);
	printf("REQ_IP : ");
	scanf_s("%s", REQ_IP, 24);

	struct handlezip hdzip;
	hdzip.req_handle = req_handle;
	hdzip.res_handle = res_handle;

	DWORD dwThreadId;

	if (CreateThread(NULL, //Choose default security
		0, //Default stack size
		(LPTHREAD_START_ROUTINE)&CallerThread,
		//Routine to execute
		(LPVOID)&hdzip, //Thread parameter
		0, //Immediately run the thread
		&dwThreadId //Thread Id
		) == NULL)
	{
		printf("Error Creating Thread\n");
		return(1);
	}

	pcap_loop(req_handle, -1, req_handling, (u_char*)res_handle);

	return 0;
}
