#ifndef __SHARED_OBJECT_H__
#define __SHARED_OBJECT_H__

namespace shared_object {

    class object {
    public:
        virtual ~object() = default;
        virtual void hello() = 0;
    };

}

#endif