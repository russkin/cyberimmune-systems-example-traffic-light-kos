#include <stdio.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KYEL  "\x1B[33m"
#define KGRN  "\x1B[32m"


void print_color(char enabled, char blink, char* color)
{
    if (enabled && blink)
        fprintf(stdout, "%s(░)", color);
    else if (enabled)
        fprintf(stdout, "%s(█)", color);
    else
        fprintf(stdout, "%s( )", color);
}

void print_ligts(int state)
{
    fprintf(stdout, "│");
    print_color(state & 0x1, state & 0x8, KRED);
    fprintf(stdout, "%s│", KNRM);
    print_color(state & 0x2, state & 0x8, KYEL);
    fprintf(stdout, "%s│", KNRM);
    print_color(state & 0x4, state & 0x8, KGRN);
    fprintf(stdout, "%s│", KNRM);
}

void print_state(int state)
{
    fprintf(stdout, "┌───┬───┬───┐  ┌───┬───┬───┐\n");
    print_ligts(state);
    fprintf(stdout, "  ");
    print_ligts(state >> 8);
    fprintf(stdout, "\n└───┴───┴───┘  └───┴───┴───┘\n");
}
