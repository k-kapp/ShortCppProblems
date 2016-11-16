/*
	My C++ implementation of the quicksort algorithm. As elements are reordered for each segment of the
		container to be orderered, no memory allocations are made. The sorting is done entirely via swaps
*/

#include <vector>
#include <functional>
#include <iostream>
#include "test_utils.h"

using namespace std;

template<typename Iterator, typename Comparator>
void quicksort(Iterator begin, Iterator end, Comparator comp)
{
	if (begin == end)
		return;
	Iterator q_iter = begin;
	Iterator curr_iter = next(begin, 1);

	while (curr_iter != end)
	{
		if (!comp(*q_iter, *curr_iter))
		{
			if (distance(q_iter, curr_iter) > 1)
			{
				swap(*next(q_iter, 1), *curr_iter);
			}
			swap(*q_iter, *next(q_iter, 1));
			advance(q_iter, 1);
		}
		advance(curr_iter, 1);
	}
	quicksort(begin, q_iter, comp);
	quicksort(next(q_iter, 1), end, comp);
}

int main(int argc, char * argv [])
{
	
	vector<int> my_vec = generate_randoms<vector<int> >(200);

	quicksort(my_vec.begin(), my_vec.end(), less<int>());

	for (auto &val : my_vec)
	{
		cout << val << " ";
	}
	cout << endl;

	if (sorted(my_vec, less_equal<int>()))
	{
		cout << "Sorting OK" << endl;
	}
	else
	{
		cout << "vector not sorted" << endl;
	}

	return 0;
}
