#ifndef __AObject_h__
#define __AObject_h__

#include "APlatform.h"
#include "ALog.h"

class AObject
{
public:
	AObject()
		: m_count(0)
	{
	}

	virtual ~AObject()
	{
		if(m_count > 0) {
			aLog("********** Shit! **********");
		} else {
			//aLog("Delete");
		}
	}

	void addRef(void)
	{
		InterlockedIncrement(&m_count);
	}

	void release(void)
	{
		if(InterlockedDecrement(&m_count) == 0) {
            delete this;
		}
	}

	LONG refCount(void)
	{
		return m_count;
	}

private:
protected:
	LONG m_count;
};

template <class T>
class AObjectPtr
{
public:
	AObjectPtr()
        : m_referencedObject(0)
    {
    }

	AObjectPtr(T *refCountedObject)
		: m_referencedObject(refCountedObject)
	{
		if(m_referencedObject != 0) {
			m_referencedObject->addRef();
		}
	}

	AObjectPtr(const AObjectPtr<T>& other)
        : m_referencedObject(other.m_referencedObject)
    {
        if (m_referencedObject != 0)
            m_referencedObject->addRef();
    }

    /** Changes this pointer to point at a different object.

        The reference count of the old object is decremented, and it might be
        deleted if it hits zero. The new object's count is incremented.
    */
    const AObjectPtr<T>& operator= (const AObjectPtr<T>& other)
    {
        T* const newObject = other.m_referencedObject;

        if (newObject != m_referencedObject)
        {
            if (newObject != 0)
                newObject->addRef();

            T* const oldObject = m_referencedObject;
            m_referencedObject = newObject;

            if (oldObject != 0)
                oldObject->release();
        }

        return *this;
    }

    /** Changes this pointer to point at a different object.

        The reference count of the old object is decremented, and it might be
        deleted if it hits zero. The new object's count is incremented.
    */
    const AObjectPtr<T>& operator= (T* const newObject)
    {
        if (m_referencedObject != newObject)
        {
            if (newObject != 0)
                newObject->addRef();

            T* const oldObject = m_referencedObject;
            m_referencedObject = newObject;

            if (oldObject != 0)
                oldObject->release();
        }

        return *this;
    }

   inline ~AObjectPtr()
    {
        if (m_referencedObject != 0)
            m_referencedObject->release();
    }

    inline operator T*() const throw()
    {
        return m_referencedObject;
    }

    inline bool operator== (T* const object) const throw()
    {
        return m_referencedObject == object;
    }

    inline bool operator!= (T* const object) const throw()
    {
        return m_referencedObject != object;
    }

    inline T* operator->() const throw()
    {
        return m_referencedObject;
    }

protected:
private:
	T* m_referencedObject;
};

#endif