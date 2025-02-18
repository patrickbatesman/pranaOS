/**
 * @file stdlibextras.h
 * @author Krisna Pranav
 * @brief StdLIBExtra.h
 * @version 1.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define UNUSED_PARAM(x) (void)x

/**
 * @brief round_up_to_power_of_two
 * 
 * @param value 
 * @param power_of_two 
 * @return constexpr unsigned 
 */
constexpr unsigned round_up_to_power_of_two(unsigned value, unsigned power_of_two) {
    return ((value - 1) & ~(power_of_two - 1)) + power_of_two;
}

namespace Mods {

    template<typename T>
    auto declval() -> T;

    /**
     * @brief array_size
     * 
     * @tparam T 
     * @tparam SizeType 
     * @tparam N 
     * @return constexpr SizeType 
     */
    template<typename T, typename SizeType = decltype(sizeof(T)), SizeType N>
    constexpr SizeType array_size(T (&)[N]) {
        return N;
    }

    /**
     * @brief min
     * 
     * @tparam T 
     * @param a 
     * @param b 
     * @return constexpr T 
     */
    template<typename T>
    constexpr T min(const T& a, const T& b) {
        return b < a ? b : a;
    }

    /**
     * @brief max
     * 
     * @tparam T 
     * @param a 
     * @param b 
     * @return constexpr T 
     */
    template<typename T>
    constexpr T max(const T& a, const T& b) {
        return a < b ? b : a;
    }
    
    /**
     * @brief clamp
     * 
     * @tparam T 
     * @param value 
     * @param min 
     * @param max 
     * @return constexpr T 
     */
    template<typename T>
    constexpr T clamp(const T& value, const T& min, const T& max) {
        ASSERT(max >= min);
        if (value > max)
            return max;
        if (value < min)
            return min;
        return value;
    }

    /**
     * @brief ceil_dev
     * 
     * @tparam T 
     * @tparam U 
     * @param a 
     * @param b 
     * @return constexpr T 
     */
    template<typename T, typename U>
    constexpr T ceil_div(T a, U b) {
        static_assert(sizeof(T) == sizeof(U));
        T result = a / b;
        if ((a % b) != 0)
            ++result;
        return result;
    }

    #ifdef __clang__
    #    pragma clang diagnostic push
    #    pragma clang diagnostic ignored "-Wconsumed"
    #endif
    template<typename T>
    constexpr T&& move(T& arg) {
        return static_cast<T&&>(arg);
    }
    #ifdef __clang__
    #    pragma clang diagnostic pop
    #endif

    /**
     * @brief swap
     * 
     * @tparam T 
     * @tparam U 
     * @param a 
     * @param b 
     */
    template<typename T, typename U>
    inline void swap(T& a, U& b) {
        U tmp = move((U&)a);
        a = (T &&) move(b);
        b = move(tmp);
    }

    /**
     * @brief EnableIf
     * 
     * @tparam B 
     * @tparam T 
     */
    template<bool B, class T = void>
    struct EnableIf {
    };

    template<class T>
    struct EnableIf<true, T> {
        using Type = T;
    };

    template<class T>
    struct AddConst {
        using Type = const T;
    };

    template<class T>
    struct RemoveConst {
        using Type = T;
    };

    template<class T>
    struct RemoveConst<const T> {
        using Type = T;
    };

    template<class T>
    struct RemoveVolatile {
        using Type = T;
    };
    template<class T>
    struct RemoveVolatile<volatile T> {
        using Type = T;
    };
    template<class T>
    struct RemoveCV {
        using Type = typename RemoveVolatile<typename RemoveConst<T>::Type>::Type;
    };

    template<class T, T v>
    struct IntegralConstant {
        static constexpr T value = v;
        using ValueType = T;
        using Type = IntegralConstant;
        constexpr operator ValueType() const { return value; }
        constexpr ValueType operator()() const { return value; }
    };

    using FalseType = IntegralConstant<bool, false>;
    using TrueType = IntegralConstant<bool, true>;
    template<typename...>
    using VoidType = void;

    template<class T>
    struct IsLvalueReference : FalseType {
    };

    template<class T>
    struct IsLvalueReference<T&> : TrueType {
    };

