#define TEXTURE_EDITOR
#define TEXTURE_LIBRARY_EDITOR
//#define TEXTURE_ENGINE

#ifdef TEXTURE_EDITOR
	#if !defined(TEXTURE_EDITOR__INCLUDED_)
	#define TEXTURE_EDITOR__INCLUDED_
		#include <OpenGL/gl.h>
		#include <deque>
		using std::deque;
		// Only for compatibility with free's, malloc's....
		#include "stdlib.h"
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

		//#include "..\stdafx.h"
		// This can be deleted
		#include <OpenGL/gl.h>
		// Only for compatibility with free's, malloc's....
		#include "stdlib.h"
		#include "string.h"
		// mandatory libs
		#include "structs.h"
		#include "textgen.h"
		#include "textgenlib.h"

	#endif // !defined(ENGINE__INCLUDED_)
#endif

#define PI 3.1415926535897932384626433832795
#define PIF 3.1415926535897932384626433832795f
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
