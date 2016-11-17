#Algorithms

I plan to put implementations of some common algorithms here, such as sorting and string processing algorithms.

Note that I DO NOT simply read pseudocode from a textbook and implement it in C++. Instead, I do some reading on how the algorithm works conceptually, then write the code in C++. I also try to make the algorithm as efficient as possible in some way (such as in the mergesort algorithm I keep the memory use as low as possible).

So far I have:

+ `mergesort.cpp`, which contains a memory-efficient, procedural implementation of the mergesort algorithm
+ `quicksort.cpp`, which contains also a memory-efficient implementation of the quicksort algorithm. No allocations or deallocations are made during the sorting procedure.
+ `shellsort.cpp`, which contains an implementation of the shellsort algorithm
+ `combsort.cpp`, which contains an implementation the combsort algorithm
+ `test_utils.h` is a header file that contains functions to test whether sorting is done correctly

#Additional Notes

Quicksort:

The quicksort implementation that I wrote in `quicksort.cpp` achieves sorting by swapping values alone. For example, if we have the following array (to be sorted in ascending order with respect to the pivot, 4)

4 6 9 2 5

then, when 4 is compared with 2 and it is found that their ordering is wrong, 2 gets swapped with 6, then 4 is swapped with 2, leading to

2 4 9 6 5

This array is not sorted, but quicksort will now sort the arrays 2 and 9 6 5, since 4 is now at the proper index. If 2 was just after 4, as follows

4 2 6 9 5

then we simply swap 4 and 2, which again yields the proper array for quicksort to continue. In this way, no allocations/insertions/deletions are done on the array - everything is done in-place. The rules for swapping with the pivot (4, in the example above) is therefore as follows:

+ the pivot is the first element in the list we are sorting with respect to the pivot
+ start with the element after the pivot, then proceed until the last element is checked. In this way, the element we are checking is always after the pivot.
+ if the element in question is in the wrong position relative to the pivot, then
 - if the element is just after the pivot, swap the element with the pivot and continue to the next element
 - if the element is not just after the pivot, but further on in the array, then swap the element in question with the element just after the pivot, then swap the pivot with the element just after it.
 
For more information on the quicksort algorithm, please consult https://en.wikipedia.org/wiki/Quicksort

