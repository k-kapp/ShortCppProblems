#ifndef PLOT_H
#define PLOT_H

#include "../gnuplot-iostream/gnuplot-iostream.h"
#include <vector>
#include <utility>
#include <iostream>
#include <string>
using namespace std;

inline pair<vector<int>, vector<int> > merge_pairs(pair<vector<int>, vector<int> > pair1, pair<vector<int>, vector<int> > pair2)
{
	for (auto p2f_iter = pair2.first.begin(), p2s_iter = pair2.second.begin();
		p2f_iter != pair2.first.end(); advance(p2f_iter, 1), advance(p2s_iter, 1))
	{
		pair1.first.push_back(*p2f_iter);
		pair1.second.push_back(*p2s_iter);
	}
	return pair1;
}
		

class Plot
{
public:
	Plot(vector<vector<double> > series) : series(series)
	{
		if (series.size() > 0)
			series_len = series[0].size();
		idx = vector<int>(series_len);
		process_data();
	}

	Plot(pair<vector<int>, vector<int> > comparison) : comparison(comparison)
	{
	}

	Plot(vector<pair<vector<int>, vector<int> > > comp_vec, vector<string> names) : comp_vec(comp_vec), names(names)
	{
	}

	Plot()
	{}
	
	void process_data()
	{
		for (int i = 0; i < series_len; i++)
			idx[i] = i + 1;

		vector< pair<vector<int>, vector<double> > > _plot_data;

		for (auto vec_iter = series.begin(); vec_iter != series.end(); advance(vec_iter, 1))
		{
			_plot_data.push_back(pair<vector<int>, vector<double> > (idx, *vec_iter));
		}
		plot_data = _plot_data;
	}

	pair<vector<double>, vector<double> > process_hist_data()
	{
		/*
		vector<double> x_pos = {35, 45, 55, 65, 135, 145, 155, 165, 235, 245, 255, 265, 335, 345, 355, 365, 435, 445, 455, 465};
		vector<double> y_pos;
		*/

		vector<double> x_pos = {35, 50, 65};
		vector<double> y_pos;
		for (int i = 0; i < 1; i++)
			for (auto comp_iter = comp_vec.begin(); comp_iter < comp_vec.end(); advance(comp_iter, 1))
			{
				y_pos.push_back(comp_iter->second[i]/1000.0);
				cout << comp_iter->second[i] << endl;
			}
		cout << "x_pos length: " << x_pos.size() << endl;
		cout << "y_pos length: " << y_pos.size() << endl;
		return {x_pos, y_pos};
	}

	void plot(vector<int> indivs_idx)
	{
		Gnuplot gp;
	
		gp << "plot ";

		for (vector<int>::iterator idx_iter = indivs_idx.begin(); 
			idx_iter != indivs_idx.end(); advance(idx_iter, 1))
		{
			gp << "'-' using 1:2 with lines title '" << *idx_iter << "'";
			if (next(idx_iter, 1) != indivs_idx.end())
				gp << ", ";
			else
				gp << "\n";
		}

		for (vector<int>::iterator idx_iter = indivs_idx.begin(); 
			idx_iter != indivs_idx.end(); advance(idx_iter, 1))
		{
			gp.send1d(plot_data[*idx_iter]);
		}
		cin.get();
	}

	void plot_comparison()
	{
		Gnuplot gp;
	
		gp << "plot '-' using 1:2 with lines title 'Iterations'\n";

		gp.send1d(comparison);

		cin.get();
	}

	void plot_comp_scatter()
	{
		Gnuplot gp;

		gp << "plot ";

		for (vector<string>::iterator str_iter = names.begin(); 
			str_iter != names.end(); advance(str_iter, 1))
		{
			gp << "'-' using 1:2 with points title '" << *str_iter << "'";
			if (next(str_iter, 1) != names.end())
				gp << ", ";
			else
				gp << "\n";
		}
		
		for (auto dat_iter = comp_vec.begin(); 
			dat_iter != comp_vec.end(); advance(dat_iter, 1))
		{
			gp.send1d(*dat_iter);
		}
		cin.get();
	}


	void plot_comp_bar()
	{
		Gnuplot gp;

		gp << "set style line 1 lc rgb 'red'\n";
		gp << "set style line 2 lc rgb 'blue'\n";
		gp << "set style line 3 lc rgb 'green'\n";
		//gp << "set style line 4 lc rgb 'yellow'\n";

		gp << "set xtics (";

		pair<vector<double>, vector<double> > dat = process_hist_data();

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

		for (vector<string>::iterator str_iter = names.begin(); 
			str_iter != names.end(); advance(str_iter, 1), count++)
		{
			gp << "'-' every 3::" << count - 1 << " using 1:2 with boxes ls " << count << " title '" << *str_iter << "'";
			if (next(str_iter, 1) != names.end())
				gp << ", ";
			else
				gp << "\n";
		}
		
		for (vector<string>::iterator str_iter = names.begin(); 
			str_iter != names.end(); advance(str_iter, 1), count++)
		{
			gp.send1d(dat);
		}

		cin.get();
	}
private:
	vector<vector<double> > series;
	vector<int> idx;
	int series_len;
	vector<pair<vector<int>, vector<double> > > plot_data;
	pair<vector<int>, vector<int> > comparison;
	vector<pair<vector<int>, vector<int> > > comp_vec;
	vector<string> names;
};

#endif

/*
		Gnuplot gp;

		pair<vector<int>, vector<double> > series (series_idx, series_val);
		pair<vector<int>, vector<double> > solution (solution_idx, solution_val);

		gp << "plot '-' using 1:2 with lines title 'Series', '-' using 1:2 with lines title 'BuySell'\n";
		gp.send1d(series);
		gp.send1d(solution);
*/
