#pragma GCC optimize("O3")
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define wei 256
#define max(a,b) a>b?a:b
#define randx(x) rand()%x

struct bignum{
	int num[260];int n;
};
struct big_binary{
	int binary[2050];int n;
};

bignum store,store1,store2,store3,store4;// to store sth. to complete the function 
bignum asubone,qq,chch,answer,Barret;
FILE *p;
static int mul_ans[256][256];//store multiply answer
int kksk,cnt;
big_binary for_qp,mol_one,mol_two,Barret_b;
const int lighter[53]={3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251};
//small prime list 

void preset(){//previously calculate multiply answer
	int i,j;
	for(i=1;i<=255;i++){
		for(j=1;j<=255;j++){
			mul_ans[i][j]=i*j;
		}
	}
	return;
}

void clean(int *a,int n){//for 数组初始化 
	int i;int *aa=a;
	for(i=0;i<n;i++,aa++)*aa=0; 
	return;
}

void clean_big(bignum *a){//for big_num 初始化 
	int i;
	for(i=0;i<a->n;i++)a->num[i]=0;
	a->n=0;
	return;
} 

void clean_binary(big_binary *a){//for big_binary 初始化 
	int i;
	for(i=0;i<a->n;i++)a->binary[i]=0;
	a->n=0;
	return;
} 

void ctrlcv(bignum *a,bignum *b){//literally b=a
	int i;
	clean_big(b);b->n=a->n;
	for(i=0;i<a->n;i++)b->num[i]=a->num[i];
	return;
}

char st(int t){//convert number to alpha
	if(t<10)return t+'0';
	if(t==10)return 'A';
	if(t==11)return 'B';
	if(t==12)return 'C';
	if(t==13)return 'D';
	if(t==14)return 'E';
	if(t==15)return 'F';
}

void outprint(bignum *a){//256->16 printf
	int i,j;
	for(i=(a->n)-1;i>=0;i--){
		j=a->num[i]>>4;
		fprintf(p,"%c%c",st(j),st(a->num[i]-(j<<4))); 
	}
	fprintf(p,"\n");
	return;
}

void ftn(bignum *a,big_binary *b){//from 2(b) to 256(a)
	int i,j,h,m=b->n;
	clean_big(a);
	a->n=((m+7)>>3);
	for(i=0;i<a->n;i++){//超级拼装！！！ 
		h=i<<3;
		for(j=7;j>=0;j--){
			a->num[i]=(a->num[i]<<1)+b->binary[j+h];
		}
	}
	return;
}

void fnt(bignum *a,big_binary *b){//from 256(a) to 2(b)
	int i,j,k=0,n=a->n;
	clean_binary(b);
	for(i=0;i<n;i++){//超级拆解！！！ 
		k=a->num[i];
		for(j=0;j<8;j++){
			b->binary[(i<<3)+j]=k&1;
			k>>=1;
		}
	}
	b->n=n<<3;
	while(b->binary[b->n-1]==0)b->n--;
	return;
}

void add(bignum *a,bignum *b){//plus,from 0 to n-1 or m-1
	int i,n=a->n,m=b->n;
	for(i=0;i<n||i<m;i++){
		if(i<m)a->num[i]+=b->num[i];
		if(a->num[i]>=wei){
			a->num[i]-=wei;
			a->num[i+1]++;
		}
	}
	if(m>n)n=m;
	if(a->num[n])n++;//to work out wei
	a->n=n;
	return;
}

int cmp(bignum *a,bignum *b,int ad){//compare,ad's meaning sees below
	int i,n=a->n,m=b->n;
	if(n>m+ad)return 1;//a>b 1 a=b 0 a<b -1
	if(n<m+ad)return -1;
	for(i=n-1;i>=ad;i--){
		if(a->num[i]>b->num[i-ad])return 1;
		if(a->num[i]<b->num[i-ad])return -1;
	}
	return 0;
}

