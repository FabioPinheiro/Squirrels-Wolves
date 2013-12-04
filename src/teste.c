#include <stdio.h>

void computedSize(int numP, int worldSize, int pId, int *result){
	*result = 0;
	*result = (int) worldSize/numP;
	if((worldSize%numP - pId) > 0){
		*result += 1;
	}
}
int main(){
	int result;
	int i, p = 9;

	for(i = 0; i <p; i++ ){
		computedSize(p, 30, i, &result);
		printf("ID %d, Resultado:%d \n", i, result);
	}


	return 0;
}
