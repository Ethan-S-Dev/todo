#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "./todo_task.c"

void free_pointer_arr(void ** arr, size_t size){
    for (size_t i = 0; i < size; i++)
        free(arr[i]);
}

#define SUB_TASKS_COUNT 24

int main(void)
{
    time_t task_time = time_add_days(time_utc_now(), 100);
    Todo project;
    errno_t err = todo_task_s(&project, "Build todo cli tool", NULL, task_time);
    if(err){
        printf("error creating project.\n");
        exit(err);
    }
    char *task_names[SUB_TASKS_COUNT];
    for (size_t i = 0; i < SUB_TASKS_COUNT; i++)
    {
        task_names[i] = malloc(sizeof(char) * 100);
        sprintf_s(task_names[i], sizeof(char) * 100, "Sub Task num %d", i + 1);
        time_t sub_task_time = time_add_days(time_utc_now(), 1 + i);
        Todo sub_task;
        todo_task_s(&sub_task, task_names[i], NULL, sub_task_time);
        if (i%3 == 0)
        {
            sub_task.completed = true;   
        }

        if(i%6 == 0)
        {
            sub_task.etc = 0;
        }

        todo_task_add_sub_task(&project, sub_task);
    }
    
    print_todo_task(&project, 0);


    free_pointer_arr((void**)task_names, SUB_TASKS_COUNT);
    todo_task_free(&project);
    return 0;
}

// int main(int argc, char *argv[])
//{
//
//
//     print_todo_task(&project, 0);
//
//
//
//     return 0;
// }

// typedef struct cmd_struct {
//     const char *cmd;
//     int (*execute)(int, char **);
// } Command;
//
//
// int add_cmd(int argc, char **argv){
//
//     return 0;
// }
//
// int info_cmd(int argc, char **argv){
//     printf("Todo cli, use -h for more information on how to use it.\n");
//
//     return 0;
// }
//
// int help_cmd(int argc,const char **argv)
//{
//     printf("Help!");
//
//     return 0;
// }
//
// #define COMMANDS_COUNT 2
// static Command commands[] = {
//     {"info", info_cmd},
//     {"add",add_cmd}
// };
//
// Command* get_command(int argc, char **argv){
//
//     if(argc < 1)
//         return NULL;
//
//     for (size_t i = 0; i < COMMANDS_COUNT; i++)
//     {
//         Command* p = commands + i;
//         if(!strcmp(p->cmd, argv[0]))
//             return p;
//     }
//
//     return NULL;
// }
//
// int main(int argc, char **argv){
//     if(argc == 1)
//     {
//         printf("you need help?");
//         exit(1);
//     }
//
//     argv++;
//     argc--;
//
//     if(!strcmp(argv[0], "-h") || !strcmp(argv[0], "--help"))
//     {
//         printf("help");
//         exit(0);
//     }
//
//     if(!strcmp(argv[0], "-v") || !strcmp(argv[0], "--version"))
//     {
//         printf("version");
//         exit(0);
//     }
//
//     Command* command = get_command(argc, argv);
//
//     if(!command)
//     {
//         printf("No valid command was entered please -h for help.");
//         exit(1);
//     }
//
//     argv++;
//     argc--;
//
//     exit(command->execute(argc, argv));
// }