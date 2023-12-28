

#include <Pipeline/OBJWorldLoader.hpp>

#include <AllegroFlare/ALLEGRO_VERTEX_WITH_NORMAL.hpp>
#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityFactory.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityRenderFlags.hpp>
#include <AllegroFlare/Logger.hpp>
#include <LabyrinthOfLore/WorldMap/TileTypeEnum.hpp>
#include <Pipeline/EntityAttributes.hpp>
#include <Pipeline/GameConfigurations/Main.hpp>
#include <Pipeline/Gameplay/Level.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Pipeline
{


OBJWorldLoader::OBJWorldLoader(AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::ModelBin* model_bin, std::string world_model_obj_name, std::string world_model_texture_name)
   : Pipeline::EntityAttributes()
   , bitmap_bin(bitmap_bin)
   , model_bin(model_bin)
   , world_model_obj_name(world_model_obj_name)
   , world_model_texture_name(world_model_texture_name)
   , goal_entity(nullptr)
   , exit_entity(nullptr)
   , player_character(nullptr)
   , portal_entity_associations({})
   , level_camera_zones({})
   , loaded(false)
{
}


OBJWorldLoader::~OBJWorldLoader()
{
}


void OBJWorldLoader::set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin)
{
   this->bitmap_bin = bitmap_bin;
}


void OBJWorldLoader::set_model_bin(AllegroFlare::ModelBin* model_bin)
{
   this->model_bin = model_bin;
}


void OBJWorldLoader::set_world_model_obj_name(std::string world_model_obj_name)
{
   this->world_model_obj_name = world_model_obj_name;
}


void OBJWorldLoader::set_world_model_texture_name(std::string world_model_texture_name)
{
   this->world_model_texture_name = world_model_texture_name;
}


AllegroFlare::BitmapBin* OBJWorldLoader::get_bitmap_bin() const
{
   return bitmap_bin;
}


AllegroFlare::ModelBin* OBJWorldLoader::get_model_bin() const
{
   return model_bin;
}


std::string OBJWorldLoader::get_world_model_obj_name() const
{
   return world_model_obj_name;
}


