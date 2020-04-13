#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <any>

struct BaseData {
    double d;
    BaseData(double d) : d(d) {};
};

struct DerivedData : public BaseData {
    int i;
    DerivedData(double d, int i): BaseData(d), i(i) {};
};

struct UnrelatedData {
    float f;
};

class JFactory;


template <typename T> class JFactoryT;

struct JFactory {
    std::unordered_map<std::type_index, std::any> vtable;

    template<typename S>
    const std::vector<S*> get_as() {
        std::vector<S*> results;
        auto ti = std::type_index(typeid(S));
        auto search = vtable.find(ti);
        if (search != vtable.end()) {
            std::cout << "Found vtable entry!!!" << std::endl;
            std::any upcast = search->second;
            auto upcast_fn = std::any_cast<std::function<std::vector<S*>()>> (upcast);
            results = upcast_fn();
        }
        return results;
    }
};


template <typename T> struct JFactoryT : public JFactory {

    std::vector<T*> data;

    void insert(T* item) {
        data.push_back(item);
    }

    const std::vector<T*>& get() const {
        return data;
    }

    // Consider:
    // - Allow user to provide their own lambdas?
    // - Does static cast always work? When does it and when doesn't it?
    // - Do we want to try sharing vtables? Right now it is one vtable per instance
    template <typename S>
    void allow_cast_to() {

        auto upcast_lambda = [this]() {
            std::vector<S*> results;
            for (auto t : data) {
                results.push_back(static_cast<S*>(t));
            }
            return results;
        };

        auto upcast_fn = std::function<std::vector<S*>()> (upcast_lambda);
        auto key = std::type_index(typeid(S));
        vtable[key] = std::any(upcast_fn);
        std::cout << "Adding conversion to " << typeid(S).name() << std::endl;
    }
};


int main() {
    std::cout << "Hello, World!" << std::endl;

    JFactoryT<DerivedData> fdd;
    fdd.insert(new DerivedData {7.0, 2});
    fdd.allow_cast_to<DerivedData>();
    fdd.allow_cast_to<BaseData>();


    fdd.get_as<BaseData>();


    //
    // JFactoryT<UnrelatedData> fud;
    // fud.insert(new UnrelatedData {9.0});
    //
    //
    // JFactory* factory = static_cast<JFactory*>(&fdd);
    //
    // auto output = factory->get_as<BaseData>();
    // for (auto item : output) {
    //     std::cout << item->d << std::endl;
    // }
    //
    // auto f = [] (int x) {return x + 1; };
    // std::cout << f(22) << std::endl;
    // std::any f_erased (f);
    // auto g = std::any_cast<int(int)>(f_erased);
    // return 0;
}
