Solution to the following hackerrank problem:

Implement a simple "text editor", with the following capabilities:

Initially, the editor contains an empty string S. The following four operations must be supported:

1. append (W): append a string W to the end of S.
2. delete (k): remove the last k characters from S.
3. print (k): print the kth character of S to the screen.
4. undo(): undo the last (not previously undone) operation of type 1 or 2, reverting S to what it was before the operation was performed.

More detailed information may be found here: https://www.hackerrank.com/challenges/simple-text-editor

My implementation is as follows:

When either of the append or delete functions are executed, a record of their changes to S is pushed onto a stack. When undo is called, the element on top of the stack is examined so that the changes to S (made by the last call to either 1 or 2) may be reversed. This record is then popped off the stack, since we cannot undo a single action twice (according to 4).

The stack was implemented using C++'s list structure, which is a doubly linked list (a dynamic array (vector) could also have been used).
