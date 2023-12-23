

#include <Pipeline/Gameplay/Screen.hpp>

#include <AllegroFlare/ALLEGRO_VERTEX_WITH_NORMAL.hpp>
#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <AllegroFlare/EventNames.hpp>
#include <AllegroFlare/GameEventDatas/String.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityFactory.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityRenderFlags.hpp>
#include <AllegroFlare/Interpolators.hpp>
#include <AllegroFlare/Logger.hpp>
#include <LabyrinthOfLore/WorldMap/BasicRenderer.hpp>
#include <LabyrinthOfLore/WorldMap/TileTypeEnum.hpp>
#include <Pipeline/DialogNodeBankFactory.hpp>
#include <Pipeline/GameConfigurations/Main.hpp>
#include <Pipeline/Gameplay/Level.hpp>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>


namespace Pipeline
{
namespace Gameplay
{


Screen::Screen(AllegroFlare::Frameworks::Full* framework, AllegroFlare::EventEmitter* event_emitter, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::FontBin* font_bin, AllegroFlare::ModelBin* model_bin, AllegroFlare::GameConfigurations::Base* game_configuration)
   : AllegroFlare::Screens::Base(Pipeline::Gameplay::Screen::TYPE)
   , framework(framework)
   , event_emitter(event_emitter)
   , bitmap_bin(bitmap_bin)
   , font_bin(font_bin)
   , model_bin(model_bin)
   , game_configuration(game_configuration)
   , game_progress_and_state_info(nullptr)
   , entity_pool()
   , player_controlled_entity(nullptr)
   , player_control_velocity()
   , player_control_dashing(false)
   , goal_entity(nullptr)
   , exit_entity(nullptr)
   , scene_renderer()
   , current_level_identifier("[unset-current_level]")
   , current_level(nullptr)
   , current_level_tile_map(nullptr)
   , current_level_song_to_perform_identifier("")
   , current_level_song_to_perform_duration_sec(0.0f)
   , currently_performing_song_identifier("")
   , currently_performing_song_duration_sec(0.0f)
   , on_finished_callback_func()
   , on_finished_callback_func_user_data(nullptr)
   , initialized(false)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
   , player_is_colliding_on_goal(false)
   , player_is_colliding_on_exit(false)
   , entities_player_entity_is_colliding_with({})
   , portal_entity_associations({})
{
}


Screen::~Screen()
{
}


void Screen::set_game_configuration(AllegroFlare::GameConfigurations::Base* game_configuration)
{
   this->game_configuration = game_configuration;
}


void Screen::set_game_progress_and_state_info(Pipeline::GameProgressAndStateInfo* game_progress_and_state_info)
{
   this->game_progress_and_state_info = game_progress_and_state_info;
}


void Screen::set_on_finished_callback_func(std::function<void(Pipeline::Gameplay::Screen*, void*)> on_finished_callback_func)
{
   this->on_finished_callback_func = on_finished_callback_func;
}


void Screen::set_on_finished_callback_func_user_data(void* on_finished_callback_func_user_data)
{
   this->on_finished_callback_func_user_data = on_finished_callback_func_user_data;
}


AllegroFlare::GameConfigurations::Base* Screen::get_game_configuration() const
{
   return game_configuration;
}


Pipeline::GameProgressAndStateInfo* Screen::get_game_progress_and_state_info() const
{
   return game_progress_and_state_info;
}


std::function<void(Pipeline::Gameplay::Screen*, void*)> Screen::get_on_finished_callback_func() const
{
   return on_finished_callback_func;
}


void* Screen::get_on_finished_callback_func_user_data() const
{
   return on_finished_callback_func_user_data;
}


uint32_t Screen::get_state() const
{
   return state;
}


void Screen::set_framework(AllegroFlare::Frameworks::Full* framework)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_framework]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_framework: error: guard \"(!initialized)\" not met");
   }
   this->framework = framework;
   return;
}

void Screen::set_event_emitter(AllegroFlare::EventEmitter* event_emitter)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_event_emitter]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_event_emitter: error: guard \"(!initialized)\" not met");
   }
   this->event_emitter = event_emitter;
   return;
}

void Screen::set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_bitmap_bin]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_bitmap_bin: error: guard \"(!initialized)\" not met");
   }
   this->bitmap_bin = bitmap_bin;
   return;
}

void Screen::set_font_bin(AllegroFlare::FontBin* font_bin)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_font_bin]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_font_bin: error: guard \"(!initialized)\" not met");
   }
   this->font_bin = font_bin;
   return;
}

void Screen::set_model_bin(AllegroFlare::ModelBin* model_bin)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_model_bin]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_model_bin: error: guard \"(!initialized)\" not met");
   }
   this->model_bin = model_bin;
   return;
   return;
}

bool Screen::trivial_collide(AllegroFlare::Vec3D p1, AllegroFlare::Vec3D p2, float min_distance)
{
   float squared_distance = (p1.x - p2.x) * (p1.x - p2.x)
                          + (p1.y - p2.y) * (p1.y - p2.y)
                          + (p1.z - p2.z) * (p1.z - p2.z);
   float min_distance_squared = min_distance * min_distance;
   return squared_distance < min_distance_squared;
}

void Screen::set_primary_camera_to_gameplay_view()
{
   AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 0.0, 18.0 };
   primary_camera->spin = 0.5;
   primary_camera->tilt = 0.75;
   primary_camera->zoom = 3.0;
   return;
}

void Screen::set_primary_camera_to_dialog_view()
{
   AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 0.0, 10.0 };
   primary_camera->spin = 0.5 - 0.2;
   primary_camera->tilt = 0.75 - 0.4;
   primary_camera->zoom = 2.8;
   return;
}

void Screen::set_primary_camera_to_music_performance_view()
{
   AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 0.25, 6.0 };
   primary_camera->spin = 0.2;
   primary_camera->tilt = 0.2;
   primary_camera->zoom = 3.1;
   return;
}

AllegroFlare::DialogTree::NodeBank Screen::build_dialog_node_bank()
{
   return Pipeline::DialogNodeBankFactory::build_production_game_node_bank();
}

