//
//  cimpl_test.c
//  cimpl
//
//  Created by Nicholas Dwork on 10/29/16.
//  Copyright © 2016 Nicholas Dwork. All rights reserved.
//

#include <math.h>
#include <stdio.h>
#include "cimpl_test.h"
#include "cimpl.h"


int test_absImg(){
  cimpl_imgf myImg, result, answer;
  unsigned int M=2;
  unsigned int N=3;
  unsigned int out;

  myImg = cimpl_mallocImg(M,N);
  answer = cimpl_mallocImg(M,N);
  for( unsigned int i=0; i<M*N; ++i ){
    myImg.data[i] = pow(-1,i) * i;
    answer.data[i] = i;
  }
  result = cimpl_mallocImg(M,N);
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
  for( unsigned int i=0; i<M*N; ++i ){
    img1.data[i] = i;
    img2.data[i] = i;
    answer.data[i] = 2*i;
  }
  result = cimpl_mallocImg(M,N);
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
  for( unsigned int i=0; i<M*N; ++i ){
    myImg.data[i] = i;
    answer.data[i] = i + 8;
  }
  result = cimpl_mallocImg(M,N);
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
  unsigned int out;
  
  myImg = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);
  
  for( unsigned int i=1; i<=M*N; ++i ){
    myImg.data[i-1] = 2*i;
    answer.data[i-1] = i;
  }
  cimpl_divideImgByScalar(myImg, 2, &result);
  out = cimpl_equalImgs(result, answer);
  
  cimpl_freeImg(&myImg);
  cimpl_freeImg(&result);
  cimpl_freeImg(&answer);
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
}
