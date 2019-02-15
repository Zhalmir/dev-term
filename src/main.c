/* C libs */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Ext. libs */
#include <windows.h>

/* WARNING: WINDOWS ONLY! */
#ifndef _WIN32
	#error "Dev Term supports only windows!"
#endif

/* Config */
#define MAX_INPUT_SIZE 256

/* Helper functions */
#define get_line(buffer) scanf("%[^\n]%*c", buffer)

/* API Wrappers */
#define dev_whoami() getenv("USERNAME")
#define dev_home() getenv("HOMEPATH")
#define dev_hostname() getenv("ComputerName")

#define dev_clear() system("cls")
#define dev_chdir(path) SetCurrentDirectory(path)
#define dev_pwd(buffer) GetCurrentDirectory(MAX_INPUT_SIZE, buffer)
#define dev_ls() system("dir")

#define COLOR_CODE int
#define COLOR_CODE_GREEN 2
#define COLOR_CODE_RED 4
#define COLOR_CODE_PURPLE 5
#define COLOR_CODE_YELLOW 6
#define COLOR_CODE_RESET 7
#define COLOR_CODE_LIGHTGREEN 10
#define COLOR_CODE_LIGHTPURPLE 13
void dev_textcolor(COLOR_CODE color_code) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color_code);
}

/* Token array */
typedef struct {
	char **items;
	int length;
	int size;
} TokenArray;
#define TokenArray_new(_size) (TokenArray) { \
	.items = malloc(sizeof(char*) * _size), \
	.length = 0, \
	.size = _size \
}
#define TokenArray_push(array, token) array.items[array.length++] = token
#define TokenArray_stretch(array) realloc(array.items, array.size = array.length)
#define TokenArray_free(array) for (int _array_i = 0; _array_i < array.length; _array_i++) free(array.items[_array_i]); free(array.items);

/* String tokenizer */
TokenArray tokenize_str(char *src) {
	int src_len = strlen(src);
	TokenArray tokens = TokenArray_new(src_len);

	char tok[src_len];
	int tok_len = 0;

	for (int i = 0; i < src_len; i++) {
		while (src[i] == ' ' && i < src_len) i++;

		while (src[i] != ' ' && i < src_len) {
			if (src[i] == '\\') {
				i++;
				continue;
			}
			tok[tok_len++] = src[i++];
		}
		tok[tok_len] = '\0';
		TokenArray_push(tokens, strdup(tok));
		tok_len = 0;
	}

	TokenArray_stretch(tokens);
	return tokens;
}

/* Argument parser */
int parse_cmd(char *cmd) {
	TokenArray tokens = tokenize_str(cmd);

	if (tokens.length != 0) {

		if (!strcmp(tokens.items[0], "exit")) return -1;
		else if (!strcmp(tokens.items[0], "clear")) return dev_clear();
		else if (!strcmp(tokens.items[0], "cd")) {
			if (tokens.length != 2) {
				printf("No directory specified(!)");
				return 1;
			}
			return dev_chdir(tokens.items[1]);
		}
		else if (!strcmp(tokens.items[0], "pwd")) {
			char dir[MAX_INPUT_SIZE] = {0};
			dev_pwd(dir);
			printf("%s\n", dir);
			return 0;
		}
		else if (!strcmp(tokens.items[0], "ls")) return dev_ls();
		else return system(cmd);

	}

	TokenArray_free(tokens);
}

/* Main */
int main(int argc, char *argv) {

	/* State */
	int last_return_value = 0;

	/* Change into home dir */
	dev_chdir(dev_home());

	/* Input buffer */
	char input[MAX_INPUT_SIZE] = {0};
	while (true) {
		putchar('\n');

		/* Print username */
		dev_textcolor(COLOR_CODE_LIGHTPURPLE);
		printf("%s", dev_whoami());

		/* Seperator */
		dev_textcolor(COLOR_CODE_RESET);
		printf(" at ");

		/* Machine name */
		dev_textcolor(COLOR_CODE_YELLOW);
		printf("%s", dev_hostname());

		/* Seperator */
		dev_textcolor(COLOR_CODE_RESET);
		printf(" in ");

		/* Print Current dir */
		dev_textcolor(COLOR_CODE_LIGHTGREEN);
		dev_pwd(input);
		printf("%s\n", input);

		/* Get input */
		!last_return_value ? dev_textcolor(COLOR_CODE_GREEN) : dev_textcolor(COLOR_CODE_RED);
		printf("-> ");
		dev_textcolor(COLOR_CODE_RESET);
		get_line(input);

		/* Run input */
		if ((last_return_value = parse_cmd(input)) == -1) break;

	}

	dev_textcolor(COLOR_CODE_RESET);
	return 0;
}
