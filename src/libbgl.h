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
	BGL_SECTION_BOUNDARY = 0x20,
	BGL_SECTION_WAYPOITN = 0x22,
	BGL_SECTION_GEOPOL = 0x23,
	BGL_SECTION_SCENERYOBJECT = 0x25,
	BGL_SECTION_NAMELIST = 0x27,
	BGL_SECTION_VORILSICAOINDEX = 0x28,
	BGL_SECTION_NDBICAOINDEX = 0x29,
	BGL_SECTION_WAYPOINTICAOINDEX = 0x2A,
	BGL_SECTION_MODELDATA = 0x2B,
	BGL_SECTION_AIRPORTSUMMARY = 0x2C,
	BGL_SECTION_EXCLUSION = 0x2E,
	BGL_SECTION_TIMEZONE = 0x2F,
	BGL_SECTION_TERRAINVECTORDB = 0x65,
	BGL_SECTION_TERRAINELEVATION = 0x67,
	BGL_SECTION_TERRAINLANDCLASS = 0x68,
	BGL_SECTION_TERRAINWATERCLASS = 0x69,
	BGL_SECTION_TERRAINREGION = 0x6A
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
	unsigned long sectionNumber;
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
	BGL_ERROR_CLEAR = 0,
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

/*
 * void bglClearError()
 * ARGUMENTS: <none>
 * RETURN:    <none>
 * USE:       To clear the libbgl error state.  Use before every libbgl function call
 *            for which you are interested in checking whether an error occurred.
 */
void bglClearError();

/*
 * unsigned long bglCountSections(bglFile *bf)
 * ARGUMENTS: bglFile *bf - pointer to a bglFile object opened with bglOpen()
 * RETURN:    unsigned long - number of BGL sections in bglFile
 * USE:       To discover how many BGL sections a given BGL file contains
 */
unsigned long bglCountSections(bglFile *bf);	// return value is # of sections

/*
 * unsigned long bglCountSectionsByType(bglFile *bf, bglSectionTypes type)
 * ARGUMENTS: bglFile *bf - pointer to a bglFile object opened with bglOpen()
 *            bglSectionTypes type - section type of interest
 * RETURN:    unsigned long - number of BGL sections of specified type in bglFile
 * USE:       TO discover how many BGL sections of a given type a given BGL file contains
 */
unsigned long bglCountSectionsByType(bglFile *bf, bglSectionTypes type);

/*
 * bglSection **bglGetSectionsByType(bglFile *bf, bglSectionTypes type)
 * ARGUMENTS: bglFile *bf - pointer to a bglFile object opened with bglOpen()
 *            bglSectionTypes type - type of BGL section to return in an array
 * RETURN:    bglSection ** - array of pointers to every section of requested type in bglFile
 * USE:       To obtain references to every BGL section of a given type in a given BGL section
 */
bglSection **bglGetSectionsByType(bglFile *bf, bglSectionTypes type);

// internal-use-only functions

#ifdef _LIBBGL_SOURCE
ssize_t _bglReadBytes(bglFile *file, unsigned long start, unsigned long count, void *buffer);
unsigned int _bglReadRawHeader(bglFile *file, _bglRawHeader *header);
void _bglSetError(bglErrors error);
int _bglSetFileOffset(bglFile *file, long offset);
unsigned long _bglSectionHeaderOffset(unsigned long sectionNumber);

void _bglReadSectionHeaders(bglFile *bf);
void _bglReserveSectionsSpace(bglFile *bf);
#endif

#endif