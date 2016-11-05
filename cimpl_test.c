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
  size_t H=2;
  size_t W=3;
  int out;
  myImg = cimpl_mallocImg(H,W);
  answer = cimpl_mallocImg(H,W);
  result = cimpl_mallocImg(H,W);

  for( size_t i=0; i<H*W; ++i ){
    myImg.data[i] = pow(-1,i) * i;
    answer.data[i] = i;
  }
  cimpl_absImg( myImg, &result );
  out = cimpl_equalImgs(result, answer);

  cimpl_freeImg(&myImg);
  cimpl_freeImg(&result);
  cimpl_freeImg(&answer);
  return out;
}

int test_absVol(){
  cimpl_volf myVol, result, answer;
  size_t H=2;
  size_t W=3;
  size_t S=4;
  int out;
  myVol = cimpl_mallocVol(H,W,S);
  answer = cimpl_mallocVol(H,W,S);
  result = cimpl_mallocVol(H,W,S);
  
  for( size_t i=0; i<H*W*S; ++i ){
    myVol.data[i] = pow(-1,i) * i;
    answer.data[i] = i;
  }
  cimpl_absVol( myVol, &result );
  out = cimpl_equalVols(result, answer);
  
  cimpl_freeVol(&myVol);
  cimpl_freeVol(&result);
  cimpl_freeVol(&answer);
  return out;
}

