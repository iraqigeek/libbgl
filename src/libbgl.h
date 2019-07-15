/*****************************************************************************
 * Copyright (C) 2019 Kurt M. Weber <weberk294@gmail.com>                    *
 * This file is part of libbgl and is made available under the terms of the  *
 * Social Justice License.  See COPYING for details.                         *
 *****************************************************************************/

#ifndef _LIBBGL_H
#define _LIBBGL_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum triBool {			// be careful, because a non-zero value does not necessarily semantically mean "true" with this
	UNSET = -1,			// best to always check for values explicitly, just to keep it at the front of your mind
	FALSE = 0,
	TRUE = 1
} triBool;

typedef enum bglSectionTypes {
	BGL_SECTION_NONE = 0x0,
	BGL_SECTION_COPYRIGHT = 0x1,
	BGL_SECTION_GUID = 0x2,
	BGL_SECTION_AIRPORT = 0x13
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

typedef struct bglFile {
	char *filePath;
	int fd;
	_bglRawHeader rawHeader;
	triBool validHeader;
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

// internal-use-only functions
ssize_t _bglReadBytes(bglFile *file, unsigned long start, unsigned long count, void *buffer);
unsigned int _bglReadRawHeader(bglFile *file, _bglRawHeader *header);
void _bglSetError(bglErrors error);
off_t _bglSetFileOffset(bglFile *file, off_t offset);

#endif