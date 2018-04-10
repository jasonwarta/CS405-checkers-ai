// RandomGenerator.h

#ifndef RANDUTILS_H_INCLUDED
#define RANDUTILS_H_INCLUDED

#include <iostream>
#include <chrono>
#include <random>

class RandomGenerator
{
public:
	RandomGenerator(double mean, double stddev, double min, double max) :
		distribution_(mean, stddev),
		min_(min),
		max_(max)
	{};

	RandomGenerator(double min, double max) : 
		distribution_( (min+max)/2, (max-min)/6), 
		max_(max),
		min_(min)
	{};

	double operator()() {
		while (true) {
			double number = this->distribution_(generator_);
			if(number >= this->min_ && number <= this->max_)
				return number;
		}
	}

private:
	std::default_random_engine generator_;
	std::normal_distribution<double> distribution_;
	double min_;
	double max_;
};

#endif