//FUZZY LOGIC LIBRARY
/* 
===============================================
Copyright (c) 2015 Pulkit, Shivam

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/
#ifndef Fuzzy_lib_h
	#define Fuzzy_lib_h
#include<cmath>
#define exp 2.7132
#ifdef ARDUINO
    #if ARDUINO < 100
        #include "WProgram.h"
    #else
        #include "Arduino.h"
    #endif
#endif
using namespace std;
class Fuzzy
{
	public:
		double multiplier,sigma,centre;
		int sets;
		double *error_y,*d_error_y,*fuzzy,*range;
		double **min_y;
		double error,d_error;
		//constructor
		Fuzzy(double s,double m=2.0,int n=5,double c=0.0) //for triangular functions, set multiplier between 1.0 and 3.0 for best results
		{
			if(s<0)
			{
				Serial.println("!! Invalid Sigma (<0) !!");
			}
			if(m<=0)
			{
				Serial.println("!! Invalid Multiplier (<0) !!\t -Changing Multiplier to 1.0");
				m=1.0;
			}
			if(n<=0)
			{
				Serial.println("!! Invalid Number of Sets(<0) !!\t -Changing Number of Sets to 5");
				n=5;
			}
			sets=n;
			//sigma - deviation (width of membership function)
			sigma=s;
			//multiplier - sets the distance between the point of intersection of the two adjacent membership functions on either side of a function (sets the distance between centres of two adjacent membership functions) BY DEFAULT 2
			multiplier=m/2;
			centre=c;
			error_y=new double[sets];
			d_error_y=new double[sets];
			*min_y=new double[sets];
			for(int i=0;i<sets;i++)
				min_y[i]=new double[sets];
			fuzzy=new double[sets*sets];
			range=new double[sets];
			//defining the membership functions
			for(int i=0;i<n;i++)
			{
					range[i]=c+(i+1-int((n+1)/2))*multiplier*sigma;
			}
			//heart of fuzzy logic
			fuzzy[0]=range[4];
			fuzzy[1]=range[4];
			fuzzy[2]=range[4];
			fuzzy[3]=range[3];
			fuzzy[4]=range[3];
			fuzzy[5]=range[4];
			fuzzy[6]=range[4];
			fuzzy[7]=range[3];
			fuzzy[8]=range[2];
			fuzzy[9]=range[2];
			fuzzy[10]=range[3];
			fuzzy[11]=range[3];
			fuzzy[12]=range[2];
			fuzzy[13]=range[1];
			fuzzy[14]=range[1];
			fuzzy[15]=range[2];
			fuzzy[16]=range[2];
			fuzzy[17]=range[1];
			fuzzy[18]=range[0];
			fuzzy[19]=range[0];
			fuzzy[20]=range[1];
			fuzzy[21]=range[1];
			fuzzy[22]=range[0];
			fuzzy[23]=range[0];
			fuzzy[24]=range[0];				
		}
		//destructor
		~Fuzzy()
		{
			delete[] error_y;
			delete[] d_error_y;
			for(int i=0;i<sets;i++)
				delete[] min_y[i];
			delete[] min_y;
			delete[] fuzzy;
			delete[] range; 			
		}
		//gives the value of weight of measurement
		double give_y_gaussian(double e,double s)
		{
			double u=-(((e-s)*(e-s))/(sigma*sigma*2));
			return pow(exp,u);
		}
		double give_y_triangular(double e,double s)
		{
			if(e<s+sigma&&e>s)
				return (s+sigma-e)/sigma;
			else
				return (e-s+sigma)/sigma;
		}
		// defines arrays of the weights of error, change_error, mininmum of all combinations of all errors and change_errors
		void define_error_array_gaussian()
		{
			for(int i=0;i<sets;i++)
			{
				error_y[i]=give_y_gaussian(error,range[i]);
				d_error_y[i]=give_y_gaussian(d_error,range[i]);
			}
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
					min_y[i][j]=(error_y[i]<d_error_y[j]?error_y[i]:d_error_y[j]);
		}
		void define_error_array_triangular()
		{
			for(int i=0;i<sets;i++)
			{
				error_y[i]=give_y_triangular(error,range[i]);
				d_error_y[i]=give_y_triangular(d_error,range[i]);
			}
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
					min_y[i][j]=(error_y[i]<d_error_y[j]?error_y[i]:d_error_y[j]);
		}
		//the main function returns the value of correction
		double correction_gaussian(double e,double de)
		{
			error=e;
			d_error=de;
			define_error_array_gaussian();
			int k=0;
			double numerator=0.0,sum=0.0;
			for(int i=0;i<sets;i++)
			{
				for(int j=0;j<sets;j++)
				{
					numerator+=min_y[i][j]*fuzzy[i+j+k];
					sum+=min_y[i][j];
				}
				k+=sets-1;
			}
			return numerator/sum;
		}
		double correction_triangular(double e,double de)
		{
			if(multiplier>=4.0)
			{
				Serial.println("!! Invalid Multiplier For Triangular Functions!!\t -Changing Multiplier to 3.0");
				multiplier=3.0;
			}
			if(e>=range[4]+sigma||e<=range[0]-sigma)
			{
				Serial.println("!! Error Out Of Bounds For Triangular Functions !!\t -Increasing (Doubling) Sigma");
				sigma*=2;
			}
			if(de>=range[4]+sigma||de<=range[0]-sigma)
			{
				Serial.println("!! Difference_Error Out Of Bounds For Triangular Functions !!\t -Increase Sigma");
				sigma*=2;
			}
			error=e;
			d_error=de;
			define_error_array_triangular();
			int k=0;
			double numerator=0.0,sum=0.0;
			for(int i=0;i<sets;i++)
			{
				for(int j=0;j<sets;j++)
				{
					numerator+=min_y[i][j]*fuzzy[i+j+k];
					sum+=min_y[i][j];
				}
				k+=sets-1;
			}
			return numerator/sum;
		}		
};
#endif
