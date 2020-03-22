#include <I2Cdev.h>
#include <Wire.h>
#include "HMC5883L.h"
int temp[3][3]={{-40,-20,0},{-20,0,20},{0,20,40}};
float arr1[2]={0.0},arr2[2]={0.0},arr3[4]={0.0};
int k=0,s=0,g=0;
int out[4][3];
float e,de,output=0,sum=0,numerator=0;;
int16_t mx,my,mz;
int ml_a=A0,ml_b=A1,mr_a=24,mr_b=26,ml_pwm=9,mr_pwm=13;
float v=50.0;
float initial_heading,heading,pe=0;
HMC5883L mag;
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(ml_a,OUTPUT);
  pinMode(ml_b,OUTPUT);
  pinMode(ml_pwm,OUTPUT);
  pinMode(mr_a,OUTPUT);
  pinMode(mr_b,OUTPUT);
  pinMode(mr_pwm,OUTPUT);
  mag.initialize();
  initial_heading=mag_read();
}
void loop()
{
  define_error();
  definerules();
  defuzzyfy();
  Serial.print("Error = ");
  Serial.print(e);
  Serial.print("  ");
  Serial.print("Error_Difference = ");
  Serial.print(de);
  Serial.print("  ");
  Serial.print("Sum = ");
  Serial.print(sum);
  Serial.print("  ");
  Serial.print("Numerator = ");
  Serial.print(numerator);
  Serial.print("  ");
  Serial.print("Output = ");
  Serial.println(output);
  digitalWrite(mr_a,LOW);
  digitalWrite(mr_b,HIGH);
  analogWrite(mr_pwm,v-output);
  digitalWrite(ml_a,LOW);
  digitalWrite(ml_b,HIGH);
  analogWrite(ml_pwm,v+output);
  cleararray();
}

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTY2MjY3NDA5Nl19
-->