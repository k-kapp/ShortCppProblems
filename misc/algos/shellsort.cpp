/*
	This is my implementation of the shellsort algorithm
*/


#include <iostream>
#include <vector>
#include <random>
#include "test_utils.h"

using namespace std;

template<typename Iterator, typename Comparator>
void sort_sublist(Iterator begin, Iterator end, Comparator comp, int h)
{
	int list_end = 0;

	Iterator curr_ins = next(begin, list_end + h);

	while (distance(curr_ins, end) > 0)
	{
		int insert_idx = 0;
		for (Iterator curr_check = begin; insert_idx <= list_end; advance(curr_check, h), insert_idx += h)
		{
			if (comp(*curr_ins, *curr_check))
			{
				break;
			}
		}

		if (insert_idx <= list_end)
		{
			Iterator new_ins = curr_ins;
			while (new_ins != next(begin, insert_idx))
			{
				swap(*new_ins, *next(new_ins, -h));
				advance(new_ins, -h);
			}
		}
		advance(curr_ins, h);
		list_end += h;
	}
}

template<typename Iterator, typename Comparator, typename Container>
void shellsort(Iterator begin, Iterator end, Comparator comp, Container h_vals)
{
	if (begin == end)
		return;

	for (auto h_iter = h_vals.begin(); h_iter != h_vals.end(); advance(h_iter, 1))
	{
		for (int i = 0; i < *h_iter; i++)
		{
			sort_sublist(next(begin, i), end, comp, *h_iter);
		}
	}

}


int main(int argc, char * argv [])
{
	vector<int> my_vec = generate_randoms<vector<int> >(250);
	vector<int> h_vals = {6, 4, 1};

	shellsort(my_vec.begin(), my_vec.end(), less<int>(), h_vals);

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
		cout << "Array not sorted" << endl;
	}

	return EXIT_SUCCESS;
}
