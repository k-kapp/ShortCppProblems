#ifndef DIFFEVO_H
#define DIFFEVO_H

#include "misc.h"

#include <random>
#include <chrono>
#include <tuple>
#include <functional>
#include <algorithm>
#include <cfloat>
#include <cassert>
/*
vector<double> DiffEvo::individual::sum_vecs(vector<double> &summed, const vector< vector<double> > &positions)

vector<double> DiffEvo::individual::crossover_diffs(vector<pair<vector<double>, vector<double> diffs)
*/

vector<double> operator * (vector<double> &&lhs, const double &scale);

class DiffEvo
{
public:
	static default_random_engine generator;
	static normal_distribution<double> norm_dist;
	static uniform_real_distribution<double> unif_r_dist;
	

	DiffEvo(nutr_data &data, int pop_size, const vector< pair<double, double> > &ranges, unsigned int crossover_num, pair<double, double> best_rand, int dim = 77);
	void run(int stop_after);
	bool iterate();

	void show_pop();
	void show_best();

	vector<double> get_best_pos();
	vector<double> best_pos_weights();

	vector<double> fitness_historical;
private:

	class individual
	{
	public:
		individual();
		individual(const vector< pair<double, double> > &ranges, double cross_prob, nutr_data * data_ptr);

		vector<double> position;
		double fitness;

		vector<double> nutrients;
		bool needs_met;

		double cross_prob;

		void operator + (const vector<double> &rhs_pos);
		void operator += (const vector<double> &rhs_pos);

		nutr_data * data_ptr;

		void show_fitness();
		void show_position();
		void show_attributes();
		void evaluate();

		void init_pos(const int &dim);
		vector<double> sphere_surface(const int &dim);


		void crossover(const vector<double> &crossover_rhs);

		bool operator > (const individual &rhs);
		bool operator < (const individual &rhs);
		bool operator >= (const individual &rhs);
		bool operator <= (const individual &rhs);
		
		bool operator > (const individual &rhs) const;
		bool operator < (const individual &rhs) const;
		bool operator >= (const individual &rhs) const;
		bool operator <= (const individual &rhs) const;
	private:
		vector<double> crossover_rhs(vector< vector<double> > &bases, vector<double> &base_coeff, vector< pair< vector<double>, vector<double> > > &diffs);

		double shortf_penalty();
	};


private:
	void iteration();
	void init_pop(const vector<pair<double, double> > &ranges);	
	vector< vector<double> > random_posses(unsigned int num_pairs, vector<int> &&exclude_idx);
	vector<double> sum_vecs(vector<double> &summed, const vector< vector<double> > &positions);

	bool crossover_one(int indiv_idx);
	individual * get_best();

	deque<DiffEvo::individual> pop;
	deque<DiffEvo::individual> offspring;

	nutr_data data;
	int pop_size;
	int dim;
	uniform_int_distribution<int> unif_i_dist;
	
	unsigned int crossover_num_pairs;
	pair<double, double> best_rand;
};

bool in_vec(int num ,vector<int> vec);
void operator *= (vector<double> &lhs, const double &rhs);
void operator += (vector<double> &lhs,  const vector<double> &rhs);
vector<double> operator - (const vector<double> &fst, const vector<double> &snd);
vector<double> operator + (const vector<double> &fst, const vector<double> &snd);
static vector<double> crossover_bases(vector< vector<double> > &bases, const vector< double > &base_coeff)  __attribute__ ((unused));
static vector<double> crossover_diffs(const vector<pair<vector<double>, vector<double> > > &diffs) __attribute__ ((unused));
static vector<double> sum_vecs(vector<double> &summed, const vector< vector<double> > &positions) __attribute__ ((unused));
static vector<double> crossover_rhs(vector< vector<double> > &bases, const vector<double> &base_coeff, const vector< pair< vector<double>, vector<double> > > &diffs)
__attribute__ ((unused));

static vector<double> sum_vecs(vector<double> &summed, const vector< vector<double> > &positions)
{
	for (auto &&iter : positions)
	{
		summed += iter;
	}


	return summed;
}

static vector<double> crossover_diffs(const vector<pair<vector<double>, vector<double> > > &diffs)
{
	vector< vector<double> > summed_pairs(diffs.size(), vector<double>(77, 0));

	for (auto &&iter : zip_cs(summed_pairs, diffs))
	{
		iter.get<0>() = iter.get<1>().first - iter.get<1>().second;
		/*
		for (auto &&iter_nest : zip_cs(iter.get<0>(), iter.get<1>().first, iter.get<1>().second))
			iter_nest.get<0>() += iter_nest.get<1>() - iter_nest.get<2>();
		*/
	}
	
	vector<double> summed_diffs(77, 0);

	sum_vecs(summed_diffs, summed_pairs);

	return summed_diffs;

}

static vector<double> crossover_bases(vector< vector<double> > &bases, const vector< double > &base_coeff)
{
	vector<double> summed(77, 0);

	for (auto &&iter : zip_cs(bases, base_coeff))
	{
		iter.get<0>() *= iter.get<1>();
		summed += iter.get<0>();
	}

	return summed;
}

static vector<double> crossover_rhs(vector< vector<double> > &bases, const vector<double> &base_coeff, const vector< pair< vector<double>, vector<double> > > &diffs)
{
	return crossover_bases(bases, base_coeff) + crossover_diffs(diffs)*0.1;
}

#endif
