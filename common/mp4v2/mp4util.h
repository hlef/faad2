/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is MPEG4IP.
 * 
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2001.  All Rights Reserved.
 * 
 * Contributor(s): 
 *		Dave Mackie		dmackie@cisco.com
 */

#ifndef __MP4_UTIL_INCLUDED__
#define __MP4_UTIL_INCLUDED__

#include <assert.h>

#ifdef NDEBUG
#define ASSERT(expr)
#define WARNING(expr)
#else
#define ASSERT(expr)	assert(expr)
#define WARNING(expr) \
	if (expr) { \
		fprintf(stderr, "Warning (%s) in %s at line %u\n", \
			__STRING(expr), __FILE__, __LINE__); \
	}
#endif

#define VERBOSE(exprverbosity, verbosity, expr)	\
	if (((exprverbosity) & (verbosity)) == (exprverbosity)) { expr; }

#define VERBOSE_ERROR(verbosity, expr)		\
	VERBOSE(MP4_DETAILS_ERROR, verbosity, expr)

#define VERBOSE_WARNING(verbosity, expr)		\
	VERBOSE(MP4_DETAILS_WARNING, verbosity, expr)

#define VERBOSE_READ(verbosity, expr)		\
	VERBOSE(MP4_DETAILS_READ, verbosity, expr)

#define VERBOSE_READ_TABLE(verbosity, expr)	\
	VERBOSE((MP4_DETAILS_READ | MP4_DETAILS_TABLE), verbosity, expr)

#define VERBOSE_READ_SAMPLE(verbosity, expr)	\
	VERBOSE((MP4_DETAILS_READ | MP4_DETAILS_SAMPLE), verbosity, expr)

#define VERBOSE_READ_HINT(verbosity, expr)	\
	VERBOSE((MP4_DETAILS_READ | MP4_DETAILS_HINT), verbosity, expr)

#define VERBOSE_WRITE(verbosity, expr)		\
	VERBOSE(MP4_DETAILS_WRITE, verbosity, expr)

#define VERBOSE_WRITE_TABLE(verbosity, expr)	\
	VERBOSE((MP4_DETAILS_WRITE | MP4_DETAILS_TABLE), verbosity, expr)

#define VERBOSE_WRITE_SAMPLE(verbosity, expr)	\
	VERBOSE((MP4_DETAILS_WRITE | MP4_DETAILS_SAMPLE), verbosity, expr)

#define VERBOSE_WRITE_HINT(verbosity, expr)	\
	VERBOSE((MP4_DETAILS_WRITE | MP4_DETAILS_HINT), verbosity, expr)

#define VERBOSE_FIND(verbosity, expr)		\
	VERBOSE(MP4_DETAILS_FIND, verbosity, expr)

#define VERBOSE_ISMA(verbosity, expr)		\
	VERBOSE(MP4_DETAILS_ISMA, verbosity, expr)

inline void Indent(FILE* pFile, u_int8_t depth) {
	fprintf(pFile, "%*c", depth, ' ');
}

inline void MP4Printf(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	// TBD API call to set error_msg_func instead of just printf
	fprintf(stdout, fmt, ap);
	va_end(ap);
}

class MP4Error {
public:
	MP4Error() {
		m_errno = 0;
		m_errstring = NULL;
		m_where = NULL;
	}
	MP4Error(int err, const char* where = NULL) {
		m_errno = err;
		m_errstring = NULL;
		m_where = where;
	}
	MP4Error(const char* errstring, const char* where = NULL) {
		m_errno = 0;
		m_errstring = errstring;
		m_where = where;
	}
	MP4Error(int err, const char* errstring, const char* where) {
		m_errno = err;
		m_errstring = errstring;
		m_where = where;
	}

	void Print(FILE* pFile = stderr);

	int m_errno;
	const char* m_errstring;
	const char* m_where;
};

void MP4HexDump(
	u_int8_t* pBytes, u_int32_t numBytes,
	FILE* pFile = stdout, u_int8_t indent = 0);

inline void* MP4Malloc(size_t size) {
	void* p = malloc(size);
	if (p == NULL && size > 0) {
		throw new MP4Error(errno);
	}
	return p;
}

inline void* MP4Calloc(size_t size) {
	return memset(MP4Malloc(size), 0, size);
}

inline char* MP4Stralloc(const char* s1) {
	char* s2 = (char*)MP4Malloc(strlen(s1) + 1);
	strcpy(s2, s1);
	return s2;
}

inline void* MP4Realloc(void* p, u_int32_t newSize) {
	// workaround library bug
	if (p == NULL && newSize == 0) {
		return NULL;
	}
	p = realloc(p, newSize);
	if (p == NULL && newSize > 0) {
		throw new MP4Error(errno);
	}
	return p;
}

inline void MP4Free(void* p) {
	free(p);
}

inline u_int32_t STRTOINT32(const char* s) {
	return (s[0] << 24) | (s[1] << 16) | (s[2] << 8) | s[3];
}

inline void INT32TOSTR(u_int32_t i, char* s) {
	s[0] = ((i >> 24) & 0xFF); s[1] = ((i >> 16) & 0xFF); 
	s[2] = ((i >> 8) & 0xFF); s[3] = (i & 0xFF); s[4] = 0;
}

inline MP4Timestamp MP4GetAbsTimestamp() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + 209606400;	// MP4 start date is 1/1/1904
}

u_int64_t MP4ConvertTime(u_int64_t t, 
	u_int32_t oldTimeScale, u_int32_t newTimeScale);

bool MP4NameFirstMatches(const char* s1, const char* s2);

bool MP4NameFirstIndex(const char* s, u_int32_t* pIndex);

char* MP4NameFirst(const char *s);

const char* MP4NameAfterFirst(const char *s);

char* MP4ToBase16(const u_int8_t* pData, u_int32_t dataSize);

char* MP4ToBase64(const u_int8_t* pData, u_int32_t dataSize);

#endif /* __MP4_UTIL_INCLUDED__ */