std::string OBJWorldLoader::get_world_model_texture_name() const
{
   return world_model_texture_name;
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* OBJWorldLoader::get_goal_entity() const
{
   return goal_entity;
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* OBJWorldLoader::get_exit_entity() const
{
   return exit_entity;
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* OBJWorldLoader::get_player_character() const
{
   return player_character;
}


std::map<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*, AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> OBJWorldLoader::get_portal_entity_associations() const
{
   return portal_entity_associations;
}


std::vector<Pipeline::Gameplay::LevelCameraZone> OBJWorldLoader::get_level_camera_zones() const
{
   return level_camera_zones;
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool OBJWorldLoader::load()
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[OBJWorldLoader::load]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("OBJWorldLoader::load: error: guard \"model_bin\" not met");
   }
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool entity_pool;
   //


   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityFactory entity_factory;
   entity_factory.set_bitmap_bin(bitmap_bin);
   entity_factory.set_model_bin(model_bin);



   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Camera3D* camera_entity =
      entity_factory.create_camera_3d();
   AllegroFlare::Camera3D &camera = camera_entity->get_camera_3d_ref();
   camera_entity->set("primary_camera");
   entity_pool.add(camera_entity);


   //
   // Create the player character
   //

   player_character = new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   player_character->set_model_3d(model_bin->auto_get("robot-02.obj"));
   player_character->set_model_3d_texture(bitmap_bin->auto_get("robot-textured-02-uv.jpg"));
   //player_character->get_placement_ref().position.x = 0.0f; //2.5;
   //player_character->get_placement_ref().position.y = 0.0f; //0.5;
   player_character->get_placement_ref().scale = { 0.08, 0.08, 0.08 };
   player_character->get_placement_ref().rotation = { 0.0, -0.25, 0.0 };
   entity_pool.add(player_character);


   //
   // Load the world obj model file, and process it.
   // extract the entities (like goals, environment model, etc)
   //

   // Extract named object from the environment model

   std::cout << "World model obj name: " << world_model_obj_name << std::endl;
   AllegroFlare::Model3D *world_model = model_bin->auto_get(world_model_obj_name);
   if (!world_model)
   {
      throw std::runtime_error("Missing world model");
   }
   else
   {
      // Extract named objects and build entities from them
      std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> named_object_vertices;

      // Find and create the goal
      named_object_vertices = world_model->extract_named_object_vertices("goal");
      if (named_object_vertices.empty())
      {
         throw std::runtime_error("Named object \"goal\" appears to not exist.");
      }
      else
      {
         // Just use the first vertex as the position of this named object
         // TODO: Use a central point of the vertices
         AllegroFlare::Vec3D object_position = lowest_y_vertex(named_object_vertices);
         //AllegroFlare::Vec3D object_position{
            //named_object_vertices[0].x,
            //named_object_vertices[0].y,
            //named_object_vertices[0].z,
         //};

         // Create our entity
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *item = 
            new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
         //item->set_model_3d(model_bin->auto_get("rounded_unit_cube-01.obj"));
         item->set_model_3d(model_bin->auto_get("gator-character-01.obj"));
         item->set_model_3d_texture(bitmap_bin->auto_get("gator-character-01.png"));
         //item->set(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityRenderFlags::RENDER_WITH_SKYBOX);
         item->get_placement_ref().position = object_position;
         //item->get_placement_ref().position.y = 0.0; // So the character is on the ground
         item->get_placement_ref().rotation.x = 0.0; // /0.05;
         item->get_placement_ref().rotation.z = 0.0; // 0.03547;

         item->set("goal"); // 0.03547;
         entity_pool.add(item);

         goal_entity = item;

         world_model->remove_named_object("goal");
      }

      // Find and create the exit
      named_object_vertices = world_model->extract_named_object_vertices("exit");
      if (named_object_vertices.empty())
      {
         throw std::runtime_error("Named object \"exit\" appears to not exist.");
      }
      else
      {
         // Just use the first vertex as the position of this named object
         // TODO: Use a central point of the vertices
         AllegroFlare::Vec3D object_position = lowest_y_vertex(named_object_vertices);
         //AllegroFlare::Vec3D object_position{
            //named_object_vertices[0].x,
            //named_object_vertices[0].y,
            //named_object_vertices[0].z,
         //};

         // Create our entity
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *item = 
            new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
         item->set_model_3d(model_bin->auto_get("exit-01.obj"));
         item->set_model_3d_texture(bitmap_bin->auto_get("exit-01.png"));
         //item->set(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityRenderFlags::RENDER_WITH_SKYBOX);
         item->get_placement_ref().position = object_position;
         //item->get_placement_ref().position.y = 0.0f;
         item->get_placement_ref().rotation.x = 0.0;
         item->get_placement_ref().rotation.z = 0.0;
         //item->get_velocity_ref().rotation.z = 0.01;
         entity_pool.add(item);

         exit_entity = item;

         world_model->remove_named_object("exit");
      }

      // Create the environment object
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *env = 
         new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
      env->set_model_3d(model_bin->auto_get(world_model_obj_name));
      env->set_model_3d_texture(bitmap_bin->auto_get(world_model_texture_name));
      env->get_placement_ref().position.x = 0;
      env->get_placement_ref().position.y = 0;
      entity_pool.add(env);
   }



   // Mushrooms
   int mushrooms_found = 0;
   if (world_model)
   {
      std::string object_name = "mushroom";
      std::string model_name = "mushroom-01.obj";
      std::string texture_name = "mushroom-01.png";

      std::vector<std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL>> objects_vertices =
         world_model->extract_named_objects_vertices(object_name);

      for (auto &object_vertices : objects_vertices)
      {
          AllegroFlare::Vec3D object_position = lowest_y_vertex(object_vertices);

          AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *object = 
             new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
          object->set_model_3d(model_bin->auto_get(model_name));
          object->set_model_3d_texture(bitmap_bin->auto_get(texture_name));
          object->get_placement_ref().position = object_position;
          object->get_placement_ref().scale = { 0.2, 0.2, 0.2 };
          //object->get_placement_ref().rotation.x = 0.01;

          object->set(ATTRIBUTE_COLLIDABLE_BY_PLAYER);
          object->set(ATTRIBUTE_ITEM_TYPE, "mushroom");
          object->set(ATTRIBUTE_ITEM_PICKUP_SOUND, "mushroom_pickup");
          //env->set(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityRenderFlags::RENDER_WITH_SKYBOX);

          //env->get_placement_ref().position.y = 0.0; // NOTE: The objects will always be placed at 0
          entity_pool.add(object);

          mushrooms_found++;
      }

       world_model->remove_named_objects(object_name);
   }
   std::cout << "Found " << mushrooms_found << " mushrooms" << std::endl;



   // Gems (green)
   int gems_found = 0;
   if (world_model)
   {
      std::string object_name = "gem";
      std::string model_name = "gem-01.obj";
      std::string texture_name = "gem-01.png";

      std::vector<std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL>> objects_vertices =
         world_model->extract_named_objects_vertices(object_name);

      for (auto &object_vertices : objects_vertices)
      {
          AllegroFlare::Vec3D object_position = lowest_y_vertex(object_vertices);

          AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *object =
             new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
          object->set_model_3d(model_bin->auto_get(model_name));
          object->set_model_3d_texture(bitmap_bin->auto_get(texture_name));
          object->get_placement_ref().position = object_position;
          object->get_placement_ref().scale = { 0.2, 0.2, 0.2 };
          //object->get_placement_ref().rotation.y = 0.01;
          object->get_velocity_ref().rotation.y = 0.005;

          object->set(ATTRIBUTE_COLLIDABLE_BY_PLAYER);
          object->set(ATTRIBUTE_ITEM_TYPE, "gem");
          object->set(ATTRIBUTE_ITEM_PICKUP_SOUND, "gem_chime");
          object->set(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityRenderFlags::RENDER_WITH_SKYBOX);

          //env->get_placement_ref().position.y = 0.0; // NOTE: The objects will always be placed at 0
          entity_pool.add(object);

          gems_found++;
      }

       world_model->remove_named_objects(object_name);
   }
   std::cout << "Found " << gems_found << " gems" << std::endl;



   // Red birds
   int red_birds = 0;
   if (world_model)
   {
      std::string object_name = "red_bird";
      std::string model_name = "red_bird-01.obj";
      std::string texture_name = "red_bird-01.png";

      std::vector<std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL>> objects_vertices =
         world_model->extract_named_objects_vertices(object_name);

      for (auto &object_vertices : objects_vertices)
      {
          AllegroFlare::Vec3D object_position = lowest_y_vertex(object_vertices);

          AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *object = 
             new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
          object->set_model_3d(model_bin->auto_get(model_name));
          object->set_model_3d_texture(bitmap_bin->auto_get(texture_name));
          object->get_placement_ref().position = object_position;
          object->get_placement_ref().scale = { 0.2, 0.2, 0.2 };
          //object->get_placement_ref().rotation.y = 0.01;

          object->set(ATTRIBUTE_COLLIDABLE_BY_PLAYER);
          object->set(ATTRIBUTE_ITEM_TYPE, "red_bird");
          object->set(ATTRIBUTE_CUSTOM_COLLISION_RADIUS, 2.2f);
          //object->set(ATTRIBUTE_ITEM_PICKUP_SOUND, "mushroom_pickup");
          //env->set(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityRenderFlags::RENDER_WITH_SKYBOX);

          //env->get_placement_ref().position.y = 0.0; // NOTE: The objects will always be placed at 0
          entity_pool.add(object);

          red_birds++;
      }

       world_model->remove_named_objects(object_name);
   }
   std::cout << "Found " << red_birds << " red_birds" << std::endl;



   // NPCs
   int npcs_found = 0;
   if (world_model)
   {
      std::string object_name = "npc1";
      std::string model_name = "gator-character-01.obj";
      std::string texture_name = "gator-character-01x.png";

      std::vector<std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL>> objects_vertices =
         world_model->extract_named_objects_vertices(object_name);

      if (objects_vertices.size() > 1) throw std::runtime_error(
         "expecting only one (or none) objects named \"" + object_name + "\""
      );

      for (auto &object_vertices : objects_vertices)
      {
          AllegroFlare::Vec3D object_position = lowest_y_vertex(object_vertices);

          AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *object = 
             new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
          object->set_model_3d(model_bin->auto_get(model_name));
          object->set_model_3d_texture(bitmap_bin->auto_get(texture_name));
          object->get_placement_ref().position = object_position;
          //object->get_placement_ref().scale = { 0.2, 0.2, 0.2 };
          //object->get_placement_ref().rotation.y = 0.01;

          object->set(ATTRIBUTE_COLLIDABLE_BY_PLAYER);
          object->set(ATTRIBUTE_IS_NPC);
          object->set(ATTRIBUTE_NPC_IDENTIFIER, NPC_FORREST_IN_THE_FOREST);
          //object->set(ATTRIBUTE_ITEM_TYPE, "mushroom");
          //object->set(ATTRIBUTE_ITEM_PICKUP_SOUND, "mushroom_pickup");
          //env->set(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityRenderFlags::RENDER_WITH_SKYBOX);

          //env->get_placement_ref().position.y = 0.0; // NOTE: The objects will always be placed at 0
          entity_pool.add(object);

          npcs_found++;
      }

      world_model->remove_named_objects(object_name);
   }
   std::cout << "Found " << npcs_found << " npcs" << std::endl;



   // Portals
   std::set<std::string> portal_identifiers = find_named_object_identifiers_for_portals(world_model);

   std::string portal_model_name = "portal-01.obj";
   std::string portal_texture_name = "portal-01.png";
   //std::string portal_model_scale = "portal-01.png";

   for (auto &portal_identifier : portal_identifiers)
   {
      std::vector<std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL>> objects_vertices =
         world_model->extract_named_objects_vertices(portal_identifier);

      // Validate there are exactly 2
      if (objects_vertices.size() != 2)
      {
         AllegroFlare::Logger::throw_error(
            "Pipeline::Gameplay::Screen::load_level_by_identifier",
            "Expecting there to be only 2 objects with a portal name, but there were \""
               + std::to_string(objects_vertices.size()) + "\" with the name \"" + portal_identifier + "\""
         );
      }

      // Build "portal 1"
      std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> portal_1_vertices = objects_vertices[0];
       AllegroFlare::Vec3D portal_1_position = lowest_y_vertex(portal_1_vertices);
       AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *portal_1 = 
          new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
       portal_1->set_model_3d(model_bin->auto_get(portal_model_name));
       portal_1->set_model_3d_texture(bitmap_bin->auto_get(portal_texture_name));
       portal_1->get_placement_ref().position = portal_1_position;
       portal_1->set(ATTRIBUTE_COLLIDABLE_BY_PLAYER);
       portal_1->set(ATTRIBUTE_IS_PORTAL);


      // Build "portal 2"
      std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> portal_2_vertices = objects_vertices[1];
      AllegroFlare::Vec3D portal_2_position = lowest_y_vertex(portal_2_vertices);
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *portal_2 = 
         new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
      portal_2->set_model_3d(model_bin->auto_get(portal_model_name));
      portal_2->set_model_3d_texture(bitmap_bin->auto_get(portal_texture_name));
      portal_2->get_placement_ref().position = portal_2_position;
      portal_2->set(ATTRIBUTE_COLLIDABLE_BY_PLAYER);
      portal_2->set(ATTRIBUTE_IS_PORTAL);


      // TODO: Link them together
      // TODO: Make a list of paired portals
      // TODO: Make sure entity does not already have a key present exist
      portal_entity_associations[portal_2] = portal_1;
      portal_entity_associations[portal_1] = portal_2;


      // Add them to the pool
      entity_pool.add(portal_1);
      entity_pool.add(portal_2);


       // Remove the named objects from the world_model
       world_model->remove_named_objects(portal_identifier);
   }




   // Cameras
   std::set<std::string> camera_identifiers = find_named_object_identifiers_for_cameras(world_model);

   for (auto &camera_identifier : camera_identifiers)
   {
      std::vector<std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL>> objects_vertices =
         world_model->extract_named_objects_vertices(camera_identifier);


      //std::vector<std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL>> object_vertices =
         //world_model->extract_named_objects_vertices(camera_identifier);
      for (auto &object_vertices : objects_vertices)
      {
         // Validate there are exactly 2
         //if (objects_vertices.size() != 2)
         //{
            //AllegroFlare::Logger::throw_error(
               //"Pipeline::Gameplay::Screen::load_level_by_identifier",
               //"Expecting there to be only 2 objects with a portal name, but there were \""
                  //+ std::to_string(objects_vertices.size()) + "\" with the name \"" + portal_identifier + "\""
            //);
         //}

         // TODO: Add the camera objects
         Pipeline::Gameplay::LevelCameraZone camera_zone;
         camera_zone.set_name(camera_identifier);
         auto &camera_zone_bounding_box = camera_zone.get_bounding_box_ref();
         camera_zone_bounding_box.set_min(build_bounding_box_min_coordinate(object_vertices));
         camera_zone_bounding_box.set_max(build_bounding_box_max_coordinate(object_vertices));

         //std::cout << "-- Camera zone found:" << std::endl;
         //std::cout << "  min_x: " << camera_zone_bounding_box.get_min().x << std::endl;
         //std::cout << "  max_x: " << camera_zone_bounding_box.get_max().x << std::endl;
         //std::cout << "  min_y: " << camera_zone_bounding_box.get_min().y << std::endl;
         //std::cout << "  max_y: " << camera_zone_bounding_box.get_max().y << std::endl;
         //std::cout << "  min_z: " << camera_zone_bounding_box.get_min().z << std::endl;
         //std::cout << "  max_z: " << camera_zone_bounding_box.get_max().z << std::endl;

         level_camera_zones.push_back(camera_zone);

         //std::cout << "-- Camera zone included:" << std::endl;
         //std::cout << "  min_x: " << level_camera_zones.back().get_bounding_box_ref().get_min().x << std::endl;
         //std::cout << "  max_x: " << level_camera_zones.back().get_bounding_box_ref().get_max().x << std::endl;
         //std::cout << "  min_y: " << camera_zone.get_min().y << std::endl;
         //std::cout << "  max_y: " << camera_zone.get_max().y << std::endl;
         //std::cout << "  min_z: " << camera_zone.get_min().z << std::endl;
         //std::cout << "  max_z: " << camera_zone.get_max().z << std::endl;
      }

      // Remove the named objects from the world_model
      world_model->remove_named_objects(camera_identifier);
   }


   loaded = true;

   return entity_pool;
}

AllegroFlare::Vec3D OBJWorldLoader::lowest_y_vertex(std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> vertices)
{
   if (!((!vertices.empty())))
   {
      std::stringstream error_message;
      error_message << "[OBJWorldLoader::lowest_y_vertex]: error: guard \"(!vertices.empty())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("OBJWorldLoader::lowest_y_vertex: error: guard \"(!vertices.empty())\" not met");
   }
   AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL result = vertices[0];
   for (auto &vertex : vertices)
   {
      if (vertex.y < result.y) result = vertex;
   }
   return AllegroFlare::Vec3D{result.x, result.y, result.z};
}

AllegroFlare::Vec3D OBJWorldLoader::build_bounding_box_min_coordinate(std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> vertices)
{
   if (!((!vertices.empty())))
   {
      std::stringstream error_message;
      error_message << "[OBJWorldLoader::build_bounding_box_min_coordinate]: error: guard \"(!vertices.empty())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("OBJWorldLoader::build_bounding_box_min_coordinate: error: guard \"(!vertices.empty())\" not met");
   }
   using AllegroFlare::vec3d;
   //validate_initialized_or_output_to_cerr("get_min_vertex_coordinate");
   if (vertices.empty()) return vec3d(0, 0, 0);

   vec3d min_coord = vec3d(vertices[0].x, vertices[0].y, vertices[0].z);
   for (unsigned i=1; i<vertices.size(); i++)
   {
      if (vertices[i].x < min_coord.x) min_coord.x = vertices[i].x;
      if (vertices[i].y < min_coord.y) min_coord.y = vertices[i].y;
      if (vertices[i].z < min_coord.z) min_coord.z = vertices[i].z;
   }
   return min_coord;
}

