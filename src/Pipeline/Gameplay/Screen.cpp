

#include <Pipeline/Gameplay/Screen.hpp>

#include <AllegroFlare/ALLEGRO_VERTEX_WITH_NORMAL.hpp>
#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <AllegroFlare/DialogTree/YAMLLoader.hpp>
#include <AllegroFlare/EventNames.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityFactory.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityRenderFlags.hpp>
#include <Pipeline/GameConfigurations/Main.hpp>
#include <Pipeline/Gameplay/Level.hpp>
#include <allegro5/allegro_primitives.h>
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
   , entity_pool()
   , player_controlled_entity(nullptr)
   , player_control_velocity()
   , goal_entity(nullptr)
   , scene_renderer()
   , current_level_identifier("[unset-current_level]")
   , current_level(nullptr)
   , on_finished_callback_func()
   , on_finished_callback_func_user_data(nullptr)
   , initialized(false)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
   , player_is_colliding_on_goal(false)
{
}


Screen::~Screen()
{
}


void Screen::set_game_configuration(AllegroFlare::GameConfigurations::Base* game_configuration)
{
   this->game_configuration = game_configuration;
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
   player_control_velocity = { 0.0f, 0.0f };
   goal_entity = nullptr;


   //
   // Clear our resources so they can be processed from freshly loaded models
   //

   model_bin->clear();
   // TODO: Clear model_bin and the entity_pool
   //entity_pool.clear();


   //
   // Create our entity_factory
   //

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityFactory entity_factory;
   entity_factory.set_bitmap_bin(bitmap_bin);
   entity_factory.set_model_bin(model_bin);


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

      // Find and create the goal
      std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> named_object_vertices =
         world_model->extract_named_object_vertices("goal");
      if (named_object_vertices.empty())
      {
         throw std::runtime_error("Named object \"goal\" appears to not exist.");
      }
      else
      {
         // Just use the first vertex as the position of this named object
         // TODO: Use a central point of the vertices
         AllegroFlare::Vec3D object_position{
            named_object_vertices[0].x,
            named_object_vertices[0].y,
            named_object_vertices[0].z,
         };

         // Create our entity
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *item = 
            new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
         item->set_model_3d(model_bin->auto_get("rounded_unit_cube-01.obj"));
         item->set(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityRenderFlags::RENDER_WITH_SKYBOX);
         item->get_placement_ref().position = object_position;
         item->get_placement_ref().rotation.x = 0.05;
         item->get_placement_ref().rotation.z = 0.03547;
         entity_pool.add(item);

         goal_entity = item;
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




   //
   // Load the dialog bank
   //

   AllegroFlare::DialogTree::NodeBank node_bank;
   std::string dialog_filename = framework->get_data_folder_path() + "/dialogs/all_dialog.yml";
   AllegroFlare::DialogTree::YAMLLoader yaml_loader;
   yaml_loader.load_file(dialog_filename);
   node_bank = yaml_loader.get_node_bank();

   // Validate important dialog nodes exist
   if (!node_bank.node_exists_by_name("package_delivery_response"))
   {
      throw std::runtime_error("Expecting \"package_delivery_response\" dialog node to exist but it does not.");
   }


   framework->set_dialog_system_dialog_node_bank(node_bank);



   //
   // Assign our "special" items
   //

   player_controlled_entity = player_character;
   //goal_entity = item;
   player_is_colliding_on_goal = false; // This needs to be changed to an "enter" collision e.g. "exit" collision



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
   //emit_event_to_update_input_hints_bar();
   //emit_show_and_size_input_hints_bar_event();
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
   //emit_hide_and_restore_size_input_hints_bar_event();
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

void Screen::on_player_entity_collide(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* colliding_entity)
{
   if (!is_state(STATE_PLAYING_GAME)) return;

   if (colliding_entity == goal_entity)
   {
      // Handle goal collision
      //player_is_colliding_on_goal = true;
      set_state(STATE_SUSPEND_FOR_DIALOG);
      event_emitter->emit_activate_dialog_node_by_name_event("package_delivery_response");
      //call_on_finished_callback_func();
   }
   return;
}

void Screen::update()
{
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
      // Translate the player control angles to be relative to the camera
      float angle = primary_camera->spin;
      float x_prime = player_control_velocity.x * cos(angle) - player_control_velocity.y * sin(angle);
      float y_prime = player_control_velocity.x * sin(angle) + player_control_velocity.y * cos(angle);

      // Move the player
      auto player_entity_as = get_player_controlled_entity_as();
      player_entity_as->get_placement_ref().position.x += x_prime;
      player_entity_as->get_placement_ref().position.z += y_prime;

      // Update the player model rotation to face the moving direction
      if (fabs(x_prime) + fabs(y_prime) > 0.001) // Only update the rotation if the player is moving
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
            float target = fmod(target_angle_in_units, 1.0f);
            float current = fmod(current_angle_in_units, 1.0f);
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
            if (!player_is_colliding_on_goal) on_player_entity_collide(goal_entity_as);
            player_is_colliding_on_goal = true;
         }
         else
         {
            player_is_colliding_on_goal = false;
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

   bool save_bitmaps = false;
   if (save_bitmaps)
   {
      al_save_bitmap(
         "/Users/markoates/Desktop/shadow_buffer_bitmap.png",
         scene_renderer.get_shadow_map_buffer_ref().get_result_bitmap()
      );
      //shadow_map_buffer.get_result_bitmap();
      al_save_bitmap(
         "/Users/markoates/Desktop/render_surface.png",
         render_surface
      );
   }

   al_set_target_bitmap(initial_target_bitmap);
   al_draw_bitmap(render_surface, 0, 0, 0);
   //al_draw_filled_rectangle(0, 0, 300, 300, ALLEGRO_COLOR{1, 0, 0, 1});
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
   // game_configuration->handle_game_event(game_event);
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
   if (!is_state(STATE_PLAYING_GAME)) return;

   float player_velocity = 0.04;
   switch(ev->keyboard.keycode)
   {
      case ALLEGRO_KEY_UP: {
         player_control_velocity.y = -player_velocity;
         //move_development_cursor_up();
      } break;

      case ALLEGRO_KEY_DOWN: {
         player_control_velocity.y = player_velocity;
         //move_development_cursor_down();
      } break;

      case ALLEGRO_KEY_LEFT: {
         player_control_velocity.x = -player_velocity;
         //move_development_cursor_down();
      } break;

      case ALLEGRO_KEY_RIGHT: {
         player_control_velocity.x = player_velocity;
         //move_development_cursor_down();
      } break;

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
      } break;

      case STATE_PLAYING_GAME: {
         set_primary_camera_to_gameplay_view();
      } break;

      case STATE_SUSPEND_FOR_DIALOG: {
         set_primary_camera_to_dialog_view();
         player_control_velocity = {0, 0};
      } break;

      default:
         throw std::runtime_error("weird error");
      break;
   }

   this->state = state;
   state_changed_at = al_get_time();

   return;
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
      case STATE_REVEALING:
      break;

      case STATE_PLAYING_GAME:
      break;

      case STATE_SUSPEND_FOR_DIALOG:
      break;

      default:
         throw std::runtime_error("weird error");
      break;
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


} // namespace Gameplay
} // namespace Pipeline


