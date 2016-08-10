# Abstracting Quotient And Remainder To Logarithm And Remainder

I have benefitted greatly from multiple readings of  *Elements of Programming* by Alexander Stepanov and Paul McJones as well as *From Mathematics to Generic Programming* by Stepanov and Daniel Rose. Each time I read either work, I learn something new.

One algorithm that receives extensive coverage early in both books is the Egyptian multiplication algorithm. Here's a simple, unoptimized version of that algorithm from http://www.elementsofprogramming.com/code/eop.h:

```c++
template<typename I, typename Op>
    requires(Integer(I) && BinaryOperation(Op))
Domain(Op) power_0(Domain(Op) a, I n, Op op)
{
    // Precondition: $\func{associative}(op) \wedge n > 0$
    if (n == I(1)) return a;
    if (n % I(2) == I(0))
        return power_0(op(a, a), n / I(2), op);
    return op(power_0(op(a, a), n / I(2), op), a);
}
```

If you use *std::plus\<int\>* as the binary operation, the algorithm will perform multiplication (*n x a*) in *O(log<sub>2</sub>(n))* steps. If you take the exact same code, but use *std::multiplies\<int\>* as the binary operation, the algorithm will perform the operation of raising to a power (*a<sup>n</sup>*), again in *O(log<sub>2</sub>(n))* steps. You're not stuck with integers, either. The algorithm will work with any type that has an associative binary operation (a.k.a. a noncommutative additive subgroup). Examples of noncommutative additive subgroups include real numbers and polynomials.

This is one of the fundamental themes of Stepanov's work. Take an algorithm that does something useful in a specific context, figure out the minimal requirements for the types used by that algorithm, and then abstract or expand the algorithm to do something useful in many broader contexts.

The Egyptian multiplication/power algorithm works by breaking down the big problem into smaller steps. At each step, the value of *a* is doubled and the value of *n* is halved. The Egyptians used the same idea in reverse to perform division with remainder. Again, here's a simple, unoptimized version of that algorithm from http://www.elementsofprogramming.com/code/eop.h:

```c++
template<typename T>
    requires(ArchimedeanMonoid(T))
pair<QuotientType(T), T>
quotient_remainder_nonnegative(T a, T b)
{
    // Precondition: $a \geq 0 \wedge b > 0$
    typedef QuotientType(T) N;
    if (a < b) return pair<N, T>(N(0), a);
    if (a - b < b) return pair<N, T>(N(1), a - b);
    pair<N, T> q = quotient_remainder_nonnegative(a, b + b);
    N m = twice(q.m0);
    a = q.m1;
    if (a < b) return pair<N, T>(m, a);
    else       return pair<N, T>(successor(m), a - b);
}
```
