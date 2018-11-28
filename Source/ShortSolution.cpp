#include "../Headers/ShortSolution.h"



ShortSolution::ShortSolution(int numObj, int numClusters)
{
	this->numObj = numObj;
	this->numClusters = numClusters;
	clusters.reserve(numClusters);
	vector <int> a;
	clusters.assign(numClusters, a);
	costClusters.assign(numClusters, 0.0);
	intraCosts.assign(numClusters, 0.0);
	externalCosts.assign(numClusters, 0.0);



	for (size_t i = 0; i < clusters.size(); i++) {
		clusters[i].reserve(numObj);
	}



	objectByCluster.assign(numObj, 0);

}


void ShortSolution::showSolution()
{
	cout << "Para " << clusters.size() << " clusters: " << endl;
	for (int i = 0; i < clusters.size(); i++) {
		cout << "Cluster " << i + 1 << " Contem " << clusters[i].size() << " objetos" << endl;
		for (int j = 0; j < clusters[i].size(); j++) {
			cout << clusters[i][j] << " ";
		}
		cout << endl;
	}
}


ShortSolution::~ShortSolution()
{
	for (auto o : *objects) {
		delete o;
	}

	delete objects;


}

void ShortSolution::addObject(int objectId, int clusterId)
{

	clusters[clusterId].push_back(objectId);


}







bool ShortSolution::isObjectInCluster(int objId, int clusterId)
{
	bool check = false;

	if (objectByCluster[objId] == clusterId) {
		check = true;
	}


	return check;


}

int ShortSolution::getNumClusters()
{
	return numClusters;
}

int ShortSolution::getNumObjs()
{
	return numObj;
}



vector<vector<int>> ShortSolution::getClusters()
{
	return clusters;
}

void ShortSolution::printGraph()
{

	cout << "Para " << clusters.size() << " clusters: " << endl;
	for (size_t i = 0; i < clusters.size(); i++) {
		cout << "Cluster " << i + 1 << " Contem " << clusters[i].size() << " objetos" << endl;
		for (size_t j = 0; j < clusters[i].size(); j++) {
			cout << clusters[i][j] << " ";
		}
		cout << endl;
	}
}

void ShortSolution::showObjectByCluster() {
	for (vector <int>::iterator it = objectByCluster.begin(); it != objectByCluster.end(); it++) {
		cout << *it << endl;
	}
}

void ShortSolution::setObjectByCluster(vector<int> objectByCluster)
{
	this->objectByCluster = objectByCluster;
}

vector<int> ShortSolution::getObjectByClusters()
{
	return this->objectByCluster;
}

void ShortSolution::setFileName(string name)
{
	this->fileName = name;
}

void ShortSolution::setObjects(vector<Object*> *objs)
{


	this->objects = objs;
}

void ShortSolution::setClusters(vector<vector<int>> clusters)
{
	this->clusters = clusters;
}

void ShortSolution::setLargerEdges(vector<Edge> largerEdges)
{

	this->largerEdges = largerEdges;

}



vector<Object*> *ShortSolution::getObjects()
{
	return this->objects;
}

vector<Edge> ShortSolution::getEdges()
{
	return edges;
}

void ShortSolution::setEdges(vector<Edge> edges)
{
	this->edges = edges;
}

void ShortSolution::calculateCostClusters()
{


	newCalculateIntraCosts();

	calculateExternalCosts();

	sumCosts = 0.0;

	for (size_t i = 0; i < numClusters; i++) {
		costClusters[i] = intraCosts[i] + externalCosts[i];
		sumCosts += costClusters[i];
	}



	//cout << "costs calculated" << endl;

}

