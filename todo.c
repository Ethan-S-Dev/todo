#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef struct timespec Todo_DateTime;
typedef struct timespec Todo_TimeSpan;

typedef struct todo_task {
    uint32_t index;
    char* task;
    Todo_DateTime etc_utc;
    bool completed;
    
    char* description;
    struct todo_task *sub_tasks;

} Todo_Task;


#define MAX_NANO_SEC 999999999
Todo_TimeSpan todo_timespan_from_nano(int64_t nano_sec){
    Todo_TimeSpan ts = {0, 0};
    if(nano_sec < 0)
    {

    }else{
        ts.tv_sec = nano_sec / MAX_NANO_SEC;
        ts.tv_nsec = nano_sec % MAX_NANO_SEC;
    }

    return ts;
}

Todo_TimeSpan todo_timespan_from_mil(int64_t mil_sec){
    return todo_timespan_from_nano(mil_sec * 1000000);
}

Todo_TimeSpan todo_timespan_from_sec(int64_t sec){
    Todo_TimeSpan ts = {sec, 0};
    return ts;
}
Todo_TimeSpan todo_timespan_from_min(int64_t min){
    return todo_timespan_from_sec(min*60);
}
Todo_TimeSpan todo_timespan_from_hours(int64_t hours){
    return todo_timespan_from_min(hours*60);
}
Todo_TimeSpan todo_timespan_from_days(int64_t days){
    return todo_timespan_from_hours(days*24);
}

Todo_DateTime todo_datetime_utc_now(void){
    Todo_DateTime tt;
    timespec_get(&tt, TIME_UTC);
    return tt;
}
void todo_datetime_add(Todo_DateTime *tt, Todo_TimeSpan time_span){
    time_t sec = tt->tv_sec + time_span.tv_sec;
    int reminder;
    long nano_sec;

    if(time_span.tv_sec < 0)
    {
        nano_sec = tt->tv_nsec - time_span.tv_nsec;
        reminder = nano_sec < 0 ? -1 : 0;
        nano_sec = nano_sec < 0 ? MAX_NANO_SEC + nano_sec : nano_sec; 
    }else{
        nano_sec = tt->tv_nsec + time_span.tv_nsec;
        reminder = nano_sec / MAX_NANO_SEC;
        nano_sec = nano_sec % MAX_NANO_SEC; 
    }

    sec += reminder;
    tt->tv_nsec = nano_sec;
    tt->tv_sec = sec;
}

Todo_Task todo_task(char* task, char* description, Todo_DateTime etc_utc){
    Todo_Task todo_task = {
        .index = 0,
        .task = task,
        .description = description,
        .etc_utc = etc_utc,
        .sub_tasks = NULL,
        .completed = false,
    };
    return todo_task;
}

// 0000-00-00T00:00:00.000000000\0
// 
size_t todo_datetime_to_sorted(char *buffer, size_t size_in_bytes, Todo_DateTime datetime){
    if(size_in_bytes < 30)
        return 0;

    struct tm bdt;
    gmtime_s(&bdt,&datetime.tv_sec);
    size_t pos = strftime(buffer, sizeof(char)*31, "%Y-%m-%dT%H:%M:%S", &bdt);
    printf("%zu\n", pos);
    buffer[pos] = '.';
    _itoa_s(datetime.tv_nsec,buffer + pos + 1, 10, 10);
    buffer[29] = 0;
    return 29;
}

int main(int argc, char *argv[]){

    //int seconds = atoi(argv[1]); 
    Todo_DateTime ts = todo_datetime_utc_now();
    //todo_datetime_add(&ts, todo_timespan_from_sec(50));
    Todo_Task task = todo_task("create remove command", "task description", ts);
    char time_str[100]; 

    size_t length = todo_datetime_to_sorted(time_str, sizeof time_str, ts);
    
    printf("Current task time: %s UTC\nLength: %zu\n", time_str, length);

    return 0;
}



//typedef struct cmd_struct {
//    const char *cmd;
//    int (*execute)(int, char **);
//} Command;
//
//
//int add_cmd(int argc, char **argv){
//
//    return 0;
//}
//
//int info_cmd(int argc, char **argv){
//    printf("Todo cli, use -h for more information on how to use it.\n");
//
//    return 0;
//}
//
//int help_cmd(int argc,const char **argv)
//{
//    printf("Help!");
//
//    return 0;
//}
//
//#define COMMANDS_COUNT 2
//static Command commands[] = {
//    {"info", info_cmd},
//    {"add",add_cmd}
//};
//
//Command* get_command(int argc, char **argv){
//
//    if(argc < 1)
//        return NULL;
//
//    for (size_t i = 0; i < COMMANDS_COUNT; i++)
//    {
//        Command* p = commands + i;
//        if(!strcmp(p->cmd, argv[0]))
//            return p;
//    }
//
//    return NULL;
//}
//
//int main(int argc, char **argv){
//    if(argc == 1)
//    {
//        printf("you need help?");
//        exit(1);
//    }
//
//    argv++;
//    argc--;
//
//    if(!strcmp(argv[0], "-h") || !strcmp(argv[0], "--help"))
//    {
//        printf("help");
//        exit(0);
//    }
//
//    if(!strcmp(argv[0], "-v") || !strcmp(argv[0], "--version"))
//    {
//        printf("version");
//        exit(0);
//    }
//
//    Command* command = get_command(argc, argv);
//
//    if(!command)
//    {
//        printf("No valid command was entered please -h for help.");
//        exit(1);
//    }
//
//    argv++;
//    argc--;
//
//    exit(command->execute(argc, argv));
//}