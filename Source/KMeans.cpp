#include "../Headers/KMeans.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <algorithm>


KMeans::KMeans(int k,int maxInterations,double maximaX,double maximaY,double minimaX,double minimaY, vector <Object*> *objects)
{
	this->k = k;
	this->maxInterations = maxInterations;
	this->maximaX = maximaX;
	this->maximaY = maximaY;
	this->minimaX = minimaX;
	this->minimaY = minimaY;
	this->objects = objects;
}

double KMeans::euclideanDistance(Object *a, Object *b)
{
	double dist=0.0;
	for (int i = 0; i < 2; i++) {

		dist += pow((a->getNormDoubleAttr(i) - b->getNormDoubleAttr(i)),2);
	}
	return sqrt(dist);
}

double KMeans::euclideanDistance(double xa, double ya, double xb, double yb)
{
	double dist = 0.0;
	int numAttr = 2;
	dist += pow((xa - xb), 2);
	dist += pow((ya - yb), 2);
	return sqrt(dist);
}

void KMeans::initializeMeans()
{
	//place centroid at random locations
	std::mt19937 mt(42);
	for (int i = 0; i < k; i++) {

		means.push_back(i);
		//uniform_real_distribution<float> linear_x(maximaX, minimaX);
		//uniform_real_distribution<float> linear_y(maximaY, minimaY);
		uniform_real_distribution<float> linear_x(0, 1);
		uniform_real_distribution<float> linear_y(0, 1);
		means[i].addNewDoubleOrigAttr(linear_x(mt));
		means[i].addNewDoubleOrigAttr(linear_y(mt));
		means[i].addNewDoubleNormAttr(linear_x(mt));
		means[i].addNewDoubleNormAttr(linear_y(mt));
	}
	int i = 0;
	vector <Object*> ::iterator it;
	it = objects->begin();
	// for each object find nearest centroid
	//Quadratic O(n²)

	for (; i < maxInterations; i++) {
		//test convergence
		while (it != objects->end()) {
			int mean = findNearestMean((*it));
			//clusters[mean].addObject((*it));
			(*it)->clusterId = mean;
			++it;
		}
		//update each centroid for each object assign to it in previous step
		//Quadratic O(n²)
	
		for (int j = 0; j < k; j++) {
			double mediaX = 0;
			double mediaY = 0;
			//0 and 1 are dimensions
			mediaX = getNewCentroid(j,0);
			mediaY = getNewCentroid(j,1);
			means[j].setNormDoubleAttr(mediaX, 0);
			means[j].setNormDoubleAttr(mediaX, 1);
		}
		
	}
}
	
	
	
void KMeans::readSolution(ShortSolution *s) {
	this->solution = s;


}

void KMeans::buildClusters() {

	int i = 0;
	vector <Object*> ::iterator it;
	it = objects->begin();
	ShortSolution *newSol = new ShortSolution(solution->getNumObjs(), solution->getNumClusters());
	newSol->means = solution->means;
	// for each object find nearest centroid
	//Quadratic O(n²)
	//Aloca cada objeto apra um mean 
	for (; i < maxInterations; i++) {
		//test convergence
		while (it != objects->end()) {
			int mean = findNearestMean((*it));
			//newSol.addObject((*it)->getId(), mean);
			solution->addObject((*it)->getId(), mean);
			++it;
		}

		//update each centroid for each object assign to it in previous step
		//Quadratic O(n²)

		for (int j = 0; j < k; j++) {
			double mediaX = 0;
			double mediaY = 0;
			//0 and 1 are dimensions
			mediaX = getNewCentroid(j, 0);
			mediaY = getNewCentroid(j, 1);
			newSol.means->at(j).x = mediaX;
			
			newSol.means->at(j).y = mediaY;
			//means[j].setNormDoubleAttr(mediaX, 0);
			//means[j].setNormDoubleAttr(mediaX, 1);
		}

	}
	newSol.calculateSilhouette();
	solution->calculateSilhouette();
	if (newSol.getSilhouette() > solution->getSilhouette()) {
		solution = &newSol;
		cout << "trocou" << endl;
	}
		
	cout << "KMeans:" << newSol.getSilhouette() << endl;
}


int KMeans::findNearestMean(Object *obj)
{
	// O(n)
	vector <Object> ::iterator mean;
	int index=0;
	int count = 0;
	double minDist = numeric_limits<double>::min();
	for (auto m : *solution->means) {
		double dist = euclideanDistance(obj->getNormDoubleAttr(0),obj->getNormDoubleAttr(1),m.x,m.y);
		if (minDist > dist) {
			minDist = dist;
			index = count;
		}
		count++;
	}
	return index;
}


KMeans::~KMeans()
{
	/*for (auto o : *objects) {
		delete o;
	}
	delete objects;*/
}

void KMeans::setmaximaX(double value)
{
	this->maximaX = value;
}

void KMeans::setmaximaY(double value)
{
	this->maximaY = value;
}

void KMeans::setminimaX(double value)
{
	this->minimaX = value;
}

void KMeans::setminimaY(double value)
{
	this->minimaY = value;
}

double KMeans::getNewCentroid(int mean,int dimAttr)
{
	double media = 0;
	vector <Object*> ::iterator it;
	it = objects->begin();
	while (it != objects->end()) {
		if ((*it)->clusterId == mean) {
			media += (*it)->getNormDoubleAttr(dimAttr);
		}
		++it;
	}
	return media / objects->size();
}

void KMeans::showInfo()
{
	vector <Object*> ::iterator it;
	it = objects->begin();
	while (it != objects->end()) {
		cout << "Obj " << (*it)->getId() << " Cluster:" << (*it)->clusterId << endl;
		++it;
	}

}

ShortSolution * KMeans::saveShortSolution()
{
	ShortSolution *s = new ShortSolution(objects->size(),k);
	vector <Object*> ::iterator it;
	it = objects->begin();
	int i = 0;
	while (it != objects->end()) {
		s->addObject((*it)->getId(), (*it)->clusterId);
		++it;
		i++;
	}
	return s;

}


void KMeansCluster::addObject(Object * obj)
{

	objects.push_back(obj);
}

void KMeansCluster::removeObject(Object * obj)
{
	objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}
