#ifndef CLASS4_H
#define CLASS4_H
#include "class2.h"
#include <cmath>
const double pi=2*asin(1);
template <class T>
class vector
{
	public:
		T x,y;
		vector(T a,T b);
		~vector(){};
		T operator*(const vector<T>&);
		vector<T> operator-(const vector<T>&);
		void operator+=( vector<T>&);
		vector<T> norm()const;
		double mag()const;
		double angle()const;
		void rotate(double angle);
		
};


template <class T>
vector<T>::vector(T a,T b):x(a),y(b)
{
		;
}
template <class T>
T vector<T>::operator*(const vector<T>& m)
{
	return this->x*m.x+this->y*m.y;	
}
template <class T>
vector<T> vector<T>::operator-(const vector<T>& m)
{
	vector<T> tmp(0,0);
	tmp.x=x-m.x;
	tmp.y=y-m.y;
	return tmp;
 } 
 template <class T>
 double vector<T>::mag() const
 {
 	return sqrt((*this)*(*this));
 }
template <class T>
vector<T> vector<T>::norm() const
{
	vector<T> tmp(0,0);
	tmp.x=x/mag();
	tmp.y=y/mag();
	return tmp;
}
template <class T>
void vector<T>::operator+=( vector<T>& m)
{
	m.x+=x;
	m.y+=y;
	return ;
}  
template <class T>
double vector<T>::angle() const
{
	
	double tmp;
	if(x!=0)
	{
		tmp=double(180/pi*atan((double)y/x));
	}
	if(x==0&&y>0)
		return 180.0;
	else if(x==0&&y<=0)
		return 0.0;
	else if(x>0)
		return 90.0+tmp;
	else if(x<0)
		return 270+tmp;
}
template <class T>
void vector<T>::rotate(double angle)
{
	T tmp=x;
	x=x*cos(angle*pi/180.0)-y*sin(angle*pi/180);
	y=y*cos(angle*pi/180.0)+tmp*sin(angle*pi/180);
//	cout<<angle<<endl;
//	cout<<"x: "<<x<<" y: "<<y<<endl;
	return;
}

#endif
