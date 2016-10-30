//
//  cimpl_test.c
//  cimpl
//
//  Created by Nicholas Dwork on 10/29/16.
//  Copyright © 2016 Nicholas Dwork.
//

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "cimpl_test.h"
#include "cimpl.h"


int test_absImg(){
  cimpl_imgf myImg, result, answer;
  unsigned int M=2;
  unsigned int N=3;
  unsigned int out;
  myImg = cimpl_mallocImg(M,N);
  answer = cimpl_mallocImg(M,N);
  result = cimpl_mallocImg(M,N);

  for( unsigned int i=0; i<M*N; ++i ){
    myImg.data[i] = pow(-1,i) * i;
    answer.data[i] = i;
  }
  cimpl_absImg( myImg, &result );
  out = cimpl_equalImgs(result, answer);

  cimpl_freeImg(&myImg);
  cimpl_freeImg(&answer);
  return out;
}

int test_addImgs(){
  cimpl_imgf img1, img2, result, answer;
  unsigned int M=2;
  unsigned int N=3;
  unsigned int out;
  img1 = cimpl_mallocImg(M,N);
  img2 = cimpl_mallocImg(M,N);
  answer = cimpl_mallocImg(M,N);
  result = cimpl_mallocImg(M,N);

  for( unsigned int i=0; i<M*N; ++i ){
    img1.data[i] = i;
    img2.data[i] = i;
    answer.data[i] = 2*i;
  }
  cimpl_addImgs( img1, img2, &result );
  out = cimpl_equalImgs(result, answer);

  cimpl_freeImg(&img1);
  cimpl_freeImg(&img2);
  cimpl_freeImg(&answer);
  cimpl_freeImg(&result);
  return out;
}

int test_addScalar2Img(){
  cimpl_imgf myImg, result, answer;
  unsigned int M=2;
  unsigned int N=3;
  unsigned int out;
  myImg = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M,N);

  for( unsigned int i=0; i<M*N; ++i ){
    myImg.data[i] = i;
    answer.data[i] = i + 8;
  }
  cimpl_addScalar2Img(8, myImg, &result);
  out = cimpl_equalImgs(result, answer);

  cimpl_freeImg(&myImg);
  cimpl_freeImg(&answer);
  cimpl_freeImg(&result);
  return out;
}

int test_divideImgs(){
  cimpl_imgf img1, img2, result, answer;
  unsigned int M=2;
  unsigned int N=3;
  unsigned int out;
  img1 = cimpl_mallocImg(M, N);
  img2 = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);

  for( unsigned int i=1; i<=M*N; ++i ){
    img1.data[i-1] = 2*i;
    img2.data[i-1] = i;
    answer.data[i-1] = 2;
  }
  cimpl_divideImgs(img1, img2, &result);
  out = cimpl_equalImgs(result, answer);

  cimpl_freeImg(&img1);
  cimpl_freeImg(&img2);
  cimpl_freeImg(&result);
  cimpl_freeImg(&answer);
  return out;
}

int test_divideImgByScalar(){
  cimpl_imgf myImg, result, answer;
  unsigned int M=2;
  unsigned int N=3;
  float scalar = 2;
  unsigned int out;
  myImg = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);
  
  for( unsigned int i=1; i<=M*N; ++i ){
    myImg.data[i-1] = scalar * i;
    answer.data[i-1] = i;
  }
  cimpl_divideImgByScalar(myImg, scalar, &result);
  out = cimpl_equalImgs(result, answer);
  
  cimpl_freeImg(&myImg);
  cimpl_freeImg(&result);
  cimpl_freeImg(&answer);
  return out;
}

int test_multiplyImgs(){
  cimpl_imgf img1, img2, result, answer;
  unsigned int M=2;
  unsigned int N=3;
  unsigned int out;
  img1 = cimpl_mallocImg(M, N);
  img2 = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);
  
  for( unsigned int i=0; i<M*N; ++i ){
    img1.data[i] = i;
    img2.data[i] = i+1;
    answer.data[i] = i*(i+1);
  }
  cimpl_multiplyImgs(img1, img2, &result);
  out = cimpl_equalImgs(result, answer);
  
  cimpl_freeImg(&img1);
  cimpl_freeImg(&img2);
  cimpl_freeImg(&result);
  cimpl_freeImg(&answer);
  return out;
}

int test_multiplyImgByScalar(){
  cimpl_imgf myImg, result, answer;
  unsigned int M=2;
  unsigned int N=3;
  float scalar = 1.8;
  unsigned int out;
  myImg = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);

  for( unsigned int i=1; i<=M*N; ++i ){
    myImg.data[i-1] = i;
    answer.data[i-1] = scalar * i;
  }
  cimpl_multiplyImgByScalar(myImg, scalar, &result);
  out = cimpl_equalImgs(result, answer);
  
  cimpl_freeImg(&myImg);
  cimpl_freeImg(&result);
  cimpl_freeImg(&answer);
  return out;
}

