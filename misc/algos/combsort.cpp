/*
	Implementation of the combsort algorithm
*/

#include <iostream>
#include <vector>
#include "test_utils.h"
#include <functional>

using namespace std;

template<typename Iterator, typename Comparator>
void combsort(Iterator begin, Iterator end, Comparator comp, int n, double shrink_f)
{
	int length = distance(begin, end);
	bool sorting_done = true;
	while (n > 1 || !sorting_done)
	{
		sorting_done = true;
		for (int i = 0; i < length - n; i++)
		{
			Iterator first_comp = next(begin, i);
			Iterator second_comp = next(begin, i + n);
			if (!comp(*first_comp, *second_comp))
			{
				sorting_done = false;
				swap(*first_comp, *second_comp);
			}
		}
		if (n > 1)
		{
			n /= shrink_f;
			if (n <= 0) n = 1;
		}
	}
}

int main(int argc, char * argv [])
{
	vector<int> vec = generate_randoms<vector<int> >(200);

	combsort(vec.begin(), vec.end(), less_equal<int>(), 20, 1.3);

	show_container(vec);

	if (sorted(vec, less_equal<int>()))
	{
		cout << "Sorting OK" << endl;
	}
	else
	{
		cout << "not sorted" << endl;
	}

	return 0;
}
