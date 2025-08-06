#ifndef REACTION_ReactImpl_H
#define REACTION_ReactImpl_H
#include "reaction/expression.h"
#include "reaction/concept.h"
namespace reaction
{
    template <typename T, typename... Args>
    class ReactImpl : public Expression<T, Args...>, public std::enable_shared_from_this<ReactImpl<T, Args...>>
    {
    public:
        using Expression<T, Args...>::Expression;
        using ValueType = Expression<T, Args...>::ValueType;
        using ExprType = Expression<T, Args...>::ExprType;
        auto get() const
        {
            return this->getValue();
        }

        template <typename U>
            requires(Convertable<U, T> && IsVarExpr<ExprType>)
        void value(U &&value)
        {
            this->updateValue(std::forward<U>(value));
            this->notify();
        }

        void addWeakRef()
        {
            ++m_wearRefCount;
        }

        void releaseWeakRef()
        {
            if (--m_wearRefCount == 0)
            {
                ObserverGraph::getInstance().removeNode(this->shared_from_this());
            }
        }

    private:
        std::atomic<int> m_wearRefCount = 0;
    };

    template <typename ReactType>
    class React
    {
    public:
        explicit React(std::shared_ptr<ReactType> ptr)
            : m_weakPtr(ptr)
        {
            if (auto p = m_weakPtr.lock())
            {
                p->addWeakRef();
            }
        }

        ~React()
        {
            if (auto p = m_weakPtr.lock())
            {
                p->releaseWeakRef();
            }
        }
        // 复制构造函数
        React(const React &other) : m_weakPtr(other.m_weakPtr)
        {
            if (auto p = m_weakPtr.lock())
            {
                p->addWeakRef();
            }
        }

        // 移动构造函数
        React(React &&other) : m_weakPtr(std::move(other.m_weakPtr))
        {
            other.m_weakPtr.reset();
        }

        // 复制赋值运算符
        React &operator=(const React &other)
        {
            if (this != &other)
            {
                if (auto p = m_weakPtr.lock())
                {
                    p->releaseWeakRef();
                }
                m_weakPtr = other.m_weakPtr;
                if (auto p = m_weakPtr.lock())
                {
                    p->addWeakRef();
                }
            }
            return *this;
        }

        // 移动赋值运算符
        React &operator=(React &&other)
        {
            if (this == &other)
            {
                if (auto p = m_weakPtr.lock())
                {
                    p->releaseWeakRef();
                }
                m_weakPtr = std::move(other.m_weakPtr);
                other.m_weakPtr.reset();
            }
            return *this;
        }

        explicit operator bool() const
        {
            return !m_weakPtr.expired();
        }

        auto get() const
        {
            return getPtr()->get();
        }

        template <typename U>
        void value(U &&value)
        {
            getPtr()->value(std::forward<U>(value));
        }

        std::shared_ptr<ReactType> getPtr() const
        {
            if (auto p = m_weakPtr.lock())
            {
                return p;
            }
            throw std::runtime_error("Weak pointer expired");
        }

    private:
        std::weak_ptr<ReactType> m_weakPtr;
    };

    template <typename T>
    auto var(T &&value)
    {
        auto ptr = std::make_shared<ReactImpl<T>>(std::forward<T>(value));
        ObserverGraph::getInstance().addNode(ptr);
        return React(ptr);
    }

    template <typename F, typename... Args>
    auto calc(F &&f, Args &&...args)
    {
        auto ptr = std::make_shared<ReactImpl<std::decay_t<F>, std::decay_t<Args>...>>(std::forward<F>(f), std::forward<Args>(args)...);
        ObserverGraph::getInstance().addNode(ptr);
        return React(ptr);
    }

} // namespace reaction
#endif // REACTION_ReactImpl_H
