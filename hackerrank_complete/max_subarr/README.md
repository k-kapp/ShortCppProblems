My solution to finding the maximum sum for any subarray in a given array A. Since A can contain positive and negative integers, one cannot simply sum the whole array. Also find the maximum sum of all non-contiguous subarrays.

Problem also specified at https://www.hackerrank.com/challenges/maxsubarray, and information on the original maximum subarray problem may be found at https://en.wikipedia.org/wiki/Maximum_subarray_problem.

To find the maximum subarray sum, one can easily implement Kadane's algorithm (found at the wikipedia link above). However, I implemented my own algorithm from scratch. This algorithm also made finding the maximum sum from non-contiguous arrays fairly trivial.

My solution is roughly as follows:

1. Firstly, find all contiguous positive subarrays, and contiguous negative subarrays, and contiguous zero subarrays. That is, subarrays where all elements have the same sign, or are all zero.
2. Create a new array B that consists of the sums of these subarrays (with the same ordering as in the original array). In effect, all contiguous subarrays described in 1 are reduced to their sums.
3. Now, examine all possible adjacent triplets of the elements of B. If 
3.1 the first and third elements of this triplet are bigger than the second, sum them together to create a new element, and replace the triplet with this element.
3.2 else, leave the triplets
4. Repeat 3 until the case 3.1 does not occur. Pick the highest element in B for the maximum contiguous subarray. Sum all the positive elements of B for the maximum non-contiguous subarray.


