

#include <Pipeline/DialogNodeBankFactory.hpp>

#include <AllegroFlare/DialogTree/NodeOptions/ExitDialog.hpp>
#include <AllegroFlare/DialogTree/NodeOptions/GoToNode.hpp>
#include <AllegroFlare/DialogTree/Nodes/EmitGameEvent.hpp>
#include <AllegroFlare/DialogTree/Nodes/ExitDialog.hpp>
#include <AllegroFlare/DialogTree/Nodes/ExitProgram.hpp>
#include <AllegroFlare/DialogTree/Nodes/MultipageWithOptions.hpp>


namespace Pipeline
{


DialogNodeBankFactory::DialogNodeBankFactory()
{
}


DialogNodeBankFactory::~DialogNodeBankFactory()
{
}


AllegroFlare::DialogTree::NodeBank DialogNodeBankFactory::build_production_game_node_bank()
{
   AllegroFlare::DialogTree::NodeBank node_bank;
   //std::string dialog_filename = framework->get_data_folder_path() + "/dialogs/all_dialog.yml";
   //AllegroFlare::DialogTree::YAMLLoader yaml_loader;
   //yaml_loader.load_file(dialog_filename);
   //node_bank = yaml_loader.get_node_bank();
   node_bank.set_nodes({
      {
         "package_delivery_response",
         new AllegroFlare::DialogTree::Nodes::MultipageWithOptions
         (
            "", // Blank speaker
            //{ "Are you sure you want to exit?" },
            { // Dialog pages
               "Amazing! Thank you so much for delivering my package.",
               "I can't wait to find out what's inside...",
            },
            { 
               {
                  "Continue",
                  new AllegroFlare::DialogTree::NodeOptions::GoToNode("robot_prompts_performance"),
                  //new AllegroFlare::DialogTree::NodeOptions::ExitDialog(),
                  AllegroFlare::BitFlags<uint32_t>(0)
               },
               //{
                  //"No",
                  //new AllegroFlare::DialogTree::NodeOptions::ExitDialog(),
                  //// TODO: Add test to ensure this option is focused
                  //AllegroFlare::BitFlags<uint32_t>(
                     //AllegroFlare::DialogTree::Nodes::MultipageWithOptions::OptionFlags::FLAG_CURSOR_HERE_ON_SPAWN
                  //)
               //},
            }
         )
      },
      {
         "robot_prompts_performance",
         new AllegroFlare::DialogTree::Nodes::MultipageWithOptions
         (
            "Robieo", // Blank speaker
            //{ "Are you sure you want to exit?" },
            { // Dialog pages
               "It is... my pleasure... to deliver.",
               "As part of my protocol, I will now sing for you..."
               "... a christmas carol."
            },
            { 
               {
                  "Continue",
                  new AllegroFlare::DialogTree::NodeOptions::GoToNode("trigger_robot_perform_music"),
                  //new AllegroFlare::DialogTree::NodeOptions::EmitGameEvent("perform_music", "exit_dialog"),
                  AllegroFlare::BitFlags<uint32_t>(0)
               },
               //{
                  //"No",
                  //new AllegroFlare::DialogTree::NodeOptions::ExitDialog(),
                  //// TODO: Add test to ensure this option is focused
                  //AllegroFlare::BitFlags<uint32_t>(
                     //AllegroFlare::DialogTree::Nodes::MultipageWithOptions::OptionFlags::FLAG_CURSOR_HERE_ON_SPAWN
                  //)
               //},
            }
         )
      },
      {
         "trigger_robot_perform_music",
         new AllegroFlare::DialogTree::Nodes::EmitGameEvent("perform_music", "exit_dialog")
      },
      {
         "exit_dialog",
         new AllegroFlare::DialogTree::Nodes::ExitDialog()
      }
   });

   return node_bank;
}


} // namespace Pipeline


