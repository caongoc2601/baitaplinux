#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdlib.h>
#include <pwd.h>

#define MAX_LINE 1024 // The maximum length command

int should_run = 1;  // flag to determine when to exit program
int should_wait = 1; // flag to determine if process should run in the background

void redirectInput(char *fileName){
    int in = open(fileName, O_RDONLY);
    dup2(in, 0);
    close(in);
}

void redirectOutput(char *fileName){
    int out = open(fileName, O_WRONLY | O_TRUNC | O_CREAT, 0600);
    dup2(out, 1);
    close(out);
}

void run(char *args[]){
    pid_t pid;
    if (strcmp(args[0], "exit") != 0){
            pid = fork();
            if (pid < 0) { 
                fprintf(stderr, "Fork Failed");
            }
            else if ( pid == 0) { // child
                execvp(args[0], args);	// load and run program
            }
            else { // parent
                if (should_wait) {
                    waitpid(pid, NULL, 0);
                } else {
                    should_wait = 0;
                }
            }
            redirectInput("/dev/tty");
            redirectOutput("/dev/tty");
        }
    else {
        should_run = 0;
    }
}

void createPipe(char *args[]){
    int fd[2];
    pipe(fd);

    dup2(fd[1], 1);
    close(fd[1]);

    run(args);

    dup2(fd[0], 0);
    close(fd[0]);
}

char * tokenize(char *input){
    int i;
    int j = 0;
    char *tokenized = (char *)malloc((MAX_LINE * 2) * sizeof(char));
    for (i = 0; i < strlen(input); i++) {
        if (input[i] != '>' && input[i] != '<' && input[i] != '|') {
            tokenized[j++] = input[i];
        } else {
            tokenized[j++] = ' ';
            tokenized[j++] = input[i];
            tokenized[j++] = ' ';
        }
    }
    tokenized[j++] = '\0';

    // add null to the end
    char *end;
    end = tokenized + strlen(tokenized) - 1;
    end--;
    *(end + 1) = '\0';

    return tokenized;
}

int
main(int argc, char *argv[]){

	struct passwd *pw = getpwuid(getuid());
	char *homedir = pw->pw_dir;
    char *args[MAX_LINE]; // command line arguments
    
    while (should_run) {
        printf(">> ");
        fflush(stdout);

        char input[MAX_LINE];
        fgets(input, MAX_LINE, stdin);

        char *tokens;
        tokens = tokenize(input);// add spaces around special characters

        if (tokens[strlen(tokens) - 1] == '&') {
            should_wait = 0;
            tokens[strlen(tokens) - 1] = '\0';
        }

        char *arg = strtok(tokens, " ");
        int i = 0;
        while (arg) {
            if (*arg == '<') {
                redirectInput(strtok(NULL, " "));
            } else if (*arg == '>') {
                redirectOutput(strtok(NULL, " "));
            } else if (*arg == '|') {
                args[i] = NULL;
                createPipe(args);
                i = 0;
            } else if(*arg == '~'){
				args[i] = homedir;
				i++;
			}else {
                args[i] = arg;
                i++;
            }
            arg = strtok(NULL, " ");
        }
        args[i] = NULL;
  
        run(args);
    }
    return 0;
}