AllegroFlare::Vec3D OBJWorldLoader::build_bounding_box_max_coordinate(std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> vertices)
{
   if (!((!vertices.empty())))
   {
      std::stringstream error_message;
      error_message << "[OBJWorldLoader::build_bounding_box_max_coordinate]: error: guard \"(!vertices.empty())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("OBJWorldLoader::build_bounding_box_max_coordinate: error: guard \"(!vertices.empty())\" not met");
   }
   using AllegroFlare::vec3d;
   //validate_initialized_or_output_to_cerr("get_max_vertex_coordinate");
   if (vertices.empty()) return vec3d(0, 0, 0);

   vec3d max_coord = vec3d(vertices[0].x, vertices[0].y, vertices[0].z);
   for (unsigned i=0; i<vertices.size(); i++)
   {
      if (vertices[i].x > max_coord.x) max_coord.x = vertices[i].x;
      if (vertices[i].y > max_coord.y) max_coord.y = vertices[i].y;
      if (vertices[i].z > max_coord.z) max_coord.z = vertices[i].z;
   }
   return max_coord;
}

std::set<std::string> OBJWorldLoader::find_named_object_identifiers_for_portals(AllegroFlare::Model3D* world_model)
{
   if (!(world_model))
   {
      std::stringstream error_message;
      error_message << "[OBJWorldLoader::find_named_object_identifiers_for_portals]: error: guard \"world_model\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("OBJWorldLoader::find_named_object_identifiers_for_portals: error: guard \"world_model\" not met");
   }
   std::set<std::string> portal_names;
   std::vector<AllegroFlare::Model3D::named_object> &named_objects = world_model->named_objects;
   for (auto &named_object : named_objects)
   {
      std::string identifier = named_object.identifier;
      if (identifier.compare(0, 7, "portal-") == 0)
      {
         portal_names.insert(identifier);
      }
   }

   // Verify there are two of each
   for (auto &portal_name : portal_names)
   {
      int num_with_name = world_model->count_num_named_objects_with_name(portal_name);
      if (num_with_name != 2)
      {
         AllegroFlare::Logger::throw_error(
            "Pipeline::Gameplay::Screen::find_portal_named_object_identfiers",
            "Expecting there to be only 2 objects with a portal name, but there were \""
               + std::to_string(num_with_name) + "\" with the name \"" + portal_name + "\""
         );
      }
   }

   return portal_names;
}

std::set<std::string> OBJWorldLoader::find_named_object_identifiers_for_cameras(AllegroFlare::Model3D* world_model)
{
   if (!(world_model))
   {
      std::stringstream error_message;
      error_message << "[OBJWorldLoader::find_named_object_identifiers_for_cameras]: error: guard \"world_model\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("OBJWorldLoader::find_named_object_identifiers_for_cameras: error: guard \"world_model\" not met");
   }
   std::set<std::string> camera_names;
   std::vector<AllegroFlare::Model3D::named_object> &named_objects = world_model->named_objects;
   for (auto &named_object : named_objects)
   {
      std::string identifier = named_object.identifier;
      if (identifier.compare(0, 7, "camera-") == 0)
      {
         camera_names.insert(identifier);
      }
   }
   return camera_names;
}


} // namespace Pipeline


