#include "SmartPointer.h"
#include "gtest/gtest.h"

TEST(UniquePtrTests, ShouldReturnPointerValueWhenDereferenceOperatorIsCalled)
{
    UniquePointer<int> uniquePointer(new int(5));
    int value = *(uniquePointer);
    EXPECT_EQ(value, 5);
}

TEST(UniquePtrTests, ShouldReturnPointerWhenReleaseIsCalled)
{
    UniquePointer<int> uniquePointer(new int(5));
    auto pointer = uniquePointer.release();
    EXPECT_EQ(*(pointer), 5);
    delete pointer;
}

TEST(UniquePtrTests, ShouldReturnNullptrWhenResetIsCalled)
{
    UniquePointer<int> uniquePointer(new int(5));
    uniquePointer.reset();
    auto pointer = uniquePointer.get();
    EXPECT_EQ(pointer, nullptr);
}

TEST(UniquePtrTests, MoveConstructorShouldChangeOwnership)
{
    UniquePointer<int> uniquePointer(new int(5));
    auto rawPtr = uniquePointer.get();
    UniquePointer<int> up2 = std::move(uniquePointer);
    auto rawPtr2 = up2.get();

    EXPECT_EQ(rawPtr, rawPtr2);
}

TEST(UniquePtrTests, MoveAssignmentConstructorShouldChangeOwnership)
{
    UniquePointer<int> uniquePointer(new int(5));
    auto rawPtr = uniquePointer.get();
    UniquePointer<int> up2(nullptr);
    up2 = std::move(uniquePointer);
    auto rawPtr2 = up2.get();

    EXPECT_EQ(rawPtr, rawPtr2);
}

TEST(SharedPtrTests, SharedPointersShouldReturnTheSameValueWhenDereferenceOperatorCalled)
{
    SharedPointer<int> sharedPointer1(new int(5));
    SharedPointer<int> sharedPointer2(sharedPointer1);
    EXPECT_EQ(*(sharedPointer1), *(sharedPointer2));
    EXPECT_EQ(sharedPointer1, sharedPointer2);
}

TEST(SharedPtrTests, CallingResetOnSharePointer1ShouldNotAffectSharedPointer2)
{
    SharedPointer<int> sharedPointer1(new int(5));
    SharedPointer<int> sharedPointer2(sharedPointer1);
    sharedPointer1.reset();
    EXPECT_EQ(sharedPointer1.get(), nullptr);
    EXPECT_EQ(*sharedPointer2, 5);
}

TEST(SharedPtrTests, WhenUniqueCalledShouldReturnTrueAndWhenANewSharedPointerIsCreatedShouldReturnFalse)
{
    SharedPointer<int> sharedPointer1(new int(5));
    bool isUnique1 = sharedPointer1.unique();
    SharedPointer<int> sharedPointer2(sharedPointer1);
    bool isUnique2 = sharedPointer1.unique();
    EXPECT_EQ(isUnique1, true);
    EXPECT_EQ(isUnique2, false);
}

TEST(SharedPtrTests, VerifySharedPointerReferenceCountAfterCopyConstruction)
{
    SharedPointer<int> sharedPointer1(new int(5));
    SharedPointer<int> sharedPointer2(sharedPointer1);
    EXPECT_EQ(sharedPointer1.use_count(), 2);
}

TEST(SharedPtrTests, VerifySharedPointerReferenceCountAfterCopyConstruction2)
{
    SharedPointer<int> sharedPointer1(new int(5));
    {
        EXPECT_EQ(sharedPointer1.use_count(), 1);
        SharedPointer<int> sharedPointer2(sharedPointer1);
        EXPECT_EQ(sharedPointer1.use_count(), 2);
    }
    EXPECT_EQ(sharedPointer1.use_count(), 1);
    sharedPointer1.reset();
    EXPECT_EQ(sharedPointer1.use_count(), 0);
}

TEST(SharedPtrTests, VerifySharedPointerReferenceCountAfterCopyConstruction3)
{
    SharedPointer<int> sharedPointer1(nullptr);
    SharedPointer<int> sharedPointer2(sharedPointer1);

    EXPECT_EQ(sharedPointer1.use_count(), 0);
    EXPECT_EQ(sharedPointer2.use_count(), 0);
}

TEST(SharedPtrTests, MoveConstructorShouldChangeOwnership)
{
    SharedPointer<int> sharedPointer(new int(5));
    auto rawPtr = sharedPointer.get();
    SharedPointer<int> up2 = std::move(sharedPointer);
    auto rawPtr2 = up2.get();

    EXPECT_EQ(rawPtr, rawPtr2);
}

TEST(SharedPtrTests, MoveAssignmentConstructorShouldChangeOwnership)
{
    SharedPointer<int> sharedPointer(new int(5));
    auto rawPtr = sharedPointer.get();
    SharedPointer<int> up2(nullptr);
    up2 = std::move(sharedPointer);
    auto rawPtr2 = up2.get();

    EXPECT_EQ(rawPtr, rawPtr2);
}

TEST(SharedPtrTests, MoveAssignmentConstructorShouldChangeOwnership2)
{
    SharedPointer<int> sp1(new int(5));
    SharedPointer<int> sp2(sp1);

    SharedPointer<int> up2(new int(4)); // tutaj mamy memory leak
    up2 = std::move(sp2);

    EXPECT_EQ(sp1.use_count(), 2);
}
