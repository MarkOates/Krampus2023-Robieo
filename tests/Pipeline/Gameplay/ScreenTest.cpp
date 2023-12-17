
#include <gtest/gtest.h>

#include <Pipeline/Gameplay/Screen.hpp>

#include <AllegroFlare/Testing/WithAllegroRenderingFixture.hpp>
#include <AllegroFlare/Testing/WithAllegroFlareFrameworksFullFixture.hpp>
//#include <AllegroFlare/Frameworks/Full.hpp>
#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <AllegroFlare/DialogTree/YAMLLoader.hpp>


class TestGameConfiguration : public AllegroFlare::GameConfigurations::Base
{
public:
   // TODO: Upgrade this out of void* to a level base class (once the upstream design is clear)
   virtual AllegroFlare::Levels::Base *load_level_by_identifier(std::string identifier) override
   {
      // NOTE: Not used in this test
      //RPG::Gameplay::Level *result = new RPG::Gameplay::Level();
      //if (identifier == "test_level")
      //{
         //result->set_background_image_identifier("maps/rpg-fit-backgrounds-x2-01.png");
      //}
      //return result;
      return nullptr;
   }
};


class Pipeline_Gameplay_ScreenTest : public ::testing::Test {};
class Pipeline_Gameplay_ScreenTestWithAllegroRenderingFixture
   : public AllegroFlare::Testing::WithAllegroRenderingFixture
{};
class Pipeline_Gameplay_ScreenTestWithAllegroFrameworksFullFixture
   : public AllegroFlare::Testing::WithAllegroFlareFrameworksFullFixture
{};



TEST_F(Pipeline_Gameplay_ScreenTest, can_be_created_without_blowing_up)
{
   Pipeline::Gameplay::Screen screen;
}


TEST_F(Pipeline_Gameplay_ScreenTest, TYPE__has_the_expected_value)
{
   Pipeline::Gameplay::Screen screen;
   EXPECT_EQ("Pipeline/Gameplay/Screen", screen.get_type());
}


TEST_F(Pipeline_Gameplay_ScreenTest, type__has_the_expected_value_matching_TYPE)
{
   Pipeline::Gameplay::Screen screen;
   EXPECT_EQ(Pipeline::Gameplay::Screen::TYPE, screen.get_type());
}


TEST_F(Pipeline_Gameplay_ScreenTestWithAllegroFrameworksFullFixture,
   TIMED_INTERACTIVE__will_run_as_expected)
{
   TestGameConfiguration game_configuration;
   Pipeline::Gameplay::Screen screen;
   screen.set_event_emitter(get_framework_event_emitter());
   screen.set_bitmap_bin(get_framework_bitmap_bin());
   screen.set_font_bin(get_framework_font_bin());
   screen.set_model_bin(get_framework_model_bin());
   screen.set_game_configuration(&game_configuration),
   screen.initialize();


   // Load a dialog bank into the system
   AllegroFlare::DialogTree::NodeBank node_bank;
   std::string dialog_filename =
      "/Users/markoates/Repos/Pipeline/tests/fixtures/dialogs/branching_dialog_with_long_text.yml";
   AllegroFlare::DialogTree::YAMLLoader yaml_loader;
   yaml_loader.load_file(dialog_filename);
   node_bank = yaml_loader.get_node_bank();
   get_framework_ref().set_dialog_system_dialog_node_bank(node_bank);


   screen.load_level_by_identifier("world-1-01");

   framework_register_and_activate_screen("screen", &screen);

   framework_run_loop(0);
}


