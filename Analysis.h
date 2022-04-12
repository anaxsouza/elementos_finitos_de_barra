#pragma once
#define PI (3.141592653589793)
#include"Model.h"
#include<math.h>

class Analysis
{
public:
	Analysis();
	~Analysis();
	virtual void makeAnalysis() = 0;
	void setData(Model* model);
	
protected:
	Model* model;
	int dof;
	vector<ComponenteModel*> nList, eList;
	vector< vector<double> > kGlobalMatrixOfStructure, kGlobalMatrixOfStructureWithBoundCond;
	vector<double> externalGlobalLoadVector, externalLoadVectorWithBoundCond, dispSolution, forceSolution, reactionValues, externalLocalLoadVector;
	vector< vector<double> > transposeMat(vector< vector<double> > mat);
	vector< vector<double> > timesMat(vector< vector<double> > m1, vector< vector<double> > m2);
	vector< vector<double> > sumMat(vector< vector<double> > m1, vector< vector<double> > m2);
	vector< vector<double> > extendedMatrix(vector< vector<double> > a, vector<double> v);
	vector<double> solveLinearSistem(vector< vector<double> > a);
	vector<double> timesMatVec(vector< vector<double> > m, vector<double> v);
	void impMat(vector< vector<double> > m1);
	void ortog(int col, vector <vector <double>> a, int n);
	void impVec(vector<double> V1);
	vector<double> sum(vector<double> v1, vector<double> v2);
	vector<double> sub(vector<double> v1,  vector<double> v2);

};

