#include<bits/stdc++.h>
using namespace std;

void p(int &x)
{
	x+=3;
	return;
}
int main()
{
	int x=4;
	p(x);
	cout<<x<<endl;
	return 0;
}