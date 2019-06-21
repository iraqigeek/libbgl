/*****************************************************************************
 * Copyright (C) 2019 Kurt M. Weber <weberk294@gmail.com>                    *
 * This file is part of libbgl and is made available under the terms of the  *
 * Social Justice License.  See COPYING for details.                         *
 *****************************************************************************/

#ifndef _BGLERROR_C
#define _BGLERROR_C

#include <libbgl.h>

bglErrors bglErrno;

void _bglSetError(bglErrors error){
	bglErrno = error;
}

#endif