#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

/* 去除字符串首尾的空白字符 */
//传入字符串的头指针，然后创建一个尾指针，while循环，一直让str指到真正的内容。
//同时尾指针开始向左扫描，一直到扫到真正的内容，在这个字符的下一个地址添加字符串的结束位。
//此时直接调用str,那么这就是一个没有空字符的字符串。
void trim(char *str) {
    char *end;
    while (*str == ' ' || *str == '\t' || *str == '\n')
        str++;
    if (*str == '\0')
        return;
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n'))
        end--;
    *(end + 1) = '\0';
}

/* 将输入字符串解析成参数数组 */
int parse_input(char *input, char **args) {
    int count = 0;
    char *token = strtok(input, " \t");
    while (token != NULL && count < MAX_ARGS - 1) {
        args[count++] = token;
        token = strtok(NULL, " \t");
    }
    args[count] = NULL; /* execvp 要求 NULL 结尾 */
    return count;
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    /* 忽略终端信号，让子进程处理 */
    signal(SIGINT, SIG_IGN);

    while (1) {
        /* 打印提示符 */
        printf("minishell$ ");
        fflush(stdout);

        /* 读取输入 */
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break; /* Ctrl+D 退出 */
        }

        /* 去除换行符和多余空白 */
        trim(input);

        /* 跳过空输入 */
        if (input[0] == '\0')
            continue;

        /* 处理内建 exit 命令 */
        if (strcmp(input, "exit") == 0)
            break;

        /* 解析输入 */
        parse_input(input, args);

        /* 创建子进程执行命令 */
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork 失败");
            continue;
        }

        if (pid == 0) {
            /* 子进程：恢复默认信号处理 */
            signal(SIGINT, SIG_DFL);

            /* 执行命令 */
            execvp(args[0], args);

            /* 如果 execvp 返回，说明出错了 */
            fprintf(stderr, "minishell: 未找到命令: %s\n", args[0]);
            exit(EXIT_FAILURE);
        } else {
            /* 父进程：等待子进程结束 */
            int status;
            waitpid(pid, &status, 0);

            /* 可选：打印退出状态 */
            if (WIFEXITED(status)) {
                /* printf("程序退出，状态码: %d\n", WEXITSTATUS(status)); */
            }
        }
    }

    printf("再见！\n");
    return 0;
}