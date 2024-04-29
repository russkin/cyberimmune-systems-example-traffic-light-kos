#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <kos_net.h>

#define EXAMPLE_HOST "localhost"
#define EXAMPLE_PORT 7777
#define NUM_RETRIES 10

int send_state(int state)
{
    /* Create socket for connection with server. */
    int clientSocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == clientSocketFD)
    {
        perror("cannot create socket");
        return EXIT_FAILURE;
    }

    /* Creating and initialisation of socket`s address structure for connection with server. */
    struct sockaddr_in stSockAddr;
    memset(&stSockAddr, 0, sizeof(stSockAddr));
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(EXAMPLE_PORT);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *resultHints = NULL;
    int res = getaddrinfo(EXAMPLE_HOST, NULL, &hints, &resultHints);
    if (res != 0)
    {
        perror("cannot get address info");
        close(clientSocketFD);
        return EXIT_FAILURE;
    }

    struct sockaddr_in* in_addr = (struct sockaddr_in *)resultHints->ai_addr;
    memcpy(&stSockAddr.sin_addr.s_addr, &in_addr->sin_addr.s_addr, sizeof(in_addr_t));
    freeaddrinfo(resultHints);

    res = -1;
    for (int i = 0; res == -1 && i < NUM_RETRIES; i++)
    {
        sleep(1); // Wait some time for server be ready.
        res = connect(clientSocketFD, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr));
    }

    if (res == -1)
    {
        perror("connect failed");
        close(clientSocketFD);
        return EXIT_FAILURE;
    }

    /* Send data to server. */
    if (-1 == send(clientSocketFD, &state, sizeof(state),0))
    {
        perror("send failed");
        close(clientSocketFD);
        return EXIT_FAILURE;
    }

    /* Close connection with server. */
    shutdown(clientSocketFD, SHUT_RDWR);
    close(clientSocketFD);

    return EXIT_SUCCESS;
}