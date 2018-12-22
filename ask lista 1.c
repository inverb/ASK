#include<stdio.h>
#include<limits.h>

int main()
{
	printf("%d %d\n", -INT_MIN, INT_MAX);
	int a=INT_MAX, b=INT_MIN;
	a=a+b;
	b=a-b;
	a=a-b;
	printf("%d %d\n", a, b);
	
	
	return 0;
}
