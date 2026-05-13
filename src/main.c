#include "parse.h"

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