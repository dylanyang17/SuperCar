#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;

void readNum(int &x){
	x=0 ;
	char c =(char)getchar();
	while(c<'0'||c>'9'){
		c=(char)getchar() ;
	}
	while(c>='0'&&c<='9'){
		x=(x<<1)+(x<<3)+c-'0' ;
	}
}

int main(){
	freopen("message.in","r",stdin) ;
	freopen("message.out","w",stdout) ;
	return 0 ;
}
