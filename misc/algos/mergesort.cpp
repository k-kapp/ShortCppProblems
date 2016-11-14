/*
	memory-efficient implementation of mergesort
*/


#include <iostream>
#include <vector>
#include <list>
#include <functional>

using namespace std;


/*
	helper function for mergesort. merges two adjacent, ordered linked lists into
	a single ordered linked list
*/
template<typename T>
list<T> merge_two(list<T> &first, list<T> &second, function<bool(T, T)> comp)
{
	list<T> new_vec;
	int fwd_count = 0;

	for (auto first_iter = first.begin(); first_iter != first.end(); )
	{
		bool inserted = false;
		for (auto second_iter = second.begin(); 
				second_iter != second.end(); 
				)
		{
			if (comp(*first_iter, *second_iter))
			{
				new_vec.push_back(*first_iter);
				first_iter = first.erase(first_iter);
				inserted = true;
				break;
			}
			else
			{
				new_vec.push_back(*second_iter);
				second_iter = second.erase(second_iter);
				fwd_count++;
			}
		}
		if (!inserted)
		{
			new_vec.push_back(*first_iter);
			advance(first_iter, 1);
		}
	}

	for (auto second_iter = second.begin(); second_iter != second.end();)
	{
		new_vec.push_back(*second_iter);
		second_iter = second.erase(second_iter);
	}


	return new_vec;
}

template<typename T>
void mergesort(T &container, function<bool(typename T::value_type, typename T::value_type)> comp_func)
{
	list<list<typename T::value_type> > temp;
	for (auto iter = container.begin(); iter != container.end();)
	{
		temp.push_back(list<typename T::value_type>{*iter});
		iter = container.erase(iter);
	}


	while (temp.size() > 1)
	{
		auto curr_iter = temp.begin();
		while ((curr_iter != temp.end()) && (curr_iter != next(temp.end(), -1)))
		{
			auto comp1 = move(*curr_iter);
			curr_iter = temp.erase(curr_iter);
			auto comp2 = move(*curr_iter);
			curr_iter = temp.erase(curr_iter);
			curr_iter = temp.insert(curr_iter, merge_two(comp1, comp2, comp_func));
			advance(curr_iter, 1);
		}
	}

	container = T(temp.front().begin(), temp.front().end());
}

int main(int argc, char * argv [])
{
	vector<int> my_vec = {8, 9, 4, 11, 15, 2, 3, 5, 1, 2, 69, 8, 8, 8, 9, 10, 4};

	mergesort(my_vec, function<bool(int, int)>(less<int>()));

	for (auto &val : my_vec)
	{
		cout << val << " ";
	}
	cout << endl;

	return 0;
}

