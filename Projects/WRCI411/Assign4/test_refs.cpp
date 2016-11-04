#include <algorithm>
#include <deque>
#include <iostream>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/tuple/tuple.hpp>
using namespace std;
/*
template<class...Conts>
static auto zip_cs (Conts&...cs) -> 
	decltype (boost::make_iterator_range(boost::make_zip_iterator(boost::make_tuple(cs.begin()...)),
						boost::make_zip_iterator(boost::make_tuple(cs.end()...))))
{
	return boost::make_iterator_range(boost::make_zip_iterator(boost::make_tuple(cs.begin()...)),
						boost::make_zip_iterator(boost::make_tuple(cs.end()...)));
}
*/
template<class...Conts>
static auto zip_cs (Conts&...cs) -> 
	decltype (boost::make_iterator_range(boost::make_zip_iterator(boost::make_tuple(cs.begin()...)),
						boost::make_zip_iterator(boost::make_tuple(cs.end()...))))
{
	return {boost::make_zip_iterator(boost::make_tuple(cs.begin()...)),
						boost::make_zip_iterator(boost::make_tuple(cs.end()...))};
}

int main()
{
	deque<int> deq = {1, 2, 3, 4, 5};
	deque<int> deq2 = {1, 5, 3, 5, 1};

	int &ref = deq.back();
	
//	iter_swap(next(deq.end(), -1), next(deq.begin(), 1));

	//deq.insert(next(deq.begin(), 3), 100);


	for (auto &&iter : zip_cs(deq, deq2))
	{
		get<0>(iter) = 5000;
	}

	for (auto &&iter2 : zip_cs(deq, deq2))
	{
		cout << get<0>(iter2) << " " << get<1>(iter2) << endl;
	}

	//cout << ref << endl;
}
