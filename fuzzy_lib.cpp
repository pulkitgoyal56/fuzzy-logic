//FUZZY LOGIC LIBRARY V*7 - WRITTEN BY PULKIT GOYAL
// I've tried to comment at all possible places, where i was able to, so that it would be easier for anyone to understand it easily.
//The following code is a modified version of the original header file. Some basic commands of the library which are arduino compatible had to be removed and are replaced by c++ commands for the same.
#include<iostream>
#include<cmath>
#define exp 2.7132
#define max_n 9 // Define the maximum sets here
using namespace std;
class Fuzzy
{
	public:
		double multiplier;// Difference/distance between sets
		double centre;// Centre of middle set
		int sets;// Number of sets
		double sigma[max_n];//Value of sigma for each set
		double error_y[max_n];//Weight of error in each set
		double d_error_y[max_n];//Weight of d_error in each set
		double min_y[max_n][max_n];//Minimum value of each possible pair of error and d_error
		double range[max_n][3];//Array of extreme and centre points of sets
		int fuzzy[max_n][max_n];//Fuzzy Logic
		double sigma_t;
		double error;//error
		double d_error;//d_error
		double numerator;//Summation of cooresponding fuzzy*minimum_y
		double sum;//sum of min_y
		//heart of fuzzy logic
		void define_fuzzylogic()//creates fuzzy logic array
		{
			for(int i=0,k=0;i<sets;i++,k=0)											//These lines define the upper triangular matrix of the fuzzy set
				for(int j=i;j<sets;j++)												//*
				{																	//*
					if(k==0)														//*
					{																//*
						do															//*This do-while loop is a part of the rule and is just a way to define the set
						{															//*(Nothing to see here)
							fuzzy[i][j]=sets-i-1;									//*
							j++;													//*
						}															//*
						while(j<int(sets+1)/2);//to account for even and odd sets 	//*
					}																//*
					fuzzy[i][j]=fuzzy[i][j-1]-1;									//**
					k++;															//**
				}
			for(int i=0;i<sets;i++)								//*These lines copy the upper triangular the the left out lower portion
				for(int j=0;j<i;j++)							//
					fuzzy[i][j]=fuzzy[j][i];					//
			fuzzy[1][1]=sets-1;				//This line modifies the (2,2) element in the fuzzy set. This line is separate because of its inconsistency to the symmetricity of the logic which could not be incorporated in the original logic above
			fuzzy[sets-2][sets-2]=0;		//Same as the above but modifies the (sets-1,sets-1) i.e. the second from last element on the main diagonal
		}
		void append_set(double a, double b, double c)//Nothing to explain here, just a very big function to add sets to the object (TRIANGULAR STYLE) //(LEFT_END, CENTRE, RIGHT_END)
		{
			if(c<b||a>b)//corrects the order of input
			{
				double temp;
				if(a>b)
				{
					temp=b;
					b=a;
					a=temp;
				}
				if(b>c)
				{
					temp=c;
					c=b;
					b=temp;
				}
				if(a>b)
				{
					temp=b;
					b=a;
					a=temp;
				}
			}
			if(sets==0)
			{
				range[0][0]=a;
				range[0][1]=b;
				range[0][2]=c;
				sigma[0]=sigma_t;
				sets++;
			}
			else
			{
				if(sets<max_n)
				{
					int i=sets-1;
					while(i>=0)
					{
						if(b==range[i][1])
						{
							cout<<"!! No two sets can have the same centre !!"<<endl;
							sets--;
							break;
						}
						if(b<range[i][1])					//finding the position for the new set
						{
							i--;
							if(i>-1)
								continue;
						}
						i++;
						for(int j=sets;j>i;j--)				//
						{									//
							range[j][0]=range[j-1][0];		//
							range[j][1]=range[j-1][1];		//Shifts the other sets to make space for the new set
							range[j][2]=range[j-1][2];		//
							sigma[j]=sigma[j-1];			//
						}									//
						range[i][0]=a;
						range[i][1]=b;
						range[i][2]=c;
						sigma[i]=sigma_t;
						break;
					}
					sets++;
				}
				else
					cout<<"!! Maximum sets limit reached -No more sets can be added !! max_n = "<<max_n<<endl;
			}
		}
		void append_set(double a, double b)//Nothing to explain here, just a very big function to add sets to the object (GAUSSIAN STYLE) //(CENTRE, SIGMA)
		{
			if(b<=0)//Checks if sigma is less than zero
			{
				cout<<"!! Invalid Sigma (<=0) !! - Changing Sigma to 10"<<endl;//if so gives a default value so as to continue the operation with a optimum value applicable in most situations
				b=10.0;
			}
			if(sets==0)
			{
				range[0][0]=a-b;
				range[0][1]=a;
				range[0][2]=a+b;
				sigma[0]=b;
				sets++;
			}
			else
			{
				if(sets<max_n)
				{
					int i=sets-1;
					while(i>=0)
					{
						if(a==range[i][1])
						{
							cout<<"!! No two sets can have the same centre !!"<<endl;
							sets--;
							break;
						}
						if(a<range[i][1])					//finding the position for the new set
						{
							i--;
							if(i!=-1)
								continue;
						}
						i++;
						for(int j=sets;j>i;j--)				//
						{									//
							range[j][0]=range[j-1][0];		//
							range[j][1]=range[j-1][1];		//Shifts the other sets to make space for the new set
							range[j][2]=range[j-1][2];		//
							sigma[j]=sigma[j-1];			//
						}									//
						range[i][0]=a-b;
						range[i][1]=a;
						range[i][2]=a+b;
						sigma[i]=b;
						break;
					}
					sets++;
				}
				else
					cout<<"!! Maximum sets limit reached -No more sets can be added !! max_n = "<<max_n<<endl;
			}
		}
		//constructor
		//CONSTRUCTOR NUMBER ONE - for symmetric sets (both Tringular and Gaussian)
		Fuzzy(double s,int n,double m=2.0,double c=0.0) //for triangular functions, set multiplier between 1.0 and 3.0 for best results
		{
			if(s<=0.0)//Checks if sigma is less than zero
			{
				cout<<"!! Invalid Sigma (<=0) !! - Changing Sigma to 10"<<endl;
				s=10.0;//if so gives a default value so as to continue the operation with a optimum value applicable in most situations
			}
			if(m<=0.0)//Checks if multiplier is less than zero
			{
				cout<<"!! Invalid Multiplier (<0) !! - Changing Multiplier to 2.0"<<endl;
				m=2.0;//if so gives it the value 2 i.e. the standard distribution of sets
			}
			if(n<=0)//Checks if the number of given sets are less than zero
			{
				cout<<"!! Invalid Number of Sets(<0) !! - Changing Number of Sets to "<<max_n<<endl;
				n=max_n;//if so sets the number_of_sets to maximum number of sets defined at the top for most accurate correction possible (but may increase compilation time)
			}
			if(n>max_n)//Since arrays are defined upto a maximum limit, exceeding that limit will create problems
			{
				cout<<"!! Invalid Number of Sets(>max_n) !! - Changing Number of Sets to "<<max_n<<endl;
				n=max_n;//if so sets the number_of_sets to maximum number of sets defined at the top for most accurate correction possible (but may increase compilation time)
			}
			sets=n;
			multiplier=m/2;
			centre=c;
			sigma_t=s;
			//defining the membership functions
			for(int i=0;i<sets;i++)
			{
				sigma[i]=s;
				range[i][1]=c+(i-(sets-1)/2)*multiplier*sigma[i];//this expression gives value to the centre of sets
																//in case of odd sets, the middle set has a centre/antinode at 'c'
																//in case of even sets, the point of intersection of the two middle sets is at 'c'
				range[i][0]=range[i][1]-sigma[i];//sets the value of ends of a set
				range[i][2]=range[i][1]+sigma[i];//same as above
			}
			numerator=0.0;					//Initialising numerator (in correction)
			sum=0.0;						//Initialising sum (denominator in correction)

		}
		//CONSTRUCTOR NUMBER 2 - for discreet/unsymmetric triangular sets + symmetric gaussian sets
		Fuzzy(double s=10.0)
		{
			if(s<=0.0)//Checks if sigma is less than zero
			{
				cout<<"!! Invalid Sigma (<=0) !! - Changing Sigma to 10"<<endl;
				s=10.0;//if so gives a default value so as to continue the operation with a optimum value applicable in most situations
			}
			sigma_t=s;
			sets=0;
			numerator=0.0;					//Initialising numerator (in correction)
			sum=0.0;						//Initialising sum (denominator in correction)
		}
		//gives the value of weight of measurement GAUSSIAN
		double give_y_gaussian(double e,int i)
		{
			double u=-(((e-range[i][1])*(e-range[i][1]))/(sigma[i]*sigma[i]*2));//gaussian function
			return pow(exp,u);
		}
		//gives the value of weight of measurement TRIANGULAR
		double give_y_triangular(double e,int i)
		{
			if(e<(range[i][2])&&e>=range[i][1])//firts half of a set
				return (range[i][2]-e)/(range[i][2]-range[i][1]);
			else if(e>(range[i][0])&&e<range[i][1])//second half of a set
				return (e-range[i][0])/(range[i][1]-range[i][0]);
			else return 0.0;
		}
		double give_centroid_triangular(int p)//gives the centroid of the triangle thus formed(the abscissa)
		{
			return ((range[p][0]+range[p][2]+range[p][1])/3);
		}
		double give_centroid_gaussian(int p)//gives the centroid value of the gassian set formed
		{
			return range[p][1];
		}
		// defines arrays of the weights of error, change_error, mininmum of all combinations of all errors and change_errors GAUSSIAN
		void define_error_array_gaussian()
		{
			for(int i=0;i<sets;i++)
			{
				error_y[i]=give_y_gaussian(error,i);	//fills the error array for each set
				d_error_y[i]=give_y_gaussian(d_error,i);//fills the change_in_error for each set
			}
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
					min_y[i][j]=(error_y[i]<d_error_y[j]?error_y[i]:d_error_y[j]);//fills the minimum value of the corresponding error and change_in_error (corresponding i.e. of the same set) in the respective cell
		}
		// defines arrays of the weights of error, change_error, mininmum of all combinations of all errors and change_errors TRIANGULAR
		void define_error_array_triangular()
		{
			for(int i=0;i<sets;i++)
			{
				error_y[i]=give_y_triangular(error,i);		//fills the error array for each set
				d_error_y[i]=give_y_triangular(d_error,i);	//fills the change_in_error for each set
			}
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
					min_y[i][j]=(error_y[i]<d_error_y[j]?error_y[i]:d_error_y[j]);//fills the minimum value of the corresponding error and change_in_error (corresponding i.e. of the same set) in the respective cell
		}
		//the main function returns the value of correction GAUSSIAN
		double correction_gaussian(double e,double de)
		{
			define_fuzzylogic();
			error=e;
			d_error=de;
			define_error_array_gaussian();
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
				{
					numerator+=min_y[i][j]*give_centroid_gaussian(fuzzy[i][j]);//calculation of numerator
					sum+=min_y[i][j];//calculation of sum
				}
			return numerator/sum;// and finally the CORRECTION!!
		}
		//the main function returns the value of correction TRIANGULAR
		double correction_triangular(double e,double de)
		{
			if(e>=range[sets-1][2]||e<=range[0][0])//checks if the error overshoots the defined range
			{
				cout<<"!! Error Out Of Bounds For Triangular Functions !!"<<endl;//same can be done for gaussian functions but they will not cause large problems as they never give weight as zero 
				//in that case something can be added here so as to redefine of sets to prevent any error in the final correction
				return 0;
			}
			if(de>=range[sets-1][2]||de<=range[0][0])//same as above but for change_in_error
			{
				cout<<"!! Difference_Error Out Of Bounds For Triangular Functions !!"<<endl;
				//--------add something here for redefining sets in case of a problem---------
				return 0;
			}
			define_fuzzylogic();
			error=e;
			d_error=de;
			define_error_array_triangular();
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
				{
					numerator+=min_y[i][j]*give_centroid_triangular(fuzzy[i][j]);//calculation of numerator
					sum+=min_y[i][j];//calculation of sum(of minimum of weights)
				}
			return numerator/sum;// and finally the CORRECTION!!
		}		
};
int main()
{
	double p,q;
	p=20;
	q=20;
	Fuzzy mag(20.0,5);//(SIGMA,SETS,MULTIPLIER,CENTRE)
	cout<<"Error_triangular = "<<mag.correction_triangular(p,q)<<endl;
	cout<<"Error_gaussian = "<<mag.correction_gaussian(p,q)<<endl;
	Fuzzy mag2(20.0);//(SIGMA)
	Fuzzy mag3;//empty
	double a=-60.0,b=-40.0,c=-20.0;
	for(int i=0;i<5;i++)
	{
		mag2.append_set(a+20.0*i,b+20.0*i,c+20.0*i);
		mag3.append_set(b+20*i,20);
	}
	cout<<"Error_triangular = "<<mag2.correction_triangular(p,q)<<endl;
	cout<<"Error_gaussian = "<<mag2.correction_gaussian(p,q)<<endl;
	cout<<"Error_triangular = "<<mag3.correction_triangular(p,q)<<endl;
	cout<<"Error_gaussian = "<<mag3.correction_gaussian(p,q);
}
