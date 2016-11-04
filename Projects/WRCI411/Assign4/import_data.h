#ifndef IMPORT_DATA_H
#define IMPORT_DATA_H

#include "misc.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

static void import(string filename, nutr_data &contain, bool prices) __attribute__ ((unused));
static void show_spec_data(const nutr_data &data, bool prices) __attribute__ ((unused));


static void import(string filename, nutr_data &contain, bool prices)
{
	ifstream stream(filename);

	string line;
	string entry;
	int line_num = 0;
	int entry_num = 0;
	
	vector<string> * head_vec_ptr;
	vector<string> * str_vec_ptr;
	vector< vector<double> > * val_vec_ptr;

	if (prices)
	{
		head_vec_ptr = &contain.price_headings;
		str_vec_ptr = &contain.price_names;
		val_vec_ptr = &contain.price_values;
	}
	else
	{
		head_vec_ptr = &contain.needs_headings;
		str_vec_ptr = &contain.needs_names;
		val_vec_ptr = &contain.needs_values;
	}

	while (stream.good())
	{
		getline(stream, line, '\n');

		stringstream line_stream(line);

		entry_num = 0;

		if (line_num != 0)
		{
			val_vec_ptr->push_back(vector<double>());
		}
		while(line_stream.good())
		{
			getline(line_stream, entry, ',');
			if (line_num == 0)
			{
				head_vec_ptr->push_back(entry);
			}
			else
			{
				if (entry_num == 0)
				{
					if (entry.empty())
					{
						val_vec_ptr->pop_back();
						break;
					}
					str_vec_ptr->push_back(entry);
				}
				else
				{
					double scale = 1.0;
					if (prices && entry_num == 6)
						scale = 1000.0;
					val_vec_ptr->back().push_back(atof(entry.c_str())*scale);
				}
				entry_num++;
			}
		}
		line_num++;
	}
}

static void show_spec_data(const nutr_data &data, bool prices)
{
	const vector<string> * headings_ptr;
	const vector<string> * names_ptr;
	const vector< vector<double> > * val_ptr;

	if (prices)
	{
		headings_ptr = &data.price_headings;
		names_ptr = &data.price_names;
		val_ptr = &data.price_values;
	}
	else
	{
		headings_ptr = &data.needs_headings;
		names_ptr = &data.needs_names;
		val_ptr = &data.needs_values;
	}

	//print needs data first
	for (auto head_iter = headings_ptr->begin(); head_iter != headings_ptr->end(); advance(head_iter, 1))
	{
		cout << *head_iter << " ";
	}
	cout << endl;
	
	vector<string>::const_iterator name_iter;
	vector< vector<double> >::const_iterator line_iter;

	for (line_iter = val_ptr->begin(), name_iter = names_ptr->begin(); line_iter != val_ptr->end(); advance(name_iter, 1), advance(line_iter, 1))
	{
		cout << *name_iter << " ";
		for (auto val_iter = line_iter->begin(); val_iter != line_iter->end(); advance(val_iter, 1))
		{
			cout << *val_iter << " ";
		}
		cout << endl;
	}
}

#endif
