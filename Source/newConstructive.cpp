
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

void Constructive::buildClusters()
{
	vector <int> clusterByObj;
	Initialize();
	solution->setEdges(candidatesEdges);
	numConvexComponents = objects.size();
	int numVertex = objects.size();
	int numEdges = candidatesEdges.size();
	vector <Edge> auxCandidatesEdges = candidatesEdges;
	int j = 0;



	int count = 0;
	while (numConvexComponents > numClusters) {
		int parentSrc = find(auxCandidatesEdges[j].getSrc());
		int parentDest = find(auxCandidatesEdges[j].getDest());
		if (parentSrc != parentDest) {
			unionSETs(auxCandidatesEdges[j].getSrc(), auxCandidatesEdges[j].getDest());
			edgesInSolution.push_back(auxCandidatesEdges[j]);
		}
		count++;
		j++;
	}





	for (int i = 0; i < numObjs; i++) {
		objByCluster[i] = find(i + 1);
	}

	int clusterIndex = 0; // Contador para os ids dos objetos
	int conta = 0;

	for (vector <int>::iterator c = objByCluster.begin(); c != objByCluster.end(); c++) {
		//cout << *c << endl;
		vector <int>::iterator b;
		for (b = objByCluster.begin(); b != c; b++) {
			if (*c == *b)
				break;
		}
		if (c == b) {
			//cout << clusterIndex << endl;
			structClusters[clusterIndex].idCluster = *c;
			clusterIndex++;
		}
	}

	//Adciona os objetos na solução
	count = 0;
	//cout << "Cluster Index: " << clusterIndex << endl;
	for (vector <int>::iterator it = objByCluster.begin(); it != objByCluster.end(); it++) {
		for (vector <struct cluster>::iterator c = structClusters.begin(); c != structClusters.end(); c++) {
			if (c->idCluster == *it) {
				solution->addObject(count + 1, c->idClusterInSolution);
				clusterByObj.push_back(c->idClusterInSolution);
			}
		}
		count++;
	}

	solution->setObjectByCluster(clusterByObj);

}

void Constructive::buildClustersRandom()
{
	vector <int> clusterByObj;
	Initialize();

	solution->setEdges(candidatesEdges);

	srand(time(NULL));
	numConvexComponents = objects.size();
	int numVertex = objects.size();
	int numEdges = candidatesEdges.size();
	vector <Edge> auxCandidatesEdges = candidatesEdges;
	int j = 0;
	edgesInSolution.reserve(auxCandidatesEdges.size());


	double sumDist = 0;
	int last = auxCandidatesEdges.size() * rndParameter;
	for (; j <= last; j++) {
		sumDist += (1 - auxCandidatesEdges[j].getWeightEdge());
	}

	while (numConvexComponents > numClusters) {
		//j = ceil((rand() % last));

		int k = ceil((rand() % (int)(sumDist * 1000)));
		int sum = 0;
		//cout << "k=" << k << endl << "sumDist=" << sumDist * 1000 << endl;
		for (j = 0; sum + (1 - auxCandidatesEdges[j].getWeightEdge()) * 1000 < k && j < last; j++) {
			sum += (1 - auxCandidatesEdges[j].getWeightEdge()) * 1000;
			/*cout  << "j=" << j << "\tw=" << auxCandidatesEdges[j].getWeightEdge() <<
			"\t1-w=" << 1-auxCandidatesEdges[j].getWeightEdge() <<
			"\tparc=" << sum + (1-auxCandidatesEdges[j].getWeightEdge())*1000 << "\tk=" << k << endl << endl;*/
		}

		int parentX = find(auxCandidatesEdges[j].getSrc());
		int parentY = find(auxCandidatesEdges[j].getDest());
		if (parentX != parentY) { //Se os "pais" deles forem os mesmos significa que h� um circulo
			unionSETs(auxCandidatesEdges[j].getSrc(), auxCandidatesEdges[j].getDest());
			edgesInSolution.push_back(auxCandidatesEdges[j]);
		}



		sumDist -= (1 - auxCandidatesEdges[j].getWeightEdge());
		auxCandidatesEdges.erase(auxCandidatesEdges.begin() + j);
		if (last != auxCandidatesEdges.size() * rndParameter)
			last = auxCandidatesEdges.size() * rndParameter;
		else
			sumDist += (1 - auxCandidatesEdges[last].getWeightEdge());
	}

	//cout << "Componentex Conexas Prontas" << endl;

	int count = 0;;
	for (int i = 0; i < numObjs; i++) {
		objByCluster[i] = find(i + 1);
	}



	int clusterIndex = 0; // Contador para os ids dos objetos


	for (vector <int>::iterator c = objByCluster.begin(); c != objByCluster.end(); c++) {
		//cout << *c << endl;
		vector <int>::iterator b;
		for (b = objByCluster.begin(); b != c; b++) {
			if (*c == *b)
				break;
		}
		if (c == b) {
			//cout << clusterIndex << endl;
			structClusters[clusterIndex].idCluster = *c;
			clusterIndex++;
		}
	}

	//cout << "Componentex Conexas Prontas" << endl;
	//Adciona os objetos na solução

	for (vector <int>::iterator it = objByCluster.begin(); it != objByCluster.end(); it++) {
		for (vector <struct cluster>::iterator c = structClusters.begin(); c != structClusters.end(); c++) {
			if (c->idCluster == *it) {
				solution->addObject(count + 1, c->idClusterInSolution);
				clusterByObj.push_back(c->idClusterInSolution);
			}
		}
		count++;
	}

	solution->setObjectByCluster(clusterByObj);
}

void Constructive::buildClustersRandom2()
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
	/*
	////cout << *c << endl;
	//vector <int>::iterator b;
	//for (b = objByCluster.begin(); b != c; b++) {
	//	if (*c == *b)
	//		break;
	//}
	//if (c == b) {
	//	//cout << clusterIndex << endl;
	//	structClusters[clusterIndex].idCluster = *c;
	//	clusterIndex++;
	//}
	}

	//Adciona os objetos na solução
	count = 0;
	//cout << "Cluster Index: " << clusterIndex << endl;
	for (vector <int>::iterator it = objByCluster.begin(); it != objByCluster.end(); it++) {
	for (vector <struct cluster>::iterator c = structClusters.begin(); c != structClusters.end(); c++) {
	if (c->idCluster == *it) {
	solution->addObject(count + 1, c->idClusterInSolution);
	clusterByObj.push_back(c->idClusterInSolution);

	}
	}
	count++;
	}

	solution->setObjectByCluster(clusterByObj);
	/**/
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