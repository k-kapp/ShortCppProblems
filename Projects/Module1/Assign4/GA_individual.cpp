#include "GA.h"
GA::individual::individual(const vector< pair<double, double> > &ranges, nutr_data * data_ptr, GA * parent_ptr) : data_ptr(data_ptr), parent_ptr(parent_ptr)
{	/*
	for (const pair<double, double> &p_iter : ranges)
	{
		position.push_back(unif_r_dist(generator)*(p_iter.second - p_iter.first) + p_iter.first);
	}
	*/

	init_pos(ranges.size());

	mutate_stddev = vector<double> (77, 1.0);
	crossover_stddev = 1.0;
	meets_needs = true;

	nutrients = vector<double> (9);

	evaluate();
}

GA::individual::individual()
{}

vector<double> GA::individual::sphere_surface(const int &dim)
{
	vector<double> surface(dim);
	double sum = 0;

	for (auto &point : surface)
	{
		point = GA::norm_dist(generator);
		sum += point*point;
	}

	sum = sqrt(sum);

	for (auto &point : surface)
	{
		point /= sum;
		point *= 50;
	}

	return surface;
}

void GA::individual::init_pos(const int &dim)
{
	position = sphere_surface(dim);

	for (auto &point : position)
	{
		point *= pow(GA::unif_r_dist(generator), 1.0/77.0);
	}
}

double GA::individual::shortf_penalty()
{
	double penalty = 0;
	for (unsigned int i = 0; i < nutrients.size(); i++)
	{
		if (nutrients[i] < data_ptr->needs_values[i][0])
		{
			penalty += (data_ptr->needs_values[i][0] - nutrients[i])/data_ptr->needs_values[i][0]*10.0 + 1.0;
		}
	}
	if (penalty == 0)
	{
		meets_needs = true;
	}
	else
	{
		meets_needs = false;
	}
	return penalty;
}

void GA::individual::crossover(GA::individual &offspring)
{
	/*
	offspring = *this;	
	double rand_num;

	for (int i = 0; i < 77; i++)
	{
		rand_num = GA::unif_r_dist(GA::generator);
		if (rand_num < 0.3)
		{
			offspring.position[i] += 0.1*(pop.back().position[i] - position[i]);
		}
		else if (rand_num < 0.6)
		{
			offspring.position[i] += 0.1*(position[i] - pop.front().position[i]);
		}
	}
	*/

	crossover_stddev = 0.5;

	const individual * chosen = parent_ptr->pop[parent_ptr->unif_i_dist(generator)];

	offspring = *this;

	for (int i = 0; i < 77; i++)
	{
		double rand_one = unif_r_dist(generator);
		double rand_two = unif_r_dist(generator);

		double coeff1 = sqrt(2*rand_one);
		double coeff2 = sqrt(1/(2*(1 - rand_one)));

		if (rand_two < 0.5)
		{
			offspring.position[i] = crossover_stddev*((1 + coeff1)*chosen->position[i] + (1 - coeff2)*offspring.position[i]);
		}
		else
		{
			offspring.position[i] = crossover_stddev*((1 - coeff1)*chosen->position[i] + (1 + coeff2)*offspring.position[i]);
		}
	}

/*
	crossover_stddev = 0.05;

	const individual * chosen1 = parent_ptr->pop[parent_ptr->unif_i_dist(generator)];
	const individual * chosen2 = parent_ptr->pop[parent_ptr->unif_i_dist(generator)];

	double fitness1;
	vector<double> temp_pos;

	offspring = *this;

	
	for (int i = 0; i < 77; i++)
	{
		double rand_one = unif_r_dist(generator);

		offspring.position[i] = crossover_stddev*((1 + sqrt(2*rand_one))*(chosen1->position[i]) + (1 - sqrt(1/(2*(1 - rand_one))))*(chosen2->position[i]));
	}
	
	offspring.evaluate();
	fitness1 = offspring.fitness;
	temp_pos = offspring.position;
	for (int i = 0; i < 77; i++)
	{
		double rand_one = unif_r_dist(generator);

		offspring.position[i] = crossover_stddev*((1 - sqrt(2*rand_one))*chosen1->position[i] + (1 + sqrt(1/(2*(1 - rand_one))))*chosen2->position[i]);
	}
	offspring.evaluate();
	if (offspring.fitness > fitness1)
		offspring.position = temp_pos;
	*/
}

void GA::individual::get_offspring(GA::individual &offspring, const pair<int, int> &jump_mod)
{
	crossover(offspring);

	offspring.mutate(jump_mod);
}

