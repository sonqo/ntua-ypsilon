#ifndef _DIJKSTRA_C_
  
    #define _DIJKSTRA_C_

    #include <stdio.h>
    #include <stdlib.h>
    #include "dijkstra.h"

    #ifndef _FUNCTIONS_INSTANTIATIONS_
        
        #define _FUNCTIONS_INSTANTIATIONS_

        QITEM* delete_node();

        void print_path(NODE *rgnNodes, int chNode);
        void insert_node(int iNode, int iDist, int iPrev);

        int qcount(void);
        int dijkstra(int chStart, int chEnd);

    #endif

    #define NONE 9999
    #define NUM_NODES 100

    #if defined(SLL)
      cdsl_sll *queue;
    #elif defined(DLL)
      cdsl_dll *queue;
    #else
      cdsl_dyn_array *queue;
    #endif

    int AdjMatrix[NUM_NODES][NUM_NODES];

    int g_qCount = 0;
    NODE rgnNodes[NUM_NODES];
    int i, ch, iPrev, iNode, iCost, iDist;

    int main(int argc, char *argv[]) {
      
        FILE *fp;
        int i, j, k;
        
        #if defined (SLL)
          iterator_cdsl_sll it, end;
          queue = cdsl_sll_init();
        #elif defined (DLL)
          iterator_cdsl_dll it, end;
          queue = cdsl_dll_init();
        #else 
          iterator_cdsl_dyn_array it, end;
          queue = cdsl_dyn_array_init();	
        #endif

        if (argc<2) {
            fprintf(stderr, "Usage: dijkstra <filename>\n");
        }

        fp = fopen (argv[1],"r");

        for (i=0; i<NUM_NODES; i++) {
            for (j=0; j<NUM_NODES; j++) {
                fscanf(fp, "%d", &k);
                AdjMatrix[i][j]= k;
            }
        }

        for (i=0, j=NUM_NODES/2; i<20; i++, j++) {
            j = j % NUM_NODES;
            dijkstra(i, j);
        }

        return 0;
    }

    #ifndef _INSERT_NODE_
        #define _INSERT_NODE_
        void insert_node(int iNode, int iDist, int iPrev) {
            QITEM *qNew = (QITEM *) malloc(sizeof(QITEM));
            QITEM *qLast = queue->get_head(0, queue);
            if (!qNew) {
                fprintf(stderr, "Out of memory.\n");
                exit(1);
            }
            qNew->iNode = iNode;
            qNew->iDist = iDist;
            qNew->iPrev = iPrev;
            qNew->qNext = NULL;
            queue->enqueue(0, queue, (void*)qNew);
            g_qCount++;
            return;
        }
    #endif

    #ifndef _DELETE_NODE_
        #define _DELETE_NODE_
        QITEM* delete_node() {
            QITEM *head = queue->get_head(0, queue);
            if (head) {
                queue->remove(0, queue, head);
                g_qCount--;
            }
            return head;        
        }            
    #endif

    #ifndef _QCOUNT_
        #define _QCOUNT_
        int qcount (void) {
            return(g_qCount);
        }
    #endif

    #ifndef _PRINT_PATH_
        #define _PRINT_PATH_
        void print_path (NODE *rgnNodes, int chNode) {
        if (rgnNodes[chNode].iPrev != NONE) {
            print_path(rgnNodes, rgnNodes[chNode].iPrev);
        }
        printf (" %d", chNode);
        fflush(stdout);
    }
    #endif
    
    #ifndef _DIJKSTRA_
        #define _DIJKSTRA_
        int dijkstra(int chStart, int chEnd) {
            
            QITEM* curr_item;

            for (ch = 0; ch < NUM_NODES; ch++) {
                rgnNodes[ch].iDist = NONE;
                rgnNodes[ch].iPrev = NONE;
            }

            if (chStart == chEnd) {
                printf("Shortest path is 0 in cost. Just stay where you are.\n");
            }
            else {
                rgnNodes[chStart].iDist = 0;
                rgnNodes[chStart].iPrev = NONE;
            
                insert_node(chStart, 0, NONE);
            
                while (qcount() > 0) {
                    curr_item = delete_node();
                    iNode = curr_item->iNode;
                    iDist = curr_item->iDist;
                    iPrev = curr_item->iPrev;
                    for (i = 0; i < NUM_NODES; i++) {
                        if ((iCost = AdjMatrix[iNode][i]) != NONE) {
                            if ((NONE == rgnNodes[i].iDist) || (rgnNodes[i].iDist > (iCost + iDist))) {
                                rgnNodes[i].iDist = iDist + iCost;
                                rgnNodes[i].iPrev = iNode;
                                insert_node(i, iDist+iCost, iNode);
                            }
                        }
                    }
                }
                printf("Shortest path is %d in cost. ", rgnNodes[chEnd].iDist);
                printf("Path is: ");
                print_path(rgnNodes, chEnd);
                printf("\n");
            }
        }
    #endif

#endif
