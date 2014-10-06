DatabaseSystem
==============

Deployment of a simple low level database system in C
The project handles records and blocks in block level inside the disk
Also, it is using indexes to observe the speed difference

Records are:

typedef struct{
  int id,
  char name[15],
  char surname[20],
  char city[10];
}Record;

