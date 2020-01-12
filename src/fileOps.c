/*****************************************************************************
 * Copyright (C) 2019 Kurt M. Weber <weberk294@gmail.com>                    *
 * This file is part of libbgl and is made available under the terms of the  *
 * Social Justice License.  See COPYING for details.                         *
 *****************************************************************************/

#ifndef _BGLFILEOPS_C
#define _BGLFILEOPS_C

#define _LIBBGL_SOURCE

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include "libbgl.h"

int _bglSetFileOffset(bglFile *file, long offset){
	int o;
	o = fseek(file->fh, offset, SEEK_SET);
	
	if (o == -1){
		_bglSetError(BGL_ERROR_INTERNALFILE);
		return -1;
	}
	
	return o;
}

ssize_t _bglReadBytes(bglFile *file, size_t start, size_t count, void *buffer){
	ssize_t ret;
	
	if (_bglSetFileOffset(file, (long) start) == -1){
		return -1;
	}
	
	ret = fread(buffer, count, 1, file->fh);
	
	if (ret == -1){
		_bglSetError(BGL_ERROR_INTERNALFILE);
		return -1;
	}
	
	return ret;
}

#endif