int cmp_binary(big_binary *a,big_binary *b,int ad){//compare(version binary)
	int i,n=a->n,m=b->n;
	if(n>m+ad)return 1;//a>b 1 a=b 0 a<b -1
	if(n<m+ad)return -1;
	for(i=n-1;i>=ad;i--){
		if(a->binary[i]>b->binary[i-ad])return 1;
		if(a->binary[i]<b->binary[i-ad])return -1;
	}
	return 0;
}

void sub_big(bignum *a,bignum *b,int ad){//subtract,from 0,and if a<b,them return 0,or if successfully run,then return 1
	if(cmp(a,b,ad)==-1)return;//ad means the position where begins the procedure
	int i,n=a->n,m=b->n;
	for(i=ad;i<n;i++){
		if(i-ad<m)a->num[i]-=b->num[i-ad];
		if(a->num[i]<0){
			a->num[i]+=256;
			a->num[i+1]--;
		}
	}
	while(a->num[n-1]==0&&n)n--;
	a->n=n;
}

int sub(big_binary *a,big_binary *b,int ad){//subtract,from 0,and if a<b,them return 0,or if successfully run,then return 1
	if(cmp_binary(a,b,ad)==-1)return 0;//ad means the position where begins the procedure
	int i,n=a->n,m=b->n;
	for(i=ad;i<n;i++){
		if(i-ad<m)a->binary[i]-=b->binary[i-ad];
		if(a->binary[i]<0){
			a->binary[i]+=2;
			a->binary[i+1]--;
		}
	}
	while(a->binary[n-1]==0&&n)n--;
	a->n=n;
	return 1;
}

void Barret_preset(bignum *mod){//devision for Barret_preset
	clean_binary(&mol_one);clean_binary(&Barret_b);//mol_one=256^128
	mol_one.n=((mod->n)<<4)+1;mol_one.binary[mol_one.n-1]=1;
	fnt(mod,&mol_two);//convert to bianry calculate
	int i=mol_one.n-mol_two.n;
	int j=i;
	while(i>=0){
		Barret_b.binary[i]=sub(&mol_one,&mol_two,i);
		i--;
	}
	while(j){
		if(Barret_b.binary[j]){
			Barret_b.n=j+1;break;
		}
		j--;
	}
	ftn(&Barret,&Barret_b);
	return;
}

void mul(bignum *a,bignum *b,bignum *ans){//multiply,the same as above
	int i,j,n=a->n,m=b->n;
	clean_big(ans);//the multiply ans can be at most n+m,so we need a bigger space for it
	for(i=0;i<n+m;i++){
		for(j=max(0,i-m+1);j<=i&&j<n;j++){
			ans->num[i]+=mul_ans[a->num[j]][b->num[i-j]]; 
		}
		if(ans->num[i]>=wei){
			j=ans->num[i]>>8;
			ans->num[i+1]+=j;
			ans->num[i]-=(j<<8);
		}
	}
	n=n+m;
	while(ans->num[n-1]==0&&n)n--;
	ans->n=n;
	return;
}

void mol(bignum *a,bignum *mod){//Barret
	clean_big(&store3);
	store3.n=a->n-mod->n+1;
	int i;
	for(i=mod->n-1;i<a->n;i++)store3.num[i-mod->n+1]=a->num[i];//a/(256^127)
	mul(&store3,&Barret,&store4);
	clean_big(&store3);
	store3.n=store4.n-mod->n-1;
	for(i=mod->n+1;i<store4.n;i++)store3.num[i-mod->n-1]=store4.num[i];
	mul(&store3,mod,&store4);
	if(cmp(&store4,a,0)==1){
		add(a,mod);sub_big(a,&store4,0);
	}
	else{
		sub_big(a,&store4,0);sub_big(a,mod,0);
	}
	return;
}