void GA::individual::mutate(const pair<int, int> &jump_mod)
{
/*
	double rand_one = norm_dist(generator);

	individual offspring = *this;
	
	for (int i = 0; i < 77; i++)
		mutate_stddev[i] *= exp(1/sqrt(2*77)*rand_one + 1/sqrt(sqrt(77)*2)*norm_dist(generator));

	for (int i = 0; i < 77; i++)
	{
		offspring.position[i] += norm_dist(generator)*mutate_stddev[i];
	}
	
	offspring.evaluate();

	for (int i = 0; i < 77; i++)
	{
		this->position[i] += cauchy_dist(generator)*mutate_stddev[i];
	}
	
	evaluate();

	if (offspring.fitness < fitness)
	{
		this->position = offspring.position;
		evaluate();
	}
*/


	/*
	double rand_num;

	for (int i = 0; i < 77; i++)
	{
		rand_num = GA::unif_r_dist(GA::generator);
		if (rand_num < 0.5)
		{
			position[i] += unif_r_dist(generator)*(parent_ptr->pop.back().position[i] - position[i]);
		}
		else if (rand_num < 1.0)
		{
			position[i] += unif_r_dist(generator)*(position[i] - parent_ptr->pop.front().position[i]);
		}
	}
	*/

	
	double prob_mutate = 0;
	for (int i = 0; i < 77; i++)
	{
		/*
		if (abs(parent_ptr->pop.front().position[i] - parent_ptr->pop.back().position[i]) < 0.5)
			prob_mutate = abs(parent_ptr->pop.front().position[i] - parent_ptr->pop.back().position[i]);
		else
			prob_mutate = 0.5;
		*/

		if (parent_ptr->better_sum < parent_ptr->pop_size/50)
		{
			prob_mutate = parent_ptr->change_better[i]/parent_ptr->better_sum*40;
		}
		else
			prob_mutate = 0.5;

		if (unif_r_dist(generator) < prob_mutate)
		{
			//position[i] += norm_dist(generator)*1.0/((double)jump_mod.first*0.00 + (double)jump_mod.second + 1.0);
			position[i] += norm_dist(generator)*fitness/400.0;
		}
	}
	
}

void GA::individual::evaluate()
{
	fill(nutrients.begin(), nutrients.end(), 0);
	fitness = 0;

	for (int i = 0; i < 77; i++)
	{
		double &amount = position[i];
		/*
		if (amount < 0)
		{
			fitness = DBL_MAX;
			fill(nutrients.begin(), nutrients.end(), 0);
			break;
		}
		*/
		if (amount < 0)
		{
			amount = 0;
		}
		

		for_each(boost::make_zip_iterator(boost::make_tuple(nutrients.begin(), next(data_ptr->price_values[i].begin(), 1))), 
			 boost::make_zip_iterator(boost::make_tuple(nutrients.end(), data_ptr->price_values[i].end())),
			[amount] (const boost::tuple<double &, double &> &tup)
			{
				get<0>(tup) += get<1>(tup)*amount;
			}
		);
		fitness += amount;
		/*
		if (!needs_met(nutrients, data_ptr))
		{
			if (fitness < 1000)
				fitness += 1000;
		}
		*/
	}

	fitness += shortf_penalty();

}

bool GA::individual::operator > (const GA::individual &rhs)
{
	return (this->fitness > rhs.fitness);
}

bool GA::individual::operator < (const GA::individual &rhs)
{
	return (this->fitness < rhs.fitness);
}

bool GA::individual::operator == (const GA::individual &rhs)
{
	return (this->fitness == rhs.fitness);
}

bool GA::individual::operator >= (const GA::individual &rhs)
{
	return (*this > rhs || *this == rhs);
}

bool GA::individual::operator <= (const GA::individual &rhs)
{
	return (*this < rhs || *this == rhs);
}

bool GA::individual::operator > (const GA::individual &rhs) const
{
	return (this->fitness > rhs.fitness);
}

bool GA::individual::operator < (const GA::individual &rhs) const
{
	return (this->fitness < rhs.fitness);
}

bool GA::individual::operator == (const GA::individual &rhs) const
{
	return (this->fitness == rhs.fitness);
}

bool GA::individual::operator >= (const GA::individual &rhs) const
{
	return (*this > rhs || *this == rhs);
}

bool GA::individual::operator <= (const GA::individual &rhs) const
{
	return (*this < rhs || *this == rhs);
}



void GA::individual::show_fitness()
{
	cout << "Fitness: " << fitness << endl;
}

void GA::individual::show_position()
{
	cout << "Position: " << endl;

	for (double &val : position)
	{
		cout << val << " ";
	}
	cout << endl;
}

void GA::individual::show_attributes()
{
	if (meets_needs)
		cout << "Needs met" << endl;
	else
		cout << "Needs NOT met" << endl;
	show_fitness();
	show_position();
}

