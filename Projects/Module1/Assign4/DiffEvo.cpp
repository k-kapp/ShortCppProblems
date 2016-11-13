#include "DiffEvo.h"

bool in_vec(int num ,vector<int> vec)
{
	for (auto &iter : vec)
	{
		if (num == iter)
		{
			return true;
		}
	}
	return false;
}

vector<double> operator + (const vector<double> &fst, const vector<double> &snd)
{
	vector<double> result(fst.size());

	for (auto &&iter : zip_cs(result, fst, snd))
	{
		iter.get<0>() = iter.get<1>() + iter.get<2>();
	}
	/*
	for_each(boost::make_zip_iterator(boost::make_tuple(result.begin(), fst.begin(), snd.begin())),
		boost::make_zip_iterator(boost::make_tuple(result.end(), fst.end(), snd.end())),
		[] (const boost::tuple<double &, const double &, const double &> &tup)
		{
			get<0>(tup) = get<1>(tup) + get<2>(tup);
		}
	);
	*/

	return result;
}

vector<double> operator - (const vector<double> &fst, const vector<double> &snd)
{
	vector<double> result(fst.size());

	for (auto &&iter : zip_cs(result, fst, snd))
	{
		iter.get<0>() = iter.get<1>() - iter.get<2>();
	}

	/*
	for_each(boost::make_zip_iterator(boost::make_tuple(result.begin(), fst.begin(), snd.begin())),
		boost::make_zip_iterator(boost::make_tuple(result.end(), fst.end(), snd.end())),
		[] (const boost::tuple<double &, const double &, const double &> &tup)
		{
			get<0>(tup) = get<1>(tup) - get<2>(tup);
		}
	);
	*/


	return result;
}

	

//TODO: test operator below

void operator += (vector<double> &lhs,  const vector<double> &rhs)
{

	for (auto &&iter : zip_cs(lhs, rhs))
	{
		iter.get<0>() += iter.get<1>();
	}

	/*

	for_each(boost::make_zip_iterator(boost::make_tuple(lhs.begin(), rhs.begin())),
		boost::make_zip_iterator(boost::make_tuple(lhs.end(), rhs.end())),
		[] (const boost::tuple<double &, const double &> &tup)
		{
			get<0>(tup) += get<1>(tup);
		}
	);

	*/
}


void operator *= (vector<double> &lhs, const double &rhs)
{
	for (auto &iter : lhs)
	{
		iter *= rhs;
	}
	/*
	for_each(boost::make_zip_iterator(boost::make_tuple(lhs.begin(), rhs.begin())),
		boost::make_zip_iterator(boost::make_tuple(lhs.end(), rhs.end())),
		[] (const boost::tuple<double &, const double &> &tup)
		{
			get<0>(tup) *= get<1>(tup);
		}
	);
	*/
}

default_random_engine DiffEvo::generator = default_random_engine
(
	chrono::steady_clock::now().time_since_epoch().count()
);
normal_distribution<double> DiffEvo::norm_dist = normal_distribution<double>(0.0, 1.0);
uniform_real_distribution<double> DiffEvo::unif_r_dist = uniform_real_distribution<double>(0.0, 1.0);

DiffEvo::DiffEvo(nutr_data &data, int pop_size, const vector< pair<double, double> > &ranges, unsigned int crossover_num_pairs, pair<double, double> best_rand, int dim) : 
data(data), pop_size(pop_size), dim(dim), crossover_num_pairs(crossover_num_pairs), best_rand(best_rand)
{
	init_pop(ranges);
	unif_i_dist = uniform_int_distribution<int> (0, pop_size - 1);
}

void DiffEvo::init_pop(const vector< pair<double, double> > &ranges)
{
	for (int i = 0; i < pop_size; i++)
	{
		pop.push_back(individual(ranges, 0.5, &data));
	}
	//sort(pop.begin(), pop.end(), greater<individual>());
}

void DiffEvo::show_pop()
{
	for (individual &indiv : pop)
	{
		indiv.show_attributes();
	}
}

bool DiffEvo::iterate()
{
	bool changed = false;
	for (int i = 0; i < pop_size; i++)
	{
		if(crossover_one(i))
		{
			changed = true;
		}
	}
	return changed;
}

