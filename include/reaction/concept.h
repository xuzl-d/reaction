#include <type_traits>
#include "reaction/expression.h"
namespace reaction
{
    template <typename T, typename U>
    concept Convertable = std::is_convertible_v<std::decay_t<U>, T>;

    template <typename T>
    concept IsVarExpr = std::is_same_v<T, VarExpr>;
}