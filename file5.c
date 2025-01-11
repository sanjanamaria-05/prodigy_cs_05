#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
unsigned char sample_packet[] = {
    0x45, 0x00, 0x00, 0x3c, 0x1c, 0x46, 0x40, 0x00, 0x40, 0x06, 0xb1, 0xe6, 0xc0, 0xa8, 0x00, 0x68,
    0xc0, 0xa8, 0x00, 0x01, 0x04, 0xd2, 0x00, 0x50, 0x6a, 0x2b, 0x1b, 0x70, 0x00, 0x00, 0x00, 0x00,
    0x70, 0x02, 0x40, 0x00, 0x72, 0x10, 0x00, 0x00, 0x02, 0x04, 0x05, 0xb4, 0x01, 0x03, 0x03, 0x06,
    0x01, 0x01, 0x08, 0x0a, 0x00, 0x27, 0xd7, 0x29, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00
};
void process_packet(unsigned char *buffer, int size) {
    struct iphdr ip_header = (struct iphdr)buffer;
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip_header->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip_header->daddr;
    printf("Source IP: %s\n", inet_ntoa(source.sin_addr));
    printf("Destination IP: %s\n", inet_ntoa(dest.sin_addr));
    switch (ip_header->protocol) {
        case IPPROTO_TCP: {
            printf("Protocol: TCP\n");
            struct tcphdr tcp_header = (struct tcphdr)(buffer + ip_header->ihl * 4);
            printf("Source Port: %u\n", ntohs(tcp_header->source));
            printf("Destination Port: %u\n", ntohs(tcp_header->dest));
            break;
        }
        case IPPROTO_UDP: {
            printf("Protocol: UDP\n");
            struct udphdr udp_header = (struct udphdr)(buffer + ip_header->ihl * 4);
            printf("Source Port: %u\n", ntohs(udp_header->source));
            printf("Destination Port: %u\n", ntohs(udp_header->dest));
            break;
        }
        case IPPROTO_ICMP:
            printf("Protocol: ICMP\n");
            break;
        default:
            printf("Protocol: Others\n");
            break;
    }
    printf("Payload:\n");
    int ip_header_len = ip_header->ihl * 4;
    unsigned char *payload = buffer + ip_header_len;
    int payload_len = size - ip_header_len;
    for (int i = 0; i < payload_len; i++) {
        printf("%02x ", payload[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n\n");
}
int main() {
    int packet_size = sizeof(sample_packet);
    printf("Starting packet capture simulation...\n");
   
    process_packet(sample_packet, packet_size);
    return 0;
}
