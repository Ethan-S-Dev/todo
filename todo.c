#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef ;

typedef struct cmd_struct {
    const char *cmd;
    int (*execute)(int, char **);
} Command;


int add_cmd(int argc, char **argv){

    return 0;
}

int info_cmd(int argc, char **argv){
    printf("Todo cli, use -h for more information on how to use it.\n");

    return 0;
}

int help_cmd(int argc,const char **argv)
{
    printf("Help!");

    return 0;
}

#define COMMANDS_COUNT 2
static Command commands[] = {
    {"info", info_cmd},
    {"add",add_cmd}
};

Command* get_command(int argc, char **argv){

    if(argc < 1)
        return NULL;

    for (size_t i = 0; i < COMMANDS_COUNT; i++)
    {
        Command* p = commands + i;
        if(!strcmp(p->cmd, argv[0]))
            return p;
    }

    return NULL;
}

int main(int argc, char **argv){
    if(argc == 1)
    {
        printf("you need help?");
        exit(1);
    }

    argv++;
    argc--;

    if(!strcmp(argv[0], "-h") || !strcmp(argv[0], "--help"))
    {
        printf("help");
        exit(0);
    }

    if(!strcmp(argv[0], "-v") || !strcmp(argv[0], "--version"))
    {
        printf("version");
        exit(0);
    }

    Command* command = get_command(argc, argv);

    if(!command)
    {
        printf("No valid command was entered please -h for help.");
        exit(1);
    }

    argv++;
    argc--;

    exit(command->execute(argc, argv));
}