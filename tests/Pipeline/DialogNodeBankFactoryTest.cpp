
#include <gtest/gtest.h>

#include <Pipeline/DialogNodeBankFactory.hpp>


TEST(Pipeline_DialogNodeBankFactoryTest, can_be_created_without_blowing_up)
{
   Pipeline::DialogNodeBankFactory dialog_node_bank_factory;
}


//TEST(Pipeline_DialogNodeBankFactoryTest, run__returns_the_expected_response)
//{
   //Pipeline::DialogNodeBankFactory dialog_node_bank_factory;
   //std::string expected_string = "Hello World!";
   //EXPECT_EQ(expected_string, dialog_node_bank_factory.run());
//}


