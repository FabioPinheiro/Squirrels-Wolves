#include <stdio.h>
#include <stdlib.h>

int worldsize = 0;

int main(int argc, char const *argv[]){
	//int ret=3, x, y;
	int ret=3, y, x;
	char chr;
	int teste;

	FILE * inputFile;
	inputFile = fopen(argv[1],"r");
	teste = fscanf(inputFile,"%d", &worldsize);
	printf("Tamanho: %d\n", worldsize);

	while(1){
		ret=fscanf(inputFile,"%d %d %c\n", &x, &y, &chr);
		if(ret != 3)
			break;
		printf("x: %d  y: %d\n", x, y);
	}
	printf("End File :D\n");
	return 0;
}
