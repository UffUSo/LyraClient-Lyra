#pragma once

#include "SharedCounter.hpp"

//Sourced from LiteBDS
//https://cpp.docs.litebds.com/api/d5/d34/Types_8hpp_source.html

//Now sourced from MrArm
//https://gist.githubusercontent.com/MCMrARM/180c743f7882f40ad1b993bdc78dca28/raw/3236a6637b0070789d0cb4543200675633bc6b81/Memory.h

//Using liteloader's again cuz I dont want an impl that actually accounts for freeing and stuff, this should be READ ONLY!!

template <typename T>
class WeakPtr {
    T** value;

public:
    inline T* get() {
        if (value)
            return *value;
        return nullptr;
    }
    inline T const* get() const {
        if (value)
            return *value;
        return nullptr;
    }
    inline T& operator*() {
        return *get();
    }
    inline T const& operator*() const {
        return *get();
    }
    inline operator bool() const {
        return get() != nullptr;
    }
};