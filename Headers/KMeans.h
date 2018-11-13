#pragma once
#include "Object.h";
#include "Solution.h"
#include "ShortSolution.h"
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
	KMeans(int k, int maxInterations, double maximaX,double maximaY, double minimaX, double minimaY, vector <Object*> objects);
	double euclideanDistance(Object *a, Object *b);
	void initializeMeans();
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
	vector <Object*> objects;
	vector <Object> means;
	double maximaX,maximaY;
	double minimaX,minimaY;
};

