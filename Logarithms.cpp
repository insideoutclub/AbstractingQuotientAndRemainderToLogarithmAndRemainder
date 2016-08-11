// intrinsics.h

// Copyright (c) 2009 Alexander Stepanov and Paul McJones
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that the above copyright notice appear in all copies
// and that both that copyright notice and this permission notice
// appear in supporting documentation. The authors make no
// representations about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.


// Implementations from Appendix B.2 of
// Elements of Programming
// by Alexander Stepanov and Paul McJones
// Addison-Wesley Professional, 2009

// Template constraints

//  The requires clause is implemented with this macro (this
//  implementation treats requirements as documentation only):

#define requires(...)

// type_functions.h

// Copyright (c) 2009 Alexander Stepanov and Paul McJones
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that the above copyright notice appear in all copies
// and that both that copyright notice and this permission notice
// appear in supporting documentation. The authors make no
// representations about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.


// Implementations of type functions for
// Elements of Programming
// by Alexander Stepanov and Paul McJones
// Addison-Wesley Professional, 2009


// InputType : FunctionalProcedure x unsigned int -> Regular

template<typename T, int i>
requires(FunctionalProcedure(T))
struct input_type;

#define InputType(T, i) typename input_type< T, i >::type


// Domain : HomogeneousFunction -> Regular

#define Domain(T) InputType(T, 0)

template<typename T>
requires(ArchimedeanMonoid(T))
struct quotient_type;
#define QuotientType(T) typename quotient_type< T >::type

// integers.h

// Copyright (c) 2009 Alexander Stepanov and Paul McJones
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that the above copyright notice appear in all copies
// and that both that copyright notice and this permission notice
// appear in supporting documentation. The authors make no
// representations about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.


// Implementation for integral types of special-case Integer procedures from
// Elements of Programming
// by Alexander Stepanov and Paul McJones
// Addison-Wesley Professional, 2009

#include <ciso646> // bitand

template<typename I>
requires(Integer(I))
I successor(const I& a)
{
    return a + I(1);
}

template<typename I>
requires(Integer(I))
I twice(const I& a)
{
    return a + a;
}

template<typename I>
requires(Integer(I))
I half_nonnegative(const I& a)
{
    return a >> I(1);
}

template<typename I>
requires(Integer(I))
bool zero(const I& a)
{
    return a == I(0);
}

template<typename I>
requires(Integer(I))
bool one(const I& a)
{
    return a == I(1);
}

template<typename I>
requires(Integer(I))
bool even(const I& a)
{
    return (a bitand I(1)) == I(0);
}

template<typename I>
requires(Integer(I))
bool odd(const I& a)
{
    return (a bitand I(1)) != I(0);
}

// eop.h

// Copyright (c) 2009 Alexander Stepanov and Paul McJones
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that the above copyright notice appear in all copies
// and that both that copyright notice and this permission notice
// appear in supporting documentation. The authors make no
// representations about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.


// Algorithms from
// Elements of Programming
// by Alexander Stepanov and Paul McJones
// Addison-Wesley Professional, 2009


template<typename T>
requires(AdditiveSemigroup(T))
struct plus
{
    T operator()(const T& x, const T& y)
    {
        return x + y;
    }
};

template<typename T>
requires(AdditiveSemigroup(T))
struct input_type< plus<T>, 0 >
{
    typedef T type;
};

template<typename T>
requires(MultiplicativeSemigroup(T))
struct multiplies
{
    T operator()(const T& x, const T& y)
    {
        return x * y;
    }
};

template<typename T>
requires(MultiplicativeSemigroup(T))
struct input_type< multiplies<T>, 0 >
{
    typedef T type;
};

template<typename T0, typename T1>
requires(Regular(T0) && Regular(T1))
struct pair
{
    T0 m0;
    T1 m1;
    pair() {} // default constructor
    pair(const T0& m0, const T1& m1) : m0(m0), m1(m1) { }
};

template<typename T0, typename T1>
requires(Regular(T0) && Regular(T1))
bool operator==(const pair<T0, T1>& x, const pair<T0, T1>& y)
{
    return x.m0 == y.m0 && x.m1 == y.m1;
}

