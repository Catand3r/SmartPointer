#pragma once

template <typename T> class SmartPointer
{
  public:
    SmartPointer(T *pointer) : pointer_(pointer)
    {
    }

    SmartPointer(const SmartPointer &) = delete;

    ~SmartPointer()
    {
        delete pointer_;
    }

    T &operator*()
    {
        return *pointer_;
    }

    T *operator->()
    {
        return pointer_;
    }

    T *release()
    {
        auto pointer = pointer_;
        pointer_ = nullptr;
        return pointer;
    }

    void reset(T *pointer = nullptr)
    {
        delete pointer_;
        pointer_ = pointer;
    }

  private:
    T *pointer_;
};
