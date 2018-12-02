#include "../Headers/Genetic.h"




vector <pair <int, int>> Genetic::matchMeansSolutions(ShortSolution * sol1, ShortSolution * sol2, vector <struct mean> *newMeans)
{
	vector <struct mean> *m1 = sol1->means;
	vector <unsigned int> aux;
	vector <pair <int, int>> matches;
	int count = 0;
	for (auto m : *m1) {
		int closestMean = findNearestMean( m.x , m.y, sol2->means);
		matches.push_back(make_pair(count, closestMean));
		count++;
	}

	return matches;

}

pair <struct mean, struct mean> Genetic::crossMeans(struct mean m1, struct mean m2) {
	//Calcular vetor entre m1 e m2, vamos andar 1/4 do tamanho de vetor na direção a outra mediana
	pair  <double, double> v ;
	pair <struct mean, struct mean> newMeans;
	v.first = m2.x - m1.x;
	v.second = m2.y - m1.y;
	struct mean newM1;
	newM1.x = m1.x += v.first;
	newM1.y = m1.y += v.second;
	newMeans.first = newM1;

	v.first = m1.x - m2.x;
	v.second = m1.y - m2.y;
	struct mean newM2;
	newM2.x = m2.x += v.first;
	newM2.y = m2.y += v.second;
	newMeans.second = newM2;

	return newMeans;

}

void Genetic::crossover(ShortSolution * sol1, ShortSolution * sol2, ShortSolution * newSol)
{
	/*The combining crossover combines the two solutions. It
builds the new offsprings centre by centre. For each centre
from the parent individual it finds the nearest centres from
the second parent and generates two new centres randomly
on the line joining the two parent centres.
	*/

	vector <struct mean> *newMeans = new  vector <struct mean> ;
	vector <struct mean> *m1 = sol1->means;
	vector <struct mean> *m2 = sol2->means;
	vector <pair <int, int>> matches = matchMeansSolutions(sol1, sol2, newMeans);
	for (auto match : matches ) {
		pair <struct mean, struct mean> pairMeans = crossMeans(sol1->means->at(matches.front), sol2->means->at(matches.back));
		
	}
	
}

void Genetic::mutation(ShortSolution * sol1)
{



}

Genetic::Genetic(vector<ShortSolution*>* sols, int maxIterations)
{
	int numOffspring = 20;
	solutions = sols;
	vector <ShortSolution*> *newPopulation = new vector <ShortSolution*>;
	this->maxInterations = maxIterations;
	int count = 0;
	int rand1, rand2;
	while (count < maxInterations) {

		for (int i = 0; i < numOffspring; i++) {

			ShortSolution *s = new ShortSolution();
			rand1 = rand() % solutions->size();
			rand2 = rand() % solutions->size();
			crossover(solutions->at(rand1), solutions->at(rand2), s);
			newPopulation->push_back(s);
			
		}

		for (auto p : *newPopulation) {

			//mutation(p);


			//refine(p);


		}

		for (auto p : *newPopulation) {

		//	p->calculateSilhouette();
		//	cout << p->getSilhouette() << endl;


		}





	}

	

}

Genetic::Genetic(vector<ShortSolution*>* sols, double  timeLimit)
{
	solutions = sols;
	this->timeLimit = timeLimit;

}


Genetic::~Genetic()
{
}


int Genetic::findNearestMean(double x, double y, vector <struct mean> *m2) {
	int index = 0;
	int count = 0;
	double minDist = numeric_limits<double>::max();
	for (auto mean : *m2) {
		double dist = euclideanDistance(x,y, mean.x , mean.y);
		if (minDist > dist) {
			minDist = dist;
			index = count;
		}
		count++;
	}
	return index;
}


double Genetic::euclideanDistance(double xa, double ya, double xb, double yb)
{
	double dist = 0.0;
	int numAttr = 2;
	dist += pow((xa - xb), 2);
	dist += pow((ya - yb), 2);
	return sqrt(dist);
}

void Genetic::refine(ShortSolution *newSol) {
	KMeans *alg = new KMeans(newSol->getNumClusters(), 5, newSol->getObjects());
	alg->readSolution(newSol);
	alg->buildClusters();

}

double Genetic::euclideanDistance(Object * a, Object * b)
{
	return 0.0;
}

int Genetic::findNearestMean(double x, double y, double x2, double y2)
{
	return 0;
}

double Genetic::getValueBestSolution()
{
	return 0.0;
}
