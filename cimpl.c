//
//  climp.c
//  climp
//
//  Created by Nicholas Dwork on 9/18/16.
//  Copyright © 2016 Nicholas Dwork. All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "cimpl.h"


void cimpl_add( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] + img2.data[i];
  }
}

void cimpl_addScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( unsigned int i=0; i<in.w*in.h; ++i )
    out->data[i] = in.data[i] + scalar;
}

void cimpl_display( cimpl_imgf const in ){
  for( unsigned int y=0; y<in.h; ++y ){
    for( unsigned int x=0; x<in.w; ++x ){
      printf( "%f ", in.data[y+x*in.h] );
    }
    printf( "\n" );
  }
}

void cimpl_divide( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] / img2.data[i];
  }
}

int cimpl_equal( cimpl_imgf const img1, cimpl_imgf const img2 ){
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

cimpl_imgf cimpl_mallocImg( unsigned int const w, unsigned int const h ){
  cimpl_imgf out;
  out.w = w;
  out.h = h;
  out.data = malloc( sizeof(float) * w * h );
  return out;
}

void cimpl_multiply( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] * img2.data[i];
  }
}

void cimpl_multiplyScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( unsigned int i=0; i<in.w*in.h; ++i )
    out->data[i] = in.data[i] * scalar;
}

void cimpl_subtract( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out ){
  assert( out->w == img1.w );
  assert( out->h == img1.h );
  assert( out->w == img2.w );
  assert( out->h == img2.h );
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] - img2.data[i];
  }
}

float cimpl_sum( cimpl_imgf const * const in ){
  float out=0;
  for( int i=0; i<in->h*in->w; ++i){
    out += in->data[i];
  }
  return out;
}



