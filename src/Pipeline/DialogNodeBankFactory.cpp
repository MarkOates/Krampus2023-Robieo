

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
               "As part of my protocol, I will now sing for you...",
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
      },



      {
         DIALOG_FOREST_NPC_LIKES_MUSHROOMS,
         new AllegroFlare::DialogTree::Nodes::MultipageWithOptions
         (
            "Forrest", // Blank speaker
            //{ "Are you sure you want to exit?" },
            { // Dialog pages
               "I really like the forest.",
               "The mushrooms around here are especially wonderful.",
               "If you can find 10 mushrooms and bring them to me, I would be amazed!",
            },
            { 
               {
                  "Exit",
                  new AllegroFlare::DialogTree::NodeOptions::ExitDialog(),
                  AllegroFlare::BitFlags<uint32_t>(0)
               },
            }
         )
      },
      {
         DIALOG_FOREST_NPC_COMPLETES_QUEST,
         new AllegroFlare::DialogTree::Nodes::MultipageWithOptions
         (
            "Forrest", // Blank speaker
            { // Dialog pages
               "WOW! You found a bunch of mushrooms!",
               "That's amazing!",
            },
            { 
               {
                  "Exit",
                  new AllegroFlare::DialogTree::NodeOptions::ExitDialog(),
                  AllegroFlare::BitFlags<uint32_t>(0)
               },
            }
         )
      },
      {
         DIALOG_FOREST_NPC_QUEST_ALREADY_COMPLETE,
         new AllegroFlare::DialogTree::Nodes::MultipageWithOptions
         (
            "Forrest", // Blank speaker
            { // Dialog pages
               "Mushrooms really are great, huh?",
            },
            { 
               {
                  "Exit",
                  new AllegroFlare::DialogTree::NodeOptions::ExitDialog(),
                  AllegroFlare::BitFlags<uint32_t>(0)
               },
            }
         )
      },



      {
         HOME_CREATOR_GIVES_YOU_INSTRUCTIONS,
         new AllegroFlare::DialogTree::Nodes::MultipageWithOptions
         (
            CHARACTER_DISPLAY_NAME_FOR_DR_LOUISE,
            //"Dr. Louise",
            { // Dialog pages
               "Today is your big day, Robieo!",
               "What a wonderful day to get started on your deliveries.",
               "As part of my delivery service, you're the best of the best, Robieo.",
               "I know that you're more than just a delivery drone.",
               "Much much more.",
               "But, you'll need to cut your teeth first.  Start simple...",
               "...you know, you'll need to get some experience before you take on the bigger world.",
               "To play, use the joystick (or keyboard arrow keys) to move.",
               "All you have to do is move through each location one by one, and deliver the package.",
               "Delivering the package is easy, just walk up to the person and your delivery mechanism will do "
                  "the rest.",
               "I just want you to remember, little Robieo...",
               "You're my pride and joy.",
               "Now go out and do your best! I'll be rooting for you all the way.",
            },
            { 
               {
                  "Exit",
                  new AllegroFlare::DialogTree::NodeOptions::ExitDialog(),
                  AllegroFlare::BitFlags<uint32_t>(0)
               },
            }
         )
      },

   });

   return node_bank;
}


} // namespace Pipeline


