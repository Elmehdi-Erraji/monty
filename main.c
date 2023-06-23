#include <stdio.h>
#include <stdlib.h>
#include "monty.h"


int TOKEN_BUFFER_SIZE = 1024;

char **tokenize(char *str, const char *delimiters)
{
    int i = 0;
    char *token;
    char **tokens = malloc(sizeof(char *) * TOKEN_BUFFER_SIZE);

    if (tokens == NULL)
    {
        fprintf(stderr, "Error: Allocation failed\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(str, delimiters);
    while (token != NULL)
    {
        tokens[i] = token;
        i++;

        if (i >= TOKEN_BUFFER_SIZE)
        {
            TOKEN_BUFFER_SIZE *= 2;
            tokens = realloc(tokens, sizeof(char *) * TOKEN_BUFFER_SIZE);

            if (tokens == NULL)
            {
                fprintf(stderr, "Error: Reallocation failed\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delimiters);
    }

    tokens[i] = NULL;

    return tokens;
}

void (*get_op_func(line_t line, meta_t *meta))(stack_t **, unsigned int)
{
    int i;
    instruction_t opcodes[] = {
        {"push", push},
        {"pall", pall},
        {"pint", pint},
        {"pop", pop},
        {"swap", swap},
        {"nop", nop},
        {"rotl", rotl},
        {"rolop", rotlop},
        {"rotrop", rotrop},
        {"pchar", pchar},
        {"pstr", pstr},
        {"addqu", addqu},
        {"addst", addst},
        {"sub", subop},
        {"add", addop},
        {"div", divop},
        {"mul", mulop},
        {"mod", modop},
        {NULL, NULL}
    };

    for (i = 0; opcodes[i].opcode != NULL; i++)
    {
        if (strcmp(line.content[0], opcodes[i].opcode) == 0)
            return opcodes[i].f;
    }

    fprintf(stderr, "L%d: unknown instruction %s\n", line.number, line.content[0]);
    exit(EXIT_FAILURE);
}

bool comment_check(line_t line)
{
    if (line.content[0] != NULL && line.content[0][0] == '#')
        return true;
    return false;
}

void push_check(line_t line, meta_t *meta, char *opcode)
{
    int i;

    if (opcode == NULL)
    {
        fprintf(stderr, "L%d: usage: push integer\n", line.number);
        exit(EXIT_FAILURE);
    }

    for (i = 0; opcode[i] != '\0'; i++)
    {
        if (i == 0 && opcode[i] == '-')
            continue;
        if (!isdigit(opcode[i]))
        {
            fprintf(stderr, "L%d: usage: push integer\n", line.number);
            exit(EXIT_FAILURE);
        }
    }

    arg.arg = atoi(opcode);
    arg.flag = 1;
}

int main(int argc, char *argv[])
{
    FILE *file;
    line_t line;
    meta_t meta;
    stack_t *stack = NULL;

    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(argv[1], "r");
    if (!file)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    ssize_t line_content = getline(&(line.content), &(size_t){0}, file);
    while (line_content != -1)
    {
        line.number++;
        line.content = tokenize(line.content, " \n");

        if (comment_check(line))
        {
            printf("Skipping comment at line %d\n", line.number);
            line_content = getline(&(line.content), &(size_t){0}, file);
            continue;
        }

        if (line.content[0] == NULL)
        {
            line_content = getline(&(line.content), &(size_t){0}, file);
            continue;
        }

        if (strcmp(line.content[0], "push") == 0)
            push_check(line, &meta, line.content[1]);

        void (*func)(stack_t **, unsigned int) = get_op_func(line, &meta);
        if (func)
        {
            printf("Executing instruction at line %d\n", line.number);
            func(&stack, line.number);
        }

        free(line.content);

        line_content = getline(&(line.content), &(size_t){0}, file);
    }

    free(line.content);
    free(meta.buf);
    free_stack(&stack);
    fclose(file);

    return EXIT_SUCCESS;
}