AllegroFlare::Vec3D Screen::lowest_y_vertex(std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> vertices)
{
   if (!((!vertices.empty())))
   {
      std::stringstream error_message;
      error_message << "[Screen::lowest_y_vertex]: error: guard \"(!vertices.empty())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::lowest_y_vertex: error: guard \"(!vertices.empty())\" not met");
   }
   AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL result = vertices[0];
   for (auto &vertex : vertices)
   {
      if (vertex.y < result.y) result = vertex;
   }
   return AllegroFlare::Vec3D{result.x, result.y, result.z};
}

std::set<std::string> Screen::find_named_object_identifiers_for_portals(AllegroFlare::Model3D* world_model)
{
   if (!(world_model))
   {
      std::stringstream error_message;
      error_message << "[Screen::find_named_object_identifiers_for_portals]: error: guard \"world_model\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::find_named_object_identifiers_for_portals: error: guard \"world_model\" not met");
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

void Screen::load_level_by_identifier(std::string level_identifier)
{
   if (!(game_configuration))
   {
      std::stringstream error_message;
      error_message << "[Screen::load_level_by_identifier]: error: guard \"game_configuration\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::load_level_by_identifier: error: guard \"game_configuration\" not met");
   }
   if (!(framework))
   {
      std::stringstream error_message;
      error_message << "[Screen::load_level_by_identifier]: error: guard \"framework\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::load_level_by_identifier: error: guard \"framework\" not met");
   }
   //
   // Set our local cached state variables to default
   //

   player_controlled_entity = nullptr;
   player_is_colliding_on_goal = false; // TODO: Replace this with a list of colliding objects
   player_is_colliding_on_exit = false; // TODO: Replace this with a list of colliding objects
   player_control_velocity = { 0.0f, 0.0f };
   player_control_dashing = false;
   goal_entity = nullptr;
   exit_entity = nullptr;
   entities_player_entity_is_colliding_with.clear();
   portal_entity_associations.clear();
   if (current_level_tile_map) delete current_level_tile_map;
   current_level_tile_map = nullptr;


   //
   // Clear our resources so they can be processed from freshly loaded models
   //

   model_bin->clear();
   // TODO: Clear model_bin and the entity_pool
   for (auto &entity : entity_pool.get_entity_pool_ref())
   {
      delete entity;
      //entity_pool.clear();
   }
   entity_pool.get_entity_pool_ref().clear();


   //
   // Create our entity_factory
   //

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityFactory entity_factory;
   entity_factory.set_bitmap_bin(bitmap_bin);
   entity_factory.set_model_bin(model_bin);


   //
   // level_identifier
   current_level_identifier = level_identifier;

   //
   // Create the camera
   //

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Camera3D* camera_entity =
      entity_factory.create_camera_3d();
   AllegroFlare::Camera3D &camera = camera_entity->get_camera_3d_ref();
   camera_entity->set("primary_camera");
   entity_pool.add(camera_entity);


   //
   // Create the player character
   //

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *player_character = 
      new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
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

   std::string world_model_name = level_identifier; //"world-1-01";
   std::string world_model_obj_name = world_model_name + ".obj";
   std::string world_model_texture_name = world_model_name + ".png";

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
          object->get_placement_ref().rotation.y = 0.01;

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



   //
   // Build the tile map (for physics)
   //

   LabyrinthOfLore::WorldMap::TileMap *result_tile_map = new LabyrinthOfLore::WorldMap::TileMap();
   //LabyrinthOfLore::WorldMap::TileTypeEnum::NORMAL_GROUND_TILE;

   result_tile_map->resize(
      16,
      16,
      LabyrinthOfLore::WorldMap::Tile(
         LabyrinthOfLore::WorldMap::NORMAL_GROUND_TILE,
         0.0f
      )
   );


   current_level_tile_map = result_tile_map;




   //
   // Load the dialog bank
   //

   AllegroFlare::DialogTree::NodeBank node_bank = build_dialog_node_bank();

   // Validate important dialog nodes exist
   if (!node_bank.node_exists_by_name("package_delivery_response"))
   {
      throw std::runtime_error("Expecting \"package_delivery_response\" dialog node to exist but it does not.");
   }

   framework->set_dialog_system_dialog_node_bank(node_bank);


   //
   // Load the song to play for this level
   //
   current_level_song_to_perform_identifier = "robot-holly_jolly";


   //
   // Assign our "special" items
   //

   player_controlled_entity = player_character;
   //goal_entity = item;
   player_is_colliding_on_goal = false; // This needs to be changed to an "enter" collision e.g. "exit" collision
   player_is_colliding_on_exit = false; // This needs to be changed to an "enter" collision e.g. "exit" collision


   //
   // Set our initial positions
   //

   set_primary_camera_to_gameplay_view(); // This will be our default initialization position, and could be modified
                                          // in the next step by starting the game


   //
   // Start the game
   //

   set_state(STATE_PLAYING_GAME);


   return;
   // Destroy the current level
   if (current_level)
   {
      // TODO: Shutdown current level
      delete current_level;
   }

   // Load the new level
   AllegroFlare::Levels::Base *loaded_level = game_configuration->load_level_by_identifier(level_identifier);
   if (loaded_level)
   {
      // TODO: Consider how to have this level loading mechanism removed, specifically the dependency on the configuration
      // For now, confirm the type, and cast
      if (!loaded_level->is_type(Pipeline::Gameplay::Level::TYPE))
      {
         throw std::runtime_error("Loaded level not of expected type");
      }
      current_level_identifier = level_identifier;
      current_level = static_cast<Pipeline::Gameplay::Level*>(loaded_level);
   }
   return;
}

