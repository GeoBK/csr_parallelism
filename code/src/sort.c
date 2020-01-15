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
    
    int number_of_digits=0;
    while(numVertices!=0){
        numVertices/=10;
        number_of_digits++;
    }
    int d;
    for(d=0;d<number_of_digits;d++){
        for(i=0;i<10;i++){
            occurence_map[i]=0;
        }
        
        for(i=0; i<numEdges; i++){
            edges[i].key = edges[i].trimmed_digit%10;
            if(edges[i].trimmed_digit!=0) edges[i].trimmed_digit=edges[i].trimmed_digit/10;
            occurence_map[edges[i].key]++;
        }
        for(i=1;i<10;i++){
            occurence_map[i]=occurence_map[i]+occurence_map[i-1];
        }
        for(i=numEdges-1;i>=0;i--){
            edges_sorted[--occurence_map[edges[i].key]]=edges[i];
        }
        for(i=0;i<numEdges;i++){            
            edges[i]=edges_sorted[i];            
        }
        //printEdgeArray(edges, numEdges); 
        
    }
    
}

void radixSortEdgesBySourceParallel(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges) {
    
    int i,occurence_map[10];
    
    int number_of_digits=0;
    while(numVertices!=0){
        numVertices/=10;
        number_of_digits++;
    }
    int d;
    int num_threads,num_threads_2;
    omp_set_num_threads(4);
    for(d=0;d<number_of_digits;d++){
        for(i=0;i<10;i++){
            occurence_map[i]=0;
        }        
        
        #pragma omp parallel for reduction(+:occurence_map[:10]), schedule (static)
        for(i=0; i<numEdges; i++){
            int id=omp_get_thread_num();
            if(id==0)
            {
                num_threads= omp_get_num_threads();
            }
            edges[i].key = edges[i].trimmed_digit%10;
            if(edges[i].trimmed_digit!=0) edges[i].trimmed_digit=edges[i].trimmed_digit/10;
            occurence_map[edges[i].key]++;
        }
        for(i=1;i<10;i++){
            occurence_map[i]=occurence_map[i]+occurence_map[i-1];
        }
        for(i=numEdges-1;i>=0;i--){
            edges_sorted[--occurence_map[edges[i].key]]=edges[i];
        }
        #pragma omp parallel for schedule (static)
        for(i=0;i<numEdges;i++){
            int id2=omp_get_thread_num();   
            if(id2==0)
            {
                num_threads_2= omp_get_num_threads();
            }       
            edges[i]=edges_sorted[i];            
        }
    }
    printf("---------------------------------------------------------------\n");
    printf(" Number of threads : %d \n",num_threads);
    printf(" Number of threads in second loop : %d \n",num_threads_2);
    printf("---------------------------------------------------------------\n");
    
}
