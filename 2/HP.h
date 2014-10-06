#ifndef __HP__ 
#define __HP__


#include "Record.h"  


int  HP_CreateFile    (char *filename);

int  HP_OpenFile      (char *filename);

int  HP_CloseFile     (int  filedesc);

int  HP_InsertEntry   (int  filedesc,Record record);

int  HP_DeleteEntry   (int  filedesc,char *fieldName,void *value);

void HP_GetAllEntries (int  filedesc,char *fieldName,void *value);



#endif
