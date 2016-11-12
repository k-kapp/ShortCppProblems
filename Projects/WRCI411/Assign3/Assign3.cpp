#include "Assign3.h"
#include <functional>

using namespace std;

template<typename T>
double
sum_vec(vector<T> vec)
{
	T sum = 0;
	for (auto vec_iter = vec.begin(); vec_iter != vec.end(); advance(vec_iter, 1))
	{
		sum += *vec_iter;
	}
	return sum;
}

/*
    constructor for evolutionary algorithm.
        Parameters:
            dim: number of dimensions of the problem
            pop_size: population size (population of solutions)
            m_method: method by which mutation is done on each individual
            cross_t: type of crossover operator to apply to each individual
            dim_ranges: ranges of each dimension

        selected class variables:
            alloy1 to alloy4, and fitness: these vectors contain a vector for each
                                            individual in population. Each of these vectors
                                            in turn contain the value of each attribute at
                                            every iteration. This is only for studying the
                                            evolution of the solution over many iterations
*/
EvolAlgo::EvolAlgo(int dim, int pop_size, mutate_method m_method, cross_type cross_t,
                   vector<pair<double, double> > dim_ranges)
                    :  pop_size(pop_size), dim(dim), dim_ranges(dim_ranges)
{
	unif_real = uniform_real_distribution<double> (0.0, 1.0);
	unif_int_dim = uniform_int_distribution<int> (0, this->dim - 1);
	unif_int_pop = uniform_int_distribution<int> (0, this->pop_size - 1);
	norm_dist = normal_distribution<double> (0.0, 1.0);
	generator.seed(chrono::steady_clock::now().time_since_epoch().count());

	alloy1 = vector<vector<double> >(pop_size);
	alloy2 = vector<vector<double> >(pop_size);
	alloy3 = vector<vector<double> >(pop_size);
	alloy4 = vector<vector<double> >(pop_size);
	fitness = vector<vector<double> >(pop_size);

	count = 0;

	this->init_pop();
	update_prog_vecs();

	cout << "Pop size: " << this->pop_size << endl;

	iter_count = 0;

	switch (m_method)
	{
		case (RNORM):
			mutate_f = &EvolAlgo::mutate_rand_norm;
			break;
		case (PPULL):
			mutate_f = &EvolAlgo::mutate;
			break;
	}

	switch (cross_t.cross_m)
	{
		case (STD):
			cout << "std assigned" << endl;
			cross_f = &EvolAlgo::crossover;
			break;
		case (WEIGHT):
			cout << "weight assigned" << endl;
			cross_f = &EvolAlgo::weight_cross;
			break;
		case (NONE):
			cout << "no_cross assigned" << endl;
			cross_f = &EvolAlgo::no_cross;
			break;
	}
	num_choices = cross_t.num_choices;
	iter_count++;

}


/*
initialize population of solutions
*/
//TODO: initialise population uniformly
void
EvolAlgo::init_pop()
{
	for (int i = 0; i < this->pop_size; i++)
	{
		vector<double> pos;
		for(auto range_iter = this->dim_ranges.begin();
                range_iter != this->dim_ranges.end();
                advance(range_iter, 1))
        {
            pos.push_back(unif_real(generator)*(range_iter->second - range_iter->first)
                          + range_iter->first);
        }
		pop.push_back({pos, 0});
		evaluate(pop.back());
	}
	sort(pop.begin(), pop.end());
}

//TODO: Still need to finish evaluation function below (READ ASSIGNMENT BRIEF FOR DETAILS)

//TODO: state that iron discount was based on per kilogram (not pro rata, eg if 4.5 kilos
//      of platinum was bought, discount is given on 4 kilos)

/*
    get the number of iterations that have passed
*/
int
EvolAlgo::num_iter()
{
	return iter_count;
}

int
EvolAlgo::get_optim_count()
{
	return iter_count - 500;
}

