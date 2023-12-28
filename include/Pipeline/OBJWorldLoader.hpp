#pragma once


#include <AllegroFlare/ALLEGRO_VERTEX_WITH_NORMAL.hpp>
#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/Base.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityPool.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <Pipeline/EntityAttributes.hpp>
#include <map>
#include <set>
#include <string>
#include <vector>


namespace Pipeline
{
   class OBJWorldLoader : public Pipeline::EntityAttributes
   {
   private:
      AllegroFlare::BitmapBin* bitmap_bin;
      AllegroFlare::ModelBin* model_bin;
      std::string world_model_obj_name;
      std::string world_model_texture_name;
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* goal_entity;
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* exit_entity;
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* player_character;
      std::map<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*, AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> portal_entity_associations;
      bool loaded;

   protected:


   public:
      OBJWorldLoader(AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::ModelBin* model_bin=nullptr, std::string world_model_obj_name="[unset-world_model_obj_name]", std::string world_model_texture_name="[unset-world_model_texture_name]");
      ~OBJWorldLoader();

      void set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin);
      void set_model_bin(AllegroFlare::ModelBin* model_bin);
      void set_world_model_obj_name(std::string world_model_obj_name);
      void set_world_model_texture_name(std::string world_model_texture_name);
      AllegroFlare::BitmapBin* get_bitmap_bin() const;
      AllegroFlare::ModelBin* get_model_bin() const;
      std::string get_world_model_obj_name() const;
      std::string get_world_model_texture_name() const;
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* get_goal_entity() const;
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* get_exit_entity() const;
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* get_player_character() const;
      std::map<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*, AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> get_portal_entity_associations() const;
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool load();
      AllegroFlare::Vec3D lowest_y_vertex(std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> vertices={});
      std::set<std::string> find_named_object_identifiers_for_portals(AllegroFlare::Model3D* world_model=nullptr);
      std::set<std::string> find_named_object_identifiers_for_cameras(AllegroFlare::Model3D* world_model=nullptr);
   };
}