    template<class T>
    struct __IsPointerHelper : FalseType {
    };

    template<class T>
    struct __IsPointerHelper<T*> : TrueType {
    };

    template<class T>
    struct IsPointer : __IsPointerHelper<typename RemoveCV<T>::Type> {
    };

    template<class>
    struct IsFunction : FalseType {
    };

    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...)> : TrueType {
    };

    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...)> : TrueType {
    };

    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...) const> : TrueType {
    };

    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...) const> : TrueType {
    };

    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...) volatile> : TrueType {
    };

    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...) volatile> : TrueType {
    };

    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...) const volatile> : TrueType {
    };

    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...) const volatile> : TrueType {
    };

    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...)&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...)&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...) const&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...) const&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...) volatile&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...) volatile&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...) const volatile&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...) const volatile&> : TrueType {
    };

    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...) &&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...) &&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...) const&&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...) const&&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...) volatile&&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...) volatile&&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args...) const volatile&&> : TrueType {
    };
    template<class Ret, class... Args>
    struct IsFunction<Ret(Args..., ...) const volatile&&> : TrueType {
    };

    template<class T>
    struct IsRvalueReference : FalseType {
    };
    template<class T>
    struct IsRvalueReference<T&&> : TrueType {
    };

    template<class T>
    struct RemovePointer {
        using Type = T;
    };
    template<class T>
    struct RemovePointer<T*> {
        using Type = T;
    };
    template<class T>
    struct RemovePointer<T* const> {
        using Type = T;
    };
    template<class T>
    struct RemovePointer<T* volatile> {
        using Type = T;
    };
    template<class T>
    struct RemovePointer<T* const volatile> {
        using Type = T;
    };

    template<typename T, typename U>
    struct IsSame {
        enum {
            value = 0
        };
    };

    template<typename T>
    struct IsSame<T, T> {
        enum {
            value = 1
        };
    };

    template<bool condition, class TrueType, class FalseType>
    struct Conditional {
        using Type = TrueType;
    };

    template<class TrueType, class FalseType>
    struct Conditional<false, TrueType, FalseType> {
        using Type = FalseType;
    };

    template<typename T>
    struct IsNullPointer : IsSame<decltype(nullptr), typename RemoveCV<T>::Type> {
    };

    template<typename T>
    struct RemoveReference {
        using Type = T;
    };
    template<class T>
    struct RemoveReference<T&> {
        using Type = T;
    };
    template<class T>
    struct RemoveReference<T&&> {
        using Type = T;
    };

    /**
     * @brief forward
     * 
     * @tparam T 
     * @param param 
     * @return constexpr T&& 
     */
    template<class T>
    constexpr T&& forward(typename RemoveReference<T>::Type& param) {
        return static_cast<T&&>(param);
    }

    /**
     * @brief forward
     * 
     * @tparam T 
     * @param param 
     * @return constexpr T&& 
     */
    template<class T>
    constexpr T&& forward(typename RemoveReference<T>::Type&& param) noexcept
    {
        static_assert(!IsLvalueReference<T>::value, "Can't forward an rvalue as an lvalue.");
        return static_cast<T&&>(param);
    }

    template<typename T>
    struct MakeUnsigned {
    };

    template<>
    struct MakeUnsigned<signed char> {
        using Type = unsigned char;
    };

    template<>
    struct MakeUnsigned<short> {
        using Type = unsigned short;
    };

    template<>
    struct MakeUnsigned<int> {
        using Type = unsigned int;
    };

    template<>
    struct MakeUnsigned<long> {
        using Type = unsigned long;
    };

    template<>
    struct MakeUnsigned<long long> {
        using Type = unsigned long long;
    };

    template<>
    struct MakeUnsigned<unsigned char> {
        using Type = unsigned char;
    };

    template<>
    struct MakeUnsigned<unsigned short> {
        using Type = unsigned short;
    };

    template<>
    struct MakeUnsigned<unsigned int> {
        using Type = unsigned int;
    };

    template<>
    struct MakeUnsigned<unsigned long> {
        using Type = unsigned long;
    };

    template<>
    struct MakeUnsigned<unsigned long long> {
        using Type = unsigned long long;
    };

    template<>
    struct MakeUnsigned<char> {
        using Type = unsigned char;
    };

    template<typename T>
    struct MakeSigned {
    };

    template<>
    struct MakeSigned<signed char> {
        using Type = signed char;
    };

    template<>
    struct MakeSigned<short> {
        using Type = short;
    };

    template<>
    struct MakeSigned<int> {
        using Type = int;
    };

    template<>
    struct MakeSigned<long> {
        using Type = long;
    };

    template<>
    struct MakeSigned<long long> {
        using Type = long long;
    };

    template<>
    struct MakeSigned<unsigned char> {
        using Type = char;
    };

    template<>
    struct MakeSigned<unsigned short> {
        using Type = short;
    };

    template<>
    struct MakeSigned<unsigned int> {
        using Type = int;
    };

    template<>
    struct MakeSigned<unsigned long> {
        using Type = long;
    };

    template<>
    struct MakeSigned<unsigned long long> {
        using Type = long long;
    };

    template<>
    struct MakeSigned<char> {
        using Type = signed char;
    };

    template<class T>
    struct IsVoid : IsSame<void, typename RemoveCV<T>::Type> {
    };

    template<class T>
    struct IsConst : FalseType {
    };

    template<class T>
    struct IsConst<const T> : TrueType {
    };

    /**
     * @brief exchange
     * 
     * @tparam T 
     * @tparam U 
     * @param slot 
     * @param value 
     * @return constexpr T 
     */
    template<typename T, typename U = T>
    constexpr T exchange(T& slot, U&& value) {
        T old_value = move(slot);
        slot = forward<U>(value);
        return old_value;
    }

    template<typename T>
    struct IsUnion : public IntegralConstant<bool, __is_union(T)> {
    };

    template<typename T>
    struct IsClass : public IntegralConstant<bool, __is_class(T)> {
    };

    template<typename Base, typename Derived>
    struct IsBaseOf : public IntegralConstant<bool, __is_base_of(Base, Derived)> {
    };

    /**
     * @brief is_trivial
     * 
     * @tparam T 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool is_trivial() {
        return __is_trivial(T);
    }

    /**
     * @brief is_trivial_copyable
     * 
     * @tparam T 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool is_trivially_copyable() {
        return __is_trivially_copyable(T);
    }

    template<typename T>
    struct __IsIntegral : FalseType {
    };

    template<>
    struct __IsIntegral<unsigned char> : TrueType {
    };

    template<>
    struct __IsIntegral<unsigned short> : TrueType {
    };

    template<>
    struct __IsIntegral<unsigned int> : TrueType {
    };

    template<>
    struct __IsIntegral<unsigned long> : TrueType {
    };

    template<>
    struct __IsIntegral<unsigned long long> : TrueType {
    };

    template<typename T>
    using IsIntegral = __IsIntegral<typename MakeUnsigned<typename RemoveCV<T>::Type>::Type>;

    template<typename T>
    struct __IsFloatingPoint : FalseType {
    };

    template<>
    struct __IsFloatingPoint<float> : TrueType {
    };

    template<>
    struct __IsFloatingPoint<double> : TrueType {
    };

    template<typename T>
    using IsFloatingPoint = __IsFloatingPoint<typename RemoveCV<T>::Type>;

    template<typename ReferenceType, typename T>
    using CopyConst =
        typename Conditional<IsConst<ReferenceType>::value, typename AddConst<T>::Type, typename RemoveConst<T>::Type>::Type;

    template<typename... Ts>
    using Void = void;

    template<typename... _Ignored>
    constexpr auto DependentFalse = false;

}

using Mods::AddConst;
using Mods::array_size;
using Mods::ceil_div;
using Mods::clamp;
using Mods::Conditional;
using Mods::declval;
using Mods::DependentFalse;
using Mods::exchange;
using Mods::forward;
using Mods::is_trivial;
using Mods::is_trivially_copyable;
using Mods::IsBaseOf;
using Mods::IsClass;
using Mods::IsConst;
using Mods::IsSame;
using Mods::IsUnion;
using Mods::IsVoid;
using Mods::MakeSigned;
using Mods::MakeUnsigned;
using Mods::max;
using Mods::min;
using Mods::move;
using Mods::RemoveConst;
using Mods::swap;
using Mods::Void;