#include <cstddef>


typedef struct Data
{
    int n;
    char data[];
} Data;

class RefObject
{
public:

    Data* m_obj;

    RefObject (size_t size)
    {
        m_obj = new Data[size];
    }

    RefObject()
    {
        m_nRefCount = 0;
    }

    ~RefObject()
    {
        if (m_obj) {
            delete m_obj;
        }
    }

    inline int refs() const
    {
        return m_nRefCount;
    }

    inline int AddRefCount()
    {
        return ++m_nRefCount;
    }

    inline int SubRefCount()
    {
        return --m_nRefCount;
    }

    void ResetRefCount()
    {
        m_nRefCount = 0;
    }

    size_t reserve(size_t size)
    {
        m_obj = new Data[size];
    }

private:

    int m_nRefCount;
};