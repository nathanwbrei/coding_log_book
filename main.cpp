#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
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
    std::unordered_map<std::type_index, const void*> vtable;

    template<typename S>
    const std::vector<S*> get_as() {
        std::vector<S*> results;
        auto ti = std::type_index(typeid(S));
        auto search = vtable.find(ti);
        if (search != vtable.end()) {
            const void* lambda = search->second;
            //std::any_cast();
            // Need to cast a raw (untyped) fn pointer back to a method
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

    template <typename S>
    const std::vector<S*> get_as() const {
        std::vector<S*> results;
        for (auto t : data) {
            results.push_back(static_cast<S*>(t));
        }
        return results;
    };

    template <typename S>
    void allow_cast_to() {
        // https://isocpp.org/wiki/faq/pointers-to-members#fnptr-vs-memfnptr-types
        std::cout << "Adding conversion to " << typeid(S).name() << std::endl;

        typedef const std::vector<S*> (JFactoryT<T>::*CastToSFn)() const;

        CastToSFn fn = &JFactoryT<T>::get_as<S>;
        auto key = std::type_index(typeid(S));
        vtable[key] = reinterpret_cast<void*>(fn);
    }
};


int main() {
    std::cout << "Hello, World!" << std::endl;

    JFactoryT<DerivedData> fdd;
    fdd.insert(new DerivedData {7.0, 2});
    fdd.allow_cast_to<DerivedData>();

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
