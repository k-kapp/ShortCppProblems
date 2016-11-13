#include "Assign3.h"
#include "Plot.h"
#include <cstring>
//#include <locale>
#include <set>

enum analysis_type
{
	ALLOY,
	POP_VS_ITER,
	AVG
};

struct iter_result
{
	int number;
	bool optimal;
};

struct avg_iter_result
{
	double avg;
	int num_optim;

	avg_iter_result() {avg = 0; num_optim = 0;}
};

struct avg_time_result
{
	double avg;
	int num_optim;
	
	avg_time_result() {avg = 0; num_optim = 0;}
};

bool is_number(const char * _str)
{
	string str(_str);
	return all_of(str.begin(), str.end(), ::isdigit);
}


string get_plot_result_str(int plot_result)
{
	switch (plot_result)
	{
		case (1):
		case (2):
		case (3):
		case (4):
		{
			auto ret_val = "'Alloy " + to_string(plot_result);
			ret_val.push_back('\'');
			return ret_val;
		}
		case (0):
			return "'Objective function'";
		default:
			return "";
	}
}

			

void alloy(int pop_size, int iter, mutate_method m_method, cross_type cross_t, set<int> plot_results)
{
	
	if (pop_size < 0)
		pop_size = 50;


	EvolAlgo obj = EvolAlgo(4, pop_size, m_method, cross_t, {{0, 100}, {0, 100}, {0, 100}, {0, 100}});

	if (iter < 0)
		obj.run(30);
	else
		obj.run(iter);


	obj.show_best();
	//obj.show_pop();

	cout << "Time taken: " << obj.get_time() << endl;

	vector<int> plot_idxes;
	
	for (int i = 1; i <= 3; i++)
		plot_idxes.push_back(i);

	for (auto res_iter = plot_results.begin(); res_iter != plot_results.end(); advance(res_iter, 1))
	{
		cout << "plotting..." << endl;
		string unit;
		if (*res_iter > 0)
			unit = "kg";
		plot(obj.get_prog_vec(*res_iter), plot_idxes, get_plot_result_str(*res_iter), 
				"'Iteration number'", unit);
	}
}

avg_iter_result run_examples(int pop_size, int iter, mutate_method mutate_m, cross_type cross_t, int num_examples)
{
	avg_iter_result avg_result;
	for (int i = 0; i < num_examples; i++)
	{
		EvolAlgo obj = EvolAlgo(4, pop_size, mutate_m, cross_t, {{0, 100}, {0, 100}, {0, 100}, {0, 100}});
		if (iter == -1)
		{
			obj.run(30);
			if (obj.best_achieved)
			{
				avg_result.avg += obj.get_optim_count();
				avg_result.num_optim++;
			}
		}
		else
		{
			obj.run(iter);
			if (obj.best_achieved)
			{
				avg_result.num_optim++;
			}
		}
		obj.show_best();
	}
	
	if (avg_result.num_optim > 0)
		avg_result.avg /= avg_result.num_optim;
	else
		avg_result.avg = 0;

	cout << "average iterations: " << avg_result.avg << endl;
	cout << "examples that were optimal: " << avg_result.num_optim << " out of " << num_examples << endl;

	return avg_result;
}


avg_time_result run_time_examples(int pop_size, int iter, mutate_method mutate_m, cross_type cross_t, int num_examples)
{
	avg_time_result avg_result;
	for (int i = 0; i < num_examples; i++)
	{
		EvolAlgo obj = EvolAlgo(4, pop_size, mutate_m, cross_t, {{0, 100}, {0, 100}, {0, 100}, {0, 100}});
		if (iter == -1)
		{
			obj.run(30);
			if (obj.best_achieved)
			{
				avg_result.avg += obj.get_time();
				avg_result.num_optim++;
			}
		}
		else
		{
			obj.run(iter);
			if (obj.best_achieved)
			{
				avg_result.avg += obj.get_time();
				avg_result.num_optim++;
			}
		}
		obj.show_best();
	}

	if (avg_result.num_optim > 0)
		avg_result.avg /= (double)avg_result.num_optim;
	else
		avg_result.avg = 0;

	cout << "average iterations: " << avg_result.avg << endl;
	cout << "examples that were optimal: " << avg_result.num_optim << " out of " << num_examples << endl;
	cout << "avg_result.avg = " << avg_result.avg << endl;


	return avg_result;
}

