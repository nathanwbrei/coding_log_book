#include <iostream>

class EventProc {
public:
    typedef void (EventProc::*ReduceFn)();

    virtual void reduce() {
        std::cout << "Calling base reduce" << std::endl;
    };

    void do_reduce();
};

class EmptyEventProc : public EventProc {
};

void EventProc::do_reduce() {
    ReduceFn virtual_reduce = &EventProc::reduce;
    ReduceFn base_reduce = &EmptyEventProc::reduce;
    if (virtual_reduce == base_reduce) {
        std::cout << "Reduce not overridden! Skipping." << std::endl;
    }
    else {
        (this->*virtual_reduce)();
    }
}

class MyEventProc : public EventProc {
    void reduce() override {
        std::cout << "Calling derived reduce" << std::endl;
    };
};

class YourEventProc : public EventProc {
    // No derived reduce
};

int main() {
    MyEventProc p1;
    YourEventProc p2;

    p1.do_reduce();
    p2.do_reduce();

    return 0;
}
