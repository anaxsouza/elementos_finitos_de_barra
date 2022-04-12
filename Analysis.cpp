#include "Analysis.h"



Analysis::Analysis()
{
}


Analysis::~Analysis()
{
}

void Analysis::setData(Model * model)
{
	this->model = model;
}

vector<vector<double>> Analysis::transposeMat(vector< vector<double> > mat)
{
	size_t numLines = mat.size();
	size_t numColumns = mat.at(0).size();
	vector< vector<double> > newMat(numColumns);

	for (size_t i = 0; i < newMat.size(); i++) {
		vector<double> row(numLines);
		newMat.at(i) = (row);
	}
	for (size_t i = 0; i < numColumns; i++) {
		
		for (size_t j = 0; j < numLines; j++) {
			newMat[i][j] = mat[j][i];
		}
	}
	return newMat;
}

vector<vector<double>> Analysis::timesMat(vector< vector<double> > m1, vector< vector<double> > m2)
{
	size_t nLines1 = m1.size();
	size_t nCollums1 = m1.at(0).size();
	size_t nLines2 = m2.size();
	size_t nCollums2 = m2.at(0).size();

	vector< vector<double> > m(nLines1);

	for (size_t i = 0; i < nLines1; i++) {

		vector<double> row(nCollums2);
		m.at(i) = (row);
	}

	if (nCollums1 != nLines2) {
		cout << "As matrizes nao podem ser multiplicadas!" << endl;
	}
	else {

		for (size_t i = 0; i < nLines1; i++) {

			for (size_t j = 0; j < nCollums2; j++) {

				for (size_t c = 0; c < nCollums1; c++) {
					m[i][j] += m1[i][c] * m2[c][j];
				}
			}
		}
	}

	return m;
}

vector<vector<double>> Analysis::sumMat(vector< vector<double> > m1, vector< vector<double> > m2)
{
	size_t nLines1 = m1.size();
	size_t nCollums1 = m1.at(0).size();
	size_t nLines2 = m2.size();
	size_t nCollums2 = m2.at(0).size();
	vector< vector<double> > m(nLines1);

	for (size_t i = 0; i < m.size(); i++) {
		vector<double> row(nCollums1);
		m.at(i) = (row);
	}

	if ((nLines1 != nLines2) || (nCollums1 != nCollums2))
		cout << "Matrizes com numero de linhas ou colunas diferentes!" << endl;
	else {
		for (size_t i = 0; i < nLines1; i++) {

			for (size_t j = 0; j < nCollums1; j++) {
				m[i][j] = m1[i][j] + m2[i][j];
			}
		}
	}

	return m;
}

vector< vector<double> > Analysis::extendedMatrix(vector< vector<double> > a, vector<double> v)
{
	vector< vector<double> > mat(a.size());

	for (size_t i = 0; i < mat.size(); i++) {
		vector<double> row(a.size() + 1);
		mat.at(i) = row;
	}
	for (int i = 0; i < a.size(); i++)
		for (int j = 0; j <= a.size(); j++) {
			if (j == a.size())
				mat[i][j] = v[i];
			else
				mat[i][j] = a[i][j];
		}

	return mat;
}

vector<double> Analysis::solveLinearSistem(vector< vector<double> > a)
{
	size_t n = a.size();
	double m;

	for (size_t k = 0; k < n - 1; k++) {
		this->ortog(k, a, n);
		for (int i = k + 1; i < n; i++) {
			m = a[i][k] / a[k][k];
			for(int j = k; j < n + 1; j++){
				a[i][j] = a[i][j] - m * a[k][j];
			}
		}
	}

	vector<double> X(n);
	X[n - 1] = a[n - 1][n] / a[n - 1][n - 1];
	cout<<"-> Vetor solucao `X`:"<<endl;
	cout << a[n - 1][n] << " / " << a[n - 1][n - 1] << "=> " << endl;
	
	for (int i = n - 2; i > -1; i--) {
		X[i] = a[i][n];
		for (int j = i + 1; j < n; j++) {
			X[i] = X[i] - a[i][j] * X[j];
		}
		X[i] = X[i] / a[i][i];
	}

	return X;
}

vector<double> Analysis::timesMatVec(vector< vector<double> > m, vector<double> v)
{
	size_t nL1 = m.size(); // numero de linhas da matriz M1
	size_t nC3 = v.size(); // numero de colunas da matriz M2

	vector<double> Sol (nL1);

	for (size_t i = 0; i < nL1; i++)
		for (size_t j = 0; j < nC3; j++)
			Sol[i] += m[i][j] * v[j];

	return Sol;
}

void Analysis::impMat(vector< vector<double> > m1)
{
	cout << "Em impMat" << endl;
	size_t nLines1 = m1.size();
	size_t nCollums1 = m1.at(0).size();

	cout << "nLl = " << nLines1 << endl;
	cout << "nCl = " << nCollums1 << endl;

	for (size_t i = 0; i < nLines1; i++) {

		for (size_t j = 0; j < nCollums1; j++) {

			if (j == (nCollums1 - 1)) {
				cout << "M[" << i << "][" << j << "] = " <<m1[i][j]<<endl;
			}
			else {
				cout << "M[" << i << "][" << j << "] = " << m1[i][j];
				cout << "\t";
			}
		}
	}

}

void Analysis::ortog(int col, vector< vector<double> > a, int n)
{
	double c = abs(a[col][col]);
	int pos = col;

	for (int i = col + 1; i < n; i++) {
		if (abs(a[i][col]) > c) {
			c = abs(a[i][col]);
			pos = i;
		}
	}
	vector<double> aa(n + 1); // vetor que guarda a linha com o valor
									 // maximo
	for (int j = col; j < n + 1; j++) {
		aa[j] = a[pos][j];
		a[pos][j] = a[col][j];
		a[col][j] = aa[j];
	}
}

void Analysis::impVec(vector<double> V1)
{
	cout << "Em impVec" << endl;
	int nL1 = V1.size();

	cout << "nLl = "<< nL1 << endl;
	for (size_t i = 0; i < nL1; i++)
		cout<<"V[" << i << "]=" << V1[i]<<endl;
}

vector<double> Analysis::sum(vector<double> v1, vector<double> v2)
{
	size_t nv1 = v1.size();
	size_t nv2 = v2.size();
	vector<double> v(nv1);

	if (nv1 != nv2) {
		cout << "Vectors with different number of elements!" << endl;
	}
	else {
		for (size_t i = 0; i < nv1; i++)
			v[i] = v1[i] + v2[i];
	}

	return v;
}

vector<double> Analysis::sub(vector<double> v1, vector<double> v2)
{
	size_t nv1 = v1.size();
	size_t nv2 = v2.size();
	vector<double> v(nv1);

	if (nv1 != nv2) {
		cout<<"Vectors with different number of elements!"<<endl;
	}
	else {
		for (size_t i = 0; i < nv1; i++)
			v[i] = v1[i] - v2[i];
	}

	return v;
}
