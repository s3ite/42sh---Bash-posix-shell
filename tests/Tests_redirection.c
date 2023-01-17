#include <criterion/criterion.h>
#include "../src/parser/parser.h"
#include "../src/lexer/lexer.h"


#include "../src/redirection/redirection.h"
#include "../src/redirection/redirection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *contains(char *path) {
    // File to read
    FILE* fp = fopen(path, "r");

    // Get the size of the file
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    // Allocate memory for the buffer
    char* buffer = (char*)malloc(sizeof(char) * (size + 1));

    // Read the contents of the file into the buffer
    fread(buffer, size, 1, fp);

    // Terminate the buffer with a null character
    buffer[size] = '\0';

    // Close the file
    fclose(fp);

    return buffer;
}

void exec_redirection(char *data)
{
    int len = strlen(data);
    struct lexer *lexer = lexer_init(len, data);
    lexer = lexer_load(data, lexer);
    parse(lexer);
}
 
// file writting redirections
Test(output_echo1, Hello_World) {
    char *input = "echo Hello, World! > 42trash_criterion/result/echo1";

    exec_redirection(input);
    char *output = contains("42trash_criterion/result/echo1");
    char *expected = contains("42trash_criterion/expected/echo1");
    cr_expect_str_eq(output, expected);
}

Test(output_echo2, Hello_World) {
    char *input = "echo Hello, World! >> 42trash_criterion/result/echo2";
    exec_redirection(input);
    exec_redirection(input);

    char *output = contains("42trash_criterion/result/echo2");
    char *expected = contains("42trash_criterion/expected/echo2");
    cr_expect_str_eq(output, expected);

    remove("42trash_criterion/expected/echo2");
    remove("42trash_criterion/result/echo2");
}

Test(output_ls1, ls_l) {
    char *input = "ls -l . > 42trash_criterion/result/ls_l";
    exec_redirection(input);

    char *output = contains("42trash_criterion/result/ls_l");
    char *expected = contains("42trash_criterion/expected/ls_l");
    cr_expect_str_eq(output, expected);
}