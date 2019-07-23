/*****************************************************************************
 * Copyright (C) 2019 Kurt M. Weber <weberk294@gmail.com>                    *
 * This file is part of libbgl and is made available under the terms of the  *
 * Social Justice License.  See COPYING for details.                         *
 *****************************************************************************/

#ifndef _LIBBGL_H
#define _LIBBGL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum triBool {			// be careful, because a non-zero value does not necessarily semantically mean "true" with this
	UNSET = -1,			// best to always check for values explicitly, just to keep it at the front of your mind
	FALSE = 0,
	TRUE = 1
} triBool;

typedef enum bglSectionTypes {
	BGL_SECTION_NONE = 0x0,
	BGL_SECTION_COPYRIGHT = 0x1,
	BGL_SECTION_GUID = 0x2,
	BGL_SECTION_AIRPORT = 0x3,
	BGL_SECTION_ILSVOR = 0x13,
	BGL_SECTION_NDB = 0x17,
	BGL_SECTION_MARKER = 0x18,
	BGL_SECTION_BOUNDARY = 0x20
} bglSectionTypes;

typedef struct _bglRawHeader{
	uint32_t magicNumber;
	uint32_t headerSize;
	uint32_t lowDateTime;
	uint32_t highDateTime;
	uint32_t magicNumber2;
	uint32_t numSections;
	uint32_t qmid[8];
} _bglRawHeader;

typedef union _bglSubSection{
	int a;
	short b;
} _bglSubSection;

typedef struct _bglSection{
	bglSectionTypes type;
	uint32_t sizeConst;
	uint32_t numSubSections;
	uint32_t fileOffset;
	uint32_t totalSubSectionSize;
	_bglSubSection *subSections;
} _bglSection;

typedef _bglSection bglSection;

typedef struct bglFile {
	char *filePath;
	FILE *fh;
	_bglRawHeader rawHeader;
	triBool validHeader;
	_bglSection *sections;
} bglFile;

// error information
typedef enum bglErrors {
	BGL_ERROR_UNKNOWN,
	BGL_ERROR_OOM,
	BGL_ERROR_PERMISSION,
	BGL_ERROR_NOFILE,
	BGL_ERROR_CANTOPEN,
	BGL_ERROR_INTERNALFILE
} bglErrors;

// public functions
bglFile *bglOpen(const char *path);
bool bglValidateHeader(bglFile *bf);
unsigned int bglEnumerateSections(bglFile *bf);		// return value is # of sections

// internal-use-only functions

#ifdef _LIBBGL_SOURCE
ssize_t _bglReadBytes(bglFile *file, unsigned long start, unsigned long count, void *buffer);
unsigned int _bglReadRawHeader(bglFile *file, _bglRawHeader *header);
void _bglSetError(bglErrors error);
int _bglSetFileOffset(bglFile *file, long offset);
//off_t _bgl
#endif

#endif