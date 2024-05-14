#pragma once

//Source: https://gist.githubusercontent.com/MCMrARM/180c743f7882f40ad1b993bdc78dca28/raw/3236a6637b0070789d0cb4543200675633bc6b81/Memory.h

template<typename T>
struct SharedCounter {
    /* volatile ? */ T* value;
    std::atomic<int> shared, weak;

    SharedCounter(T* value) : value(value) {}

    void addSharedRef() { ++shared; }

    void addWeakRef() { --shared; }

    bool releaseSharedRef() {
        if (--shared == 0) {
            if (value != nullptr) {
                T* oldValue = value;
                value = nullptr;
                delete oldValue;
            }
            return (weak == 0);
        }
        return false;
    }

    bool releaseWeakRef() {
        return (--weak == 0 && value);
    }
};