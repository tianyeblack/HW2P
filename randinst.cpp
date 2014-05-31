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

int** randInstGen(unsigned int chemicals, unsigned int containers, double yes_rate, double no_rate) {
	srand(time(0));
	if (yes_rate < 0 || no_rate < 0 || yes_rate + no_rate > 1) return 0;
	int** cstr_tab = new int*[chemicals];
	for (unsigned int i = 0; i < chemicals; i++) cstr_tab[i] = new int[chemicals];
	
	unsigned int spot = chemicals * (chemicals - 1) / 2;
	unsigned int yes = (unsigned int) (spot * yes_rate);
	unsigned int no = (unsigned int) (spot * no_rate);
	int *spots = new int[spot];
	unsigned int i = 0;
	while (i < spot) spots[i++] = 0;
	i = 0;
	while (i < yes) {
		unsigned int r = rand() % spot;
		if (spots[r] != 1) {
			spots[r] = 1;
			i++;
		}
	}
	i = 0;
	while (i < no) {
		unsigned int r = rand() % spot;
		if (spots[r] != 1 && spots[r] != -1) {
			spots[r] = -1;
			i++;
		}
	}
	i = 0;
	unsigned int j = 0;
	while (i < chemicals - 1) {
		while (j < (chemicals - 1 + chemicals - 1 - i) * (i + 1) / 2) {
			cstr_tab[i][j - (chemicals - 1 + chemicals - 2 - i) * i / 2 + i + 1] = spots[j];
			j++;
		}
		i++;
	}
	return cstr_tab;
}

