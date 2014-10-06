#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BF.h"                                         /* Include BF library */
#include "HP.h"                                        /* Include HP librarty */
#include "Record.h"        /* DEFINE THE RECORD STRUCTURE IN THIS LIBRARY!!!! */


int Join_MergeSort(char *inputName1,char *inputName2,char *outputName)  {
	int m;
	int filedesc1,filedesc2,filedesc3;
	Record rec1,rec2;
	int done1=0,done2=0; //flag gia na kserw poio apo ta arxeia teleiwnei
	char *block1,*block2,*block3;
	int sunolo_blocks_1=1, trexon_block_1=1 , sunolo_recs_1=1 , trexon_rec_1=1;//metrites gia to 1o arxeio pou anoigw
	int sunolo_blocks_2=1, trexon_block_2=1 , sunolo_recs_2=1 , trexon_rec_2=1;//metrites gia to 1o arxeio pou anoigw
	int                    trexon_block_3=1 , sunolo_recs_3   , trexon_rec_3=0;//metrites gia to output arxeio

	if((filedesc1=HP_OpenFile(inputName1))<0) {
		return -1;
	}
	if((filedesc2=HP_OpenFile(inputName2))<0) {
		return -1;
	}
	if(HP_CreateFile(outputName)<0) return -1;
	
	if((filedesc3=HP_OpenFile(outputName))<0) {
		return -1;
	}
	
	if (BF_ReadBlock(filedesc1,1,(void*) &block1)<0) { //diavazw to 1o block apo to 1o arxeio
		BF_PrintError("Error reading the block from the 1st file");
		return -1;
	}
	
	if (BF_ReadBlock(filedesc2,1,(void*) &block2)<0) { //diavazw to 1o block apo to 2o arxeio
		BF_PrintError("Error reading the block from the 2nd file");
		return -1;
	}

	if (BF_ReadBlock(filedesc3,1,(void*) &block3)<0) { //diavazw to 1o block apo to output arxeio
		BF_PrintError("Error reading the block from the output file");
		return -1;
	}
		//////Arxikopoihsh metritwn//////////
	
	sunolo_blocks_1=BF_GetBlockCounter(filedesc1)-1;		//sunolika blocks tou 1ou arxeiou pou anoigw
	sunolo_blocks_2=BF_GetBlockCounter(filedesc2)-1;		//sunolika blocks tou 2ou arxeiou pou anoigw	
	sunolo_recs_3 = (BLOCK_SIZE-sizeof(int)) / (sizeof(Record)); //max recs pou mpainoun s kathe block tou 3ou arxeiou

	trexon_block_1=1;				////metrites gia to 1o arxeio pou anoigw////////////
	sunolo_recs_1=*(int*)block1;
	trexon_rec_1=0;

	trexon_block_2=1;				////metrites gia to 2o arxeio pou anoigw////////////
	sunolo_recs_2=*(int*)block2;
	trexon_rec_2=0;

	trexon_block_3=1;				////metrites gia to output arxeio pou ftiaxnw////////////
	trexon_rec_3=0;
	
	if(*(int*)block1==0 ) { //an to 1o arxeio pou anoiksa dn exei egrafes mesa paw k vazw egrafes kateuthian apo to arxeio 2
		done1=1;
	}

	if(*(int*)block2==0 ) { //an to 2o arxeio pou anoiksa dn exei egrafes mesa paw k vazw egrafes kateuthian apo to arxeio 1
		done2=1;
	}

	if(done1==1 && done2==1) { 
		printf("Ta arxeia eisodou einai kai ta 2 adeia!\n");
		return -1;  //einai k ta 2 arxeia adeia !
	}

	if(done1==0 && done2==0) { //an exoun k ta 2 arxeia egrafes
	
		while(trexon_block_1<=sunolo_blocks_1 && trexon_block_2<=sunolo_blocks_2) //mexri na teleiwsei 1 apo ta 2 arxeia//oso kai ta 2 arxeia exoun egrafes
		{
			memcpy(&rec1,block1+sizeof(int)+trexon_rec_1*sizeof(Record),sizeof(Record)); //fernw tis recs apo to 1o arxeio
			memcpy(&rec2,block2+sizeof(int)+trexon_rec_2*sizeof(Record),sizeof(Record)); //fernw tis recs apo to 2o arxeio
			if(rec1.id < rec2.id) {
				memcpy(block3+sizeof(int)+trexon_rec_3*sizeof(Record),&rec1,sizeof(Record));//antigrafw apo 1o arxeio
				trexon_rec_3++;
				trexon_rec_1++;
			}
			else {
				memcpy(block3+sizeof(int)+trexon_rec_3*sizeof(Record),&rec2,sizeof(Record));//antigrafw apo 2o arxeio
				trexon_rec_3++;
				trexon_rec_2++;
			}

			if(trexon_rec_1==sunolo_recs_1) { //an eftasa stin teleutaia egrafi prepei na paw sto epomeno block --1o arxeio
				if(trexon_block_1==sunolo_blocks_1){ //an einai to teleutaio block teleiwsame apo to 1o arxeio
					done1=1; //flag gia na kserw oti teleiwse to 1o arxeio
					*(int*)block3=trexon_rec_3; //grafw pisw sto disko
					if(BF_WriteBlock(filedesc3,trexon_block_3)<0) {
						BF_PrintError("Error writing the block");
					}
					break;
				}
				trexon_block_1++;  //diaforetika diavazw to epomeno block
				if (BF_ReadBlock(filedesc1,trexon_block_1,(void*) &block1)<0) { //diavazw to epomeno block apo to 1o arxeio
					BF_PrintError("Error reading the block");
				}	
				sunolo_recs_1=*(int*)block1; //to plithos twn egrafwn tou epomenou block
				trexon_rec_1=0;
			}

			if(trexon_rec_2==sunolo_recs_2) { //an eftasa stin teleutaia egrafi prepei na paw sto epomeno block --2o arxeio
				if(trexon_block_2==sunolo_blocks_2){ //an einai to teleutaio block teleiwsame apo to 2o arxeio
					done2=1; //flag gia na kserw oti teleiwse to 2o arxeio
					*(int*)block3=trexon_rec_3; //grafw pisw sto disko
					if(BF_WriteBlock(filedesc3,trexon_block_3)<0) {
						BF_PrintError("Error writing the block");
					}
					break;
				} 
				trexon_block_2++;
				if (BF_ReadBlock(filedesc2,trexon_block_2,(void*) &block2)<0) { //diavazw to epomeno block apo to 2o arxeio
					BF_PrintError("Error reading the block");
				}	
				sunolo_recs_2=*(int*)block2; //to plithos twn egrafwn tou epomenou block
				trexon_rec_2=0;
			}

			if(trexon_rec_3==sunolo_recs_3) { //an gemise to block pou bazw egrafes prepei na desmeusw kainourio
				*(int*)block3=sunolo_recs_3; //grafw pisw sto disko
				if(BF_WriteBlock(filedesc3,trexon_block_3)<0) {
					BF_PrintError("Error writing the block");
				}
				trexon_block_3++;
				if(BF_AllocateBlock(filedesc3)<0 )	{  //desmeuw to epomeno block sto output arxeio
					BF_PrintError("Error allocating the block");
				}
				if(BF_ReadBlock(filedesc3,trexon_block_3,(void*) &block3)<0){//diavazw to epomeno block apo to output arxeio
					BF_PrintError("Error reading the block");
				}
				trexon_rec_3=0; 	//midenizw ton metriti
			}
	
		}
	}
	if(done1==1) { //teleiwse to 1o arxeio , ara vazoume egrafes apo to 2o arxeio
		while(trexon_block_2<=sunolo_blocks_2) {
			memcpy(&rec2,block2+sizeof(int)+trexon_rec_2*sizeof(Record),sizeof(Record));//fernw tis recs apo 2o arxeio
			memcpy(block3+sizeof(int)+trexon_rec_3*sizeof(Record),&rec2,sizeof(Record));//antigrafw apo 2o arxeio
			trexon_rec_3++;
			trexon_rec_2++;
			if(trexon_rec_2==sunolo_recs_2){//an eftasa stin teleutaia egrafi prepei na paw sto epomeno block --2o arxeio
				if(trexon_block_2==sunolo_blocks_2){ //an einai to teleutaio block teleiwsame apo to 2o arxeio
					*(int*)block3=trexon_rec_3;
					if(BF_WriteBlock(filedesc3,trexon_block_3)<0) {
						BF_PrintError("Error writing the block");
					}
					break;
				}
				trexon_block_2++;
				if (BF_ReadBlock(filedesc2,trexon_block_2,(void*) &block2)<0){//diavazw to epomeno block apo to 2o arxeio
					BF_PrintError("Error reading the block");
				}	
				sunolo_recs_2=*(int*)block2; //to plithos twn egrafwn tou epomenou block
				trexon_rec_2=0;
			}
			
			if(trexon_rec_3==sunolo_recs_3) { //an gemise to block pou bazw egrafes prepei na desmeusw kainourio
				*(int*)block3=sunolo_recs_3;
				if(BF_WriteBlock(filedesc3,trexon_block_3)<0) {
					BF_PrintError("Error writing the block");
				}
				trexon_block_3++;
				if(BF_AllocateBlock(filedesc3)<0 )	{  //desmeuw to epomeno block sto output arxeio
					BF_PrintError("Error allocating the block");
				}
				if(BF_ReadBlock(filedesc3,trexon_block_3,(void*) &block3)<0){//diavazw to epomeno block apo to output
					BF_PrintError("Error reading the block");
				}
				trexon_rec_3=0; 	//midenizw ton metriti
			}
		}
	}


	if(done2==1) { //teleiwse to 2o arxeio , ara vazoume egrafes apo to 1o arxeio
		while(trexon_block_1<=sunolo_blocks_1) {
			memcpy(&rec1,block1+sizeof(int)+trexon_rec_1*sizeof(Record),sizeof(Record)); //fernw tis recs apo 1o arxeio
			memcpy(block3+sizeof(int)+trexon_rec_3*sizeof(Record),&rec1,sizeof(Record));//antigrafw apo 1o arxeio
			trexon_rec_3++;
			trexon_rec_1++;

			if(trexon_rec_1==sunolo_recs_1){//an eftasa stin teleutaia egrafi prepei na paw sto epomeno block -1o arxeio
				if(trexon_block_1==sunolo_blocks_1){ //an einai to teleutaio block teleiwsame apo to 1o arxeio
					*(int*)block3=trexon_rec_3;
					if(BF_WriteBlock(filedesc3,trexon_block_3)<0) {
						BF_PrintError("Error writing the block");
					}
					break;
				}
				trexon_block_1++;
				if(BF_ReadBlock(filedesc1,trexon_block_1,(void*) &block1)<0){//diavazw to epomeno block apo to 1o arxeio
					BF_PrintError("Error reading the block");
				}	
				sunolo_recs_1=*(int*)block1; //to plithos twn egrafwn tou epomenou block
				trexon_rec_1=0;
			}
			
			if(trexon_rec_3==sunolo_recs_3) { //an gemise to block pou bazw egrafes prepei na desmeusw kainourio
				*(int*)block3=sunolo_recs_3;
				if(BF_WriteBlock(filedesc3,trexon_block_3)<0) {
					BF_PrintError("Error writing the block");
				}
				trexon_block_3++;
				if(BF_AllocateBlock(filedesc3)<0 )	{  //desmeuw to epomeno block sto output arxeio
					BF_PrintError("Error allocating the block");
				}
				if(BF_ReadBlock(filedesc3,trexon_block_3,(void*) &block3)<0){//diavazw to epomeno block apo to output
					BF_PrintError("Error reading the block");
				}
				trexon_rec_3=0; 	//midenizw ton metriti
			}
		}
	} 

	HP_CloseFile(filedesc1);
	HP_CloseFile(filedesc2);
	HP_CloseFile(filedesc3);
	printf("OK!\n");
	return 0;

}



