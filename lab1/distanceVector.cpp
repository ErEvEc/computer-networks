#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <iostream>

using namespace std;

vector<vector<int> > buildGraph(string fileName){

	   ifstream graphFile (fileName);

		vector<vector<int> > edges;
		string line;

      while (getline(graphFile, line, '\n')) {

			istringstream iss(line);
         vector<int> neighbors;
         string neighbor;
         while (iss >> neighbor) {
         	// cout<< neighbor <<",";
            if(neighbor=="inf") {
            	neighbors.push_back(INT_MAX);
            } else if(neighbor.substr(0, 1)=="["){
            	neighbor = neighbor.substr(1, neighbor.size()-2);
            	stringstream multi(neighbor);
            	int sum = 0;
            	string cost;
            	getline(multi, cost, ',');
            	sum += stoi(cost);
            	getline(multi, cost, ',');
            	sum += stoi(cost);
            	getline(multi, cost);
            	sum += stoi(cost);
            	neighbors.push_back(sum);
            }else{
            	neighbors.push_back(stoi(neighbor));
            }
        }

        
        edges.push_back(neighbors);
      }

      return edges;

}

vector<int> getPath(vector<int> preArray, int current) {

	vector<int> path;
	while(current != 0) {
		if(current !=0) path.insert(path.begin(), current);
		current = preArray[current];
	}
	return path;
}

void outputDis(int n, vector<int> preArray, vector<int> inDisArray, int iter){

	ofstream output;
	output.open ("output-N" + to_string(n) + "-hw.txt", ios::out | ios::app);
   for (int i=0; i<n; i++) output << inDisArray[i] << ",";
   output << endl;

	for(int i=0; i<n; i++){
		std::vector<int> path = getPath(preArray, i);
		output<< 0;
		for(int node: path) output<< "->" << node;
		output<<endl;

	}

	output << "Iteration:" << iter << endl;


   output.close();

}

void outputNeg(int n, vector<vector<int> > negCycles){
	ofstream output;
	output.open ("output-N" + to_string(n) + "-_neg-hw.txt", ios::out | ios::app);
	output << "Negative Loop Detected" << endl;
	for(vector<int> cycle: negCycles){
		output << 1;
		for(int node: cycle) output << "->" << node+1;
		output << endl;
	}
	output.close();

}

bool BellmanFord(vector<vector<int> > edges){
	int n = edges[0].size();
	
	vector<int> preArray(n, -1);
	vector<int> inDisArray(n, INT_MAX);

	int iter = 1;
	bool hasNeg = false;
	bool finishOutput = false;
	bool change = false;

	vector<vector<int> > negCycles;

	inDisArray[0] = 0;
	preArray[0] = 0;
	// cout<<n;
	for(int m=0; m<n; m++){
		change = false;
		iter++;


	for(int i=1; i<n; i++) {


		for(int j=0; j<n; j++){
			if(j!=i && edges[j][i] != INT_MAX && inDisArray[j]!=INT_MAX){
				if(inDisArray[j] + edges[j][i] <= inDisArray[i]) {
					if(inDisArray[j] + edges[j][i] < inDisArray[i]) change = true;
					inDisArray[i] = inDisArray[j] + edges[j][i];
					preArray[i] = j;

					if(edges[i][0] != INT_MAX){
						if(inDisArray[i] + edges[i][0] < 0) {
							hasNeg = true;
							std::vector<int> negPath = getPath(preArray, i);
							negPath.push_back(0);
							negCycles.push_back(negPath);

						}
					}

				}
			}
		}
	}
		if((!change || (m==n-1)) && !finishOutput && !hasNeg ) {
			bool reachall = true;
			for(int k=0; k<n; k++){
				if(preArray[k]==-1) reachall = false;
			}
			if(reachall){
				outputDis(n, preArray, inDisArray, iter);
				finishOutput = true;
			}
			
		}

	
}

	if(hasNeg) outputNeg(n, negCycles);

	return hasNeg;



}

int main(int argc, char * argv[]) {

	if ( argc != 2 ) 
   	cout<<"usage: "<< argv[0] <<" <filename>\n";
  	else {
   	ifstream graphFile;
   	graphFile.open(argv[1]);
    	if ( !graphFile)
      	cout << "Could not open file\n";
    	else {
      	BellmanFord(buildGraph(argv[1]));
    	}
  }
   return 0;
}