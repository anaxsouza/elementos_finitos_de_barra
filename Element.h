#pragma once
#include"ComponenteModel.h"
#include"Node.h"

#include<vector>
#include<typeinfo>
#include<math.h>
#include<iostream>

using namespace std;
class Element : public ComponenteModel
{
public:
	Element();
	Element(int id, int nds, double angle, int dof);
	~Element();
	void evaluateLengthOfElement();
	void addComponentModel(ComponenteModel* component);
	void setMaterialParam(vector<double> matParam);
	void setIncNodalMatrix(vector< vector<double> > inc);
	void setLocalStiffMatrix(vector< vector<double> > kl);
	void setGlobalStiffMatrix(vector< vector<double> > kl);
	void setTransformationMatrix(vector < vector<double> > tMatriz);
	void setDispLocalSolution(vector<double> v);
	void setDispGlobalSolution(vector<double> v);
	void setForceLocalSolution(vector<double> v);
	vector<double> getForceLocalSolution();
	vector<double> getDispLocalSolution();
	void setExternalGlobalForce(vector<double> f);
	void setExternalLocalForce(vector<double> f);
	void setExternalLocalLoads_Test(vector< vector<double> > f);
	vector< vector<double> > getExternalLocalLoads_Test();
	void setExternalLocalLoads(vector<double> f);
	vector<double> getExternalLocalLoads();
	vector<double> getExternalLocalForce();
	vector<double> getExternalGlobalForce();
	vector<double> getDispGlobalSolution();
	vector< vector<double> > getIncNodalMatrix();
	vector < vector<double> > getTransformationMatrix();
	vector< vector<double> > getGlobalStiffMatrix();
	vector< vector<double> > getLocalStiffMatrix();
	double getAngle();
	double getNormalModulos();
	double getInercia();
	double getArea();
	double getLengthOfElement();
	vector<ComponenteModel*> getNodeList();
	int getNumberOfNodes();
	string getType();

private:
	double normalModulos, inertia, area, angle, lengthOfElement;
	vector< vector< double > > kLocalMatriz, kGlobalMatriz, tMatriz, incNodalMatrix, externalLocalLoads_Test;
	vector<double> dispGlobalSolution, dispLocalSolution, externalLocalLoads, forceLocalSolution, externalGlobalForce, externalLocalForce;
	vector<ComponenteModel*> nodeList;

};

