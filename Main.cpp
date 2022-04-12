#include<iostream>
#include<stdio.h>
#include<vector>

#include"Input.h"
#include"Linear.h"

using namespace std;
int main() {

	Input* input = new Input();
	Linear* analise = new Linear();
	analise->setData(input->getModel());	
	analise->makeAnalysis();
	delete input;
	getchar();
	system("pause");
	return 0;
	
}