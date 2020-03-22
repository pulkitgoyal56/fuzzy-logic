#ifndef Fuzzy_lib_h
	#define Fuzzy_lib_h
#include<iostream>
#include<cmath>
#define exp 2.7132
using namespace std;
class Fuzzy
{
	public:
		double multiplier,sigma,r0,r1,r2,r3,r4;
		double error_y[5],d_error_y[5],min_y[5][5],fuzzy[25],range[5];
		double sum;
		Fuzzy(double s,double m)
		{
			sigma=s;
			multiplier=m;
			r0=-2*multiplier*sigma;
			r1=-1*multiplier*sigma;
			r2=0.0;
			r3=multiplier*sigma;
			r4=2*multiplier*sigma;
			sum=0.0;
			range[0]=r0;
			range[1]=r1;
			range[2]=r2;
			range[3]=r3;
			range[4]=r4;
		}
		double error,d_error;
		double give_y(double e,double s)
		{
			double u=-((e-s)*(e-s))/(sigma*sigma*2);
			return pow(exp,u); 
		}
		double compare(double ye,double yde)
		{
			return (ye<yde?ye:yde);
		}
		void define_error_array()
		{
			for(int i=0;i<5;i++)
			{
				error_y[i]=give_y(error,range[i]);
				d_error_y[i]=give_y(d_error,range[i]);
			}
			for(int i=1;i<6;i++)
				for(int j=1;j<6;j++)
					min_y[i][j]=compare(error_y[i],d_error_y[j]);
		}
		void assign_fuzzyset()
		{
			int i=0;
				fuzzy[i]=r4;
				i++;
				fuzzy[i]=r4;
				i++;
				fuzzy[i]=r4;
				i++;
				fuzzy[i]=r3;
				i++;
				fuzzy[i]=r2;
				i++;
				fuzzy[i]=r4;
				i++;
				fuzzy[i]=r4;
				i++;
				fuzzy[i]=r3;
				i++;
				fuzzy[i]=r2;
				i++;
				fuzzy[i]=r1;
				i++;
				fuzzy[i]=r3;
				i++;
				fuzzy[i]=r3;
				i++;
				fuzzy[i]=r2;
				i++;
				fuzzy[i]=r1;
				i++;
				fuzzy[i]=r1;
				i++;
				fuzzy[i]=r1;
				i++;
				fuzzy[i]=r2;
				i++;
				fuzzy[i]=r1;
				i++;
				fuzzy[i]=r0;
				i++;
				fuzzy[i]=r0;
				i++;
				fuzzy[i]=r1;
				i++;
				fuzzy[i]=r1;
				i++;
				fuzzy[i]=r0;
				i++;
				fuzzy[i]=r0;
				i++;
				fuzzy[i]=r0;
				i++;
		}
		double correction(double e,double de)
		{
			define_error_array();
			assign_fuzzyset();
			int k=0;
			double numerator=0.0;
			for(int i=0;i<5;i++)
			{
				for(int j=0;j<5;j++)
					{
						numerator+=min_y[i][j]*fuzzy[i+j+k];
						sum+=min_y[i][j];
					}
				k+=5;
			}
			return numerator/sum;
		}
};
#endif
