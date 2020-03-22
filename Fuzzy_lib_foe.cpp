#include<iostream>
#include<cmath>
#define exp 2.7132
#define max_n 9
using namespace std;
class Fuzzy
{
	public:
		double multiplier;// Difference between sets
		double centre;// Centre of centre set
		int sets;// Number of sets
		double sigma[max_n];//Value of sigma for each set
		double error_y[max_n];//Weight of error in each Sets
		double d_error_y[max_n];//Weight of d_error in each Sets
		double min_y[max_n][max_n];//Minimum value of each possible pair of error and d_error
		double range[max_n][3];//Array of extreme and centre points of sets
		int fuzzy[max_n][max_n];//Fuzzy Logic
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
			for(int i=0;i<sets;i++)								//*These lines copies the upper triangular the the left out lower portion
				for(int j=0;j<i;j++)							//
					fuzzy[i][j]=fuzzy[j][i];					//
			fuzzy[1][1]=sets-1;				//This line modifies the (2,2) element in the fuzzy set. This line is separate because of its inconsistency to the symmetricity of the logic which could not be incorporated in the oiginal logic above
			fuzzy[sets-2][sets-2]=0;		//Same as the above but modifies the (sets-1,sets-1) i.e. the second from last element on the main diagonal
			numerator=0.0;					//Initialising numerator (in correction)
			sum=0.0;						//Initialising sum (denominator in correction)
		}
		//constructor
		//CONSTRUCTOR NUMBER ONE - for symettric sets (both Tringular and Gaussian)
		Fuzzy(double s,double m=2.0,int n=5,double c=0.0) //for triangular functions, set multiplier between 1.0 and 3.0 for best results
		{
			if(s<=0.0)//Checks if sigma is less than zero
			{
				//Serial.println("!! Invalid Sigma (<=0) !! - Changing Sigma to 10");
				s=10.0;//if so gives a default value so as to continue the operation with a optimum value applicable in most situations
			}
			if(m<=0.0)//Checks if multiplier is less than zero
			{
				//Serial.println("!! Invalid Multiplier (<0) !! - Changing Multiplier to 2.0");
				m=2.0;//if so gives it the value 2 i.e. the standart distribution of sets
			}
			if(n<=0)//Checks if the number of given sets are less than zero
			{
				//Serial.println("!! Invalid Number of Sets(<0) !! - Changing Number of Sets to 5");
				n=max_n;//if so sets the sets to maximum number of sets defines at the top for most accurate corretion possible (but may increase compilation time)
			}
			sets=n;			//skip
			multiplier=m/2;	//nothing here
			centre=c;		//
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
		}
		//CONSTRUCTOR NUMBER 2 - for discreet/unsymettric sets + symettric gaussian function
		Fuzzy(double **a,int n,double s)//a is a nX3 matrix with the columns containing (the_left_end_value, the_centre_value, the_right_end_value)
		{
			if(s<=0.0)//Checks if sigma is less than zero
			{
				//Serial.println("!! Invalid Sigma (<=0) !! - Changing Sigma to 10");
				s=10.0;//if so gives a default value so as to continue the operation with a optimum value applicable in most situations
			}
			if(n<=0)//Checks if the number of given sets are less than zero
			{
				//Serial.println("!! Invalid Number of Sets(<0) !! - Changing Number of Sets to 5");
				n=max_n;//if so sets the sets to maximum number of sets defines at the top for most accurate corretion possible (but may increase compilation time)
			}
			sets=n;
			for(int i=0;i<sets;i++)
			{
				sigma[i]=s;
				if(a[i][1]<a[i-1][1])//checks if the sets are given in ascending order or not - It is required because the library assumes that the user will do so mainly to save precious compilation time
				{
					//Serial.println("!! Invalid Order of Sets - Input The Sets In Ascending Order!!");//if you want you can set a operation in case this problem is dected
																									 //perhaps a exception handling statement or a sorting loop
				}			
				range[i][0]=a[i][0];//copies value from input set
				range[i][1]=a[i][1];//"""""""
				range[i][2]=a[i][2];//"""""""
			}
		}
		//CONSTRUCTOR NUMBER 3 - for discreet/unsymmetric gaussian functions 
		Fuzzy(double** a,int n)//a is a nX2 matrix with the first column containing the centre of sets and the second one containing the corresponding standard-deviation(sigma) 
		{
			if(n<=0)//Checks if the number of given sets are less than zero
			{
				//Serial.println("!! Invalid Number of Sets(<0) !! - Changing Number of Sets to 5");
				n=max_n;//if so sets the sets to maximum number of sets defines at the top for most accurate corretion possible (but may increase compilation time)
			}
			sets=n;
			for(int i=0;i<sets;i++)
			{
				if(a[i][1]<=0)//Checks if sigma is less than zero
				{
					//Serial.println("!! Invalid Sigma (<=0) !! - Changing Sigma to 10");//if so gives a default value so as to continue the operation with a optimum value applicable in most situations
					a[i][1]=10.0;;
				}
				sigma[i]=a[i][1];
				if(a[i][0]<a[i-1][0])//checks if the sets are given in ascending order or not - It is required because the library assumes that the user will do so mainly to save precious compilation time
				{
					//Serial.println("!! Invalid Order of Sets - Input The Sets In Ascending Order!!");
					//--------------add something here----------------
				}
				range[i][1]=a[i][0];		//sets the centre value
				range[i][0]=a[i][0]-a[i][1];//sets the left_end_value
				range[i][2]=a[i][0]+a[i][1];//sets the right_end_value
			}
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
			else if(e>(range[i][0])&&e<range[i][1])//second half of set
				return (e-range[i][0])/(range[i][1]-range[i][0]);
			else return 0.0;
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
					numerator+=min_y[i][j]*range[fuzzy[i][j]][1];//calculation of numerator
					sum+=min_y[i][j];//calculation of sum(of minimum of weights)
				}
			return numerator/sum;// and finally the CORRECTION!!
		}
		//the main function returns the value of correction TRIANGULAR
		double correction_triangular(double e,double de)
		{
			if(e>=range[sets-1][2]||e<=range[0][0])//checks if the error overshoots the defined range
			{
				//Serial.println("!! Error Out Of Bounds For Triangular Functions !!)");//same can be done for gaussian functions but they will not cause large problems as they never give weight as zero 
				//in that case something can be added here such as a redefinement of sets to prevent any error in the final correction
			}
			if(de>=range[sets-1][2]||de<=range[0][0])//same as above bus for change_in_error
			{
				//Serial.println("!! Difference_Error Out Of Bounds For Triangular Functions !!");
				//--------add something here for redefining sets in case of a problem---------
			}
			define_fuzzylogic();
			error=e;
			d_error=de;
			define_error_array_triangular();
			for(int i=0;i<sets;i++)
				for(int j=0;j<sets;j++)
				{
					numerator+=min_y[i][j]*range[fuzzy[i][j]][1];//calculation of numerator
					sum+=min_y[i][j];//calculation of sum(of minimum of weights)
				}
			return numerator/sum;// and finally the CORRECTION!!
		}		
};
int main()
{
	double p,q;
	p=59.9;
	q=59.9;
	Fuzzy mag(20.0);//(SIGMA,MULTIPLIER,SETS,CENTRE)
	cout<<"Error_triangular = "<<mag.correction_triangular(p,q)<<endl;
	cout<<"Error_gaussian = "<<mag.correction_gaussian(p,q)<<endl;
	double **a;
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
	cout<<"Error_triangular = "<<mag2.correction_triangular(p,q)<<endl;
	cout<<"Error_gaussian = "<<mag2.correction_gaussian(p,q)<<endl;
	double **b;
	b=new double*[5];
	for(int i=0;i<5;i++)
		b[i]=new double[3];
	b[0][0]=-40;
	b[0][1]=20;
	for(int i=1;i<5;i++)
	{
		b[i][0]=b[i-1][0]+20;
		b[i][1]=20;
	}
	Fuzzy mag3(b,5);//(SETS**,NSETS)
	cout<<"Error_triangular = "<<mag3.correction_triangular(p,q)<<endl;
	cout<<"Error_gaussian = "<<mag3.correction_gaussian(p,q);
}
