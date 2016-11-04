#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <tuple>

using namespace std;

int main()
{
	vector<double> my_vec1 (5, 1);
	vector<double> my_vec2 (5, 2);

	double var1, var2;
	double const & var = var1;

	var = 8;

	const tuple<double&, double&> my_tup(var1, var2);

	cout << get<0>(my_tup) << endl;

	var1 = 5;
	var2 = 6;

	cout << get<0>(my_tup) << endl;
	cout << get<1>(my_tup) << endl;

	get<0>(my_tup) = 100;
	get<1>(my_tup) = 200;

	cout << get<0>(my_tup) << endl;
	cout << get<1>(my_tup) << endl;

	for_each(boost::make_zip_iterator(boost::make_tuple(my_vec1.begin(), my_vec2.begin())), 
		 boost::make_zip_iterator(boost::make_tuple(my_vec1.end(), my_vec2.end())),
		 [](const boost::tuple<double &, double &> &tup) { boost::get<0>(tup) = 5154;});
	for_each(boost::make_zip_iterator(boost::make_tuple(my_vec1.begin(), my_vec2.begin())), 
		 boost::make_zip_iterator(boost::make_tuple(my_vec1.end(), my_vec2.end())),
		 [](const boost::tuple<double &, double &> &tup) { cout << boost::get<0>(tup) << endl;});
	/*
	for_each(my_vec.begin(), my_vec.end(), [](double &num) { num = 3215;});

	for_each(my_vec.begin(), my_vec.end(), [](double &num) { cout << num << endl;});
	*/
	return 0;
}