void Screen::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"(!initialized)\" not met");
   }
   if (!(framework))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"framework\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"framework\" not met");
   }
   if (!(al_is_system_installed()))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"al_is_system_installed()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"al_is_system_installed()\" not met");
   }
   if (!(al_is_primitives_addon_initialized()))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"al_is_primitives_addon_initialized()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"al_is_primitives_addon_initialized()\" not met");
   }
   if (!(al_is_font_addon_initialized()))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"al_is_font_addon_initialized()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"al_is_font_addon_initialized()\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"event_emitter\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"bitmap_bin\" not met");
   }
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"font_bin\" not met");
   }
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"model_bin\" not met");
   }
   // Setup scene renderer
   scene_renderer.set_entity_pool(&entity_pool);
   scene_renderer.set_data_path_for_shaders(framework->get_data_folder_path() + "shaders/");
   //scene_renderer.setup_result_surface_bitmap(1920 / 3, 1080 / 3);
   scene_renderer.setup_result_surface_bitmap(1920, 1080); // For some reason, 
   scene_renderer.setup_shadow_map_buffer();
   scene_renderer.setup_cubemapping(bitmap_bin->get_path() + "black_prism_1-01.png");
   scene_renderer.setup_multitexture_shader();

   initialized = true;
   return;
}

void Screen::on_activate()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::on_activate]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::on_activate: error: guard \"initialized\" not met");
   }
   //TODO: emit_event_to_update_input_hints_bar();
   //TODO: emit_show_and_size_input_hints_bar_event();
   return;
}

void Screen::on_deactivate()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::on_deactivate]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::on_deactivate: error: guard \"initialized\" not met");
   }
   //TODO: emit_hide_and_restore_size_input_hints_bar_event();
   //event_emitter->emit_hide_input_hints_bar_event();
   return;
}

AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* Screen::get_player_controlled_entity_as()
{
   if (!player_controlled_entity->is_type(
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D::TYPE
         )
      )
   {
      throw std::runtime_error("unexpected player controlled entity type");
   }

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *as =
      static_cast<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *>(
         player_controlled_entity
      );
   return as;
}

AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* Screen::get_goal_entity_as()
{
   if (!goal_entity->is_type(
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D::TYPE
         )
      )
   {
      throw std::runtime_error("unexpected player controlled entity type");
   }

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *as =
      static_cast<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *>(
         goal_entity
      );
   return as;
}

AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* Screen::get_exit_entity_as()
{
   if (!exit_entity->is_type(
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D::TYPE
         )
      )
   {
      throw std::runtime_error("unexpected player controlled entity type");
   }

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *as =
      static_cast<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *>(
         exit_entity
      );
   return as;
}

AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* Screen::get_entity_as_dynamic_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* colliding_entity)
{
   if (!exit_entity->is_type(
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D::TYPE
         )
      )
   {
      throw std::runtime_error("unexpected entity type not being DynamicModel3D");
   }

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *as =
      static_cast<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *>(
         colliding_entity
      );
   return as;
}

void Screen::on_player_entity_raw_collide(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* colliding_entity)
{
   if (!is_state(STATE_PLAYING_GAME)) return;

   if (colliding_entity == goal_entity)
   {
      set_state(STATE_SUSPEND_FOR_DIALOG);
      event_emitter->emit_activate_dialog_node_by_name_event("package_delivery_response");
   }
   else if (colliding_entity == exit_entity)
   {
      call_on_finished_callback_func();
   }
   return;
}

