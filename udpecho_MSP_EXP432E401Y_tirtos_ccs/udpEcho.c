#include "ph001.h"  // Your main header

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
/* BSD support */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>

#include <ti/net/slnetutils.h>

#define UDPPACKETSIZE 1472
#define MAXPORTLEN    6
#define DEFAULT_PORT  1000  // Default listening port if none is specified

extern void fdOpenSession();
extern void fdCloseSession();
extern void *TaskSelf();

// Multicast definitions if needed
typedef struct _my_ip_mreq {
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
} my_ip_mreq;

// If you had a matchsub or VoiceParse, they should already be defined elsewhere
#define DEFAULT_NET_PORT 100;
char *UDPParse(char *buff, struct sockaddr_in *clientAddr, bool todash) {
    char *StrBufPTR = buff;
    char *colon;
    int32_t AddrByte;
    uint32_t PortWord = DEFAULTPORT; // Use your default port defined as DEFAULTPORT

    if(!StrBufPTR) return NULL;

    // Trim leading spaces
    while (isspace((int)*StrBufPTR)) StrBufPTR++;

    // Check for special keywords: localhost, broadcast, nobody
    if (strstr(StrBufPTR, "localhost") != NULL) {
        clientAddr->sin_addr.s_addr = 0x0100007F; // 127.0.0.1
        StrBufPTR = strstr(StrBufPTR, "localhost");
        StrBufPTR += strlen("localhost");
        goto coloncheck;
    } else if (strstr(StrBufPTR, "broadcast") != NULL) {
        clientAddr->sin_addr.s_addr = 0xFFFFFFFF;
        StrBufPTR = strstr(StrBufPTR, "broadcast");
        StrBufPTR += strlen("broadcast");
        goto coloncheck;
    } else if (strstr(StrBufPTR, "nobody") != NULL) {
        clientAddr->sin_addr.s_addr = 0x00000000;
        StrBufPTR = strstr(StrBufPTR, "nobody");
        StrBufPTR += strlen("nobody");
        goto coloncheck;
    } else {
        // Parse numeric IP: a.b.c.d
        // Extract first segment
        StrBufPTR++;
        StrBufPTR++;
        StrBufPTR++;
        StrBufPTR++;
        StrBufPTR++;
        StrBufPTR++;
        StrBufPTR++;
        StrBufPTR++;
        while(isspace((int)*StrBufPTR)) StrBufPTR++;
        if(!isdigit((int)*StrBufPTR)) {
            enqueueMessage("-print UDPParse: Missing digits for IP");
            return NULL;
        }
        AddrByte = atoi(StrBufPTR);

        char *dot = strchr(StrBufPTR, '.');
        if(!dot) return NULL;
        StrBufPTR = dot+1;
        if(!isdigit((int)*StrBufPTR)) {
            enqueueMessage("-print UDPParse: Missing digits after first segment");
            return NULL;
        }
        clientAddr->sin_addr.s_addr = AddrByte;

        // Second segment
        AddrByte = atoi(StrBufPTR);
        dot = strchr(StrBufPTR, '.');
        if(!dot) return NULL;
        clientAddr->sin_addr.s_addr |= AddrByte << 8;
        StrBufPTR = dot+1;
        if(!isdigit((int)*StrBufPTR)) {
            enqueueMessage("-print UDPParse: Missing digits after second segment");
            return NULL;
        }

        // Third segment
        AddrByte = atoi(StrBufPTR);
        dot = strchr(StrBufPTR, '.');
        if(!dot) return NULL;
        clientAddr->sin_addr.s_addr |= AddrByte << 16;
        StrBufPTR = dot+1;
        if(!isdigit((int)*StrBufPTR)) {
            enqueueMessage("-print UDPParse: Missing digits after third segment");
            return NULL;
        }

        // Fourth segment
        AddrByte = atoi(StrBufPTR);
        clientAddr->sin_addr.s_addr |= AddrByte << 24;
        // Move StrBufPTR ahead until space or colon
        while(*StrBufPTR && *StrBufPTR != ' ' && *StrBufPTR != ':') StrBufPTR++;
    }

coloncheck:
    colon = strchr(StrBufPTR, ':');
    if (!colon) {
        // no colon means no custom port, use default
        char *space = strchr(StrBufPTR, ' ');
        StrBufPTR = space ? space : NULL;
    } else {
        StrBufPTR = colon + 1;
        while(isspace((int)*StrBufPTR)) StrBufPTR++;
        if(!isdigit((int)*StrBufPTR)) {
            enqueueMessage("-print UDPParse: Missing digits for port");
            return NULL;
        }
        PortWord = atoi(StrBufPTR);
        while(isdigit((int)*StrBufPTR)) StrBufPTR++;
    }

    clientAddr->sin_port = htons((uint16_t)PortWord);
    clientAddr->sin_family = AF_INET;

    // Skip spaces
    while(StrBufPTR && isspace((int)*StrBufPTR))
        StrBufPTR++;

    if(todash && StrBufPTR) {
        char *dash = strchr(StrBufPTR, '-');
        if(dash) StrBufPTR = dash;
    }

    return StrBufPTR;
}