void mulmod(bignum *a,bignum *b,bignum *mod){
	mul(a,b,&store);
	mol(&store,mod);
	ctrlcv(&store,a);
	return;
} 

void div_small(bignum *a){//a/=2 
	int i,j=0,k,n=a->n;
	for(i=n-1;i>=0;i--){
		k=a->num[i]+(j<<8);
		a->num[i]=k>>1;
		j=k&1;
	}
	if(a->num[n-1])a->n=n;
	else a->n=n-1;
	return;
}

bool try_small(bignum *a,int s){//a%s(s<256),small prime check
	ctrlcv(a,&store2);
	int i,k,n=a->n;
	for(i=n-1;i>=0;i--){
		k=store2.num[i]+(store2.num[i+1]<<8);
		store2.num[i]=k%s;
	}
	if(store2.num[0]==0)return 1;
	else return 0;
}

void quickpow(bignum *a,bignum *mod,bignum *t){ //quickpow 
	int i,n=a->n,k=mod->n,tt=t->n;
	ctrlcv(a,&store2);
	fnt(t,&for_qp);//to work out the position we need to multiply an extra a
	for(i=for_qp.n-2;i>=0;i--){
		mulmod(a,a,mod);
		if(for_qp.binary[i])mulmod(a,&store2,mod);
	}
	return;
}

void make(bignum *a){// to work out k and q
	ctrlcv(a,&asubone);
	asubone.num[0]--;//to store n-1
	kksk=0;
	int i;
	ctrlcv(&asubone,&qq);
	while((qq.num[0]&1)==0){
		kksk++;
		div_small(&qq);
	}
	return;
}

void random(bignum *a,int n){//随机数 
	int i;
	clean_big(a);a->n=n;
	for(i=0;i<n-1;i++){
		a->num[i]=randx(256);
	}
	a->num[n-1]=randx(255)+1;//to ensure there exists no 0 in the first position
	return;
}

bool check(bignum *a){// for one time check Miller-Robin
	int i,m;
	//正文 
	clean_big(&chch);
	random(&chch,randx(120)+1);
	outprint(&chch);
	quickpow(&chch,a,&qq);//to work out a^q mod n
	if(chch.n==1&&chch.num[0]==1)return 1;
	if(cmp(&asubone,&chch,0)==0)return 1;//if the answer==n-1,return 1
	for(i=1;i<kksk;i++){//to work out a^(2^i*q)
		mulmod(&chch,&chch,a);
		if(cmp(&asubone,&chch,0)==0){//if the ans == n-1,return 1
			return 1; 
		}
		if(chch.n==1&&chch.num[0]==1)return 0;
	}
	return 0;
}

bool cprime(bignum *a){//main function
	fprintf(p,"第%d次尝试:\n小质数尝试:",cnt);
	int i;
	for(i=0;i<40;i++){//小质数尝试 
		if(try_small(a,lighter[i])){
			fprintf(p,"失败！\n");return 0;
		}
	} 
	fprintf(p,"成功！\n");
	Barret_preset(a);
	make(a);//to work out k and q 
	for(i=1;i<=10;i++){//test
		fprintf(p,"这是第%d次检验，随机数：\n",i); 
		if(!check(a)){
			fprintf(p,"结果：失败!\n");return 0;
		}
		fprintf(p,"结果：成功！\n");
	}
	return 1;
}

void makeit(){//make a random number 
	clean_big(&answer);
	random(&answer,127);
	answer.num[127]=randx(128)+128;
	if(answer.num[0]%2==0)answer.num[0]++;
	answer.n=128;
	return;
}

int main(){
	p=fopen("answer.txt","w"); 
	srand((unsigned)time(NULL));
	preset();//预处理 
	makeit();
	cnt=1; // how many times we have tried
	while(!cprime(&answer)){//continuously check whether a is a prime 
		makeit();cnt++;
	}
	fprintf(p,"The answer:\n");//print the answer
	outprint(&answer);
	return 0;
}
