Solution to the following hackerrank problem:

Given an array of integers, find the number of pairs of integers with a difference of K between them.

More details on the problem may be found here https://www.hackerrank.com/challenges/pairs

The basics of my solution is shown below.

Given an array A, first sort the array from smallest to largest. Then, go through each element i in the sorted array as follows:

1. Take two elements, the element immediately after element i, which is referred to as j, and the last element in the sorted array, k.
2. Now, compute values c = i - j and d = i - k. If c x d > 0, then go to the element after i. This element is now element i. Start at 1 again.
3. If c x d < 0, take the element after j, which is now j, and similarly, take the element before k, which is now the new k.
4. If j and k are the same elements, or j is now an element after k, assign the element after element i, to i, and start again at 1.
5. If |j - i| = K, then take element j, and all elements after element j having the same value, and count them. Add this count to the total count.
6. If |k - i| = K, then take element k, and all elements after k having the same value, and count them. Add this count to the total count.
7. If element i is the last element in the array, stop the algorithm. Return the total count.
