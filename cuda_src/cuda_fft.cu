#include <iostream>
#include <fstream>
#include <cuda.h>
#include <cmath>
#include <algorithm>
#include <stdio.h>
#include <iomanip>
#include "common.h"
#include "cmdline.h"

using namespace std;
typedef double Type;
const Type pi=3.1415926535897932384626433832795028;

__global__ void fft(Type* devI_r,Type* devI_i,Type* devW_r,Type* devW_i,int nElem, int l, int s, int m){
	const int tid = threadIdx.x + blockIdx.x * blockDim.x;
	int wid;
	int p,q;
	Type Tp_r,Tp_i,Tq_r,Tq_i,Wr,Wi,TqW_r,TqW_i;
	wid = tid + s*nElem/2;
	p = 2*wid-wid % m-nElem*s;
	q = p + m;
	Wr = devW_r[wid];
	Wi = devW_i[wid];
	Tp_r = devI_r[p];
	Tp_i = devI_i[p];
	Tq_r = devI_r[q];
	Tq_i = devI_i[q];
	TqW_r = Tq_r*Wr - Tq_i*Wi;
	TqW_i = Tq_r*Wi + Tq_i*Wr;
	devI_r[p] = Tp_r + TqW_r;
	devI_i[p] = Tp_i + TqW_i;

	devI_r[q] = Tp_r - TqW_r;
	devI_i[q] = Tp_i - TqW_i;
}

void bit_reverse(Type *A,int sz){
    for(int i=0,j=0;i<sz-1;++i){
        if(i<j) {swap(A[i],A[j]);}
         int k=sz/2;
        while(k<=j){
            j-=k;
            k/=2;
        }
        j+=k;
    }
}

void w_fun(Type *W_r, Type *W_i, int nElem){
	int s=0;
    for(int m=2; m<=nElem; m*=2){
    	int i=0;
        for(int k=0; k<nElem; k+=m){
            for(int j=0;2*j<m;j++){
                W_r[s*nElem/2+i] =  cos(2*pi*(k+j)/m);
                W_i[s*nElem/2+i] = -sin(2*pi*(k+j)/m);
                i++;
            }
        }
        s++;
    }
}

void InitialData(Type *I_r,Type *I_i,int nElem,string data_file){
	FILE* f;
	const char* file_name = data_file.c_str();
	f = fopen(file_name,"r+");
	int i=0;
	while(i<nElem && fscanf(f,"%lf",I_r+i)==1){
		++i;
		I_i[i] = 0;
	}
    bit_reverse(I_r,nElem);
    bit_reverse(I_i,nElem);
}
int main(int argc, char *argv[]){
	cmdline::parser a;
	a.add<string>("data", 'd', "data file", true, "");
	a.add<int>("num", 'n', "data number", false, 20, cmdline::range(2, 23));
	a.add<string>("result", 'r', "result", false, "./fftresult.txt");

	a.parse_check(argc, argv);
	int N,l;
	l = a.get<int>("num");
	string data_file = a.get<string>("data");
	string result_file = a.get<string>("result");
	N = 1<<l;
	Type *W_r,*W_i,*I_r,*I_i;
	unsigned int flags = cudaHostAllocMapped;
	CHECK( cudaHostAlloc( (Type**)&W_r,sizeof(Type)*l*N/2+100,flags) );
	CHECK( cudaHostAlloc( (Type**)&W_i,sizeof(Type)*l*N/2+100,flags) );
	CHECK( cudaHostAlloc( (Type**)&I_r,sizeof(Type)*N+100,flags) );
	CHECK( cudaHostAlloc( (Type**)&I_i,sizeof(Type)*N+100,flags) );
    w_fun(W_r,W_i,N);
    InitialData(I_r,I_i,N,data_file);
    Type *devI_r, *devI_i;


    CHECK( cudaMalloc( (Type**)&devI_r, sizeof(Type)*N+100 ) );
    CHECK( cudaMalloc( (Type**)&devI_i, sizeof(Type)*N+100 ) );
    Type *devW_r, *devW_i;
    CHECK( cudaMalloc( (Type**)&devW_r, sizeof(Type)*l*N/2+100 ) );
    CHECK( cudaMalloc( (Type**)&devW_i, sizeof(Type)*l*N/2+100 ) );
    CHECK( cudaMemcpy(devI_r, I_r, sizeof(Type)*N+100, cudaMemcpyHostToDevice) );
    CHECK( cudaMemcpy(devI_i, I_i, sizeof(Type)*N+100, cudaMemcpyHostToDevice) );
    CHECK( cudaMemcpy(devW_r, W_r, sizeof(Type)*l*N/2+100, cudaMemcpyHostToDevice) );
    CHECK( cudaMemcpy(devW_i, W_i, sizeof(Type)*l*N/2+100, cudaMemcpyHostToDevice) );
    cudaFreeHost(W_r);
    cudaFreeHost(W_i);
    int m=1;
	int iStart,iEnd;
	double Time = 0;
	iStart = clock();
    for (int s=0;s<l;s++){
        if(N<=1024)
        	fft<<<1,N/2>>>(devI_r,devI_i,devW_r,devW_i,N,l,s,m);
        else
        	fft<<<N/1024,512>>>(devI_r,devI_i,devW_r,devW_i,N,l,s,m);
        m*=2;
    }
    cudaDeviceSynchronize();
    iEnd = clock();
    Time = (iEnd - iStart);
    Time=(Time+0.0)/(CLOCKS_PER_SEC);
    double onceTime = Time/N;
    int p1 = data_file.find_last_of('/');
    string dir_loc = data_file.substr(0,p1);
    string r_file = dir_loc + "/fft_accel_tmp";
    const char * target_file = r_file.c_str();
    FILE *f;
    f = fopen(target_file,"w+");
    fprintf(f,"%f\t%d\t%.12f\n",Time,N,onceTime);
    fclose(f);
    cudaFree(devW_r);
    cudaFree(devW_i);
    CHECK( cudaMemcpy(I_r, devI_r, sizeof(Type)*N+100, cudaMemcpyDeviceToHost) );
    CHECK( cudaMemcpy(I_i, devI_i, sizeof(Type)*N+100, cudaMemcpyDeviceToHost) );
    ofstream outFile(result_file);

	for(int i=0;i<N;i++){
		outFile<<setprecision(10)<<I_r[i]<<" + "<<I_i[i]<<"i\n";
	}
    cudaFree(devI_r);
    cudaFree(devI_i);
    cudaFreeHost(I_r);
    cudaFreeHost(I_i);
    cudaDeviceReset();
    return 0;
}
