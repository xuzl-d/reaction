#ifndef REACTION_DATASOURCE_H
#define REACTION_DATASOURCE_H
#include "reaction/expression.h"

namespace reaction
{
    template <typename T, typename... Args>
    class DataSource : public Expression<T, Args...>
    {
    public:
        using Expression<T, Args...>::Expression;
        auto get() const
        {
            return this->getValue();
        }
    };

    template <typename T>
    auto var(T &&value)
    {
        return DataSource<T>(std::forward<T>(value));
    }

    template <typename F, typename... Args>
    auto calc(F &&f, Args &&...args)
    {
        return DataSource<std::decay_t<F>, std::decay_t<Args>...>(std::forward<F>(f), std::forward<Args>(args)...);
    }
} // namespace reaction
#endif // REACTION_DATASOURCE_H
