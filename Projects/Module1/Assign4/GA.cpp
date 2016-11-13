#include "GA.h"

bool GA::ptr_comp(GA::individual * ptr1, GA::individual * ptr2)
{
	return ptr1->fitness > ptr2->fitness;
}

double sum_vec(const vector<double> &vec)
{
	double sum = 0;

	for (const auto &el : vec)
	{
		sum += el;
	}
	return sum;
}

void abs_vecs(vector<double> & target, vector<double> &vec1, vector<double> &vec2)
{
	for (auto &&tup : zip_cs(target, vec1, vec2))
	{
		tup.get<0>() += abs(tup.get<1>() - tup.get<2>());
	}
}

default_random_engine GA::generator = default_random_engine
(
	chrono::steady_clock::now().time_since_epoch().count()
);

uniform_real_distribution<double> GA::unif_r_dist = uniform_real_distribution<double> (0.0, 1.0);
normal_distribution<double> GA::norm_dist = normal_distribution<double> (0.0, 1.0);
cauchy_distribution<double> GA::cauchy_dist = cauchy_distribution<double>();

GA::GA(nutr_data &data, int pop_size, const vector< pair<double, double> > &ranges, int dim) : data(data), pop_size(pop_size), dim(dim)
{
	init_pop(ranges);
	offspring_vec = vector<individual *>(pop_size);

	unif_i_dist = uniform_int_distribution<int> (0, pop_size - 1);

	change_better = vector<double> (77, 1.0);
	better_sum = 77.0;
}

void GA::init_pop(const vector< pair<double, double> > &ranges)
{
	for (int i = 0; i < pop_size; i++)
	{
		pop.push_back(new individual(ranges, &data, this));
	}

	//sort(pop.begin(), pop.end(), greater<GA::individual>());
	sort(pop.begin(), pop.end(), GA::ptr_comp);
}

void GA::offspring_vector()
{
	vector<double> new_change_better = vector<double> (change_better.size(), 0);
	int count = 0;

	//for_each(offspring_vec.begin(), offspring_vec.end(), [] (individual * indiv_ptr) { delete indiv_ptr; });

	for_each(offspring_vec.begin(), offspring_vec.end(), [] (individual * &indiv_ptr){ indiv_ptr = new individual(); });

	for (individual * indiv_ptr : pop)
	{
		indiv_ptr->get_offspring(*offspring_vec[count], {iters, no_change});
		offspring_vec[count]->evaluate();
		if (offspring_vec[count]->fitness < indiv_ptr->fitness)
		{
			abs_vecs(new_change_better, offspring_vec[count]->position, indiv_ptr->position);
		}
		count++;
	}
	better_sum = sum_vec(new_change_better);
	change_better = new_change_better;
	
}

void GA::insert_one(individual * offspring)
{
	//cout << "in insert_one" << endl;
	if (*offspring > *pop.front())
	{
		delete offspring;
		return;
	}

	bool inserted = false;

	for (auto pop_iter = next(pop.begin(), 1); pop_iter != pop.end(); advance(pop_iter, 1))
	{
		if (*offspring >= *(*pop_iter))
		{
			pop.insert(pop_iter, offspring);
			//cout << "inserting" << endl;
			inserted = true;
			break;
		}
	}
	if (!inserted)
	{
		pop.push_back(offspring);
	}
	delete pop.front();
	pop.pop_front();
}

void GA::insert_offspring()
{
	for (individual * indiv : offspring_vec)
	{
		insert_one(indiv);
	}
}

void GA::new_generation(individual * best)
{

	vector<double> new_change_better = vector<double> (change_better.size(), 0);
	
	for (int i = 0; i < pop_size; i++)
	{
		individual * offspring = new individual();
		pop[i]->get_offspring(*offspring, {iters, no_change});
		offspring->evaluate();
		if (offspring->fitness < pop[i]->fitness)
		{
			abs_vecs(new_change_better, offspring->position, pop[i]->position);
			delete pop[i];
			pop[i] = offspring;
			if (offspring->fitness < best->fitness)
				best = pop[i];
		}
	}
	better_sum = sum_vec(new_change_better);
	change_better = new_change_better;
}

void GA::iteration()
{
	offspring_vector();
	insert_offspring();
}	

void GA::run(int stop_after)
{
	iters = 0;
	//individual best;
	double prev_best_fitness;

	//individual * best_ptr = &pop.back();
	//double prev_best_fitness = best_ptr->fitness;
	
	no_change = 0;

	while (no_change < stop_after)
	{
		prev_best_fitness = pop.back()->fitness;
		fitness_historical.push_back(get_best()->fitness);
		iteration();
		//new_generation(best_ptr);
		if (prev_best_fitness > pop.back()->fitness)
		//if (best_ptr->fitness < prev_best_fitness)
		{
			no_change = 0;
		}
		else
		{
			no_change++;
		}
		iters++;
		//prev_best_fitness = best_ptr->fitness;
		if (iters % 100 == 0)
		{
			//show_best(best_ptr);
			show_best();
		}
		//if (iters > 2000)
		//	break;
	}
	cout << "Number of iterations to get to optimal point: " << iters - stop_after << endl;
}

void GA::show_best()
{
	pop.back()->show_attributes();
}

void GA::show_best(individual * best)
{
	best->show_attributes();
}

void GA::show_best_fitness()
{
	pop.back()->show_fitness();
}

GA::individual * GA::get_best()
{
	individual * best = pop.front();
	for (auto &indiv : pop)
	{
		if (indiv->fitness < best->fitness)
			best = indiv;
	}
	return best;
}

vector<double> GA::get_best_pos()
{
	return get_best()->position;
}

vector<double> GA::best_pos_weights()
{
	vector<double> pos = get_best()->position;

	for (int i = 0; i < 77; i++)
	{
		cout << "weight: " << data.price_values[i][0] << endl;
		pos[i] *= data.price_values[i][0];
	}
	
	return pos;
}
