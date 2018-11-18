
#include "../Headers/Constructive.h"
#include <list>


Constructive::Constructive(int numVertex, int numClusters, vector <Object*> objects, int type)
{
	solution = new ShortSolution(numVertex, numClusters);
	this->objects = objects;
	this->numObjs = numVertex;
	this->numClusters = numClusters;
	this->graph = objects;
}


void Constructive::Initialize() {
	//Inicializo os clusters
	//1
	solution = new ShortSolution(this->numObjs, numClusters);
	solution->setObjects(graph);
	vector <int> a;
	clusters.clear();
	clusters.assign(numClusters, a);
	for (size_t i = 0; i < clusters.size(); i++) {
		clusters[i].reserve(numObjs);
	}

	//2

	cluster n;
	structClusters.clear();
	for (int i = 0; i < numClusters; i++) {
		n.idCluster = 0;
		n.idClusterInSolution = i;
		structClusters.push_back(n);
	}

	//Inicializo os subconjuntos
	element newSet;
	elements.clear();
	for (int i = 1; i <= numObjs; ++i)
	{
		newSet.parent = i;
		newSet.rank = 0;
		elements.push_back(newSet);
	}

	objByCluster.clear();
	//Dizendo no vetor objByCluster, que cada no inicialmente é um cluster
	for (int i = 1; i <= numObjs; i++) {
		objByCluster.push_back(i);
	}

	edgesInSolution.clear();

	this->type = type;
	solution->setLargerEdges(largerEdges);
}

Constructive::~Constructive()
{
	delete solution;
}

void Constructive::buildGraph()
{

	/*for (int i = 0; i < numObjs; i++) {
	Node no = Node();
	no.setID(objects[i]->getId());
	no.setX(objects[i]->getNormDoubleAttr(0));
	no.setY(objects[i]->getNormDoubleAttr(1));
	graph.push_back(no);
	}*/

	for (int i = 0; i < numObjs; i++) {
		for (int j = 0; j < numObjs; j++) {
			if (i != j) {
				double distance = euclideanDistance(objects[i], objects[j]);
				graph[i]->addEdge(graph[i]->getId(), distance, graph[j]->getId());
			}
			else if (i > j) {
				double distance = euclideanDistance(objects[i], objects[j]);
				graph[i]->addEdge(graph[i]->getId(), distance, graph[j]->getId());
				objects[i]->addEdge(objects[i]->getId(), distance, objects[j]->getId());
			}

		}
	}

	// define a lista de candidatos
	for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
		vector <Edge> edge = (*it)->getEdges();
		for (vector <Edge>::iterator a = edge.begin(); a != edge.end(); a++) {
			candidatesEdges.push_back(*a);
		}
	}
	sortEdges();
	
	cout << "Grafo construido" << endl;




}


double Constructive::euclideanDistance(Object *a, Object *b)
{
	double dist = 0.0;
	int numAttr = 2;
	for (int i = 0; i<numAttr; i++)
		dist += pow((a->getNormDoubleAttr(i) - b->getNormDoubleAttr(i)), 2);

	return sqrt(dist);
}

void Constructive::sortEdges()
{
	sort(candidatesEdges.begin(), candidatesEdges.end());
	largerEdges = candidatesEdges;
	reverse(largerEdges.begin(), largerEdges.end());
}

vector<Edge> Constructive::getCandidatesEdges()
{
	return candidatesEdges;
}


