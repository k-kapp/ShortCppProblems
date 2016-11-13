#ifndef GA_H
#define GA_H

#include "misc.h"
#include <deque>
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
#include <cfloat>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>
using namespace std;

void init_static();

class GA
{


	class individual
	{
	public:
		individual();
		individual(const vector< pair<double, double> > &ranges, nutr_data * data_ptr, GA * parent_ptr);

		void show_fitness();
		void show_position();
		void show_attributes();
	
		void crossover(individual &offspring);
		void get_offspring(individual &offspring, const pair<int, int> &jump_mod);
		void mutate(const pair<int, int> &jump_mod);

		void evaluate();

		vector<double> position;
		double fitness;

		vector<double> nutrients;
		bool meets_needs;

		vector<double> mutate_stddev;
		double crossover_stddev;
		
		nutr_data * data_ptr;
		GA * parent_ptr;


		double shortf_penalty();
		vector<double> sphere_surface(const int &dim);
		void init_pos(const int &dim);
		
		bool operator < (const individual &rhs);
		bool operator > (const individual &rhs);
		bool operator <= (const individual &rhs);
		bool operator >= (const individual &rhs);
		bool operator == (const individual &rhs);
		bool operator < (const individual &rhs) const;
		bool operator > (const individual &rhs) const;
		bool operator <= (const individual &rhs) const;
		bool operator >= (const individual &rhs) const;
		bool operator == (const individual &rhs) const;
	};


public:
	GA(nutr_data &data, int pop_size, const vector< pair<double, double> > &ranges, int dim = 77);
	void run(int stop_after);

	void show_best();
	void show_best(individual * best);
	void new_generation(individual * best);
	void show_best_fitness();
	deque<GA::individual *> pop;
	nutr_data data;

	static bool ptr_comp(GA::individual * ptr1, GA::individual * ptr2);

	vector<double> change_better;
	double better_sum;

	static default_random_engine generator;
	static normal_distribution<double> norm_dist;
	static uniform_real_distribution<double> unif_r_dist;
	uniform_int_distribution<int> unif_i_dist;
	static cauchy_distribution<double> cauchy_dist;


	individual * get_best();
	vector<double> get_best_pos();
	vector<double> best_pos_weights();

	vector<double> fitness_historical;


private:
	void init_pop(const vector< pair<double, double> > &ranges);

	void offspring_vector();
	void insert_one(individual * offspring);
	void insert_offspring();
	void iteration();

	int pop_size, dim;
	int no_change;
	int iters;

	vector<individual *> offspring_vec;
};

#endif
