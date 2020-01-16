/*****************************************************************************
 * Copyright (C) 2019 Kurt M. Weber <weberk294@gmail.com>                    *
 * This file is part of libbgl and is made available under the terms of the  *
 * Social Justice License.  See COPYING for details.                         *
 *****************************************************************************/

#ifndef _BGLSECTION_C
#define _BGLSECTION_C

#define _LIBBGL_SOURCE

#include <stdlib.h>

#include "libbgl.h"

const short BGL_SECTION_HEADER_SIZE = 20;

unsigned long _bglSectionHeaderOffset(unsigned long sectionNumber){
	const unsigned int fileHeaderSize = 56;
	const unsigned int sectionHeaderSize = 20;
	
	return fileHeaderSize + ((sectionNumber - 1) * sectionHeaderSize);
}

void _bglReserveSectionsSpace(bglFile *bf){
	bf->sections = malloc(sizeof(_bglSection) * bf->rawHeader.numSections);
	
	return;
}

void _bglReadSectionHeaders(bglFile *bf){
	char *buf;
	unsigned long sectionOffset;
	uint32_t numSections;
	
	if (bf->sections){
		return;
	}
	
	_bglReserveSectionsSpace(bf);

	buf = (char *)malloc(BGL_SECTION_HEADER_SIZE * sizeof(char));
	
	for (numSections = 0; numSections < bf->rawHeader.numSections; numSections++){
		sectionOffset = _bglSectionHeaderOffset(numSections + 1);
		
		_bglReadBytes(bf, sectionOffset, 20, buf);
		
		bf->sections[numSections].type = *((uint32_t *)&buf[0]);
		bf->sections[numSections].sizeConst = *((uint32_t *)&buf[4]);
		bf->sections[numSections].numSubSections = *((uint32_t *)&buf[8]);
		bf->sections[numSections].fileOffset = *((uint32_t *)&buf[12]);
		bf->sections[numSections].totalSubSectionSize = *((uint32_t *)&buf[16]);
		bf->sections[numSections].sectionNumber = numSections;
	}
	
	free(buf);
	
	return;
}

unsigned long bglCountSections(bglFile *bf){
	if (!bf->sections){
		_bglReadSectionHeaders(bf);
	}
	
	return bf->rawHeader.numSections;	// this returns a 1-based count, rather than a 0-based index
						// it is on the correct value because of the final numSections++
						// in the for loop above, which brings up to the count of sections
}

unsigned long bglCountSectionsByType(bglFile *bf, bglSectionTypes type){
	uint32_t numSections = 0;
	uint32_t i;
	
	if (!bf->sections){
		_bglReadSectionHeaders(bf);
	}
	
	for (i = 0; i < bf->rawHeader.numSections; i++){
		if (bf->sections[i].type == type){
			numSections++;
		}
	}
	
	return numSections;
}

bglSection **bglGetSectionsByType(bglFile *bf, bglSectionTypes type){
	uint32_t numSections = 0;
	uint32_t numSectionsType = 0;
	bglSection **sectionArray = 0;
	uint32_t i = 0, j = 0;
	
	numSections = bf->rawHeader.numSections;
	
	numSectionsType = bglCountSectionsByType(bf, type);
	if (!numSectionsType){
		return 0;
	}
	
	sectionArray = calloc(numSectionsType, sizeof(bglSection *));
	if (!sectionArray){
		_bglSetError(BGL_ERROR_OOM);
		return 0;
	}
	
	for (i = 0; i < numSections; i++){
		if (bf->sections[i].type == type){
			sectionArray[j] = &(bf->sections[i]);
			j++;
		}
	}
	
	return sectionArray;
}

#endif