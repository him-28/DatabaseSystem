#ifndef __Sorted__ 
#define __Sorted__


#include "Record.h"  


int  Sorted_CreateFile    (char *filename);

int  Sorted_OpenFile      (char *filename);

int  Sorted_CloseFile     (int filedesc);

int  Sorted_InsertEntry   (int  filedesc,Record record);

int  Sorted_DeleteEntry   (int  filedesc,char *fieldName,void *value);

void Sorted_GetAllEntries (int  filedesc,char *fieldName,void *value);



#endif
