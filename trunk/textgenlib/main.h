#define TEXTURE_EDITOR
#define TEXTURE_LIBRARY_EDITOR
//#define TEXTURE_ENGINE

#ifdef WIN32
#endif
#ifdef __APPLE__
#endif
#ifdef LINUX

#endif

#ifdef TEXTURE_EDITOR
	#if !defined(TEXTURE_EDITOR__INCLUDED_)
	#define TEXTURE_EDITOR__INCLUDED_
		#ifdef __APPLE__
			#include <deque>
			using std::deque;
			#include <OpenGL/gl.h>
		#endif
		#ifdef WIN32
			#include "../stl/deque.h"
			#include <gl/gl.h>
		#endif
		#include "stdio.h"
		#include "string.h"
		// mandatory libs
		#include "structs.h"
		#include "textgen.h"
		#include "textgenlib.h"

	#endif // !defined(TEXTURE_EDITOR__INCLUDED_)
#endif


#ifdef TEXTURE_ENGINE
	#if !defined(TEXTURE_ENGINE__INCLUDED_)
	#define TEXTURE_ENGINE__INCLUDED_

		#include "../stdafx.h"
		#include "structs.h"
		#include "textgen.h"
		#include "textgenlib.h"

	#endif // !defined(ENGINE__INCLUDED_)
#endif


#define PI 3.1415926535897932384626433832795
#define PIF 3.1415926535897932384626433832795f
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))