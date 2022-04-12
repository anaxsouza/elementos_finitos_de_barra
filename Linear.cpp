#include "Linear.h"



Linear::Linear()
{
}


Linear::~Linear()
{
}

void Linear::makeAnalysis()
{
	
	this->eList = this->model->getElementList();	
	this->nList = this->model->getNodeList();	
	this->dof = this->model->getDof();

	// Cálcula a matriz de rigidez local de cada elemento
	this->evaluateLocalStiffMatrixOfEachElement();	//done
	// Cálcula a matriz de rotação de cada elemento
	this->evaluateTransformationMatrix();//done	
	// Cálcula a matriz de rigidez global do elemento
	this->evaluateGlobalStiffMatrixOfEachElement();	//done
	// Define a matriz de incidência nodal
	this->evaluateIncNodalMatrix();	//done
	// Monta a matriz de rigidez global da estrutura	
	this->assemblyGlobalStiffMatrix();//done	
	// Montagem do vetor carregamento
	this->assemblyExternalLoadVector();	//done
	// Aplicação das condições de contorno na matriz de rigidez global da
	// estrutura.
	this->applyingBoundCond();	//done
	// Solucionando o sistema.
	this->calcDisplacementGlobalVector();//done	
	// Encontrando os esforços em cada nó
	this->calcForceGlobalVector();	//done
	// Calcula o vetor dos deslocamentos nodais de cada elemento
	this->calcDisplacementLocalVector();//done	
	// Calcula o vetor dos esforços nodais locais de cada elemento
	this->calcForceLocalVector();	
}

void Linear::defLocalStiffMatrixOfElement(Element * e)
{
	double normalModulos = e->getNormalModulos();
	double inertia = e->getInercia();
	double area = e->getArea();
	double lengthOfElement = e->getLengthOfElement();
	int numOfNodes = e->getNumberOfNodes();
	int dof = e->getDof();	
	vector< vector<double> > kLocal(numOfNodes * dof);

	for (size_t i = 0; i < kLocal.size(); i++) {
		vector<double> row(numOfNodes * dof);
		kLocal.at(i) = row;
	}	

	kLocal[0][0] = normalModulos * area / lengthOfElement; // k11
	kLocal[3][0] = -normalModulos * area / lengthOfElement; // k41
	kLocal[0][3] = -normalModulos * area / lengthOfElement; // k14
	kLocal[3][3] = normalModulos * area / lengthOfElement; // k44

	kLocal[1][1] = 12 * normalModulos * inertia / pow(lengthOfElement, 3); // k22
	kLocal[4][1] = -12 * normalModulos * inertia / pow(lengthOfElement, 3); // k52
	kLocal[1][4] = -12 * normalModulos * inertia / pow(lengthOfElement, 3); // k25
	kLocal[4][4] = 12 * normalModulos * inertia / pow(lengthOfElement, 3); // k25

	kLocal[1][2] = 6 * normalModulos * inertia / pow(lengthOfElement, 2); // k23
	kLocal[4][2] = -6 * normalModulos * inertia / pow(lengthOfElement, 2); // k53
	kLocal[4][5] = -6 * normalModulos * inertia / pow(lengthOfElement, 2); // k56
	kLocal[1][5] = 6 * normalModulos * inertia / pow(lengthOfElement, 2); // k56

	kLocal[2][1] = 6 * normalModulos * inertia / pow(lengthOfElement, 2); // k32
	kLocal[5][1] = 6 * normalModulos * inertia / pow(lengthOfElement, 2); // k62
	kLocal[2][4] = -6 * normalModulos * inertia / pow(lengthOfElement, 2); // k35
	kLocal[5][4] = -6 * normalModulos * inertia / pow(lengthOfElement, 2); // k65

	kLocal[2][2] = 4 * normalModulos * inertia / lengthOfElement; // k33
	kLocal[5][2] = 2 * normalModulos * inertia / lengthOfElement; // k63
	kLocal[2][5] = 2 * normalModulos * inertia / lengthOfElement; // k36
	kLocal[5][5] = 4 * normalModulos * inertia / lengthOfElement; // k66

	e->setLocalStiffMatrix(kLocal);
}

