#include <gtest/gtest.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/ast.h"
#include <memory>

// Helper function to parse an input string and return the AST
ASTPtr parseInput(const std::string& input) {
    Lexer lexer(input);
    Parser parser(lexer);
    return parser.parse();
}

TEST(ParserTest, ParsesVariableAssignment) {
    std::string input = "a = 5;";
    ASTPtr tree = parseInput(input);

    // Expecting a Compound node with one child (Assign)
    Compound* compound = dynamic_cast<Compound*>(tree.get());
    ASSERT_NE(compound, nullptr) << "Root node is not a Compound node";

    ASSERT_EQ(compound->children.size(), 1) << "Compound node does not have exactly one child";

    Assign* assign = dynamic_cast<Assign*>(compound->children[0].get());
    ASSERT_NE(assign, nullptr) << "Child node is not an Assign node";
    EXPECT_EQ(assign->op.type, TokenType::ASSIGN) << "Operator is not ASSIGN";

    // Cast left to Var and check value
    Var* var = dynamic_cast<Var*>(assign->left.get());
    ASSERT_NE(var, nullptr) << "Left side of assignment is not a Var node";
    EXPECT_EQ(var->value, "a") << "Variable name is not 'a'";

    // Cast right to Num and check value
    Num* num = dynamic_cast<Num*>(assign->right.get());
    ASSERT_NE(num, nullptr) << "Right side of assignment is not a Num node";
    EXPECT_DOUBLE_EQ(num->value, 5.0) << "Assigned value is not 5.0";
}

TEST(ParserTest, ParsesCompoundStatements) {
    std::string input = "a = 5; b = a * 2; c = b + 3;";
    ASTPtr tree = parseInput(input);

    Compound* compound = dynamic_cast<Compound*>(tree.get());
    ASSERT_NE(compound, nullptr);
    EXPECT_EQ(compound->children.size(), 3);

    // First assignment: a = 5
    Assign* assign1 = dynamic_cast<Assign*>(compound->children[0].get());
    ASSERT_NE(assign1, nullptr);
    EXPECT_EQ(assign1->op.type, TokenType::ASSIGN);

    Var* var1 = dynamic_cast<Var*>(assign1->left.get());
    ASSERT_NE(var1, nullptr);
    EXPECT_EQ(var1->value, "a");

    Num* num1 = dynamic_cast<Num*>(assign1->right.get());
    ASSERT_NE(num1, nullptr);
    EXPECT_DOUBLE_EQ(num1->value, 5.0);

    // Second assignment: b = a * 2
    Assign* assign2 = dynamic_cast<Assign*>(compound->children[1].get());
    ASSERT_NE(assign2, nullptr);
    EXPECT_EQ(assign2->op.type, TokenType::ASSIGN);

    Var* var2 = dynamic_cast<Var*>(assign2->left.get());
    ASSERT_NE(var2, nullptr);
    EXPECT_EQ(var2->value, "b");

    BinOp* binOp = dynamic_cast<BinOp*>(assign2->right.get());
    ASSERT_NE(binOp, nullptr);
    EXPECT_EQ(binOp->op.type, TokenType::MULTIPLY);

    Var* varOpLeft = dynamic_cast<Var*>(binOp->left.get());
    ASSERT_NE(varOpLeft, nullptr);
    EXPECT_EQ(varOpLeft->value, "a");

    Num* numOpRight = dynamic_cast<Num*>(binOp->right.get());
    ASSERT_NE(numOpRight, nullptr);
    EXPECT_DOUBLE_EQ(numOpRight->value, 2.0);

    // Third assignment: c = b + 3
    Assign* assign3 = dynamic_cast<Assign*>(compound->children[2].get());
    ASSERT_NE(assign3, nullptr);
    EXPECT_EQ(assign3->op.type, TokenType::ASSIGN);

    Var* var3 = dynamic_cast<Var*>(assign3->left.get());
    ASSERT_NE(var3, nullptr);
    EXPECT_EQ(var3->value, "c");

    BinOp* binOp3 = dynamic_cast<BinOp*>(assign3->right.get());
    ASSERT_NE(binOp3, nullptr);
    EXPECT_EQ(binOp3->op.type, TokenType::PLUS);

    Var* varOpLeft3 = dynamic_cast<Var*>(binOp3->left.get());
    ASSERT_NE(varOpLeft3, nullptr);
    EXPECT_EQ(varOpLeft3->value, "b");

    Num* numOpRight3 = dynamic_cast<Num*>(binOp3->right.get());
    ASSERT_NE(numOpRight3, nullptr);
    EXPECT_DOUBLE_EQ(numOpRight3->value, 3.0);
}
