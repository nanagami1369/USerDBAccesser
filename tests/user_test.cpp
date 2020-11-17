#include "User.h"
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>

TEST(UserTest, toString) {
    auto user = new User(1, "山田 太郎", "12345678", true, ADMIN);
    ASSERT_EQ("1|山田 太郎|********|true|ADMIN", user->toString());
}
