#include "control_panel.h"

int hashed=0;

void help(){
	printf("COMMANDS:\n\nhelp: command list\n\nairports: list of airports serviced by the FTA\n\nquit: exits the system\n\ndistance <airport 1> <airport2>: shortest path & length between 2 airports using routes flown by the airline\n\n");
}

void airports(){
	int c;
	FILE *file=fopen(airports_txt, "r");
	if (file) {
   		 while ((c = getc(file)) != EOF)
        		putchar(c);
	}
	fclose(file);
	printf("\n");
}

int calc_line_num(FILE *fp){
	int count=0;
	for (char c = getc(fp); c != EOF; c = getc(fp))
                if (c == '\n')
			count++;
	rewind(fp);
	return count;
}

void shortest_distance(char *port1, char *port2, FILE **speed){
	FILE *dist=fopen(dist_txt, "r");
	FILE *names=fopen(airports_txt, "r");

	if (!dist){
		printf("No distance file found.\n\n");
		return;
	} else if (!names){
		printf("No airport file found.\n\n");
		return;
	}
	int dist_line_num=calc_line_num(dist), names_line_num=calc_line_num(names);
	
	char src[dist_line_num][4], dest[dist_line_num][4], airport_names[names_line_num][4], temp_int[10];
	int weights[dist_line_num];

	for (int i=0; i<dist_line_num; i++){
		fscanf(dist, "%s\t%s\t%s\n", src[i], dest[i], temp_int);
		weights[i]=atoi(temp_int);
	}
	fclose(dist);

	for (int i=0; i<names_line_num; i++)
		fscanf(names, "%s\t%*[^\n]\n", airport_names[i]);
	fclose(names);

	int if_port1=0, if_port2=0;
	/*if (!strcmp(port1,port2)){
		printf("Same airports. Please input different ones.\n\n");
		return;
	}*/
	if (hashed==0)
		initialize(names_line_num);
	for (int i=0;i<names_line_num;i++){
		if (hashed==0){
			insert(airport_names[i], i);
			strcpy(map[i], airport_names[i]);
		}
		if (!strcmp(port1, airport_names[i]))
			if_port1=1;
		else if (!strcmp(port2, airport_names[i]))
			if_port2=1;
	}
	hashed=1;

	//generate_random(src, dest, 150000);
	if (if_port1 && if_port2){
		clock_t s1 = clock();
		dijkstra(src, dest, weights, port1, port2, dist_line_num, names_line_num);
		double diff1 = clock() - s1;
		clock_t s2 = clock();
		bellman_ford(src, dest, weights, port1, port2, dist_line_num*2, names_line_num);
		double diff2 = clock() - s2;
		fprintf(*speed, "%lf,%lf\n", diff1/CLOCKS_PER_SEC, diff2/CLOCKS_PER_SEC);
	} else
		printf("Wrong airport names.\n\n");
	//printf("%s\n\n", ports);
}

void commands(){
	FILE *speed=fopen("speed.csv", "w");
	char commd[17];
	while (1){
		printf("> ");
		//malloc_stats();
		if(fgets(commd,17,stdin)){
			char *p;
		    	if((p=strchr(commd, '\n'))!=NULL)//check exist newline
				*p = 0;
		    	else {
				scanf("%*[^\n]");
				scanf("%*c");//clear upto newline
		   	}
			printf("\n");
			commd[strcspn(commd, "\n")]=0;
			for (int i=0;i<strlen(commd);i++)
				commd[i]=tolower(commd[i]);
			char test[9];
			char port1[4], port2[4];
			strncpy(test, commd, 9);
			test[8]='\0';
			if (strcmp(commd, "help")==0)
				help();
			else if (strcmp(commd, "quit")==0)
				break;
			else if (strcmp(commd, "airports")==0)
				airports();
			else if (strcmp(test, "distance")==0){
	                        strncpy(port1, commd+9, 4);
				port1[3]='\0';
        	                strncpy(port2, commd+13, 4);
				port2[3]='\0';
				for (int i=0; i<strlen(port1); i++){
					port1[i]=toupper(port1[i]);
					port2[i]=toupper(port2[i]);
				}
				shortest_distance(port1, port2, &speed);
			}else
				printf("Wrong command. Type 'help' for command list.\n\n"); 
		}
	}
	if (hashed){
		for (int i=0; i<sizeof(map)/sizeof(map[0]);i++)
                	free(map[i]);
        	free(map);
	}
	fclose(speed);
}

int main(){
	commands();
}
