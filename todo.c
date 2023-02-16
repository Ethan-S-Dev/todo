#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "./todo_task.c"


//#define WINDOWS
//#ifdef WINDOWS
//#include <sysinfoapi.h>
//void init_local_time(){
//    ctime
//}
//#endif


int main(void){

    return 0;
}

//int main(int argc, char *argv[])
//{
//    time_t task_time = time_add_days(time_utc_now(), 100);
//    Todo_Task project = todo_task("Build todo cli tool", NULL, task_time);
//
//    char *task_names[5];
//    for (size_t i = 0; i < 5; i++)
//    {
//        task_names[i] = malloc(sizeof(char) * 100);
//
//        sprintf_s(task_names[i], sizeof(char) * 100, "Sub Task num %d", i + 1);
//
//        time_t sub_task_time = time_add_days(time_utc_now(), 1 + i);
//        Todo_Task sub_task = todo_task(task_names[i], NULL, sub_task_time);
//
//        if(i == 2)
//        {
//            sub_task.completed = true;
//        }
//
//        todo_task_add_sub_task(&project, sub_task);
//    }
//
//    print_todo_task(&project, 0);
//
//    for (size_t i = 0; i < 5; i++)
//        free(task_names[i]);
//
//    todo_task_free(&project);
//
//    return 0;
//}

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