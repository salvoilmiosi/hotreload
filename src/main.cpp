#include <iostream>
#include <filesystem>
#include <memory>

#include <Windows.h>
#include <Shlwapi.h>

#include "shared_object/shared_object.h"

#ifdef WIN32
    #define STDCALL __stdcall
#else
    #define STDCALL
#endif

typedef shared_object::object * (STDCALL *new_shared_object_fun_t)();
typedef void (STDCALL *delete_shared_object_fun_t)(shared_object::object *);

struct shared_object_deleter {
    delete_shared_object_fun_t fn;
    void operator()(shared_object::object *ptr) {
        fn(ptr);
    }
};

using loaded_shared_object = std::unique_ptr<shared_object::object, shared_object_deleter>;

loaded_shared_object load_shared_object(const std::filesystem::path &filename) {
    if (!PathFileExistsW(filename.c_str())) return nullptr;
    HINSTANCE dll = LoadLibraryW(filename.c_str());
    if (!dll) return nullptr;

    auto new_shared_object_fun = (new_shared_object_fun_t) GetProcAddress(dll, "new_shared_object");
    auto delete_shared_object_fun = (delete_shared_object_fun_t) GetProcAddress(dll, "delete_shared_object");

    if (!new_shared_object_fun || !delete_shared_object_fun) return nullptr;
    return loaded_shared_object(new_shared_object_fun(), shared_object_deleter{delete_shared_object_fun});
}

int main(int argc, char **argv) {
    auto object = load_shared_object(std::filesystem::path(argv[0]).parent_path() / "lib/libshared_object.dll");
    if (!object) {
        std::cerr << "Could not load shared_object\n";
        return 1;
    }

    object->hello();
    return 0;
}