void Screen::on_player_entity_enter_collide(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* colliding_entity)
{
   if (!(player_controlled_entity))
   {
      std::stringstream error_message;
      error_message << "[Screen::on_player_entity_enter_collide]: error: guard \"player_controlled_entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::on_player_entity_enter_collide: error: guard \"player_controlled_entity\" not met");
   }
   if (!is_state(STATE_PLAYING_GAME)) return;

   if (colliding_entity->exists(ATTRIBUTE_IS_PORTAL))
   {
      // TODO: Find teleport to next portal
      // Find this portal in list of portals
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* this_portal =
         colliding_entity;
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* target_portal = nullptr;
      
      // Get the correlated portal
      // TODO: Confirm exists
      if (portal_entity_associations.find(this_portal) == portal_entity_associations.end())
      {
         // This portal doesn't have an entry in the "portal_entity_associations"
         AllegroFlare::Logger::throw_error(
            "Pipeline::Gameplay::Screen::on_player_entity_enter_collide",
            "Expecting there to be an entry for portal (to find the correlated destination portal) but there was "
               "none in the list."
         );
      }
      target_portal = portal_entity_associations[this_portal];
      if (!target_portal)
      {
         // This portal doesn't have an entry in the "portal_entity_associations"
         AllegroFlare::Logger::throw_error(
            "Pipeline::Gameplay::Screen::on_player_entity_enter_collide",
            "The portal was found in the list of \"portal_entity_associations\" but its target portal was a nullptr."
         );
      }

      // Extract the correlated portal's position
      AllegroFlare::Vec3D target_portal_position = target_portal->get_placement_ref().position;

      // Place the player character at the destiation portal
      get_player_controlled_entity_as()->get_placement_ref().position = target_portal_position;

      // Add target_portal to "colliding_with"
         // consider that the calling loop could process a "continuing to collide with" on this entered portal or not
      // TODO: Confirm is not already in the list
      entities_player_entity_is_colliding_with.insert(target_portal);
      
      // Remove the this_portal from "colliding_with"
      // TODO: Confirm is already in the list
      // TODO: Confirm is removed from list
      entities_player_entity_is_colliding_with.erase(this_portal);
   }
   else if (colliding_entity->exists(ATTRIBUTE_IS_NPC))
   {
      std::string npc_identifier = colliding_entity->get(ATTRIBUTE_NPC_IDENTIFIER);
      if (npc_identifier == NPC_FORREST_IN_THE_FOREST)
      {
         std::string quest_name = "mushroom_quest";
         bool mushroom_quest_is_finished = game_progress_and_state_info->is_quest_completed(quest_name);
         if (!mushroom_quest_is_finished)
         {
            int num_mushrooms_in_inventory =
               game_progress_and_state_info->count_num_items_in_inventory_with_identifier("mushroom");
            int num_needed_mushrooms = 10;
            if (num_mushrooms_in_inventory >= num_needed_mushrooms)
            {
               // On completed
               std::string dialog_to_activate = Pipeline::DialogNodeBankFactory::DIALOG_FOREST_NPC_COMPLETES_QUEST;
               game_progress_and_state_info->mark_quest_as_completed(quest_name);
               emit_event_to_save_progress();
               set_state(STATE_SUSPEND_FOR_DIALOG);
               event_emitter->emit_activate_dialog_node_by_name_event(dialog_to_activate);
            }
            else
            {
               // Quest not completed, give prompt
               std::string dialog_to_activate = Pipeline::DialogNodeBankFactory::DIALOG_FOREST_NPC_LIKES_MUSHROOMS;
               set_state(STATE_SUSPEND_FOR_DIALOG);
               event_emitter->emit_activate_dialog_node_by_name_event(dialog_to_activate);
            }
         }
         else
         {
            // Quest is already complete
            std::string dialog_to_activate = Pipeline::DialogNodeBankFactory::DIALOG_FOREST_NPC_QUEST_ALREADY_COMPLETE;
            set_state(STATE_SUSPEND_FOR_DIALOG);
            event_emitter->emit_activate_dialog_node_by_name_event(dialog_to_activate);
         }
      }
      else
      {
         // Unhandled case for this identifier
         throw std::runtime_error("Eeks couldn't find npc handler!");
      }
   }
   else if (colliding_entity->exists(ATTRIBUTE_ITEM_TYPE, "mushroom"))
   {
      // Collect this mushroom
      // TODO: Add the attribute "COLLECTED"
      // TODO: Add the attribute "COLLECTED_AT"
      // TODO: Remove the attribute "ATTRIBUTE_COLLIDABLE_BY_PLAYER"

      // Play sound effect
      if (colliding_entity->exists(ATTRIBUTE_ITEM_PICKUP_SOUND))
      {
         std::string pickup_sound_effect = colliding_entity->get(ATTRIBUTE_ITEM_PICKUP_SOUND);
         event_emitter->emit_play_sound_effect_event(pickup_sound_effect);
      }

      // TODO: Consider preventing player from collecting more than the max allowable for this item

      // Add item to inventory
      if (!game_progress_and_state_info) throw std::runtime_error("Gameplay::Screen::on_player_entity_raw_collide AGH!");
      game_progress_and_state_info->add_item_to_inventory("mushroom");

      // Delete the entity and remove it from the scene (for now. Later, do a collect animation)
      delete colliding_entity; // TODO: Don't delete here, delete in a follow-up pass after collision and everything
      entity_pool.remove(colliding_entity);
      // Remove the entity from the list of entities_player_entity_is_colliding_with
      entities_player_entity_is_colliding_with.erase(colliding_entity);
   }
   else if (colliding_entity->exists(ATTRIBUTE_ITEM_TYPE, "red_bird"))
   {
      // Bird chirps
      // TODO: Add the attribute "COLLECTED"
      // TODO: Add the attribute "COLLECTED_AT"
      // TODO: Remove the attribute "ATTRIBUTE_COLLIDABLE_BY_PLAYER"

      // Play sound effect
      //if (colliding_entity->exists(ATTRIBUTE_ITEM_PICKUP_SOUND))
      //{
      std::string chirp_sound_effect = "red_bird_chirp";
      event_emitter->emit_play_sound_effect_event(chirp_sound_effect);
      //}

      // TODO: Consider preventing player from collecting more than the max allowable for this item

      // Add item to inventory
      //if (!game_progress_and_state_info) throw std::runtime_error("Gameplay::Screen::on_player_entity_raw_collide AGH!");
      //game_progress_and_state_info->add_item_to_inventory("mushroom");

      // Delete the entity and remove it from the scene (for now. Later, do a collect animation)
      //delete colliding_entity; // TODO: Don't delete here, delete in a follow-up pass after collision and everything
      //entity_pool.remove(colliding_entity);
      // Remove the entity from the list of entities_player_entity_is_colliding_with
      //entities_player_entity_is_colliding_with.erase(colliding_entity);
   }
   return;
}

void Screen::on_player_entity_exit_collide(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* colliding_entity)
{
   if (!is_state(STATE_PLAYING_GAME)) return;

   // No exit collisions at this point
   return;
}

