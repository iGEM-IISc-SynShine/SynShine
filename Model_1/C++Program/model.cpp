#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//The constants
double timeStep;
double kd1, kd2;
double k1, k2, k3;
double alpha1, alpha2, beta1, beta2, gamma1, gamma2;
double b1, b2, sigma_b, delta_b, sigma_g, delta_g, sigma_r, delta_r, sigma1_b, delta1_b, sigma1_g, delta1_g, sigma1_r, delta1_r;
double mu1o, mu2o, pi1o, pi2o, pi3o;
double p1o, p2o;

//All variables except S. (Because their differentials have the same form)
class Var{
public:
	double value;
	//double a, b, x1, x2;
	Var(double initial){
		value = initial;

	}
	double diff(double a, double b, double x1, double x2){				//d(value)/dt = a*x1 - b*x2
		double dx = (a*x1 - b*x2)*timeStep;
		return dx;
	}
	void update(double dx){
		value = max(value+dx,0.0);
	}
};

class VarS{
public:
	double value;
	double a1, a2, b1, b2, c1, c2;

	VarS(double initial, double alpha1, double alpha2, double beta1, double beta2, double gamma1, double gamma2){
		value = initial;
		a1 = alpha1;
		a2 = alpha2;
		b1 = beta1;
		b2 = beta2;
		c1 = gamma1;
		c2 = gamma2;
		
	}
	double diff(double x1, double x2, double mu1, double mu2){
		double dS = (-a1*x1 - a2*x2 - b1*mu1*x1 - b2*mu2*x2 + c1*kd1*x1 + c2*kd2*x2)*timeStep;
		return dS;
	}
	void update(double dS){
		value = max(value+dS,0.0);
	}
};

double logistic(double sigma, double var, double delta){
	double value = (sigma*var*var)/(delta+var*var);
	return value;
}

double mu1(double s, double b){
	double value = max(s/(b1+s), 0.0) * (1-logistic(sigma_b, b, delta_b)) * mu1o;
	return value;
}

double mu2(double s, double r, double g){
	double value = max(s/(b2+s), 0.0) * (1-logistic(sigma_r, r, delta_r)+logistic(sigma_g, g, delta_g)) * mu2o;
	return value;
}

double pi1(double b){
	double value = (1-logistic(sigma1_b, b, delta1_b)) * pi1o;
	return value;
}

double pi2(double r, double g){
	double value = (1-logistic(sigma1_r, r, delta1_r)+logistic(sigma1_g, g, delta1_g)) * pi2o;
	return value;
}

double pi3(double b, double p1, double p2){
	double value = (1-exp(-p1/p1o)) * (1-exp(-p2/p2o)) * pi3o;
	return value;
}

int main(){

	//Input Format - timeStep, kd1, kd2, k1, k2, k3, alpha1, alpha2, beta1, beta2, gamma1, gamma2, b1, b2, sigma_b, delta_b, sigma_g, delta_g, sigma_r, delta_r, sigma1_b, delta1_b, sigma1_g, delta1_g, sigma1_r, delta1_r, mu1o, mu2o, pi1o, pi2o, pi3o, p1o, p2o
	//Input Format - x10, x20, p10, p20, p30, s0, b, g, r
	ifstream input;
	input.open("input.txt");
	string line;

	ofstream output;
	output.open("output.csv");

	vector<double> const_arr;
	vector<double> initial_arr;


	//Taking in the constants and the initial values of the variables from the input file
	int j = 0;
	string i;
	while(getline(input, line)){
		stringstream ss(line);

		while(getline(ss, i, ',')){
			if(j==0){
				const_arr.push_back(stod(i));
			}
			else{
				initial_arr.push_back(stod(i));
			}
		}
		j++;

	}

	input.close();



	//Setting up the constants
	double b, g, r;

	b = initial_arr[6];
	g = initial_arr[7];
	r = initial_arr[8];

	timeStep = const_arr[0];
	kd1 = const_arr[1];
	kd2 = const_arr[2];
	k1 = const_arr[3];
	k2 = const_arr[4];
	k3 = const_arr[5];
	alpha1 = const_arr[6];
	alpha2 = const_arr[7];
	beta1 = const_arr[8];
	beta2 = const_arr[9];
	gamma1 = const_arr[10];
	gamma2 = const_arr[11];
	b1 = const_arr[12];
	b2 = const_arr[13];
	sigma_b = const_arr[14];
	delta_b = const_arr[15];
	sigma_g = const_arr[16];
	delta_g = const_arr[17];
	sigma_r = const_arr[18];
	delta_r = const_arr[19];
	sigma1_b = const_arr[20];
	delta1_b = const_arr[21];
	sigma1_g = const_arr[22];
	delta1_g = const_arr[23];
	sigma1_r = const_arr[24];
	delta1_r = const_arr[25];
	mu1o = const_arr[26];
	mu2o = const_arr[27];
	pi1o = const_arr[28];
	pi2o = const_arr[29];
	pi3o = const_arr[30];
	p1o = const_arr[31];
	p2o = const_arr[32];

	//Setting up the variables
	Var x1(initial_arr[0]);
	Var x2(initial_arr[1]);
	Var p1(initial_arr[2]);
	Var p2(initial_arr[3]);
	Var p3(initial_arr[4]);
	VarS s(initial_arr[5], alpha1, alpha2, beta1, beta2, gamma1, gamma2);

	//The variables which store the differentials before changing the original value
	double diff_x1, diff_x2, diff_p1, diff_p2, diff_p3, diff_s;

	int iter = 1000000;

	//Euler Integration
	for(int i=0; i<iter; i++){
		output << (to_string(i*timeStep)+','+to_string(x1.value)+','+to_string(x2.value)+','+to_string(s.value)+','+to_string(p1.value)+','+to_string(p2.value)+','+to_string(p3.value)+','+to_string(x1.value/x2.value)+'\n');

		diff_x1 = x1.diff(mu1(s.value, b), kd1, x1.value, x1.value*(x1.value+x2.value));
		diff_x2 = x2.diff(mu2(s.value, r, g), kd2, x2.value, x2.value*(x1.value+x2.value));
		diff_p1 = p1.diff(pi1(b), k1, x1.value, p1.value);
		diff_p2 = p2.diff(pi2(r, g), k2, x2.value, p2.value);
		diff_p3 = p3.diff(pi3(b, p1.value, p2.value), k3, x1.value, p3.value);
		diff_s = s.diff(x1.value, x2.value, mu1(s.value, b), mu2(s.value, r, g));

		x1.update(diff_x1);
		x2.update(diff_x2);
		p1.update(diff_p1);
		p2.update(diff_p2);
		p3.update(diff_p3);
		s.update(diff_s);
	}

	output << (to_string(iter*timeStep)+','+to_string(x1.value)+','+to_string(x2.value)+','+to_string(s.value)+','+to_string(p1.value)+','+to_string(p2.value)+','+to_string(p3.value)+','+to_string(x1.value/x2.value)+'\n');
	output.close();

	//Output Format - t, x1, x2, s, p1, p2, p3, x1/x

	return 0;
}