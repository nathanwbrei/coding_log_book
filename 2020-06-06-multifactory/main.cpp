#include <iostream>

/// Base case
template <typename... Ts>
struct Multifactory {
};

/// Specialization for non-empty template parameters
template <typename T, typename... Ts>
struct Multifactory<T, Ts...> : public Multifactory<Ts...> {
	T data;

	explicit Multifactory(T t, Ts... ts) : Multifactory<Ts...>(ts...), data(t) {}
};

int main() {
	std::cout << "Hello world from multifactory" << std::endl;
	Multifactory<int, double, char> f (7, 9.0, 'c');

}


