#include "Node.h"



Node::Node(int id, double x1, double x2, int dof)
{
	this->id = id;
	this->dof = dof;	
	
	// Creates and initializes the double array comp which contains nodes
	// coordinates
	for (int i = 0; i < 2; i++) this->comp.push_back(0);

	for (int i = 0; i < dof; i++) { 
		this->boundCond.push_back(""); 
		this->boundCondValue.push_back(0);
		this->externalGlobalLoads.push_back(0);
		this->externalLocalLoads.push_back(0);
	}
	// Sets nodes coordinates(2D case)
	this->comp.at(0) = (x1);
	this->comp.at(1) = (x2);

}

Node::Node(int id, double x1, double x2, double x3, int dof)
{
	this->id = id;
	this->dof = dof;

	// Creates and initializes the double array comp which contains nodes
	// coordinates
	for (int i = 0; i < 3; i++) this->comp.push_back(-1);

	for (int i = 0; i < dof; i++) {
		this->boundCond.push_back("");
		this->boundCondValue.push_back(0);
		this->externalGlobalLoads.push_back(0);		
	}

	this->comp.at(0) = (x1);
	this->comp.at(1) = (x2);
	this->comp.at(2) = (x3);

}

Node::Node() {

}


Node::~Node()
{
}

vector<double> Node::getNodeCoords()
{
	return this->comp;
}

void Node::setExternalGlobalLoads(vector<double> f)
{
	this->externalGlobalLoads = f;
}

vector<double> Node::getExternalGlobalLoads()
{
	return this->externalGlobalLoads;
}

void Node::setBoundCondition(int k, double par, string boundCond)
{
	this->boundCond[k] = boundCond;
	this->boundCondValue[k] = par;
}

vector<string> Node::getBoundCond()
{
	return this->boundCond;
}

vector<double> Node::getBoundCondValue()
{
	return this->boundCondValue;
}

void Node::setExternalLocalLoads(vector<double> f)
{
	this->externalLocalLoads = f;
}

vector<double> Node::getExternalLocalLoads()
{
	return this->externalLocalLoads;
}

string Node::getType()
{
	return "Node";
}
