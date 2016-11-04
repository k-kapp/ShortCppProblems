#ifndef MISC_H
#define MISC_H

#include <vector>
#include <iostream>
#include <deque>

#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/range/iterator_range.hpp>

using namespace std;

struct nutr_data
{
	vector<string> price_headings;	
	vector<string> price_names;
	vector< vector<double> > price_values;
	
	vector<string> needs_headings;
	vector<string> needs_names;
	vector< vector<double> > needs_values;
};

static bool needs_met(const vector<double> &nutrients, nutr_data * data_ptr) __attribute__((unused));

static bool needs_met(const vector<double> &nutrients, nutr_data * data_ptr)
{
	for (unsigned int i = 0; i < nutrients.size(); i++)
	{
		if (nutrients[i] < data_ptr->needs_values[i][0])
		{
			return false;
		}
	}
	return true;
}


template<class...Conts>
static auto zip_cs (Conts&...cs) -> 
	decltype (boost::make_iterator_range(boost::make_zip_iterator(boost::make_tuple(cs.begin()...)),
						boost::make_zip_iterator(boost::make_tuple(cs.end()...))))
{
	return {boost::make_zip_iterator(boost::make_tuple(cs.begin()...)),
						boost::make_zip_iterator(boost::make_tuple(cs.end()...))};
}

#endif
