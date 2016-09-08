#include "type.h"
#include "const.h"
#include "proto.h"
#include "global.h"

// called when clock int happens
PUBLIC void ClockHandler()
{
	if (k_reenter != 0) 
	{
		// reenter takes place in this clock int
	}
	if( ProcessSheduler() == FALSE)
	{
		// if return false, we should continue the process we interrupt.
		// but we cannot do it now, and we must switch to the process the "current_process" pointer points to
		// because our process system is not complete
	}
}

