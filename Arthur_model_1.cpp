// Arthur_model_1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <random>

using namespace std;

int a1 = 10, b1 = 5, c1=1, a2 = 1, b2 = 10, c2= 5, a3=5, b3=1, c3=10; //Natural preference
double slope; //r value
double ab1, ab2, ab3; //
double sh_1 = 1/3, sh_2 = 1/3, sh_3= 1/3; //initial market share
double p = 0.5; //probabiltiy drawing a R-agent
int n1 = 5, n2 = 5, n3=5, d, n = 15; //initial marekt condition 
int tech, maxIter, x;
double distribution[1000];
void genDistribution(int);

void enterInputs();
void adoptionDynamics();

int constant(double);
int decreasing(int, double, double, double);
int increasing(int, double, double, double);
int genBernoulli();

void writeResult(int);
void writeDistribution();

ofstream outputFile;
ofstream outputFile_dist;

void main()
{
	enterInputs();

	adoptionDynamics();

	for (int i = 0; i < 1000; i++)
	{
		genDistribution(i);
	}

	writeDistribution();
}

void enterInputs()
{
	maxIter = 1000;

	cout << "please enter the type of dynamics(1,2,3):";
	cin >> tech;

	switch (tech)
	{
		case 1:
			slope = 0; //constant returns -> r=0
			break;
		case 2:
			slope = -1; //decreasing returns -> r<0
			break;
		case 3:
			slope = 1; //increasing returns -> r>0
			break;
	}

	if (tech != 1) //ab1과 ab2 정의
	{
		ab1 = (double)-1 * (a1 - (b1 + c1)) / slope; 
		ab2 = (double)-1 * (a2 - (b2 + c2)) / slope;
		ab3 = (double)-1 * (a3 - (b3 + c3)) / slope;
	}

	//cout << "Please enter the maximum iterations:";
	//cin >> maxIter;

	outputFile.open("Arthur_dynamics_1.txt", ios::out);

	if (!outputFile)
	{
		cerr << " File could not be opened" << endl;
		exit(1);
	}

	//outputFile << "   Time   R-agent   S-agent   Share of A   Share of B" << endl;
	outputFile.close();
}

void genDistribution(int i)
{
	//initializing equation
	n1 = 5, n2 = 5, n = 10;
	n = n1 + n2; //시장 내 총 사용자 수
	d = n1 - n2; //기술1과 기술2의 사용자수의 차이

	if (tech != 1) //ab1과 ab2 정의
	{
		ab1 = (double)-1 * (a1 - b1) / slope;
		ab2 = (double)-1 * (a2 - b2) / slope;
	}

	sh_1 = (double)n1 / n; //기술1의 점유율
	sh_2 = 1 - sh_1; //기술2의 점유율

	for (int t = 0; t <= maxIter; t++)
	{
		n = n1 + n2; //시장 내 총 사용자 수
		d = n1 - n2; //기술1과 기술2의 사용자수의 차이

		sh_1 = (double)n1 / n; //기술1의 점유율
		sh_2 = 1 - sh_1; //기술2의 점유율

		if (tech == 1) //처음에 constant 모형을 선택
		{
			x = constant(p); //-->constant dyanmics
		}
		else if (tech == 2) //처음에 decreasing 모형을 선택
		{
			x = decreasing(d, ab1, ab2, p);//-->decreasing dynamics
		}
		else
		{
			x = increasing(d, ab1, ab2, p);  //-->increasing dyanamics
		}
		n1 = n1 + x;
		n2 = n2 + (1 - x);
	}

	distribution[i] = sh_1;
}
//Aopdtion dynamics
void adoptionDynamics()
{
	for (int t = 0; t <= maxIter; t++)
	{
		n = n1 + n2; //시장 내 총 사용자 수
		d = n1 - n2; //기술1과 기술2의 사용자수의 차이

		sh_1 = (double)n1 / n; //기술1의 점유율
		sh_2 = 1 - sh_1; //기술2의 점유율

		writeResult(t);//각기 t의 시장점유율 sh_1과 sh_2를 기록

		if (tech == 1) //처음에 constant 모형을 선택
		{
			x = constant(p); //-->constant dyanmics
		}
		else if (tech == 2) //처음에 decreasing 모형을 선택
		{
			x = decreasing(d, ab1, ab2, p);//-->decreasing dynamics
		}
		else
		{
			x = increasing(d, ab1, ab2, p);  //-->increasing dyanamics
		}

		n1 = n1 + x;
		n2 = n2 + (1 - x);
	}
}

