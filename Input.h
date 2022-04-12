#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<vector>

#include"Model.h"
#include"Node.h"

using namespace std;
class Input
{
public:
	Input();
	~Input();
	Model* getModel();
private:
	string path;
	string header;
	string analysis;
	vector<int> nMat;
	vector<double> matParam;
	string readString(ifstream* file, string line);
	string ignoreParenthesis(string txt);
	string ignoreEmptySpaces(string txt);
	string getFirstWord(string txt);
	vector<string> getWordsOfLine(string txt, char separator);	
	vector<string> getWordsOfLine(string txt, char separator,char separator_2 );
	vector<string> split(string text, char delimiter);
	int dim;
	int dof = 3;
	int nNodes;
	int nElem;
	int nCol;
	Model* model;
};

