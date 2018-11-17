#ifndef __AQueue_h__
#define __AQueue_h__

#include "AList.h"
#include "ALock.h"

namespace Atom {

template <class T>
class AQueue
{
public:
	void push(T item)
	{
		//ALock lock(&cs);

		m_list.append(item);
	}

	T pop(void)
	{
		//ALock lock(&cs);

		if(m_list.size() < 1) {
			return 0;
		}

		T item = m_list[0];
		m_list.removeAt(0);
		return item;
	}

	T top(void)
	{
		//ALock lock(&cs);

		return m_list[0];
	}

	bool isEmpty(void)
	{
		//ALock lock(&cs);

		if(m_list.size() > 0) {
			return false;
		}

		return true;
	}

	int size(void)
	{
		//ALock lock(&cs);

		return m_list.size();
	}

protected:
private:
	AList<T> m_list;
	//ACriticalSection cs;
};

}

#endif