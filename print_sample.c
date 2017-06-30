int global_variable;
int f1(int x,int y);
int f2();
float f3(float n);
int f4();
int main();

f1(int x,int y){
	int a,c;
	int b;
	a=a+c;
	x=y+3;
	b=b+6;
	a = f2();
	print(a);
	print(4.2);
	f2(f2());
	return a+c;
}

f2(){
	return 2000;
}

f3(float n){
	return 3.6;
}



main(){
	int a,b,c;
	float d;
	b = f2();
	 c = f1(c+c,c+c);
	 d=f3(d);
	print("hi");
	f1(1,2);
	f2();
	 d = 20.22;
	 d = 543.6 + d;
	global_variable = a+b;
	print("a : ");
	print(a);
	print("\nb : ");
	print(d);
	a = f1(a,b);
	print("\nc : ");
	print(c);
	print("\nd : ");
	print(d);
	print("\nglobal_variable : ");
	print(global_variable);
	print("\n");
	// return a;
	if(a == 1) a = 0;
	else if(b == 1) a = 1;
	else if(c == 1) a = 2;
	else a = 3;
	return 0;	
}