void DiffEvo::run(int stop_after)
{
	
	//iterate();

	int count = 0;
	int no_change = 0;

	while (no_change < stop_after)
	{
		fitness_historical.push_back(get_best()->fitness);
		if (iterate())
		{
			no_change = 0;
		}
		else
		{
			no_change++;
		}
		count++;
		//best_rand.second *= exp(-(double)count*0.00000001);
		//best_rand.first = 1 - best_rand.second;
		if (count % 100 == 0)
			get_best()->show_attributes();


		if (count > 350)
			break;
	}
	
}

vector< vector<double> > DiffEvo::random_posses(unsigned int num_pairs, vector<int> &&exclude_idx)
{

	vector< vector<double> > posses(num_pairs*2);
	vector<int> pos_idx;
	int idx;

	while (pos_idx.size() < num_pairs*2)
	{
		idx = unif_i_dist(generator);
		assert(idx < pop_size);
		if (!in_vec(idx, pos_idx) && !in_vec(idx, exclude_idx))
		{
			pos_idx.push_back(idx);
			posses[pos_idx.size() - 1] = pop[idx].position;
		}
		else
		{
		}
	}

	return posses;	
}



/*
vector<double> DiffEvo::individual::calc_base(vector< pair< double, vector<double> > > base_components)
{
	for (auto pair_iter& : base_components)
	{
*/



vector< pair<vector<double>, vector<double> > > make_pairs(vector< vector<double> > vecs)
{
	vector< pair< vector<double>, vector<double> > > pairs(vecs.size()/2);

	auto pairs_iter = pairs.begin();
	auto vecs_iter = vecs.begin();

	for ( ; pairs_iter != pairs.end(); advance(pairs_iter, 1), advance(vecs_iter, 2))
	{
		*pairs_iter = {*vecs_iter, *next(vecs_iter, 1)};
	}

	return pairs;

}

DiffEvo::individual * DiffEvo::get_best()
{
	DiffEvo::individual * best = &pop.front();
	for (auto &indiv : pop)
	{
		if (indiv < *best)
			best = &indiv;
	}
	return best;
}

void DiffEvo::show_best()
{
	get_best()->show_attributes();
}

vector<double> DiffEvo::get_best_pos()
{
	return get_best()->position;
}

vector<double> DiffEvo::best_pos_weights()
{
	vector<double> pos = get_best()->position;

	for (int i = 0; i < 77; i++)
	{
		cout << "weight: " << data.price_values[i][0] << endl;
		pos[i] *= data.price_values[i][0];
	}
	
	return pos;
}

bool DiffEvo::crossover_one(int indiv_idx)
{
	vector< vector<double> > rand_posses = random_posses(crossover_num_pairs, {indiv_idx});
	vector< pair< vector<double>, vector<double> > > pairs = make_pairs(rand_posses);
	vector< vector<double> > bases = random_posses(1, {indiv_idx});
	bases[0] = get_best()->position;
	/*
	vector<double> diffs_summed = crossover_diffs(pairs);
	vector<double> bases_summed = crossover_bases(bases, {best_rand.first, best_rand.second});
	*/
	vector<double> rhs_crossover = crossover_rhs(bases, {best_rand.first, best_rand.second}, pairs);
	individual offspring = pop[indiv_idx];
	offspring += rhs_crossover;
	//pop[indiv_idx].show_attributes();
	//cout << "offspring fitness before evaluation: " << offspring.fitness << endl << endl;
	offspring.evaluate();
	//offspring.show_attributes();
	//cout << "offspring fitness after evaluation: " << offspring.fitness << endl << endl;

	if (offspring < pop[indiv_idx])
	{
		pop[indiv_idx] = offspring;
		return true;
	}
	else
	{
		return false;
	}

}
/*
void GA::insert_one(individual &offspring)
{
	if (offspring > pop.front())
		return;

	bool inserted = false;

	for (auto pop_iter = next(pop.begin(), 1); pop_iter != pop.end(); advance(pop_iter, 1))
	{
		if (offspring >= *pop_iter)
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
	pop.pop_front();
}

void GA::insert_offspring()
{
	for (individual &indiv : offspring_vec)
	{
		insert_one(indiv);
	}
}
*/