void Screen::update()
{
   update_state(); // Consider if this would need to be moved to a different place, or if it conflicts
                   // with the logic below

   // Spin our shadow casted light
   AllegroFlare::Camera3D *light = scene_renderer.get_shadow_map_buffer_ref().get_light();
   light->spin = -1.0f;

   // Pan the camera
   AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   //primary_camera->stepout.z += 0.01;
   //primary_camera->spin += 0.0005;
   //primary_camera->tilt += 0.0008;

   // HERE:

   //player_control_velocity.x = -0.001;
   bool lock_light_on_player_controlled_entity = true;
   bool lock_camera_on_player_controlled_entity = true;

   if (player_controlled_entity)
   {
      float speed_modifier = 0.046f;

      bool using_dash_controls = false;
      if (using_dash_controls)
      {
         bool player_controlled_entity_is_dashing = player_control_dashing; // TODO: Extract this from somewhere
         if (player_controlled_entity_is_dashing) speed_modifier = 0.09f;
      }

      AllegroFlare::Vec2D controlled_entity_velocity = player_control_velocity * speed_modifier;
      // Translate the player control angles to be relative to the camera
      float angle = primary_camera->spin;
      float x_prime = controlled_entity_velocity.x * std::cos(angle) - controlled_entity_velocity.y * std::sin(angle);
      float y_prime = controlled_entity_velocity.x * std::sin(angle) + controlled_entity_velocity.y * std::cos(angle);

      // Move the player
      auto player_entity_as = get_player_controlled_entity_as();
      player_entity_as->get_placement_ref().position.x += x_prime;
      player_entity_as->get_placement_ref().position.z += y_prime;

      // Update the player model rotation to face the moving direction
      if (std::fabs(x_prime) + std::fabs(y_prime) > 0.001) // Only update the rotation if the player is moving
      {
         AllegroFlare::Vec2D prime(x_prime, y_prime);
         float angle_in_radians = prime.get_angle();
         float angle_in_degrees = angle_in_radians * (180.0 / ALLEGRO_PI);
         float angle_in_units = angle_in_degrees / 360.0f;

         bool smooth = true;
         float final_angle = 0.0f;

         if (!smooth)
         {
            final_angle = -angle_in_units;
         }
         else
         {
            // Smoothly rotate our player character to the direction of travel
            float target_angle_in_units = -angle_in_units;
            float current_angle_in_units = player_entity_as->get_placement_ref().rotation.y;

            float rotation_rate = 0.1;
            float target = std::fmod(target_angle_in_units, 1.0f);
            float current = std::fmod(current_angle_in_units, 1.0f);
            float angular_distance = (target - current);

            // Use "angular wraparound" to ensure the player doesn't do an unnecessary full 360
            // If the distance is more than 0.5, subtract 1 to go the shorter way
            // If the distance is less than -0.5, add 1 to go the shorter way
            if (angular_distance > 0.5f) angular_distance -= 1.0f;
            else if (angular_distance < -0.5f) angular_distance += 1.0f;

            // Set the final angle
            final_angle = angular_distance * rotation_rate + current;
         }

         player_entity_as->get_placement_ref().rotation.y = final_angle;
      }


      if (lock_camera_on_player_controlled_entity)
      {
         primary_camera->position.x = player_entity_as->get_placement_ref().position.x;
         primary_camera->position.y = player_entity_as->get_placement_ref().position.y;
         primary_camera->position.z = player_entity_as->get_placement_ref().position.z;
      }

      if (lock_light_on_player_controlled_entity)
      {
         light->position.x = player_entity_as->get_placement_ref().position.x;
         light->position.y = player_entity_as->get_placement_ref().position.y;
         light->position.z = player_entity_as->get_placement_ref().position.z;
      }

      // Check collide with goal
      if (!goal_entity)
      {
         throw std::runtime_error("Pipeline::Gameplay::Screen::update: no goal_entity");
      }
      else
      {
         auto goal_entity_as = get_goal_entity_as();
         bool collides = trivial_collide(
            player_entity_as->get_placement_ref().position,
            goal_entity_as->get_placement_ref().position,
            1.0
         );
         if (collides)
         {
            if (!player_is_colliding_on_goal) on_player_entity_raw_collide(goal_entity_as);
            player_is_colliding_on_goal = true;
         }
         else
         {
            player_is_colliding_on_goal = false;
         }
      }

      // Check collide with exit
      if (!exit_entity)
      {
         throw std::runtime_error("Pipeline::Gameplay::Screen::update: no exit_entity");
      }
      else
      {
         auto exit_entity_as = get_exit_entity_as();
         bool collides = trivial_collide(
            player_entity_as->get_placement_ref().position,
            exit_entity_as->get_placement_ref().position,
            1.0
         );
         if (collides)
         {
            if (!player_is_colliding_on_exit) on_player_entity_raw_collide(exit_entity_as);
            player_is_colliding_on_exit = true;
         }
         else
         {
            player_is_colliding_on_exit = false;
         }
      }

      // Check collidable entities
      std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base*> collidables =
         entity_pool.find_all_with_attribute(ATTRIBUTE_COLLIDABLE_BY_PLAYER);
      //std::cout << "collidables.size(): " << collidables.size() << std::endl; // DEBU
      for (auto &collidable : collidables)
      {
         //bool player_is_already_colliding_on_this_object = false; // TODO: Extract item from list of items

         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* this_collidable_as =
            get_entity_as_dynamic_model_3d(collidable);

         float collision_radius = 0.7;
         if (this_collidable_as->exists(ATTRIBUTE_CUSTOM_COLLISION_RADIUS))
         {
            collision_radius = this_collidable_as->get_as_float(ATTRIBUTE_CUSTOM_COLLISION_RADIUS);
         }

         bool player_is_currently_colliding_with_this_object = trivial_collide(
            player_entity_as->get_placement_ref().position,
            this_collidable_as->get_placement_ref().position,
            collision_radius
         );

         bool player_is_previously_colliding_with_this_object =
            entities_player_entity_is_colliding_with.find(collidable)
               != entities_player_entity_is_colliding_with.end();

         if (player_is_currently_colliding_with_this_object)
         {
            if (!player_is_previously_colliding_with_this_object)
            {
               // On enter
               entities_player_entity_is_colliding_with.insert(collidable);
               // TODO: Maybe have an entered_collision_at for certain collision objects?
               on_player_entity_enter_collide(this_collidable_as);
            }
            else
            {
               // Continuing to collide. So, already colliding, do nothing.
            }
         }
         else
         {
            if (player_is_previously_colliding_with_this_object)
            {
               // On exit
               // TODO: Consider checking presence before erasing
               entities_player_entity_is_colliding_with.erase(collidable);
               // TODO: Maybe have a exited_collision_at for certain collision objects?
               on_player_entity_exit_collide(this_collidable_as);
            }
            else
            {
               // Do nothing, no collision now or after.
            }
         }
      }
   }



   //if (is_state(SUSPEND_FOR_DIALOG))
   //{
      
   //}

   // Check player collision on item
   //if (player_controlled_entity && goal_entity)
   //{
      //auto player_entity_as = get_player_controlled_entity_as();
   //}



   // Rotate objects in the scene
   //item->get_placement_ref().rotation.x += 0.005;
   //item->get_placement_ref().rotation.z += 0.003547;
   return;
}

