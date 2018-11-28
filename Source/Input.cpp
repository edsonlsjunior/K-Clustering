#include "../Headers/Input.h"
#include <fstream>
#include <iostream>


using namespace std;

Input::Input()
{

	

}


Input::~Input()
{
}

Input* Input::instance = 0;

Input * Input::getInstance()
{
	if (instance == 0)
	{
		instance = new Input();
	}

	return instance;
}

void Input::addObject(Object *object)
{
	this->objects->push_back(object);
}



void Input::readObjectInstances()
{

	string file = inputPath;
	int m = 0;
	int n = 0;
	char comma;
	ifstream f;
	int index;
	double x, y;
	int ix, iy;
	vector <Object*> *objects = new vector <Object*>;
	vector <Object> *test = new vector <Object>;
	vector <double> attrs;
	int i = 0;
	f.open(file);
	if (f.is_open()) {
		f >> m;
		f >> comma;
		f >> n;
		for (int j = 0; j < m; j++)
			test->push_back(Object(j + 1));
		for (int j = 0; j < m; j++)
			objects->push_back(new Object(j + 1));
		
		while (i < m) {
			f >> index;
			f >> comma;
			if (type == 1) {
				f >> x;
				f >> comma;
				f >> y;
				test->at(index - 1).addNewDoubleOrigAttr(x);
				test->at(index - 1).addNewDoubleOrigAttr(y);
				objects->at(index - 1)->addNewDoubleOrigAttr(x);
				objects->at(index - 1)->addNewDoubleOrigAttr(y);
			}
			else if (type == 2) {
				f >> ix;
				f >> comma;
				f >> iy;
				objects->at(index - 1)->addNewIntOrigAttr(ix);
				objects->at(index - 1)->addNewIntOrigAttr(iy);
			}
			i++;
		}
		cout << "Leitura do arquivo concluida. Num Obj: " << m << endl;
	}
	else {
		cerr << "Couldn't open file!" << endl;
	}
	f.close();
	this->objects = objects;
}

void Input::normEntry()
{
	const int numAttr = 2;
	double normAttrs[numAttr];
	if (type == 1) { // type double
		double minDoubleAttrs[numAttr], maxDoubleAttrs[numAttr];

		for (int i = 0; i < numAttr; i++) {
			minDoubleAttrs[i] = getMinDoubleAttr(i);
			maxDoubleAttrs[i] = getMaxDoubleAttr(i);
		}

		for (std::vector<Object*>::iterator it = objects->begin(); it != objects->end(); ++it) {
			normAttrs[0] = (((*it)->getOrigDoubleAttr(0) - minDoubleAttrs[0]) / (maxDoubleAttrs[0] - minDoubleAttrs[0]));
			(*it)->addNewDoubleNormAttr(normAttrs[0]);
			normAttrs[1] = ((*it)->getOrigDoubleAttr(1) - minDoubleAttrs[1]) / (maxDoubleAttrs[1] - minDoubleAttrs[1]);
			(*it)->addNewDoubleNormAttr(normAttrs[1]);
		}
		cout << "Entry normalized" << endl;
	}
	else if (type == 2) {
		int minIntAttrs[numAttr], maxIntAttrs[numAttr];
		for (int i = 0; i < numAttr; i++) {
			minIntAttrs[i] = getMinIntAttr(i);
			maxIntAttrs[i] = getMaxIntAttr(i);

		}

		for (std::vector<Object*>::iterator it = objects->begin(); it != objects->end(); ++it) {
			normAttrs[0] = ( (double)(  (*it)->getOrigIntAttr(0) - minIntAttrs[0] ) / (double)(maxIntAttrs[0] - minIntAttrs[0]) );
			(*it)->addNewDoubleNormAttr(normAttrs[0]);
			normAttrs[1] = ((double) (*it)->getOrigIntAttr(1) - minIntAttrs[1]) / (double) (maxIntAttrs[1] - minIntAttrs[1]);
			(*it)->addNewDoubleNormAttr(normAttrs[1]);
		}
		cout << "Entry normalized" << endl;
	}
}

void Input::readNormEntry()
{
	string file = normInputPath;
	int m = 0;
	int n = 0;
	char comma;
	ifstream f;
	double index;
	double x, y;
	vector <Object*> objects;
	vector <double> attrs;
	int i = 0;
	f.open(file);
	if (f.is_open()) {
		f >> m;
		f >> comma;
		f >> n;
		for (int j = 0; j < m; j++)
			objects.push_back(new Object(j + 1));
		while (i < m) {
			f >> index;
			f >> comma;
			f >> x;
			f >> comma;
			f >> y;
			objects[index - 1]->addNewDoubleNormAttr(x);
			objects[index - 1]->addNewDoubleNormAttr(y);
			i++;
		}
		cout << "Leitura do arquivo normalizado concluida. Num Obj: " << m << endl;
	}
	else {
		cerr << "Couldn't open file!" << endl;
	}
	f.close();
	this->objects = &objects;
}




