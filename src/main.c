#include "parsed_line.h"
#include "constants.h"
#include "output.h"


int main(int argc, char* argv[])
{
    ssize_t bytes_read;

    //o arraylist do nelson era o historico
    
    ReadlnBuffer *rb = (ReadlnBuffer *)calloc(1, sizeof(ReadlnBuffer));
    initRB(0, rb, MAX_BUFFER);
    ParsedLine *pl = calloc(1, sizeof(ParsedLine));

    if(argc == 1) {
            while((bytes_read = readlnToPL(rb,pl)) > 0){
                if(pl->opt == 'o') show_output(atoi(pl->arg));
                    else save_output(pl->arg);
        }
    } else {
        pl->arg = argv[2];
        validate(argv[1],pl);
        
        if(pl->opt == 'o') show_output(atoi(pl->arg));
            else save_output(pl->arg);
        
    }

    return 0;
}