void Screen::render()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::render]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::render: error: guard \"initialized\" not met");
   }
   ALLEGRO_BITMAP *initial_target_bitmap = al_get_target_bitmap();

   // Render the scene
   scene_renderer.render();
   ALLEGRO_BITMAP *render_surface = scene_renderer.get_render_surface_ref().obtain_surface();

   al_set_target_bitmap(initial_target_bitmap);
   al_draw_bitmap(render_surface, 0, 0, 0);

   // Slopily render the tile map

   //al_clear_depth_buffer(1.0);

   int tile_size = 32;
   AllegroFlare::Vec3D player_position = get_player_controlled_entity_as()->get_placement_ref().position;
   float player_map_position_x = player_position.x * tile_size;
   float player_map_position_y = player_position.z * tile_size;

   AllegroFlare::Placement3D map_placement;
   map_placement.position.x = 1920/2 - player_map_position_x;
   map_placement.position.y = 1080/2 - player_map_position_y;
   //map_placement.rotation.x = 0.35;
   map_placement.start_transform();
   LabyrinthOfLore::WorldMap::BasicRenderer basic_renderer;
   basic_renderer.set_tile_map(current_level_tile_map);
   basic_renderer.set_tile_width(tile_size);
   basic_renderer.set_tile_height(tile_size);
   basic_renderer.render();

   // Draw the player marker
   al_draw_filled_circle(
      player_map_position_x, //player_position.x * basic_renderer.get_tile_width(),
      player_map_position_y, //player_position.z * basic_renderer.get_tile_height(),
      8,
      al_color_name("azure")
   );
   map_placement.restore_transform();

   ALLEGRO_FONT *ui_font = obtain_ui_font();
   ALLEGRO_COLOR hud_text_color = al_color_name("cyan");
   std::stringstream coordinates;
   coordinates << std::setprecision(3) << player_position.x << ", " << player_position.z;
   al_draw_text(ui_font, hud_text_color, 1920 - 300, 1080 - 200, ALLEGRO_ALIGN_RIGHT, coordinates.str().c_str());
   std::stringstream coordinates2;
   coordinates2 << std::setprecision(3) << player_position.y;
   al_draw_text(ui_font, hud_text_color, 1920 - 300, 1080 - 200 + 20, ALLEGRO_ALIGN_RIGHT, coordinates2.str().c_str());

   return;
}

void Screen::save_bitmap_buffers_to_files()
{
   ALLEGRO_BITMAP *render_surface = scene_renderer.get_render_surface_ref().obtain_surface();

   std::string location_for_saving_files = "tmp/";
   bool directory_creation_successful = al_make_directory(location_for_saving_files.c_str());
   if (!directory_creation_successful)
   {
      // TODO: Consider if should throw, create notification, something else.
   }

   // Save the shadow depth map render
   bool file1_ok = al_save_bitmap(
      (location_for_saving_files + "shadow_depth_map_bitmap.png").c_str(),
      scene_renderer.get_shadow_map_buffer_ref().get_shadow_depth_map_renderer_ref().get_result_surface_bitmap()
   );

   // Save the shadow_buffer
   bool file2_ok = al_save_bitmap(
      (location_for_saving_files + "shadow_buffer_bitmap.png").c_str(),
      scene_renderer.get_shadow_map_buffer_ref().get_result_bitmap()
   );

   // Save the scene
   bool file3_ok = al_save_bitmap(
      (location_for_saving_files + "render_surface.png").c_str(),
      scene_renderer.get_render_surface_ref().obtain_surface()
   );
   return;
}

void Screen::call_on_finished_callback_func()
{
   // TODO: Test this callback call
   if (on_finished_callback_func) on_finished_callback_func(this, on_finished_callback_func_user_data);
}

void Screen::on_event(ALLEGRO_EVENT* ev)
{
   if (!(ev))
   {
      std::stringstream error_message;
      error_message << "[Screen::on_event]: error: guard \"ev\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::on_event: error: guard \"ev\" not met");
   }
   if (ev->type == ALLEGRO_FLARE_EVENT_DIALOG_SWITCHED_OUT && is_state(STATE_SUSPEND_FOR_DIALOG))
   {
      set_state(STATE_PLAYING_GAME);
   }
   return;
}

void Screen::game_event_func(AllegroFlare::GameEvent* game_event)
{
   if (!(game_event))
   {
      std::stringstream error_message;
      error_message << "[Screen::game_event_func]: error: guard \"game_event\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::game_event_func: error: guard \"game_event\" not met");
   }
   if (game_event->is_type("perform_music"))
   {
      activate_music_performance(current_level_song_to_perform_identifier);
   }
   //else if (game_event->is_type("collect_special_item"))
   //{
      //activate_music_performance(current_level_song_to_perform_identifier);
   //}
   return;
}

void Screen::primary_timer_func()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::primary_timer_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::primary_timer_func: error: guard \"initialized\" not met");
   }
   update();
   render();
   return;
}

void Screen::key_up_func(ALLEGRO_EVENT* ev)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::key_up_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::key_up_func: error: guard \"initialized\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::key_up_func]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::key_up_func: error: guard \"event_emitter\" not met");
   }
   if (!is_state(STATE_PLAYING_GAME)) return;

   switch(ev->keyboard.keycode)
   {
      case ALLEGRO_KEY_UP:
      case ALLEGRO_KEY_DOWN: {
         player_control_velocity.y = 0;
      } break;

      case ALLEGRO_KEY_LEFT:
      case ALLEGRO_KEY_RIGHT: {
         player_control_velocity.x = 0;
      } break;

      case ALLEGRO_KEY_R: {
         player_control_dashing = false;
      } break;

      default: {
         //attempt_an_action_at(ev->keyboard.keycode);
      } break;
   }

   return;
}

