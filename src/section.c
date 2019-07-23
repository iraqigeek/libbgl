/*****************************************************************************
 * Copyright (C) 2019 Kurt M. Weber <weberk294@gmail.com>                    *
 * This file is part of libbgl and is made available under the terms of the  *
 * Social Justice License.  See COPYING for details.                         *
 *****************************************************************************/

#ifndef _BGLSECTION_C
#define _BGLSECTION_C

#define _LIBBGL_SOURCE

#include <libbgl.h>

unsigned int bglEnumerateSections(bglFile *bf){
	uint32_t numSections;
	
	for (numSections = 0; numSections < bf->rawHeader.numSections; numSections++){
	}
}

#endif