template<typename I, typename Op>
requires(Integer(I) && BinaryOperation(Op))
Domain(Op) power_accumulate_positive(Domain(Op) r,
Domain(Op) a, I n,
Op op)
{
    // Precondition: $\func{associative}(op) \wedge \func{positive}(n)$
    while (true) {
        if (odd(n)) {
            r = op(r, a);
            if (one(n)) return r;
        }
        a = op(a, a);
        n = half_nonnegative(n);
    }
}

template<typename I, typename Op>
requires(Integer(I) && BinaryOperation(Op))
Domain(Op) power(Domain(Op) a, I n, Op op)
{
    // Precondition: $\func{associative}(op) \wedge \func{positive}(n)$
    while (even(n)) {
        a = op(a, a);
        n = half_nonnegative(n);
    }
    n = half_nonnegative(n);
    if (zero(n)) return a;
    return power_accumulate_positive(a, op(a, a), n, op);
}

// Copyright (c) 2016 David T. Sanders

template<typename T>
struct divides
{
    T operator()(const T& x, const T& y)
    {
        return x / y;
    }
};

template<typename T>
divides<T> inverse(multiplies<T>) {
    return divides<T>();
}

template<typename T>
struct minus
{
    T operator()(const T& x, const T& y)
    {
        return x - y;
    }
};

template<typename T>
minus<T> inverse(plus<T>) {
    return minus<T>();
}

#include <cstdint>
#include <boost/rational.hpp>

template<typename T>
struct quotient_type<boost::rational<T>>
{
    typedef T type;
};

template<>
struct quotient_type<long long>
{
    typedef long long type;
};

template<typename Op>
pair<QuotientType(long long), long long>
builtin_quotient_remainder(long long a, long long b, Op op)
{
    return pair<QuotientType(long long), long long>(a / b, a % b);
}

template<typename Op>
pair<QuotientType(Domain(Op)), Domain(Op)>
slow_quotient_remainder(Domain(Op) a, Domain(Op) b, Op op)
{
    // Precondition: a >= 0 ^ b > 0
    typedef Domain(Op) T;
    typedef QuotientType(T) N;
    N n(0);
    while (b <= a) {
        a = inverse(op)(a, b);
        n = successor(n);
    }
    return pair<N, T>(n, a);
}

template<typename Op>
requires(HomogeneousFunction(Op) && Arity(Op) == 2 &&
    ArchimedeanGroup(Domain(Op)) &&
    Codomain(Op) == Domain(Op))
    pair<QuotientType(Domain(Op)), Domain(Op)>
    quotient_remainder_nonnegative_0(Domain(Op) a, Domain(Op) b, Op op)
{
    // Precondition: a >= 0 ^ b > 0
    typedef Domain(Op) T;
    typedef QuotientType(T) N;
    if (a < b) return pair<N, T>(N(0), a);
    if (inverse(op)(a, b) < b) return pair<N, T>(N(1), inverse(op)(a, b));
    pair<N, T> q = quotient_remainder_nonnegative_0(a, op(b, b), op);
    N m = twice(q.m0);
    a = q.m1;
    if (a < b) return pair<N, T>(m, a);
    else       return pair<N, T>(successor(m), inverse(op)(a, b));
}

template<typename Op>
requires(HomogeneousFunction(Op) && Arity(Op) == 2 &&
    ArchimedeanGroup(Domain(Op)) &&
    Codomain(Op) == Domain(Op))
    pair<QuotientType(Domain(Op)), Domain(Op)>
    quotient_remainder_nonnegative_1(Domain(Op) a, Domain(Op) b, Op op)
{
    // Precondition: a >= 0 ^ b > 0
    typedef Domain(Op) T;
    typedef QuotientType(T) N;
    if (a < b) return pair<N, T>(N(0), a);
    T const c = inverse(op)(a, b);
    if (c < b) return pair<N, T>(N(1), c);
    pair<N, T> q = quotient_remainder_nonnegative_1(a, op(b, b), op);
    N m = twice(q.m0);
    a = q.m1;
    if (a < b) return pair<N, T>(m, a);
    else       return pair<N, T>(successor(m), inverse(op)(a, b));
}

