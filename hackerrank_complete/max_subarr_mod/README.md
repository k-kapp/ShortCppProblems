A variation of the maximum subarray sum problem. Here, all elements are positive. However, the task is to find the maximum sum of contiguous arrays modulo m.

Idea behind the final solution (in `max_subarr.cpp`) is not my own, however, I coded it myself (and understand it, at least). The rationale behind the algorithm is easily demonstrated with an example, as follows:

Given an array A = [4, 5, 2, 3], with m = 8. The prefix_sum[n] is the sum of the first n elements of A.

1. Create a new array of prefix sums modulo m, with the original indices: B = [(0, 4), (1, 1),(2, 3), (3, 6)]. 
2. Now order B according to the second element of each pair in B: C = [(1, 1), (2, 3), (0, 4), (3, 6)]. 
3. Find the smallest difference between two successive elements in C, given that the first element's first value in the pair is smaller than the second element's first value in its pair. Assign to min_diff the absolute value of this difference.
4. The maximum sum modulo m is m - min_diff.

The reason this works, is that the difference between two subarrays characterised by (n, a) and (m, b) gives us the sum of the subarray from n + 1 to m, given that `n < m`. The array C is ordered from smallest to largest according to the second element in each pair, and we only take into account those pairs of elements where the first of the pair has a larger first value than that of the second. Therefore, we take into account only those elements where the second element represents a subarray of the first. If the first element's subarray has a lower sum modulo m (a) than that of the second (b), then we know that the subarray from n + 1 to m has sum modulo m equal to m - min_diff.

Since all elements are positive, the longer subarray must have a total sum equal to or greater than the shorter subarray. Therefore if we subtract the subarray represented by b (shorter) from the subarray represented by a (longer), we will always have a positive sum. However, subtracting b from a, both modulos, gives us another modulo, but negative. Since we know the actual difference (with no modulo) is positive, the subarray's modulo sum, resulting from subtracting b from a, is given by m - min_diff instead of 0 - min_diff (which would have been the case if the subarray represented by b was longer than that represented by a).