/*
    Evaluate the solution represented by indiv parameter, then assign the value
        indiv's fitness
*/
void
EvolAlgo::evaluate(individual &indiv)
{
	vector<double> pos = indiv.position;

	double plat_kilos = pos[0]*0.2 + pos[1]*0.3 + pos[2]*0.8 + pos[3]*0.1;
	double iron_kilos = pos[0]*0.7 + pos[1]*0.2 + pos[2]*0.1 + pos[3]*0.5;
	double copper_kilos = pos[0]*0.1 + pos[1]*0.5 + pos[2]*0.1 + pos[3]*0.4;

	double base = pos[0]*3000 + pos[1]*3100 + pos[2]*5200 + pos[3]*2500
                    - exp(0.01*(25*pos[0] + 23*pos[1] + 35*pos[2] + 20*pos[3]))
                    - plat(plat_kilos) - iron(iron_kilos) - copper(copper_kilos);

	//(int) is taken here, check comment above this function
	double iron_discount = 300*((int)(plat_kilos));

	//make sure we get discount on the iron we needed to buy only
	//TODO: maybe remove this feature, and say the company made money on selling extra iron?
	if (iron_discount > 300*(pos[0]*0.7 + pos[1]*0.2 + pos[2]*0.1 + pos[3]*0.5))
		iron_discount = 300*(pos[0]*0.7 + pos[1]*0.2 + pos[2]*0.1 + pos[3]*0.5);

	double copper_discount = 0;

	if (copper_kilos > 8)
		copper_discount = 800*(copper_kilos)*0.1;

	base += (iron_discount + copper_discount);

	indiv.value = base;
}

/*
    same as evaluate function above, but here referenced by index of
        individual instead of reference to individual itself
*/
void
EvolAlgo::evaluate(int indiv_idx)
{
	evaluate(pop[indiv_idx]);
	cout << "Individual fitness: " << pop[indiv_idx].value << endl;
}

/*
    Calculate and return the cost of platinum based on the number of
        kilograms
*/
double
EvolAlgo::plat(double kilos)
{
	return 1200*kilos + 10*kilos*kilos;
}

/*
    Calculate and return the cost of iron based on the number of
        kilograms
*/
double
EvolAlgo::iron(double kilos)
{
	return 300*kilos;
}

/*
    Calculate and return the cost of copper based on the number of
        kilograms
*/
double
EvolAlgo::copper(double kilos)
{
	return 800*kilos;
}

/*
    calculate and return the total cost of all materials. the perc vector contains
        the percentage of all materials that each material makes up. kilos is the
        weight of all materials
*/
double
EvolAlgo::material_cost(vector<double> perc, double kilos)
{
	return plat(kilos*perc[0]) + iron(kilos*perc[1]) + copper(kilos*perc[2]);
}

/*
    return the time taken by the algorithm so far
*/
double
EvolAlgo::get_time()
{
	return time_taken;
}

/*
    run the genetic algorithm until no improvement in best solution occurs
        for 500 iterations
    TODO: create parameter here to specify how many iterations must pass with
        no improvement in solution for algorithm to end
*/
void
EvolAlgo::run(int no_change_stop, int max_iter)
{
	double best_val = -DBL_MAX;
	int no_change = 0;
	best_achieved = false;

	auto begin_time = chrono::duration_cast<chrono::milliseconds>
                            (chrono::steady_clock::now().time_since_epoch());

	while ((no_change < no_change_stop) && (iter_count < max_iter))
	{
		new_generation();
		if (this->pop.back().value != best_val)
			no_change = 0;
		else
			no_change++;
		best_val = this->pop.back().value;
		iter_count++;
		if (best_val > 95080.5 - 0.1 && best_val < 95080.5 + 0.1 && !best_achieved)
		{
			best_achieved = true;
		}

		update_prog_vecs();
	}


	auto end_time = chrono::duration_cast<chrono::milliseconds>
                        (chrono::steady_clock::now().time_since_epoch());

	time_taken = (double)((end_time - begin_time).count())/1000;

	cout << "optim_count = " << optim_count << endl;
	cout << "Stopped after " << iter_count << " iterations." << endl;
	cout << "Best value: " << best_val << endl;

	reverse_prog_vecs();
}

