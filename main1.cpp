#include"class.h"
#include <ctime>
#include <omp.h>

void TestInheritance(){
	time_t end0, start0, end1, start1, end2, start2;
	time(&start0);
	vector<CFabricVector*> fabric;
	vector<CComplexVector*> w1,w2;
	fabric.push_back(new CFabricVector1);
	fabric.push_back(new CFabricVector2);
	ifstream in("in2.txt"); 
    string str;
	while(getline(in,str)){
		int Type; 
        string outfile; 
        vector<ComplexNumber> vec1, vec2;
		stringstream ss(str);
		ss >> Type >> outfile;
		for(ComplexNumber tmp;ss >> tmp;){
			vec1.push_back(tmp);
			vec2.push_back(tmp);
		}
		CComplexVector *t1  = fabric[Type-1]->Create();
		CComplexVector *t2 = fabric[Type-1]->Create();
		*t1 = vec1;
		*t2 = vec2;
		//t1->show(outfile);
		//t2->show(outfile);
		w1.push_back(t1);
		w2.push_back(t2);
	}

	ComplexNumber tmp;

	time(&end0);
	cout << "Time for Input: " <<difftime(end0,start0) << endl << "Number of vectors : " <<  w1.size() << endl;
	time(&start1);
	for(int j = 0;j < 3000;j++){
		for(size_t i = 0; i < w1.size();i++){
			tmp = *w1[i]* *w1[i];
			w1[i]->Inc();
			w1[i]->Dec();
		}
	}
	time(&end1);
	cout << "Time without OpenMP: " << difftime(end1,start1) << endl;


	time(&start2);
	omp_set_num_threads(12);
	#pragma omp parallel for private(tmp)
	for(int j=0;j<3000;j++){
		for(size_t i=0; i<w2.size();i++){
			tmp=*w2[i]* *w2[i];
			w2[i]->Inc();
			w2[i]->Dec();
		}
	}
	time(&end2);
	cout << "Time with OpenMP: " <<difftime(end2,start2) << endl;

	ofstream out;
	out.open("out1.txt");
	for(size_t i=0;i<w1.size();i++){
		out << "{";
		for(size_t j=0; j<w1[i]->Size();j++){
			out << w1[i]->operator[](j) << " ";
		}
		out << "}" << endl;
	}
	out.close();
	out.open("out2.txt");
	for(size_t i=0;i<w2.size();i++){
		out << "{";
		for(size_t j=0; j<w2[i]->Size();j++){
			out << w2[i]->operator[](j) << " ";
		}
		out << "}" << endl;
	}
	out.close();
	for(size_t i=0;i<fabric.size();i++){
		delete fabric[i];
	}
	for(size_t i=0;i<w1.size();i++){
		delete w1[i];
	}
	for(size_t i=0;i<w2.size();i++){
		delete w2[i];
	}
}

int main() {
	try{
		TestInheritance();
	}catch(exception &err){
		cout << err.what() << endl;
	}
	return 0;
}