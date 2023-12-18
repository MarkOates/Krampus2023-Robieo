

#include <Pipeline/DialogNodeBankFactory.hpp>

#include <AllegroFlare/DialogTree/NodeOptions/ExitDialog.hpp>
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
   node_bank.add_node(
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
                  //new AllegroFlare::DialogTree::NodeOptions::GotoNode("System::INTERNAL::wait_before_exit_program"),
                  new AllegroFlare::DialogTree::NodeOptions::ExitDialog(),
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
      );

   return node_bank;
}


} // namespace Pipeline


