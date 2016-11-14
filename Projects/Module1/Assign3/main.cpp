#include "Assign3.h"
#include "Plot.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

double colville_func(const vector<double> &inputs)
{
	return 100*pow((inputs.at(0) - pow(inputs.at(1), 2)), 2) + pow((1 - inputs.at(0)), 2) + 
			90*pow(inputs.at(3) - pow(inputs.at(2), 2), 2) + pow(1 - inputs.at(2), 2) + 
			10.1*(pow(inputs.at(1) - 1, 2) + pow(inputs.at(3) - 1, 2)) + 
			19.8*(inputs.at(1) - 1)*(inputs.at(3) - 1);
}

void plot_types(EvolAlgo &obj, vector<int> prog_types)
{
	for (auto int_iter = prog_types.begin(); int_iter != prog_types.end(); advance(int_iter, 1))
	{
		string title_str = "Progression for ";
		if (*int_iter >= obj.get_dim())
			title_str += "fitness value";
		else
			title_str += "variable " + to_string(*int_iter);
		plot(obj.get_prog_vec(*int_iter), {1, 2, 3}, title_str, "Iteration number", "");
	}
}

int main(int argc, char * argv [])
{
	int pop_size = 100;
	int no_change_stop = 100;
	int max_iter = -1;
	int dim = -1;

	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "-psize")
		{
			if (i + 1 == argc)
			{
				cout << "population size flag must be passed positive, nonzero integer" << endl;
				return EXIT_FAILURE;
			}
			else
			{
				try
				{
					pop_size = stoi(string(argv[i + 1]));
					if (pop_size < 1)
						throw invalid_argument("integer must be nonzero and positive");
				}
				catch (invalid_argument &a)
				{
					cout << "population size parameter must be a positive, nonzero integer" << endl;
					return EXIT_FAILURE;
				}
			}
			i++;
		}
		if (string(argv[i]) == "-no_chg_stop")
		{
			if (i + 1 == argc)
			{
				cout << "no change stop flag must be passed a positive, nonzero integer" << endl;
				return EXIT_FAILURE;
			}
			else
			{
				try
				{
					no_change_stop = stoi(string(argv[i + 1]));
					if (no_change_stop < 1)
						throw invalid_argument("integer must be nonzero and positive");
				}
				catch (invalid_argument &a)
				{
					cout << "no change stop parameter must be a positive nonzero integer" << endl;
					return EXIT_FAILURE;
				}
			}
		}
		if (string(argv[i]) == "-max_iter")
		{
			if (i + 1 == argc)
			{
				cout << "maximum iterations flag must be passed a positive, nonzero integer" << endl;
				return EXIT_FAILURE;
			}
			else
			{
				try
				{
					max_iter = stoi(string(argv[i + 1]));
				}
				catch (invalid_argument &a)
				{
					cout << "maximum iterations parameter must be a positive, nonzero integer" << endl;
					return EXIT_FAILURE;
				}
			}
		}
		if (string(argv[i]) == "-dim")
		{
			if (i + 1 == argc)
			{
				cout << "number of dimensions flag must be passed a positive, nonzero integer" << endl;
				return EXIT_FAILURE;
			}
			else
			{
				try
				{
					dim = stoi(string(argv[i + 1]));
				}
				catch (invalid_argument &a)
				{
					cout << "number of dimensions must be a positive, nonzero integer" << endl;
					return EXIT_FAILURE;
				}
			}
		}
	}

	if (dim == -1)
	{
		cout << "Error: number of dimensions must be set. It may be set with -dim <positive int value>" << endl;
		return EXIT_FAILURE;
	}

	function<double(const vector<double> &)> obj_func(colville_func);
	mutate_method mutate_m = PPULL;
	cross_type cross_std = {STD, 0};
	vector<pair<double, double> > dim_limits(4, {-10, 10});
	auto ea_obj = EvolAlgo(dim, pop_size, obj_func, false, mutate_m, cross_std, dim_limits);

	if (max_iter == -1)
		ea_obj.run(no_change_stop);
	else
		ea_obj.run(no_change_stop, max_iter);
	
	plot_types(ea_obj, {0, 1, 2, 3, 4});

	return 0;
}
