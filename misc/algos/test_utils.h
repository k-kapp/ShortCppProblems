#include <random>

using namespace std;

/*
	the following two functions are utility functions to check whether sorting
		was done properly.
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

template<typename Container>
void show_container(Container ctr)
{
	for (auto &val : ctr)
	{
		cout << val << " ";
	}
	cout << endl;
}
