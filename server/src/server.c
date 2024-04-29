#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define RECEIVE_BUF_SIZE 1024
#define EXAMPLE_PORT 7777
#define MAX_PENDING_CONNECTIONS 10
#define STATE_REQUEST -1
#define STATE_UNKNOWN -2

int main(void)
{
    struct sockaddr_in stSockAddr;

    /* Create receiving socket for incomming connection requests. */
    int serverSocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(-1 == serverSocketFD)
    {
      perror("can not create socket");
      return EXIT_FAILURE;
    }

    /* Create and initialise socket`s address structure. */
    memset(&stSockAddr, 0, sizeof(stSockAddr));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(EXAMPLE_PORT);
    stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bind socket with local address. */
    if(-1 == bind(serverSocketFD,(struct sockaddr *)&stSockAddr, sizeof(stSockAddr)))
    {
      perror("error bind failed");
      close(serverSocketFD);
      return EXIT_FAILURE;
    }

    /* Set socket to listening state. */
    if(-1 == listen(serverSocketFD, MAX_PENDING_CONNECTIONS))
    {
      perror("error listen failed");
      close(serverSocketFD);
      return EXIT_FAILURE;
    }

    fd_set readset, writeset;

    int state = STATE_UNKNOWN;

    while(1)
    {
        FD_ZERO(&readset);
        FD_SET(serverSocketFD, &readset);

        FD_ZERO(&writeset);
        FD_SET(serverSocketFD, &writeset);

        /* Waiting for incoming connection request. */
        if (select(serverSocketFD + 1, &readset, &writeset, NULL, NULL) == 1)
        {
            int new_state = STATE_UNKNOWN;
            int ConnectFD = -1;
            if (FD_ISSET(serverSocketFD, &readset))
            {

                /* Receiving of incoming connection request. */
                ConnectFD = accept(serverSocketFD, NULL, NULL);

                if (ConnectFD == -1)
                {
                    perror("error accept failed");
                    close(serverSocketFD);
                    return EXIT_FAILURE;
                }

                /* Read data from incoming connection. */
                if (-1 == recv(ConnectFD, &new_state, sizeof(new_state), 0))
                {
                    perror("recv failed");
                    close(ConnectFD);
                    close(serverSocketFD);
                    return EXIT_FAILURE;
                }
            }

            /* synchronizations isn't required */
            if (new_state == STATE_REQUEST)
            {
                if (-1 == send(ConnectFD, &state, sizeof(state), 0))
                {
                    perror("send failed");
                    close(ConnectFD);
                    return EXIT_FAILURE;
                }
            }
            else
            {
                state = new_state;
            }

            /* Close incomming connection. */
            if (-1 == shutdown(ConnectFD, SHUT_RDWR))
            {
                perror("can not shutdown socket");
                close(ConnectFD);
                close(serverSocketFD);
                return EXIT_FAILURE;
            }

            /* Close accepted connection handler. */
            close(ConnectFD);

        }
        else
        {
            /* In case of error close receive socket. */
            close(serverSocketFD);
            return EXIT_FAILURE;
        }
    }

    /* Stop processing of incoming connection requests. */
    shutdown(serverSocketFD, SHUT_RDWR);
    close(serverSocketFD);

    return EXIT_SUCCESS;
}