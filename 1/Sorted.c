#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BF.h"
#include "Record.h"  
#include "Sorted.h"
#include "HP.h"


int Sorted_CreateFile (char *filename) {
	int filedesc,blockNumber;
	char *block;
	if(BF_CreateFile(filename)<0 )	{
		BF_PrintError("Error creating the file");
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
	*(char*)block ='s';  //metadata vazw to s gia arxeia taxinomimena
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


int Sorted_OpenFile (char *filename){
	int bf=-1;
	int filedesc,blockNumber;
   	char *block;
	if ((filedesc=BF_OpenFile(filename))<0 ) {
		BF_PrintError("Error opening the file");
		return -1;
	}
	blockNumber=0; //diavazw to block 0
	bf=BF_ReadBlock(filedesc,blockNumber,(void*) &block);
	if(bf==0) {
		if ( strcmp((char*)block,"s")==0  ) //elegxw an prokeitai gia taxinomimeno arxeio
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


int Sorted_CloseFile (int filedesc){
     int bf=-1;
     bf=BF_CloseFile(filedesc);
     if (bf==0) return 0;
     else  {
	BF_PrintError("Error closing the file");
	return -1;
	}
}


int Sorted_InsertEntry  (int filedesc,Record record){
	int i,j,flag=0,lastblock;
	char *block;
	Record temp,prosorinos,lastrec;
	lastblock=BF_GetBlockCounter(filedesc)-1;
	BF_ReadBlock(filedesc,lastblock,(void*) &block); 
	memcpy(&lastrec,block+sizeof(int)+(*(int*)block-1)*sizeof(Record),sizeof(Record));
	for(i=1;i<BF_GetBlockCounter(filedesc);i++) {    //gia ola ta blocks
		BF_ReadBlock(filedesc,i,(void*) &block);  //diavasw kathe block
		for(j=0;j<*(int*)block;j++) {  //gia oles tis eggafes
			memcpy(&temp,block+sizeof(int)+j*sizeof(Record),sizeof(Record)); //fernw stin mnimi kathe egrafi
			if(record.id<=temp.id) {  	
				memcpy(&prosorinos,&temp,sizeof(Record));
				memcpy(block+sizeof(int)+j*sizeof(Record),&record,sizeof(Record));
				flag=1;
				BF_WriteBlock(filedesc,i);
			}
	
			if(flag==1) break;			
		}
		if(flag==1) break;
		
	}
	if (flag==0) {
				BF_AllocateBlock(filedesc);
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
				
			
	}
	if(prosorinos.id != lastrec.id ) {
		Sorted_InsertEntry(filedesc,prosorinos);

	}
	
	else {
		BF_AllocateBlock(filedesc);
		if(BF_ReadBlock(filedesc,BF_GetBlockCounter(filedesc)-1,(void*) &block)<0) {
				BF_PrintError("Error reading the block");
				return -1;
		}
		*(int*)block=1; //sto kainourio block mpainei i prwti egrafi
		memcpy(block+sizeof(int), &lastrec, sizeof(Record));
		if(BF_WriteBlock(filedesc,BF_GetBlockCounter(filedesc)-1)<0) {
			BF_PrintError("Error writing the block");
			return -1;
		}
	}
	return 0;	
}



int Sorted_DeleteEntry (int filedesc,char *fieldName,void *value){
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




void Sorted_GetAllEntries (int filedesc,char *fieldName,void *value){
	Record first,last,temp;
	int pb=1,tb,mb,j;
	int metritis=0;
	tb=BF_GetBlockCounter(filedesc)-1;
	char *block;
	int dmb,amb;
	int flag_deksia=0;
	int flag_aristera=0;
	if(value==NULL || strcmp(fieldName,"id")!=0  ) { //an mas dwsei NULL i to pedio den einai id ektelw seiriaki.kalw tin HP_get gia seiriaki
		HP_GetAllEntries(filedesc,fieldName,value);
		return;
	}	
	//diaforetika ektelo diadiki ws pros to id 
	while(pb<=tb) {  		
		mb=(pb+tb)/2;
		if (BF_ReadBlock(filedesc,mb,(void*) &block)<0) {
			BF_PrintError("Error reading the block");
			return;
		}
		metritis++;
		memcpy(&first,block+sizeof(int),sizeof(Record)); //diavazw tin 1i egrafi
		memcpy(&last,block+sizeof(int)+(*(int*)block-1)*sizeof(Record),sizeof(Record));
		if(*(int*)value<first.id) { 
			tb=mb-1;//to teleutaio block eine to proigoumeno tou middle(kinoumaste aristera)
		}
		else if(*(int*)value>last.id) {
			pb=mb+1;//to prwto block eine to epomeno tou middle(kinoumaste deksia)
		}
		else  {	//to vrikame to block
                	break;
		}
	}
	//printf("To block pou vrikame einai to %d\n",mb);
	for(j=0;j<*(int*)block;j++) {  //gia kathe eggafi mesa sto block pou vrikame parapanw
		memcpy(&temp,block+sizeof(int)+j*sizeof(Record),sizeof(Record));
		if(temp.id==*(int*)value) {
			printf("%d %s %s %s!\n" , temp.id,temp.name,temp.surname,temp.city);
			if(j ==  (*(int*)block-1)    ) //an i teleutaia egrafi einai to zitoumeno
				{ flag_deksia=1; dmb=mb; } //prepei na elegksoume k sta deksia mplok
			if(j==0)//an i prwti egrafi einai to zitoumeno pame aristera
				{ flag_aristera=1; amb=mb; } //prepei na elegksoume k sta aristera mplok
		}
	}
 	while(flag_deksia==1){//oso i teleutaia egrafi tou block pou eimaste einai to zitoumeno pame deksia
		flag_deksia=0;
		dmb++; //pame sto epomeno deksia block
		if(dmb == (BF_GetBlockCounter(filedesc)) ) break; //an ftasame sto teleutaio block stamatame
		if (BF_ReadBlock(filedesc,dmb,(void*) &block)<0) {
			BF_PrintError("Error reading the block");
			return;
		}
		metritis++;
		//printf("Eimaste sto block %d!\n",dmb);
		for(j=0;j<*(int*)block;j++) {  
			memcpy(&temp,block+sizeof(int)+j*sizeof(Record),sizeof(Record));
			if(temp.id==*(int*)value) {
				printf("%d %s %s %s!\n" , temp.id,temp.name,temp.surname,temp.city);
				if(j==(*(int*)block-1))  {flag_deksia=1;}
			}
		}
	}
	while(flag_aristera==1){//oso i prwti egrafi tou block pou eimaste einai to zitoumeno pame aristera
		flag_aristera=0;
		amb--;
		if (amb==0) break;
		if (BF_ReadBlock(filedesc,amb,(void*) &block)<0) {
			BF_PrintError("Error reading the block");
			return;
		}
		//printf("Eimaste sto block %d!\n",amb);
		metritis++;
		for(j=0;j<*(int*)block;j++) {  
			memcpy(&temp,block+sizeof(int)+j*sizeof(Record),sizeof(Record));
			if(temp.id==*(int*)value) {
				printf("%d %s %s %s!\n" , temp.id,temp.name,temp.surname,temp.city);
				if(j==0) {flag_aristera=1; }
			}
		}
	}	
	printf("Diavastikan %d blocks!\n",metritis);
	return;
}
