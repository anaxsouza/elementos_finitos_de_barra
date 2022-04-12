#pragma once
#include<vector>

#include"ComponenteModel.h"

using namespace std;
class Node : public ComponenteModel
{
public:
	Node(int id, double x1, double x2, int dof);
	Node(int id, double x1, double x2, double x3, int dof);
	Node();
	~Node();
	vector<double> getNodeCoords();
	void setExternalGlobalLoads(vector<double> f);
	vector<double> getExternalGlobalLoads();
	void setBoundCondition(int k, double par, string boundCond);
	vector<string> getBoundCond();
	vector<double> getBoundCondValue();
	void setExternalLocalLoads(vector<double> f);
	vector<double> getExternalLocalLoads();
	string getType();
private:
	vector<double> comp, boundCondValue, externalLocalLoads, externalGlobalLoads;
	vector<string> boundCond;//configura as condições de contorno em deslocamentos
};

