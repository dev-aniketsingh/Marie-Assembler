/*
 * CREATED BY: SUBHAM SINGH & ANIKET K SINGH
 * DATE: 04/08/2019
 */

#include<iostream>
#include<string>
#include <cstdlib>
#include <fstream>
#include <map>
#include <sstream>
#include <iomanip>      

using namespace std;

//variables required
map <string, string> opcode;
string symbol[106];
string operand[100];
string insertF[100];
string address[100];
string code[100];
ifstream inFile;

//initilizes OpCodes 
void initializeOpCodes(std::map<string,string> &opcode) {
  opcode.insert(pair <string, string> ("JNS", "0"));
  opcode.insert(pair <string, string> ("LOAD", "1"));
  opcode.insert(pair <string, string> ("STORE", "2"));
  opcode.insert(pair <string, string> ("ADD", "3"));
  opcode.insert(pair <string, string> ("SUBT", "4"));
  opcode.insert(pair <string, string> ("INPUT", "5"));
  opcode.insert(pair <string, string> ("OUTPUT", "6"));
  opcode.insert(pair <string, string> ("HALT", "7"));
  opcode.insert(pair <string, string> ("SKIPCOND", "8"));
  opcode.insert(pair <string, string> ("JUMP", "9"));
  opcode.insert(pair <string, string> ("CLEAR", "A"));
  opcode.insert(pair <string, string> ("ADDI", "B"));
  opcode.insert(pair <string, string> ("JUMPI", "C"));
  opcode.insert(pair <string, string> ("LOADI", "D"));
  opcode.insert(pair <string, string> ("unused", "F"));
}

//Checks if the string has commands, returns true if it's a command
bool isCommand(string s) {
	bool flag = false;
	if(opcode.find(s) != opcode.end()) {
		flag = true;
	}
	else if(s == "DEC" || s == "HEX" || s == "ORG" || s == "dec" || s == "hex" || s == "org") {
		flag = true;
	}
	else {
		flag = false;
	}
  return flag;
}

//Checks is the string has Symbol eg x, y, z, (Symbols includes "x," ,"y,")
bool isSymbol(string s, int &index) {
	bool flag = false;
	for(int i = 0; i < 100; i++) {
		while(symbol[i] != "" && i < 100) {
			if(symbol[i] == s + ",") {
				index = i;
				flag = true;
			}
			i++;
		}
	}
	return flag;
}

//First Pass
void firstPass(string s, char *outfn) {
	int i = 0;
	int counter;
	string temp;
	ofstream outFile;
	address[0] = "000";
	inFile.open(s);
    if (!inFile) {
       cout << "Error: cannot open input file" << endl;
     return;
    }
    outFile.open(outfn);
  	if (!outFile) {
    	inFile.close();
    	cout << "Error: cannot create output file" << endl;
     return;
    }
    while(inFile) {
    	if(insertF[i] == "") {
    		inFile >> insertF[i];
    	}
    	if(!isCommand(insertF[i])){
    		symbol[i] = insertF[i];
    		inFile >> insertF[i];
    	}
    	inFile >> operand[i];
    	if(insertF[i] == "ORG" || insertF[i] == "org") {
    		address[1] = operand[i];
    		counter = stoi(operand[i]);
    	}
    	if(insertF[i] == "HALT" || insertF[i] == "OUTPUT" || insertF[i] == "INPUT" || insertF[i] == "CLEAR" || insertF[i] == "halt" || insertF[i] == "output" || insertF[i] == "input" || insertF[i] == "clear") {
    		temp = operand[i];
    		int j = i + 1;
    		if(!isCommand(temp)) {
    			symbol[j] = temp;
    		}
    		else {
    			insertF[j] = temp; 
    		}
    		operand[i] = "";
    	}
    	i++;
    	counter++;
    	address[i+1] = to_string(counter);
    }
    outFile << "First pass assembly \n";
    outFile << "-------------------------------- \n";
    outFile << "Symbol Table \n";
    outFile << "-------------------------------- \n";
    outFile << setw(10) << "Symbol" << setw(10) << "   Location" << "\n";
    outFile << "-------------------------------- \n";
    for(int i = 0; i < 100; i++) {
    	while(symbol[i] != "" && i < 100) {
    		outFile << setw(10) << symbol[i] <<  setw(10) << "   ||   "  << setw(10) << address[i] << "\n";
    		i++;
    	}
    }
    outFile << "-------------------------------- \n";
    outFile << endl;
	outFile.close();
}

//Second Pass
void secondPass(string s,char *outfn) {
  int index =0;
  int dec;
  string hexString = "";
  ofstream outFile;
  outFile.open(outfn);
  if (!outFile) {
	   cout << "Error: cannot create output file" << endl;
    return;
  }
  for(int i = 0; i < 100; i++){
    while(insertF[i] != "" && i < 100) {
      if(insertF[i] == "ORG" || insertF[i] == "org")  {
      	code[i] = "0" + operand[i];
      } 	
      else if(insertF[i] == "DEC" || insertF[i] == "dec") {
		  stringstream ss;
		  ss.fill('0');
		  ss.width(4);
		  ss << hex << stoi(operand[i]);
		  code[i] = ss.str();
	  }
      else if (insertF[i] == "HEX" || insertF[i] == "hex")  {
      	  code[i] = operand[i];
      }
      else if (insertF[i] == "HALT" || insertF[i] == "halt") {
      	  code[i] = "7000";
      }
      else if (insertF[i] == "CLEAR" || insertF[i] == "clear") {
      	  code[i] = "A000";
      }	
      else if (insertF[i] == "OUTPUT" || insertF[i] == "output") {
      	  code[i] = "6000"; 
      }	
      else {
    		auto iterator = opcode.find(insertF[i]);
    		if(isSymbol(operand[i],index)) {
    		  	code[i] = iterator->second + address[index];
    		}
    		else {
    	  		code[i] = iterator->second + operand[i];
    	    }
      }
      i++;
    }
  }
    outFile << "Second pass assembly" << endl;
    outFile << "--------------------" << endl;
    outFile << endl;
    outFile << setw(10)<<  "Address" << setw(10) << "Symbol" << setw(10) << "Code " << setw(10) << "   Instruction" << setw(10) << "Operand" << endl;
    outFile << "-------------------------------------------------------------------" << endl;
    for(int i = 0; i < 100; i++) {
      outFile << setw(10) << address[i] << setw(10) << symbol[i] << setw(10) << code[i] << setw(10) << insertF[i] << setw(10) << operand[i] << endl;
      if(insertF[i] == "") 
      	break;
    }
    outFile << "-------------------------------------------------------------------" << endl;
    outFile << endl;
    inFile.close();
	  outFile.close();
}
//to run 
// "./assembler filename.mas firstPasFilename.mas secondPasFileName.mas" 
//Here file name the file being opened should be in the folder, firstPasFilename and secondPasFile name can be anything 

int main(int argc, char* argv[]) {
	  string infile;
  	infile = argv[1];
  	initializeOpCodes(opcode);
  	firstPass(infile,argv[2]);
  	secondPass(infile,argv[3]);
  	return 0;
}