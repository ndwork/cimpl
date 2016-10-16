//
//  main.c
//  climp
//
//  Created by Nicholas Dwork on 9/18/16.
//  Copyright © 2016 Nicholas Dwork. All rights reserved.
//

#include <stdio.h>
#include "cimpl.h"


int main(int argc, const char * argv[]) {

  cimpl_imgf myImg, subImg, subImg2, tmp;

  // mallocImg
  printf("Testing mallocImg");
  myImg = cimpl_mallocImg( 50, 40 );
  for( unsigned int i=0; i<myImg.w*myImg.h; ++i ){
    myImg.data[i] = i;
  }

  // subImg
  printf("Testing subImg and display");
  subImg = cimpl_extractSubImg(myImg,0,4,0,3);
  cimpl_display(subImg);

  // add
  printf("Testing add");
  subImg2 = cimpl_extractSubImg(myImg,1,5,1,4);
  tmp = cimpl_mallocImg( subImg2.w, subImg2.h );
  cimpl_add( subImg, subImg2, &tmp );
  cimpl_display(tmp);

  // free
  printf("Testing free");
  cimpl_freeImg( &myImg );
  cimpl_freeImg( &subImg );
  cimpl_freeImg( &subImg2 );
  cimpl_freeImg( &tmp );
  return 0;
}
