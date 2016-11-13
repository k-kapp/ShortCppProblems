Solution to the following hackerrank problem:

The city of Gridland is represented as an n x m matrix where the rows are numbered from 1 to n and the columns are numbered from 1 to m.

Gridland has a network of train tracks that always run in straight horizontal lines along a row. In other words, the start and end points of a train track are (r, c1) and (r, c2), where r represents the row number, c1 represents the starting column, and c2 represents the ending column of the train track.

The mayor of Gridland is surveying the city to determine the number of locations where lampposts can be placed. A lamppost can be placed in any cell that is not occupied by a train track.

Given a map of Gridland and its k train tracks, find and print the number of cells where the mayor can place lampposts.


My solution is roughly as follows:

Since train tracks can run only horizontally, overlap can also happen only horizontally. Therefore, for each row, store a list of pairs, where each pair describes a contiguous section occupied by train tracks. Now use the following algorithm:

1. When inserting a new train track Q for a particular row, find the corresponding list for that row, then examine the pairs in the row. Q is represented as a pair, with the starting and ending columns for the track.

2. If the new train track, Q, is found to overlap with one of the existing contiguous tracks (given as a pair P), then create a new pair S which is a new contiguous track section resulting from the combination between the new track, Q, and the pair P, then go on to step 3. Else, if no overlap occurs, insert Q into the list, and skip steps 3 and 4.

3. Erase P from the list

4. Go back to 1, with S as the new "track" to be inserted

The solution I have here (in main.cpp) is not really optimal, since if the new pair S is created, the list is searched from the beginning. A "memory" of where in the list we encountered the existing pair P may be used so that we do not unnecessarily start from the beginning (using a C++ `std::list<pair<int, int> >::iterator` as the "memory"). 

The reasoning behind the use of the linked list is that we can insert and remove from a given list many times, and the complexity of removing from a doubly linked list is O(1) (given that we have a reference/pointer to the element to be removed). The complexity of removing from a dynamic array (C++ `std::vector< pair<int, int> >`) is O(n) on average, where n is the length of the array. We also do not need random access to any elements in a given list, strengthening the case for a linked list.
