#include "control_panel.h"

void generate_random(char src[][4], char dest[][4], int total){
	FILE *file=fopen("random.txt", "w");
	if (file==NULL){
		printf("Error opening file.");
		exit(1);
	}
	char source[4], destination[4];

	for (int i=0; i<total; i++){
		int a=0, b=0;
		while (a==b){
			a=rand() % (sizeof(src[0])/sizeof(src[0][0]) + 1 - 0);
			b=rand() % (sizeof(dest[0])/sizeof(dest[0][0]) + 1 - 0);
		}
		strcpy(source, src[a]);
		strcpy(destination, dest[b]);

		fprintf(file, "distance %s %s\n", source, destination);
	}
	fclose(file);
}

		
