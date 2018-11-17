#ifndef __CsSampleBuffer_h__
#define __CsSampleBuffer_h__

namespace Cs {

struct CsSampleBuffer
{
	int numStreams;
	int numFrames;
	float **buffer32;
};

}

#endif