// Forward declare TransmitFxn
void *TransmitFxn(void *arg0);

// ListenFxn: Listens for incoming UDP messages on a specified port
void *ListenFxn(void *arg0) {
    int                bytesRcvd;
    int                status;
    int                server = -1;
    fd_set             readSet;
    struct addrinfo    hints;
    struct addrinfo    *res = NULL, *p;
    struct sockaddr_in clientAddr;
    socklen_t          addrlen;
    char               buffer[UDPPACKETSIZE];
    char               portNumber[MAXPORTLEN];
    char               message[128];
    uint16_t           listenPort = (arg0) ? *((uint16_t *)arg0) : DEFAULTPORT;

    fdOpenSession(TaskSelf());

    snprintf(portNumber, sizeof(portNumber), "%d", listenPort);
    snprintf(message, sizeof(message), "-print UDP Listen started on port %s", portNumber);
    enqueueMessage(message);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags    = AI_PASSIVE;

    status = getaddrinfo(NULL, portNumber, &hints, &res);
    if (status != 0) {
        snprintf(message, sizeof(message), "-print Error: getaddrinfo failed: %s", gai_strerror(status));
        enqueueMessage(message);
        goto shutdown;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        server = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server == -1) continue;

        status = bind(server, p->ai_addr, p->ai_addrlen);
        if (status != -1) break;

        close(server);
    }

    if (server == -1) {
        enqueueMessage("-print Error: socket not created");
        goto shutdown;
    } else if (p == NULL) {
        enqueueMessage("-print Error: bind failed");
        goto shutdown;
    } else {
        freeaddrinfo(res);
        res = NULL;
    }

    while (1) {
        FD_ZERO(&readSet);
        FD_SET(server, &readSet);
        addrlen = sizeof(clientAddr);

        status = select(server + 1, &readSet, NULL, NULL, NULL);
        if (status > 0) {
            if (FD_ISSET(server, &readSet)) {
                bytesRcvd = (int)recvfrom(server, buffer, UDPPACKETSIZE, 0,
                                          (struct sockaddr *)&clientAddr, &addrlen);

                if (bytesRcvd > 0) {
                    buffer[bytesRcvd] = '\0';

                    // Check if message contains "-voice"
                    if (matchsub("-voice", buffer)) {
                        // Handle voice command
                        VoiceParse(buffer);
                    } else {
                        // Construct the UDP received message info
                        char MsgBuff[320];
                        sprintf(MsgBuff, "UDP %d.%d.%d.%d> %s",
                                (uint8_t)(clientAddr.sin_addr.s_addr & 0xFF),
                                (uint8_t)((clientAddr.sin_addr.s_addr >> 8) & 0xFF),
                                (uint8_t)((clientAddr.sin_addr.s_addr >> 16) & 0xFF),
                                (uint8_t)((clientAddr.sin_addr.s_addr >> 24) & 0xFF),
                                buffer);

                        // Print the message to the console
                        char printMessage[320];
                        snprintf(printMessage, sizeof(printMessage), "-print %s", MsgBuff);
                        enqueueMessage(printMessage);

                        // If you want this received message to be processed by your command parser,
                        // enqueue the raw buffer as well:
                        enqueueMessage(buffer);
                    }
                }
            }
        }

    }

