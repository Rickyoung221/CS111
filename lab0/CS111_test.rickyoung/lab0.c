//NAME: Weikeng Yang
//EMAIL: weikengyang@gmail.com
//ID: 405346443

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#define INPUT 'i'
#define OUTPUT 'o'
#define SEGFAULT 's'
#define CATCH 'c'

void handle_sigsegv() {
    fprintf(stderr, "Scaught and received SIGSEGV. \n");
    exit(4);
}
void segFault(){
    char* ptr = NULL;
    *ptr = 'a';
}
// Reference: File Descriptor Management from course material
void redirect_input(char *file){
    int ifd = open(optarg, O_RDONLY);    //Return a file descriptor ; O_RDONLY: Open for reading only.
    if (ifd >= 0)
    {
        close(0);
        dup(ifd);
        close(ifd);
    }
    else
    {
        fprintf(stderr, "INPUT ERROR. Unable to open input file %s: \n", file);
        fprintf(stderr, "%s\n", strerror(errno));
        exit(2);
    }
}
void redirect_output(char *file){
    int ofd = creat(optarg, 0666);
    if (ofd >= 0)
    {
        close(1);
        dup(ofd);
        close(ofd);
    }
    else
    {
        fprintf(stderr, "OUTPUT ERROR. Unable to create output file %s: \n", file);
        fprintf(stderr, "%s\n", strerror(errno));
        exit(3);
    }
}


int main(int argc, char *argv[])
{
    
    int opt_index;
    
    static struct option long_options[] =
    {
        {"input", required_argument,NULL, INPUT},
        {"output", required_argument,NULL, OUTPUT},
        {"segfault", no_argument, NULL, SEGFAULT},
        {"catch", no_argument, NULL, CATCH},
        {0, 0, 0, 0}
    };
    
    int c;
    c = getopt_long(argc, argv, "i:o:sc", long_options, &opt_index);
    while (c != -1)
    {
        switch (c)
        {
            case INPUT:
                redirect_input(optarg);
                break;
            case OUTPUT:
                redirect_output(optarg);
                break;
            case SEGFAULT:
                segFault();
                break;
            case CATCH:
                signal(SIGSEGV, handle_sigsegv);
                break;
            default:
                fprintf(stderr, " ERROR: unrecognized option argument.\n");
                exit(1);
        }
    
        

    char *buffer = (char*) malloc(sizeof(char));
    
    ssize_t rd = read(0, buffer, 1);
        
    while (rd > 0)
    {
        
        // write to standard output
        ssize_t wd = write(1, buffer, 1);
        
        if (wd <= 0)
        {
            fprintf(stderr, "ERROR writing from output file:\n%s\n", strerror(errno));
            exit(3);
        }
        
        rd = read(0, buffer, 1);
    }

    if (rd < 0)
    {
        fprintf(stderr, "Errror reading from the input file: %s\n", strerror(errno));
        exit(2);
    }
    

    free(buffer);
    exit(0);
    
    }
}
