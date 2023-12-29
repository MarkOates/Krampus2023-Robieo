#pragma once




namespace Pipeline
{
   class EntityAttributes
   {
   public:
      static constexpr char* ATTRIBUTE_COLLIDABLE_BY_PLAYER = (char*)"collidable_by_player";
      static constexpr char* ATTRIBUTE_CUSTOM_COLLISION_RADIUS = (char*)"custom_collision_radius";
      static constexpr char* ATTRIBUTE_ITEM_TYPE = (char*)"item_type";
      static constexpr char* ATTRIBUTE_ITEM_PICKUP_SOUND = (char*)"item_pickup_sound";
      static constexpr char* ATTRIBUTE_IS_PORTAL = (char*)"is_portal";
      static constexpr char* ATTRIBUTE_IS_NPC = (char*)"is_npc";
      static constexpr char* ATTRIBUTE_NPC_IDENTIFIER = (char*)"is_npc_identifier";
      static constexpr char* NPC_FORREST_IN_THE_FOREST = (char*)"forrest_in_the_forest";
      static constexpr char* ATTRIBUTE_TYPE_IS_SWITCH = (char*)"is_switch";
      static constexpr char* ATTRIBUTE_TYPE_SWITCH_IS_ACTIVE = (char*)"switch_is_active";

   private:

   protected:


   public:
      EntityAttributes();
      ~EntityAttributes();

   };
}



