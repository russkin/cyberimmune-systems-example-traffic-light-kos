#include <netdb.h>
#include <sys/socket.h>
#include <kos_net.h>

#define STATE_REQUEST -1
#define STATE_UNKNOWN -2

bool init(void);
int get_state(int* state);
void print_state(int state);

int main(void)
{
    if (!init())
    {
        perror("can not init network");
        return EXIT_FAILURE;
    }

    int prev_state = STATE_UNKNOWN;
    while (1)
    {
        int state = STATE_REQUEST;
        if (get_state(&state) == EXIT_SUCCESS && 
            state != prev_state)
        {
            /* display current state */
            print_state(state);
            prev_state = state;
        }
    };

    return EXIT_SUCCESS;
}