/*
    print information on best solution to screen
*/
void
EvolAlgo::show_best()
{
	cout << "Position: ";
	for (auto pos_iter = pop.back().position.begin();
            pos_iter != pop.back().position.end();
            advance(pos_iter, 1))
    {
        cout << *pos_iter << " ";
    }
	cout << endl;
	cout << "Fitness: ";
	cout << pop.back().value << endl;
}

/*
    print information on individual specified by indiv_iter to screen
*/
void
EvolAlgo::show_indiv(deque<individual>::iterator indiv_iter)
{
	for (auto pos_iter = indiv_iter->position.begin();
            pos_iter != indiv_iter->position.end();
            advance(pos_iter, 1))
	{
		cout << *pos_iter << " ";
	}
	cout << endl;
	cout << "Fitness: " << indiv_iter->value << endl;
}

/*
    print information on entire population of solutions to screen
*/
void
EvolAlgo::show_pop()
{
	for (auto indiv_iter = pop.begin();
            indiv_iter != pop.end();
            advance(indiv_iter, 1))
	{
		show_indiv(indiv_iter);
		cout << endl;
	}
}

void
EvolAlgo::update_prog_vecs()
{
	vector<vector<double> >::iterator prog_al1_iter, prog_al2_iter, prog_al3_iter,
                                        prog_al4_iter, prog_fit_iter;
	prog_al1_iter = alloy1.begin();
	prog_al2_iter = alloy2.begin();
	prog_al3_iter = alloy3.begin();
	prog_al4_iter = alloy4.begin();
	prog_fit_iter = fitness.begin();
	for (auto indiv_iter = pop.begin(); indiv_iter != pop.end();
            advance(indiv_iter, 1),
            advance(prog_al1_iter, 1),
            advance(prog_al2_iter, 1),
            advance(prog_al3_iter, 1),
            advance(prog_al4_iter, 1),
            advance(prog_fit_iter, 1))
	{
		prog_al1_iter->push_back(indiv_iter->position[0]);
		prog_al2_iter->push_back(indiv_iter->position[1]);
		prog_al3_iter->push_back(indiv_iter->position[2]);
		prog_al4_iter->push_back(indiv_iter->position[3]);
		prog_fit_iter->push_back(indiv_iter->value);
	}
}
/*
void
EvolAlgo::weight_cross()
{
	for (int i = 0; i < pop_size; i++)
	{
		individual offspring = weight_cross_sub();
		(this->*(mutate_f))(offspring);
		evaluate(offspring);
		insert_offspring(offspring);
	}
}
*/

void
EvolAlgo::new_generation()
{
	for (indiv_idx = 0; indiv_idx < pop.size(); indiv_idx++)
	{
		individual offspring;
		(this->*cross_f)(offspring);
		(this->*mutate_f)(offspring);
		evaluate(offspring);
		insert_offspring(offspring);
	}
}

void 
EvolAlgo::weight_cross(individual &new_indiv)
{
	vector<int> choices(num_choices);
	for (auto choice_iter = choices.begin();
            choice_iter != choices.end();
            advance(choice_iter, 1))
	{
	    //line below used to be: *choice_iter = unif_int_pop(generator) + pop_size;
	    //   any idea why???
		*choice_iter = unif_int_pop(generator) + 1;
	}
	double sum = sum_vec<int>(choices);
	new_indiv.position = vector<double>(dim, 0);
	for (auto choice_iter = choices.begin();
            choice_iter != choices.end();
            advance(choice_iter, 1))
	{
	    double weight = *choice_iter/sum;
		// choice index is divided by sum of all choice indexes. this increases
        //     weight of choices closer to more optimal values relative to less
        //     optimal values
		for (int i = 0; i < dim; i++)
		{
			new_indiv.position[i] += weight*(pop[*choice_iter - 1].position[i]);
		}
	}
}

