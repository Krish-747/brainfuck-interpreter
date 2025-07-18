#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(const char *filename)
{
    FILE *filepointer = NULL;
    char *buffer = NULL;
    int filesize;

    filepointer = fopen(filename, "r");
    if (filepointer == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    if (fseek(filepointer, 0, SEEK_END) != 0)
    {
        perror("Error reading file");
        return NULL;
    }

    if ((filesize = ftell(filepointer)) == -1L)
    {
        perror("Error getting file length");
        return NULL;
    }
    rewind(filepointer);

    buffer = (char *)calloc(filesize + 1, sizeof(char));
    fread(buffer, sizeof(char), filesize, filepointer);
    buffer[filesize] = '\0';
    fclose(filepointer);

    return buffer;
}

int *bracket_map(const char *program)
{
    int program_length = strlen(program);
    int *map;
    int *brackets;

    map = (int *)calloc(program_length, sizeof(int));
    brackets = (int *)calloc(program_length / 2 + 1, sizeof(int));
    int bracket_top = -1;

    for (int i = 0; i < program_length; i++)
    {
        switch (program[i])
        {
        case '[':
            brackets[++bracket_top] = i;
            break;
        case ']':
        {
            int closing_bracket = brackets[bracket_top--];

            map[i] = closing_bracket;
            map[closing_bracket] = i;
            break;
        }
        }
    }

    free(brackets);
    return map;
}

void interpreter(const char *program)
{
    int stack[512] = {0};
    int ptr = 0;
    int program_length = strlen(program);
    int *bracketmap = bracket_map(program);

    int program_ptr = 0;

    while (program_ptr < program_length)
    {
        switch (program[program_ptr])
        {
        case '+':
            stack[ptr]++;
            break;

        case '-':
            stack[ptr]--;
            break;

        case '>':
            ptr++;
            if (ptr >= 512)
            {
                fprintf(stderr, "Error: Stack pointer too high\n");
                exit(EXIT_FAILURE);
            }
            break;

        case '<':
            ptr--;
            if (ptr < 0)
            {
                fprintf(stderr, "Error: Stack pointer too low\n");
                exit(EXIT_FAILURE);
            }
            break;

        case '.':
            if (stack[ptr] < 128)
                printf("%c", stack[ptr]);
            else
            {
                fprintf(stderr, "Error, Non ASCII Value detected \n");
                exit(EXIT_FAILURE);
            }

            break;

        case ',':
            scanf("%d", &stack[ptr]);
            break;

        case '[':
            if (stack[ptr] == 0)
            {
                program_ptr = bracketmap[program_ptr];
            }
            break;

        case ']':
            if (stack[ptr] != 0)
            {
                program_ptr = bracketmap[program_ptr];
            }
            break;
        }
        program_ptr++;
    }

    free(bracketmap);
}

int main(void)
{
    char *he = read_file("test.bf");
    interpreter(he);
}