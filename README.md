# K-Way Merge (Min Heap) vs Simple Pairwise Merging

**Input data**

```
L1 = 10, 30, 50, 70
L2 = 20, 40, 60, 80
L3 = 15, 35, 55, 75
```

Three sorted lists, 4 elements each, N = 12 total elements, k = 3 lists.

---

## (a) K-Way Merge using a Min Heap

### Data structures
- Each list is stored as a plain sorted `int` array with a known length (`lists[i][]`, `listSize[i]`).
- The heap is an array `heap[k]` of `HeapNode { value, listIdx, elemIdx }`.
  `listIdx` remembers which of the k lists a value came from, `elemIdx` remembers its position
  in that list, so the next element from the same list can be fetched after extraction.
- Heap size never exceeds **k = 3** (one slot per active list) — this is the key property of
  the heap approach.

### Algorithm
1. Insert the first element of every list into the heap.
2. `buildHeap()` — sift-down from the last internal node to the root, so the array satisfies
   the min-heap property.
3. Repeat until the heap is empty:
   - Extract the root (the current global minimum) → append to output.
   - If the source list still has elements, put the next element of that list at the root.
     Otherwise move the last heap element to the root and shrink the heap by one.
   - Sift-down the root to restore the heap property.

### Heap states (as executed — see `src/kway_heap_merge.c`, output in `output/heap_output.txt`)

| Step | Action | Heap after step |
|---|---|---|
| init | insert first of each list | `[10(L1), 20(L2), 15(L3)]` |
| buildHeap | heapify | `[10(L1), 20(L2), 15(L3)]` |
| 1 | extract 10, push 30 | `[15(L3), 20(L2), 30(L1)]` |
| 2 | extract 15, push 35 | `[20(L2), 35(L3), 30(L1)]` |
| 3 | extract 20, push 40 | `[30(L1), 35(L3), 40(L2)]` |
| 4 | extract 30, push 50 | `[35(L3), 50(L1), 40(L2)]` |
| 5 | extract 35, push 55 | `[40(L2), 50(L1), 55(L3)]` |
| 6 | extract 40, push 60 | `[50(L1), 60(L2), 55(L3)]` |
| 7 | extract 50, push 70 | `[55(L3), 60(L2), 70(L1)]` |
| 8 | extract 55, push 75 | `[60(L2), 75(L3), 70(L1)]` |
| 9 | extract 60, push 80 | `[70(L1), 75(L3), 80(L2)]` |
| 10 | extract 70, L1 empty | `[75(L3), 80(L2)]` |
| 11 | extract 75, L3 empty | `[80(L2)]` |
| 12 | extract 80, L2 empty | `[]` |

**Merged output:** `10 15 20 30 35 40 50 55 60 70 75 80`
**Measured comparisons: 21**

---

## (b) Simple Pairwise Merging

Two sorted lists are merged at a time using the classic 2-way merge routine
(`src/pairwise_merge.c`):

1. `merge(L1, L2) → M12`
2. `merge(M12, L3) → FINAL`

### Execution (from `output/pairwise_output.txt`)

| Stage | Inputs | Output | Comparisons |
|---|---|---|---|
| Step 1 | L1 (4) , L2 (4) | `10 20 30 40 50 60 70 80` | 7 |
| Step 2 | M12 (8), L3 (4) | `10 15 20 30 35 40 50 55 60 70 75 80` | 11 |
| **Total** | | | **18** |

**Merged output:** `10 15 20 30 35 40 50 55 60 70 75 80` (identical to the heap result, as expected)

---

## (c) Analysis and Comparison

| Criterion | Min-Heap k-way merge | Pairwise (sequential) merge |
|---|---|---|
| **Heap / auxiliary size** | Fixed at **k = 3** throughout (one node per list) | No heap; largest auxiliary array is the intermediate result, **8** elements (grows toward N as merging proceeds) |
| **Comparisons (measured)** | 21 (2 per sift-down step, worst case, × 10 extraction steps + heapify) | 18 (7 + 11) |
| **Comparisons (general formula)** | Each extraction costs O(log k) comparisons → total ≈ N·log₂k = 12·log₂3 ≈ 19 | Each of the (k−1) merge stages costs up to (size of left + size of right − 1) comparisons → total ≈ N·(k−1) for k small, but grows fast with k |
| **Time Complexity** | **O(N log k)** — N extractions, each O(log k) to restore the heap | **O(N·k)** in general (k−1 sequential 2-way merges, each merge touching all elements processed so far); for very small k it can look comparable, but it does not scale as k grows |
| **Space Complexity** | **O(k)** extra space for the heap (+ O(N) for the output) | **O(N)** extra space for the growing intermediate arrays (+ O(N) for output) |
| **Scalability as k increases** | Heap size grows only as O(k); comparisons grow as O(log k) per element → very good for large k | Each additional list needs one more full O(N)-ish merge pass → cost grows roughly linearly with k → poor for large k |

### Why the numbers look close here
With only k = 3 small lists, `log₂k ≈ 1.58` is not very different from `k − 1 = 2`, so the two
comparison counts (21 vs 18) are close. This gap widens sharply as k grows — that is the
entire point of using a heap.

**Illustration for larger k** (N total elements spread over k equal lists):

| k (lists) | Heap: O(N log₂k) | Pairwise: O(N·(k−1)) |
|---|---|---|
| 3 | ~19 | ~24 |
| 10 | N·3.3 | N·9 |
| 100 | N·6.6 | N·99 |
| 1000 | N·10 | N·999 |

As k increases, `log₂k` grows extremely slowly while `(k−1)` grows linearly — the pairwise
approach's cost blows up while the heap approach barely changes.

---

## Conclusion

For **k = 3** small files, both approaches are practical and give identical, correctly
sorted output; pairwise merging is even marginally cheaper in raw comparisons at this tiny
scale and is simpler to implement.

However, **as the number of sorted files (k) increases, the Min-Heap based k-way merge is
clearly the more suitable approach**:
- Its heap never grows beyond size k, so memory overhead stays small and predictable — **O(k)**.
- Its time complexity, **O(N log k)**, scales far better than the pairwise approach's
  **O(N·k)**, because `log k` grows much slower than `k`.
- It performs the merge in a single pass over all k lists simultaneously, rather than
  repeatedly re-merging a growing intermediate result.

**Recommendation:** Use the Min-Heap k-way merge for any system (such as a financial
transaction-merging system) that must combine many sorted sources, especially when the
number of sources can grow; use simple pairwise merging only when the number of lists is
small and fixed (e.g., k ≤ 3–4).

---

## Repository contents
```
kway_merge_assignment/
├── src/
│   ├── kway_heap_merge.c     # Part (a): min-heap k-way merge, prints heap states
│   └── pairwise_merge.c      # Part (b): sequential pairwise 2-way merges
├── input/
│   └── input.txt             # Given input lists
├── output/
│   ├── heap_output.txt       # Actual program run: heap states + comparisons
│   └── pairwise_output.txt   # Actual program run: merge stages + comparisons
└── README.md                 # This file: analysis, comparison table, conclusion
```

## How to compile and run
```bash
gcc -O0 -o kway_heap_merge src/kway_heap_merge.c
gcc -O0 -o pairwise_merge  src/pairwise_merge.c
./kway_heap_merge
./pairwise_merge
```
