#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/wait.h>

void trigger_tshark_capture(int capture_count) {
    pid_t pid = fork();

    if (pid == 0) {

        char filename[50];
        snprintf(filename, sizeof(filename), "captures_nmap/cnmap%d.pcap", capture_count);

        char *args[] = {"tshark", "-i", "any", "-a", "duration:600", "-w", filename, NULL};
        execvp("tshark", args);

        perror("Failed to run Tshark");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {

        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else {

        wait(NULL);
    }
}

int detect_nmap_scan(unsigned char *buffer, int data_size) {
    struct iphdr *iph = (struct iphdr *)buffer;

        if (iph->protocol == 6) {
            struct tcphdr *tcph = (struct tcphdr *)(buffer + iph->ihl * 4);

            if (tcph->syn == 1 && tcph->ack == 0) {
                printf("Potential Nmap SYN scan detected!\n");
                return 1;
        }
        }

    return 0;
}

int main() {
    int sock_raw;
    unsigned char *buffer = (unsigned char *)malloc(65536);
    struct sockaddr saddr;
    int saddr_size = sizeof(saddr);
    int data_size;
    int capture_count = 1;

    sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_raw < 0) {
        perror("Socket Error");
        return 1;
    }

    printf("Listening for packets...\n");

    while (1) {

        data_size = recvfrom(sock_raw, buffer, 65536, 0, &saddr, &saddr_size);
        if (data_size < 0) {
            perror("Recvfrom error, failed to get packets");
            return 1;
        }

        if (detect_nmap_scan(buffer, data_size)) {

            trigger_tshark_capture(capture_count);
            capture_count++;
            sleep(600);
        }
    }

    close(sock_raw);
    free(buffer);
    return 0;
}



