#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SEPARATORS " -\r\t\n./,"

int
main(int argc, char *argv[])
{
    int fd;
    char c;
    int num = 0;
    int in_number = 0;
    int valid_number = 1;
    int previous_separator = 1;

    
    if (argc == 1) {
        fd = 0;
    }
    
    else if (argc == 2) {
        fd = open(argv[1], 0);

        if (fd < 0) {
            fprintf(2, "sixfive: cannot open %s\n", argv[1]);
            exit(1);
        }
    }
    else {
        fprintf(2, "usage: sixfive [file]\n");
        exit(1);
    }

   
    while (read(fd, &c, 1) == 1) {

        if (c >= '0' && c <= '9') {

            
            if (!in_number) {
                num = 0;
                valid_number = previous_separator;
                in_number = 1;
            }

            num = num * 10 + (c - '0');

           
            previous_separator = 0;
        }
        else {
            
            if (in_number && strchr(SEPARATORS, c) != 0) {

                if (valid_number &&
                    (num % 5 == 0 || num % 6 == 0)) {
                    printf("%d\n", num);
                }

                in_number = 0;
                num = 0;
            }

            
            if (strchr(SEPARATORS, c) != 0)
                previous_separator = 1;
            else
                previous_separator = 0;
        }
    }

    
    if (in_number && valid_number &&
        (num % 5 == 0 || num % 6 == 0)) {
        printf("%d\n", num);
    }

    if (fd != 0)
        close(fd);

    exit(0);
}
