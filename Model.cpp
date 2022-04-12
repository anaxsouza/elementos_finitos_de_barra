#include "Model.h"



Model::Model(int id)
{
	this->id = id;
}


Model::~Model()
{
}

void Model::addComponentModel(ComponenteModel* component)
{	
	
	if(component->getType() == "Node"){
		this->nodeList.push_back(component);
	}
	else if(component->getType() == "Element"){
		this->elementList.push_back(component);
	}
	else {
		cout << "Invalid Model Component type to this object"<<endl;
	}
}

string Model::getType()
{
	return "Model";
}

vector<ComponenteModel*>& Model::getNodeList()
{
	return this->nodeList;
}

vector<ComponenteModel*>& Model::getElementList()
{
	return this->elementList;
}