void Constructive::randomMSTClustering()
{
	vector <int> clusterByObj;
	vector <int> clusterId;
	Initialize();
	solution->setEdges(candidatesEdges);
	srand(time(NULL));
	numConvexComponents = objects.size();
	int numVertex = objects.size();
	int numEdges = candidatesEdges.size();
	list <Edge> auxCandidatesEdges;
	int j = 0;

	clusterId.clear();
	clusterId.resize(numVertex + 1, -1);

	for (j = 0; j < numEdges; j++)
		auxCandidatesEdges.push_back(candidatesEdges[j]);

	int count = 0;
	while (numConvexComponents > numClusters) {
		j = rand() % (int)(auxCandidatesEdges.size() * rndParameter);
		list <Edge>::iterator it = (auxCandidatesEdges.begin());
		for (int i = 0; i < j; i++)
			it++;
		int parentSrc = find(it->getSrc());
		int parentDest = find(it->getDest());
		if (parentSrc != parentDest) {
			unionSETs(it->getSrc(), it->getDest());
			edgesInSolution.push_back(*it);
		}
		count++;
		auxCandidatesEdges.erase(it);
	}

	for (int i = 0; i < numObjs; i++) {
		objByCluster[i] = find(i + 1);
	}

	int clusterIndex = 0; // Contador para os ids dos objetos
	int obj = 0;

	for (vector <int>::iterator c = objByCluster.begin(); c != objByCluster.end(); c++, obj++) {

		if (clusterId[*c] == -1) {
			clusterId[*c] = clusterIndex;
			structClusters[clusterIndex].idCluster = clusterIndex;
			clusterIndex++;
		}


		solution->addObject(obj + 1, clusterId[*c]);
		clusterByObj.push_back(clusterId[*c]);
	}

	solution->setObjectByCluster(clusterByObj);

}

void Constructive::meansClustering()
{

	vector <int> clusterByObj;
	vector <int> clusterId;
	Initialize();
	solution->setEdges(candidatesEdges);




	//means.assign(numClusters, 0);
	int count = 0;
	while( count < numClusters ) {
		unsigned int id = rand() % numObjs;
		bool br = false;
		for (auto sortedId  : means) {
			if (objects[id - 1]->getId() == sortedId) {
				br = true;
			}
		}
		if (!br) {
			means.push_back(id);
			count++;
		}
	}
	
	for (auto obj : objects) {
		int clusterID = findNearestMean(obj->getId());
		objByCluster[obj->getId() - 1] = clusterID;
		clusters[clusterID].push_back(obj->getId());
		solution->addObject(obj->getId(), clusterID);
	}

	solution->setObjectByCluster(objByCluster);

	for (auto k : objByCluster) {
		cout << k << endl;
	}
	
}

int Constructive::findNearestMean(unsigned int id)
{
	// O(n)
	vector <Object> ::iterator mean;
	int index = 0;
	int count = 0;
	
	double minDist = euclideanDistance(objects[id-1], objects[means[0]]);
//	double minDist = objects[id - 1]->getDistance(means[0]);
	for (auto mean : means) {
		double dist = euclideanDistance(objects[id - 1], objects[mean]);
		if (minDist > dist) {
			minDist = dist;
			index = count;
		}
		count++;
	}
	return index;
}



vector<vector<int>> Constructive::getClusters()
{
	return clusters;
}

vector<int> Constructive::getObjByCluster()
{
	return objByCluster;
}

void Constructive::setRandomParameter(double alpha)
{
	this->rndParameter = alpha;
}

void Constructive::setCandidatesEdges(vector<Edge> edges)
{
	this->candidatesEdges = edges;
}

ShortSolution  *Constructive::getSolution()
{
	return solution;
}

void Constructive::unionSETs(int idX, int idY)
{


	int xroot = find(idX);
	int yroot = find(idY);
	numConvexComponents--;
	if (elements[xroot - 1].rank < elements[yroot - 1].rank) {
		elements[xroot - 1].parent = yroot;
		objects[idX - 1]->clusterId = yroot;

	}
	else if (elements[xroot - 1].rank > elements[yroot - 1].rank) {
		elements[yroot - 1].parent = xroot;
		objects[idY - 1]->clusterId = yroot;
	}
	else
	{
		elements[yroot - 1].parent = xroot;
		objects[idY - 1]->clusterId = xroot;
		elements[xroot - 1].rank++;

	}
}

int Constructive::find(int id)
{
	if (elements[id - 1].parent != id)
		elements[id - 1].parent = find(elements[id - 1].parent);

	return elements[id - 1].parent;
}