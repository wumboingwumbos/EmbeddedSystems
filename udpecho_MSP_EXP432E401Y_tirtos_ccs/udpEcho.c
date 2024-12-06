#include "ph001.h"

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
#define DEFAULT_PORT  1000  // Define a default port if none provided

extern void fdOpenSession();
extern void fdCloseSession();
extern void *TaskSelf();

void *echoFxn(void *arg0)
{
    int                bytesRcvd;
    int                bytesSent;
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

    fdOpenSession(TaskSelf());

    // If arg0 is provided as a port number pointer, use it. Otherwise, use default.
    uint16_t listenPort = (arg0) ? *((uint16_t *)arg0) : DEFAULT_PORT;
    snprintf(portNumber, sizeof(portNumber), "%d", listenPort);

    snprintf(message, sizeof(message), "-print UDP Echo started on port %s\r\n", portNumber);
    enqueueMessage(message);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags    = AI_PASSIVE;

    // Resolve address/port for binding
    status = getaddrinfo(NULL, portNumber, &hints, &res);
    if (status != 0) {
        snprintf(message, sizeof(message), "-print Error: getaddrinfo failed: %s\r\n", gai_strerror(status));
        enqueueMessage(message);
        goto shutdown;
    }

    // Attempt to create and bind socket
    for (p = res; p != NULL; p = p->ai_next) {
        server = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server == -1) {
            continue;
        }

        status = bind(server, p->ai_addr, p->ai_addrlen);
        if (status != -1) {
            break; // successfully bound
        }

        close(server);
    }

    if (server == -1) {
        enqueueMessage("-print Error: socket not created\r\n");
        goto shutdown;
    } else if (p == NULL) {
        enqueueMessage("-print Error: bind failed\r\n");
        goto shutdown;
    } else {
        freeaddrinfo(res);
        res = NULL;
    }

    // Ready to listen for incoming packets
    while (1) {
        FD_ZERO(&readSet);
        FD_SET(server, &readSet);
        addrlen = sizeof(clientAddr);

        // Wait for incoming UDP packet
        status = select(server + 1, &readSet, NULL, NULL, NULL);
        if (status > 0 && FD_ISSET(server, &readSet)) {
            bytesRcvd = (int)recvfrom(server, buffer, UDPPACKETSIZE, 0, (struct sockaddr *)&clientAddr, &addrlen);

            if (bytesRcvd > 0) {
                buffer[bytesRcvd] = '\0'; // Null-terminate the received data

                // Here’s where you adapt what you do with the received data.
                // Option A: Echo back the data (original behavior)
                bytesSent = (int)sendto(server, buffer, bytesRcvd, 0, (struct sockaddr *)&clientAddr, addrlen);
                if (bytesSent < 0 || bytesSent != bytesRcvd) {
                    enqueueMessage("-print Error: sendto failed\r\n");
                    // Decide whether to break or continue
                    // break; // if you want to stop on error
                }

                // Option B: Process locally:
                // enqueueMessage(buffer); // Add received message to your local queue

                // If you detect special commands like "-voice" or "-netudp", you could parse them here:
                // if (matchsub("-voice", buffer)) {
                //     VoiceParse(buffer);
                // }
                // else {
                //     enqueueMessage(buffer);
                // }
            }
        } else if (status < 0) {
            enqueueMessage("-print Error: select() failed\r\n");
            // Decide whether to break or continue
            // break;
        }
    }

shutdown:
    if (res) {
        freeaddrinfo(res);
    }

    if (server != -1) {
        close(server);
    }

    fdCloseSession(TaskSelf());

    return (NULL);
}
