#ifndef __ARingBuffer_h__
#define __ARingBuffer_h__

#include "APlatform.h"
#include "ALog.h"
#include "ALock.h"

namespace Atom {

template <class T>
class ARingBuffer
{
public:
	// ----------------------------------------------------------------------------------------------------
	ARingBuffer(long size)
		: m_size(size)
	{
		buffer = (T*) malloc(sizeof(T)*(size));

		InterlockedExchange(&readPointer, 0);
		InterlockedExchange(&writePointer, 0);
	}

	// ----------------------------------------------------------------------------------------------------
	~ARingBuffer()
	{
		free((T*)buffer);
	}

	// ----------------------------------------------------------------------------------------------------
	int add(T item)
	{
		//ALock lock(&cs);

		long tempWritePointer = InterlockedExchange (&writePointer, writePointer);
		buffer[tempWritePointer] = item;

		if(InterlockedIncrement(&writePointer) >= m_size)
			InterlockedExchange (&writePointer,0);
		

		return 1;
	}

	// ----------------------------------------------------------------------------------------------------
	T get(void)
	{
		//ALock lock(&cs);

		T item;

		long tempReadPointer = InterlockedExchange (&readPointer, readPointer);
		item = buffer[tempReadPointer];

		if (InterlockedIncrement(&readPointer) >= m_size)
			InterlockedExchange(&readPointer,0);

		return item;
	}

	// ----------------------------------------------------------------------------------------------------
	T head(void)
	{
		//ALock lock(&cs);

		T item;

		long tempReadPointer = InterlockedExchange(&readPointer, readPointer);
		item = buffer[tempReadPointer];

		return item;
	}

	// ----------------------------------------------------------------------------------------------------
	long size(void)
	{
		//ALock lock(&cs);

		long w = InterlockedExchange(&writePointer, writePointer);
		long r = InterlockedExchange(&readPointer, readPointer);

		//aLog("w=%d, r=%d", w, r);

		if (w >= r) {
			return w - r;
		}

		return (w - r + m_size);// & sizeMask;
	}

protected:
private:
	T *buffer;
	long m_size;
	long readPointer;
	long writePointer;
	//ACriticalSection cs;
};

}

#endif