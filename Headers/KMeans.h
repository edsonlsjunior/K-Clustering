#pragma once
#include "Object.h";
#include <limits>
#include "ShortSolution.h"
#include <cstdlib> 
class KMeansCluster {
private:
	int idCluster;
	vector <Object*> objects;
public:
	void addObject(Object *obj);
	void removeObject(Object *obj);
	
};
class KMeans
{
public:
	KMeans(int k, int maxInterations, double maximaX,double maximaY, double minimaX, double minimaY, vector <Object*> *objects);
	double euclideanDistance(Object *a, Object *b);
	double euclideanDistance(double xa, double ya, double xb, double yb);
	void initializeMeans();
	void readSolution(ShortSolution * s);
	void buildClusters();
	int findNearestMean(Object *obj);
	void setmaximaX(double value);
	void setmaximaY(double value);
	void setminimaX(double value);
	void setminimaY(double value);
	double getNewCentroid(int mean,int dimAttr);
	void showInfo();
	ShortSolution * saveShortSolution();
	~KMeans();
private:
	int k; //num of clusters;
	int valuesTotal, objectsTotal, maxInterations;
	vector <int> clusters;
	vector <Object*> *objects;
	vector <Object> means;
	double maximaX,maximaY;
	double minimaX,minimaY;
	ShortSolution *solution;
};

