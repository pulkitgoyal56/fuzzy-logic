#include<cmath>
#define exp 2.7132
#include<iostream>
using namespace std;
class Fuzzy
{
	public:
		double multiplier,centre;
		double* sigma;
		int sets;
//		double error_y[9],d_error_y[9],fuzzy[9][9],range[9]
//		double min_y[9][9];
		double *error_y,*d_error_y;
		double **min_y,**range;
		int **fuzzy;
		double error,d_error;
		double numerator,sum;
		//constructor
		Fuzzy(double s,double m=2.0,int n=5,double c=0.0) //for triangular functions, set multiplier between 1.0 and 3.0 for best results
		{
			if(s<=0.0)
			{
//				Serial.println("!! Invalid Sigma (<=0) !!");
			}
			if(m<=0.0)
			{
//				Serial.println("!! Invalid Multiplier (<0) !!\t -Changing Multiplier to 1.0");
				m=1.0;
			}
			if(n<=0)
			{
//				Serial.println("!! Invalid Number of Sets(<0) !!\t -Changing Number of Sets to 5");
				n=5;
			}
			sigma=new double[sets];
			sets=n;
			//sigma - deviation (width of membership function)
			//multiplier - sets the distance between the point of intersection of the two adjacent membership functions on either side of a function (sets the distance between centres of two adjacent membership functions) BY DEFAULT 2
			multiplier=m/2;
			numerator=0.0;
			sum=0.0;
			centre=c;
			error_y=new double[sets];
			d_error_y=new double[sets];
			min_y=new double*[sets];
			fuzzy=new int*[sets];
			range=new double*[sets];
			//defining the membership functions
			for(int i=0;i<sets;i++)
			{
				sigma[i]=s;
				range[i]=new double[3];
				min_y[i]=new double[sets];
				fuzzy[i]=new int[sets];
				range[i][1]=c+(i+1-int((sets+1)/2))*multiplier*sigma[i];
				range[i][0]=range[i][1]-sigma[i];
				range[i][2]=range[i][1]+sigma[i];
			}
			//heart of fuzzy logic
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
		Fuzzy(double **a,int n,double s)
		{
			if(s<=0.0)
			{
//				Serial.println("!! Invalid Sigma (<=0) !!");
			}
			if(n<=0)
			{
//				Serial.println("!! Invalid Number of Sets(<0) !!\t -Changing Number of Sets to 5");
				n=9;
			}
			error_y=new double[sets];
			d_error_y=new double[sets];
			min_y=new double*[sets];
			fuzzy=new int*[sets];
			range=new double*[sets];
			sigma=new double [sets];
			//defining the membership functions
			for(int i=0;i<sets;i++)
			{
				range[i]=new double[3];
				min_y[i]=new double[sets];
				fuzzy[i]=new int[sets];
			}
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
		}
		Fuzzy(double** a,int n)
		{
			if(n<=0)
			{
//				Serial.println("!! Invalid Number of Sets(<0) !!\t -Changing Number of Sets to 5");
				n=9;
			}
			numerator=0.0;
			sum=0.0;
			sets=n;
			sigma=new double[sets];
			for(int i=0;i<sets;i++)
			{
				sigma[i]=a[i][1];
				range[i][0]=a[i][0];
				range[i][1]=a[i][0]-a[i][1];
				range[i][2]=a[i][0]+a[i][1];
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
		}
		//destructor
		~Fuzzy()
		{
			delete[] error_y;
			delete[] d_error_y;
			for(int i=0;i<sets;i++)
			{
				delete[] fuzzy[i];
				delete[] min_y[i];
				delete[] range[i];
			}
			delete[] min_y;
			delete[] fuzzy;
			delete[] range;
			delete[] sigma;
		}
		//gives the value of weight of measurement
		double give_y_gaussian(double e,int i)
		{
			double u=-(((e-range[i][1])*(e-range[i][1]))/(sigma[i]*sigma[i]*2));
			return pow(exp,u);
		}
		double give_y_triangular(double e,int i)
		{
			if(e<(range[i][2])&&e>=range[i][1])
				return (range[i][2]-e)/(range[i][2]-range[i][1]);
			if(e>(range[i][0])&&e<range[i][1])
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
			{
				for(int j=0;j<sets;j++)
					min_y[i][j]=(error_y[i]<d_error_y[j]?error_y[i]:d_error_y[j]);
			}
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
					numerator+=min_y[i][j]*fuzzy[i][j];
					sum+=min_y[i][j];
				}
			return numerator/sum;
		}
		double correction_triangular(double e,double de)
		{
			if(multiplier>=4.0)
			{
//				Serial.println("!! Invalid Multiplier For Triangular Functions!!\t -Changing Multiplier to 3.0");
				multiplier=3.0;
			}
			if(e>=range[sets-1][0]||e<=range[0][2])
			{
//				Serial.println("!! Error Out Of Bounds For Triangular Functions !!);
				return 0;
			}
			if(de>=range[sets-1][0]||de<=range[0][2])
			{
//				Serial.println("!! Difference_Error Out Of Bounds For Triangular Functions !!");
				return 0;
			}
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
	Fuzzy mag(20.0);//(SIGMA,MULTIPLIER,SETS,CENTRE)
//	for(int i=0;i<5;i++)
//		cout<<mag.range[i]-mag.sigma<<" "<<mag.range[i]<<" "<<mag.range[i]+mag.sigma<<endl;
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
			cout<<mag.fuzzy[i][j]<<" ";
		cout<<endl;
	}
	cout<<"Error_triangular = "<<mag.correction_triangular(16,17)<<endl;
	cout<<"Numerator_triangular = "<<mag.numerator<<endl;
	cout<<"Sum_triangular = "<<mag.sum<<endl;
	cout<<"Error_gaussian = "<<mag.correction_gaussian(16,17)<<endl;
	cout<<"Numerator_gaussian = "<<mag.numerator<<endl;
	cout<<"Sum_gaussian = "<<mag.sum;
	double** a;
	a=new double*[5];
	for(int i=0;i<5;i++)
		a[i]=new double[3];
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
	cout<<"Error_triangular = "<<mag2.correction_triangular(16,17)<<endl;
	cout<<"Numerator_triangular = "<<mag2.numerator<<endl;
	cout<<"Sum_triangular = "<<mag2.sum<<endl;
	cout<<"Error_gaussian = "<<mag2.correction_gaussian(16,17)<<endl;
	cout<<"Numerator_gaussian = "<<mag2.numerator<<endl;
	cout<<"Sum_gaussian = "<<mag2.sum;
	double **b;
	b=new double*[5];
	for(int i=0;i<5;i++)
		b[i]=new double[2];
	b[0][0]=-40;
	b[0][1]=20;
	for(int i=1;i<5;i++)
	{
		b[i][0]=b[i-1][0]+20;
		b[i][1]=20;
	}
	Fuzzy mag3(b,5);//(SETS**,NSETS)
	cout<<"Error_triangular = "<<mag3.correction_triangular(16,17)<<endl;
	cout<<"Numerator_triangular = "<<mag3.numerator<<endl;
	cout<<"Sum_triangular = "<<mag3.sum<<endl;
	cout<<"Error_gaussian = "<<mag3.correction_gaussian(16,17)<<endl;
	cout<<"Numerator_gaussian = "<<mag3.numerator<<endl;
	cout<<"Sum_gaussian = "<<mag3.sum;
}
