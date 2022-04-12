#pragma once
#include"Analysis.h"
#include"Element.h"

class Linear : public Analysis
{
public:
	Linear();
	~Linear();
	void makeAnalysis();
private:
	void defLocalStiffMatrixOfElement(Element* e);
	void evaluateLocalStiffMatrixOfEachElement();
	void evaluateTransformationMatrix();
	void defTransformationMatrixOfElement(Element* e);
	void evaluateGlobalStiffMatrixOfEachElement();
	void defGlobalStiffMatrixOfElement(Element* e);
	void evaluateIncNodalMatrix();
	void defIncNodalMatrix(Element* e);
	void assemblyGlobalStiffMatrix();
	void assemblyExternalLoadVector();
	void applyingBoundCond();
	void applyingBoundCondInKMatrix(string boundCond, int nId, int direction);
	void calcDisplacementGlobalVector();
	void calcForceGlobalVector();
	void calcDisplacementLocalVector();
	void setDisplacementGlobalVectorForEachElement(Element* e);
	void calcForceLocalVector();
};

