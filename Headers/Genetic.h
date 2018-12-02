#pragma once

#include "Object.h";

#include "ShortSolution.h"
#include "KMeans.h"
#include <cstdlib> 
#include <limits>
#include <utility>


class Genetic
{
private:
	vector <pair <int, int>> matchMeansSolutions(ShortSolution *sol1, ShortSolution *sol2, vector <struct mean> *newMeans);
	pair<struct mean, struct mean> crossMeans(mean m1, mean m2);
	void crossover(ShortSolution * sol1, ShortSolution * sol2, ShortSolution * newSol, ShortSolution * newSol2);
	void mutation(ShortSolution *sol1);
	vector <ShortSolution*> *solutions;
	vector <ShortSolution*> *newSols;
	int maxInterations;
	int timeLimit;
public:
	Genetic(vector <ShortSolution*> *sols, int maxIterations);
	Genetic(vector<ShortSolution*>* sols, double timeLimit);
	~Genetic();

	int findNearestMean(double x, double y, vector<struct mean>* m2);

	double euclideanDistance(double xa, double ya, double xb, double yb);

	void refine(ShortSolution * newSol);

	double euclideanDistance(Object * a, Object * b);

	int findNearestMean(double x, double y, double x2, double y2);
	
	double getValueBestSolution();
};

