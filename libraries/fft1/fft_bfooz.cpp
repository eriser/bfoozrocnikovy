//
//	                 FFT library
//
//  (one-dimensional complex and real FFTs for array 
//  lengths of 2^n)
//
//	Author: Toth Laszlo (tothl@inf.u-szeged.hu)
//  
//	Research Group on Artificial Intelligence
//  H-6720 Szeged, Aradi vertanuk tere 1, Hungary
//	
//	Last modified: 97.05.29
/////////////////////////////////////////////////////////

#include <math.h>
#include <stdlib.h>
#include "fft_bfooz.h"

using namespace FFT;

FFT_bfooz::FFT_bfooz() {
	pi = 3.14;
}

/////////////////////////////////////////////////////////
// in-place Radix-2 FFT for complex values
// data: array of doubles:
// re(0),im(0),re(1),im(1),...,re(size-1),im(size-1)
// it means that size=array_length/2 !!
// 
// output is in similar order, normalized by array length
//
// Source: see the routines it calls ...


void FFT_bfooz::fft(double *data, long size){

	double *l, *end;

	dif_butterfly(data,size);
	unshuffle(data,size);

	end=data+size+size;
	for(l=data;l<end;l++){*l=*l/size;};
}

/////////////////////////////////////////////////////////
// in-place Radix-2 inverse FFT for complex values
// data: array of doubles:
// re(0),im(0),re(1),im(1),...,re(size-1),im(size-1)
// it means that size=array_length/2 !!
// 
// output is in similar order, NOT normalized by 
// array length
//
// Source: see the routines it calls ...

void FFT_bfooz::ifft(double* data, long size){

	unshuffle(data,size);
	inverse_dit_butterfly(data,size);
}

/////////////////////////////////////////////////////////
// Decimation-in-freq radix-2 in-place butterfly
// data: array of doubles:
// re(0),im(0),re(1),im(1),...,re(size-1),im(size-1)
// it means that size=array_length/2 !!
// 
// suggested use:
// intput in normal order
// output in bit-reversed order
//
// Source: Rabiner-Gold: Theory and Application of DSP,
// Prentice Hall,1978

void FFT_bfooz::dif_butterfly(double *data,long size){

	long angle,astep,dl;
	double xr,yr,xi,yi,wr,wi,dr,di,ang;
	double *l1, *l2, *end, *ol2;

     astep=1;
	 end=data+size+size;
     for(dl=size;dl>1;dl>>=1,astep+=astep){
					  l1=data;
					  l2=data+dl;
                      for(;l2<end;l1=l2,l2=l2+dl){
							ol2=l2;
                            for(angle=0;l1<ol2;l1+=2,l2+=2){
                               ang=2*pi*angle/size;
                               wr=cos(ang);
                               wi=-sin(ang);
                               xr=*l1+*l2;
                               xi=*(l1+1)+*(l2+1);
                               dr=*l1-*l2;
                               di=*(l1+1)-*(l2+1);
                               yr=dr*wr-di*wi;
                               yi=dr*wi+di*wr;
                               *(l1)=xr;*(l1+1)=xi;
                               *(l2)=yr;*(l2+1)=yi;
                               angle+=astep;
							   }
					  }
	 }
}

/////////////////////////////////////////////////////////
// Decimation-in-time radix-2 in-place inverse butterfly
// data: array of doubles:
// re(0),im(0),re(1),im(1),...,re(size-1),im(size-1)
// it means that size=array_length/2 !!
//
// suggested use:
// intput in bit-reversed order
// output in normal order
//
// Source: Rabiner-Gold: Theory and Application of DSP,
// Prentice Hall,1978

void FFT_bfooz::inverse_dit_butterfly(double *data,long size){

	long angle,astep,dl;
	double xr,yr,xi,yi,wr,wi,dr,di,ang;
	double *l1, *l2, *end, *ol2;

     astep=size>>1;
	 end=data+size+size;
     for(dl=2;astep>0;dl+=dl,astep>>=1){
					  l1=data;
					  l2=data+dl;
                      for(;l2<end;l1=l2,l2=l2+dl){
							ol2=l2;
                            for(angle=0;l1<ol2;l1+=2,l2+=2){
                               ang=2*pi*angle/size;
                               wr=cos(ang);
                               wi=sin(ang);
                               xr=*l1;
                               xi=*(l1+1);
                               yr=*l2;
                               yi=*(l2+1);
                               dr=yr*wr-yi*wi;
                               di=yr*wi+yi*wr;
                               *(l1)=xr+dr;*(l1+1)=xi+di;
                               *(l2)=xr-dr;*(l2+1)=xi-di;
                               angle+=astep;
							   }
					  }
	 }
}

/////////////////////////////////////////////////////////
// data shuffling into bit-reversed order
// data: array of doubles:
// re(0),im(0),re(1),im(1),...,re(size-1),im(size-1)
// it means that size=array_length/2 !!
//
// Source: Rabiner-Gold: Theory and Application of DSP,
// Prentice Hall,1978 

long bitreverse(long i, long size){

	long result,mask;

	result=0;
	for(;size>1;size>>=1){
		mask=i&1;
		i>>=1;
		result<<=1;
		result|=mask;
	}
	
/*     __asm{       the same in assebly
     mov eax,i
	 mov ecx,sizze
     mov ebx,0
   l:shr eax,1
     rcl ebx,1 
	 shr ecx,1
	 cmp ecx,1
     jnz l 
     mov result,ebx
     }*/
	 return result;
}

void FFT_bfooz::unshuffle(double *data, long size){

	long i,j,k,l,m;
	double re,im;

	//old version - turned out to be a bit slower
    for(i=0;i<size-1;i++){
             j=bitreverse(i,size);
             if (j>i){   //swap
                    re=data[i+i];im=data[i+i+1];
                    data[i+i]=data[j+j];data[i+i+1]=data[j+j+1];
                    data[j+j]=re;data[j+j+1]=im;
			 }
	}

/*
l=size-1;
m=size>>1;
for (i=0,j=0; i<l ; i++){
	  if (i<j){
				re=data[j+j]; im=data[j+j+1];
				data[j+j]=data[i+i]; data[j+j+1]=data[i+i+1];
				data[i+i]=re; data[i+i+1]=im;
				}
	  k=m;
	  while (k<=j){
				j-=k;
				k>>=1;	
				}
	  j+=k;
      }*/
}