#ifndef REACTION_CONCEPT_H
#define REACTION_CONCEPT_H
#include <type_traits>
#include <memory>
namespace reaction
{
    //=================== forward declaration =================//
    struct VarExpr;
    template <typename T, typename... Args>
    class ReactImpl;
    template <typename T>
    class React;

    class ObserveNode;
    using NodePtr = std::shared_ptr<ObserveNode>;

    //=================== Expression Traits =================//
    template <typename T>
    struct ExpressionTraits
    {
        using type = T;
    };

    template <typename T>
    struct ExpressionTraits<React<ReactImpl<T>>>
    {
        using type = T;
    };

    template <typename Fun, typename... Args>
    struct ExpressionTraits<React<ReactImpl<Fun, Args...>>>
    {
        using type = std::invoke_result_t<Fun, typename ExpressionTraits<Args>::type...>;
    };

    template <typename Fun, typename... Args>
    using ReturnType = typename ExpressionTraits<React<ReactImpl<Fun, Args...>>>::type;

    //=================== Concepts =================//
    template <typename T, typename U>
    concept Convertable = std::is_convertible_v<std::decay_t<U>, T>;

    template <typename T>
    concept IsVarExpr = std::is_same_v<T, VarExpr>;

    template <typename T>
    concept IsConstType = std::is_const_v<std::remove_reference_t<T>>;

    template <typename T>
    concept IsVoidType = std::is_void_v<std::remove_reference_t<T>>;

    template <typename T>
    concept IsReactNode = requires(T t) {
        { t.shared_from_this() } -> std::convertible_to<NodePtr>;
    };

    template <typename T>
    concept IsDataReact = requires(T t) {
        typename T::ValueType;
        requires IsReactNode<T> && !IsVoidType<typename T::ValueType>;
    };
}
#endif // REACTION_CONCEPT_H