#include "ComponenteModel.h"



ComponenteModel::ComponenteModel()
{
}


ComponenteModel::~ComponenteModel()
{
}

int ComponenteModel::getDim()
{
	return this->dim;
}

void ComponenteModel::setDim(int dim)
{
	this->dim = dim;
}

int ComponenteModel::getId()
{
	return this->id;
}

void ComponenteModel::setId(int id)
{
	this->id = id;
}

int ComponenteModel::getDof()
{
	return this->dof;
}

void ComponenteModel::setDof(int dof)
{
	this->dof = dof;
}
