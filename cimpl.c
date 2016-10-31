//
//  cimpl.c
//  cimpl
//
//  Created by Nicholas Dwork starting on 9/18/16.
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
  for( int i=0; i<in.h*in.w; ++i )
    out->data[i] = fabsf(in.data[i]);
}

void cimpl_absVol( cimpl_volf const in, cimpl_volf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  assert( out->s == in.s );
  for( int i=0; i<in.h*in.w*in.s; ++i )
    out->data[i] = fabsf(in.data[i]);
}

void cimpl_addImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i)
    out->data[i] = img1.data[i] + img2.data[i];
}

void cimpl_addVols( cimpl_volf const vol1, cimpl_volf const vol2, cimpl_volf * const out ){
  assert( out->w == vol1.w );
  assert( out->h == vol1.h );
  assert( out->s == vol1.s );
  assert( out->w == vol2.w );
  assert( out->h == vol2.h );
  assert( out->s == vol2.s );
  for( int i=0; i<vol1.h*vol1.w*vol1.s; ++i)
    out->data[i] = vol1.data[i] + vol2.data[i];
}

void cimpl_addScalar2Img( float const scalar, cimpl_imgf const in, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( unsigned int i=0; i<in.w*in.h; ++i )
    out->data[i] = in.data[i] + scalar;
}

void cimpl_addScalar2Vol( float const scalar, cimpl_volf const in, cimpl_volf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  assert( out->s == in.s );
  for( unsigned int i=0; i<in.w*in.h*in.s; ++i )
    out->data[i] = in.data[i] + scalar;
}

void cimpl_circShiftImg( cimpl_imgf const in, int hShift, int vShift, cimpl_imgf * const out ){
  assert( out->w == in.w );
  assert( out->h == in.h );
  assert( out->data != in.data );
  int inX, inY;
  
  for( int x=0; x<in.w; ++x ){
    inX = x+hShift;
    inX = inX % in.w;
    for( int y=0; y<in.h; ++y ){
      inY = y+vShift;
      inY = inY % in.h;

      out->data[y+x*in.h] = in.data[inY+inX*in.h];
  } }
}

void cimpl_circShiftVol( cimpl_volf const in, int hShift, int vShift, int sShift,
  cimpl_volf * const out ){
  assert( out->w == in.w );
  assert( out->h == in.h );
  assert( out->s == in.s );
  assert( out->data != in.data );
  int inX, inY, inZ;
  for( int x=0; x<in.w; ++x ){
    inX = x + hShift;
    inX = inX % in.w;
    for( int y=0; y<in.h; ++y ){
      inY = y + vShift;
      inY = inY % in.h;
      for( int z=0; z<in.s; ++z ){
        inZ = z + sShift;
        inZ = inZ % in.s;

        out->data[y+x*in.h+z*in.h*in.w] = in.data[inY+inX*in.h+inZ*in.h*in.w];
  } } }
}

void cimpl_cropImg( cimpl_imgf const in, cimpl_imgf * const out ){
  // Crops data to size of out
  assert( out->w <= in.w );
  assert( out->h <= in.h );
  int halfW, minW;
  int halfH, minH;
  int colOffset, minColOffset;

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

  for( unsigned int x=0; x<out->w; ++x ){
    colOffset = x*out->h;
    minColOffset = (minW+x)*in.h;

    for( unsigned int y=0; y<out->h; ++y ){
      out->data[y+colOffset] = in.data[ (minH+y)+ minColOffset ];
  } }
}

void cimpl_cropVol( cimpl_volf const in, cimpl_volf * const out ){
  // Crops data to size of out
  assert( out->w <= in.w );
  assert( out->h <= in.h );
  assert( out->s <= in.s );
  int halfW, minW;
  int halfH, minH;
  int halfS, minS;
  int minColOffset, minSliceOffset;
  int colOffset, sliceOffset;
  
  if( in.w % 2 == 0 )
    halfW = in.w/2;
  else
    halfW = (in.w-1)/2;
  if( in.h % 2 == 0 )
    halfH = in.h/2;
  else
    halfH = (in.h-1)/2;
  if( in.s % 2 == 0 )
    halfS = in.s/2;
  else
    halfS = (in.s-1)/2;
  
  if( out->w % 2 == 0 )
    minW = halfW - out->w/2;
  else
    minW = halfW - (out->w-1)/2;
  if( out->h % 2 == 0 )
    minH = halfH - out->h/2;
  else
    minH = halfH - (out->h-1)/2;
  if( out->s % 2 == 0 )
    minS = halfS - out->s/2;
  else
    minS = halfS - (out->s-1)/2;

  for( unsigned int z=0; z<out->s; ++z ){
    sliceOffset = z*out->h*out->w;
    minSliceOffset = (minS+z)*in.h*in.w;

    for( unsigned int x=0; x<out->w; ++x ){
      colOffset = x*out->h;
      minColOffset = (minW+x)*in.h;

      for( unsigned int y=0; y<out->h; ++y ){
        out->data[y + colOffset + sliceOffset] =
          in.data[ (minH+y) + minColOffset + minSliceOffset ];
  } } }
}

