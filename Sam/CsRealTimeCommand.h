#ifndef __CsRealTimeCommand_h__
#define __CsRealTimeCommand_h__

#include "CsTypes.h"

namespace Cs {

class CsRealTimeCommand
{
public:
	virtual void execute(CsNanoTime timestamp, CsSampleTime samples) = 0;

protected:
private:
};

}

#endif