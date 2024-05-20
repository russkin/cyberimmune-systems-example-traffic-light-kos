#ifndef RESPONSE_PARSER
#define RESPONSE_PARSER

#define DEBUG_LEVEL 0
#if DEBUG_LEVEL > 1
#define D(A) A
#else
#define D(A) 
#endif

int parse_response(char *response);

struct traffic_light_mode_t{
    int id;
};

extern struct traffic_light_mode_t traffic_light_mode;

#endif // RESPONSE_PARSER