int test_rot90(){
  cimpl_imgf myImg, result, answer;
  unsigned int M, N, out;
  float answerValues[6] = {4, 2, 0, 5, 3, 1};
  M=2; N=3;

  myImg = cimpl_mallocImg(M,N);
  result = cimpl_mallocImg(N,M);
  answer = cimpl_mallocImg(N,M);
  
  for( unsigned int i=0; i<myImg.h*myImg.w; ++i ){
    myImg.data[i] = i;
  }
  cimpl_rot90( myImg, &result );
  memcpy( answer.data, answerValues, sizeof(float)*answer.h*answer.w );
  out = cimpl_equalImgs(result, answer);

  cimpl_freeImg(&myImg);
  cimpl_freeImg(&result);
  cimpl_freeImg(&answer);
  return out;
}

int test_rot180(){
  cimpl_imgf myImg, result, answer;
  unsigned int M, N, out;
  M=2; N=3;

  myImg = cimpl_mallocImg(M,N);
  result = cimpl_mallocImg(M,N);
  answer = cimpl_mallocImg(M,N);
  
  for( unsigned int i=0; i<myImg.h*myImg.w; ++i ){
    myImg.data[i] = i;
    answer.data[i] = myImg.h*myImg.w-i-1;
  }
  cimpl_rot180( myImg, &result );
  out = cimpl_equalImgs(result, answer);

  cimpl_freeImg(&myImg);
  cimpl_freeImg(&result);
  cimpl_freeImg(&answer);
  return out;
}

int test_rot270(){
  cimpl_imgf myImg, result, answer;
  unsigned int M, N, out;
  float answerValues[6] = {1, 3, 5, 0, 2, 4};
  M=2; N=3;

  myImg = cimpl_mallocImg(M,N);
  result = cimpl_mallocImg(N,M);
  answer = cimpl_mallocImg(N,M);

  for( unsigned int i=0; i<myImg.h*myImg.w; ++i ){
    myImg.data[i] = i;
  }
  cimpl_rot270( myImg, &result );
  memcpy( answer.data, answerValues, sizeof(float)*answer.h*answer.w );
  out = cimpl_equalImgs(result, answer);

  cimpl_freeImg(&myImg);
  cimpl_freeImg(&result);
  cimpl_freeImg(&answer);
  return out;
}

int test_sumImg(){
  cimpl_imgf myImg;
  float result, answer;
  unsigned int M=2;
  unsigned int N=3;
  unsigned int out;
  myImg = cimpl_mallocImg(M,N);

  answer = 0;
  for( unsigned int i=0; i<M*N; ++i ){
    myImg.data[i] = (float) i/3;
    answer += (float) i/3;
  }
  result = cimpl_sumImg(&myImg);
  out = ( result == answer );
  
  cimpl_freeImg(&myImg);
  return out;
}



void cimpl_test(){

  if( test_absImg() ){
    printf("passed - cimpl_absImg\n");
  } else {
    printf("failed - cimpl_absImg\n");
  }

  if( test_addImgs() ){
    printf("passed - cimpl_addImgs\n");
  } else {
    printf("failed - cimpl_addImgs\n");
  }

  if( test_addScalar2Img() ){
    printf("passed - cimpl_addScalar2Img\n");
  } else {
    printf("failed - cimpl_addScalar2Img\n");
  }

  if( test_divideImgs() ){
    printf("passed - cimpl_divideImgs\n");
  } else {
    printf("failed - cimpl_divideImgs\n");
  }

  if( test_divideImgByScalar() ){
    printf("passed - cimpl_divideImgByScalar\n");
  } else {
    printf("failed - cimpl_divideImgByScalar\n");
  }

  if( test_multiplyImgs() ){
    printf("passed - cimpl_multiplyImgs\n");
  } else {
    printf("failed - cimpl_multiplyImgs\n");
  }

  if( test_rot90() ){
    printf("passed - cimpl_rot90\n");
  } else {
    printf("failed - cimpl_rot90\n");
  }

  if( test_rot180() ){
    printf("passed - cimpl_rot180\n");
  } else {
    printf("failed - cimpl_rot180\n");
  }

  if( test_rot270() ){
    printf("passed - cimpl_rot270\n");
  } else {
    printf("failed - cimpl_rot270\n");
  }

  if( test_sumImg() ){
    printf("passed - cimpl_sumImg\n");
  } else {
    printf("failed - cimpl_sumImg\n");
  }
  
}
