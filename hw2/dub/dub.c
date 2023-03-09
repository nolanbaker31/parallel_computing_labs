#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char * dub(char * name){
	memmove(name + 4, name, strlen(name)+1);
	memcpy(name, "Sir ", 4);
	return(name);
}
int main(int argc, char *argv[]){
	if(argc > 1){
		char *knights[argc-1];
	for(int i = 1; i < argc; i++){
		char * name = malloc(strlen(argv[i]) + 4 * sizeof(char));
		strncpy(name, argv[i], strlen(argv[i]));
		knights[i] = dub(name);
		printf("%s\n", knights[i]);
		free(name);
	}
	}
	else{
		printf("Please enter command line arguments!\n");
	}
	return 0;
}
