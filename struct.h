#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct{
	int id;
	int habilitacoes;
	int preferencia[5];
} PROF;

typedef struct{
	int id;
	int exigencia;
	int vagas;
} ESCOLA;

typedef struct{
	PROF professor[100];
	ESCOLA escola[50];
} NETWORK;

#endif // __STRUCT_H__
