#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 144 //Frame dimension for QCIF format
#define M 176 //Frame dimension for QCIF format
#define p 7 //Search space, restricted in a [-p,p] region around the original location of the block

int B;

void read_sequence(int** current, int** previous) { 
	int i, j;
    FILE *picture0, *picture1;

    if ((picture0=fopen("akiyo0.y","rb")) == NULL) {
		printf("Previous frame doesn't exist.\n");
		exit(-1);
	}

	if ((picture1=fopen("akiyo1.y","rb")) == NULL) {
		printf("Current frame doesn't exist.\n");
		exit(-1);
	}

    //Input for the previous frame
    for (i=0; i<N; i++) {
        for (j=0; j<M; j++) {
            previous[i][j] = fgetc(picture0);
        }
    }

	//Input for the current frame
	for (i=0; i<N; i++) {
		for (j=0; j<M; j++) {
			current[i][j] = fgetc(picture1);
        }
    }

	fclose(picture0);
	fclose(picture1);
}

void phods_motion_estimation(int** current, int** previous, int** vectors_x, int** vectors_y) {
  
    int x, y, i, k, l, p1, p2, q2, distx, disty, S, min1, min2, bestx, besty;

    distx = 0;
    disty = 0; 

    //For all blocks in the current frame
    for (x=0; x<N/B; x++) {
        for (y=0; y<M/B; y++) {
            S = 4;
            //Initialize the vector motion matrices
            vectors_x[x][y] = 0;
            vectors_y[x][y] = 0;
            while (S > 0) {
                min1 = 255*B*B;
                min2 = 255*B*B;
                //For all candidate blocks in X-Y dimension
                for (i=-S; i<S+1; i+=S) {
                    distx = 0;
                    disty = 0;
                    //For all pixels in the block
                    for (k=0; k<B; k++) {
                        for (l=0; l<B; l++) {
                            p1 = current[B*x+k][B*y+l];
                            if ((B*x+vectors_x[x][y]+i+k) < 0 || (B*x+vectors_x[x][y]+i+k) > (N-1) || (B*y+vectors_y[x][y]+l) < 0 || (B*y+vectors_y[x][y]+l) > (M-1)) {
                                p2 = 0;
                            } else {
                                p2 = previous[B*x+vectors_x[x][y]+i+k][B*y+vectors_y[x][y]+l];
                            }
                            if (i == 0) {
                                disty = distx;
                            } else {
                                if ((B*x+vectors_x[x][y]+k) < 0 || (B*x+vectors_x[x][y]+k) > (N-1) || (B*y+vectors_y[x][y]+i+l) < 0 || (B*y+vectors_y[x][y]+i+l) > (M-1)) {
                                    q2 = 0;
                                } else {
                                    q2 = previous[B*x+vectors_x[x][y]+k][B*y+vectors_y[x][y]+i+l];
                                }
                                distx += abs(p1-p2);
                                disty += abs(p1-q2);
                            }
                        }
                    }
                    if (distx < min1) {
                        min1 = distx;
                        bestx = i;
                    }
                    if (disty < min2) {
                        min2 = disty;
                        besty = i;
                    }
                }
                S = S/2;
                vectors_x[x][y] += bestx;
                vectors_y[x][y] += besty;
            }
        }
    }
} 

int main(int argc, char* argv[]) {  
    
    B = atoi(argv[1]);

    int i, j;
    struct timeval start, finish;

    int **current = malloc(N * sizeof(int*));
    for (i=0; i<N; i++) {
        current[i] = malloc(M * sizeof(int));
    }

    int **previous = malloc(N * sizeof(int*));
    for (i=0; i<N; i++) {
        previous[i] = malloc(M * sizeof(int));
    }

    int **motion_vectors_x = malloc((N/B) * sizeof(int*));
    for (i=0; i<(N/B); i++) {
        motion_vectors_x[i] = malloc((M/B) * sizeof(int));
    }

    int **motion_vectors_y = malloc((N/B) * sizeof(int*));
    for (i=0; i<(N/B); i++) {
        motion_vectors_y[i] = malloc((M/B) * sizeof(int));
    }

    read_sequence(current, previous);

    gettimeofday(&start, NULL);
    phods_motion_estimation(current, previous, motion_vectors_x, motion_vectors_y);
    gettimeofday(&finish, NULL);

    printf("%d", finish.tv_usec-start.tv_usec);

    return 0;
}
