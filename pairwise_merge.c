/* ==========================================================================
   pairwise_merge.c
   Simple PAIRWISE merging approach for the same 3 sorted lists:
      Step 1: merge(L1, L2) -> M12
      Step 2: merge(M12, L3) -> FINAL
   Counts comparisons made during the standard 2-way merge routine.
   ========================================================================== */

#include <stdio.h>

#define MAXN 100

long comparisons = 0;

/* Standard 2-way merge of two sorted arrays a[0..na-1], b[0..nb-1] into result */
int mergeTwo(int a[], int na, int b[], int nb, int result[]) {
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        comparisons++;
        if (a[i] <= b[j]) result[k++] = a[i++];
        else               result[k++] = b[j++];
    }
    while (i < na) result[k++] = a[i++];   /* leftover of a, no comparisons needed */
    while (j < nb) result[k++] = b[j++];   /* leftover of b, no comparisons needed */
    return k;
}

void printArr(const char *label, int arr[], int n) {
    printf("%-22s: ", label);
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int L1[] = {10, 30, 50, 70};
    int L2[] = {20, 40, 60, 80};
    int L3[] = {15, 35, 55, 75};
    int n1 = 4, n2 = 4, n3 = 4;

    int M12[MAXN];
    int FINAL[MAXN];

    printf("================ SIMPLE PAIRWISE MERGING ================\n");
    printf("Lists:\n L1 = 10 30 50 70\n L2 = 20 40 60 80\n L3 = 15 35 55 75\n\n");

    printf("Step 1: merge(L1, L2) -> M12\n");
    long before = comparisons;
    int m12n = mergeTwo(L1, n1, L2, n2, M12);
    printArr("Result M12", M12, m12n);
    printf("Comparisons used in Step 1 : %ld\n\n", comparisons - before);

    printf("Step 2: merge(M12, L3) -> FINAL\n");
    before = comparisons;
    int finaln = mergeTwo(M12, m12n, L3, n3, FINAL);
    printArr("Result FINAL", FINAL, finaln);
    printf("Comparisons used in Step 2 : %ld\n\n", comparisons - before);

    printf("--- Result ---\n");
    printArr("Merged output", FINAL, finaln);
    printf("\nTotal elements merged (N)        : %d\n", finaln);
    printf("Number of merge stages (k-1)     : %d\n", 2);
    printf("Max auxiliary array size used    : %d\n", m12n); /* size of intermediate M12 */
    printf("Total value comparisons (pairwise): %ld\n", comparisons);

    return 0;
}
