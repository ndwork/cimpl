//
//  climp.c
//  climp
//
//  Created by Nicholas Dwork on 9/18/16.
//  Copyright © 2016 Nicholas Dwork. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "cimpl.h"


void cimpl_add( struct cimpl_imgf const img1, struct cimpl_imgf const img2, struct cimpl_imgf * const out ){
    for( int i=0; i<img1.h*img1.w; ++i){
        out->data[i] = img1.data[i] + img2.data[i];
    }
}

void cimpl_display( struct cimpl_imgf const in ){
  for( unsigned int y=0; y<in.h; ++y ){
    for( unsigned int x=0; x<in.w; ++x ){
      printf( "%f ", in.data[y+x*in.h] );
    }
    printf( "\n" );
  }
}

void cimpl_divide( struct cimpl_imgf const img1, struct cimpl_imgf const img2, struct cimpl_imgf * const out ){
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] / img2.data[i];
  }
}

struct cimpl_imgf cimpl_extractSubImg( struct cimpl_imgf const in, unsigned int const h1, unsigned int const h2, unsigned int const v1, unsigned int const v2 ){
  struct cimpl_imgf out;
  out.w = h2-h1+1;
  out.h = v2-v1+1;

  for( int x=0; x<out.w; ++x ){
    for( int y=0; y<out.h; ++y ){
      // Column ordered data
      out.data[y+x*out.h] = in.data[(v1+y)+(h1+x)*in.h];
    }
  }
  return out;
}

void cimpl_freeImg( struct cimpl_imgf *out ){
  free( out->data );
}

struct cimpl_imgf cimpl_mallocImg( unsigned int const w, unsigned int const h ){
  struct cimpl_imgf out;
  out.w = w;
  out.h = h;
  out.data = malloc( sizeof(float) * w * h );
  return out;
}

void cimpl_multiply( struct cimpl_imgf const img1, struct cimpl_imgf const img2, struct cimpl_imgf * const out ){
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] * img2.data[i];
  }
}

void cimpl_subtract( struct cimpl_imgf const img1, struct cimpl_imgf const img2, struct cimpl_imgf * const out ){
  for( int i=0; i<img1.h*img1.w; ++i){
    out->data[i] = img1.data[i] - img2.data[i];
  }
}

float cimpl_sum( struct cimpl_imgf const * const in ){
  float out=0;
  for( int i=0; i<in->h*in->w; ++i){
    out += in->data[i];
  }
  return out;
}



