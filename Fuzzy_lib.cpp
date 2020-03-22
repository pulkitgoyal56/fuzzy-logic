#include<cmath>
#define exp 2.7132
#include<iostream>
using namespace std;
class Fuzzy
{
	public:
		double multiplier,sigma,centre;
		int sets;
//		double error_y[9],d_error_y[9],fuzzy[9][9],range[9]
//		double min_y[9][9];
		double *error_y,*d_error_y,*range;
		double **min_y;
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
			sets=n;
			//sigma - deviation (width of membership function)
			sigma=s;
			//multiplier - sets the distance between the point of intersection of the two adjacent membership functions on either side of a function (sets the distance between centres of two adjacent membership functions) BY DEFAULT 2
			multiplier=m/2;
			numerator=0.0;
			sum=0.0;
			centre=c;
			error_y=new double[sets];
			d_error_y=new double[sets];
			min_y=new double*[sets];
			fuzzy=new int*[sets];
			range=new double[sets];
			//defining the membership functions
			for(int i=0;i<sets;i++)
			{
				min_y[i]=new double[sets];
				fuzzy[i]=new int[sets];
				range[i]=c+(i+1-int((sets+1)/2))*multiplier*sigma;
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
			for(int i=0;i<sets;i++)
				delete[] fuzzy[i];
			delete[] range; 
		}
		//gives the value of weight of measurement
		double give_y_gaussian(double e,int i)
		{
			double u=-(((e-range[i])*(e-range[i]))/(sigma*sigma*2));
			return pow(exp,u);
		}
		double give_y_triangular(double e,int i)
		{
			if(e<(range[i]+sigma)&&e>=range[i])
				return (range[i]+sigma-e)/sigma;
			if(e>(range[i]-sigma)&&e<range[i])
				return (e-range[i]+sigma)/sigma;
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
			if(e>=range[sets-1]+sigma||e<=range[0]-sigma)
			{
//				Serial.println("!! Error Out Of Bounds For Triangular Functions !!\t -Increasing (Doubling) Sigma");
				sigma*=2;
			}
			if(de>=range[sets-1]+sigma||de<=range[0]-sigma)
			{
//				Serial.println("!! Difference_Error Out Of Bounds For Triangular Functions !!\t -Increasing (Doubling) Sigma");
				sigma*=2;
			}
			error=e;
			d_error=de;
			define_error_array_triangular();
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
				{
					numerator+=min_y[i][j]*range[fuzzy[i][j]];
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
}
