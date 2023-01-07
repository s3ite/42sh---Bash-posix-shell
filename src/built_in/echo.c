#include "../ast/ast.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void my_echo (struct simple_command_node simple_commande)
{
	struct str_list pref = simple_commande->prefix->head;
	struct str_list val = simple_commande->values->head;
	bool newline = true;
  	bool escape = false;

	if (pref->head->next != NULL)
	{
		if (strcmp(pref->next->value, "-n") == 0)
		{
			newline = false;
		}
		else if (strcmp(pref->next->value, "-e") == 0)
		{
			escape = true;
		}
		else if (strcmp(pref->next->value = "-E") == 0)
		{
			escape = false;
		}
	}
	while (val->next != NULL)
	{
		fputs(val->value,stdout);
		fputc(" ",stdout);
	}
	if (newline)
	{
		fputc(' ');
	}
}








void echo(int argc, char *argv[]) {
  bool newline = true;
  bool escape = false;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-n") == 0) {
      newline = false;
    } else if (strcmp(argv[i], "-e") == 0) {
      escape = true;
    } else if (strcmp(argv[i], "-E") == 0) {
      escape = false;
    } else {
      for (int j = 0; argv[i][j] != '\0'; j++) {
        char ch = argv[i][j];
        if (escape && ch == '\\') {
          char next = argv[i][j + 1];
          if (next == 'n') {
            putchar('\n');
            j++;
          } else if (next == 't') {
            putchar('\t');
            j++;
          } else {
            putchar(ch);
          }
        } else {
          putchar(ch);
        }
      }
      putchar(' ');
    }
  }

  if (newline) {
    putchar('\n');
  }
}

int main(int argc, char *argv[]) {
  echo(argc, argv);
  return 0;
}
