#ifndef FixPointData_H
#define FixPointData_H

//        void * memcpy ( void * destination, const void * source, size_t num );
#pragma pack(4)
template <typename UINT_T, int MAX_LEN> struct FixPointArray {
public:
    explicit FixPointArray(size_t s = MAX_LEN)
    {
        memset(Data, 0, MAX_LEN * sizeof(UINT_T));
    }
    FixPointArray(const FixPointArray& a)
    {
        memcpy((void*)(Data), (void*)a.Data, MAX_LEN * sizeof(UINT_T));
    }
    FixPointArray& operator=(const FixPointArray& a)
    {
        if(this != &a) {
            memcpy((void*)(Data), (void*)a.Data, MAX_LEN * sizeof(UINT_T));
        }
        return *this;
    }
    inline UINT_T& operator[](const size_t i)
    {
        return this->Data[i];
    }
    inline const UINT_T& operator[](const size_t i) const
    {
        return Data[i];
    }
    inline size_t size() const
    {
        return MAX_LEN;
    }
    UINT_T* data() noexcept
    {
        return this->Data;
    };
    const UINT_T* data() const noexcept
    {
        return this->Data;
    };
    //    inline UINT_T& back() { return this->Data[MAX_LEN - 1]; }
    inline UINT_T& at(const size_t i)
    {
        if(i < MAX_LEN && i >= 0) {
            return this->Data[i];
        } else {
            std::cerr << "FixPointArray Line " << __LINE__ << " : " << i << std::endl;
            return this->Data[0];
        }
    }

    //    static const int FixPoint_MAX_LEN = MAX_LEN;
    //  protected:
    UINT_T Data[MAX_LEN];
};
#pragma pack()

template <typename UINT_T, int MAX_LEN> class FixPointVector : public std::vector<UINT_T>
{
public:
    explicit FixPointVector(size_t s = MAX_LEN)
        : std::vector<UINT_T>(s){};
    //    static const int FixPoint_MAX_LEN = MAX_LEN;
};

#endif // FixPointData_H
