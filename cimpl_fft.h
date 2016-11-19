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


//------  PRE FFT  ------

void cimpl_prefftCmpImg( cimpl_cmpImg const fftIn, cimpl_cmpImg const fftOut, void* out );
void cimpl_prefftCmpVol( cimpl_cmpVol const fftIn, cimpl_cmpVol const fftOut, void* out );
void cimpl_prefftImg( cimpl_img const in, cimpl_cmpImg const fftOut, void* out );

void cimpl_preifftCmpImg( cimpl_cmpImg const in, cimpl_cmpImg const fftOut, void* out );
void cimpl_preifftCmpVol( cimpl_cmpVol const fftIn, cimpl_cmpVol const fftOut, void* out );


//------  FFT  ------

void cimpl_fftCmpImg( void* in );
void cimpl_fftCmpVol( void* in );
void cimpl_fftImg( void* in );

void cimpl_ifftCmpImg( void* in );
void cimpl_ifftCmpVol( void* in );
void cimpl_ifftImg( void* in );


//------  POST FFT  ------

void cimpl_postfftCmpImg( void* in );
void cimpl_postfftCmpVol( void* in );
void cimpl_postfftImg( void* in );

void cimpl_postifftCmpImg( void* in );
void cimpl_postifftCmpVol( void* in );
void cimpl_postifftImg( void* in );



#endif /* cimpl_fft_h */
