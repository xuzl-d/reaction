#ifndef REACTION_UTILITY_H
#define REACTION_UTILITY_H

#include <cstdint>
#include <atomic>
#include <unordered_set>
namespace reaction
{
    class UniqueID
    {
    public:
        UniqueID() : m_id(generate())
        {
        }
        UniqueID(uint64_t id) : m_id(id)
        {
        }
        operator uint16_t() const
        {
            return m_id;
        }

        bool operator==(const UniqueID &other) const
        {
            return m_id == other.m_id;
        }

    private:
        uint64_t generate()
        {
            static std::atomic<uint64_t> counter{0};
            return counter.fetch_add(1, std::memory_order_relaxed);
        }

        uint16_t m_id;

        friend struct std::hash<UniqueID>;
    };
}

namespace std
{
    template <>
    struct hash<reaction::UniqueID>
    {
        size_t operator()(const reaction::UniqueID &uid) const
        {
            return hash<uint64_t>()(uid);
        }
    };
}

#endif // REACTION_UTILITY_H