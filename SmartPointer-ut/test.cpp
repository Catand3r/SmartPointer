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