void cimpl_divideImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i)
    out->data[i] = img1.data[i] / img2.data[i];
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
  for( int i=0; i<img1.h*img1.w; ++i )
    out += img1.data[i] * img2.data[i];

  return out;
}

int cimpl_equalImgs( cimpl_imgf const img1, cimpl_imgf const img2 ){
  if( img1.h != img2.h )
    return 0;
  if( img1.w != img2.w )
    return 0;
  for( unsigned int i=0; i<img1.h*img1.w; ++i){
    if( img1.data[i] != img2.data[i] ){
      return 0;
  } }
  return 1;
}

int cimpl_equalVols( cimpl_volf const vol1, cimpl_volf const vol2 ){
  if( vol1.h != vol2.h )
    return 0;
  if( vol1.w != vol2.w )
    return 0;
  if( vol1.s != vol2.s )
    return 0;
  for( unsigned int i=0; i<vol1.h*vol1.w*vol1.s; ++i){
    if( vol1.data[i] != vol2.data[i] ){
      return 0;
  } }
  return 1;
}

void cimpl_flipImgLR( cimpl_imgf const in, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  assert( out->data != in.data );
  for( int x=0; x<in.w; ++x ){
    for( int y=0; y<in.h; ++y ){
      out->data[y+x*in.h] = in.data[y+(in.w-x-1)*in.h];
  } }
}

void cimpl_flipImgUD( cimpl_imgf const in, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  assert( out->data != in.data );
  for( int x=0; x<in.w; ++x ){
    for( int y=0; y<in.h; ++y ){
      out->data[y+x*in.h] = in.data[(in.h-y-1)+x*in.h];
  } }
}

void cimpl_freeImg( cimpl_imgf *out ){
  out->w = 0;
  out->h = 0;
  free( out->data );
  out->data = NULL;
}

float cimpl_linInterp( unsigned int const N, float const * const x, float const * const y,
  float const outOfBounds, float const q ){
  // N is the number of values in the x and y arrays
  // x is an array of inputs (or domain values) in ascending order
  // y is an array of outputs (or range values) where y[i] corresponds to x[i]
  // outOfBounds is the value to return if extrapolating
  // q is the query domain value.

  float out=outOfBounds;
  unsigned int lowIndx=0;
  unsigned int highIndx=N-1;
  unsigned int midIndx;

  if( q < x[0] || q > x[N-1] ) return outOfBounds;

  // Perform a binary search
  while( highIndx - lowIndx > 1 ){
    midIndx = lowIndx + (highIndx-lowIndx)/2;
    if( q < x[midIndx] ){
      highIndx = midIndx;
    } else {
      lowIndx = midIndx;
    }
  }
  out = y[lowIndx] + (y[highIndx]-y[lowIndx])/(x[highIndx]-x[lowIndx]) * (q-x[lowIndx]);

  return out;
}

void cimpl_linInterps( unsigned int const N, float const * const x, float const * const y,
                      float const outOfBounds, unsigned int const M, float const * const q,
                      float * const out ){
  // This function takes advantage of order of q for greater efficiency than calling
  //   cimpl_linInterp multiple times.
  // N is the number of values in the x and y arrays
  // x is an array of inputs (or domain values) in ascending order
  // y is an array of outputs (or range values) where y[i] corresponds to x[i]
  // outOfBounds is the value to return if extrapolating
  // M is the number of query values
  // q are the query domain values in ascending order.

  unsigned int qIndx=0;
  unsigned int xIndx;

  while( q[qIndx] < x[0] ){
    out[qIndx] = outOfBounds;
    qIndx++;
    if( qIndx >= M )
      return;
  }

  for( xIndx=0; xIndx<N-1; ++xIndx ){
    while( q[qIndx] > x[xIndx] && q[qIndx] < x[xIndx+1] ){
      out[qIndx] = (y[xIndx+1]-y[xIndx])/(x[xIndx+1]-x[xIndx]) * (q[qIndx]-x[xIndx]);
      ++qIndx;
  } }

  while( qIndx < M ){
    out[qIndx] = outOfBounds;
    ++qIndx;
  }

  return;
}

