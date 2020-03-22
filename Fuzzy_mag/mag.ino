float mag_val()
{
  float head;
  mag.getHeading(&mx,&my,&mz);
  head = atan2(my,mx);
  if(head<0)
    head+=2*M_PI;
  return (head*180/M_PI);
}
float mag_read()
{
  float sum1=0;
  for(int i=0;i<5;i++)
    sum1+=mag_val();
  return sum1/5.0;
}
void define_error()
{
  heading=mag_read();
  e=heading-initial_heading;
  de=e-pe;
  pe=e;
}
void cleararray()
{
  delete[] arr1;
  delete[] arr2;
  delete[] arr3;
}
