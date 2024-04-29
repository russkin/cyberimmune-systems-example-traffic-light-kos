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
#define STATE_REQUEST -1
#define STATE_UNKNOWN -2

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KYEL  "\x1B[33m"
#define KGRN  "\x1B[32m"


void print_color(bool state, bool blink, char* color)
{
    if (state && blink)
        fprintf(stderr, "%s(░)", color);
    else if (state)
        fprintf(stderr, "%s(█)", color);
    else
        fprintf(stderr, "%s( )", color);
}

void print_ligts(int state)
{
    fprintf(stderr, "│");
    print_color(state & 0x1, state & 0x8, KRED);
    fprintf(stderr, "%s│", KNRM);
    print_color(state & 0x2, state & 0x8, KYEL);
    fprintf(stderr, "%s│", KNRM);
    print_color(state & 0x4, state & 0x8, KGRN);
    fprintf(stderr, "%s│", KNRM);
}

void print_state(int state)
{
    fprintf(stderr, "┌───┬───┬───┐  ┌───┬───┬───┐\n");
    print_ligts(state);
    fprintf(stderr, "  ");
    print_ligts(state >> 8);
    fprintf(stderr, "\n└───┴───┴───┘  └───┴───┴───┘\n");
}

int main(void)
{
    /* Initialisation of network interface "en0". */
    if (!configure_net_iface(DEFAULT_INTERFACE, DEFAULT_ADDR, DEFAULT_MASK,
                DEFAULT_GATEWAY, DEFAULT_MTU))
    {
        perror("can not init network");
        return EXIT_FAILURE;
    }

    int prev_state = STATE_UNKNOWN;
    while(1)
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

        int state = STATE_REQUEST;
        /* Send data to server. */
        if (-1 == send(clientSocketFD, &state, sizeof(state),0))
        {
            perror("send failed");
            close(clientSocketFD);
            return EXIT_FAILURE;
        }

        /* Recieve data from server. */
        if (-1 == recv(clientSocketFD, &state, sizeof(state),0))
        {
            perror("recv failed");
            close(clientSocketFD);
            return EXIT_FAILURE;
        }

        /* Print sended to server data in terminal. */
        // fprintf(stderr, "Current state: %0x\n", (int)state);
        if (state != prev_state)
        {
            print_state(state);
            prev_state = state;
        }

        /* Close connection with server. */
        shutdown(clientSocketFD, SHUT_RDWR);
        close(clientSocketFD);
    }

    return EXIT_SUCCESS;
}