//(1)Arthur's constant returns
int constant(double pro)
{
	int agent, choice;

	agent = genBernoulli(); //agent 형태는 1 또는 0 

	choice = agent; //agent는 r=0이므로 무조건 natural preference를 따른다. 

	return choice;
}

//(2)Arthur's decreasing returns
int decreasing(int diff, double prefer1, double prefer2, double pro)
{
	int agent, choice;
	agent = genBernoulli();
	//agent 1이 뽑혔을 경우 (tech 1선호)
	if ((agent == 1) && (diff <= prefer1)) //switching condition --> don't switch
	{
		choice = 1;
	}
	else if ((agent == 1) && (diff > prefer1)) //swithcing condition --> switch
	{
		choice = 0;
	}

	//agent 2가 뽑혔을 경우 (tech 2선호)
	else if ((agent == 0) && (diff >= prefer2)) //switching condition --> don't switch
	{
		choice = 0;
	}
	else
	{
		choice = 1; //switch
	}
	return choice;
}

//(3) Arthur's increasign returns
int increasing(int diff, double prefer1, double prefer2, double pro)
{
	int agent, choice;

	agent = genBernoulli();

	if ((agent == 1) && (diff >= prefer1))
	{
		choice = 1;
	}
	else if ((agent == 1) && (diff < prefer1))
	{
		choice = 0;
	}
	else if ((agent == 0) && (diff <= prefer2))
	{
		choice = 0;
	}
	else
	{
		choice = 1;
	}

	return choice;
}

//Random number function
int genBernoulli()
{
	random_device rd;
	mt19937 bernoulli(rd());
	uniform_int_distribution<>dist(0, 1);

	return dist(bernoulli);
}

//Write Result to the disk
void writeResult(int t)
{
	outputFile.open("Arthur_dynamics_1.txt", ios::app);

	if (!outputFile){
		cerr << "File could not be opened" << endl;
		exit(1);
	}

	/*outputFile << setw(5) << t
		<< setw(10) << n1 << setw(10) << n2
		<< setiosflags(ios::fixed | ios::showpoint)
		<< setw(13) << setprecision(2) << sh_1
		<< setiosflags(ios::fixed |  ios::showpoint)
		<< setw(13) << setprecision(2) << sh_2 << endl;*/

	outputFile << sh_1 << endl;

	outputFile.close();
}

//write distribution to the disk
void writeDistribution()
{
	int Freq[10] = { 0 };

	for (int i = 0; i < 1000; i++)
	{
		if (distribution[i] <= 0.1) { Freq[0]++; }
		else if ((distribution[i] >0.1) && (distribution[i] <= 0.2))
		{
			Freq[1]++;
		}
		else if ((distribution[i] >0.2) && (distribution[i] <= 0.3))
		{
			Freq[2]++;
		}
		else if ((distribution[i] > 0.3) && (distribution[i] <= 0.4))
		{
			Freq[3]++;
		}
		else if ((distribution[i] > 0.4) && (distribution[i] <= 0.5))
		{
			Freq[4]++;
		}
		else if ((distribution[i] > 0.5) && (distribution[i] <= 0.6))
		{
			Freq[5]++;
		}
		else if ((distribution[i] > 0.6) && (distribution[i] <= 0.7))
		{
			Freq[6]++;
		}
		else if ((distribution[i] > 0.7) && (distribution[i] <= 0.8))
		{
			Freq[7]++;
		}
		else if ((distribution[i] > 0.8) && (distribution[i] <= 0.9))
		{
			Freq[8]++;
		}
		else if ((distribution[i] > 0.9) && (distribution[i] <= 1.0))
		{
			Freq[9]++;
		}
	}

	outputFile_dist.open("Distribution.txt", ios::out);
	for (int j = 0; j < 10; j++)
	{
		outputFile_dist << Freq[j] << endl;
	}

	if (!outputFile_dist)
	{
		cerr << "File could not be opened" << endl;
		exit(1);
	}

	outputFile_dist.close();
}