void Linear::evaluateLocalStiffMatrixOfEachElement()
{
	for (size_t i = 0; i < this->eList.size(); i++)
		this->defLocalStiffMatrixOfElement((Element*) this->eList.at(i));
}

void Linear::evaluateTransformationMatrix()
{
	for (size_t i = 0; i < this->eList.size(); i++)
		this->defTransformationMatrixOfElement((Element*) this->eList.at(i));
}

void Linear::defTransformationMatrixOfElement(Element * e)
{
	double angle = e->getAngle();
	double angleRad = PI * angle / 180;
	int numOfNodes = e->getNumberOfNodes();	
	int dof = e->getDof();
	vector< vector<double> > tMat(numOfNodes * dof);

	for (int i = 0; i < tMat.size(); i++) {

		vector<double> row(numOfNodes * dof);
		tMat.at(i) = (row);
	}

	tMat[0][0] = cos(angleRad); // t[1][1]
	tMat[1][1] = cos(angleRad); // t[2][2]
	tMat[3][3] = cos(angleRad); // t[4][4]
	tMat[4][4] = cos(angleRad); // t[5][5]

	tMat[1][0] = -sin(angleRad); // t[2][1]
	tMat[0][1] = sin(angleRad); // t[1][2]
	tMat[4][3] = -sin(angleRad); // t[5][4]
	tMat[3][4] = sin(angleRad); // t[4][5]

	tMat[2][2] = 1; // t[3][3]
	tMat[5][5] = 1; // t[6][6]
	
	e->setTransformationMatrix(tMat);
}

void Linear::evaluateGlobalStiffMatrixOfEachElement()
{
	for (size_t i = 0; i < this->eList.size(); i++)
		this->defGlobalStiffMatrixOfElement((Element*) this->eList.at(i));
}

void Linear::defGlobalStiffMatrixOfElement(Element* e)
{
	vector< vector<double> > tMat = e->getTransformationMatrix();	
	vector< vector<double> > tMatTransp = this->transposeMat(tMat);	
	vector< vector<double> > kLMat = e->getLocalStiffMatrix();
	vector< vector<double> > k1 = this->timesMat(tMatTransp, kLMat);
	vector< vector<double> > kGMatrix = this->timesMat(k1, tMat);
		
	e->setGlobalStiffMatrix(kGMatrix);	
}

void Linear::evaluateIncNodalMatrix()
{
	for (size_t i = 0; i < this->eList.size(); i++)
		this->defIncNodalMatrix((Element*) this->eList.at(i));
}

void Linear::defIncNodalMatrix(Element * e)
{
	int numberOfStrucNodes = this->model->getNodeList().size();
	int numbOfElementNodes = e->getNumberOfNodes();
	vector<ComponenteModel*> nList = e->getNodeList();
	vector< vector<double> > incMat(numbOfElementNodes * this->dof);
	
	for (size_t i = 0; i < incMat.size(); i++) {
		vector<double> row(numberOfStrucNodes * this->dof);
		incMat.at(i) = row;
	}
	
	for (size_t i = 0; i < nList.size(); i++) {
		
		Node* n = ((Node*) nList.at(i));
		int nId = n->getId();
		
		for (int j = 0; j < this->dof; j++) {			
			incMat[(i * this->dof + j)][(nId * this->dof - 3 + j)] = 1;
		}		
	}	

	e->setIncNodalMatrix(incMat);
}

void Linear::assemblyGlobalStiffMatrix()
{
	size_t numberOfStrucNodes = this->model->getNodeList().size();	
	this->kGlobalMatrixOfStructure.resize((numberOfStrucNodes * dof));

	for (int i = 0; i < (numberOfStrucNodes * dof); i++) {
		vector<double> row(numberOfStrucNodes * dof);
		this->kGlobalMatrixOfStructure.at(i) = (row);
	}
	
	for (size_t i = 0; i < this->eList.size(); i++) {

		Element* e = ((Element*) this->eList.at(i));		
		vector< vector<double> > incMat = e->getIncNodalMatrix();		
		vector< vector<double> > incMatTransp = this->transposeMat(incMat);		
		vector< vector<double> > kGEMat = e->getGlobalStiffMatrix();		
		vector< vector<double> > m1 = this->timesMat(incMatTransp, kGEMat);		
		vector< vector<double> > m2 = this->timesMat(m1, incMat);		
		this->kGlobalMatrixOfStructure = this->sumMat(this->kGlobalMatrixOfStructure, m2);		
	}
	
	cout << "" << endl;
	cout << ". Matriz de Rigidez global da estrutura!" << endl;
	this->impMat(this->kGlobalMatrixOfStructure);	
}

