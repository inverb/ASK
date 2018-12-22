#include<stdio.h>

//zadanie 4
union elem{
	struct{
		long *p;
		long y;
	};
	struct{
		long x;
		union elem *next;
	};
};

void proc(union elem *i)
{
	union elem *j = i->next;
	long *k = j->p;
	i -> x = *k - j->y;
	return;
}

int zad4()
{
	union elem e,e1;
	e.x=1;
	e1.y=3;
	long p=5;
	e1.p=&p;
	e.next=&e1;
	proc(&e);
	//printf("%d", e.x);
	return 0;
}

//zadanie 5

typedef struct{
	long u[2];
	long *v;
}SA;
typedef struct{
	long p[2];
	long q;
}SB;

SB eval(SA s)
{
	SB sb;
	(sb.p)[0]= (*(s.v)) * (s.u)[1];
	(sb.p)[1]= (s.u)[0] - (*(s.v));
	sb.q= (s.u)[0]-(s.u)[1];
	return sb;
}

long wrap(long x, long y, long z)
{
	SA sa;
	(sa.u)[0]=x;
	(sa.u)[1]=y;
	*sa.v=z;
	SB sb = eval(sa);
	x=(sb.p[0]+sb.p[1])*sb.q;
	return x;
}

//zadanie 6

typedef struct P{
	long n;
	int tab[10];
} PP;

float puzzle6(struct P *p, float f)
{
	float w=0, x=0x3f800000;
	long n=p->n;
	int i;
	for(i=0; i<n; i++)
	{
		w+=p->tab[i]*x;
		x*=f;
	}
	return w;
}

int zadanie6()
{
	PP p;
	p.n=10;
	int i;
	float f=1.0000;
	for(i=0; i<10; i++) p.tab[i]=i;
	float w=puzzle6(&p,f);
	printf("%f", w);
	return 0;
}

//zadanie 7

float puzzle7(float a, float b)
{
	int aprim=a*0.159154936; //1/2pi
	float c=a-(aprim*6.283185482); //2pi
	if(c>3.141592741) c-=6.283185482;
	float licznik=c, mianownik=1,l=2,w=c,d;
	do
	{
		printf("%f ", w);
		licznik*=(-c*c);
		mianownik*=l*(l+1);
		l+=2;
		d=licznik/mianownik;
		w+=d;
		float dprim=0x7fffffff;
		if(d<0) d=-d;
	}while(d>=b);
	return w;
}

int main()
{
	float a=1.5707963, b=0.00001;
	float w=puzzle7(a,b);
	printf("%f", w);
	return 0;
}
