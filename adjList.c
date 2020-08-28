#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* --- Adjacency List -------------------- */
typedef struct edge{  /*                   */
    int dest;         /* adj node          */
    int cost;         /* edge weight       */
    struct edge *adj; /* list of adj nodes */
}*AdjList;            /*                   */
/* --------------------------------------- */
typedef AdjList* Graph;
/* --------------------------------------- */
int NV;
/* --------------------------------------- */

/**
 * Returns an empty graph with capacity for nv nodes 
 * nv : number of nodes in the graph
 */
Graph initGraph(int nv){
    Graph g = malloc(sizeof(struct edge) * nv);

    for(int i = 0; i < nv; g[i++] = 0);

    return g;
}

/**
 * Adds a new edge to the graph given the two nodes and the edge cost 
 * g : graph
 * cur_node : origin node
 * out_node : destination node 
 * cost : edge cost
 */
void addEdge(Graph g, int cur_node, int out_node, int cost){
    AdjList tmp, edge = malloc(sizeof(struct edge));
    edge->dest = out_node;
    edge->cost = cost;
    edge->adj  = 0;
    
    if( g[cur_node] ){
        for(tmp = g[cur_node]; tmp->adj != NULL; tmp = tmp->adj);
        tmp->adj = edge;
    }
    else 
        g[cur_node] = edge;
}

/** 
 * Populate the graph with the info contained in given file 
 * The file format may be the following:
 *
 *      3      <- number of vertices 
 *      0 1 3
 *      0 2 1
 *                3
 *      | 0 | --------> | 1 |
 *        |       
 *      1 |      
 *        |     
 *       \ /   
 *      | 2 | 
 */
Graph readGraphFromFile(char* filePath){
    FILE* fp = fopen(filePath,"r");
    char line[50];
    char* delim = " ";

    fgets(line,50,fp);
    int nv = atoi(line);
    NV = nv;

    Graph g = initGraph(nv);
    int cur_node, out_node, edge_cost;

    while( fgets(line,50,fp) ){
        cur_node  = atoi( strtok(line,delim) );
        out_node  = atoi( strtok(NULL,delim) );
        edge_cost = atoi( strtok(NULL,delim) );
        addEdge(g,cur_node,out_node,edge_cost);
    }
    return g;
}

/**
 * Determine, if it exists, the cost of the edge that connects node o and d 
 * g : graph
 * o : origin node 
 * d : destination node 
 */
int edgeWeight(Graph g, int o, int d, int* c){
    int found = 0;
    AdjList tmp;

    for(tmp = g[o]; tmp && !found; tmp = tmp->adj)
        if( tmp->dest == d ){
            found = 1;
            *c = tmp->cost;
        }
    return found;
}

/**
 * Calculates the number of edges that have node 'o' as destination node 
 * g : graph 
 * o : node
 */
int inDegree(Graph g, int o){
    AdjList tmp;
    int found, c = 0;

    for(int i = 0; i < NV; i++){
        found = 0;
        for(tmp = g[i]; tmp && !found; tmp = tmp->adj)
            if( tmp->adj == o )
                c++;
    }
    return c;
}

/**
 * Calculates the number of edges that have node 'o' as source node
 * g : graph 
 * o : node 
 */
int outDegree(Graph g, int o){
    int c = 0;

    for(AdjList tmp = g[o]; tmp; c++, tmp = tmp->adj);

    return c;
}

/**
 * Implementation of Depth First Search (DFS) algorithm 
 * Check if node d is reachable from o 
 * The algorithm is splited in two functions, one for initializing 
 * variables and the other for recursion.
 *
 * g : graph
 * o : origin node 
 * d : destination node 
 *
 */
int dfsRec(Graph g, int v[], int o, int d){
    int found = 0;
    AdjList tmp;

    v[o] = 1;

    if( o == d )
        found = 1;
    else
        for(tmp = g[o]; tmp && !found; tmp = tmp->adj)
            if( !v[tmp->dest] )
                found = dfsRec(g,v,tmp->dest,d);

    return found;
}

int dfs(Graph g, int o, int d){
    int v[NV];
    memset(v, 0, NV * sizeof(int));

    return dfsRec(g,v,o,d);
}

/** 
 * Find connected components
 * The algorithm uses DFS and colors to find each component 
 */
void dfsCompRec(Graph g, int o, int v[], int c[], int color){
    AdjList tmp;

    v[o] = 1;
    c[o] = color;

    for(tmp = g[o]; tmp; tmp = tmp->adj)
        if( !v[tmp->dest] )
            dfsCompRec(g,tmp->dest,v,c,color);
}

void dfsComp(Graph g){
    int v[NV], c[NV];
    int i, color = 0;
    memset(v, 0, NV * sizeof(int));
    memset(c, 0, NV * sizeof(int));

    for(i = 0; i < NV; i++)
        if( !v[i] ){
            dfsCompRec(g,i,v,c,color);
            color++;
        }

    for(i = 0; i < NV; i++)
        printf("Node %d : %d\n", i, c[i]);
}

/**
 * Implementation of Breadth First Search (BFS) algorithm.
 * Check if node d is reachable from node o 
 *
 * g : graph 
 * o : origin node 
 * d : destination node 
 *
 */
int bfs(Graph g, int o, int d){
    int i = 0, j = 0, found = 0, node;
    int q[NV], v[NV];
    memset(v, 0, NV * sizeof(int));
    AdjList tmp;

    q[j++] = o;
    
    while( i < j ){
        node = q[i++];
        v[node] = 1;
        if( node == d )
            found = 1;
        for(tmp = g[node]; tmp && !found; tmp = tmp->adj)
            if( !v[tmp->dest] ){
                q[j++] = tmp->dest;
                v[tmp->dest] = 1;
            }
    }
    return found;
}


int main(int argc, char** argv){
    Graph g = 0;
    AdjList tmp;

    if( argc == 2 )
        g = readGraphFromFile(argv[1]);

    for(int i = 0; i < NV; i++)
        for(tmp = g[i]; tmp; tmp = tmp->adj)
            printf("(%d,%d,%d)\n",i,tmp->dest,tmp->cost);
    
    int found = bfs(g,6,0);
    printf("Found? %d\n",found);

    return 0;
}

