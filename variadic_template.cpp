/**
 * Example of variadic template
 *
 * Use pattern
 * 1. pass as parameter to another function
 * 2. make base case, and call recursively.
 */
#include <iostream>

// case 1:
template<typename Mapper, typename Reducer, typename ...Args>
decltype(auto) mapReduce(Mapper mapper, Reducer reducer, Args&&... args)
{
    return reducer(std::forward<decltype(mapper(std::forward<Args>(args)))>(mapper(std::forward<Args>(args)))... );
}


// case 2:
class dog
{
 public:
    void speak() const { std::cout << "bow wow" << std::endl; }
};

class cat
{
 public:
    void speak() const { std::cout << "meow" << std::endl; }
};

class human
{
 public:
    void speak() const { std::cout << "get out" << std::endl; }
};



// Variadic Template example
// If this function doesn't exist, then it must have compile error.
template <typename T>
void
speak(const T& p)
{
    std::cout << "speak maaaan~\n";
    p.speak();
}

template <typename T, typename... ARGS>
void
speak(const T& p, ARGS ... args)
{
    std::cout << "speak variadic maaaan~\n";
    p.speak();
    speak(args...);
}

template <class dog, class cat>
void
speak_dog_and_cat(const dog& d, const cat& c)
{
    std::cout << "speak meeeeeen~\n";
    d.speak();
}

template <class dog, class cat, class... ARGS>
void
speak_dog_and_cat(const dog& d, const cat& c, ARGS... args)
{
    std::cout << "speak variadic meeeeeen~\n";
    c.speak();
    speak_dog_and_cat(args...);
}

template <typename T>
constexpr inline T
sum(T t)
{
    return t;
}

// check T is arithmetic template by  std::is_arithmetic and std::enable_if
template <typename T,
          typename = std::enable_if_t<std::is_arithmetic<T>::value>,
          typename ...Args>
decltype(auto)
sum(T t, Args... args)
{
    return t + sum(args...);
}

// ellipse operator

/**
 * Function Argument list
 * 1. f(&args...); // expands to f(&E1, &E2, &E3)
 * 2. f(n, ++args...); // expands to f(n, ++E1, ++E2, ++E3);
 * 3. f(++args..., n); // expands to f(++E1, ++E2, ++E3, n);
 * 4. f(h(args...) + args...); // expands to f(h(E1,E2,E3) + E1, h(E1,E2,E3) + E2, h(E1,E2,E3) + E3)
 */

// sizeof operator
template<class... Types>
struct count
{
    static const std::size_t value = sizeof...(Types);
};

// Initializer list of array
template<typename... Ts>
void
func(Ts... args)
{
    const int size = sizeof...(args) + 2;
    int res[size] = {1, args..., 2};
    for (int i = 0; i < size; ++i) {
        std::cout << res[i] << " ";
    }
    std::cout << std::endl;
}

// Member initializer list
template<class... Mixins>
class X : public Mixins...
{
 public:
    X() = default;
    X(const Mixins&... mixins) : Mixins(mixins)... { }
};

int main(void)
{
    dog a, b, c;
    cat d, e, f;
    speak(a, d, b, e, c, f);
    speak(d, a, b, e, f, c);
    speak_dog_and_cat(a, d, b, e, c, f);
    // speak_dog_and_cat(d, a, b, e, f, c); // compile error!

    auto result = sum(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    std::cout << "result: " << result << std::endl;

    // sizeof operator
    count <int, char> cnt;
    std::cout << cnt.value << std::endl;

    // Initializer list of array
    func(2, 3, 4, 5, 6, 7);

    // Member initializer list
    X <dog, cat> x(a, d);

    return 0;
}
