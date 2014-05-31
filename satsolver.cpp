#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "probleminstance.h"

using namespace std;
int stringToInt(const string &s) {
	stringstream ss(s);
	int result;
	return ss >> result ? result : 0;
}

int main(int argc, char* argv[]) {
	ifstream ifile(argv[1]);
	ofstream ofile(argv[2]);
	unsigned int chemicals, containers;
	double p;
	unsigned int max_flips;
	/*
	int** tab = randInstGen(4, 2, 0.33, 0.33);
	cout << (tab ? "Succeed" : "Failed") << endl;
	*/
	if (argc != 5) {
		cout << "Not enough arguments" << endl;
		return -1;
	}
	if (ifile.is_open() && ofile.is_open()) {
		string firstline;
		getline(ifile, firstline);
		int blank = firstline.find(" ", 0);
		chemicals = stringToInt(firstline.substr(0, blank));
		containers = stringToInt(firstline.substr(++blank));
//		cout << chemicals << " " << containers << endl;
	} else {
		cout << "File I/O error" << endl;
		return -1;
	}
	
	p = atof(argv[3]);
	max_flips = (unsigned int) atoi(argv[4]);
	if (p < 0 || p > 1 || max_flips == 0) {
		cout << "Invalid p or max_flips" << endl;
		return -1;
	}
	int** csrt_tab = new int*[chemicals];
	for (unsigned int i = 0; i < chemicals; i++) csrt_tab[i] = new int[chemicals];
	
	for (unsigned int i = 0; i < chemicals; i++) {
		string o_line;
		getline(ifile, o_line);
		unsigned int j = 0, k = 0, l = o_line.find(" ", 0);
		while (j < chemicals - 1 && l != string :: npos) {
			csrt_tab[i][j] = stringToInt(o_line.substr(k, l - k));
			k = ++l;
			l = o_line.find(" ", l);
			j++;
		}
		csrt_tab[i][j] = stringToInt(o_line.substr(k));
	}
	/*
	for (int i = 0; i < chemicals; i++) {
		for (int j = 0; j < chemicals; j++) {
			cout << csrt_tab[i][j];
		}
		cout << endl;
	}
	*/
	ProblemInstance pi(chemicals, containers, csrt_tab);
	if (true == pi.plResolution()) {
		int flips;
		bool** slt = pi.walkSAT(p, max_flips, &flips);
		if (slt) {
			ofile << 1 << endl;
			for (unsigned int j = 0; j < containers; j++) {
				unsigned int i = 0;
				while (i < chemicals - 1) {
					ofile << slt[i][j] << " ";
					i++;
				}
				ofile << slt[i][j] << endl;
			}
		} else ofile << 0 << endl;
	} else ofile << 0 << endl;
	return 0;
}
