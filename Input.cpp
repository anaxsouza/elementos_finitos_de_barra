#include "Input.h"



Input::Input()
{
	cout << "Enter File name(*.inp) : " << endl;
	cin >> this->path; //get data
	this->path += ".inp";	//extension	
	cout << " " << endl;

	//Creating file and etc...
	ifstream* file =  new ifstream();
	file->open(this->path);

	if (file->is_open()) {
		string line;
		while(!file->eof()){
		
			getline(*file, line);

			if (line.find("HEADER") != string::npos) {
				
				cout << "Reading File!" << endl;
				this->header = this->readString(file, line);
				cout <<"1.HEADER: " <<this->header << endl;	
				cout << " " << endl;
			}
			else if (line.find("ANALYSIS") != string::npos) {

				cout << "Reading Analysys!" << endl;
				analysis = this->readString(file, line);
				cout << "2. ANALYSIS:" << endl;
				this->dim = 2;
				cout << "Creating Model..." << endl;
				this->model = new Model(1);
				this->model->setDim(this->dim);
				this->model->setDof(this->dof);
				cout << " " << endl;

			}
			else if (line.find("NODE_LIST") != string::npos) {
				cout << "Reading NODE_LIST!" << endl;
				getline(*file, line);//next line
				this->nNodes = atoi(this->readString(file, line).c_str());
				cout << "3. NODES: " << endl;
				cout << "   Number of nodes = "<<this->nNodes<< endl;
				cout << ".creates nodes..." << endl;
				Node* n = new Node();// verify possible memory leak after

				int count = 1;
				this->nCol = this->dim + 1;
				vector<double> c (nCol);
				getline(*file, line);//next line

				do {			
					c.clear();
					string lvLine = readString(file, line);				
					lvLine = ignoreEmptySpaces(lvLine);	
				    vector<string> data = this->split(lvLine,' ');
					
					c.push_back(atof(data[0].c_str()));
					c.push_back(atof(data[1].c_str()));
					c.push_back(atof(data[2].c_str()));
					if(this->dim == 3)
						c.push_back(atof(data[3].c_str()));
					
					if (this->dim == 2)
						n = new Node((int) c[0], c[1], c[2], this->dof);
					else if (this->dim == 3)
						n = new Node((int) c[0], c[1], c[2], c[3], this->dof);

					this->model->addComponentModel(n);
					
					count++;
				} while (count <= nNodes);

				cout << ".nodes done!" << endl;
				cout << " " << endl;
			}
			else if (line.find("MATERIAL_LIST") != string::npos) {
				cout << "Reading MATERIAL_LIST!" << endl;
				getline(*file, line);//next line
				string lvLine = readString(file, line);				
				this->nMat.push_back(atoi(lvLine.c_str()));//number of materials
				getline(*file, line);//next line				
				double normalModulus = atof(this->getFirstWord(line).c_str());
				getline(*file, line);//next line		
				double inertia = atof(this->getFirstWord(line).c_str());
				getline(*file, line);//next line		
				double area = atof(this->getFirstWord(line).c_str());
				cout << " " << endl;
				cout << "4. MATERIAL_LIST: " << endl;
				cout << "   Number of Material : " << this->nMat.at(0) << endl;
				cout << "  Normal Modulus : " << normalModulus << endl;
				cout << "   Inercia: " << inertia << endl;
				cout << "   Area: " << area << endl;
				cout << ".material parameters done!" << endl;

				this->matParam.push_back(normalModulus);
				this->matParam.push_back(inertia);
				this->matParam.push_back(area);

				cout << "   Normal Modulus (used): " << normalModulus << endl;
				cout << "  Inercia(used) : " << inertia<< endl;
				cout << "   Area (used): " << area << endl;
				cout << ".material parameters done!" << endl;
				cout << "" << endl;
			}
			else if (line.find("ELEMENT_LIST") != string::npos) {
				cout << "Reading ELEMENT_LIST!" << endl;
				getline(*file, line);//next line
				this->nElem = atoi(this->readString(file, line).c_str());
				getline(*file, line);//next line
				getline(*file, line);//next line to materials
				cout << "5. ELEMENT_LIST: " << endl;
				cout << "   Number of elements = " << this->nElem << endl;
				cout << ".creates elements..." << endl;
				Element* e;

				vector<ComponenteModel*> nList = this->model->getNodeList();				
				int id, mat;
				double angle;
				
				for (int i = 0; i < nElem; i++) {
					
					vector<ComponenteModel*> elemNodeList;
					vector<ComponenteModel*> nodes(2);
					vector<string> words = getWordsOfLine(line, '	');
					id = atoi(words.at(0).c_str());
					mat = atoi(words.at(1).c_str());
					angle = atof(words.at(2).c_str());
					
					int id_node_1, id_node_2;
					id_node_1 = atoi(words.at(3).c_str());
					id_node_2 = atoi(words.at(4).c_str());
					e = new Element(id, 2, angle, this->dof);

					for (int k = 0; k < nList.size(); k++) {

						ComponenteModel* node_Test = nList.at(k);						
						int id_test = node_Test->getId();
					
						if (id_test == id_node_1) {
							nodes.at(0) = (node_Test);
						}
						else if (id_test == id_node_2) {								
							nodes.at(1) = (node_Test);
						}						
					}				
					
					e->addComponentModel(nodes.at(0));
					e->addComponentModel(nodes.at(1));
					e->evaluateLengthOfElement();

					for (size_t j = 0; j < this->nMat.size(); j++) {

						if (mat == this->nMat.at(0)) {							
							e->setMaterialParam(this->matParam);
						}
					}
					cout << "Element " << e->getId()<<endl;
					vector<ComponenteModel*> n = e->getNodeList();
					cout << "Node " << n.at(0)->getId() << endl;
					vector<double> coords_Node1 = ((Node*) n.at(0))->getNodeCoords();
					cout << "Coord x = " << coords_Node1.at(0) << endl;
					cout << "Coord y = " << coords_Node1.at(1) << endl;
					cout << "Node " << n.at(1)->getId() << endl;
					vector<double> coords_Node2 = ((Node*) n.at(1))->getNodeCoords();
					cout << "Coord x = " << coords_Node2.at(0) << endl;
					cout << "Coord y = " << coords_Node2.at(1) << endl;
					cout << "" << endl;
					this->model->addComponentModel(e);
						
					getline(*file, line);//next line
				}
				cout << ".elements done!" << endl;
				cout << "" << endl;
			}
			else if (line.find("NODAL_BOUND_COND_GLOBAL_WITH_CONCENTRATED_LOAD") != string::npos) {
				cout << "Reading NODAL_BOUND_COND!" << endl;
				getline(*file, line);//next line
				cout << "6. NODAL_BOUND_COND: " << endl;
				vector<ComponenteModel*> nodeList = this->model->getNodeList();
				string boundCond = "None";
				int nBC = atoi(this->readString(file, line).c_str());
				double par;
				getline(*file, line);//next line
				getline(*file, line);//next line
				getline(*file, line);//next line
				for (int i = 0; i < nBC; i++) {
					vector<string> words = this->getWordsOfLine(line, '	');
					this->nNodes = atoi(words.at(0).c_str());					
					int count = 0;
					bool flag = false;
				
					do {
						Node* node = ((Node*) nodeList[(count)]);
						int nodeId = node->getId();

						if (nodeId == this->nNodes) {
							int indexCond = 1;
							int indexPar = 2;
							for (int k = 0; k < this->dof; k++) {
								boundCond = words.at(indexCond);
								par = atof(words.at(indexPar).c_str());									
								node->setBoundCondition(k, par, boundCond);
								indexCond += 2;
								indexPar += 2;
							}							
							flag = true;
							
						}
						count++;
					} while (flag == false);					
					getline(*file, line);//next line
				}
				
			}
			else if (line.find("DISTRIBUTED_LOAD") != string::npos) {
				cout << "Reading DISTRIBUTED_LOAD!" << endl;
				cout << "7. DISTRIBUTED_LOAD: " << endl;
				getline(*file, line);//next line
				int nElem = atoi(this->readString(file, line).c_str());
				int numberOfElementNodes = 2;
				vector<ComponenteModel*> eList = model->getElementList();
				int eId, n1Id, n2Id;
				vector< vector<double> > loadValues(numberOfElementNodes);

				for (size_t k = 0; k < loadValues.size(); k++) {
					vector<double> row(this->dof);
					loadValues.at(k) = (row);
				}

				getline(*file, line);//next line
				getline(*file, line);//next line
				getline(*file, line);//next line

				for (int i = 0; i < nElem; i++) {
					Element* e;
					vector<ComponenteModel*> elm_Nodes;
					vector<string> words = this->getWordsOfLine(line, '		  ','		');
					
					eId = atoi(words.at(0).c_str());
					int lvCount = 1;

					for (int j = 0; j < eList.size(); j++) {
						Element* e_Test = ((Element*) eList.at(j));
						vector<int> elementNodesId(numberOfElementNodes);
						int eId_Test = e_Test->getId();

						if (eId_Test == eId) {

							e = e_Test;
							elm_Nodes = e->getNodeList();							
							elementNodesId.at(0) = elm_Nodes.at(0)->getId();
							elementNodesId.at(1) = elm_Nodes.at(1)->getId();

							for (size_t k = 0; k < loadValues.size(); k++) {
								for (size_t m = 0; m < loadValues.at(0).size(); m++) {
									loadValues.at(k).at(m) = atof(words.at(lvCount).c_str());
									lvCount++;
								}
							}
							
							vector<double> n1ExtLoads(this->dof);
							for (int l = 0; l < 3; l++) 
								n1ExtLoads.at(l) = (loadValues.at(0)).at(l) + ((Node*) elm_Nodes.at(0))->getExternalGlobalLoads().at(l);
							((Node*) elm_Nodes.at(0))->setExternalGlobalLoads(n1ExtLoads);
							
							vector<double> n2ExtLoads(this->dof);
							for (int l = 0; l < 3; l++)
								n2ExtLoads.at(l) = (loadValues.at(1)).at(l) + ((Node*) elm_Nodes.at(1))->getExternalGlobalLoads().at(l);
							((Node*) elm_Nodes.at(1))->setExternalGlobalLoads(n2ExtLoads);
							
							vector<double> n1ExtLocalLoads(this->dof);
							for (int l = 0; l < 3; l++) 
								n1ExtLocalLoads.at(l) = (-(loadValues.at(0).at(l)));
							((Node*) elm_Nodes.at(0))->setExternalLocalLoads(n1ExtLocalLoads);
							
							vector<double> n2ExtLocalLoads(this->dof);
							for (int l = 0; l < 3; l++)
								n2ExtLocalLoads.at(l) = (-(loadValues.at(1).at(l)));
							((Node*) elm_Nodes.at(1))->setExternalLocalLoads(n2ExtLocalLoads);
							
							e->setExternalLocalLoads_Test(loadValues);
							j = eList.size() - 1;
						}
					}
				}
			}
		
		}// fim while

		file->close();
		delete file;
	}// Fim verificar se arquivo está aberto
	else {
		cout << "Não foi possivel abrir o arquivo!" << endl;
	}
	
}


