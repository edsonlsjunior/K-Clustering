#pragma once

#include "Object.h";

#include "ShortSolution.h"
#include <cstdlib> 
#include <limits>


class Genetic
{
private:
	void matchMeansSolutions(ShortSolution *sol1, ShortSolution *sol2);
	void crossover(ShortSolution *sol1, ShortSolution *sol2, ShortSolution *newSol);
	void mutation(ShortSolution *sol1);
	vector <ShortSolution*> *solutions;
	int maxInterations;
	int timeLimit;
public:
	Genetic(vector <ShortSolution*> *sols, int maxIterations);
	Genetic(vector<ShortSolution*>* sols, double timeLimit);
	~Genetic();


	
	double getValueBestSolution();
};