void cimpl_linInterpImg( cimpl_imgf const img, unsigned int const N, float const * const xq,
  float const * const yq, float const outOfBounds, float * const out ){
  unsigned int x1, x2, y1, y2;
  float tmpX1, tmpX2;
  for( unsigned int i=0; i<N; ++i ){
    assert( xq[i] >= 0 );  assert( xq[i] <= img.w );
    assert( yq[i] >= 0 );  assert( yq[i] <= img.h );
    if( xq[i]<0 || xq[i]>=img.w || yq[i]<0 || yq[i]>=img.h ){
      out[i] = outOfBounds;
    } else {
      x1 = floorf( xq[i] );  x2 = ceilf( xq[i] );
      y1 = floorf( yq[i] );  y2 = ceilf( yq[i] );
      tmpX1 = (y2-yq[i])*img.data[y1+x1*img.h] + (yq[i]-y1)*img.data[y2+x1*img.h];
      tmpX2 = (y2-yq[i])*img.data[y1+x2*img.h] + (yq[i]-y1)*img.data[y2+x2*img.h];
      out[i] = (x2-xq[i])*tmpX1 + (xq[i]-x1)*tmpX2;
  } }
}

cimpl_imgf cimpl_mallocImg( unsigned int const h, unsigned int const w ){
  cimpl_imgf out;
  out.h = h;
  out.w = w;
  out.data = (float*) malloc( sizeof(float) * w * h );
  return out;
}

void cimpl_multiplyImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i)
    out->data[i] = img1.data[i] * img2.data[i];
}

void cimpl_multiplyImgByScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( unsigned int i=0; i<in.w*in.h; ++i )
    out->data[i] = in.data[i] * scalar;
}

void cimpl_printImg( cimpl_imgf const in ){
  for( unsigned int y=0; y<in.h; ++y ){
    for( unsigned int x=0; x<in.w; ++x ){
      printf( "%f ", in.data[y+x*in.h] );
    }
    printf( "\n" );
  }
}

void cimpl_reshapeImg( unsigned int H, unsigned int W, cimpl_imgf * const out ){
  assert( H*W == out->h*out->w );
  out->h = H;
  out->w = W;
}

void cimpl_reshapeVol( unsigned int H, unsigned int W, unsigned int S, cimpl_volf * const out ){
  assert( H*W*S == out->h*out->w*out->s );
  out->h = H;
  out->w = W;
  out->s = S;
}

//void cimpl_rot( cimpl_imgf const in, float const angle, cimpl_imgf * const out ){
//  // angle has units of radians
//  float sAngle, cAngle;
//  unsigned int cx, cy;
//
//  sAngle = sin( angle );
//  cAngle = cos( angle );
//  cx = ceil( (float) out->w / 2 );
//  cy = ceil( (float) out->h / 2 );
//
//  for( int x=0; x<out->w; ++x){
//    for( int y=0; y<out->h; ++y){
//
//  } }
//}

void cimpl_rot90( cimpl_imgf const in, cimpl_imgf * const out ){
  assert( out->h == in.w );
  assert( out->w == in.h );
  assert( out->data != in.data );
  for( unsigned int x=0; x<out->w; ++x ){
    for( unsigned int y=0; y<out->h; ++y ){
      out->data[y+x*out->h] = in.data[x+(out->h-y-1)*in.h];
  } }
}

void cimpl_rot180( cimpl_imgf const in, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  assert( out->data != in.data );
  unsigned int imgSize = in.h*in.w;
  for( unsigned int i=0; i<imgSize; ++i )
    out->data[i] = in.data[imgSize-i-1];
}

void cimpl_rot270( cimpl_imgf const in, cimpl_imgf * const out ){
  assert( out->h == in.w );
  assert( out->w == in.h );
  assert( out->data != in.data );
  for( unsigned int x=0; x<out->w; ++x ){
    for( unsigned int y=0; y<out->h; ++y ){
      out->data[y+x*out->h] = in.data[(out->w-x-1)+y*in.h];
  } }
}

void cimpl_subImg( cimpl_imgf const in, unsigned int const h1, unsigned int const w1,
  cimpl_imgf * const out ){
  assert( h1+out->h < in.h );
  assert( w1+out->w < in.w );

  for( unsigned int x=0; x<out->w; ++x ){
    for( unsigned int y=0; y<out->h; ++y ){
      out->data[y+x*out->h] = in.data[(w1+y)+(h1+x)*in.h];
  } }
}

void cimpl_subtractImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i)
    out->data[i] = img1.data[i] - img2.data[i];
}

void cimpl_subtractImgFromScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out ){
  assert( out->w == in.w );
  assert( out->h == in.h );
  for( unsigned int i=0; i<in.w*in.h; ++i )
    out->data[i] = scalar - in.data[i];
}

void cimpl_subtractScalarFromImg( cimpl_imgf const in, float const scalar, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( unsigned int i=0; i<in.w*in.h; ++i )
    out->data[i] = in.data[i] - scalar;
}

float cimpl_sumImg( cimpl_imgf const * const in ){
  float out=0;
  for( int i=0; i<in->h*in->w; ++i)
    out += in->data[i];
  return out;
}