double Input::getMaxDoubleAttr(int index)
{
	
	double maxAttr = objects->at(0)->getOrigDoubleAttr(index);
	vector <Object*>::iterator it;
	for (it = objects->begin(); it != objects->end(); ++it) {
		if (maxAttr < (*it)->getOrigDoubleAttr(index)) {
			maxAttr = (*it)->getOrigDoubleAttr(index);
		}
	}
	return maxAttr;
}

double Input::getMaxNormDoubleAttr(int index)
{
	
	double maxAttr = objects->at(0)->getNormDoubleAttr(index);
	vector <Object*>::iterator it;
	for (it = objects->begin(); it != objects->end(); ++it) {
		if (maxAttr < (*it)->getNormDoubleAttr(index)) {
			maxAttr = (*it)->getNormDoubleAttr(index);
		}
	}
	return maxAttr;
}





double Input::getMinNormDoubleAttr(int index)
{
	
	double minAttr = objects->at(0)->getNormDoubleAttr(index);
	vector <Object*>::iterator it;
	for (it = objects->begin(); it != objects->end(); ++it) {
		if (minAttr > (*it)->getNormDoubleAttr(index)) {
			minAttr = (*it)->getNormDoubleAttr(index);
		}
	}
	return minAttr;
}

int Input::getMaxIntAttr(int index)
{
	int maxAttr = objects->at(0)->getOrigIntAttr(index);
	vector <Object*>::iterator it;
	for (it = objects->begin(); it != objects->end(); ++it) {
		if (maxAttr < (*it)->getOrigIntAttr(index)) {
			maxAttr = (*it)->getOrigIntAttr(index);
		}
	}
	return maxAttr;
}

int Input::getMaxNormIntAttr(int index)
{
	int maxAttr = objects->at(0)->getOrigIntAttr(index);
	vector <Object*>::iterator it;
	for (it = objects->begin(); it != objects->end(); ++it) {
		if (maxAttr < (*it)->getNormIntAttr(index)) {
			maxAttr = (*it)->getNormIntAttr(index);
		}
	}
	return maxAttr;
}

double Input::getMinDoubleAttr(int index)
{

	double minAttr = objects->at(0)->getOrigDoubleAttr(index);
	vector <Object*>::iterator it;
	for (it = objects->begin(); it != objects->end(); ++it) {
		if (minAttr > (*it)->getOrigDoubleAttr(index)) {
			minAttr = (*it)->getOrigDoubleAttr(index);
		}
	}
	return minAttr;
}


int Input::getMinIntAttr(int index)
{
	int minAttr = objects->at(0)->getOrigIntAttr(index);
	vector <Object*>::iterator it;
	for (it = objects->begin(); it != objects->end(); ++it) {
		if (minAttr > (*it)->getOrigIntAttr(index)) {
			minAttr = (*it)->getOrigIntAttr(index);
		}
	}
	return minAttr;
}

int Input::getMinNormIntAttr(int index)
{
	int minAttr = objects->at(0)->getOrigIntAttr(index);
	vector <Object*>::iterator it;
	for (it = objects->begin(); it != objects->end(); ++it) {
		if (minAttr > (*it)->getNormIntAttr(index)) {
			minAttr = (*it)->getNormIntAttr(index);
		}
	}
	return minAttr;
}



vector<Object*>* Input::getObjects()
{
	return objects;
}

int Input::getNumObjects()
{
	return this->objects->size();
}

void Input::setInputPath(string path)
{
	this->inputPath = path;
	string tempPath = inputPath;
	tempPath.pop_back();
	tempPath.pop_back();
	tempPath.pop_back();
	tempPath.pop_back();
	tempPath += "Norm.txt";
	normInputPath = tempPath;
}

void Input::saveNormEntry()
{
	system("cls");
	ofstream a;

	string filePath = normInputPath;


	cout << filePath.c_str();

	a.open(filePath);

	int numObjs = objects->size();
	int numAttrs = 2;  // BAD CODE 

	a << numObjs;
	a << ",";
	a << numAttrs;
	a << "\n";


	for (std::vector<Object*>::iterator it = objects->begin(); it != objects->end(); ++it)
		a << (*it)->getId() << "," << (*it)->getNormDoubleAttr(0) << "," << (*it)->getNormDoubleAttr(1) << "\n" << endl;
	


	a.close();


}

void Input::setType(int type)
{
	this->type = type;
}


void Input::buildGraph()
{
	for (int i = 0; i < numObjs; i++) {
		for (int j = 0; j < numObjs; j++) {
			if (i != j) {
				double distance = euclideanDistance(objects->at(i), objects->at(j));

			}
			else if (i > j) {
				double distance = euclideanDistance(objects->at(i), objects->at(j));
				objects->at(i)->addEdge(objects->at(i)->getId(), distance, objects->at(j)->getId());
			}

		}
	}
}


double Input::euclideanDistance(Object *a, Object *b)
{
	double dist = 0.0;
	int numAttr = 2;
	for (int i = 0; i<numAttr; i++)
		dist += pow((a->getNormDoubleAttr(i) - b->getNormDoubleAttr(i)), 2);

	return sqrt(dist);
}