#include "shared_object/shared_object.h"

#include "shared_object_export.h"

#include <iostream>

#ifdef WIN32
    #define STDCALL __stdcall
#else
    #define STDCALL
#endif

namespace shared_object {
    class object_impl : public object {
    public:
        void hello() override {
            std::cout << "Hello World\n";
        }
    };
};

extern "C" shared_object::object * STDCALL new_shared_object() {
    return new shared_object::object_impl();
}

extern "C" void STDCALL delete_shared_object(shared_object::object *ptr) {
    delete ptr;
}