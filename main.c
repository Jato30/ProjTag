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
// #include "network.h"


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
    PROF_ESC G;

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
    if(read_prof_escola(&G, file) != 0){ // função parser que lê o arquivo texto e preenche a variavel grafo
        printf("Couldn't read the network.");
        return 1;
    }

    fclose(file);






    int i;
    printf("\n");
    printf("\t IMPRIME INFOS LIDAS\n\n");
    for(i = 0; i < 100; i++){
        printf("Professor %d:  habilitacoes: %d  preferencias: E%d E%d E%d E%d E%d\n", G.professor[i].id, G.professor[i].habilitacoes, G.professor[i].preferencia[i], G.professor[i].preferencia[1], G.professor[i].preferencia[2], G.professor[i].preferencia[3], G.professor[i].preferencia[4]);
    }
    printf("\n");
    for(i = 0; i < 50; i++){
        printf("Escola %d:  exige %d habilitacoes e tem %d vagas\n", G.escola[i].id, G.escola[i].exigencia, G.escola[i].vagas);
    }
    printf("\n");


    // *********** IMPRIMIR GRAFO **********

    int j;
    printf("\n");
    printf("\n\t**************\t PROFESSOR (id) :\t { E id  DAS ESCOLAS QUE PODEM ACEITA-LO }\t ***************\n\n");
    for(i = 0; i < 100; i++){
        printf("P(%d): {", G.grafo.vertex_p[i].id);

        for(j = 0; j < G.grafo.vertex_p[i].degree; j++){
            printf("E%d", G.grafo.vertex_p[i].edge[j].target);
            if(j+1 < G.grafo.vertex_p[i].degree){
                printf(";");
            }
        }
        printf("}\n");
    }
    printf("\n");
    
    printf("\n\t**************\t ESCOLAS (id) :\t { P id  DOS PROFESSORES QUE POSSUEM HABILITACOES SUFICIENTES }\t ***************\n");
    for(i=0; i<50; i++){
        printf("\nE(%d): {", G.grafo.vertex_e[i].id);

        for(j=0; j < G.grafo.vertex_e[i].degree; j++){
            printf("P%d", G.grafo.vertex_e[i].edge[j].target);
            if(j+1 < G.grafo.vertex_e[i].degree){
                printf(";");
            }
        }
        printf("}\n");
    }



    // libera a memória
    free_memo(&G);
    printf("\n");
    return 0;
}