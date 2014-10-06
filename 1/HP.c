#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BF.h"
#include "Record.h"  
#include "HP.h"

int  HP_CreateFile(char *filename) {
	int filedesc,blockNumber;
	char *block;
	if(BF_CreateFile(filename)<0 )	{
		BF_PrintError("Error creatig the file");
		return -1;
	}
	if( (filedesc=BF_OpenFile(filename))<0 ) {
		BF_PrintError("Error opening the file");
		return -1;
	}
	BF_AllocateBlock(filedesc); //desmeuw to 1o block gia metadata
	blockNumber=BF_GetBlockCounter(filedesc)-1;	
	if(BF_ReadBlock(filedesc,blockNumber,(void*) &block)<0) {
		BF_PrintError("Error reading the block");
		return -1;
	}	
	*(char*)block ='h';  //metadata vazw to h gia arxeia swrou
	if(BF_WriteBlock(filedesc,blockNumber)<0) {
		BF_PrintError("Error writing the block");
		return -1;
	}
	BF_AllocateBlock(filedesc);	//desmeuw to 2o block gia egrafes
	blockNumber=BF_GetBlockCounter(filedesc)-1;
	if(BF_ReadBlock(filedesc,blockNumber,(void*) &block)<0) {
		BF_PrintError("Error reading the block");
		return -1;
	}		
	if(BF_WriteBlock(filedesc,blockNumber)<0) {
		BF_PrintError("Error writing the block");
		return -1;
	}
	if(BF_CloseFile(filedesc)<0) { //telos kleinw to arxeio pou anoiksa
		BF_PrintError("Error closing the file");
		return -1;
	}
	return 0;
}

int HP_OpenFile (char *filename)  {
	int bf=-1;
	int filedesc,blockNumber;
   	char *block;
	if ( (filedesc=BF_OpenFile(filename))<0 ) {
		BF_PrintError("Error opening the file");
		return -1;
	}
	blockNumber=0; //diavazw to block 0
	bf=BF_ReadBlock(filedesc,blockNumber,(void*) &block);
	if(bf==0) {
		if ( strcmp((char*)block,"h")==0  ) //elegxw an prokeitai gia arxei swrou
			{return filedesc;}
		else {   // den prokeitai gia arxeio swrou
			BF_CloseFile(filedesc);
                  	return -1;
		}
       	}
	else {
		BF_CloseFile(filedesc);
		return -1;
	}
}


int  HP_CloseFile (int filedesc) {
	int bf=-1;
	bf=BF_CloseFile(filedesc);
	if (bf==0) return 0;
	else  {
		BF_PrintError("Error closing the file");
		return -1;
	}
}   
 


int HP_InsertEntry (int filedesc,Record record) {
	char *block;
	int max_egrafes,teleutaio_block,poses_einai_mesa;
	max_egrafes = (BLOCK_SIZE-sizeof(int)) / (sizeof(Record)); //poses egrafes tha mpainoun se kathe block
	teleutaio_block=BF_GetBlockCounter(filedesc)-1;
	if (BF_ReadBlock(filedesc,teleutaio_block,(void*) &block)<0) {
		BF_PrintError("Error reading the block");
		return -1;
	}
	poses_einai_mesa = *(int*)block; //poses egrafes vriskontai sto teleutaio block pou diavasa
	if (poses_einai_mesa < max_egrafes ) {		//an i eggrafi xwraei na mpei
		memcpy(block+sizeof(int)+poses_einai_mesa*sizeof(Record) , &record , sizeof(Record)); // mpike i egrafi
		*(int*)block=*(int*)block+1;	//o metritis tou block auksanei kata 1 
		if(BF_WriteBlock(filedesc,teleutaio_block)<0) {		  //write pisw sto disko
			BF_PrintError("Error writing the block");
			return -1;
		}
		return 0;					
	}
	else  {   
		BF_AllocateBlock(filedesc); //ftiaxnw kainouri block gt den xwraei na mpei i egrafi
		if(BF_ReadBlock(filedesc,BF_GetBlockCounter(filedesc)-1,(void*) &block)<0) {
			BF_PrintError("Error reading the block");
			return -1;
		}
		*(int*)block=1; //sto kainourio block mpainei i prwti egrafi
		memcpy(block+sizeof(int), &record, sizeof(Record));
		if(BF_WriteBlock(filedesc,BF_GetBlockCounter(filedesc)-1)<0) {
			BF_PrintError("Error writing the block");
			return -1;
		}
		return 0;
	}	
				
} 