void Screen::key_down_func(ALLEGRO_EVENT* ev)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::key_down_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::key_down_func: error: guard \"initialized\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::key_down_func]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::key_down_func: error: guard \"event_emitter\" not met");
   }
   // Debugging
   switch(ev->keyboard.keycode)
   {
      case ALLEGRO_KEY_0: {
         save_bitmap_buffers_to_files();
      } break;

      default: {
      } break;
   }

   // Cancel out of music performance
   if (is_state(STATE_PERFORMING_MUSIC))
   {
      switch(ev->keyboard.keycode)
      {
         case ALLEGRO_KEY_X: {
            deactivate_music_performance();
         } break;

         default: {
         } break;
      }
   }

   if (!is_state(STATE_PLAYING_GAME)) return;

   // Normal gameplay controls
   switch(ev->keyboard.keycode)
   {
      case ALLEGRO_KEY_UP: {
         player_control_velocity.y = -1.0;
         //move_development_cursor_up();
      } break;

      case ALLEGRO_KEY_DOWN: {
         player_control_velocity.y = 1.0;
         //move_development_cursor_down();
      } break;

      case ALLEGRO_KEY_LEFT: {
         player_control_velocity.x = -1.0;
         //move_development_cursor_down();
      } break;

      case ALLEGRO_KEY_RIGHT: {
         player_control_velocity.x = 1.0;
         //move_development_cursor_down();
      } break;

      case ALLEGRO_KEY_R: {
         player_control_dashing = true;
      } break;

      //case ALLEGRO_KEY_P: {
         //activate_music_performance(current_level_song_to_perform_identifier);
         ////move_development_cursor_down();
      //} break;

      //case ALLEGRO_KEY_X: {
         //deactivate_music_performance();
         ////move_development_cursor_down();
      //} break;

      // TODO: Deliver the package with "enter"
      //case ALLEGRO_KEY_ENTER: {
         //attempt_to_deliver_package();
      //} break;

      default: {
         //attempt_an_action_at(ev->keyboard.keycode);
      } break;
   }

   return;
}

void Screen::joy_button_down_func(ALLEGRO_EVENT* ev)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::joy_button_down_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::joy_button_down_func: error: guard \"initialized\" not met");
   }
   if (!(ev))
   {
      std::stringstream error_message;
      error_message << "[Screen::joy_button_down_func]: error: guard \"ev\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::joy_button_down_func: error: guard \"ev\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::joy_button_down_func]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::joy_button_down_func: error: guard \"event_emitter\" not met");
   }
   int button = ev->joystick.button;

   std::cout << "joy button event (" << ev->joystick.id << ")" << std::endl;
   std::cout << "   button: " << ev->joystick.button << std::endl;

   // Cancel out of music performance
   if (is_state(STATE_PERFORMING_MUSIC))
   {
      // Use just any button to skip performance
      deactivate_music_performance();
      return;
   }


   if (!is_state(STATE_PLAYING_GAME)) return;

   player_control_dashing = true; // Any button activates run

   return;
}

void Screen::joy_button_up_func(ALLEGRO_EVENT* ev)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::joy_button_up_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::joy_button_up_func: error: guard \"initialized\" not met");
   }
   if (!(ev))
   {
      std::stringstream error_message;
      error_message << "[Screen::joy_button_up_func]: error: guard \"ev\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::joy_button_up_func: error: guard \"ev\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::joy_button_up_func]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::joy_button_up_func: error: guard \"event_emitter\" not met");
   }
   int button = ev->joystick.button;

   std::cout << "joy button UP event (" << ev->joystick.id << ")" << std::endl;
   std::cout << "   button: " << ev->joystick.button << std::endl;

   if (!is_state(STATE_PLAYING_GAME)) return;

   player_control_dashing = false; // Any button activates run

   return;
}

void Screen::joy_axis_func(ALLEGRO_EVENT* ev)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::joy_axis_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::joy_axis_func: error: guard \"initialized\" not met");
   }
   if (!(ev))
   {
      std::stringstream error_message;
      error_message << "[Screen::joy_axis_func]: error: guard \"ev\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::joy_axis_func: error: guard \"ev\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::joy_axis_func]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::joy_axis_func: error: guard \"event_emitter\" not met");
   }
   if (!is_state(STATE_PLAYING_GAME)) return;

   //std::cout << "joy event (" << ev->joystick.id << ")" << std::endl;
   //std::cout << "   stick: " << ev->joystick.stick << std::endl;
   //std::cout << "   axis: " << ev->joystick.axis << std::endl;
   //std::cout << "   pos: " << ev->joystick.pos << std::endl;
   float min_stick_threshold = 0.2;

   int stick = ev->joystick.stick;
   int axis = ev->joystick.axis;
   float pos = ev->joystick.pos;
   switch (stick)
   {
      case 0: { // The primary joystick, on the left
        if (axis == 0) // horizontal axis
        {
           if (std::fabs(pos) < min_stick_threshold) player_control_velocity.x = 0;
           else player_control_velocity.x = pos;
        }
        else if (axis == 1) // vertical axis
        {
           if (std::fabs(pos) < min_stick_threshold) player_control_velocity.y = 0;
           else player_control_velocity.y = pos;
        }
      } break;

      case 1: { // The secondary joystick, on the right
      } break;

      case 2: { // The hat, on the left
      } break;
   }

   return;
}

void Screen::virtual_control_button_up_func(AllegroFlare::Player* player, AllegroFlare::VirtualControllers::Base* virtual_controller, int virtual_controller_button_num, bool is_repeat)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::virtual_control_button_up_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::virtual_control_button_up_func: error: guard \"initialized\" not met");
   }
   // TODO: this function
   return;
}

void Screen::virtual_control_button_down_func(AllegroFlare::Player* player, AllegroFlare::VirtualControllers::Base* virtual_controller, int virtual_controller_button_num, bool is_repeat)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::virtual_control_button_down_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::virtual_control_button_down_func: error: guard \"initialized\" not met");
   }
   // TODO: this function
   //call_on_finished_callback_func(); // Consider technique to exit
   return;
}

void Screen::virtual_control_axis_change_func(ALLEGRO_EVENT* ev)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::virtual_control_axis_change_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::virtual_control_axis_change_func: error: guard \"initialized\" not met");
   }
   // TODO: this function
   return;
}

