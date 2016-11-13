This folder contains implementations of various data structures. They are as follows:

`linked_list_proc.cpp` contains a procedural implementation of a singly linked list. Operations on the linked list are done recursively, instead of iteratively (with loops).

`linked_list.h` contains an object oriented implementation of a doubly linked list.

`b_tree.cpp` contains a procedural implementation of a b-tree (see https://en.wikipedia.org/wiki/B-tree). This was done with every node in the b-tree containing references only to the child nodes, and not the parent node. This implied that all operations on the b-tree were to be done recursively. I specifically did it this way so that it would be more of a challenge to implement it, as implementing it with each node containing a reference to its parent too is much easier.

