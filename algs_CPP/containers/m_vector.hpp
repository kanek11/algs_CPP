
#include <memory>
#include <stdexcept>

#include <iostream>
// allocator is exception safe ,but we use placement new ;

namespace m_std
{

template <typename T>
class vector
{
public:
    using value_type = T;
    using iterator   = T*;
    using size_t     = std::size_t;

    iterator begin() { return m_data; }
    iterator end() { return m_data + m_size; }

private:
    T*     m_data     = nullptr;
    size_t m_size     = 0;
    size_t m_capacity = 0;

public:
    ~vector()
    {
        for (size_t i = 0; i < m_size; i++)
        {
            m_data[i].~T();
        }
        ::operator delete(m_data);

        std::cout << "destructor called" << std::endl;
    }
    vector()
    {
        // initial capacity with ,say 4
        resize(4);
    }

    vector(size_t size)
    {
        m_size     = size;
        m_capacity = size;
        m_data     = (T*)::operator new(sizeof(T) * m_capacity);
        for (size_t i = 0; i < m_size; i++)
        {
            new (m_data + i) T();
        }
    }

    vector(size_t size, const T& value)
    {
        m_size     = size;
        m_capacity = size;
        m_data     = (T*)::operator new(sizeof(T) * m_capacity);
        for (size_t i = 0; i < m_size; i++)
        {
            new (m_data + i) T(value);
        }
    }

    // copy constructor
    vector(const vector& other)
    {
        m_size     = other.m_size;
        m_capacity = other.m_capacity;
        m_data     = (T*)::operator new(sizeof(T) * m_capacity);
        for (size_t i = 0; i < m_size; i++)
        {
            new (m_data + i) T(other.m_data[i]); // construct
        }

        std::cout << "copy constructor called" << std::endl;
    }

    // copy assignment
    vector& operator=(const vector& other)
    {
        if (this != &other) // self assignment
        {
            for (size_t i = 0; i < m_size; i++)
            {
                m_data[i].~T(); // destruct
            }
            ::operator delete(m_data);

            m_size     = other.m_size;
            m_capacity = other.m_capacity;
            m_data     = (T*)::operator new(sizeof(T) * m_capacity);
            for (size_t i = 0; i < m_size; i++)
            {
                ::new (m_data + i) T(other.m_data[i]); // construct
            }
        }

        std::cout << "copy assignment called" << std::endl;
        return *this;
    }

    // move constructor
    vector(vector&& other) noexcept
    {
        m_size           = other.m_size;
        m_capacity       = other.m_capacity;
        m_data           = other.m_data;
        other.m_data     = nullptr;
        other.m_size     = 0;
        other.m_capacity = 0;

        std::cout << "move constructor called" << std::endl;
    }

    // move assignment
    vector& operator=(vector&& other) noexcept
    {
        if (this == &other) // self assignment
        {
            return *this;
        }
        for (size_t i = 0; i < m_size; i++)
        {
            m_data[i].~T(); // destruct
        }
        ::operator delete(m_data);

        m_size           = other.m_size;
        m_capacity       = other.m_capacity;
        m_data           = other.m_data;
        other.m_data     = nullptr;
        other.m_size     = 0;
        other.m_capacity = 0;

        std::cout << "move assignment called" << std::endl;
        return *this;
    }

private:
    void resize(size_t new_capacity)
    {
        T* new_data = (T*)::operator new(sizeof(T) * new_capacity);

        // case when it's a shrink
        if (new_capacity < m_size)
        {
            // destruct the elements that are truncated
            for (size_t i = new_capacity; i < m_size; i++)
            {
                m_data[i].~T();
            }
            m_size = new_capacity;
        }

        for (size_t i = 0; i < m_size; i++)
        {
            ::new (new_data + i) T(std::move(m_data[i])); // move construct
            m_data[i].~T();
        }

        ::operator delete(m_data);

        m_data     = new_data;
        m_capacity = new_capacity;
    }

public:
    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool   empty() const { return m_size == 0; }

    T& operator[](size_t index)
    {
        if (index >= m_size)
        {
            throw std::out_of_range("index out of range");
        }
        return m_data[index];
    }
    void push_back(const T& value)
    {
        if (m_size == m_capacity)
        {
            resize(m_capacity * 2);
        }
        ::new (m_data + m_size) T(value); // dont use assignment for unconstruted memory
        m_size++;
    }

    void push_back(T&& value) // move semantics for optimization
    {
        if (m_size == m_capacity)
        {
            resize(m_capacity * 2);
        }
        ::new (m_data + m_size) T(std::move(value)); // move construct
        m_size++;
    }

    void emplace_back(T&& value)
    {
        if (m_size == m_capacity)
        {
            resize(m_capacity * 2);
        }
        ::new (m_data + m_size) T(std::move(value)); // move construct
        m_size++;
    }

    void pop_back()
    {
        if (m_size > 0)
        {
            m_data[m_size - 1].~T();
            m_size--;
        }
        else
        {
            throw std::out_of_range("pop_back() on empty vector");
        }
    }
};

} // namespace m_std