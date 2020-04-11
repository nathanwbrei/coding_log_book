#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>
#include <typeindex>

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

template <typename T> class JFactoryT;


struct JFactoryVisitor {

    std::vector<BaseData*> bases;

    template <typename T> void visit(JFactoryT<T>* t) {

        std::cout << "Visiting " << typeid(T).name() << std::endl;
        if constexpr (std::is_base_of<BaseData, T>::value) {
            auto inserts = t->template get_as<BaseData>();
            for (auto x : inserts) {
                bases.push_back(x);
            }
        }
    }
};

struct JFactory {

    virtual void accept(JFactoryVisitor& visitor) = 0;
};


template <typename T> struct JFactoryT : public JFactory {
    std::vector<T*> data;


    void accept(JFactoryVisitor& visitor) override {
        visitor.visit(this);
    }

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
};


int main() {
    std::cout << "Hello, World!" << std::endl;

    JFactoryT<DerivedData> fdd;
    fdd.insert(new DerivedData {7.0, 2});

    JFactoryT<UnrelatedData> fud;
    fud.insert(new UnrelatedData {9.0});


    JFactory* factory = static_cast<JFactory*>(&fdd);

    auto output = factory.get_as<BaseData>();
    for (auto item : output) {
        std::cout << item->d << std::endl;
    }
    return 0;
}
