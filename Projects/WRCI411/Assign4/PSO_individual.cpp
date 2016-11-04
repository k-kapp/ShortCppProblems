#include "PSO.h"

PSO::individual::individual(int dim, nutr_data * data_ptr) : data_ptr(data_ptr)
{
	position = vector<double>(dim);
}

PSO::individual::individual(vector< pair<double, double> > ranges, nutr_data * data_ptr) : data_ptr(data_ptr)
{
	/*
	for (auto range_iter = ranges.begin(); range_iter != ranges.end(); advance(range_iter, 1))
	{
		position.push_back(PSO::unif_r_dist(generator)*(range_iter->second - range_iter->first) + range_iter->first);
	}
	*/

	init_pos(ranges.size());

	best_fitness = DBL_MAX;
	update_fitness();
	init_velocity();
	init_params();
	nutrients = vector<double>(9, 0);

	
}

PSO::individual::individual()
{}

vector<double> PSO::individual::sphere_surface(const int &dim)
{
	vector<double> surface(dim);
	double sum = 0;

	for (auto &point : surface)
	{
		point = PSO::norm_dist(PSO::generator);
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

void PSO::individual::init_pos(const int &dim)
{
	position = sphere_surface(dim);

	for (auto &point : position)
	{
		point *= pow(PSO::unif_r_dist(generator), 1.0/77.0);
	}
}

void PSO::individual::init_velocity()
{
	velocity = vector<double>(position.size(), 0);
	for (auto vel_iter = velocity.begin(); vel_iter != velocity.end(); advance(vel_iter, 1))
	{
		*vel_iter = 0;
	}
}

void PSO::individual::init_params()
{
	scale_cog = 0.8;
	scale_social = 0.8;
	damp = 0.99;
}

void PSO::individual::show_position()
{
	for (auto pos_iter = position.begin(); pos_iter != position.end(); advance(pos_iter, 1))
	{
		cout << *pos_iter << " ";
	}
	cout << endl;
}

void PSO::individual::update_velocity(const vector<double> &social_target)
{
	/*
	for (int i = 0; i < velocity.size(); i++)
	{
		velocity[i] = velocity[i]*damp + scale_cog*(best_position[i] - position[i]) + scale_social*(best_indiv.position[i] - position[i]);
	}
	*/


	for_each(boost::make_zip_iterator(boost::make_tuple(velocity.begin(), position.begin(), best_position.begin(), social_target.begin())), 
		 boost::make_zip_iterator(boost::make_tuple(velocity.end(), position.end(), best_position.end(), social_target.end())), 
			[this](boost::tuple<double &, double &, double &, const double &> const& tup)
			{/*
				cout << get<0>(tup) << endl;
				cout << get<1>(tup) << endl;
				cout << get<2>(tup) << endl;
				cout << get<3>(tup) << endl;
			*/
			//	cout << "velocity before change: " << get<0>(tup) << endl;
				get<0>(tup) = get<0>(tup)*this->damp + this->scale_cog*(get<2>(tup) - get<1>(tup)) 
						+ this->scale_social*(get<3>(tup) - get<1>(tup));
				
			//	cout << "velocity after change: " << get<0>(tup) << endl;
				
				/*
				for (auto vel_iter = velocity.begin(); vel_iter != velocity.end(); advance(vel_iter, 1))
				{
					cout << *vel_iter << endl;
				}
				*/
			}
		);

}

void PSO::individual::update_position()
{
	for_each(boost::make_zip_iterator(boost::make_tuple(position.begin(), velocity.begin())), 
		 boost::make_zip_iterator(boost::make_tuple(position.end(), velocity.end())),
		
		[this](boost::tuple<double &, double &> const& tup)
		{
			get<0>(tup) += get<1>(tup);
			if (get<0>(tup) < 0)
			{
				get<0>(tup) = 0;
				get<1>(tup) = 0;
			}
		}
	);

	update_fitness();
}	

double PSO::individual::shortf_penalty()
{
	double penalty = 0;
	needs_met = true;
	for (unsigned int i = 0; i < nutrients.size(); i++)
	{
		if (nutrients[i] < data_ptr->needs_values[i][0])
		{
			penalty += (data_ptr->needs_values[i][0] - nutrients[i])/data_ptr->needs_values[i][0]*20.0 + 1.0;
			needs_met = false;
			//penalty += 50;
		}
	}
	return penalty;
}

void PSO::individual::update_fitness()
{
	double prev_fitness = fitness;
	fitness = 0;
	fill(nutrients.begin(), nutrients.end(), 0);
	for (unsigned int i = 0; i < position.size(); i++)
	{
		if (position[i] < 0)
		{
			position[i] = 0;
			//fitness += 1000;
		}
		for (unsigned int j = 0; j < nutrients.size(); j++)
		{
			nutrients[j] += data_ptr->price_values[i][j + 1]*position[i];
		}
		fitness += position[i];
	}

	fitness += shortf_penalty();

	if (abs(prev_fitness - fitness) < 0.01)
	{
		damp = 1.2;
		//scale_social *= exp(0.1*norm_dist(generator));
		//scale_cog *= exp(0.1*norm_dist(generator));
		//cout << "Decreasing damp" << endl;
	}
	/*
	else if (abs(prev_fitness - fitness) > 20.0)
	{
		damp = 0.9;
		//scale_social *= exp(0.1*norm_dist(generator));
		//scale_cog *= exp(0.1*norm_dist(generator));
	}
	*/
	else
	{
		damp = 0.90;
		//scale_social *= exp(0.1*norm_dist(generator));
		//scale_cog *= exp(0.1*norm_dist(generator));
	}
	/*
	if (!needs_met(nutrients, data_ptr))
		fitness = DBL_MAX;
	*/
	update_cognitive();
}

double PSO::individual::get_fitness()
{
	return fitness;
}

void PSO::individual::update_cognitive()
{
	if (fitness < best_fitness)
	{
		best_fitness = fitness;
		best_position = position;
	}
}

void PSO::individual::show_attributes()
{
	if (needs_met)
		cout << "Needs met" << endl;
	else
		cout << "Needs NOT met" << endl;
	cout << "Fitness: " << fitness << endl;
	show_position();
}
