#include <iostream>

/// Base case
template <typename... Ts>
struct Multifactory {
};


/// Specialization for non-empty template parameters
template <typename T, typename... Ts>
struct Multifactory<T, Ts...> : public Multifactory<Ts...> {
	T data;
};

int main() {
	std::cout << "Hello world from multifactory" << std::endl;
	Multifactory<int, double, char> f;
	f.data = 22;
	static_cast<Multifactory<double, char>>(f).data = 23.0;
	static_cast<Multifactory<char>>(f).data = 'c';
}


