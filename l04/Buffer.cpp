#include "Buffer.h"
#include <iostream>
#include <cstring>


Buffer::Buffer (size_t size)
{
    if (size == 0)
    {
        std::cout << std::endl;
        m_refObj = new RefObject(32);
        this->b_size = 0;
        m_refObj->AddRefCount();
        this->b_capacity = 32;
    }
    else {
        m_refObj = new RefObject(size * 32);
        if (m_refObj != NULL) {
            m_refObj->AddRefCount();
            this->b_size = size;
            this->b_capacity = size * 32;
        }
    }
}

Buffer::Buffer(const Buffer &other)
{
    m_refObj = other.m_refObj;
    if(m_refObj != NULL)
    {
        m_refObj->AddRefCount();

    }
}

int Buffer::refs() const
{
    return this->m_refObj->refs();
}

Buffer::~Buffer()
{
    if(m_refObj != NULL && m_refObj->SubRefCount() == 0)
    {
        delete m_refObj;
    }
}

std::size_t Buffer::size() const
{
    return this->b_size;
}

char* Buffer::data()
{
    return this->m_refObj->m_obj->data;
}


const char* Buffer::cdata() const
{
    return this->m_refObj->m_obj->data;
}

Buffer& Buffer::operator=(Buffer other)
{
    if(m_refObj != NULL && m_refObj->SubRefCount() == 0)
    {
        delete m_refObj;
    }

    m_refObj = other.m_refObj;

    if(m_refObj != NULL)
    {
        m_refObj->AddRefCount();
    }

    return *this;
}


void Buffer::append(const char *data, int len)
{
    if (strlen(m_refObj->m_obj->data) == 0)
    {
        size_t length = strlen(data);
        m_refObj = new RefObject(length*2);
        strncpy(m_refObj->m_obj->data, data, length);
        this->b_size = strlen(m_refObj->m_obj->data);
    }
    else if (len == -1 && (strlen(m_refObj->m_obj->data) != 0))
    {
        strncat(m_refObj->m_obj->data, data, strlen(data));
        this->b_size = strlen(m_refObj->m_obj->data);
    }
    else
    {
        size_t lengthInputData = strlen(data);
        if (lengthInputData >= this->capacity())
        {
            strcat(m_refObj->m_obj->data, data);
            this->b_size = strlen(m_refObj->m_obj->data);
        }
        else
        {
            strcat(m_refObj->m_obj->data, data);
            this->b_size = strlen(m_refObj->m_obj->data);
        }
    }
}

std::size_t Buffer::capacity() const
{
    return this->b_capacity;
}


void Buffer::remove(std::size_t pos, std::size_t size)
{
    Data* tpmp = new Data[this->size()-size];
    int j = 0;

    if (pos < this->size() && pos >= 0)
    {
        for (int i = 0; this->data()[i] != '\0'; ++i)
        {
            if (i >= pos && i < (pos+size)) {}
            else
            {
                tpmp->data[j] = this->m_refObj->m_obj->data[i];
                j++;
            }
        }
        this->m_refObj->m_obj = tpmp;
        this->b_size = strlen(this->data());
    }
}

void Buffer::chop(std::size_t size)
{
    Data* tpmp = new Data[this->size()-size];
    int ix = strlen(this->data())-size;
    for (int i = 0; this->data()[i] != '\0'; ++i)
    {
        if (i < ix)
        {
            tpmp->data[i] = this->m_refObj->m_obj->data[i];
        }
    }
    this->m_refObj->m_obj = tpmp;
    this->b_size = strlen(this->data());
}

void Buffer::reserve(std::size_t newSize)
{
    Data* tpmp = this->m_refObj->m_obj;
    this->b_capacity = this->m_refObj->reserve(newSize);
    this->m_refObj->m_obj = tpmp;
}



























