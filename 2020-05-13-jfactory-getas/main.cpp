#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <functional>

struct BaseData {
    double d;
    BaseData(double d) : d(d) {};
};

struct DerivedData : public BaseData {
    int i;
    DerivedData(double d, int i): BaseData(d), i(i) {};
};

struct DifferentBaseData {
    double dbd;
    DifferentBaseData(double d) : dbd(d) {};
};

struct MultipleDerivedData : public BaseData, public DifferentBaseData {
    double mdd;
    MultipleDerivedData(double bd, double dbd, double mdd) : BaseData(bd), DifferentBaseData(dbd), mdd(mdd) {};
};

struct UnrelatedData {
    float f;
};

class JFactory;


template <typename T> class JFactoryT;

struct JFactory {
    std::unordered_map<std::type_index, void*> vtable;

    template<typename S>
    std::vector<S*> get_as() {
        std::vector<S*> results;
        auto ti = std::type_index(typeid(S));
        auto search = vtable.find(ti);
        if (search != vtable.end()) {
            std::cout << "Found conversion to " << typeid(S).name() << std::endl;
            auto upcast_fn = reinterpret_cast<std::function<std::vector<S*>()>*>(search->second);
            results = (*upcast_fn)();
        }
        return results;
    }
};


template <typename T> struct JFactoryT : public JFactory {

    std::vector<T*> data;

    void insert(T* item) {
        data.push_back(item);
    }

    JFactoryT() {
        allow_cast_to<T>();
    }


    std::vector<T*>& get() const {
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

        auto upcast_fn = new std::function<std::vector<S*>()> (upcast_lambda);
        auto key = std::type_index(typeid(S));
        vtable[key] = upcast_fn;
        std::cout << "Adding conversion to " << typeid(S).name() << std::endl;
    }
};


int main() {
    std::cout << "Hello, World!" << std::endl;

    auto fdd = new JFactoryT<DerivedData>;
    fdd->insert(new DerivedData {7.0, 2});
    fdd->allow_cast_to<BaseData>();

    std::cout << "From DerivedData" << std::endl;
    auto bs = fdd->get_as<BaseData>();
    for (auto x: bs) {
        std::cout << "Got " << x->d << std::endl;
    }

    std::cout << "From UnrelatedData" << std::endl;
    auto fud = new JFactoryT<UnrelatedData>;
    fud->insert(new UnrelatedData {9.0});
    JFactory* f = static_cast<JFactory*>(fud);

    bs = f->get_as<BaseData>();
    for (auto x: bs) {
        std::cout << "Got " << x->d << std::endl;
    }

    std::cout << "Get DerivedData as DerivedData" << std::endl;
    auto dds = fdd->get_as<DerivedData>();
    for (auto x: dds) {
        std::cout << "Got " << x->d << ", " << x->i << std::endl;
    }

    // Test multiple (non-virtual) inheritance
    std::cout << "Attempting multiple inheritance" << std::endl;
    auto fmd = new JFactoryT<MultipleDerivedData>;
    fmd->allow_cast_to<BaseData>();
    fmd->allow_cast_to<DifferentBaseData>();
    fmd->insert(new MultipleDerivedData(1,2,3));

    for (auto x: fmd->get_as<MultipleDerivedData>()) {
        std::cout << "Got mdd = " << x->d << ", " << x->dbd << ", " << x->mdd << std::endl;
    }
    for (auto x: fmd->get_as<BaseData>()) {
        std::cout << "Got basedata = " << x->d << std::endl;
    }
    for (auto x: fmd->get_as<DifferentBaseData>()) {
        std::cout << "Got differentbasedata = " << x->dbd << std::endl;
    }

}