void
EvolAlgo::crossover(individual &new_indiv)
{
	int rand_choice_weak = (int)(unif_real(generator)*(pop_size - 1));
	int rand_choice_strong = rand_choice_weak + (int)(unif_real(generator)*
		(pop_size - 1 - rand_choice_weak)) + 1;
	auto indiv_worse = next(pop.begin(), rand_choice_weak);
	auto indiv_better = next(pop.begin(), rand_choice_strong);
	new_indiv = (*indiv_better - *indiv_worse)*
                            unif_real(generator)*(4/(count + 1)) + *indiv_worse;
}

void
EvolAlgo::no_cross(individual &new_indiv)
{
	new_indiv = pop.at(indiv_idx);
}


void
EvolAlgo::mutate(individual &offspring)
{
	int pos_count = 0;
	for (auto pos_iter = offspring.position.begin();
            pos_iter != offspring.position.end();
            advance(pos_iter, 1))
	{
		if (unif_real(generator) < 0.3)
		{
			*pos_iter += 2*unif_real(generator)*
                            (pop.back().position[pos_count] - *pos_iter);
		}
		else
		{
			*pos_iter += 2*unif_real(generator)*
                            (*pos_iter - pop.front().position[pos_count]);
		}
        if (*pos_iter < 0)
            *pos_iter = 0;
		pos_count++;
	}
}


void
EvolAlgo::mutate_rand_norm(individual &offspring)
{
	for (auto pos_iter = offspring.position.begin();
            pos_iter != offspring.position.end();
            advance(pos_iter, 1))
	{
		*pos_iter += norm_dist(generator)*exp(-(double)count/1000.0)*3;
		*pos_iter = (*pos_iter < 0) ? 0 : *pos_iter;
	}
}

void
EvolAlgo::insert_offspring(individual &offspring)
{
	if (offspring < pop.front())
		return;

	int curr_idx = (int)((pop_size - 1)/2);
	int lower_idx = 0;
	int higher_idx = pop_size - 1;
	while (true)
	{
		if (offspring >= pop[curr_idx])
		{
			if (curr_idx == pop_size - 1)
			{
				pop.pop_front();
				pop.push_back(offspring);
				return;
			}
			if (offspring <= pop[curr_idx + 1])
			{
				pop.insert(next(pop.begin(), curr_idx + 1), offspring);
				pop.pop_front();
				return;
			}

			lower_idx = curr_idx;
			curr_idx = (int)((higher_idx + lower_idx)/2);
			if (curr_idx == lower_idx) curr_idx++;
		}
		else
		{
			if (offspring >= pop[curr_idx - 1])
			{
				pop.insert(next(pop.begin(), curr_idx), offspring);
				pop.pop_front();
				return;
			}

			higher_idx = curr_idx;
			curr_idx = (int)((higher_idx + lower_idx)/2);
			if (curr_idx == higher_idx) curr_idx--;
		}
	}
}

/*
	temporary fix for making picking the 1st, 2nd, etc best out
		of prog vecs
		TODO: let pop.front() be the best individual, and pop.back() be worst
*/
void
EvolAlgo::reverse_prog_vecs()
{
	reverse(fitness.begin(), fitness.end());
	reverse(alloy1.begin(), alloy1.end());
	reverse(alloy2.begin(), alloy2.end());
	reverse(alloy3.begin(), alloy3.end());
	reverse(alloy4.begin(), alloy4.end());
}

vector<vector<double> >
EvolAlgo::get_prog_vec(int type)
{
	assert(type > -1 && type < 5);
	switch (type)
	{
		case (0):
			return fitness;
		case (1):
			return alloy1;
		case (2):
			return alloy2;
		case (3):
			return alloy3;
		case (4):
			return alloy4;
	}
}
