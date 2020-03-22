#include<iostream>
using namespace std;
class Fuzzy
{
	public:
		int sets;
		int **fuzzy;
		Fuzzy(int s)
		{
			sets=s;
			fuzzy=new int*[sets];
			for(int i=0;i<sets;i++)
				fuzzy[i]=new int[sets];
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
						while(j<int((sets+sets%2)/2));
					}
					fuzzy[i][j]=fuzzy[i][j-1]-1;
					k++;
				}
			for(int i=0;i<sets;i++)
				for(int j=0;j<i;j++)
					fuzzy[i][j]=fuzzy[j][i];
		}
};
int main()
{
	int no;
	no=7;
	Fuzzy fo(no);
	for(int i=0;i<no;i++)
	{
		for(int j=0;j<no;j++)
			cout<<fo.fuzzy[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
	int ne;
	ne=6;
	Fuzzy fe(ne);
	for(int i=0;i<ne;i++)
	{
		for(int j=0;j<ne;j++)
			cout<<fe.fuzzy[i][j]<<" ";
		cout<<endl;
	}
}
