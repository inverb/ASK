#include<iostream>
#include<cstdarg>
using namespace std;

void funkcja( int *a)
{
	*a = 3;
	return;
}


int solve()
{
	int a = 2 , b = 1;
	int *s = &a, *s1 = &b;
	funkcja(s);
	funkcja(s1);
	cout<<a<<" "<<b;
	cout<<"\n\n";
	int n;
	cin>>n;
	switch(n){
		case 1: 
		case 2: cout<<"2";
		//break;
		case 3: cout<<"4";
	}
	
	return 0;
}

void fun(long n, ...)
{
	long wynik=0,i;
	va_list ap;
	va_start(ap,n);
	while(n--)
	{
		i=va_arg(ap,long);
		wynik+=i;
	}
	va_end(ap);
	cout<<wynik;
	return;
}

int main()
{
	fun(7,1,2,3,4,5,6,7,8);
	
	
	
	return 0;
}
