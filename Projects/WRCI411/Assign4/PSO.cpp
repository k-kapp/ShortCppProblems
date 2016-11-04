#include "PSO.h"

default_random_engine PSO::generator;
normal_distribution<double> PSO::norm_dist;
uniform_real_distribution<double> PSO::unif_r_dist;
uniform_int_distribution<int> PSO::unif_i_dist;


PSO::PSO(nutr_data data, int dim, int pop_size, int local_size) : data(data), dim(dim), pop_size(pop_size), local_size(local_size)
{
	int seed_time = chrono::steady_clock::now().time_since_epoch().count();
	
	PSO::unif_r_dist = uniform_real_distribution<double> (0.0, 1.0);
	PSO::generator.seed(seed_time);

	no_change = 0;

	cout << "local size: " << this->local_size << endl;

	if (local_size > pop_size/2)
		local_size = pop_size/2;

	local_bests = deque<individual> (pop_size);

	init_pop();
	best_indiv = pop.front();
	update_best_indiv();
	get_local_bests();
	no_change = 0;
}

void PSO::init_pop()
{
	vector< pair<double, double> > ranges(77, {0.0, 50.0});
	for (int i = 0; i < pop_size; i++)
	{
		pop.push_back(PSO::individual(ranges, &data));
	}
}

void PSO::update_best_indiv()
{
	bool changed = false;
	for (auto indiv_iter = pop.begin(); indiv_iter != pop.end(); advance(indiv_iter, 1))
	{
		if (indiv_iter->fitness < best_indiv.fitness)
		{
			best_indiv = *indiv_iter;
			changed = true;
		}
	}
	if (changed)
		no_change = 0;
	else
		no_change++;
}

PSO::individual * PSO::get_area_best(pair<int, int> &range)
{
	individual * best = &pop[range.first];

	range.first++;

	for ( ; range.first <= range.second; range.first++)
	{
		if (pop[range.first].fitness < best->fitness)
		{
			best = &pop[range.first];
		}
	}

	return best;
}

void PSO::get_indiv_loc_best(int indiv_idx)
{
	pair<int, int> range;
	range.first = (indiv_idx < local_size ? 0 : indiv_idx - local_size);
	range.second = (pop_size - 1 < local_size + indiv_idx ? pop_size - 1 : indiv_idx + local_size);

	local_bests[indiv_idx] = *get_area_best(range);
}

//call this function in main run function
void PSO::get_local_bests()
{
	for (int i = 0; i < pop_size; i++)
	{
		get_indiv_loc_best(i);
	}
}

void PSO::update_fitness()
{
	for (auto indiv_iter = pop.begin(); indiv_iter != pop.end(); advance(indiv_iter, 1))
	{
		indiv_iter->update_fitness();	
	}
}

void PSO::do_updates(int idx, const vector<double> &best_pos)
{
	pop[idx].update_velocity(best_pos);
	pop[idx].update_position();
}

void PSO::iteration()
{
	vector<thread> thread_vec(4);


	for (int i = 0; i < pop_size; i++)
	{
		do_updates(i, local_bests[i].position);
	}

	/*
	int count = 0;	
	for (auto pop_iter = pop.begin(); pop_iter != pop.end(); advance(pop_iter, 4))
	{


		count = 0;
		//cout << "making threads" << endl;
		for (thread &t : thread_vec)
		{
			t = thread(&PSO::do_updates, this, count);
			count++;
		}
		//cout << "done making threads" << endl;
		for (thread &t : thread_vec)
		{
			t.join();
		}
		//cout << "done joining threads" << endl;
		//do_updates(pop_iter);
	}
	*/

	//cout << "updating best individual" << endl;
	update_best_indiv();

	get_local_bests();

	//cout << "done updating best individual" << endl;
}

void PSO::run(int stop_after)
{
	/*
	int count = 0;
	while (count < 10)
	{
		cout << "beginning iteration " << count << endl;
		iteration();
		count++;
	}
	*/
	
	int count = 0;

	while (true)
	{
		fitness_historical.push_back(get_best_fitness());
		iteration();
		count++;
		//cout << "no_change = " << no_change << endl;
		//show_best_position();
		//cout << "best fitness: " << get_best_fitness() << endl;
		//if (count > 5)
		//	break;
		if (count % 100 == 0)
			show_best();
		if (count == 800)
			break;
	}
	cout << "count: " << count << endl;
}

double PSO::get_best_fitness()
{
	return best_indiv.fitness;
}

vector<double> PSO::get_best_pos()
{
	return best_indiv.position;
}

void PSO::show_best_position()
{
	for (auto pos_iter = best_indiv.position.begin(); pos_iter != best_indiv.position.end(); advance(pos_iter, 1))
	{
		cout << *pos_iter << " ";
	}
	cout << endl;
}

void PSO::show_pop()
{
	cout << "fitness for each individual: " << endl;
	for (auto pop_iter = pop.begin(); pop_iter != pop.end(); advance(pop_iter, 1))
	{
		cout << pop_iter->fitness << endl;
		pop_iter->show_position();
		
	}
}

void PSO::show_best()
{
	best_indiv.show_attributes();
}

vector<double> PSO::best_pos_weights()
{
	vector<double> pos = get_best_pos();

	for (int i = 0; i < 77; i++)
	{
		cout << "weight: " << data.price_values[i][0] << endl;
		pos[i] *= data.price_values[i][0];
	}
	
	return pos;
}
