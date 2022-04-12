#include "Element.h"



Element::Element()
{
}


Element::Element(int id, int nds, double angle, int dof)
{
	this->id = id;
	this->dof = dof;
	this->angle = angle;

	for (int i = 0; i < nds; i++) {

		vector<double> row(dof);
		this->externalLocalLoads_Test.push_back(row);
	}
}

Element::~Element()
{
}

void Element::evaluateLengthOfElement()
{
	double lElement = 0;

	Node* n1 = (Node*) this->nodeList.at(0);
	vector<double> coordsN1 = n1->getNodeCoords();

	Node* n2 = (Node*) this->nodeList.at(1);
	vector<double> coordsN2 = n2->getNodeCoords();

	double par1 = pow(coordsN2.at(0) - coordsN1.at(0), 2) + pow(coordsN2.at(1) - coordsN1.at(1), 2);
	lElement = sqrt(par1);

	this->lengthOfElement = lElement;
}

void Element::addComponentModel(ComponenteModel* component)
{
	if (component->getType() == "Node")
		this->nodeList.push_back(component);
	else
		cout << "Invalid LModel component type to this object" << endl;
}

void Element::setMaterialParam(vector<double> matParam)
{
	this->normalModulos = matParam.at(0);
	this->inertia = matParam.at(1);
	this->area = matParam.at(2);
}

void Element::setIncNodalMatrix(vector< vector<double> > inc)
{
	this->incNodalMatrix = inc;
}

void Element::setLocalStiffMatrix(vector< vector<double> > kl)
{
	this->kLocalMatriz = kl;
}

void Element::setGlobalStiffMatrix(vector<vector<double>> kl)
{
	this->kGlobalMatriz = kl;
}

void Element::setTransformationMatrix(vector<vector<double>> tMatriz)
{
	this->tMatriz = tMatriz;
}

void Element::setDispLocalSolution(vector<double> v)
{
	this->dispLocalSolution = v;
}

void Element::setDispGlobalSolution(vector<double> v)
{
	this->dispGlobalSolution = v;
}

void Element::setForceLocalSolution(vector<double> v)
{
	this->forceLocalSolution = v;
}

vector<double> Element::getForceLocalSolution()
{
	return this->forceLocalSolution;
}

vector<double> Element::getDispLocalSolution()
{
	return this->dispLocalSolution;
}

void Element::setExternalGlobalForce(vector<double> f)
{
	this->externalGlobalForce = f;
}

void Element::setExternalLocalForce(vector<double> f)
{
	this->externalLocalForce= f;
}

void Element::setExternalLocalLoads_Test(vector<vector<double>> f)
{
	for (size_t i = 0; i < externalLocalLoads_Test.size(); i++)
		for (size_t j = 0; j < (externalLocalLoads_Test[0]).size(); j++)
			this->externalLocalLoads_Test[i][j] = -f[i][j];
}

vector<vector<double>> Element::getExternalLocalLoads_Test()
{
	return this->externalLocalLoads_Test;
}

void Element::setExternalLocalLoads(vector<double> f)
{
	this->externalLocalLoads = f;
}

vector<double> Element::getExternalLocalLoads()
{
	return this->externalLocalLoads;
}

vector<double> Element::getExternalLocalForce()
{
	return this->externalLocalForce;
}

vector<double> Element::getExternalGlobalForce()
{
	return this->externalGlobalForce;
}

vector<double> Element::getDispGlobalSolution()
{
	return this->dispGlobalSolution;
}

vector<vector<double>> Element::getIncNodalMatrix()
{
	return this->incNodalMatrix;
}

vector< vector<double> > Element::getTransformationMatrix()
{
	return this->tMatriz;
}

vector<vector<double>> Element::getGlobalStiffMatrix()
{
	return this->kGlobalMatriz;
}

vector<vector<double>> Element::getLocalStiffMatrix()
{
	return this->kLocalMatriz;
}

double Element::getAngle()
{
	return this->angle;
}

double Element::getNormalModulos()
{
	return this->normalModulos;
}

double Element::getInercia()
{
	return this->inertia;
}

double Element::getArea()
{
	return this->area;
}

double Element::getLengthOfElement()
{
	return this->lengthOfElement;
}

vector<ComponenteModel*> Element::getNodeList()
{
	return this->nodeList;
}

int Element::getNumberOfNodes()
{
	return this->nodeList.size();
}

string Element::getType()
{
	return "Element";
}