template<typename Op>
requires(HomogeneousFunction(Op) && Arity(Op) == 2 &&
    ArchimedeanGroup(Domain(Op)) &&
    Codomain(Op) == Domain(Op))
    pair<QuotientType(Domain(Op)), Domain(Op)>
    quotient_remainder_nonnegative_2(Domain(Op) a, Domain(Op) b, Op op)
{
    // Precondition: a >= 0 ^ b > 0
    typedef Domain(Op) T;
    typedef QuotientType(T) N;
    if (a < b) return pair<N, T>(N(0), a);
    T const c = op(b, b);
    if (a < c) return pair<N, T>(N(1), inverse(op)(a, b));
    pair<N, T> q = quotient_remainder_nonnegative_2(a, c, op);
    N m = twice(q.m0);
    a = q.m1;
    if (a < b) return pair<N, T>(m, a);
    else       return pair<N, T>(successor(m), inverse(op)(a, b));
}

template<typename Op>
requires(HomogeneousFunction(Op) && Arity(Op) == 2 &&
    ArchimedeanGroup(Domain(Op)) &&
    Codomain(Op) == Domain(Op))
    pair<QuotientType(Domain(Op)), Domain(Op)>
    largest_doubling(Domain(Op) a, Domain(Op) b, Op op)
{
    // Precondition: a >= b > 0
    typedef Domain(Op) T;
    typedef QuotientType(T) N;
    N m = N(1);
    T c = op(b, b);
    while (c <= a)
    {
        m = twice(m);
        b = c;
        c = op(c, c);
    }
    return pair<N, T>(m, b);
}

template<typename Op>
requires(HomogeneousFunction(Op) && Arity(Op) == 2 &&
    ArchimedeanGroup(Domain(Op)) &&
    Codomain(Op) == Domain(Op))
    pair<QuotientType(Domain(Op)), Domain(Op)>
    quotient_remainder_nonnegative_with_largest_doubling(Domain(Op) a, Domain(Op) b, Op op)
{
    // Precondition: a >= T(0) ^ b > T(0)
    typedef Domain(Op) T;
    typedef QuotientType(T) N;
    N quotient = N(0);
    while (b <= a)
    {
        pair<N, T> const m = largest_doubling(a, b, op);
        quotient += m.m0;
        a = inverse(op)(a, m.m1);
    }
    return pair<N, T>(quotient, a);
}

#include <tuple>

template<typename Op>
std::pair<Domain(Op), Domain(Op)> next_fibonacci(Domain(Op) x, Domain(Op) y, Op op)
{
    return std::pair<Domain(Op), Domain(Op)>(y, op(x, y));
}

template<typename Op>
std::pair<Domain(Op), Domain(Op)> previous_fibonacci(Domain(Op) x, Domain(Op) y, Op op) {
    return std::pair<Domain(Op), Domain(Op)>(inverse(op)(y, x), x);
}

template<typename Op>
requires(HomogeneousFunction(Op) && Arity(Op) == 2 &&
    ArchimedeanGroup(Domain(Op)) &&
    Codomain(Op) == Domain(Op))
    pair<QuotientType(Domain(Op)), Domain(Op)>
    quotient_remainder_nonnegative_fibonacci(Domain(Op) a, Domain(Op) b, Op op)
{
    // Precondition: a >= 0 ^ b > 0
    typedef Domain(Op) T;
    typedef QuotientType(T) N;
    N n = N(0);
    if (a < b) return pair<N, T>(n, a);
    T c = b;
    N d = N(1), e = N(1);
    plus<N> integer_op;
    do {
        std::tie(b, c) = next_fibonacci(b, c, op);
        std::tie(d, e) = next_fibonacci(d, e, integer_op);
    } while (a >= c);
    do {
        if (a >= b)
        {
            a = inverse(op)(a, b);
            n = integer_op(n, d);
        }
        std::tie(b, c) = previous_fibonacci(b, c, op);
        std::tie(d, e) = previous_fibonacci(d, e, integer_op);
    } while (b < c);
    return pair<N, T>(n, a);
}

template<typename T>
T identity_element(plus<T>)
{
    return T(0);
}

template<typename T>
T identity_element(multiplies<T>)
{
    return T(1);
}

