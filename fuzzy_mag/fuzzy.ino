//void assign()
//{
//  for(int i=0;i<3;i++)
//  {
//    memcpy(temp[i],arr[i],3*sizeof(int));
//  }
//}
bool is_in(float e,int set[])
{
  if(e>set[0]&&e<set[2])
    return true;
  else return false;
}
float mf(float x,int t[])
{
  float y=0;
  if (x<=(t[0])||x>=(t[2]))
  {
    y=0;
    g=0;
  }
  else
  {
    g=1;
    if(x>(t[0])&&x<=(t[1]))
      y=(x-t[0])/(t[1]-t[0]);
    else
      y=(t[2]-x)/(t[2]-t[1]);
  }
  return y;
}
void copy(int d,int t[])
{
  for(int i=0;i<3;i++)
  {
    out[d][i]=t[i];
  }
}
void definerules()
{
  int d=0;
  if(is_in(e,temp[0])&&is_in(de,temp[0]))
  {
    copy(d,temp[2]);
    d++;
  }
  if(is_in(e,temp[0])&&is_in(de,temp[1]))
  {
    copy(d,temp[2]);
    d++;
  }
  if(is_in(e,temp[0])&&is_in(de,temp[2]))
  {
    copy(d,temp[1]);
    d++;
  }
  if(is_in(e,temp[1])&&is_in(de,temp[0]))
  {
    copy(d,temp[2]);
    d++;
  }
  if(is_in(e,temp[1])&&is_in(de,temp[1]))
  {
    copy(d,temp[1]);
    d++;
  }
  if(is_in(e,temp[1])&&is_in(de,temp[2]))
  {
    copy(d,temp[0]);
    d++;
  }
  if(is_in(e,temp[2])&&is_in(de,temp[0]))
  {
    copy(d,temp[1]);
    d++;
  }
  if(is_in(e,temp[2])&&is_in(de,temp[1]))
  {
    copy(d,temp[0]);
    d++;
  }
  if(is_in(e,temp[2])&&is_in(de,temp[2]))
  {
    copy(d,temp[0]);
    d++;
  }
}
void defuzzyfy()
{
  for(int i=0;i<3;i++)
  {
    float p;
    p=mf(e,temp[i]);
    if(g==1)
    {
      arr1[k]=p;
      k++;
    }
  }
  for(int i=0;i<3;i++)
  {
    float q;
    q=mf(e,temp[i]);
    if(g==1)
    {
      arr2[s]=q;
      s++;
    }
  }
  int d=0;
  for(int i=0;i<2;i++)
    for(int j=0;j<2;j++)
    {
      arr3[d]=mini(arr1[i],arr2[j]);
      d++;
    }
  for(int i=0;i<4;i++)
   sum+=arr3[i]; 
  for(int i=0;i<4;i++)
  {
    numerator+=arr3[i]*out[i][1];
  }
  output=numerator/sum;
}
float mini(float a,float b)
{
  if(a<=b)
    return a;
  else return b;
}