int main(int argc, char* argv[]) {
	unsigned int chemicals, containers;
	double p, yes_rate, no_rate;
	unsigned int max_flips;
	int opt = atoi(argv[1]);
	
	if (opt == 8) {
		chemicals = 16;
		containers = 2;
		no_rate = 0.02;
		yes_rate = 0.0;
		p = 0.5;
		max_flips = 100;
		ofstream ofile("question8.txt");
		if (!ofile.is_open()) cout << "I/O error" << endl;
		while (no_rate <= 0.2) {
			int reso = 0;
			int walk = 0;
			for (int i = 0; i < 50; i++) {
				int flips = 0;
				int** cstr_tab = randInstGen(chemicals, containers, yes_rate, no_rate);
				ProblemInstance* pi = new ProblemInstance(chemicals, containers, cstr_tab);
				if (true == pi->plResolution()) reso++;
				if (pi->walkSAT(p, max_flips, &flips)) walk++;
				delete[] cstr_tab;
				delete pi;
			}
			double sat_rate1 = (double) reso / 50;
			double sat_rate2 = (double) walk / 50;
			cout << no_rate << " " << sat_rate1 << " " << sat_rate2 << endl;
			ofile << no_rate << " " << sat_rate1 << " " << sat_rate2 << endl;
			no_rate += 0.02;
		}
		
	} else if (opt == 9) {
		chemicals = 16;
		containers = 2;
		no_rate = 0.05;
		yes_rate = 0.02;
		p = 0.5;
		max_flips = 1000;
		ofstream ofile("question9.txt");
		if (!ofile.is_open()) cout << "I/O error" << endl;
		while (yes_rate <= 0.2) {
			int sat = 0;
			for (int i = 0; i < 100; i++) {
				int flips;
				int** cstr_tab = randInstGen(chemicals, containers, yes_rate, no_rate);
				ProblemInstance pi(chemicals, containers, cstr_tab);
				if (pi.walkSAT(p, max_flips, &flips)) sat++;
				delete[] cstr_tab;
			}
			double sat_rate = (double) sat / 100;
			cout << yes_rate << " " << sat_rate << endl;
			ofile << yes_rate << " " << sat_rate << endl;
			yes_rate += 0.02;
		}
	} else if (opt == 10) {
		int i = 0;
		int flips_sum = 0;
		int avg = 0;
		int size = 0;
		int round = 25;
		yes_rate = 0.02;
		no_rate = 0.02;
		p = 0.5;
		max_flips = 1000;
		
		ofstream ofile("question10.txt");
		if (!ofile.is_open()) cout << "I/O error" << endl;
		
		chemicals = 16;
		containers = 2;
		flips_sum = 0;
		avg = 0;
		size = 0;
		for (int m = 0; m < round; m++) {
			i = 0;
			while (i < 20) {
				int flips = 0;
				int** cstr_tab = randInstGen(chemicals, containers, yes_rate, no_rate);
				ProblemInstance pi(chemicals, containers, cstr_tab);
				bool** slt = pi.walkSAT(p, max_flips, &flips);
				if (slt != 0) {
					size += pi.kbSize();
					i++;
				} else flips = max_flips;
				flips_sum += flips;
				delete[] slt;
				delete[] cstr_tab;
			}
		}
		avg = flips_sum / round;
		cout << (double) size / (20 * round * chemicals * containers) << " " << avg << endl;
		ofile << (double) size / (20 * round * chemicals * containers) << " " << avg << endl;
		
		chemicals = 24;
		containers = 3;
		i = 0;
		flips_sum = 0;
		avg = 0;
		size = 0;
		for (int m = 0; m < round; m++) {
			i = 0;
			while (i < 20) {
				int flips = 0;
				int** cstr_tab = randInstGen(chemicals, containers, yes_rate, no_rate);
				ProblemInstance pi(chemicals, containers, cstr_tab);
				bool** slt = pi.walkSAT(p, max_flips, &flips);
				if (slt != 0) {
					size += pi.kbSize();
					i++;
				} else flips = max_flips;
				flips_sum += flips;
				delete[] slt;
				delete[] cstr_tab;
			}
		}
		avg = flips_sum / round;
		cout << (double) size / (20 * round * chemicals * containers) << " " << avg << endl;
		ofile << (double) size / (20 * round * chemicals * containers) << " " << avg << endl;
		
		chemicals = 32;
		containers = 4;
		i = 0;
		flips_sum = 0;
		avg = 0;
		size = 0;
		for (int m = 0; m < round; m++) {
			i = 0;
			while (i < 20) {
				int flips = 0;
				int** cstr_tab = randInstGen(chemicals, containers, yes_rate, no_rate);
				ProblemInstance pi(chemicals, containers, cstr_tab);
				bool** slt = pi.walkSAT(p, max_flips, &flips);
				if (slt != 0) {
					size += pi.kbSize();
					i++;
				} else flips = max_flips;
				flips_sum += flips;
				delete[] slt;
				delete[] cstr_tab;
			}
		}
		avg = flips_sum / round;
		cout << (double) size / (20 * round * chemicals * containers) << " " << avg << endl;
		ofile << (double) size / (20 * round * chemicals * containers) << " " << avg << endl;
		
		chemicals = 40;
		containers = 5;
		i = 0;
		flips_sum = 0;
		avg = 0;
		size = 0;
		for (int m = 0; m < round; m++) {
			i = 0;
			while (i < 20) {
				int flips = 0;
				int** cstr_tab = randInstGen(chemicals, containers, yes_rate, no_rate);
				ProblemInstance pi(chemicals, containers, cstr_tab);
				bool** slt = pi.walkSAT(p, max_flips, &flips);
				if (slt != 0) {
					size += pi.kbSize();
					i++;
				} else flips = max_flips;
				flips_sum += flips;
				delete[] slt;
				delete[] cstr_tab;
			}
		}
		avg = flips_sum / round;
		cout << (double) size / (20 * round * chemicals * containers) << " " << avg << endl;
		ofile << (double) size / (20 * round * chemicals * containers) << " " << avg << endl;
		
		chemicals = 48;
		containers = 6;
		i = 0;
		flips_sum = 0;
		avg = 0;
		size = 0;
		for (int m = 0; m < round; m++) {
			i = 0;
			while (i < 20) {
				int flips = 0;
				int** cstr_tab = randInstGen(chemicals, containers, yes_rate, no_rate);
				ProblemInstance pi(chemicals, containers, cstr_tab);
				bool** slt = pi.walkSAT(p, max_flips, &flips);
				if (slt != 0) {
					size += pi.kbSize();
					i++;
				} else flips = max_flips;
				flips_sum += flips;
				delete[] slt;
				delete[] cstr_tab;
			}
		}
		avg = flips_sum / round;
		cout << (double) size / (20 * round * chemicals * containers) << " " << avg << endl;
		ofile << (double) size / (20 * round * chemicals * containers) << " " << avg << endl;
	} else {
	
	}
	return 0;
}
