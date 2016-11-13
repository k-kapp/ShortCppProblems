Solution to the following hackerrank problem:

For a list of integers, find the median every time after an integer is added to the list

More details on the problem can be found here: https://www.hackerrank.com/challenges/find-the-running-median

My solution is as follows:

A balanced binary tree was used for storing the array. The specific way in which balancing was achieved is similar to an AVL tree (https://en.wikipedia.org/wiki/AVL_tree). However, rebalancing was done in such a way so that the root of the tree (one of the candidate values for the median) was always right next to the two values that would be either side of it if the array was ordered. In this way, the median could be found very easily when required.
