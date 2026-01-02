/**********************************************************************
 *
 * Filename:    crc.h
 * 
 * Description: A header file describing the various CRC standards.
 *
 * Notes:       
 *
 * 
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/

#ifndef __LYNXENCRYPTION_H__
#define __LYNXENCRYPTION_H__

#include <lynxType.h>

#ifdef __cplusplus
extern "C" {
#endif								

LYNXCOREAPI int		lynxRC2Encrypt(char *cipher, const char* key, const char *plain);
LYNXCOREAPI void	lynxRC2Decrypt(char *plain, const char* key, const char *cipher, const int len);

#ifdef __cplusplus
}
#endif

#endif