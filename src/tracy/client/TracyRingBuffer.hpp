#include <atomic>
#include <assert.h>
#include <errno.h>
#include <linux/perf_event.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "TracyDebug.hpp"

namespace tracy
{

class RingBuffer
{
public:
    RingBuffer( unsigned int size, int fd, int id, int cpu = -1 )
        : m_size( size )
        , m_id( id )
        , m_cpu( cpu )
        , m_fd( fd )
    {
        const auto pageSize = uint32_t( getpagesize() );
        assert( size >= pageSize );
        assert( __builtin_popcount( size ) == 1 );
        m_mapSize = size + pageSize;
        auto mapAddr = mmap( nullptr, m_mapSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
        if( mapAddr == MAP_FAILED )
        {
            TracyDebug( "mmap failed: errno %i (%s)\n", errno, strerror( errno ) );
            m_fd = 0;
            mMetadata = nullptr;
            close( fd );
            return;
        }
        mMetadata = (perf_event_mmap_page*)mapAddr;
        assert( mMetadata->data_offset == pageSize );
        m_buffer = ((char*)mapAddr) + pageSize;
        m_tail = mMetadata->data_tail;
    }

    ~RingBuffer()
    {
        if( mMetadata ) munmap( mMetadata, m_mapSize );
        if( m_fd ) close( m_fd );
    }

    RingBuffer( const RingBuffer& ) = delete;
    RingBuffer& operator=( const RingBuffer& ) = delete;

    RingBuffer( RingBuffer&& other )
    {
        memcpy( (char*)&other, (char*)this, sizeof( RingBuffer ) );
        mMetadata = nullptr;
        m_fd = 0;
    }

    RingBuffer& operator=( RingBuffer&& other )
    {
        memcpy( (char*)&other, (char*)this, sizeof( RingBuffer ) );
        mMetadata = nullptr;
        m_fd = 0;
        return *this;
    }

    bool IsValid() const { return mMetadata != nullptr; }
    int GetId() const { return m_id; }
    int GetCpu() const { return m_cpu; }

    void Enable()
    {
        ioctl( m_fd, PERF_EVENT_IOC_ENABLE, 0 );
    }

    void Read( void* dst, uint64_t offset, uint64_t cnt )
    {
        const auto size = m_size;
        auto src = ( m_tail + offset ) % size;
        if( src + cnt <= size )
        {
            memcpy( dst, m_buffer + src, cnt );
        }
        else
        {
            const auto s0 = size - src;
            const auto buf = m_buffer;
            memcpy( dst, buf + src, s0 );
            memcpy( (char*)dst + s0, buf, cnt - s0 );
        }
    }

    void Advance( uint64_t cnt )
    {
        m_tail += cnt;
        StoreTail();
    }

    bool CheckTscCaps() const
    {
        return mMetadata->cap_user_time_zero;
    }

    int64_t ConvertTimeToTsc( int64_t timestamp ) const
    {
        if( !mMetadata->cap_user_time_zero ) return 0;
        const auto time = timestamp - mMetadata->time_zero;
        const auto quot = time / mMetadata->time_mult;
        const auto rem = time % mMetadata->time_mult;
        return ( quot << mMetadata->time_shift ) + ( rem << mMetadata->time_shift ) / mMetadata->time_mult;
    }

    uint64_t LoadHead() const
    {
        return std::atomic_load_explicit( (const volatile std::atomic<uint64_t>*)&mMetadata->data_head, std::memory_order_acquire );
    }

    uint64_t GetTail() const
    {
        return m_tail;
    }

private:
    void StoreTail()
    {
        std::atomic_store_explicit( (volatile std::atomic<uint64_t>*)&mMetadata->data_tail, m_tail, std::memory_order_release );
    }

    unsigned int m_size;
    uint64_t m_tail;
    char* m_buffer;
    int m_id;
    int m_cpu;
    perf_event_mmap_page* mMetadata;

    size_t m_mapSize;
    int m_fd;
};

}
