#ifndef __AStack_h__
#define __AStack_h__

#include "AList.h"

namespace Atom {

template <class T>
class AStack
{
public:
	void push(T item)
	{
		m_list.append(item);
	}

	T pop(void)
	{
		if(m_list.size() < 1) {
			return 0;
		}

		T item = m_list[m_list.size()-1];
		m_list.removeAt(m_list.size()-1);
		return item;
	}

	T top(void)
	{
		return m_list[m_list.size()-1];
	}

	bool isEmpty(void)
	{
		if(m_list.size() > 0) {
			return false;
		}

		return true;
	}

protected:
private:
	AList<T> m_list;
};

}

#endif