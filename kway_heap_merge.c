/* ==========================================================================
   kway_heap_merge.c
   k-way merge of K sorted lists using a MIN HEAP.
   Prints heap state at every important step and counts comparisons.
   ========================================================================== */

#include <stdio.h>
#include <stdlib.h>

#define MAXK 10
#define MAXN 100

/* A heap node stores the value, which list it came from, and its index
   within that list (so we know where to fetch the next element from). */
typedef struct {
    int value;
    int listIdx;   /* 0-based index of source list        */
    int elemIdx;   /* index of this element within list   */
} HeapNode;

HeapNode heap[MAXK];
int heapSize = 0;
long comparisons = 0;   /* counts element-value comparisons only */

int lists[MAXK][MAXN];
int listSize[MAXK];
int K; /* number of lists */

void swapNode(HeapNode *a, HeapNode *b) {
    HeapNode t = *a; *a = *b; *b = t;
}

void printHeap(const char *label) {
    printf("%-28s : [", label);
    for (int i = 0; i < heapSize; i++) {
        printf("%d(L%d)", heap[i].value, heap[i].listIdx + 1);
        if (i != heapSize - 1) printf(", ");
    }
    printf("]\n");
}

/* Standard min-heap sift-down, counting each value comparison */
void siftDown(int i) {
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < heapSize) {
            comparisons++;
            if (heap[left].value < heap[smallest].value) smallest = left;
        }
        if (right < heapSize) {
            comparisons++;
            if (heap[right].value < heap[smallest].value) smallest = right;
        }
        if (smallest == i) break;
        swapNode(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

void buildHeap() {
    for (int i = heapSize / 2 - 1; i >= 0; i--) siftDown(i);
}

int main() {
    /* --- (a) Represent the 3 sorted lists as arrays --- */
    K = 3;
    int L1[] = {10, 30, 50, 70};
    int L2[] = {20, 40, 60, 80};
    int L3[] = {15, 35, 55, 75};

    listSize[0] = 4; for (int i = 0; i < 4; i++) lists[0][i] = L1[i];
    listSize[1] = 4; for (int i = 0; i < 4; i++) lists[1][i] = L2[i];
    listSize[2] = 4; for (int i = 0; i < 4; i++) lists[2][i] = L3[i];

    int totalN = 0;
    for (int i = 0; i < K; i++) totalN += listSize[i];

    int output[MAXN];
    int outCount = 0;

    printf("================ K-WAY MERGE USING MIN HEAP ================\n");
    printf("Lists:\n L1 = 10 30 50 70\n L2 = 20 40 60 80\n L3 = 15 35 55 75\n");
    printf("Heap capacity (k) = %d  (one slot per list)\n\n", K);

    /* Step 1: insert the first element of every list into the heap */
    heapSize = 0;
    for (int i = 0; i < K; i++) {
        heap[heapSize].value = lists[i][0];
        heap[heapSize].listIdx = i;
        heap[heapSize].elemIdx = 0;
        heapSize++;
    }
    printHeap("Initial heap (unheapified)");

    buildHeap();
    printHeap("After buildHeap()  ");
    printf("\n--- Extraction phase ---\n");

    int step = 1;
    while (heapSize > 0) {
        HeapNode root = heap[0];
        output[outCount++] = root.value;
        printf("\nStep %2d: Extract MIN = %d (from L%d)\n",
               step++, root.value, root.listIdx + 1);

        int nextIdx = root.elemIdx + 1;
        if (nextIdx < listSize[root.listIdx]) {
            /* replace root with next element of the SAME list */
            heap[0].value = lists[root.listIdx][nextIdx];
            heap[0].listIdx = root.listIdx;
            heap[0].elemIdx = nextIdx;
            printf("  Insert next element of L%d -> %d, then sift-down\n",
                   root.listIdx + 1, heap[0].value);
        } else {
            /* that list is exhausted: move last heap element to root */
            heap[0] = heap[heapSize - 1];
            heapSize--;
            printf("  L%d exhausted -> move last heap node to root, sift-down\n",
                   root.listIdx + 1);
        }
        if (heapSize > 0) siftDown(0);
        printHeap("  Heap state now");
    }

    printf("\n--- Result ---\nMerged output: ");
    for (int i = 0; i < outCount; i++) printf("%d ", output[i]);
    printf("\n\nTotal elements merged (N)      : %d\n", totalN);
    printf("Number of lists (k)             : %d\n", K);
    printf("Max heap size used               : %d\n", K);
    printf("Total value comparisons (heap)   : %ld\n", comparisons);

    return 0;
}