pair<vector<int>, vector<int> > pop_vs_iter(pair<int, int> pop_range, mutate_method m_method, cross_type cross_t)
{
	vector<int> pop_sizes;
	vector<int> iterations;
	for (int pop_size = pop_range.first; pop_size < pop_range.second; pop_size += 100)
	{

		EvolAlgo obj = EvolAlgo(4, pop_size, m_method, cross_t,  {{0, 100}, {0, 100}, {0, 100}, {0, 100}});
		obj.run(30);
		pop_sizes.push_back(pop_size);
		iterations.push_back(run_time_examples(pop_size, -1, m_method, cross_t, 30).avg*1000);
	}
	return {pop_sizes, iterations};
}

void pop_vs_iter(pair<int, int> pop_range)
{
	vector<int> pop_sizes;
	vector<int> iterations;
	mutate_method m_method = RNORM;
	cross_type cross_t = {STD, 0};
	for (int pop_size = pop_range.first; pop_size < pop_range.second; pop_size += 100)
	{

		EvolAlgo obj = EvolAlgo(4, pop_size, m_method, cross_t,  {{0, 100}, {0, 100}, {0, 100}, {0, 100}});
		obj.run(30);
		pop_sizes.push_back(pop_size);
		iterations.push_back(obj.get_optim_count());
	}
	plot_comparison({pop_sizes, iterations});
}

void aa_pop_vs_iter()
{
	vector<pair<vector<int>, vector<int> > > plot_data;
	vector<string> name_data;

	vector<mutate_method> mut_vec = {PPULL, RNORM};
	vector<cross_type> cross_vec = {{STD, 0}, {WEIGHT, 5}};
/*
	for (auto mut_iter = mut_vec.begin(); mut_iter != mut_vec.end(); advance(mut_iter, 1))
	{
		for (auto cross_iter = cross_vec.begin(); cross_iter != cross_vec.end(); advance(cross_iter, 1))
		{
			plot_data.push_back(pop_vs_iter({50, 550}, *mut_iter, *cross_iter));
			if (cross_iter->cross_m == STD)
				name_data.push_back("Directional Crossover/");
			else
				name_data.push_back("Weighted Crossover/");
			if (*mut_iter == PPULL)	
				name_data.back() += "Uniform Mutation";
			else
				name_data.back() += "Gaussian Mutation";
		}
	}
*/	
	name_data.push_back("Gaussian Mutation");
	name_data.push_back("Gaussian Mutation/Directional Crossover");
	name_data.push_back("Gaussian Mutation/Weighted Crossover");
	
	plot_data.push_back(pop_vs_iter({15, 20}, RNORM, {NONE, 0}));
	plot_data.push_back(pop_vs_iter({15, 20}, RNORM, {STD, 0}));
	plot_data.push_back(pop_vs_iter({15, 20}, RNORM, {WEIGHT, 3}));
	
	plot_comp_bar(plot_data, name_data);
}

