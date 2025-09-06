#ifndef REACTION_RESOURCE_H
#define REACTION_RESOURCE_H
#include "reaction/observeNode.h"

namespace reaction
{
    template <typename Type>
    class Resource : public ObserveNode
    {
    public:
        Resource() : m_ptr(nullptr) {}

        template <typename T>
        Resource(T &&t) : m_ptr(std::make_unique<Type>(std::forward<T>(t))) {}

        Resource(const Resource &) = delete;
        Resource &operator=(const Resource &) = delete;

        Resource(Resource &&) = default;
        Resource &operator=(Resource &&) = default;

        virtual ~Resource() = default;

        Type &getValue() { 
            if (!m_ptr)
            {
                throw std::runtime_error("Resource is not initialized");
            }
            
            return *m_ptr; 
        }
        const Type &getValue() const
         {
             if (!m_ptr)
             {
                 throw std::runtime_error("Resource is not initialized");
             }
            return *m_ptr; 
        }

        template <typename T>
        void updateValue(T&& value) {
            if (!m_ptr)
            {
                m_ptr = std::make_unique<std::decay_t<T>>(std::forward<T>(value));
            }
            else
            {
                *m_ptr = std::forward<T>(value);
            }
        }
    private:
        std::unique_ptr<Type> m_ptr;
    };

    template <>
    class Resource<void> : public ObserveNode
    {
    };
}
#endif  // RESOURCE_H