void ShortSolution::calculateIntraCosts()
{

/*
	//Para cada cluster i com k objetos
	for (int i = 0; i < clusters.size(); i++) {
		//Para cada objeto j Node cluster i
		double soma = 0;
		for (int j = 0; j < clusters[i].size(); j++) {
			//Somatorio das distancias do obj j aos outros k-1 objetos do cluster
			// soma += distancia obj j
			for (int k = j; k < clusters[i].size(); k++) {
				if (j != k) {
					//cout << "J: " << j << " K " << k << endl;
					double a = objects->at(j)->getDistance(k - 1);
					soma += a;
				}
			}
		}
		intraCosts[i] = soma;

		//cout << costClusters[i] << endl;
	}
	*/
}

void ShortSolution::newCalculateIntraCosts()
{
	intraCosts.assign(numClusters, 0);
	
	for (auto edge : largerEdges) {
		if (objectByCluster[edge.getSrc() - 1] == objectByCluster[edge.getDest() - 1]) {
			if (edge.getWeightEdge() > intraCosts[objectByCluster[edge.getSrc() - 1]] ) {
				intraCosts[objectByCluster[edge.getSrc() - 1]] = edge.getWeightEdge();
			}
		}
	}
	
	
	cout << endl;
	

}

void ShortSolution::calculateExternalCosts()
{


}

vector<double> ShortSolution::getCostClusters()
{
	return costClusters;
}

double ShortSolution::getCost() {
	return sumCosts;

}





void ShortSolution::calculateSilhouette() {

	vector <double> silhouettes;
	double max;



	for (int n = 1; n <= numObj; n++) {

		int clusterObj = objectByCluster[n - 1];


		//dissimilaridade1
		double a;
		if (clusters[clusterObj].size() == 1)
			a = 0;
		else {
			float m = ((float)1 / (float)clusters[clusterObj].size());

			double sumDists = 0.0;
			for (int i = 0; i < clusters[clusterObj].size(); i++) {
				sumDists = euclideanDistance(objects->at(clusters[clusterObj][i] - 1), objects->at(n-1)); 
			}
			a = sumDists * m;
			//cout << "dist" << sumDists << endl;
		}

		

		//dissimilaridade2
		double b;
		int closestCluster = 0;
		double minDist = std::numeric_limits<double>::max();
		for (int i = 1; i <= numObj; i++) {
			double d = 0;
			if (n != i) {
				d = euclideanDistance(objects->at(n-1), objects->at(i-1));
				if (d < minDist && objectByCluster[i - 1] != clusterObj) {
					minDist = d;
					closestCluster = objectByCluster[i - 1];
				}
			}
		}

		//cout << "b 1 " << endl;

		if (clusters[closestCluster].size() == 1)
			b = 0;
		else {
			double m = ((float)1 / (float)clusters[closestCluster].size());

			double sumDists = 0.0;
			
			
			for (int i = 0; i < clusters[closestCluster].size(); i++) {

				if (i != n) {
				
					
					sumDists = euclideanDistance(objects->at(clusters[closestCluster][i] - 1), objects->at(n - 1));
				}
	
			}
			
			
			b = sumDists * m;
		}
			//cout << "b 2 " << endl;


		//	cout << "b 3 " << endl;


		//cout << a << endl;
		//cout << b << endl;
		//cout << "checar se max(a,b)" << endl;
		if (a > b)
			max = a;
		else
			max = b;


		double s;
		if (max > 0 || max < 0) {
			s = (b - a) / (max);
		}
		else
			s = 0;

		//cout << "max" << endl;

		silhouettes.push_back(s);

	//	cout << silhouettes[n-1] << endl;
	}

	double sum = 0.0;

	for (vector <double>::iterator it = silhouettes.begin(); it != silhouettes.end(); it++) {
		sum += *it;
	}

	Silhouette = sum / numObj;

}

double ShortSolution::getSilhouette()
{
	return this->Silhouette;
}

double ShortSolution::euclideanDistance(Object *a, Object *b)
{
	double dist = 0.0;
	int numAttr = 2;
	for (int i = 0; i<numAttr; i++)
		dist += pow((a->getNormDoubleAttr(i) - b->getNormDoubleAttr(i)), 2);

	return sqrt(dist);
}