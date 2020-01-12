/*****************************************************************************
 * Copyright (C) 2019 Kurt M. Weber <weberk294@gmail.com>                    *
 * This file is part of libbgl and is made available under the terms of the  *
 * Social Justice License.  See COPYING for details.                         *
 *****************************************************************************/

#ifndef _BGLOPEN_C
#define _BGLOPEN_C

#define _LIBBGL_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "libbgl.h"

bglFile *bglOpen(const char *path){
	bglFile *bf;
	
	bf = (bglFile *)malloc(sizeof(bglFile));
	
	if (!bf){
		switch (errno){
			case ENOMEM:
				_bglSetError(BGL_ERROR_OOM);
				break;
			default:	// can't happen
				_bglSetError(BGL_ERROR_UNKNOWN);
				break;
		}
		
		return NULL;
	}

	bf->filePath = malloc((strlen(path) + 1) * sizeof(char));	// strlen works here even for UTF-8 strings since what we really need is the
									// # of bytes rather than the # of characters for malloc() purposes
	
	if (!bf->filePath){
		switch (errno){
			case ENOMEM:
				_bglSetError(BGL_ERROR_OOM);
				break;
			default:	// can't happen
				_bglSetError(BGL_ERROR_UNKNOWN);
				break;
		}
		
		return NULL;
	}
	
	strcpy(bf->filePath, path);	
	
	bf->fh = fopen(path, "r");
	if (!bf->fh){
		switch (errno){
			case EACCES:
				_bglSetError(BGL_ERROR_PERMISSION);
				break;
			case ENOENT:
				_bglSetError(BGL_ERROR_NOFILE);
				break;
			case ENOMEM:					// is there a way to reconceptualize the error system to do things
				_bglSetError(BGL_ERROR_OOM);		// like distinguishing between this and an ENOMEM from the malloc above?
				break;
			case EFAULT:
			case EFBIG:
			case EINTR:
			case ELOOP:
			case EMFILE:
			case ENAMETOOLONG:
			case ENFILE:
			case ENODEV:
			case ENOTDIR:
			case EOVERFLOW:
			case EPERM:
				_bglSetError(BGL_ERROR_CANTOPEN);
				break;
			default:
				_bglSetError(BGL_ERROR_UNKNOWN);
				break;
		}
		
		return NULL;
	}

	_bglReadRawHeader(bf, &(bf->rawHeader));	// it is not an error to have an invalid header; conceivably, someone might use this
							// library as part of a utility to examine and fix corrupted BGL files, for example
							// we do check for validity and set a flag, however
	
	bf->validHeader = UNSET;
	bf->sections = NULL;
	
	return bf;
}

#endif