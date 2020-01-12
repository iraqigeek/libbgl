/*****************************************************************************
 * Copyright (C) 2019 Kurt M. Weber <weberk294@gmail.com>                    *
 * This file is part of libbgl and is made available under the terms of the  *
 * Social Justice License.  See COPYING for details.                         *
 *****************************************************************************/

#ifndef _BGLHEADER_C
#define _BGLHEADER_C

#define _LIBBGL_SOURCE

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "libbgl.h"

const short BGL_HEADER_SIZE = 56;

unsigned int _bglReadRawHeader(bglFile *file, _bglRawHeader *header){
	char *buf;
	unsigned short i;
	
	buf = (char *)malloc(BGL_HEADER_SIZE * sizeof(char));
	if (!buf){
		switch (errno){
			case ENOMEM:
				_bglSetError(BGL_ERROR_OOM);
				break;
			default:	// can't happen
				_bglSetError(BGL_ERROR_UNKNOWN);
				break;
		}
		
		return 0;
	}
	
	_bglReadBytes(file, 0, 56, buf);
		
	header->magicNumber = *((uint32_t *)&buf[0]);
	header->headerSize = *((uint32_t *)&buf[4]);
	header->lowDateTime = *((uint32_t *)&buf[8]);
	header->highDateTime = *((uint32_t *)&buf[12]);
	header->magicNumber2 = *((uint32_t *)&buf[16]);
	header->numSections = *((uint32_t *)&buf[20]);
	
	for (i = 0; i < 8; i++){
		header->qmid[i] = *((uint32_t *)&buf[24 + (i * 4)]);
	}
	
	free(buf);
	
	return 1;
}

bool bglValidateHeader(bglFile *bf){
	if (bf->validHeader != UNSET){
		return bf->validHeader;
	}
	
	if (bf->rawHeader.magicNumber != 0x19920201){
		bf->validHeader = false;
		return bf->validHeader;
	}
}

#endif