int HP_DeleteEntry (int filedesc,char *fieldName,void *value) {
	Record temp;
	char *block;
	int bf,i,j,k;
	if(value==NULL || fieldName==NULL) return -1;
	if(strcmp(fieldName,"id")!=0) {
		if(strcmp(fieldName,"name")!=0) {
			if(strcmp(fieldName,"surname")!=0) {
				if(strcmp(fieldName,"city")!=0)
					return -1;
			}
		}
	}
	for(i=1;i<BF_GetBlockCounter(filedesc);i++) {    //gia ola ta blocks
		bf=BF_ReadBlock(filedesc,i,(void*) &block);  //diavasw kathe block
		if(bf==0) {
			for(j=0;j<*(int*)block;j++) {  //gia kathe eggafi
				memcpy(&temp,block+sizeof(int)+j*sizeof(Record),sizeof(Record));
				if(strcmp(fieldName,"id")==0)  {  //an to fieldname einai id
					if(*(int*)value==temp.id) {  //an i timi tou id einai value		
						for(k=j;k<*(int*)block-1;k++) {
							memcpy(block+sizeof(int)+k*sizeof(Record), block+sizeof(int)+(k+1)*sizeof(Record) ,sizeof(Record) );
							j--;	
						}
						*(int*)block=*(int*)block-1;
					}
				}
				else if(strcmp(fieldName,"name")==0) {  //an to fieldname einai name
					if(strcmp((char*)value,temp.name)==0) { //an i timi tou name einai value
						for(k=j;k<*(int*)block-1;k++) {
							memcpy(block+sizeof(int)+k*sizeof(Record), block+sizeof(int)+(k+1)*sizeof(Record) ,sizeof(Record) );
							j--;	
						}
						*(int*)block=*(int*)block-1;
					}
				}
				else if(strcmp(fieldName,"surname")==0) {  //an to fieldname einai surname
					if(strcmp((char*)value,temp.surname)==0) { //an i timi tou surname einai value
						for(k=j;k<*(int*)block-1;k++) {
							memcpy(block+sizeof(int)+k*sizeof(Record), block+sizeof(int)+(k+1)*sizeof(Record) ,sizeof(Record) );
							j--;	
						}
						*(int*)block=*(int*)block-1;
					}
				}
				else {  //an to fieldname einai city
					if(strcmp((char*)value,temp.city)==0) { //an i timi tou city einai value
						for(k=j;k<*(int*)block-1;k++) {
							memcpy(block+sizeof(int)+k*sizeof(Record), block+sizeof(int)+(k+1)*sizeof(Record) ,sizeof(Record) );
							j--;	
						}
						*(int*)block=*(int*)block-1;
					}
				}
			}
			bf=BF_WriteBlock(filedesc,i);
		}
		else {
			BF_PrintError("Error reading the block");
			return -1;
		}
	}
	if (bf==0) return 0; //an egine swsta to teleutaio write tou block
	else {
		BF_PrintError("Error writing the block");
		return -1;
	}	
}

void HP_GetAllEntries (int filedesc,char *fieldName,void *value){
	Record temp;
	int bf,i,j,metritis_block=0;
	char *block;
	if(fieldName!=NULL) {
		if(strcmp(fieldName,"id")!=0) {
			if(strcmp(fieldName,"name")!=0) {
				if(strcmp(fieldName,"surname")!=0) {
					if(strcmp(fieldName,"city")!=0) {
						if(value!=NULL) {
							printf("Exete dwsei lathos fieldname!\n");	
							return;
						}	
					}
				}
			}
		}
	}
	else if(value!=NULL) {
		printf("Exete dwsei lathos fieldname!\n");	
		return;
	}
	if(value==NULL) { //ta ektipwnoume ola
		for(i=1;i<BF_GetBlockCounter(filedesc);i++) {    //gia ola ta blocks
			//printf("vriskomaste sto block %d\n",i );	
			bf=BF_ReadBlock(filedesc,i,(void*) &block);  //diavasw kathe block
			metritis_block++;	
			if(bf==0) {
				for(j=0;j<*(int*)block;j++) {  //gia kathe eggafi
					memcpy(&temp,block+sizeof(int)+j*sizeof(Record),sizeof(Record));
					printf("%d %s %s %s!\n" , temp.id,temp.name,temp.surname,temp.city);
				}
			}
			else { 
				BF_PrintError("Error reading the block");
				return;
			}	
		}
		printf("Diavastikan %d blocks !\n",metritis_block);
		return;
	}	
	for(i=1;i<BF_GetBlockCounter(filedesc);i++) {    //gia ola ta blocks
		bf=BF_ReadBlock(filedesc,i,(void*) &block);  //diavasw kathe block
		metritis_block++;
		if(bf==0) {
			for(j=0;j<*(int*)block;j++) {  //gia kathe eggafi
				memcpy(&temp,block+sizeof(int)+j*sizeof(Record),sizeof(Record));
				if(strcmp(fieldName,"id")==0)  {  //an to fieldname einai id
					if(*(int*)value==temp.id) {  //an i timi tou id einai value	
						printf("%d %s %s %s!\n" , temp.id,temp.name,temp.surname,temp.city);
					}
				}
				else if(strcmp(fieldName,"name")==0) {  //an to fieldname einai name
					if(strcmp((char*)value,temp.name)==0) { //an i timi tou name einai value
						printf("%d %s %s %s!\n" , temp.id,temp.name,temp.surname,temp.city);
					}
				}
				else if(strcmp(fieldName,"surname")==0) {  //an to fieldname einai surname
					if(strcmp((char*)value,temp.surname)==0) { //an i timi tou surname einai value
						printf("%d %s %s %s!\n" , temp.id,temp.name,temp.surname,temp.city);
					}
				}
				else {  //an to fieldname einai city
					if(strcmp((char*)value,temp.city)==0) { //an i timi tou city einai value
						printf("%d %s %s %s!\n" , temp.id,temp.name,temp.surname,temp.city);
					}
				}
			}
		}
		else {
			BF_PrintError("Error reading the block");
			return;
		}
	}
	printf("Diavastikan %d blocks !\n",metritis_block);
	return;
}


