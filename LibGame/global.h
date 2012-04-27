#ifndef __GLOBAL__H__
#define __GLOBAL__H__

#include "platform.h"
#include "typedef.h"
#include "csystem.h"


#include "..\LibGameWin32\global_win32.h"


#ifndef M_DELETE
#define M_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#endif
#ifndef M_DELETE_ARRAY
#define M_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#endif
#ifndef M_FREE
#define M_FREE(p) { if(p) { free(p); (p)=NULL; } }
#endif


#define INSTANCE_METHOD( cl )					\
	static cl* s_Instance;						\
	static cl*  CreateInstance(){				\
		s_Instance = new cl();					\
		return s_Instance;						\
	}											\
	void DeleteInstance(){						\
		delete this;							\
	}											\
	static cl*	 GetInstance(){					\
		if(s_Instance)	return s_Instance;		\
		return CreateInstance();				\
	}											\
	static cl*	 GetInstanceN(){				\
		if(s_Instance)	return s_Instance;		\
		return NULL;							\
	}

#define  INSTANCE_DEFINED( cl )			\
	cl* cl::s_Instance = NULL;




#endif
