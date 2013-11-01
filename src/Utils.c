#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
	/****************  DECLARATIONS  ********************/
	FILE * inputFile;
	FILE * outputFile;
	int teste,ret;
	double TOTALP,TOTALS, aux, contP, contS;
	char type;
	TOTALP=0.0;
	TOTALS=0.0;
	contP=0.0;
	contS=0.0;
	inputFile = fopen(argv[1], "r");
	outputfile = fopen("tempos.out", "a");
	/*
	 READ FILE
	 */

	while (1) {
		ret = fscanf(inputFile, "%c %lf\n", &type, &aux);
		if (ret != 2)
			break;
		if (type == 'P'){
			TOTALP+=aux;
			contP++;
		}
		else{
			TOTALS+=aux;
			contS++;
		}



	}
	fclose (inputFile);
	fprintf(outputfile, "%lf\n", TOTALS/TOTALP);
	printf("PARALELO DEMOROU:       TOTAL ->  %f    MEDIA-> %f \n", TOTALP, TOTALP/contP);
	printf("SERIE DEMOROU:       TOTAL ->  %f    MEDIA-> %f \n", TOTALS, TOTALS/contS);
	printf("SPEEDUP: %f \n", TOTALS/TOTALP);
	return 0;
}
