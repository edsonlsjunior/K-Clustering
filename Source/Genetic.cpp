#include "../Headers/Genetic.h"




Genetic::Genetic(vector<ShortSolution*>* sols, int maxIterations)
{
	solutions = sols;
	this->maxInterations = maxIterations;


}

Genetic::Genetic(vector<ShortSolution*>* sols, double  timeLimit)
{
	solutions = sols;
	this->timeLimit = timeLimit;

}


Genetic::~Genetic()
{
}
