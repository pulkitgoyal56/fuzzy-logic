#include<cmath>
#define exp 2.7132
using namespace std;
class Fuzzy
{
	public:
		double multiplier,sigma;
		double error_y[5],d_error_y[5],min_y[5][5],fuzzy[25],range[5];
		double error,d_error;
		Fuzzy(double s,double m=2.0)
		{
			//sigma - deviation (width of membership function)
			sigma=s;
			//multiplier - sets the distance between the point of intersection of the two adjacent membership functions on either side of a function (sets the distance between centres of two adjacent membership functions) BY DEFAULT 2
			multiplier=m/2;
			//defining the membership functions
			range[0]=-2*multiplier*sigma;
			range[1]=-1*multiplier*sigma;
			range[2]=0.0;
			range[3]=multiplier*sigma;
			range[4]=2*multiplier*sigma;
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
		~Fuzzy(){}
		//gives the value of weight of measurement
		double give_y(double e,double s)
		{
			double u=-(((e-s)*(e-s))/(sigma*sigma*2));
			return pow(exp,u); 
		}
		// defines arrays of the weights of error, change_error, mininmum of all combinations of all errors and change_errors
		void define_error_array()
		{
			for(int i=0;i<5;i++)
			{
				error_y[i]=give_y(error,range[i]);
				d_error_y[i]=give_y(d_error,range[i]);
			}
			for(int i=0;i<5;i++)
				for(int j=0;j<5;j++)
					min_y[i][j]=(error_y[i]<d_error_y[j]?error_y[i]:d_error_y[j]);
		}
		//the main function returns the value of correction
		double correction(double e,double de)
		{
			error=e;
			d_error=de;
			define_error_array();
			int k=0;
			double numerator=0.0,sum=0.0;
			for(int i=0;i<5;i++)
			{
				for(int j=0;j<5;j++)
				{
					numerator+=min_y[i][j]*fuzzy[i+j+k];
					sum+=min_y[i][j];
				}
				k+=4;
			}
			return numerator/sum;
		}
		
};
//int main()
//{
////	cout<<fuz.correction(-5,8.78908012);
//}
