#include<cmath>
#define exp 2.7132
#include<iostream>
using namespace std;
class Fuzzy
{
	public:
		int sets;
		double error_y[9],d_error_y[9];
		double min_y[9][9];
		double logic[9][9]={{8,8,8,8,8,7,6,5,4},
							{8,7,7,7,7,6,5,4,3},
							{8,7,6,6,6,5,4,3,2},
							{8,7,6,5,5,4,3,2,1},
							{8,7,6,5,4,3,2,1,0},
							{7,6,5,4,3,3,2,1,0},
							{6,5,4,3,2,2,2,1,0},
							{5,4,3,2,1,1,1,1,0},
							{4,3,2,1,0,0,0,0,0}};
							
		double fuzzy[81];
		double range[9][3];
		double error,d_error;
		//constructor
		Fuzzy(double a[9][3],int n)
		{
			sets=n;
//			error_y=new double[sets];
//			d_error_y=new double[sets];
//			min_y=new double*[sets];
//			range=new double*[sets];
//			for(int i=0;i<sets;i++)
//				range[i]=new double[3];
			for(int i=0;i<sets;i++)
			{
				range[i][0]=a[i][0];
				range[i][1]=a[i][1];
				range[i][2]=a[i][2];
			}
//			fuzzy=new double [n*n];
//			for(int p=0,r=0;p<sets;p++)
//			{
//				for(int q=0;q<sets;q++)
//					fuzzy[q+r]=logic[int((9-sets)/2)+p][int((9-sets)/2)+(sets+1)%2+q];
//				r+=sets;
//			}
			for(int i=0;i<sets;i++)
				for(int j=i;j<sets;j++)
				{
					do
					{
						fuzzy[i][j]=sets-i-1;
						j++;
					}
					while(j<int((sets+1)/2));
					fuzzy[i][j]=fuzzy[i][j-1]-1;
				}
			for(int i=0;i<sets;i++)
				for(int j=0;j<i;j++)
					fuzzy[i][j]=fuzzy[j][i];
			for(int i=0;i<sets;i++)
				for(int j=0;j<i;j++)
					fuzzy[i][j]=range[int(fuzzy[i][j])];
		}
		//gives the value of weight of measurement
		double give_y_triangular(double e,int i)
		{
			if(e<range[i][2]&&e>=range[i][1])
				return (range[i][2]-e)/(range[i][2]-range[i][1]);
			else if(e>range[i][0]&&e<range[i][1])
				return (e-range[i][0])/(range[i][1]-range[i][0]);
			else return 0.0;
		}
		// defines arrays of the weights of error, change_error, mininmum of all combinations of all errors and change_errors
		void define_error_array_triangular()
		{
			for(int i=0;i<sets;i++)
			{
				error_y[i]=give_y_triangular(error,i);
				d_error_y[i]=give_y_triangular(d_error,i);
			}
			for(int i=0;i<sets;i++)
			{
//				min_y[i]=new double[sets];
				for(int j=0;j<sets;j++)
					min_y[i][j]=(error_y[i]<d_error_y[j]?error_y[i]:d_error_y[j]);
			}
		}
		//the main function returns the value of correction
		double correction_triangular(double e,double de)
		{ 
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
int main()
{
	double a[9][3];
	a[0][0]=-20;
	a[0][1]=-15;
	a[0][2]=-10;
	for(int i=0;i<9;i++)
	{
		a[i][0]=a[i-1][0]+5;
		a[i][1]=a[i-1][1]+5;
		a[i][2]=a[i-1][2]+5;
	}
	Fuzzy f(a,9);
	cout<<f.correction_triangular(5,5);
}
