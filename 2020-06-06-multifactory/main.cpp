#include <iostream>
#include <vector>


template <typename... Ts>
struct Multifactory {

	std::tuple<std::vector<Ts*>...> data;

	void set(std::vector<Ts*>... ts) {
		data = std::make_tuple(ts...);
	}

	template<typename T>
	std::vector<T*>& get() {
		return std::get<std::vector<T*>>(data);
	}

	template <int level = 0>
	void print() {
		auto x = std::get<level>(data);
		for (int i=0; i<x.size(); ++i) {
			std::cout << *(x[i]) << ", ";
		}
		std::cout << "; ";
		print<level+1>();
	}

	template <>
	void print<sizeof...(Ts)>() {
		std::cout << std::endl;
	}
};


int main() {
	std::cout << "Hello world from multifactory" << std::endl;
	Multifactory<int, double, char> f;

	f.print();
	std::vector<int*> xs = {new int(22), new int(1)};
	std::vector<double*> ys = {new double(19.9), new double(1.0)};
	std::vector<char*> zs = {new char('x'), new char('y')};
	f.set(xs, ys, zs);
	f.print();

	auto xss = f.get<int>();
	auto zss = f.get<char>();

	std::cout << *xss[0] << ", " << *xss[1] << "; " << *zss[0] << ", " << *zss[1] << std::endl;

}


