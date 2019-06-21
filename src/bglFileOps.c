/*****************************************************************************
 * Copyright (C) 2019 Kurt M. Weber <weberk294@gmail.com>                    *
 * This file is part of libbgl and is made available under the terms of the  *
 * Social Justice License.  See COPYING for details.                         *
 *****************************************************************************/

#ifndef _BGLFILEOPS_C
#define _BGLFILEOPS_C

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include <libbgl.h>

off_t _bglSetFileOffset(bglFile *file, off_t offset){
	off_t o;
	o = lseek(file->fd, offset, SEEK_SET);
	
	if (o == -1){
		_bglSetError(BGL_ERROR_INTERNALFILE);
		return -1;
	}
	
	return o;
}

ssize_t _bglReadBytes(bglFile *file, size_t start, size_t count, void *buffer){
	ssize_t ret;
	
	if (_bglSetFileOffset(file, (off_t) start) == -1){
		return -1;
	}
	
	ret = read(file->fd, buffer, count);
	
	if (ret == -1){
		_bglSetError(BGL_ERROR_INTERNALFILE);
		return -1;
	}
	
	return ret;
}

#endif