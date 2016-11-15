/*
	this file contains implementations of some useful plotting functions for the genetic algorithm
		implementation. 

		TODO: make plot function more standalone (e.g. no processing of data may be done by plot
				function. It must only plot
*/


#include "gnuplot-iostream/gnuplot-iostream.h"
#include <vector>
#include <utility>
#include <iostream>
#include <string>
using namespace std;

	
vector<pair<vector<int>, vector<double> > > process_data(vector< vector<double> > data)
{
	int data_len = data.at(0).size();
	auto idx = vector<int>(data_len);

	vector<pair<vector<int>, vector<double> > > plot_data;

	for (int i = 0; i < data_len; i++)
		idx[i] = i + 1;

	for (auto vec_iter = data.begin(); vec_iter != data.end(); advance(vec_iter, 1))
	{
		plot_data.push_back(pair<vector<int>, vector<double> > (idx, *vec_iter));
	}
	return plot_data;
}

pair<vector<double>, vector<double> > process_hist_data(vector<pair<vector<int>, vector<int> > > data)
{
	vector<double> x_pos = {35, 50, 65};
	vector<double> y_pos;
	for (int i = 0; i < 1; i++)
		for (auto comp_iter = data.begin(); comp_iter < data.end(); advance(comp_iter, 1))
		{
			y_pos.push_back(comp_iter->second[i]);
			cout << comp_iter->second[i] << endl;
		}
	cout << "x_pos length: " << x_pos.size() << endl;
	cout << "y_pos length: " << y_pos.size() << endl;
	return {x_pos, y_pos};
}

void plot(const vector< vector<double> > &raw_data, const vector<int> &indivs_idx,
			string title, string x_label, string unit)
{
	auto data = process_data(raw_data);

	Gnuplot gp;

	if (title.size() > 0)
		gp << "set title '" << title << "'\n";
	if (x_label.size() > 0)
		gp << "set xlabel '" << x_label << "'\n";
	gp << "plot ";

	for (auto idx_iter = indivs_idx.begin(); 
		idx_iter != indivs_idx.end(); advance(idx_iter, 1))
	{
		string add_string;
		if (*idx_iter > 10 && *idx_iter <= 20)
		{
			add_string = "th";
		}
		else
		{
			int val = *idx_iter % 10;
			switch (val)
			{
				case (1):
					add_string = "st";
					break;
				case (2):
					add_string = "nd";
					break;
				case (3):
					add_string = "rd";
					break;
				default:
					add_string = "th";
			}
		}
		gp << "'-' using 1:2 with lines title '" << *idx_iter << add_string << " best individual " << unit << "'";
		if (next(idx_iter, 1) != indivs_idx.end())
			gp << ", ";
		else
			gp << "\n";
	}
	

	for (auto idx_iter = indivs_idx.begin(); 
		idx_iter != indivs_idx.end(); advance(idx_iter, 1))
	{
		gp.send1d(data[*idx_iter - 1]);
	}
	cout << "Press any key to continue..." << endl;
	cin.get();
}

void plot_comparison(const pair<vector<int>, vector<int> > &data)
{
	Gnuplot gp;

	gp << "plot '-' using 1:2 with lines title 'Iterations'\n";

	gp.send1d(data);

	cout << "Press any key to continue..." << endl;
	cin.get();
}


void plot_comp_bar(const vector<pair<vector<int>, vector<int> > > &raw_data,
					const vector<string> &names)
{
	Gnuplot gp;

	gp << "set style line 1 lc rgb 'red'\n";
	gp << "set style line 2 lc rgb 'blue'\n";
	gp << "set style line 3 lc rgb 'green'\n";
	//gp << "set style line 4 lc rgb 'yellow'\n";

	gp << "set xtics (";

	pair<vector<double>, vector<double> > dat = process_hist_data(raw_data);

	int count = 0;
	for (int i = 0; i < 1; i++)
	{
		int pos = 15 + i*100;
		gp << "\"" << pos << "\" " << pos;
		if (i < 0)
			gp << ", ";
		else
			gp << ")\n";
	}

	gp << "set boxwidth 8\n";
	gp << "set style fill solid\n";

	gp << "plot ";

	count = 1;

	for (auto str_iter = names.begin(); 
		str_iter != names.end(); advance(str_iter, 1), count++)
	{
		gp << "'-' every 3::" << count - 1 << " using 1:2 with boxes ls " << count << " title '" << *str_iter << "'";
		if (next(str_iter, 1) != names.end())
			gp << ", ";
		else
			gp << "\n";
	}
	
	for (auto str_iter = names.begin(); 
		str_iter != names.end(); advance(str_iter, 1), count++)
	{
		gp.send1d(dat);
	}
	cout << "Press any key to continue..." << endl;
	cin.get();
}
