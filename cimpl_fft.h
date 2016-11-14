//
//  cimpl_fft.h
//  cimpl
//
//  Created by Nicholas Dwork on 11/12/16.
//  Copyright © 2016 Nicholas Dwork. All rights reserved.
//

#ifndef cimpl_fft_h
#define cimpl_fft_h

#include <stdio.h>
#include "cimpl.h"


void cimpl_prefftCmpImg( cimpl_cmpImg const fftIn, cimpl_cmpImg const fftOut, void* out );
void cimpl_prefftCmpVol( cimpl_cmpVol const fftIn, cimpl_cmpVol const fftOut, void* out );

void cimpl_fftCmpImg( void* in );
void cimpl_fftCmpVol( void* in );

void cimpl_postfftCmpImg( void* in );
void cimpl_postfftCmpVol( void* in );


#endif /* cimpl_fft_h */
