#ifndef PLOT_H
#define PLOT_H

#include "gnuplot-iostream/gnuplot-iostream.h"
#include <vector>
#include <utility>
#include <iostream>
#include <string>
using namespace std;

	
vector<pair<vector<int>, vector<double> > > process_data(vector< vector<double> > data);

pair<vector<double>, vector<double> > process_hist_data(vector<pair<vector<int>, vector<int> > > data);

void plot(const vector< vector<double> > &raw_data, const vector<int> &indivs_idx, 
			string title = string(), string xlabel = string(), string unit = string());

void plot_comparison(const pair<vector<int>, vector<int> > &data);

void plot_comp_bar(const vector<pair<vector<int>, vector<int> > > &raw_data,
					const vector<string> &names);
#endif

