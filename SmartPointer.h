#pragma once
#include <cassert>
#include <cstdint>

template <typename T> class UniquePointer
{
  public:
    UniquePointer(T *pointer) : pointer_(pointer)
    {
    }

    UniquePointer(const UniquePointer<T> &) = delete;

    UniquePointer(UniquePointer<T> &&uniquePtr)
    {
        pointer_ = uniquePtr.pointer_;
        uniquePtr.pointer_ = nullptr;
    }

    ~UniquePointer()
    {
        delete pointer_;
    }

    void operator=(UniquePointer<T> &&uniquePtr)
    {
        pointer_ = uniquePtr.pointer_;
        uniquePtr.pointer_ = nullptr;
    }

    void operator=(const UniquePointer<T> &) = delete;

    T &operator*()
    {
        return *pointer_;
    }

    T *operator->()
    {
        return pointer_;
    }

    bool operator==(std::nullptr_t) const
    {
        return pointer_ == nullptr;
    }

    T *release()
    {
        auto pointer = pointer_;
        pointer_ = nullptr;
        return pointer;
    }

    void reset(T *pointer = nullptr)
    {
        assert(pointer != pointer_);
        delete pointer_;
        pointer_ = pointer;
    }

    T *get() const
    {
        return pointer_;
    }

  private:
    T *pointer_ = nullptr;
};

class ControlBlock
{
  public:
    uint64_t counter_ = 1;
};

template <typename T> class SharedPointer
{
  public:
    SharedPointer(T *pointer) : pointer_(pointer)
    {
        if (pointer_ != nullptr)
        {
            controlBlock_ = new ControlBlock();
        }
    }

    SharedPointer(SharedPointer<T> &&sharedPtr)
    {
        pointer_ = sharedPtr.pointer_;
        sharedPtr.pointer_ = nullptr;
        controlBlock_ = sharedPtr.controlBlock_;
        sharedPtr.controlBlock_ = nullptr;
    }

    SharedPointer(const SharedPointer<T> &copyPointer)
    {
        pointer_ = copyPointer.pointer_;
        controlBlock_ = copyPointer.controlBlock_;
        if (pointer_ != nullptr)
            controlBlock_->counter_++;
    }

    ~SharedPointer()
    {
        if (unique())
        {
            delete pointer_;
            delete controlBlock_;
        }
        else if (controlBlock_ != nullptr)
            controlBlock_->counter_--;
    }

    void operator=(SharedPointer<T> &&sharedPtr)
    {
        if (unique())
        {
            delete pointer_;
            delete controlBlock_;
        }
        else if (controlBlock_ != nullptr)
            controlBlock_->counter_--;
        pointer_ = sharedPtr.pointer_;
        sharedPtr.pointer_ = nullptr;
        controlBlock_ = sharedPtr.controlBlock_;
        sharedPtr.controlBlock_ = nullptr;
    }

    void operator=(const SharedPointer<T> &copyPointer)
    {
        if (unique())
        {
            delete pointer_;
            delete controlBlock_;
        }
        else if (controlBlock_ != nullptr)
            controlBlock_->counter_--;
        pointer_ = copyPointer.pointer_;
        controlBlock_ = copyPointer.controlBlock_;
        if (pointer_ != nullptr)
            controlBlock_->counter_++;
    }

    T &operator*()
    {
        return *pointer_;
    }

    T *operator->()
    {
        return pointer_;
    }

    bool operator==(const SharedPointer<T> &comparePtr) const
    {
        return pointer_ == comparePtr.get();
    }

    void reset(T *pointer = nullptr)
    {
        if (unique())
        {
            delete pointer_;
            controlBlock_->counter_--;
        }
        else
        {
            if (controlBlock_ != nullptr)
                controlBlock_->counter_--;
            controlBlock_ = new ControlBlock();
        }
        pointer_ = pointer;
    }

    bool unique() const
    {
        if (controlBlock_ != nullptr)
            return controlBlock_->counter_ == 1;
        return false;
    }

    T *get() const
    {
        return pointer_;
    }

    uint64_t use_count() const
    {
        if (controlBlock_ != nullptr)
            return controlBlock_->counter_;
        return 0;
    }

  private:
    ControlBlock *controlBlock_ = nullptr;
    T *pointer_ = nullptr;
};
