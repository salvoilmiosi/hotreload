#include <filesystem>
#include <utility>

#include <Windows.h>
#include <Shlwapi.h>

#include "shared_object/shared_object.h"

#ifdef WIN32
    #define STDCALL __stdcall
#else
    #define STDCALL
#endif

class loaded_shared_object {
public:
    loaded_shared_object(HINSTANCE hInstance)
        : m_ptr{assert_not_null((new_shared_object_fun_t) GetProcAddress(hInstance, "new_shared_object"))()}
        , m_delete_fun{assert_not_null((delete_shared_object_fun_t) GetProcAddress(hInstance, "delete_shared_object"))} {}
    
    ~loaded_shared_object() {
        if (m_ptr && m_delete_fun) {
            m_delete_fun(m_ptr);
        }
    }

    loaded_shared_object(const loaded_shared_object &) = delete;
    loaded_shared_object(loaded_shared_object &&other) noexcept
        : m_ptr{std::exchange(other.m_ptr, nullptr)}
        , m_delete_fun{std::exchange(other.m_delete_fun, nullptr)} {}
    
    loaded_shared_object &operator = (const loaded_shared_object &) = delete;
    loaded_shared_object &operator = (loaded_shared_object &&other) noexcept {
        std::swap(m_ptr, other.m_ptr);
        std::swap(m_delete_fun, other.m_delete_fun);
        return *this;
    }

public:
    shared_object::object &operator *() { return *m_ptr; }
    const shared_object::object &operator *() const { return *m_ptr; }

    shared_object::object *operator ->() { return m_ptr; }
    const shared_object::object *operator ->() const { return m_ptr; }

private:
    typedef shared_object::object * (STDCALL *new_shared_object_fun_t)();
    typedef void (STDCALL *delete_shared_object_fun_t)(shared_object::object *);

    template<typename T>
    static T *assert_not_null(T *ptr) {
        if (!ptr) {
            throw std::runtime_error("Could not load shared_object");
        }
        return ptr;
    }

    shared_object::object *m_ptr = nullptr;
    delete_shared_object_fun_t m_delete_fun = nullptr;
};

class loaded_dll {
public:
    loaded_dll(const std::filesystem::path &filename) {
        if (!PathFileExistsW(filename.c_str())) {
            throw std::runtime_error("Could not load dll");
        }
        hInstance = LoadLibraryW(filename.c_str());
    }

    ~loaded_dll() {
        if (hInstance) FreeLibrary(hInstance);
    }

    loaded_dll(const loaded_dll &) = delete;
    loaded_dll(loaded_dll &&other) noexcept
        : hInstance(std::exchange(other.hInstance, nullptr)) {}

    loaded_dll &operator = (const loaded_dll &) = delete;
    loaded_dll &operator = (loaded_dll &&other) noexcept {
        std::swap(hInstance, other.hInstance);
        return *this;
    }

    loaded_shared_object load_shared_object() {
        return loaded_shared_object(hInstance);
    }

private:
    HINSTANCE hInstance = nullptr;
};

int main(int argc, char **argv) {
    auto dll = loaded_dll{std::filesystem::path(argv[0]).parent_path() / "lib/libshared_object.dll"};
    auto object = dll.load_shared_object();
    object->hello();
    return 0;
}