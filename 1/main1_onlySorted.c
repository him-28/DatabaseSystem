#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BF.h"                                         /* Include BF library */
#include "HP.h"                                        /* Include HP librarty */
#include "Sorted.h"                                 /* Include Sorted library */
#include "Record.h"        /* DEFINE THE RECORD STRUCTURE IN THIS LIBRARY!!!! */

#define NAMES_SIZE 12
#define SURNAMES_SIZE 11
#define CITIES_SIZE 12



Record randomRecord();

char *names[] = {"AUANASIA", "DIMITRIS", "ELENH", "GIWRGOS", "JERRY", "MARINA", "NANCY",
		         "PANOS", "PETROS", "TOM", "VASSILIS", "YANNIS"};

char *surnames[] = {"PANAGIWTOU", "ASHMOGLOU", "ARAVANTHS", "PAPAPETROU", "LAMBROU", "THEOLOGOU",
		            "ZERBA", "LEE-LEWIS", "SPANOU", "ZIGKOLHS", "TZIOKOU"};
		            
char *cities[] = {"KAVALA", "DRAMA", "GIANNENA", "KOZANH", "KALAMATA", "PATRA",
                  "TRIPOLI", "EDESSA", "AMFISSA", "PREBEZA", "MESOLOGGI", "SPARTH"};

int main( int argc, char* argv[] ){

	int i;
	int fdsrt1;
	int fdsrt2;
	Record rec;
	
	srand( time(NULL) );
	BF_Init();

    if ( Sorted_CreateFile("srt1") < 0 )
    {
         BF_PrintError("Error creating file srt1");
		 return -1;     
    }
    
    if ( (fdsrt1 = HP_OpenFile("srt1")) < 0 )   
    {                                 /* This function call must return error */
		BF_PrintError("Error opening file srt1 (You should see this message)");
    }
	else
    {
        printf("HP_OpenFile should have failed!");
        return -1;
    }
    

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////    
    if ( (fdsrt1 = Sorted_OpenFile("srt1")) < 0 )   
    {
		BF_PrintError("Error opening file srt1");
		return -1;
	}
	
	if ( (fdsrt2 = Sorted_OpenFile("srt1")) < 0 )   
    {
		BF_PrintError("Error opening file srt1");
		return -1;
	}
////////////////////////////////////////////////////////////////////////////////
    rec.id = 27;
    strcpy( rec.name, "DIMITRIS" );
    strcpy( rec.surname, "ARAVANTHS" );
    strcpy( rec.city, "PREBEZA" );

	if( Sorted_InsertEntry (fdsrt2, rec) < 0)
    {
            BF_PrintError("Error inserting record in file srt1");
		    Sorted_CloseFile(fdsrt2);
		    return -1;
    }
    printf("\nRecord (%d, %s, %s, %s) was successfully inserted in file srt1\n", rec.id, rec.name, rec.surname, rec.city);
	
	i = 0;
	while ( i < 100 )                    /* Insert 100 records in sorted file */
    {

		Record r = randomRecord();
		if( Sorted_InsertEntry (fdsrt1, r) < 0)
        {
            BF_PrintError("Error inserting record in file srt1");
		    Sorted_CloseFile(fdsrt1);
		    return -1;
        }
        printf("\nRecord (%d, %s, %s, %s) was successfully inserted in file srt1\n", r.id, r.name, r.surname, r.city);
		i++;
	}
	
	rec.id = 18;
    strcpy( rec.name, "PETROS" );
    strcpy( rec.surname, "ASHMOGLOU" );
    strcpy( rec.city, "EDESSA" );

	if( Sorted_InsertEntry (fdsrt2, rec) < 0)
    {
            BF_PrintError("Error inserting record in file srt1");
		    Sorted_CloseFile(fdsrt2);
		    return -1;
    }
    printf("\nRecord (%d, %s, %s, %s) was successfully inserted in file srt1\n", rec.id, rec.name, rec.surname, rec.city);
////////////////////////////////////////////////////////////////////////////////
	
                                 /* Print everything that exists in file srt1 */
	printf("Printing everything in file srt1\n");
	Sorted_GetAllEntries(fdsrt1, NULL, NULL);
	
	if( Sorted_CloseFile(fdsrt1) < 0)
	{
        BF_PrintError("Error closing file srt1");
        return -1;
    }
    
    if( Sorted_CloseFile(fdsrt2) < 0)
	{
        BF_PrintError("Error closing file srt1");
        return -1;
    }
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
	return 0;
}


Record randomRecord(){

	Record rec;
	int idx;

	rec.id = rand() % 50 + 1;
	idx = drand48() * (NAMES_SIZE - 1);
	strcpy( rec.name, names[idx] );
	idx = drand48() * (SURNAMES_SIZE - 1);
	strcpy( rec.surname, surnames[idx] );
	idx = drand48() * (CITIES_SIZE - 1);
	strcpy( rec.city, cities[idx] );
    
	return rec;
}