// Adapted from https://programmingpraxis.com/2010/05/07/integer-logarithms/
template<typename Op>
requires(HomogeneousFunction(Op) && Arity(Op) == 2 &&
    ArchimedeanGroup(Domain(Op)) &&
    Codomain(Op) == Domain(Op))
    pair<QuotientType(Domain(Op)), Domain(Op)>
    ilog(Domain(Op) a, Domain(Op) b, Op op)
{
    typedef Domain(Op) T;
    typedef QuotientType(T) N;
    N lo(0), hi(1);
    T b_lo(identity_element(op)), b_hi(b);
    while (b_hi < a)
    {
        lo = hi;
        b_lo = b_hi;
        hi = twice(hi);
        b_hi = op(b_hi, b_hi);
    }
    while (hi - lo > 1)
    {
        N const mid = (lo + hi) / 2;
        T const b_mid = op(power(b, mid - lo, op), b_lo);
        if (a < b_mid)
        {
            hi = mid;
            b_hi = b_mid;
        }
        else if (b_mid < a)
        {
            lo = mid;
            b_lo = b_mid;
        }
        else
            return pair<N, T>(mid, inverse(op)(a, b_mid));
    }
    return b_hi == a ? pair<N, T>(hi, inverse(op)(a, b_hi)) : pair<N, T>(lo, inverse(op)(a, b_lo));
}

#include <chrono>
#include <iostream>

typedef long long int_type;

template<typename OutputIterator, typename InputIterator, typename Functor, typename Op>
void test_case(OutputIterator firstOut, OutputIterator lastOut, InputIterator firstGold, Functor functor, Op op)
{
    int_type const bs[] = { 2, 3, 5, 7 };

    auto const start = std::chrono::high_resolution_clock::now();
    int_type a = 1;
    auto currentOut = firstOut;
    while (true)
    {
        for (auto firstB = std::begin(bs), lastB = std::end(bs); firstB != lastB; ++firstB)
        {
            if (currentOut == lastOut)
                goto done;
            *currentOut++ = functor(a, *firstB, op);
        }
        ++a;
    }
done:
    auto const end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double>(end - start).count() << " s\n";
    std::cout << (std::equal(firstOut, lastOut, firstGold) ? "Success!" : "Failure") << std::endl;
}

#include <vector>

int main()
{
    auto const size = 4000000;
    {
        std::vector<pair<int_type, int_type>> expected(size);
        std::vector<pair<int_type, int_type>> actual(size);

        typedef plus<int_type> plus;
        test_case(std::begin(expected), std::end(expected), std::begin(expected), builtin_quotient_remainder<plus>, plus());
        //test_case(std::begin(actual), std::end(actual), std::begin(expected), slow_quotient_remainder<operation_type>);
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_0<plus>, plus());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_1<plus>, plus());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_2<plus>, plus());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_with_largest_doubling<plus>, plus());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_fibonacci<plus>, plus());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), ilog<plus>, plus());

        typedef multiplies<int_type> multiplies_int_type;
        test_case(std::begin(expected), std::end(expected), std::begin(expected), quotient_remainder_nonnegative_0<multiplies_int_type>, multiplies_int_type());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_1<multiplies_int_type>, multiplies_int_type());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_2<multiplies_int_type>, multiplies_int_type());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_with_largest_doubling<multiplies_int_type>, multiplies_int_type());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_fibonacci<multiplies_int_type>, multiplies_int_type());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), ilog<multiplies_int_type>, multiplies_int_type());
    }

    {
        typedef boost::rational<int_type> rational_type;
        std::vector<pair<int_type, rational_type>> expected(size);
        std::vector<pair<int_type, rational_type>> actual(size);

        typedef multiplies<boost::rational<int_type>> multiplies_rational_type;
        test_case(std::begin(expected), std::end(expected), std::begin(expected), quotient_remainder_nonnegative_0<multiplies_rational_type>, multiplies_rational_type());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_1<multiplies_rational_type>, multiplies_rational_type());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_2<multiplies_rational_type>, multiplies_rational_type());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_with_largest_doubling<multiplies_rational_type>, multiplies_rational_type());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), quotient_remainder_nonnegative_fibonacci<multiplies_rational_type>, multiplies_rational_type());
        test_case(std::begin(actual), std::end(actual), std::begin(expected), ilog<multiplies_rational_type>, multiplies_rational_type());
    }
}