Input::~Input()
{
}

Model * Input::getModel()
{
	return this->model;
}

string Input::readString(ifstream* file, string line)
{
	getline(*file, line);//Next Line
	string anotherString = line;	
	anotherString = this->ignoreParenthesis(anotherString);	
	anotherString = this->ignoreEmptySpaces(anotherString);	

	return anotherString;
}

string Input::ignoreParenthesis(string txt)
{
	int i = 0;
	string word = txt;

	while (i < word.size())
	{
		if (word[i] == '(' || word[i] == ')')
		{
			word.erase(i, 1);
		}
		i++;
	}

	return word;
}

string Input::ignoreEmptySpaces(string txt)
{
	int i = 0;
	string word = txt;

	while (i < word.size())
	{
		if (word[i] == ' ')
		{
			word.erase(i, 1);
		}
		i++;
	}

	return word;
}

string Input::getFirstWord(string txt)
{

	int i = 0;
	string word = txt;
	string aux = "";

	while (i < word.size())
	{
		if (word[i] == ' ')		
			break;
		
		aux.push_back(word[i]);
		i++;
	}

	return aux;
}

vector<string> Input::getWordsOfLine(string txt, char separator)
{
	vector<string> words;
	string text = txt;
	string word = "";
	size_t i = 0;

	while(i < text.size()){	
		
		bool flag = false;
		int count = i;
		
		while ((count < text.size()) && !(text[count] == separator)) {
			flag = true;		
			word.push_back(text[count]);	
			count++;
		}		
		if (flag) {
			words.push_back(word);
			word = "";
		}

		i = flag ? count : (++i);	// think about it	
	}	
	
	return words;
}

vector<string> Input::getWordsOfLine(string txt, char separator, char separator_2)
{
	vector<string> words;
	string text = txt;
	string word = "";
	size_t i = 0;

	while (i < text.size()) {

		bool flag = false;
		int count = i;

		while ((count < text.size()) && (!(text[count] == separator) && !(text[count] == separator_2) )) {
			flag = true;
			word.push_back(text[count]);
			count++;
		}
		if (flag) {
			words.push_back(word);
			word = "";
		}

		i = flag ? count : (++i);	// think about it	
	}

	return words;
}

vector<string> Input::split(string text, char delimiter)
{
	vector<string> internal;
	stringstream ss(text); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);		
	}

	return internal;
}
