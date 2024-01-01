#pragma once


#include <AllegroFlare/DialogTree/NodeBank.hpp>


namespace Pipeline
{
   class DialogNodeBankFactory
   {
   public:
      static constexpr char* DIALOG_FOREST_NPC_LIKES_MUSHROOMS = (char*)"forest_npc_likes_mushrooms";
      static constexpr char* DIALOG_FOREST_NPC_COMPLETES_QUEST = (char*)"forest_npc_completes_quest";
      static constexpr char* DIALOG_FOREST_NPC_QUEST_ALREADY_COMPLETE = (char*)"forest_npc_already_completed_quest";
      static constexpr char* HOME_CREATOR_GIVES_YOU_INSTRUCTIONS = (char*)"creator_gives_instructions";
      static constexpr char* CHARACTER_DISPLAY_NAME_FOR_DR_LOUISE = (char*)"Dr. Louise";
      static constexpr char* DIALOG_FOR_DR_LOUISE_IN_FOREST = (char*)"dr_louise-forest";
      static constexpr char* DIALOG_FOR_DR_LOUISE_IN_TURRET = (char*)"dr_louise-turret";

   private:

   protected:


   public:
      DialogNodeBankFactory();
      ~DialogNodeBankFactory();

      static AllegroFlare::DialogTree::NodeBank build_production_game_node_bank();
   };
}



