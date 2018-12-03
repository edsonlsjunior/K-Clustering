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

KMeans::KMeans(int k, int maxInterations, vector <Object*> *objects) {
	this->k = k;
	this->maxInterations = maxInterations;
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

	
	
void KMeans::readSolution(ShortSolution *s) {
	this->solution = s;


}

void KMeans::buildClusters() {

	int i = 0;
	vector <Object*> ::iterator it;
	it = objects->begin();
	//solution->calculateSilhouette();
	ShortSolution *newSol = new ShortSolution(solution->getNumObjs(), solution->getNumClusters());
	solution->copySolution(newSol);
	// for each object find nearest centroid
	//Quadratic O(n²)
	//Aloca cada objeto apra um mean 
	for (; i < maxInterations; i++) {
		//test convergence
		if (i != 0) {
			while (it != objects->end()) {
				int mean = findNearestMean((*it));
				newSol->updateObjectCluster((*it)->getId(), mean);
				++it;
			}
		}
		newSol->updateAllClusters();
		//update each centroid for each object assign to it in previous step
		//Quadratic O(n²)

		for (int j = 0; j < k; j++) {
			double mediaX = 0;
			double mediaY = 0;
			int count = 0;
			double media=0;
			vector <Object*> ::iterator it;
			vector <int> objByCluster = newSol->getObjectByClusters();

			it = objects->begin();
			//0 and 1 are dimensions
			while (it != objects->end()) {
				
				if (objByCluster[(*it)->getId()-1 ] == j) {
					media += (*it)->getNormDoubleAttr(0);
				}
				++it;
			}
			mediaX = media / newSol->clusters[j].size();

			newSol->means->at(j).x = mediaX;

			media = 0;
			it = objects->begin();
			//0 and 1 are dimensions
			while (it != objects->end()) {
				if (objByCluster[(*it)->getId() - 1] == j) {
					media += (*it)->getNormDoubleAttr(1);
				}
				++it;
			}
			mediaY = media / newSol->clusters[j].size();

			
			
			newSol->means->at(j).y = mediaY; 

		}
	
	}
	newSol->calculateSilhouette();
	int count = 0;
	if (newSol->getSilhouette() > solution->getSilhouette() && newSol->checkViability()) {
		newSol->copySolution(solution);
		//cout << "K Means Melhorou Sol" << endl;
		count = 0;
		//cout << "KMeans:" << newSol->getSilhouette() << endl;

	}

	
}


int KMeans::findNearestMean(Object *obj)
{
	// O(n)
	int index=0;
	int count = 0;
	double minDist = numeric_limits<double>::max();
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
		//cout << "Obj " << (*it)->getId() << " Cluster:" << (*it)->clusterId << endl;
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
	//	s->addObject((*it)->getId(), (*it)->id ;
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
