#ifndef __ARefCountedObject_h__
#define __ARefCountedObject_h__

#include "APlatform.h"
#include "ALog.h"

class ARefCountedObject
{
public:
	ARefCountedObject()
		: m_count(0)
	{
	}

	virtual ~ARefCountedObject()
	{
		if(m_count > 0) {
			aLog("********** Shit! **********");
		} else {
			aLog("Delete");
		}
	}

	void incRefCount(void)
	{
		InterlockedIncrement(&m_count);
	}

	void decRefCount(void)
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
class ARefCountedPtr
{
public:
	ARefCountedPtr()
        : m_referencedObject(0)
    {
    }

	ARefCountedPtr(T *refCountedObject)
		: m_referencedObject(refCountedObject)
	{
		if(m_referencedObject != 0) {
			m_referencedObject->incRefCount();
		}
	}

	ARefCountedPtr(const ARefCountedPtr<T>& other)
        : m_referencedObject(other.m_referencedObject)
    {
        if (m_referencedObject != 0)
            m_referencedObject->incRefCount();
    }

    /** Changes this pointer to point at a different object.

        The reference count of the old object is decremented, and it might be
        deleted if it hits zero. The new object's count is incremented.
    */
    const ARefCountedPtr<T>& operator= (const ARefCountedPtr<T>& other)
    {
        T* const newObject = other.m_referencedObject;

        if (newObject != m_referencedObject)
        {
            if (newObject != 0)
                newObject->incRefCount();

            T* const oldObject = m_referencedObject;
            m_referencedObject = newObject;

            if (oldObject != 0)
                oldObject->decRefCount();
        }

        return *this;
    }

    /** Changes this pointer to point at a different object.

        The reference count of the old object is decremented, and it might be
        deleted if it hits zero. The new object's count is incremented.
    */
    const ARefCountedPtr<T>& operator= (T* const newObject)
    {
        if (m_referencedObject != newObject)
        {
            if (newObject != 0)
                newObject->incRefCount();

            T* const oldObject = m_referencedObject;
            m_referencedObject = newObject;

            if (oldObject != 0)
                oldObject->decRefCount();
        }

        return *this;
    }

    /** Destructor.

        This will decrement the object's reference-count, and may delete it if it
        gets to zero.
    */
    inline ~ARefCountedPtr()
    {
        if (m_referencedObject != 0)
            m_referencedObject->decRefCount();
    }

    /** Returns the object that this pointer references.

        The pointer returned may be zero, of course.
    */
    inline operator T*() const throw()
    {
        return m_referencedObject;
    }

    /** Returns true if this pointer refers to the given object. */
    inline bool operator== (T* const object) const throw()
    {
        return m_referencedObject == object;
    }

    /** Returns true if this pointer doesn't refer to the given object. */
    inline bool operator!= (T* const object) const throw()
    {
        return m_referencedObject != object;
    }

    // the -> operator is called on the referenced object
    inline T* operator->() const throw()
    {
        return m_referencedObject;
    }

protected:
private:
	T* m_referencedObject;
};

#endif