#include "kernel/types.h"
#include "user/user.h"

int main(int argc , char *argv[])
{
    if(argc < 2)
    {
        printf("Man What the hell are you doing!!!. Give some time\n");
        exit(0);    
    }

    pause(atoi(argv[1]));
    exit(0);
    
}

