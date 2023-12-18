#pragma once


#include <AllegroFlare/DialogTree/NodeBank.hpp>


namespace Pipeline
{
   class DialogNodeBankFactory
   {
   private:

   protected:


   public:
      DialogNodeBankFactory();
      ~DialogNodeBankFactory();

      static AllegroFlare::DialogTree::NodeBank build_production_game_node_bank();
   };
}



