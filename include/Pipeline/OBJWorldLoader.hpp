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
      std::map<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*, AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> portal_entity_associations;

   protected:


   public:
      OBJWorldLoader(AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::ModelBin* model_bin=nullptr, std::string world_model_obj_name="[unset-world_model_obj_name]", std::string world_model_texture_name="[unset-world_model_texture_name]");
      ~OBJWorldLoader();

      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool load();
      AllegroFlare::Vec3D lowest_y_vertex(std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> vertices={});
      std::set<std::string> find_named_object_identifiers_for_portals(AllegroFlare::Model3D* world_model=nullptr);
   };
}



