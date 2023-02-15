#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef struct todo_task_arr
{
    struct todo_task *values;
    uint32_t length;
    uint32_t capacity;
};

typedef struct todo_task
{
    uint32_t id;
    char *task;
    time_t etc_utc;
    bool completed;
    char *description;
    struct todo_task_arr sub_tasks;
} Todo_Task;

size_t time_to_sorted_str(char *buffer, size_t size_in_bytes, time_t datetime)
{
    if (size_in_bytes < sizeof(char) * 20)
        return 0;

    struct tm bdt;
    gmtime_s(&bdt, &datetime);
    return strftime(buffer, size_in_bytes, "%Y-%m-%dT%H:%M:%S", &bdt);
}

time_t time_utc_now(void)
{
    struct timespec tt;
    timespec_get(&tt, TIME_UTC);
    return tt.tv_sec;
}

time_t time_add_secs(time_t time, uint64_t sec)
{
    return time + sec;
}

time_t time_add_mins(time_t time, uint64_t min)
{
    return time + min * 60;
}

time_t time_add_hours(time_t time, uint64_t hours)
{
    return time + hours * 60 * 60;
}

time_t time_add_days(time_t time, uint64_t days)
{
    return time + days * 24 * 60 * 60;
}

Todo_Task todo_task(char *task, char *description, time_t etc_utc)
{
    Todo_Task todo_task = {
        .id = 0,
        .task = task,
        .description = description,
        .etc_utc = etc_utc,
        .sub_tasks.values = NULL,
        .sub_tasks.capacity = 0,
        .sub_tasks.length = 0,
        .completed = false,
    };
    return todo_task;
}

void todo_task_free(Todo_Task *task)
{
    if (task == NULL)
        return;

    for (uint32_t i = 0; i < task->sub_tasks.length; i++)
        todo_task_destroy(task->sub_tasks.values + i);

    free(task->sub_tasks.values);
    task->sub_tasks.length = 0;
    task->sub_tasks.capacity = 0;
}

bool todo_task_add_sub_task(Todo_Task *parent, Todo_Task child)
{
    if (parent == NULL)
        return false;

    if (parent->sub_tasks.length + 1 >= parent->sub_tasks.capacity)
    {
        uint32_t min_capacity = 4;
        if (parent->sub_tasks.length + 1 > min_capacity)
            min_capacity = 2 * parent->sub_tasks.capacity;
        Todo_Task *tasks = realloc(tasks->sub_tasks.values, sizeof(*tasks) * min_capacity);

        if (tasks == NULL)
            return false;

        parent->sub_tasks.values = tasks;
        parent->sub_tasks.capacity = min_capacity;
    }

    parent->sub_tasks.values[parent->sub_tasks.length++] = child;
    if (!child.completed)
        parent->completed = false;
}

void print_todo_task(Todo_Task *task, uint32_t indention)
{
    for (uint32_t i = 0; i < indention; i++)
        putc('\t', stdout);

    char datetime[20];
    time_to_sorted_str(datetime, sizeof(*datetime) * 20, task->etc_utc);
    char *completed_str = task->completed ? "v" : "";
    printf("[%s] %d: %s - t: %s\n", completed_str, task->id, task->task, datetime);

    for (uint32_t i = 0; i < task->sub_tasks.length; i++)
        print_todo_task(&task->sub_tasks.values[i], indention + 1);
}

uint64_t _todo_task_save(Todo_Task *task, const FILE *stream, uint64_t index)
{
    char *desc = NULL;
    bool has_desc = task->description != NULL;
    if (has_desc)
    {
        uint64_t desc_len = strlen(task->description);
        uint64_t desc_cap = desc_len + 2 + 1;
        desc = malloc(sizeof(*desc) * desc_cap);
        if (desc == NULL)
        {
            has_desc = false;
        }
        else
        {
            desc[0] = '\"';
            for (uint32_t i = 0; i < desc_len; i++)
                desc[i + 1] = task->description[i];
            desc[desc_len + 1] = '\"';
            desc[desc_len + 2] = 0;
        }
    }

    uint64_t chars_written = fprintf_s(stream, "{\"id\":\"%zu\",\"task\":\"%s\",\"description\":%s,\"etc_utc\":%zu,\"completed\":%s\"sub_tasks\":[",
                                  index, task->task, has_desc ? desc : "null",task->etc_utc, task->completed ? "true" : "false");
    if (has_desc)
        free(desc);

    for (uint32_t i = 0; i < task->sub_tasks.length; i++)
        chars_written += _todo_task_save(&task->sub_tasks.values[i], stream, chars_written);
    
    chars_written += fprintf_s(stream, "]}");

    return chars_written;
}

bool todo_task_save(Todo_Task *task, const char *filePath)
{
    if (task == NULL)
        return false;

    FILE *fs;
    errno_t err = fopen_s(&fs, filePath, "w");

    if (err || fs == NULL)
        return false;

    uint64_t chars = _todo_task_save(task, fs, 0);

    return chars != 0;
}

int main(int argc, char *argv[])
{
    time_t task_time = time_add_days(time_utc_now(), 100);
    Todo_Task project = todo_task("Build todo cli tool", NULL, task_time);

    char *task_names[5];
    for (size_t i = 0; i < 5; i++)
    {
        task_names[i] = malloc(sizeof(char) * 100);
        sprintf_s(task_names[i], sizeof(char) * 100, "Sub Task num %d", i + 1);
        time_t sub_task_time = time_add_days(time_utc_now(), 1 + i);
        Todo_Task sub_task = todo_task(task_names[i], NULL, sub_task_time);
        todo_task_add_sub_task(&project, sub_task);
    }

    print_todo_task(&project, 0);

    for (size_t i = 0; i < 5; i++)
        free(task_names[i]);

    todo_task_free(&project);

    return 0;
}

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