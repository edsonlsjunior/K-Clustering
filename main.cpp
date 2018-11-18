
#include <iostream>
#include "Headers/Input.h"
#include "Headers/KMeans.h"
#include "Headers/ShortSolution.h"
#include "Headers/Constructive.h"
#include "Headers/Utils.h"
#include <time.h>
using namespace std;


int main(int   argc, char *argv[]) {


	clock_t tInicio, tFim, tDecorrido;
	const int numSolutions = 1;
	srand(time(0));


	double alphas[10] = { 0.7,0.3,0.25 ,0.3 ,0.2 ,0.5 ,0.2 ,0.3 ,0.2 ,0.9 };


	int type; // 1  float , 2  Int


	string strings[8] = { "C:/Users/HAL/Documents/GitHub/GeneticClustering/Files/DS2-300p3c.csv" };

	//DS1/DS1-gauss9t.txt
	/*	string strings[8] = { "instancias/DS2/DS2-100p3c.csv","instancias/DS2/DS2-100p3c1.csv","instancias/DS2/DS2-100p5c1.csv","instancias/DS2/DS2-100p7c1.csv","instancias/DS2/DS2-100p7c1.csv","instancias/DS2/DS2-100p8c1.csv",
	"instancias/DS2/DS2-100p10c.csv" }; */


	int types[3] = { 2 };
	int Ks[8] = { 3 };

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

		tInicio = clock();
		int numClusters = Ks[n];

		Constructive c = Constructive(i->getNumObjects(), numClusters, i->getObjects(), type);
		c.buildGraph();
		
		ShortSolution *s;


		for (int i = 0; i < numSolutions; i++) {

		
			int alfaSel = rand() % 10;
			c.setRandomParameter(alphas[alfaSel]);
			c.meansClustering();
		}
		
		tFim = clock();
		tDecorrido = ((tFim - tInicio) / (CLOCKS_PER_SEC));

		cout << "Tempo: " << tDecorrido << endl;
		s = c.getSolution();
		s->calculateSilhouette();
		cout << s->getSilhouette() << endl;



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
		a << s->getSilhouette();
		a.close();
		Utils u = Utils(strings[n]);
		u.ShowSolution(s); 
		*/



	} 
	cout << endl;
	cout << "Fim da Aplicacao" << endl;



	return 0;
}
