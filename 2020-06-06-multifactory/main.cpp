#include <iostream>

/// Base case
template <typename... Ts>
struct Multifactory {
	void print() {
		std::cout << std::endl;
	};
	void set_all() {};
};

/// Specialization for non-empty template parameters
template <typename T, typename... Ts>
struct Multifactory<T, Ts...> : public Multifactory<Ts...> {
	T data;

	explicit Multifactory(T t, Ts... ts) : Multifactory<Ts...>(ts...), data(t) {}

	void print() {
		std::cout << data << ", ";
		Multifactory<Ts...>::print();
	}

	void set_all(T t, Ts... ts) {
		data = t;
		Multifactory<Ts...>::set_all(ts...);
	}

};

int main() {
	std::cout << "Hello world from multifactory" << std::endl;
	Multifactory<int, double, char> f (7, 9.0, 'c');

	f.print();
	f.set_all(8, 10.0, 'd');
	f.print();
}


