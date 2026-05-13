#include "parse.h"

/* 去除字符串首尾的空白字符 
传入字符串的头指针，然后创建一个尾指针，while循环，一直让str指到真正的内容。
同时尾指针开始向左扫描，一直到扫到真正的内容，在这个字符的下一个地址添加字符串的结束位。
此时直接调用str,那么这就是一个头尾没有空字符的字符串 */
void trim(char *str) {
    char *end;
    char *start = str;

    /* 跳过前导空白字符 */
    while (*start == ' ' || *start == '\t' || *start == '\n')
        start++;

    /* 全空白字符串，直接清空 */
    if (*start == '\0') {
        *str = '\0';
        return;
    }

    /* 将去掉前导空白的内容移回数组开头 */
    if (start != str)
        memmove(str, start, strlen(start) + 1);

    /* 去除尾部空白字符 */
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n'))
        end--;
    *(end + 1) = '\0';
}

/* 将输入字符串解析成参数数组 */
int parse_input(char *input, char **args){
    int count = 0;
    char *token = strtok(input, " \t");
    while (token != NULL && count < MAX_ARGS - 1) {
        args[count++] = token;
        token = strtok(NULL, " \t");
    }
    args[count] = NULL; /* execvp 要求 NULL 结尾 */
    return count;
}