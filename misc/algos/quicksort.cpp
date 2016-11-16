/*
	My C++ implementation of the quicksort algorithm. As elements are reordered for each segment of the
		container to be orderered, no memory allocations are made. The sorting is done entirely via swaps
*/

#include <vector>
#include <functional>
#include <iostream>
#include <random>

using namespace std;

/*
	the following two functions, generate_randoms and sorted, are utility functions to check whether sorting
		was done properly. the quicksort function follows
*/

template<typename Container>
Container generate_randoms(int num)
{
	default_random_engine generator;
	uniform_int_distribution<typename Container::value_type> unif_dist(0, 10000);

	Container return_vals = Container(num);

	for (auto iter = return_vals.begin(); iter != return_vals.end(); advance(iter, 1))
	{
		*iter = unif_dist(generator);
	}

	return return_vals;
}

template<typename Container, typename Comparator>
bool sorted(const Container &vals, Comparator comp)
{
	if (vals.begin() == vals.end())
		return true;
	for (typename Container::const_iterator iter = vals.begin(); next(iter, 1) != vals.end(); advance(iter, 1))
	{
		if (!comp(*iter, *next(iter, 1)))
		{
			return false;
		}
	}
	return true;
}

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
