#pragma once

#include "Node.h";
#include "Object.h";
#include "ShortSolution.h"
#include <ctime>
#include <algorithm>

using namespace std;

class Constructive
{
public:
	Constructive(int numVertex, int numClusters, vector <Object*> objects, int type);
	void Initialize();
	~Constructive();

	void buildGraph();
	double euclideanDistance(Object * a, Object * b);

	void sortEdges();

	vector <Edge> getCandidatesEdges();

	void buildClusters();
	void buildClustersRandom();
	void buildClustersRandom2();

	vector <Node> getGraph();
	vector <vector<int>> getClusters();
	vector <int> getObjByCluster();

	void setRandomParameter(double alpha);


	void setCandidatesEdges(vector <Edge> edges);

	ShortSolution * getSolution();



private:
	

	vector <Object*> objects;

	vector <double> clustersCosts;
	vector <double> finalCosts;
	

	int numObjs;
	int numClusters;

	double rndParameter;
	int numConvexComponents;

	vector <Edge> candidatesEdges;
	vector <Edge> largerEdges;

	vector <Edge> edgesInSolution;

	vector <struct element> elements;

	vector <struct cluster> structClusters;

	vector <vector<int>> clusters;

	vector <int> objByCluster;

	vector <int> parents;

	ShortSolution *solution;

	vector <Object*> graph;

	int type;

	void unionSETs(int idX, int idY);
	int find(int id);


};

struct element {
	int parent;
	int rank;
};

struct cluster {
	vector <int> objsId;
	int idCluster;
	int idClusterInSolution;
};