void aa_time_pop_vs_iter()
{
	vector<pair<vector<int>, vector<int> > > plot_data;
	vector<string> name_data;

	vector<mutate_method> mut_vec = {PPULL, RNORM};
	vector<cross_type> cross_vec = {{STD, 0}, {WEIGHT, 5}};
	/*
	for (auto mut_iter = mut_vec.begin(); mut_iter != mut_vec.end(); advance(mut_iter, 1))
	{
		for (auto cross_iter = cross_vec.begin(); cross_iter != cross_vec.end(); advance(cross_iter, 1))
		{
			plot_data.push_back(pop_vs_iter({50, 550}, *mut_iter, *cross_iter));
			if (cross_iter->cross_m == STD)
				name_data.push_back("Directional Crossover/");
			else
				name_data.push_back("Weighted Crossover/");
			if (*mut_iter == PPULL)	
				name_data.back() += "Uniform Mutation";
			else
				name_data.back() += "Gaussian Mutation";
		}
	}
	*/

	name_data.push_back("Gaussian Mutation");
	name_data.push_back("Gaussian Mutation/Directional Crossover");
	name_data.push_back("Uniform Mutation/Directional Crossover");
	
	plot_data.push_back(pop_vs_iter({50, 100}, RNORM, {NONE, 0}));
	plot_data.push_back(pop_vs_iter({50, 100}, RNORM, {STD, 0}));
	plot_data.push_back(pop_vs_iter({50, 100}, PPULL, {STD, 0}));

	
	//Plot plt(plot_data, name_data);
	//plt.plot_comp_bar();
	plot_comp_bar(plot_data, name_data);
}
int main(int argc, char * argv [])
{
	int iter = -1;
	int pop_size = -1;
	set<int> plot_results;
	mutate_method m_method;
	analysis_type analysis;
	cross_type cross_t;

	m_method = PPULL;
	cross_t  = {STD, 1};
	analysis = ALLOY;
	
	int num_examples = 0;


	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-p"))
		{
			i++;
			if (!is_number(argv[i]))
			{
				cout << "Population size argument must be an integer" << endl;
				return 1;
			}
			pop_size = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-comp_all"))
		{
			aa_pop_vs_iter();
			return 0;
		}
		else if (!strcmp(argv[i], "-comp_all_time"))
		{
			aa_time_pop_vs_iter();
			return 0;
		}
		else if (!strcmp(argv[i], "-i"))
		{
			i++;
			if (!is_number(argv[i]))
			{
				cout << "Iteration argument must be an integer" << endl;
				return 1;
			}
			iter = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-a1"))
		{
			plot_results.insert(1);
		}
		else if (!strcmp(argv[i], "-a2"))
		{
			plot_results.insert(2);
		}
		else if (!strcmp(argv[i], "-a3"))
		{
			plot_results.insert(3);
		}
		else if (!strcmp(argv[i], "-a4"))
		{
			plot_results.insert(4);
		}
		else if (!strcmp(argv[i], "-f"))
		{
			plot_results.insert(0);
		}
		else if (!strcmp(argv[i], "-Pall"))
		{
			for (int i = 0; i < 5; i++)
				plot_results.insert(i);
		}
		else if (!strcmp(argv[i], "-rnorm"))
		{
			m_method = RNORM;
		}
		else if (!strcmp(argv[i], "-ppull"))
		{
			m_method = PPULL;
		}
		else if (!strcmp(argv[i], "-alloy"))
		{
			analysis = ALLOY;
		}
		else if (!strcmp(argv[i], "-pop_iter"))
		{
			analysis = POP_VS_ITER;
		}
		else if (!strcmp(argv[i], "-cross_std"))
		{
			cross_t.cross_m = STD;
			cross_t.num_choices = 0;
		}
		else if (!strcmp(argv[i], "-cross_weight"))
		{
			cross_t.cross_m = WEIGHT;
			i++;
			if (!is_number(argv[i]))
			{
				cout << "Argument that follows weight crossover specification must be an integer" << endl;
				return 1;
			}
			cross_t.num_choices = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-eg"))
		{
			analysis = AVG;	
			i++;
			if (!is_number(argv[i]))
			{
				cout << "Argument that follows examples analysis specification must be an integer" << endl;
				return 1;
			}
			num_examples = atoi(argv[i]);
		}
			
	}

	switch (analysis)
	{
		case (ALLOY):
			alloy(pop_size, iter, m_method, cross_t, plot_results);
			break;
		case (POP_VS_ITER):
			pop_vs_iter({50, 550});
			break;
		case (AVG):
			run_examples(pop_size, iter, m_method, cross_t, num_examples);
			break;
	}
	
	return 0;

}
