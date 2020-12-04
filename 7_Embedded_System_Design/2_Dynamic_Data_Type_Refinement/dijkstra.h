#define SLL
//#define DLL
//#define DYN_ARR

#if defined(SLL)
    #include "./synch_implementations/cdsl_queue.h"
#endif
    #if defined(DLL)
#include "./synch_implementations/cdsl_deque.h"
    #endif
#if defined(DYN_ARR)
    #include "./synch_implementations/cdsl_dyn_array.h"
#endif

struct _NODE {
  int iDist;
  int iPrev;
};
typedef struct _NODE NODE;

struct _QITEM {
  int iNode;
  int iDist;
  int iPrev;
  struct _QITEM *qNext;
};
typedef struct _QITEM QITEM;