void Screen::activate_music_performance(std::string music_identifier)
{
   if (is_state(STATE_PERFORMING_MUSIC)) return; // TODO: Test this
   // Set our current song state variables
   // TODO: Validate music track exists
   currently_performing_song_identifier = music_identifier;
   currently_performing_song_duration_sec = 15.0; // TODO: Replace this hard-coded value with the actual duration
   //currently_performing_song_duration_sec = 5.0; // TODO: Replace this hard-coded value with the actual duration
                                                 // of the currently_performing_song_identifier

   // Set the player to a good front-facing rotation for performance 
   auto player_entity_as = get_player_controlled_entity_as();
   player_entity_as->get_placement_ref().rotation = { 0.0, -0.25, 0.0 };

   // Set the control text @ bottom of screen to show skippable cutscene controls
   std::vector<std::string> tokens = {
      //"I", "%SPACER", "LABEL>>", "Toggle inventory", 
      //"%SEPARATOR",
      //"X", "%SPACER", "LABEL>>", "Exit", 
      //"%SEPARATOR",
      "X", "%SPACER", "LABEL>>", "Skip", 
      //"%SEPARATOR",
      //"P", "%SPACER", "LABEL>>", "Pause", 
      //"%SEPARATOR",
      //"SHIFT", "%SPACE", "%PLUS", "%SPACE", "ESC", "%SPACER", "LABEL>>", "Exit program", 
   };
   event_emitter->emit_set_input_hints_bar_event(tokens);
   event_emitter->emit_show_input_hints_bar_event();
   event_emitter->emit_set_input_hints_bar_text_opacity_event(1.0);

   // Play the music track
   event_emitter->emit_play_music_track_event(currently_performing_song_identifier);

   // Set the state
   set_state(STATE_PERFORMING_MUSIC);
   return;
}

void Screen::deactivate_music_performance()
{
   if (!is_state(STATE_PERFORMING_MUSIC)) return; // TODO: Test this

   event_emitter->emit_stop_all_music_tracks_event(); // TODO: Consider if alternative would be better than stopping
                                                      // everything.
   currently_performing_song_identifier = "";
   currently_performing_song_duration_sec = 0.0;

   // Hide the input hings bar
   event_emitter->emit_hide_input_hints_bar_event();

   // Emit an event that the package was delivered
   event_emitter->emit_game_event(
      AllegroFlare::GameEvent(
         "package_delivered",
         new AllegroFlare::GameEventDatas::String(current_level_identifier) //"world-1-05")
      )
   );

   set_state(STATE_PLAYING_GAME);
   // TODO: Consider how to re-activate current level music
   return;
}

void Screen::emit_event_to_save_progress()
{
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::emit_event_to_save_progress]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::emit_event_to_save_progress: error: guard \"event_emitter\" not met");
   }
   // TODO: Test this
   // TODO: Consider using a constant rather than "save_progress"
   event_emitter->emit_game_event(AllegroFlare::GameEvent("save_progress"));
   return;
}

void Screen::set_state(uint32_t state, bool override_if_busy)
{
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_state: error: guard \"is_valid_state(state)\" not met");
   }
   if (this->state == state) return;
   if (!override_if_busy && state_is_busy) return;
   uint32_t previous_state = this->state;

   switch (state)
   {
      case STATE_REVEALING: {
         player_control_velocity = {0, 0};
         player_control_dashing = false;
      } break;

      case STATE_PLAYING_GAME: {
         set_primary_camera_to_gameplay_view();
      } break;

      case STATE_SUSPEND_FOR_DIALOG: {
         set_primary_camera_to_dialog_view();
         player_control_velocity = {0, 0};
         player_control_dashing = false;
      } break;

      case STATE_PERFORMING_MUSIC: {
         set_primary_camera_to_music_performance_view();
      } break;

      default:
         throw std::runtime_error("weird error");
      break;
   }

   this->state = state;
   state_changed_at = al_get_time();

   return;
}

float Screen::normalize_age_no_clamp(float start_time, float end_time, float time_now)
{
   // TODO: Consider that capping to 1.0 if past may not be preferred
   float length = (end_time - start_time);
   float time_now_in_range = time_now - start_time;
   if (length == 0.0f) return 0.0f;
   //if (time_now_in_range >= length) return 1.0f;
   return time_now_in_range / length;
}

void Screen::update_state(float time_now)
{
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Screen::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::update_state: error: guard \"is_valid_state(state)\" not met");
   }
   float age = infer_current_state_age(time_now);

   switch (state)
   {
      case STATE_REVEALING: {
      } break;

      case STATE_PLAYING_GAME: {
      } break;

      case STATE_SUSPEND_FOR_DIALOG: {
      } break;

      case STATE_PERFORMING_MUSIC: {
         // TODO: Have camera zoom in slowly
         // If age > song duration, exit
         if (age >= currently_performing_song_duration_sec) deactivate_music_performance();

         // Slowly dolly shot with the camera
         // TODO: Consider doing this with a step-out?
         AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
         float DOLLY_SHOT_DURATION = 12.0;
         float normalized_age = normalize_age_no_clamp(
               state_changed_at,
               state_changed_at + DOLLY_SHOT_DURATION,
               time_now
            );
         if (normalized_age >= 0.0f && normalized_age <= 1.0f)
         {
            float speed_multiplier = 1.0; // - AllegroFlare::interpolator::fast_in(normalized_age);
            if (normalized_age >= 0.5)
            {
               float local = (normalized_age - 0.5) * 2;
               speed_multiplier = 1.0 - AllegroFlare::interpolator::slow_out(local);
            }
            float zoom_speed = 0.001f * speed_multiplier;
            //float zoom_speed = 0.0012f * speed_multiplier;
            primary_camera->zoom += zoom_speed;
         }

         // Rotate character along a sine wave (dancing)
         auto player_entity_as = get_player_controlled_entity_as();
         player_entity_as->get_placement_ref().rotation.z = std::sin(age * 3) * 0.012;
      } break;

      default: {
         throw std::runtime_error("weird error");
      } break;
   }

   return;
}

bool Screen::is_valid_state(uint32_t state)
{
   std::set<uint32_t> valid_states =
   {
      STATE_REVEALING,
      STATE_PLAYING_GAME,
      STATE_SUSPEND_FOR_DIALOG,
      STATE_PERFORMING_MUSIC,
   };
   return (valid_states.count(state) > 0);
}

bool Screen::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float Screen::infer_current_state_age(float time_now)
{
   return (time_now - state_changed_at);
}

ALLEGRO_FONT* Screen::obtain_ui_font()
{
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Screen::obtain_ui_font]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::obtain_ui_font: error: guard \"font_bin\" not met");
   }
   return font_bin->auto_get("Oswald-Medium.ttf -32");
}


} // namespace Gameplay
} // namespace Pipeline


