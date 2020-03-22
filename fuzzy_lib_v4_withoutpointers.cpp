#include<iostream>
#include<cmath>
#define exp 2.7132
using namespace std;
class Fuzzy
{
	public:
		int sets;
		double sigma[9];
		double error_y[9],d_error_y[9];
		double min_y[9][9];
		int fuzzy[9][9];
		double range[9][3];
		double error,d_error;
		double numerator,sum;
		//constructor
		Fuzzy(double a[9][3],int n,double s)
		{
			numerator=0.0;
			sum=0.0;
			sets=n;
			for(int i=0;i<sets;i++)
			{
				sigma[i]=s;
				range[i][0]=a[i][0];
				range[i][1]=a[i][1];
				range[i][2]=a[i][2];
			}
			for(int i=0,k=0;i<sets;i++,k=0)
				for(int j=i;j<sets;j++)
				{
					if(k==0)
					{
						do
						{
							fuzzy[i][j]=sets-i-1;
							j++;
						}
						while(j<int((sets+1)/2));
					}
					fuzzy[i][j]=fuzzy[i][j-1]-1;
					k++;
				}
			for(int i=0;i<sets;i++)
				for(int j=0;j<i;j++)
					fuzzy[i][j]=fuzzy[j][i];
			fuzzy[1][1]=sets-1;
			fuzzy[sets-2][sets-2]=0;
		}
		//gives the value of weight of measurement
		double give_y_gaussian(double e,int i)
		{
			double u=-(((e-range[i][1])*(e-range[i][1]))/(sigma[i]*sigma[i]*2));
			return pow(exp,u);
		}
		double give_y_triangular(double e,int i)
		{
			if(e<range[i][2]&&e>=range[i][1])
				return (range[i][2]-e)/(range[i][2]-range[i][1]);
			else if(e>range[i][0]&&e<range[i][1])
				return (e-range[i][0])/(range[i][1]-range[i][0]);
			else return 0.0;
		}
		// defines arrays of the weights of error, change_error, mininmum of all combinations of all errors and change_errors
		void define_error_array_gaussian()
		{
			for(int i=0;i<sets;i++)
			{
				error_y[i]=give_y_gaussian(error,i);
				d_error_y[i]=give_y_gaussian(d_error,i);
			}
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
					min_y[i][j]=(error_y[i]<d_error_y[j]?error_y[i]:d_error_y[j]);
		}
		void define_error_array_triangular()
		{
			for(int i=0;i<sets;i++)
			{
				error_y[i]=give_y_triangular(error,i);
				d_error_y[i]=give_y_triangular(d_error,i);
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
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
				{
					numerator+=min_y[i][j]*range[fuzzy[i][j]][1];
					sum+=min_y[i][j];
				}
			return numerator/sum;
		}
		double correction_triangular(double e,double de)
		{
			error=e;
			d_error=de;
			define_error_array_triangular();
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
				{
					numerator+=min_y[i][j]*range[fuzzy[i][j]][1];
					sum+=min_y[i][j];
				}
			return numerator/sum;
		}		
};
int main()
{
	double a[9][3];
	a[0][0]=-60.0;
	a[0][1]=-40.0;
	a[0][2]=-20.0;
	for(int i=1;i<5;i++)
	{
		a[i][0]=a[i-1][0]+20;
		a[i][1]=a[i-1][1]+20;
		a[i][2]=a[i-1][2]+20;
	}
	Fuzzy mag2(a,5,20.0);//(SETS**,NSETS,SIGMA)
//	for(int i=0;i<5;i++)
//	{
//		for(int j=0;j<5;j++)
//			cout<<mag2.fuzzy[i][j]<<" ";
//		cout<<endl;
//	}
//	cout<<"Error_triangular = "<<mag2.correction_triangular(43,0)<<endl;
//	cout<<"Numerator_triangular = "<<mag2.numerator<<endl;
//	cout<<"Sum_triangular = "<<mag2.sum<<endl;
	cout<<"Error_triangular = "<<mag2.correction_triangular(16,17)<<endl;
	cout<<"Error_gaussian = "<<mag2.correction_gaussian(16,17);
}
