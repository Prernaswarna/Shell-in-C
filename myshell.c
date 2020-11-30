#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int myshell_cd(char **args);
int myshell_help(char **args);
int myshell_exit(char **args);

char *builtincommands[] = {
	"cd",
	"help",
	"exit",
};

int (*builtin_functions[])(char **)={
	&myshell_cd,
	&myshell_help,
	&myshell_exit
};

int myshell_number_builtins()
{
	return ((sizeof(builtincommands))/(sizeof(char*)));
}

int myshell_cd(char **args)
{
	if(args[1]==NULL){
		fprintf(stderr , "Arguement expected for cd\n");
	}else{
		if(chdir(args[1])!=0){
			perror("myshell");
		}
	}
	return 1;
}

int myshell_help(char **args)
{
	int i;
	printf("Type name and press enter\n");
	printf("Following commands are built in\n");
	
	for(i=0;i<myshell_number_builtins();i++){
		printf(" %s\n" , builtincommands[i]);
	}
	return 1;
}	

int myshell_exit(char **args)
{
	return 0;
}

#define BUFFSIZE 1024
char *myshell_readline(void)
{
	int buffersize = BUFFSIZE;
	int pos=0;
	char *bufferinput = malloc(sizeof(char)*buffersize);
	int ch;

	if(!bufferinput){
		fprintf(stderr , "myshell : allocation error");
		exit(EXIT_FAILURE);
	}

	while(1){
		ch = getchar();
		if(ch==EOF || ch=='\n'){
			bufferinput[pos]='\0';
			return bufferinput;
		}
		else{
			bufferinput[pos]=ch;
		}
		pos++;

		if(pos>=buffersize){
			buffersize=buffersize+BUFFSIZE;
			bufferinput = realloc(bufferinput , buffersize);
			if(!bufferinput){
				fprintf(stderr , "myshell : Reallocation failed");
				exit(EXIT_FAILURE);
			}
		}
	}
}


#define TOKEN_BUFFSIZE 64
#define TOKEN_DELIM " \t\r\n\a"

char **myshell_splitline(char *inputline)
{
	int buffsize = TOKEN_BUFFSIZE;
	int pos=0;
	char **tokens = malloc(buffsize*sizeof(char*));
	char *splittokens;

	if(!tokens){
		fprintf(stderr, "myshell : Allocation failure\n");
		exit(EXIT_FAILURE);
	}
	
	splittokens = strtok(inputline, TOKEN_DELIM);
	while(splittokens!=NULL){
		tokens[pos++] = splittokens;
		
		if(pos>=buffsize){
			buffsize = buffsize + TOKEN_BUFFSIZE;
			tokens = realloc(tokens, buffsize*sizeof(char*));
			if(!tokens){
				fprintf(stderr , "myshell : Reallocation failure");
				exit(EXIT_FAILURE);
			}
		}
		splittokens = strtok(NULL , TOKEN_DELIM);
	}
	tokens[pos] = NULL;
	return tokens;
}

int myshell_launch(char **inputargs)
{
	pid_t pid, wpid;
	int status;

	pid = fork();

	if(pid == 0){
		if(execvp(inputargs[0], inputargs) == -1){
			perror("myshell");
		}
		else if(pid<0){
			perror("myshell");
		}else{
			do{
				wpid = waitpid(pid , &status , WUNTRACED);
			}while(!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}
	return 1;
}

int myshell_executeline(char **inputargs)
{
	if(inputargs[0]==NULL){
		return 1;
	}
	int i;
	for(i=0;i<myshell_number_builtins();i++){
		if(strcmp(inputargs[0], builtincommands[i])==0){ 
			return(builtin_functions[i])(inputargs);
		}
	}

	return myshell_launch(inputargs);
}

void myshell_loop(void)
{
	char *inputline;
	char **inputargs;
	int status;

	do
	{
		printf("> ");
		inputline = myshell_readline();
		inputargs = myshell_splitline(inputline);
		status = myshell_executeline(inputargs);

		free(inputline);
		free(inputargs);
	}while(status);
}


int main(int argc , char **argv)
{
	myshell_loop();
	return EXIT_SUCCESS;
}
