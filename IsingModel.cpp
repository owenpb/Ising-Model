

#include "stdafx.h"
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       
#include <iostream>
#include <math.h> 
#include <vector>
#include <fstream>
using namespace std;

int main()
{
	srand((unsigned int)time(NULL)); //Ensures different sequence of random numbers each time program is run

	//Define lattice size, e.g. 8x8 lattice
	int N = 4; //Lattice width or length
	int spin[4][4]; //Create a 2D array of size 8x8, to store spin values
	int J = 1; //Interaction parameter

	int Mdrop = 1000; //Set initial number of sweeps, to equilibriate 

	//Set random starting configuration (each spin +1 or -1)
	for (int i = 0; i <= (N - 1); i++) {

		for (int j = 0; j <= (N - 1); j++) {

			double r = ((double)rand() / (RAND_MAX));
			if (r >= 0.5)
			{
				spin[i][j] = 1;  //If random number is greater than or equal to 0.5, have spin-up at site (i,j)
			}
			else {
				spin[i][j] = -1; //Else have spin-down at site (i,j)
			}

			//cout << spin[i][j] << " "; //Can display a grid of the spins

		}

		//cout << "\n"; //Can display a grid of the spins
	}

	//Set temperature increment, and minimum and maximum temperatures
	double Tmin = 0.5;
	double Tmax = 5.0;
	double Tinc = 0.1;
	vector<double> Temperatures; //for T values
	vector<double> AbsMagnetization; //for <|M|> values
	vector<double> Magnetization; //for <M> values
	vector<long double> MagSquared; //for <M^2> values
	vector<long double> MagFourth; //for <M^4> values
	vector<double> Energy; //for <E> values
	vector<double> EneSquared; //for <E^2> values
	vector<double> Capacities; //for C values
	vector<double> Susceptibilities; //for X values
	vector<double> Binder; //for B values
	



	for (double T = Tmin; T <= Tmax; T = T + Tinc) {

		for (int m = 1; m <= Mdrop; m++) {

			for (int i = 0; i <= (N - 1); i++) {

				for (int j = 0; j <= (N - 1); j++) {

					//Spin of nearest neighbor to the right
					int right;
					if (j == (N - 1)) {  //e.g. if j = 7 for 8x8 grid
						right = spin[i][0]; //Periodic BC for sites on right edge of grid
					}
					else {
						right = spin[i][j + 1];
					}

					//Spin of nearest neighbor below
					int down;
					if (i == (N - 1)) {  //e.g. if i=7 for 8x8 grid
						down = spin[0][j]; //Periodic BC for sites on bottom edge of grid
					}
					else {
						down = spin[i + 1][j];
					}

					//Spin of nearest neighbor to the left
					int left;
					if (j == 0) {
						left = spin[i][N - 1]; //Periodic BC for sites on left edge of grid
					}
					else {
						left = spin[i][j - 1];
					}

					//Spin of nearest neighbor above
					int up;
					if (i == 0) {
						up = spin[N - 1][j]; //Periodic BC for sites on top edge of grid
					}
					else {
						up = spin[i - 1][j];
					}

					int deltaE = 2 * J*spin[i][j] * (right + down + left + up); //Energy required to flip spin

					double r = ((double)rand() / (RAND_MAX)); //r is a random number between 0 and 1
					if (r < exp((-1 * deltaE) / (T))) {
						spin[i][j] = spin[i][j] * (-1); //if random number less than e^(-deltaE/T), flip. If deltaE is negative, this flips with probability 1.
					}

				} //Finish loop over j

			}//Finish loop over i

			//Can print out each configuration with each of the Mdrop sweeps
			//cout << m << ":\n";
			//for (int a = 0; a <= (N - 1); a++) {
				//for (int b = 0; b <= (N - 1); b++) {
					//cout << spin[a][b] << " ";
				//}
				//cout << "\n";
			//}
			//cout << "\n";

		} //Finish performing Mdrop initial sweeps


		//Now perform Mrun sweeps. After each run, calculate the magnetization, and the Energy

		double Mrun = 50000;
		int AbsMagTot = 0; //The sum of the absolute magnetizations obtained after each sweep
		int MagTot = 0; //The sum of the magnetizations obtained after each sweep
		unsigned long long MagSquaredTot = 0; //The sum of the squared magnetizations obtained after each sweep
		unsigned long long MagFourthTot = 0; //The sum of the M^4 values obtained after each sweep
		double EneTot = 0; //The sum of the energies obtained after each sweep
		double EneSquaredTot = 0; //The sum of the squared energies obtained after each sweep

		for (int s = 1; s <= Mrun; s++) {

			int M = 0; //Set initial count of magnetization for this sweep to be zero
			double E = 0; //Set initial count of energy for this sweep to be zero

			for (int i = 0; i <= (N - 1); i++) {

				for (int j = 0; j <= (N - 1); j++) {

					//Spin of nearest neighbor to the right
					int right;
					if (j == (N - 1)) {  //e.g. if j = 7 for 8x8 grid
						right = spin[i][0]; //Periodic BC for sites on right edge of grid
					}
					else {
						right = spin[i][j + 1];
					}

					//Spin of nearest neighbor below
					int down;
					if (i == (N - 1)) {  //e.g. if i=7 for 8x8 grid
						down = spin[0][j]; //Periodic BC for sites on bottom edge of grid
					}
					else {
						down = spin[i + 1][j];
					}

					//Spin of nearest neighbor to the left
					int left;
					if (j == 0) {
						left = spin[i][N - 1]; //Periodic BC for sites on left edge of grid
					}
					else {
						left = spin[i][j - 1];
					}

					//Spin of nearest neighbor above
					int up;
					if (i == 0) {
						up = spin[N - 1][j]; //Periodic BC for sites on top edge of grid
					}
					else {
						up = spin[i - 1][j];
					}

					int deltaE = 2 * J*spin[i][j] * (right + down + left + up); //Energy required to flip spin

					double r = ((double)rand() / (RAND_MAX)); //r is a random number between 0 and 1
					if (r < exp((-1 * deltaE) / (T))) {
						spin[i][j] = spin[i][j] * (-1); //if random number less than e^(-deltaE/T), flip. If deltaE is negative, this flips with probability 1.
					}

					M = M + spin[i][j]; //add spin value to the total count of magnetization

				} //Finish loop over j

			}//Finish loop over i

			//Now after the sweep, increase the sum of measured M values:
			AbsMagTot = AbsMagTot + abs(M);
			MagTot = MagTot + M;
			MagSquaredTot = MagSquaredTot + pow(M,2);
			MagFourthTot = MagFourthTot + pow(M,4);

			for (int i = 0; i <= (N - 1); i++) {
				for (int j = 0; j <= (N - 1); j++) {
					//Spin of nearest neighbor to the right
					int right;
					if (j == (N - 1)) {  //e.g. if j = 7 for 8x8 grid
						right = spin[i][0]; //Periodic BC for sites on right edge of grid
					}
					else {
						right = spin[i][j + 1];
					}
					//Spin of nearest neighbor below
					int down;
					if (i == (N - 1)) {  //e.g. if i=7 for 8x8 grid
						down = spin[0][j]; //Periodic BC for sites on bottom edge of grid
					}
					else {
						down = spin[i + 1][j];
					}
					//Spin of nearest neighbor to the left
					int left;
					if (j == 0) {
						left = spin[i][N - 1]; //Periodic BC for sites on left edge of grid
					}
					else {
						left = spin[i][j - 1];
					}
					//Spin of nearest neighbor above
					int up;
					if (i == 0) {
						up = spin[N - 1][j]; //Periodic BC for sites on top edge of grid
					}
					else {
						up = spin[i - 1][j];
					}
					E = E - 0.5*J*spin[i][j]*(up + right + down + left);
				}
			}
			EneTot = EneTot + E;
			EneSquaredTot = EneSquaredTot + (E*E);

			//cout << "sweep: " << s << "," << M << "," << pow(M,2) << "," << pow(M,4) << "," << MagTot << "," << MagSquaredTot << "," << MagFourthTot << "\n";
			//cout << "sweep: " << s << " " << "Temp: " << T << " " << "Mag: " << M << " " << "MagTot: " << MagTot << " " << "Abs Mag: " << abs(M) << " " << "Abs Mag Tot: " << AbsMagTot << "\n";

		} //Finish Mrun sweeps

		//Calculate average absolute value of magnetization, <|M|>:
		double AbsMavg = AbsMagTot / double(Mrun);
		//cout << "AbsMavg: " << AbsMavg << "\n";
		//Calculate average abs magnetization per site, <|M|>/N^2:
		double AbsMavgsite = AbsMavg / pow(N, 2);
		//cout << "AbsMavgsite: " << AbsMavgsite << "\n";

		//Calculate average value of magnetization, <M>:
		double Mavg = MagTot / double(Mrun);
		//Calculate average magnetization per site, <M>/N^2:
		double Mavgsite = Mavg / pow(N, 2);

		//Calculate average value of squared magnetization , <M^2>:
		double MSquaredavg = MagSquaredTot / double(Mrun);
		//Calculate average squared magnetization per site, <M^2>/N^2:
		double MSquaredavgsite = MSquaredavg / pow(N, 2);

		//Calculate average value of M^4 , <M^4>:
		long double MFourthavg = MagFourthTot / double(Mrun);
		//Calculate average magnetization^4 per site, <M^4>/N^2:
		long double MFourthavgsite = MFourthavg / pow(N, 2);

		//Calculate average energy <E>:
		double Eavg = EneTot / double(Mrun);
		//Calculate average energy per spin, <E>/N^2:
		double Eavgsite = Eavg / pow(N, 2);

		//Calculate average squared energy <E^2>:
		double ESquaredavg = EneSquaredTot / double(Mrun);
		//Calculate average energy per spin, <E>/N^2:
		double ESquaredavgsite = ESquaredavg / pow(N, 2);

		//Calculate Heat Capacity: C = [<E^2> - <E>^2]/T^2
		double C = (ESquaredavg - (Eavg*Eavg)) / (T*T);
		//Calculate Heat Capacity per spin, C/N^2:
		double Csite = C / pow(N, 2);

		//Calculate Magnetic Susceptibility: X = [<M^2> - <|M|>^2]/T
		double X = (MSquaredavg - pow(AbsMavg, 2)) / (T);
		//Calculate Magnetic Susceptibility per spin, X/N^2:
		double Xsite = X / pow(N, 2);

		//Calculate Binder ratio: B = 1.5*(1-(<M^4>/(3*(<M^2>^2))))
		double B = 1.5*(1 - ((MFourthavg) / (3*(MSquaredavg*MSquaredavg))));

		//cout << "Avg M^4: " << MFourthavg << "," << "Avg M^4 per site: " << MFourthavgsite << "\n";
		//cout << "Avg M^2: " << MSquaredavg << "," << "Avg M^2 per site: " << MSquaredavgsite << "\n";


		Temperatures.push_back(T);
		AbsMagnetization.push_back(AbsMavgsite);
		Magnetization.push_back(Mavgsite);
		MagSquared.push_back(MSquaredavgsite);
		MagFourth.push_back(MFourthavgsite);
		Energy.push_back(Eavgsite);
		EneSquared.push_back(ESquaredavgsite);
		Capacities.push_back(Csite);
		Susceptibilities.push_back(Xsite);
		Binder.push_back(B);

		//cout << Mavgsite << "\n";
	}

	//for (int i = 0; i < Temperatures.size(); i++) {

		//cout << Temperatures[i] << ", " << MagFourth[i] << "\n";
	//}

	ofstream myfile;
	myfile.open("Fixed4x4MCvalues.txt");
	for (size_t m = 0; m < Temperatures.size(); m++)
	{
		myfile << Temperatures[m] << "," << AbsMagnetization[m] << "," << Magnetization[m] << "," << MagSquared[m] << "," << MagFourth[m] << "," << Energy[m] << "," << EneSquared[m] << "," << Capacities[m] << "," << Susceptibilities[m] << "," << Binder[m] << "\n";
	}
	myfile.close();

	//ofstream myfile;
	//myfile.open("MPowers.txt");
	//for (size_t m = 0; m < Temperatures.size(); m++)
	//{
	//myfile << Temperatures[m] << "," << Energy[m] << "," << Magnetization[m] << "," << MagSquared[m] << "," << MagFourth[m] << "\n";
	//}
	//myfile.close();


	return 0;
}