void Linear::assemblyExternalLoadVector()
{
	size_t numberOfStrucNodes = this->nList.size();
	
	this->externalGlobalLoadVector.resize(numberOfStrucNodes * this->dof);
	this->externalLocalLoadVector.resize(numberOfStrucNodes * this->dof);
	
	for (size_t i = 0; i < numberOfStrucNodes; i++) {

		Node* n = (Node*) this->nList.at(i);
		int nId = n->getId();
		vector<double> boundCondValue = n->getBoundCondValue();
				
		for (int j = 0; j < this->dof; j++) {			
			this->externalGlobalLoadVector[3 * (nId - 1) + j] = boundCondValue.at(j);			
		}		
		
	}
	
	for (size_t i = 0; i < numberOfStrucNodes; i++) {

		Node* n = (Node*) this->nList.at(i);
		int nId = n->getId();
		vector<double> externalDistrGlobalForceValue = n->getExternalGlobalLoads();
		vector<double> externalDistrLocalForceValue = n->getExternalLocalLoads();
		
		for (int j = 0; j < this->dof; j++) {
			this->externalGlobalLoadVector[3 * (nId - 1) + j] += externalDistrGlobalForceValue.at(j);
			this->externalLocalLoadVector[3 * (nId - 1) + j] += externalDistrLocalForceValue[j];
		}
		
	}
}

void Linear::applyingBoundCond()
{
	this->kGlobalMatrixOfStructureWithBoundCond.resize(this->kGlobalMatrixOfStructure.size());//modified later

	for (size_t i = 0; i < this->kGlobalMatrixOfStructure.size(); i++) {
		vector<double> row(kGlobalMatrixOfStructure.at(1).size());
		this->kGlobalMatrixOfStructureWithBoundCond.at(i) = (row);
	}

	for (size_t i = 0; i < this->kGlobalMatrixOfStructureWithBoundCond.at(0).size(); i++)
		for (size_t j = 0; j < kGlobalMatrixOfStructureWithBoundCond.at(1).size(); j++)
			this->kGlobalMatrixOfStructureWithBoundCond[i][j] = this->kGlobalMatrixOfStructure[i][j];
	
	size_t numberOfStrucNodes = this->nList.size();

	for (size_t i = 0; i < numberOfStrucNodes; i++) {
		Node* n = (Node*) this->nList.at(i);
		int nId = n->getId();
		vector<string> bCond = n->getBoundCond();		
		
		for (int j = 0; j < this->dof; j++) {
			string boundCond = bCond.at(j);			
			this->applyingBoundCondInKMatrix(boundCond, nId, j);
		}
	}

	this->externalLoadVectorWithBoundCond.resize(numberOfStrucNodes * this->dof);
	vector<ComponenteModel*> nList = this->model->getNodeList();
	
	for (size_t i = 0; i < nList.size(); i++) {
		Node* n = (Node*) this->nList.at(i);
		int nId = n->getId();
		vector<string> boundCond = n->getBoundCond();		
		
		for (int j = 0; j < boundCond.size(); j++) {
			if (boundCond[j].find("DIRI") != string::npos)
				this->externalLoadVectorWithBoundCond[3 * (nId - 1) + j] = 0;
			else if (boundCond[j].find("NEUM") != string::npos)
				this->externalLoadVectorWithBoundCond[3 * (nId - 1) + j] += externalGlobalLoadVector[3 * (nId - 1) + j];
		}		
	}
}

void Linear::applyingBoundCondInKMatrix(string boundCond, int nId, int direction)
{
	for (size_t i = 0; i < this->kGlobalMatrixOfStructureWithBoundCond[0].size(); i++) {
		if (boundCond.find("DIRI") != string::npos) {
			if (3 * (nId - 1) + direction == i)
				this->kGlobalMatrixOfStructureWithBoundCond[i][i] = 1;
			else {
				this->kGlobalMatrixOfStructureWithBoundCond[i][3 * (nId - 1) + direction] = 0;
				this->kGlobalMatrixOfStructureWithBoundCond[3 * (nId - 1) + direction][i] = 0;
			}
		}
	}
}

