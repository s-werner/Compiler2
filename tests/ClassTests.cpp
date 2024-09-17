#include <gtest/gtest.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/ast.h"
#include "TestUtils.h"

TEST(ClassTest, ParsesClassDefinition) {
    std::string input = R"(
        class MyClass {
            function greet() {
                return 42;
            }
        }
    )";
    ASTPtr tree = parseInput(input);

    Compound* compound = dynamic_cast<Compound*>(tree.get());
    ASSERT_NE(compound, nullptr);
    ASSERT_EQ(compound->children.size(), 1);

    ClassDef* classDef = dynamic_cast<ClassDef*>(compound->children[0].get());
    ASSERT_NE(classDef, nullptr);
    EXPECT_EQ(classDef->name, "MyClass");
    ASSERT_EQ(classDef->methods.size(), 1);

    FunctionDef* funcDef = dynamic_cast<FunctionDef*>(classDef->methods[0].get());
    ASSERT_NE(funcDef, nullptr);
    EXPECT_EQ(funcDef->name, "greet");
}