int Sort(char *fileName,int bufferSize)  {
	int i,j,k,l,done=0,fd,tempfd,omades_blocks=0;
	char onoma[15]; //to xreiazomaste gia na dimiourgw prosorina arxeia
	Record temp,min;
	int thesi_min;
	char *block;
	int trexon_block_3=1,sunolo_recs_3,trexon_rec_3=0;  //metrites gia to temp arxeio kathe fora
	
	sunolo_recs_3 = (BLOCK_SIZE-sizeof(int)) / (sizeof(Record));
	
	if(bufferSize<3) { //elegxw to megethos tou buffer//an einai <3 dn mporw na taksinomisw to arxeio
		printf("Den einai dunato na ginei i taksinomisi!\n");
		return -1;
	}

	//anoigw to arxeio
	if((fd=HP_OpenFile(fileName))<0) {
		return -1;
	}

	//Desmeuoume enan pinaka pou tha krataei tis dieuthniseis twn M-1 blocks kathe fora
	char **pinakas;
	pinakas = malloc((bufferSize-1)*sizeof(char*));

	//fernoume omades apo block kathe fora (M-1 blocks)
	//gia kathe omada block //////
	for(k=1;k<=BF_GetBlockCounter(fd)-1;k=k+bufferSize-1) { //gia kathe omada block o metritis auksanetai ana bufferSize-1 gia na pernoume tis omades 

		//fernw stin mnimi M-1 blocks kathe fora//
		//krataw se enan pinaka ta M-1 blocks kathe fora//Mia omada blocks kathe fora
		omades_blocks++; //metritis gia na kserw poses omades block exw diavasei
		for(i=1;i<bufferSize;i++) {  //apo 1 mexri buffeSize-1// topothetw tis dieuthinseis apo kathe block ston pinaka pou exw
			if (BF_ReadBlock(fd,i+k-1,(void*)(pinakas[i-1]))<0) { //topothetw tis dieuthinseis twn block ston pinaka
				done=1; //flag gia na kseroume oti teleiwsan ta blocks
				break;
			}
		}
		if(done==1) break; //an teleiwsan ola ta block stamataw

		//dimiourgoume prosorina arxeia pou tha xreiastoune stin taksinomisi kathe omadas block
		
		sprintf(onoma,"tmp%d",omades_blocks); //Ara tha ftiaxtei to tmp1,tmp2,tmp3 klp
		if(HP_CreateFile(onoma)<0) return -1;
		if((tempfd=HP_OpenFile(onoma))<0) { //anoigw ta arxeia pou ftiaxnw kathe fora
			return -1;
		}
		
	

		//Diavazoume oles tis egrafes pou exei mesa i omada block //vriskoume tis mikroteres apo kathe block
		for(i=1;i<bufferSize;i++) {
				memcpy(&min,pinakas[i-1]+sizeof(int),sizeof(Record)); //estw oti kathe fora i min rec einai i prwti egrafi tou kathe block
			for(j=0,j<=*(int*)pinakas[i-1];j++;){ //gia oles tis egrafes tou kathe block me tin seira
				memcpy(&temp,pinakas[i-1]+sizeof(int)+j*sizeof(Record),sizeof(Record));
				if(temp.id<min.id) {
					memcpy(&min,pinakas[i-1]+sizeof(int)+j*sizeof(Record),sizeof(Record));
					thesi_min=j;
				}
			}
			//prepei na svisw tin mikroteri egrafi pou vrika gia na min tin ksanaparw
			for(l=thesi_min;l<*(int*)pinakas[i-1]-1;l++) { //svinw me olisthisi apo tin thesi min pou vrika k meta
				memcpy(pinakas[i-1]+sizeof(int)+l*sizeof(Record), pinakas[i-1]+sizeof(int)+(l+1)*sizeof(Record) ,sizeof(Record) );
				*(int*)pinakas[i-1]=*(int*)pinakas[i-1]-1; //meiwnw ton metriti
			}
			

			
		}

		//tin mikroteri rec pou vrika(min) tha tin grapsw sto temp arxieo pou dimiourgia
		

		if (BF_ReadBlock(tempfd,trexon_block_3,(void*) &block)<0) { //diavazw to  block apo to temp arxeio
			BF_PrintError("Error reading the block from the output file");
			return -1;
		}
		
		memcpy(block+sizeof(int)+trexon_rec_3*sizeof(Record),&min,sizeof(Record));//antigrafw apo to min pou vrika
		trexon_rec_3++; //i egrafi mpike sto temp arxeio
		*(int*)block=trexon_rec_3; //grafw pisw sto disko
		if(BF_WriteBlock(tempfd,trexon_block_3)<0) {
			BF_PrintError("Error writing the block");
		}
		
		//eksetazw an prepei na paw sto epomeno block tou temp arxeiou
		//an gemise paw sto epomeno block
		if(trexon_rec_3==sunolo_recs_3) { //an gemise to block pou bazw egrafes prepei na desmeusw kainourio
				*(int*)block=sunolo_recs_3;
				if(BF_WriteBlock(tempfd,trexon_block_3)<0) {
					BF_PrintError("Error writing the block");
				}
				trexon_block_3++;
				if(BF_AllocateBlock(tempfd)<0 )	{  //desmeuw to epomeno block sto output arxeio
					BF_PrintError("Error allocating the block");
				}
				if(BF_ReadBlock(tempfd,trexon_block_3,(void*) &block)<0){//diavazw to epomeno block apo to output
					BF_PrintError("Error reading the block");
				}
				trexon_rec_3=0; 	//midenizw ton metriti
			}
		
		

	}
	return 0;
}

