#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BF.h"                                         /* Include BF library */
#include "HP.h"                                        /* Include HP librarty */
#include "Record.h"        /* DEFINE THE RECORD STRUCTURE IN THIS LIBRARY!!!! */
#include "Sorted.h"		/* Include Sorted librarty */



int main() {
	int x,fd,i;
	Record temp,record;
	BF_Init();
	x = Sorted_CreateFile("arxeio");	
	fd = Sorted_OpenFile("arxeio");
	strcpy(temp.name,"nikos");
	strcpy(temp.surname,"mixelis");
	strcpy(temp.city,"athens");
	for(i=1;i<=5;i++) {
		temp.id=i;
		x=HP_InsertEntry(fd,temp);
	}	
	for(i=1;i<=1;i++) {
		temp.id=7;
		x=HP_InsertEntry(fd,temp);
	}	
	for(i=8;i<=16;i++) {
		temp.id=i;
		x=HP_InsertEntry(fd,temp);
	}	
	Sorted_GetAllEntries(fd,"id",NULL);
	strcpy(record.name,"nikos");
	strcpy(record.surname,"liappas");
	strcpy(record.city,"athens");
	record.id=6;
	Sorted_InsertEntry(fd,record);
	Sorted_GetAllEntries(fd,"id",NULL);
	
	return 0;

}
