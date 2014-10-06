#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BF.h"                                         /* Include BF library */
#include "HP.h"                                        /* Include HP librarty */
#include "Record.h"        /* DEFINE THE RECORD STRUCTURE IN THIS LIBRARY!!!! */



int main() {
	int fd,fd2,i,k1=0,k2=0;
	Record temp;
	BF_Init();
/////////////////////////////////////////////////1o arxeio/////////////////////
	HP_CreateFile("hp");	
	fd = HP_OpenFile("hp");
	strcpy(temp.name,"nikos");
	strcpy(temp.surname,"liappas");
	strcpy(temp.city,"athens");
	for(i=500;i<=1000;i++) {
		if(i%2==0) {
			temp.id=i;
			HP_InsertEntry(fd,temp); 
			k1++;
		}
	}
	HP_GetAllEntries(fd,"id",NULL);
	HP_CloseFile(fd);
/////////////////////////////////////////////////////////////////////	
//////////////////////////////////////2o arxeio ////////////////////////////////////////////////
	HP_CreateFile("hp2");	
	fd2 = HP_OpenFile("hp2");
	strcpy(temp.name,"nikos");
	strcpy(temp.surname,"mixelis");
	strcpy(temp.city,"athens");
	for(i=700;i<=1500;i++) {
		if(i%2==1) {
			temp.id=i;
			HP_InsertEntry(fd2,temp); 
			k2++;
		}
	}
	HP_GetAllEntries(fd2,"id",NULL);
	printf("Mpikan sunolika %d egrafes sto hp arxeio!\n", k1);
	printf("Mpikan sunolika %d egrafes sto hp2 arxeio!\n", k2);
	printf("Mpikan sunolika %d egrafes kai sta 2 arxeia!\n",k1+k2);
	HP_CloseFile(fd2);
//////////////////////////////////////////////////////////////////
	return 0;
}
