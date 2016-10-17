//
//  cimpl.c
//  cimpl
//
//  Created by Nicholas Dwork on 9/18/16.
//  Copyright © 2016 Nicholas Dwork.
//

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cimpl.h"



void cimpl_absImg( cimpl_imgf const in, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( int i=0; i<in.h*in.w; ++i ){
    out->data[i] = fabsf(in.data[i]);
  }
}

void cimpl_addImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] + img2.data[i];
  }
}

void cimpl_addScalar2Img( float const scalar, cimpl_imgf const in, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( unsigned int i=0; i<in.w*in.h; ++i )
    out->data[i] = in.data[i] + scalar;
}

void cimpl_circShiftImg( cimpl_imgf const in, int hShift, int vShift, cimpl_imgf * const out ){
  assert( out->w == in.w );
  assert( out->h == in.h );
  int inX, inY;
  for( int x=0; x<in.w; ++x ){
    inX = x+hShift;
    while( inX < 0)
      inX += in.w;
    inX = inX % in.w;
    for( int y=0; y<in.h; ++y ){
      inY = y+vShift;
      while( inY < 0 )
        inY += in.h;
      inY = inY % in.h;
      out->data[y+x*in.h] = in.data[inY+inX*in.h];
    }
  }
}

void cimpl_cropImg( cimpl_imgf const in, cimpl_imgf * const out ){
  // Crops data to size of out
  assert( out->w <= in.w );
  assert( out->h <= in.h );
  int halfW, minW;
  int halfH, minH;

  if( in.w % 2 == 0 )
    halfW = in.w/2;
  else
    halfW = (in.w-1)/2;
  if( in.h % 2 == 0 )
    halfH = in.h/2;
  else
    halfH = (in.h-1)/2;

  if( out->w % 2 == 0 )
    minW = halfW - out->w/2;
  else
    minW = halfW - (out->w-1)/2;
  if( out->h % 2 == 0 )
    minH = halfH - out->h/2;
  else
    minH = halfH - (out->h-1)/2;

  for( int x=0; x<out->w; ++x ){
    for( int y=0; y<out->h; ++y ){
      out->data[y+x*out->h] = in.data[(minH+y)+(minW+x)*in.h];
    }
  }
}

void cimpl_displayImg( cimpl_imgf const in ){
  for( unsigned int y=0; y<in.h; ++y ){
    for( unsigned int x=0; x<in.w; ++x ){
      printf( "%f ", in.data[y+x*in.h] );
    }
    printf( "\n" );
  }
}

void cimpl_divideImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] / img2.data[i];
  }
}

void cimpl_divideImgByScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( unsigned int i=0; i<in.w*in.h; ++i )
    out->data[i] = in.data[i] / scalar;
}

float cimpl_dotImgs( cimpl_imgf const img1, cimpl_imgf const img2 ){
  assert( img1.w == img2.w );
  assert( img1.h == img2.h );
  float out = 0;
  for( int i=0; i<img1.h*img1.w; ++i ){
    out += img1.data[i] * img2.data[i];
  }
  return out;
}

int cimpl_equalImgs( cimpl_imgf const img1, cimpl_imgf const img2 ){
  if( img1.h != img2.h )
    return 0;
  if( img1.w != img2.w )
    return 0;
  for( unsigned int i=0; i<img1.h*img1.w; ++i){
    if( img1.data[i] != img2.data[i] )
      return 0;
  }
  return 1;
}

cimpl_imgf cimpl_extractSubImg( cimpl_imgf const in, unsigned int const h1, unsigned int const h2, unsigned int const v1, unsigned int const v2 ){
  cimpl_imgf out = cimpl_mallocImg( h2-h1+1, v2-v1+1 );

  for( int x=0; x<out.w; ++x ){
    for( int y=0; y<out.h; ++y ){
      // Column ordered data
      out.data[y+x*out.h] = in.data[(v1+y)+(h1+x)*in.h];
    }
  }
  return out;
}

void cimpl_freeImg( cimpl_imgf *out ){
  free( out->data );
  out->w = 0;
  out->h = 0;
}

void cimpl_linInterpImg( cimpl_imgf const img, unsigned int const N, float const * const xq,
  float const * const yq, float * const out ){
  unsigned int x1, x2, y1, y2;
  float tmpX1, tmpX2;
  for( unsigned int i=0; i<N; ++i ){
    assert( xq[i] >= 0 );  assert( xq[i] <= img.w );
    assert( yq[i] >= 0 );  assert( yq[i] <= img.h );
    x1 = floorf( xq[i] );  x2 = ceilf( xq[i] );
    y1 = floorf( yq[i] );  y2 = ceilf( yq[i] );
    tmpX1 = (y2-yq[i])*img.data[y1+x1*img.h] + (yq[i]-y1)*img.data[y2+x1*img.h];
    tmpX2 = (y2-yq[i])*img.data[y1+x2*img.h] + (yq[i]-y1)*img.data[y2+x2*img.h];
    out[i] = (x2-xq[i])*tmpX1 + (xq[i]-x1)*tmpX2;
  }
}

cimpl_imgf cimpl_mallocImg( unsigned int const w, unsigned int const h ){
  cimpl_imgf out;
  out.w = w;
  out.h = h;
  out.data = malloc( sizeof(float) * w * h );
  return out;
}

void cimpl_multiplyImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] * img2.data[i];
  }
}

void cimpl_multiplyImgByScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( unsigned int i=0; i<in.w*in.h; ++i )
    out->data[i] = in.data[i] * scalar;
}

void cimpl_subtractImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] - img2.data[i];
  }
}

void cimpl_subtractScalarFromImg( cimpl_imgf const in, float const scalar, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( unsigned int i=0; i<in.w*in.h; ++i )
    out->data[i] = in.data[i] - scalar;
}

float cimpl_sumImg( cimpl_imgf const * const in ){
  float out=0;
  for( int i=0; i<in->h*in->w; ++i){
    out += in->data[i];
  }
  return out;
}