void Linear::calcDisplacementGlobalVector()
{
	vector< vector<double> > extMat = this->extendedMatrix(this->kGlobalMatrixOfStructureWithBoundCond, 
															this->externalLoadVectorWithBoundCond);

	//parte do sistema linear
	this->dispSolution = this->solveLinearSistem(extMat);
	cout << "" << endl;
	cout << ". Vetor dos deslocamentos nodais da estrutura!" << endl;
	this->impVec(this->dispSolution);
}

void Linear::calcForceGlobalVector()
{
	this->forceSolution = this->timesMatVec(this->kGlobalMatrixOfStructure, this->dispSolution);	
	this->reactionValues.resize(this->forceSolution.size());
	this->reactionValues = this->sub(this->forceSolution, this->externalGlobalLoadVector);
	cout << "" << endl;
	cout << ". Vetor com as reações de apoio da estrutura!" << endl;
	this->impVec(this->reactionValues);
}

void Linear::calcDisplacementLocalVector()
{
	// Para deslocamento
	for (int i = 0; i < this->eList.size(); i++) {
		Element* e = (Element*) this->eList.at(i);
		this->setDisplacementGlobalVectorForEachElement(e);
		vector<double> v = e->getDispGlobalSolution();
		vector< vector<double> > mT = e->getTransformationMatrix();
		e->setDispLocalSolution(this->timesMatVec(mT, v));

	}

	// Para força externas globais
	for (int i = 0; i < eList.size(); i++) {
		Element* e = (Element*) this->eList.at(i);
		vector<double> v = e->getExternalGlobalForce();
		vector< vector<double> > mT = e->getTransformationMatrix();
		e->setExternalLocalForce(this->timesMatVec(mT, v));
	}
}

void Linear::setDisplacementGlobalVectorForEachElement(Element * e)
{
	vector<double> dispGlobalVector (e->getNumberOfNodes() * this->dof);
	vector<double> forceGlobalVector(e->getNumberOfNodes() * this->dof);
	vector<double> forceLocalVector (e->getNumberOfNodes() * this->dof);
	vector<ComponenteModel*> nList = e->getNodeList();
	
	for (size_t i = 0; i < nList.size(); i++) {
		Node* n = (Node*) nList.at(i);
		int nId = n->getId();
		for (int j = 0; j < this->dof; j++) {
			dispGlobalVector[3 * i + j] = this->dispSolution[3 * (nId - 1) + j];
			forceGlobalVector[3 * i + j] = this->externalGlobalLoadVector[3 * (nId - 1) + j];
			forceLocalVector[3 * i + j] = this->externalLocalLoadVector[3 * (nId - 1) + j];
		}
	}
	e->setDispGlobalSolution(dispGlobalVector);
	e->setExternalGlobalForce(forceGlobalVector);
	e->setExternalLocalLoads(forceLocalVector);
}

void Linear::calcForceLocalVector()
{
	for (int i = 0; i < this->eList.size(); i++) {
		Element* e = (Element*) this->eList.at(i);
		vector< vector<double> > eStMat = e->getLocalStiffMatrix();
		vector<double> eDispLVec = e->getDispLocalSolution();		
		vector<double> partForceSolution = this->timesMatVec(eStMat, eDispLVec);
		
		// INÍCIO DO TESTE
		vector< vector<double> > externalLocalLoadsTest = e->getExternalLocalLoads_Test();		
		vector<double> externalLocalLoads (partForceSolution.size());
		
		for (size_t j = 0; j < externalLocalLoadsTest.size(); j++) {

			for (size_t k = 0; k < externalLocalLoadsTest[0].size(); k++) {
				externalLocalLoads[3 * j + k] = externalLocalLoadsTest[j][k];
			}
		}
		// FIM DO TEST	
		
		e->setForceLocalSolution(this->sum(partForceSolution, externalLocalLoads));

		cout << "" << endl;
		cout<<". Vetor local dos esforços do elemento "<< e->getId()<<endl;
		this->impVec(e->getForceLocalSolution());
	}
}
