#include "DiffEvo.h"

vector<double> operator * (vector<double> &&lhs, const double &scale)
{
	for (auto &val : lhs)
	{
		val *= scale;
	}
	return lhs;
}

bool DiffEvo::individual::operator > (const individual &rhs)
{
	return this->fitness > rhs.fitness;
}

bool DiffEvo::individual::operator < (const individual &rhs)
{
	return this->fitness < rhs.fitness;
}

bool DiffEvo::individual::operator >= (const individual &rhs)
{
	return *this > rhs || this->fitness == rhs.fitness;
}

bool DiffEvo::individual::operator <= (const individual &rhs)
{
	return *this < rhs || this->fitness == rhs.fitness;
}

bool DiffEvo::individual::operator > (const individual &rhs) const
{
	return this->fitness > rhs.fitness;
}

bool DiffEvo::individual::operator < (const individual &rhs) const
{
	return this->fitness < rhs.fitness;
}

bool DiffEvo::individual::operator >= (const individual &rhs) const
{
	return *this > rhs || this->fitness == rhs.fitness;
}

bool DiffEvo::individual::operator <= (const individual &rhs) const
{
	return *this < rhs || this->fitness == rhs.fitness;
}

DiffEvo::individual::individual(const vector< pair<double, double> > &ranges, double cross_prob, nutr_data * data_ptr) : cross_prob(cross_prob), data_ptr(data_ptr)
{	/*
	for (const pair<double, double> &r : ranges)
	{
		position.push_back(unif_r_dist(generator)*(r.second - r.first) + r.first);
	}
	*/
	init_pos(77);

	nutrients = vector<double> (9, 0);
	evaluate();
}

DiffEvo::individual::individual()
{}

vector<double> DiffEvo::individual::sphere_surface(const int &dim)
{
	vector<double> surface(dim);
	double sum = 0;

	for (auto &point : surface)
	{
		point = norm_dist(generator);
		sum += point*point;
	}

	sum = sqrt(sum);

	for (auto &point : surface)
	{
		point /= sum;
		point *= 50;
		if (point < 0)
			point = -point;
	}

	return surface;
}

void DiffEvo::individual::init_pos(const int &dim)
{
	position = sphere_surface(dim);

	for (auto &point : position)
	{
		point *= pow(unif_r_dist(generator), 1.0/77.0);
	}
}

void DiffEvo::individual::operator + (const vector<double> &rhs_pos)
{

	for (auto &&iter : zip_cs(this->position, rhs_pos))
	{
		iter.get<0>() += iter.get<1>();
	}

	/*
	for_each(boost::make_zip_iterator(boost::make_tuple(this->position.begin(), rhs_pos.begin())),
		boost::make_zip_iterator(boost::make_tuple(this->position.end(), rhs_pos.end())),
		[] (const boost::tuple<double &, const double &> &tup)
		{
			get<0>(tup) += get<1>(tup);
		}
	);
	*/
}

void DiffEvo::individual::operator += (const vector<double> &rhs_pos)
{
	for (auto &&iter : zip_cs(this->position, rhs_pos))
	{
		if (unif_r_dist(generator) < cross_prob)
			iter.get<0>() = iter.get<1>();
	}
	
	/*
	for_each(boost::make_zip_iterator(boost::make_tuple(this->position.begin(), rhs_pos.begin())),
		boost::make_zip_iterator(boost::make_tuple(this->position.end(), rhs_pos.end())),
		[this] (const boost::tuple<double &, const double &> &tup)
		{
			if (unif_r_dist(generator) < cross_prob)
				get<0>(tup) = get<1>(tup);
		}
	);
	*/
}


void DiffEvo::individual::show_fitness()
{
	cout << "Fitness: " << fitness << endl;
}

void DiffEvo::individual::show_position()
{
	cout << "Position: " << endl;

	for (const double &pos : position)
	{
		cout << pos << " ";
	}
	cout << endl;
}

void DiffEvo::individual::show_attributes()
{
	if (needs_met)
		cout << "Needs met" << endl;
	else
		cout << "Needs NOT met" << endl;
	show_fitness();
	show_position();
}



/*
vector<double> DiffEvo::individual::calc_base(vector< pair< double, vector<double> > > base_components)
{
	for (auto pair_iter& : base_components)
	{
*/

void DiffEvo::individual::crossover(const vector<double> &crossover_rhs_result)
{
	*this += crossover_rhs_result;
}

vector<double> DiffEvo::individual::crossover_rhs(vector< vector<double> > &bases, vector<double> &base_coeff, vector< pair< vector<double>, vector<double> > > &diffs)
{
	return crossover_bases(bases, base_coeff) + crossover_diffs(diffs)*0.5;
}

double DiffEvo::individual::shortf_penalty()
{
	double penalty = 0;
	needs_met = true;
	for (unsigned int i = 0; i < nutrients.size(); i++)
	{
		if (nutrients[i] < data_ptr->needs_values[i][0])
		{
			penalty += (data_ptr->needs_values[i][0] - nutrients[i])/data_ptr->needs_values[i][0]*20.0 + 1.0;
			needs_met = false;
		}
	}
	/*
	if (penalty == 0)
		needs_met = true;
	else
		needs_met = false;
	*/
	return penalty;
}
void DiffEvo::individual::evaluate()
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


	/*
	for_each(boost::make_zip_iterator(boost::make_tuple(summed_pairs.begin(), diffs.begin())),
		boost::make_zip_iterator(boost::make_tuple(summed_pairs.begin(), diffs.begin())),
		[] (const boost::tuple<vector<double> &, pair< vector<double>, vector<double> > &> &tup)
		{
			
			for_each(boost::make_zip_iterator(boost::make_tuple(get<0>(tup).begin(), get<1>(tup).first.begin(), get<1>(tup).second.begin())),
				boost::make_zip_iterator(boost::make_tuple(get<0>(tup).end(), get<1>(tup).first.end(), get<1>(tup).second.end())),
				[] (const boost::tuple<double &, double &, double &> &tup)
				{
					get<0>(tup) = get<1>(tup) - get<2>(tup);
				}
		}
		)
	)
	*/

	/*
	for_each(boost::make_zip_iterator(boost::make_tuple(summed_diffs.begin(), summed_pairs.begin())),
		boost::make_zip_iterator(boost::make_tuple(summed_diffs.end(), summed_pairs.end())),
		[] (const boost:tuple<double &, vector<double> &> &tup)
		{
			
			for (auto sum_pair& : get<1>(tup).begin())
			{
				get<0>(tup) += sum_pair;
			}
		}
	)
	*/
	
	//summation for base...put it in a different function?
	/*
	for_each(boost::make_zip_iterator(boost::make_tuple(summed_diffs.begin(), base.begin())),
		boost::make_zip_iterator(boost::make_tuple(summed_diffs.end(), base.end())),
		[] (const boost:tuple<double &, vector<double> &> &tup)
		{
			get<0>(tup) += get<1>(tup);
		}
	*/
