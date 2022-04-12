#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<typeinfo>
#include<string>

#include"Model.h"
#include"ComponenteModel.h"
#include"Node.h"
#include"Element.h"

using namespace std;

class Model : public ComponenteModel
{
public:
	Model(int id);
	~Model();
	void addComponentModel(ComponenteModel* component);
	string getType();
	vector<ComponenteModel*>& getNodeList();
	vector<ComponenteModel*>& getElementList();
private:
	vector<ComponenteModel*> nodeList;
	vector<ComponenteModel*> elementList;
	double normalModulos, inertia, area;
};

