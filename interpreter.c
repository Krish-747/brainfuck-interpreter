#include <stdio.h>

int ptr;
int stack[512];

void execute_bf(FILE *bfFile)
{
    int character;

    while ((character = fgetc(bfFile)) != EOF)
    {
        switch (character)
        {
        case '+':
            stack[ptr]++;
            break;

        case '-':
            stack[ptr]--;
            break;

        case '>':
            ptr++;
            break;

        case '<':
            ptr--;
            break;

        case '.':
            if (stack[ptr] < 128)
                printf("%c", stack[ptr]);
            else
                printf("Error, Non ASCII Value detected");

            break;

        case ',':
            scanf("%d", &stack[ptr]);
            break;

        case '[':
            if (stack[ptr] == 0)
            {
                int brackets = 1;
                while (brackets > 0)
                {
                    character = fgetc(bfFile);
                    if (character == '[')
                        brackets++;
                    else if (character == ']')
                        brackets--;
                }
            }
            break;

        case ']':
            if (stack[ptr] != 0)
            {
                int brackets = 1;
                fseek(bfFile, -2, SEEK_CUR);
                while (brackets > 0){
                    fseek(bfFile, -1, SEEK_CUR);
                    character = fgetc(bfFile);
                    fseek(bfFile, -1, SEEK_CUR);

                    if (character == '[')
                        brackets--;
                    else if (character == ']')
                        brackets++;

                }
            }
            break;

        default:
            break;
        }
    }

    fclose(bfFile);
}

int main(void)
{
    execute_bf(fopen("test.bf", "r"));
}