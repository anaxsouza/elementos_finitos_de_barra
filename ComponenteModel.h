#pragma once
#include<string>
using namespace std;
class ComponenteModel
{
public:
	ComponenteModel();
	~ComponenteModel();
	int getDim();
	void setDim(int dim);
	int getId();
	void setId(int id);
	int getDof();
	void setDof(int dof);
	virtual string getType() = 0;
	
protected:
	int id, dim, dof;

};

