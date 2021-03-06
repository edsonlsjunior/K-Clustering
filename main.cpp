
#include <iostream>
#include "Headers/Input.h"
#include "Headers/KMeans.h"
#include "Headers/ShortSolution.h"
#include "Headers/Constructive.h"
#include "Headers/Genetic.h"
#include "Headers/Utils.h"
#include <time.h>
using namespace std;


int main(int   argc, char *argv[]) {


	clock_t tInicio, tFim, tDecorrido;
	const int numSolutions = 300;


	srand(13); //17, 19, 23, 29, 31, 37
	//srand(19);

	double alphas[10] = { 0.7,0.3,0.25 ,0.3 ,0.2 ,0.5 ,0.2 ,0.3 ,0.2 ,0.9 };


	int type; // 1  float , 2  Int

		/*200p3c1
		200p4c string strings[8] = {"C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/DS2-200p4c.csv" };
		200p4c1 string strings[8] = {"C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/DS2-200p4c1.csv" };
		300p3c	string strings[8] = {"C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/DS2-300p3c.csv" };
		300p3c1 string strings[8] = {"C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/DS2-300p3c1.csv" };
		400p3c  string strings[8] = {"C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/DS2-400p3c.csv" };
		400p4c1 string strings[8] = {"C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/DS2-400p4c1.csv" };
		500p3c  string strings[8] = {"C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/DS2-500p3c.csv" };
		500p4c1 string strings[8] = {"C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/DS2-500p4c1.csv" };
		500p6c1 string strings[8] = {"C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/DS2-500p6c1.csv" };
		
		*/

	string strings[8] = {"C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/instancias/DS2/DS2-500p6c1.csv" };
	
	

	int types[3] = { 2 };
	int Ks[8] = { 8 };

	//i->setInputPath("DS1-maronna.txt"); //type 1
	//i->setInputPath("DS2-300p4c12.txt"); //type 2
	int numInstances = 1;
	int numClusters;
	Input* i = Input::getInstance();
	for (int n = 0; n < numInstances; n++) {

		i->setInputPath(strings[n]);
		type = types[n];

		i->setType(type);
		i->readObjectInstances();
		i->normEntry();
		i->buildGraph();

		tInicio = clock();
		int numClusters = Ks[n];

		

		
		
		vector <ShortSolution*> sols;


		//Gera População Inicial
		for (int j = 0; j < numSolutions; j++) {
			ShortSolution *s;	
			Constructive *c = new Constructive(i->getNumObjects(), numClusters, i->getObjects(), type,13);
			int alfaSel = rand() % 10;
			c->setRandomParameter(alphas[alfaSel]);
			c->meansClustering();
			s = c->getSolution();
		
			//s->calculateSilhouette();
			//cout << "Constructive: " << s->getSilhouette() << endl;
			/*KMeans alg = KMeans(numClusters, 20, i->getMaxNormDoubleAttr(0), i->getMaxNormDoubleAttr(1), i->getMinNormDoubleAttr(0), i->getMinNormDoubleAttr(1), i->getObjects());
			alg.readSolution(s);
			cout << j << endl;
			alg.buildClusters();*/
			sols.push_back(s);
		}
		cout << "GENETIC --------------------------------" << endl;
		//system("cls");
		vector <ShortSolution*> *newPopulation = new vector <ShortSolution*>;

		Genetic *G = new Genetic( &sols , 5, newPopulation);
		//system("cls");
		double maxS = numeric_limits<double>::min();
		int count = 0;
		int index;
		cout << endl;
		cout << sols.at(0)->getSilhouette() << endl;
		cout << sols.at(0)->checkViability() << endl;
		cout << endl;
		cout << newPopulation->at(0)->getSilhouette() << endl;
		for (auto sol : sols) {
			if (sol->getSilhouette() > maxS) {
				index = count;
				maxS = sol->getSilhouette();
			}
			count++;
		}
		cout.precision(3);
		cout << "Max" <<  maxS << " " << sols.at(index)->getSilhouette() << endl;
		cout << newPopulation->at(index)->checkViability() << endl;
		tFim = clock();
		tDecorrido = ((tFim - tInicio) / (CLOCKS_PER_SEC));

		cout << "Tempo: " << tDecorrido << endl;
		
		//cout << s->getSilhouette() << endl;



		/*
		string tempPath = strings[n];
		tempPath.pop_back();
		tempPath.pop_back();
		tempPath.pop_back();
		tempPath.pop_back();
		tempPath += "Sol.txt";
		fstream a;
		a.open(tempPath, fstream::out);
		a << "Tempo Decorrido";
		a << endl;
		a << tDecorrido;
		a << endl;
		a << "S";
		a << endl;
		a << newPopulation->at(index)->getSilhouette();
		a.close();
		*/
		Utils u = Utils(strings[n]);
		u.ShowSolution(newPopulation->at(index));
		



	} 
	cout << endl;
	cout << "Fim da Aplicacao" << endl;



	return 0;
}
