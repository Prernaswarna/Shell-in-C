#define BUFFSIZE 1024
char *myshell_readline(void)
{
	int buffersize = BUFFSIZE;
	int pos=0;
	char *bufferinput = malloc(size(char)*buffersize);
	int ch;

	if(!bufferinput){
		fprintf(stderr , "myshell : allocation error");
		exit(EXIT_FAILURE);
	}

	while(true){
		ch = getchar();
		if(ch==EOF || ch='\n'){
			bufferinput[pos]='\0';
			return bufferinput;
		}
		else{
			bufferinput[pos]=c;
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