shutdown:
    if (res) freeaddrinfo(res);
    if (server != -1) close(server);
    fdCloseSession(TaskSelf());
    return NULL;
}


// TransmitFxn: Sends queued messages (from Glo.NetOutQ) over UDP to specified destinations
void *TransmitFxn(void *arg0) {
    int bytesSent;
    int status;
    int server = -1;
    fd_set writeSet;
    struct sockaddr_in clientAddr;
    socklen_t addrlen;
    char* StrBufPTR;
    int allow_broadcast = 1;
    uint32_t gateKey;
    uint32_t payloadnext;
    int bytesRequested;

    fdOpenSession(TaskSelf());
    enqueueMessage("-print UDP Transmit started");

    // Create a UDP socket for sending
    server = socket(AF_INET, SOCK_DGRAM, 0);
    if (server == -1) {
        enqueueMessage("-print Error: Transmit socket not created");
        goto shutdown;
    }

    status = setsockopt(server, SOL_SOCKET, SO_BROADCAST, &allow_broadcast, sizeof(allow_broadcast));
    if (status < 0) {
        enqueueMessage("-print Error: setsockopt SO_BROADCAST failed");
    }

    // Bind to ephemeral port
    struct sockaddr_in bindAddr;
    memset(&bindAddr, 0, sizeof(bindAddr));
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindAddr.sin_port = 0; // ephemeral port
    if (bind(server, (struct sockaddr *)&bindAddr, sizeof(bindAddr)) < 0) {
        enqueueMessage("-print Error: bind in TransmitFxn failed");
        goto shutdown;
    }

    while (1) {
        FD_ZERO(&writeSet);
        FD_SET(server, &writeSet);
        addrlen = sizeof(clientAddr);

        // Wait until something is queued in NetOutQ
        Semaphore_pend(semaphore2, BIOS_WAIT_FOREVER);

        // Typically UDP send doesn't need select, but we do it to mirror example logic
        status = select(server + 1, NULL, &writeSet, NULL, NULL);
        if (status <= 0) {
            enqueueMessage("-print Error: Select Server Failed");
            continue;
        }

        GateSwi_enter(gateSwi3);
        char *payload = Glo.NetOutQ.payloads[Glo.NetOutQ.payloadReading];
        int binaryCount = Glo.NetOutQ.binaryCount[Glo.NetOutQ.payloadReading];
        GateSwi_leave(gateSwi3, 0);

        // Parse IP/Port
        StrBufPTR = UDPParse(payload, &clientAddr, true);
        if (!StrBufPTR) {
            enqueueMessage("-print UDP Parse Failed");
            // Advance queue index
            gateKey = GateSwi_enter(gateSwi3);
            payloadnext = Glo.NetOutQ.payloadReading + 1;
            if(payloadnext >= NetQueueLen) payloadnext = 0;
            Glo.NetOutQ.payloadReading = payloadnext;
            GateSwi_leave(gateSwi3, gateKey);
            continue;
        }

        // Calculate how many bytes to send: remainder command + binaryCount
        bytesRequested = (int)strlen(StrBufPTR) + 1 + binaryCount;
        bytesSent = (int)sendto(server, StrBufPTR, bytesRequested, 0, (struct sockaddr *)&clientAddr, addrlen);

        if (bytesSent < 0 || bytesSent != bytesRequested) {
            enqueueMessage("-print Sendto() failed");
        }

        // After sending out, enqueue remainder command for local processing:
        enqueueMessage(StrBufPTR);

        // Advance read pointer
        gateKey = GateSwi_enter(gateSwi3);
        payloadnext = Glo.NetOutQ.payloadReading + 1;
        if(payloadnext >= NetQueueLen)
            payloadnext = 0;
        Glo.NetOutQ.payloadReading = payloadnext;
        GateSwi_leave(gateSwi3, gateKey);
    }

shutdown:
    if (server != -1) close(server);
    fdCloseSession(TaskSelf());
    return NULL;
}


