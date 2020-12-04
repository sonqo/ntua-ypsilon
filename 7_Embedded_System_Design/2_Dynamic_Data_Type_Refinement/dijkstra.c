#ifndef _DIJKSTRA_C_
  
    #define _DIJKSTRA_C_

    // DONE
    #include <stdio.h>
    #include <stdlib.h>
    #include "dijkstra.h"

    #define NONE 9999
    #define NUM_NODES 100

    // DONE
    #if defined(SLL)
      cdsl_sll *queue;
    #elif defined(DLL)
      cdsl_dll *queue;
    #else
      cdsl_dyn_array *queue;
    #endif

    // DONE
    int AdjMatrix[NUM_NODES][NUM_NODES];

    // DONE
    int g_qCount = 0;
    NODE rgnNodes[NUM_NODES];
    int i, ch, iPrev, iNode, iCost, iDist;

    // DONE
    void print_path (NODE *rgnNodes, int chNode) {
        if (rgnNodes[chNode].iPrev != NONE) {
            print_path(rgnNodes, rgnNodes[chNode].iPrev);
        }
        printf (" %d", chNode);
        fflush(stdout);
    }

    void enqueue (int iNode, int iDist, int iPrev) {
        QITEM *qNew = (QITEM *) malloc(sizeof(QITEM));
        QITEM *qLast = qHead;
        if (!qNew) {
            fprintf(stderr, "Out of memory.\n");
            exit(1);
        }
        qNew->iNode = iNode;
        qNew->iDist = iDist;
        qNew->iPrev = iPrev;
        qNew->qNext = NULL;
        if (!qLast) {
            qHead = qNew;
        }
        else {
            while (qLast->qNext) qLast = qLast->qNext;
            qLast->qNext = qNew;
        }
        g_qCount++;
    }

    void dequeue (int *piNode, int *piDist, int *piPrev) {
        QITEM *qKill = qHead;
        if (qHead) {
            *piNode = qHead->iNode;
            *piDist = qHead->iDist;
            *piPrev = qHead->iPrev;
            qHead = qHead->qNext;
            free(qKill);
            g_qCount--;
        }
    }

    // DONE
    int qcount (void) {
        return(g_qCount);
    }

    int dijkstra(int chStart, int chEnd) {
      
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
        
          enqueue (chStart, 0, NONE);
        
          while (qcount() > 0) {
              dequeue (&iNode, &iDist, &iPrev);
              for (i = 0; i < NUM_NODES; i++) {
                  if ((iCost = AdjMatrix[iNode][i]) != NONE) {
                      if ((NONE == rgnNodes[i].iDist) || (rgnNodes[i].iDist > (iCost + iDist))) {
                          rgnNodes[i].iDist = iDist + iCost;
                          rgnNodes[i].iPrev = iNode;
                          enqueue (i, iDist + iCost, iNode);
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

#endif
