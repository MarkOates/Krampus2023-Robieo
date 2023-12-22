#pragma once


#include <AllegroFlare/DialogTree/NodeBank.hpp>


namespace Pipeline
{
   class DialogNodeBankFactory
   {
   public:
      static constexpr char* DIALOG_FOREST_NPC_LIKES_MUSHROOMS = (char*)"forest_npc_likes_mushrooms";

   private:

   protected:


   public:
      DialogNodeBankFactory();
      ~DialogNodeBankFactory();

      static AllegroFlare::DialogTree::NodeBank build_production_game_node_bank();
   };
}



