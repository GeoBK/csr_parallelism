#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <omp.h>

#include "sort.h"
#include "edgelist.h"

// Order edges by id of a source vertex, 
// using the Counting Sort
// Complexity: O(E + V)
void countSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges) {

    
    int i;
    int key;
    int pos;

    // auxiliary arrays, allocated at the start up of the program
    int *vertex_cnt = (int*)malloc(numVertices*sizeof(int)); // needed for Counting Sort

    for(i = 0; i < numVertices; ++i) {
        vertex_cnt[i] = 0;
    }

    // count occurrence of key: id of a source vertex
    for(i = 0; i < numEdges; ++i) {
        key = edges[i].src;
        vertex_cnt[key]++;
    }

    // transform to cumulative sum
    for(i = 1; i < numVertices; ++i) {
        vertex_cnt[i] += vertex_cnt[i - 1];
    }

    // fill-in the sorted array of edges
    for(i = numEdges - 1; i >= 0; --i) {
        key = edges[i].src;
        pos = vertex_cnt[key] - 1;
        edges_sorted[pos] = edges[i];
        vertex_cnt[key]--;
    }


    free(vertex_cnt);

}


void radixSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges) {
    int i,occurence_map[10];
    for(i=0;i<10;i++){
        occurence_map[i]=0;
    }
    int number_of_digits=0;
    while(numVertices!=0){
        numVertices/=10;
        number_of_digits++;
    }
    int d;
    for(d=0;d<number_of_digits;d++){
        for(i=0; i<numEdges; i++){
            edges[i].key = edges[i].src%10;
            if(edges[i].src!=0) edges[i].src=edges[i].src/10;
            occurence_map[edges[i].key]++;
        }
        for(i=1;i<10;i++){
            occurence_map[i]=occurence_map[i]+occurence_map[i-1];
        }
        for(i=numEdges-1;i>=0;i--){
            edges_sorted[occurence_map[edges[i].key]--]=edges[i];
        }
        for(i=0;i<numEdges;i++){            
            edges[i]=edges_sorted[i];
        }
        
    }
    
}
