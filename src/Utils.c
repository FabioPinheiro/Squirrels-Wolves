

int main(int argc, char const *argv[]) {
	/****************  DECLARATIONS  ********************/
	FILE * inputFile;
	int teste,ret;
	double TOTAL, aux, cont;

	cont=0;
	inputFile = fopen(argv[1], "r");
	teste = fscanf(inputFile, "%d", &worldsize);
	if (teste != 1) {
		printf("Input error!\n");
		exit(-1);
	}
	/*
	 READ FILE
	 */

	while (1) {
		ret = fscanf(inputFile, "%f\n", &aux);
		TOTAL+=aux;
		cont++;
		if (ret != 1)
			break;
	}
	fclose (inputFile);

	printf("%s DEMOROU:       TOTAL ->  %f    MEDIA-> %f ",argv[1], TOTAL, TOTAL/cont);
	printf("End File :D\n");
	return 0;
}
