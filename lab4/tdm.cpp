//
//  tdm.cpp
//  
//
//  Created by 干宇 on 6/30/19.
//

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <math.h>

using namespace std;



vector<map<int, string> > readData(string fileName){
    ifstream inputFile (fileName);
    
    vector<map<int, string>> data;
    string line;
    int time = 0;
    int capacity = 0;
    
    while (getline(inputFile, line, '\n')) {
        if(line.find(':') != string::npos){
            int seperator = line.find(':');
            line = line.substr(seperator+1);
            
            map<int, string> channel;
            while(line.find(',') != string::npos){
                int comma = line.find(',');
                string slot = line.substr(0, comma);
                istringstream iss(slot);
                string startTime, endTime, content;
                iss >> startTime;
                iss >> endTime;
                iss >> content;
                channel.insert(pair<int, string>(stoi(startTime), content));
		cout << "pair: " << stoi(startTime) << ", " << content << endl;
                time = max(time, stoi(endTime));
                capacity++;
                line = line.substr(comma+1);
            }
            istringstream iss(line);
            string startTime, endTime, content;
            iss >> startTime;
            iss >> endTime;
            iss >> content;
            channel.insert(pair<int, string>(stoi(startTime), content));
		cout << "pair: " << stoi(startTime) << ", " << content << endl;
            time = max(time, stoi(endTime));
            capacity++;
            
            data.push_back(channel);
        }
        
    }
    map<int, string> count = {{time, "time"}, {capacity, "capacity"}};
    data.insert(data.begin(), count);
    
    return data;
}

void tdm(vector<map<int, string>> data) {
    
    queue <pair<int, string>> buffer;
    
    map<int, string> count = data[0];
    float time, capacity;
    for (map<int, string>::iterator it = count.begin(); it != count.end(); ++it ){
        if (it->second == "time")
            time = it->first;
        if (it -> second =="capacity")
            capacity = it->first;
    }
    int slotNumber = ceil(capacity/time);
    cout << "slotN: " << slotNumber <<endl;
	cout << "time: " << time << endl;
cout << "capacity: " << capacity << endl;
    
    int curTime = 1;
	int n = 0;
    
    ofstream output;
    output.open ("tmd_output.txt", ios::out | ios::app);
    
    while(curTime<=(int)time || !buffer.empty()){
// cout << "curTime: " << curTime << endl;
        for(int i=1; i<data.size(); i++) {
            map<int, string> channel = data[i];
            if(channel.find(curTime-1) != channel.end()){
                buffer.push(pair<int, string>(i-1, channel.find(curTime-1)->second));
            }
        }
        
        if(!buffer.empty()){
            output<< "SF" <<endl;

            for(int i=0; i<slotNumber; i++){
                if(!buffer.empty()){
                    pair<int, string> subframe = buffer.front();
			n++;
                    buffer.pop();
                    output<< subframe.first << ", " << curTime + i*(1.0/slotNumber) << " " << curTime + (i+1)*(1.0/slotNumber) << " " << subframe.second << endl;
                }
            }
            output << "EF" << endl;
        }
        curTime++;
    }
    
    output.close();
    
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
            tdm(readData(argv[1]));
        }
    }
    return 0;
}
