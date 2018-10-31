// #ifdef _WIN32
// 	#include "windows.h"
// #elif __linux__
// 	#include <unistd.h>
// #else
// 	#error "Unknown compiler"
// #endif

#include <stdio.h>
#include <stdlib.h>
#include "readgml.h"
#include "struct.h"
#include "network.h"


int main(int argc, char**argv){
    // if(argc < 2){
    //     printf("Falha ao receber argumentos.\n");
    //     return 1;
    // }


    // arquivo texto contendo informações do grafo
    FILE* file = fopen("./entrada.txt", "r");
    if(file == NULL) {
        fprintf(stderr, "Can't open output file \"%s\" !\n", "./entrada.txt");
        return 1;
    }


    // grafo lido do arquivo texto
    PROF professor[100];
    ESCOLA escola[50];

    /* Grafo é preenchido com:
        número de matérias obrigatórias do curso de Ciência da Computação (nvertices)
        flag (directed) indicando que é direcionado (1)
        array de vértices contendo:
        id do vértice de [0 a nvertices-1]. 0 para Ciência da computação (vértice raiz para tornar o grafo conectado), as matérias começam em 1
        grau do vértice (quantidade de arestas que saem)
        string (label) contendo o nome completo da matéria
        array de arestas contendo:
        id do vértice de destino
        peso da aresta.
            ** IMPORTANTE **
            O peso da aresta se dá da seguinte forma:
            O peso das arestas que saem de um vértice v é inicializado com o número de créditos de v
            O peso de uma matéria é o weight das arestas que CHEGAM a ela (representa a dificuldade pra concluir a materia)
    */
    if(read_prof_escola(&professor, &escola, file) != 0){ // função parser que lê o arquivo texto e preenche a variavel grafo
        printf("Couldn't read the network.");
        return 1;
    }

    fclose(file);

    /*
        Os pesos foram inicializados com o número de créditos da matéria,
        esta função multiplica hardcoded esse valor por um valor empírico n inteiro | 1 <= n <= 3
    */
    // preenchePeso(&grafo); // A partir de aqui, está tudo pronto para resolver as questões do Projeto






    // printf("\n\t##### CURSO: %s #####\n\n", grafo.vertex[0].label);


    // OrdenacaoTopologica(grafo);
    // CaminhoCritico(grafo);




    // *********** IMPRIMIR GRAFO **********

    // int i;
    // for(i = 0; i < grafo.nvertices; i++){
    //     printf("\n(%d): %s\n", grafo.vertex[i].id, grafo.vertex[i].label);
    //     printf("Pre requisitos:\n");
    //     int j, k;
    //     for(j = 0; j < grafo.nvertices; j++){
    //         for(k = 0; k < grafo.vertex[j].degree; k++){
    //             if(i == grafo.vertex[j].edge[k].target){
    //                 printf("\t(%d): %s\n", grafo.vertex[j].id, grafo.vertex[j].label);
    //             }
    //         }
    //     }
    // }



    // libera a memória
    // free_network(&grafo);
    printf("\n");
    return 0;
}