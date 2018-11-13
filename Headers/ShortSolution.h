#pragma once
#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Object.h"
#include <limits>
#include "Node.h"
using namespace std;
class ShortSolution
{
public:
	ShortSolution(int numObj, int numClusters);
	void showSolution();
	~ShortSolution();

	void addObject(int objectId, int clusterId);

	int getId();

	void setId(int id);



	bool isObjectInCluster(int objId, int clusterId);

	int getNumClusters();

	int getNumObjs();

	vector <vector<int>> getClusters();
	void printGraph();

	void showObjectByCluster();

	void setObjectByCluster(vector <int> objectByCluster);

	vector <int> getObjectByClusters();


	void setFileName(string name);

	void setObjects(vector <Object*> objs);

	void setClusters(vector <vector<int>> clusters);

	void setLargerEdges(vector <Edge> largerEdges);
	

	vector <Object*> getObjects();

	vector <Edge> getEdges();

	void setEdges(vector <Edge> edges);

	void calculateCostClusters();

	void calculateIntraCosts();

	void newCalculateIntraCosts();

	void calculateExternalCosts();

	vector <double> getCostClusters();

	double getCost();

	void calculateSilhouette();

	double getSilhouette();

private:
	vector <int> objectByCluster; // for each object i have the clusters which it belongs

	vector <vector<int>> clusters; //for each cluster i have a subgroup of objects in it

	vector <double> intraCosts;

	vector <double> externalCosts;

	vector <double> costClusters;

	int id; // solution id


	int numObj;

	int numClusters;


	vector <Object*> objects;

	vector <Edge> edges;
	vector <Edge> largerEdges;

	string fileName;
	
	double sumCosts; //cost of solution

	double Silhouette;

};