int test_addImgs(){
  cimpl_imgf img1, img2, result, answer;
  size_t H=2;
  size_t W=3;
  int out;
  img1 = cimpl_mallocImg(H,W);
  img2 = cimpl_mallocImg(H,W);
  answer = cimpl_mallocImg(H,W);
  result = cimpl_mallocImg(H,W);

  for( size_t i=0; i<H*W; ++i ){
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

int test_addVols(){
  cimpl_volf vol1, vol2, result, answer;
  size_t H=2;
  size_t W=3;
  size_t S=3;
  int out;
  vol1 = cimpl_mallocVol(H,W,S);
  vol2 = cimpl_mallocVol(H,W,S);
  answer = cimpl_mallocVol(H,W,S);
  result = cimpl_mallocVol(H,W,S);
  
  for( size_t i=0; i<H*W*S; ++i ){
    vol1.data[i] = i;
    vol2.data[i] = i;
    answer.data[i] = 2*i;
  }
  cimpl_addVols( vol1, vol2, &result );
  out = cimpl_equalVols(result, answer);
  
  cimpl_freeVol(&vol1);
  cimpl_freeVol(&vol2);
  cimpl_freeVol(&answer);
  cimpl_freeVol(&result);
  return out;
}

int test_addScalar2Img(){
  cimpl_imgf myImg, result, answer;
  size_t M=2;
  size_t N=3;
  int out;
  myImg = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M,N);

  for( size_t i=0; i<M*N; ++i ){
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

int test_addScalar2Vol(){
  cimpl_volf myVol, result, answer;
  size_t H=2;
  size_t W=3;
  size_t S=4;
  int out;
  myVol = cimpl_mallocVol(H,W,S);
  result = cimpl_mallocVol(H,W,S);
  answer = cimpl_mallocVol(H,W,S);

  for( size_t i=0; i<H*W*S; ++i ){
    myVol.data[i] = i;
    answer.data[i] = i + 8;
  }
  cimpl_addScalar2Vol(8, myVol, &result);
  out = cimpl_equalVols(result, answer);

  cimpl_freeVol(&myVol);
  cimpl_freeVol(&answer);
  cimpl_freeVol(&result);
  return out;
}

int test_divideImgs(){
  cimpl_imgf img1, img2, result, answer;
  size_t M=2;
  size_t N=3;
  int out;
  img1 = cimpl_mallocImg(M, N);
  img2 = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);

  for( size_t i=1; i<=M*N; ++i ){
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
  size_t M=2;
  size_t N=3;
  float scalar = 2;
  int out;
  myImg = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);
  
  for( size_t i=1; i<=M*N; ++i ){
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

int test_flipImgLR(){
  cimpl_imgf myImg, result, answer;
  int out;
  myImg = cimpl_mallocImg(3, 3);
  result = cimpl_mallocImg(3, 3);
  answer = cimpl_mallocImg(3, 3);

  for( int x=0; x<myImg.w; ++x ){
    for( int y=0; y<myImg.h; ++y ){
      myImg.data[y+x*myImg.h] = y+x*myImg.h;
      answer.data[y+x*myImg.h] = y+(myImg.w-x-1)*myImg.h;
  } }
  cimpl_flipImgLR(myImg, &result);
  out = cimpl_equalImgs(result, answer);
  
  return out;
}

int test_flipImgUD(){
  cimpl_imgf myImg, result, answer;
  int out;
  myImg = cimpl_mallocImg(3, 3);
  result = cimpl_mallocImg(3, 3);
  answer = cimpl_mallocImg(3, 3);
  
  for( int x=0; x<myImg.w; ++x ){
    for( int y=0; y<myImg.h; ++y ){
      myImg.data[y+x*myImg.h] = y+x*myImg.h;
      answer.data[y+x*myImg.h] = (myImg.h-y-1)+x*myImg.h;
    } }
  cimpl_flipImgUD(myImg, &result);
  out = cimpl_equalImgs(result, answer);

  return out;
}

int test_multiplyImgs(){
  cimpl_imgf img1, img2, result, answer;
  size_t M=2;
  size_t N=3;
  int out;
  img1 = cimpl_mallocImg(M, N);
  img2 = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);
  
  for( size_t i=0; i<M*N; ++i ){
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
  size_t M=2;
  size_t N=3;
  float scalar = 1.8;
  int out;
  myImg = cimpl_mallocImg(M, N);
  result = cimpl_mallocImg(M, N);
  answer = cimpl_mallocImg(M, N);

  for( size_t i=1; i<=M*N; ++i ){
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
  size_t M, N;
  int out;
  float answerValues[6] = {4, 2, 0, 5, 3, 1};
  M=2; N=3;

  myImg = cimpl_mallocImg(M,N);
  result = cimpl_mallocImg(N,M);
  answer = cimpl_mallocImg(N,M);
  
  for( size_t i=0; i<myImg.h*myImg.w; ++i ){
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
  size_t M, N;
  int out;
  M=2; N=3;

  myImg = cimpl_mallocImg(M,N);
  result = cimpl_mallocImg(M,N);
  answer = cimpl_mallocImg(M,N);
  
  for( size_t i=0; i<myImg.h*myImg.w; ++i ){
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
  size_t M, N;
  int out;
  float answerValues[6] = {1, 3, 5, 0, 2, 4};
  M=2; N=3;

  myImg = cimpl_mallocImg(M,N);
  result = cimpl_mallocImg(N,M);
  answer = cimpl_mallocImg(N,M);

  for( size_t i=0; i<myImg.h*myImg.w; ++i ){
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
  size_t H=2;
  size_t W=3;
  int out;
  myImg = cimpl_mallocImg(H,W);

  answer = 0;
  for( size_t i=0; i<H*W; ++i ){
    myImg.data[i] = (float) i/3;
    answer += (float) i/3;
  }
  result = cimpl_sumImg(&myImg);
  out = ( result == answer );
  
  cimpl_freeImg(&myImg);
  return out;
}

int test_sumVol(){
  cimpl_volf myVol;
  float result, answer;
  size_t H=2;
  size_t W=3;
  size_t S=3;
  int out;
  myVol = cimpl_mallocVol(H, W, S);

  answer = 0;
  for( size_t i=0; i<H*W*S; ++i ){
    myVol.data[i] = (float) i/3;
    answer += (float) i/3;
  }
  result = cimpl_sumVol(&myVol);
  out = ( result == answer );

  cimpl_freeVol(&myVol);
  return out;
}



void cimpl_test(){

  if( test_absImg() ){
    printf("passed - cimpl_absImg\n");
  } else {
    printf("failed - cimpl_absImg\n");
  }

  if( test_absVol() ){
    printf("passed - cimpl_absVol\n");
  } else {
    printf("failed - cimpl_absVol\n");
  }

  if( test_addImgs() ){
    printf("passed - cimpl_addImgs\n");
  } else {
    printf("failed - cimpl_addImgs\n");
  }

  if( test_addVols() ){
    printf("passed - cimpl_addVols\n");
  } else {
    printf("failed - cimpl_addVols\n");
  }

  if( test_addScalar2Img() ){
    printf("passed - cimpl_addScalar2Img\n");
  } else {
    printf("failed - cimpl_addScalar2Img\n");
  }

  if( test_addScalar2Vol() ){
    printf("passed - cimpl_addScalar2Vol\n");
  } else {
    printf("failed - cimpl_addScalar2Vol\n");
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

  if( test_flipImgLR() ){
    printf("passed - cimpl_flipImgLR\n");
  } else {
    printf("failed - cimpl_flipImgLR\n");
  }

  if( test_flipImgUD() ){
    printf("passed - cimpl_flipImgUD\n");
  } else {
    printf("failed - cimpl_flipImgUD\n");
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

  if( test_sumVol() ){
    printf("passed - cimpl_sumVol\n");
  } else {
    printf("failed - cimpl_sumVol\n");
  }

}
