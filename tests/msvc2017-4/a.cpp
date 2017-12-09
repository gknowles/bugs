// Copyright Glen Knowles 2017.
// Distributed under the Boost Software License, Version 1.0.
//
// a.cpp - bugs msvc2017-4
//
// Compile with: Visual C++ 2017 Version 15.5.1
// (This is not a problem with 2017 15.4.5)
//
// Bad code generation with /O2

#include <algorithm>
#include <cassert>
#include <limits>
#include <memory>
#include <string_view>

using namespace std;


/****************************************************************************
*
*   Temp heap interface
*
***/

class ITempHeap {
public:
    virtual ~ITempHeap() = default;

    template <typename T, typename... Args> T * emplace(Args &&... args);
    template <typename T> T * alloc(size_t num);

    char * strdup(const char src[]);
    char * strdup(std::string_view src);
    char * strdup(const char src[], size_t len);

    char * alloc(size_t bytes);
    virtual char * alloc(size_t bytes, size_t align) = 0;
};

//===========================================================================
template <typename T, typename... Args>
inline T * ITempHeap::emplace(Args &&... args) {
    char * tmp = alloc(sizeof(T), alignof(T));
    return new (tmp) T(args...);
}

//===========================================================================
template <typename T> inline T * ITempHeap::alloc(size_t num) {
    char * tmp = alloc(num * sizeof(T), alignof(T));
    return new (tmp) T[num];
}

//===========================================================================
inline char * ITempHeap::strdup(const char src[]) {
    return strdup(std::string_view(src));
}

//===========================================================================
inline char * ITempHeap::strdup(std::string_view src) {
    return strdup(src.data(), src.size());
}

//===========================================================================
inline char * ITempHeap::strdup(const char src[], size_t len) {
    char * out = alloc(len + 1, alignof(char));
    std::memcpy(out, src, len);
    out[len] = 0;
    return out;
}

//===========================================================================
inline char * ITempHeap::alloc(size_t bytes) {
    return alloc(bytes, alignof(char));
}


/****************************************************************************
*
*   TempHeap
*
***/

class TempHeap : public ITempHeap {
public:
    ~TempHeap();
    TempHeap & operator=(const TempHeap & from) = delete;
    TempHeap & operator=(TempHeap && from);

    void clear();
    void swap(TempHeap & from);

    // ITempHeap
    char * alloc(size_t bytes, size_t align) override;

private:
    void * m_buffer{nullptr};
};


/****************************************************************************
*
*   Tuning parameters
*
***/

const unsigned kInitBufferSize = 256;
const unsigned kMaxBufferSize = 4096;


/****************************************************************************
*
*   Declarations
*
***/

namespace {

static_assert(kMaxBufferSize < numeric_limits<unsigned>::max());

struct Buffer {
    Buffer * m_next;
    unsigned m_avail;
    unsigned m_reserve;
};

} // namespace


/****************************************************************************
*
*   TempHeap
*
***/

//===========================================================================
TempHeap::~TempHeap() {
    clear();
}

//===========================================================================
TempHeap & TempHeap::operator=(TempHeap && from) {
    swap(from);
    from.clear();
    return *this;
}

//===========================================================================
void TempHeap::clear() {
    Buffer * ptr = (Buffer *)m_buffer;
    while (ptr) {
        Buffer * next = ptr->m_next;
        free(ptr);
        ptr = next;
    }
    m_buffer = nullptr;
}

//===========================================================================
void TempHeap::swap(TempHeap & from) {
    std::swap(m_buffer, from.m_buffer);
}

//===========================================================================
char * TempHeap::alloc(size_t bytes, size_t align) {
    Buffer * buf = (Buffer *)m_buffer;
    Buffer * tmp;
    constexpr unsigned kBufferLen = sizeof(Buffer);
    for (;;) {
        if (buf) {
            void * ptr = (char *)buf + buf->m_reserve - buf->m_avail;
            size_t avail = buf->m_avail;
            if (std::align(align, bytes, ptr, avail)) {
                buf->m_avail = unsigned(avail - bytes);
                return (char *)ptr;
            }
        }
        auto required = unsigned(bytes + align);
        if (required > kMaxBufferSize / 3) {
            tmp = (Buffer *)malloc(kBufferLen + required);
            assert(tmp != nullptr);
            tmp->m_avail = required;
            tmp->m_reserve = kBufferLen + required;
            if (buf) {
                tmp->m_next = buf->m_next;
                buf->m_next = tmp;
            } else {
                tmp->m_next = nullptr;
                m_buffer = tmp;
            }
        } else {
            auto reserve = buf
                ? min(kMaxBufferSize, 2 * buf->m_reserve)
                : kInitBufferSize;
            if (reserve < kBufferLen + required)
                reserve = kBufferLen + required;
            tmp = (Buffer *)malloc(reserve);
            assert(tmp != nullptr);
            tmp->m_next = buf;
            tmp->m_avail = reserve - kBufferLen;
            tmp->m_reserve = reserve;
            m_buffer = tmp;
        }
        buf = tmp;
    }
}


/****************************************************************************
*
*   main
*
***/

//===========================================================================
int main() {
    TempHeap heap;
    heap.strdup("hello");
    puts("All tests passed");
}
