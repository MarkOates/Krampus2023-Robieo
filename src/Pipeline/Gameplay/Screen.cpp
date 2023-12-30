

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
#include <AllegroFlare/Physics/AABB3D.hpp>
#include <LabyrinthOfLore/Physics/EntityTileMapCollisionStepper.hpp>
#include <LabyrinthOfLore/WorldMap/BasicRenderer.hpp>
#include <LabyrinthOfLore/WorldMap/MultiBitmapFilenameToWorldBuilder.hpp>
#include <LabyrinthOfLore/WorldMap/TileTypeEnum.hpp>
#include <Pipeline/CSVToLevelLoader.hpp>
#include <Pipeline/CurrentFloorInferencer.hpp>
#include <Pipeline/DialogNodeBankFactory.hpp>
#include <Pipeline/GameConfigurations/Main.hpp>
#include <Pipeline/Gameplay/Level.hpp>
#include <Pipeline/OBJWorldLoader.hpp>
#include <Pipeline/Physics/GravityStepper.hpp>
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
   , Pipeline::EntityAttributes()
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
   , level_camera_zones({})
   , level_switch_plate_zones({})
   , level_switch_plate_zones_player_is_currently_colliding_with({})
   , boss_switch_plate_zones({})
   , boss_switch_plate_zones_player_is_currently_colliding_with({})
   , goal_entity(nullptr)
   , exit_entity(nullptr)
   , scene_renderer()
   , show_map_overlay(false)
   , smooth_camera_from({})
   , smooth_camera_to({})
   , smooth_camera_started_at(0.0f)
   , smooth_camera_duration(2.5)
   , smooth_camera_is_active(false)
   , smooth_camera({})
   , current_level_identifier("[unset-current_level]")
   , current_level(nullptr)
   , current_level_tile_maps({})
   , current_level_tile_map_tile_alignment_offset({ 0.5, -0.5 })
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
   , king_turret_health(8)
   , king_turret_health_max(8)
   , king_turret_is_defeated(false)
   , king_turret_boss_mode_is_active(false)
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


std::string Screen::obtain_data_folder()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::obtain_data_folder]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::obtain_data_folder: error: guard \"initialized\" not met");
   }
   if (!(framework))
   {
      std::stringstream error_message;
      error_message << "[Screen::obtain_data_folder]: error: guard \"framework\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::obtain_data_folder: error: guard \"framework\" not met");
   }
   return framework->get_data_folder_path();
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

std::map<int, float> Screen::build_elevations_and_indices_for_floors()
{
   if (!(current_level))
   {
      std::stringstream error_message;
      error_message << "[Screen::build_elevations_and_indices_for_floors]: error: guard \"current_level\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::build_elevations_and_indices_for_floors: error: guard \"current_level\" not met");
   }
   std::map<int, float> result;
   int i=0;
   // TODO: Confirm not duplicating levels
   //std::cout << "========" << std::endl;
   for (auto &tile_map : current_level->get_tile_maps_ref())
   {
      result[i] = tile_map.get_groundlevel_height();
      //std::cout << i << ":" << tile_map.get_groundlevel_height() << std::endl;
      i++;
   }
   // Sanity check
   if (result.size() != current_level->get_tile_maps_ref().size())
   {
      throw std::runtime_error("Pipeline::Gameplay::Screen::build_elevations_and_for_floors: error: result size "
              "doesn't match source size");
   }
   return result;
}

bool Screen::trivial_collide(AllegroFlare::Vec3D p1, AllegroFlare::Vec3D p2, float min_distance)
{
   float squared_distance = (p1.x - p2.x) * (p1.x - p2.x)
                          + (p1.y - p2.y) * (p1.y - p2.y)
                          + (p1.z - p2.z) * (p1.z - p2.z);
   float min_distance_squared = min_distance * min_distance;
   return squared_distance < min_distance_squared;
}

void Screen::set_camera_to_gameplay_view(AllegroFlare::Camera3D* primary_camera)
{
   //AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 0.0, 18.0 };
   primary_camera->spin = 0.5;
   primary_camera->tilt = 0.75;
   primary_camera->zoom = 3.0;
   return;
}

void Screen::set_camera_to_dialog_view(AllegroFlare::Camera3D* primary_camera)
{
   //AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 0.0, 10.0 };
   primary_camera->spin = 0.5 - 0.2;
   primary_camera->tilt = 0.75 - 0.4;
   primary_camera->zoom = 2.8;
   return;
}

void Screen::set_camera_to_music_performance_view(AllegroFlare::Camera3D* primary_camera)
{
   //AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 0.25, 6.0 };
   primary_camera->spin = 0.2;
   primary_camera->tilt = 0.2;
   primary_camera->zoom = 3.1;
   return;
}

void Screen::set_camera_to_custom_view_1(AllegroFlare::Camera3D* primary_camera)
{
   //AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 2.4, 20.0 };
   primary_camera->spin = 1.3;
   primary_camera->tilt = 0.3;
   primary_camera->zoom = 3.6;
   return;
}

void Screen::set_camera_to_custom_view_2(AllegroFlare::Camera3D* primary_camera)
{
   //AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 1.2, 9.0 };
   primary_camera->spin = 0.0;
   primary_camera->tilt = 0.21;
   primary_camera->zoom = 2.6;
   return;
}

void Screen::set_camera_to_custom_view_3(AllegroFlare::Camera3D* primary_camera)
{
   //AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 0.0, 18.0 };
   primary_camera->spin = 0.0;
   primary_camera->tilt = 1.12;
   primary_camera->zoom = 3.0;
   return;
}

void Screen::set_camera_to_custom_view_4(AllegroFlare::Camera3D* primary_camera)
{
   //AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 2.0, 20.0 };
   primary_camera->spin = 0.5;
   primary_camera->tilt = 0.75;
   primary_camera->zoom = 2.85;
   return;
}

void Screen::set_camera_to_custom_view_5(AllegroFlare::Camera3D* primary_camera)
{
   //AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   primary_camera->stepout = { 0.0, 2.6, 26.0 };
   primary_camera->spin = 0.5;
   primary_camera->tilt = 0.75;
   primary_camera->zoom = 3.2;
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

Pipeline::Gameplay::Level Screen::build_level(std::string level_identifier)
{
   ///*
   std::string data_folder = obtain_data_folder();
   std::string levels_csv_file = data_folder + "/levels/universe.csv";

   bool csv_file_exists = std::filesystem::exists(levels_csv_file);
   if (!csv_file_exists)
   {
      AllegroFlare::Logger::throw_error(
         "Pipeline::Gameplay::Screen::build_level",
         "The CSV file \"" + levels_csv_file + "\" does not exist."
      );
   }

   Pipeline::CSVToLevelLoader csv_level_loader;
   csv_level_loader.set_csv_full_path(levels_csv_file);
   csv_level_loader.load();

   bool level_exists = csv_level_loader.level_exists(level_identifier);
   if (!level_exists)
   {
      AllegroFlare::Logger::throw_error(
         "Pipeline::Gameplay::Screen::build_level",
         "Level with the identifier \"" + level_identifier + "\" does not exist."
      );
   }
   //*/

   return csv_level_loader.find_level(level_identifier);
}

LabyrinthOfLore::WorldMap::TileMap* Screen::get_current_level_tile_map(int floor_index)
{
   if (!((!current_level_tile_maps.empty())))
   {
      std::stringstream error_message;
      error_message << "[Screen::get_current_level_tile_map]: error: guard \"(!current_level_tile_maps.empty())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::get_current_level_tile_map: error: guard \"(!current_level_tile_maps.empty())\" not met");
   }
   if (!((floor_index >= 0)))
   {
      std::stringstream error_message;
      error_message << "[Screen::get_current_level_tile_map]: error: guard \"(floor_index >= 0)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::get_current_level_tile_map: error: guard \"(floor_index >= 0)\" not met");
   }
   if (!((floor_index < current_level_tile_maps.size())))
   {
      std::stringstream error_message;
      error_message << "[Screen::get_current_level_tile_map]: error: guard \"(floor_index < current_level_tile_maps.size())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::get_current_level_tile_map: error: guard \"(floor_index < current_level_tile_maps.size())\" not met");
   }
   return current_level_tile_maps[floor_index]; // TODO: Grab the level nearest the player (or y-position)
}

void Screen::write_tile_elevation_value(int floor_index, int tile_x, int tile_y, float elevation)
{
   if (!((!current_level_tile_maps.empty())))
   {
      std::stringstream error_message;
      error_message << "[Screen::write_tile_elevation_value]: error: guard \"(!current_level_tile_maps.empty())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::write_tile_elevation_value: error: guard \"(!current_level_tile_maps.empty())\" not met");
   }
   if (!((floor_index >= 0)))
   {
      std::stringstream error_message;
      error_message << "[Screen::write_tile_elevation_value]: error: guard \"(floor_index >= 0)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::write_tile_elevation_value: error: guard \"(floor_index >= 0)\" not met");
   }
   if (!((floor_index < current_level_tile_maps.size())))
   {
      std::stringstream error_message;
      error_message << "[Screen::write_tile_elevation_value]: error: guard \"(floor_index < current_level_tile_maps.size())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::write_tile_elevation_value: error: guard \"(floor_index < current_level_tile_maps.size())\" not met");
   }
   // HERE
   LabyrinthOfLore::WorldMap::Tile t = current_level_tile_maps[floor_index]->get_tile(tile_x, tile_y);
   t.set_height(elevation);
   current_level_tile_maps[floor_index]->set_tile(tile_x, tile_y, t);
}

void Screen::load_tile_map(std::string level_identifier)
{
   for (auto &current_level_tile_map : current_level_tile_maps)
   {
      if (current_level_tile_map) delete current_level_tile_map;
   }

   current_level_tile_maps.clear();

   //int num_levels = 1;
   //for (int i=0; i<num_levels; i++)
   //{
      current_level_tile_maps = load_tile_map_from_bitmap(level_identifier); // TODO: Load each level
      //current_level_tile_maps.push_back(load_tile_map_from_bitmap(level_identifier)); // TODO: Load each level
   //}

   return;
}

std::vector<LabyrinthOfLore::WorldMap::TileMap*> Screen::load_tile_map_from_bitmap(std::string level_identifier)
{
   // TODO: Don't pass along floor, instead do better thing

   std::string tile_map_bitmap_folder_location = bitmap_bin->get_path();
   std::vector<LabyrinthOfLore::WorldMap::TileMap*> result; // = new LabyrinthOfLore::WorldMap::TileMap();

   // Create some level data

   Pipeline::Gameplay::Level level = build_level(level_identifier);


   for (auto &tile_map : level.get_tile_maps())
   {
      LabyrinthOfLore::WorldMap::TileMap *local_result = new LabyrinthOfLore::WorldMap::TileMap();
      std::string tile_elevation_bitmap = tile_map.get_tile_elevation_bitmap_filename();
      std::string tile_type_bitmap = tile_map.get_tile_type_bitmap_filename();

      //bool has_bitmap_files_for_loading = !(level.get_tile_map_tile_elevation_bitmap_filename().empty()
                                        //&& level.get_tile_map_tile_type_bitmap_filename().empty());
      bool has_bitmap_files_for_loading = !(tile_elevation_bitmap.empty() && tile_type_bitmap.empty());

      if (has_bitmap_files_for_loading)
      {
         // Load the level (with bitmap files)

         std::string elevation_bitmap_filename = tile_map_bitmap_folder_location
                                               + tile_elevation_bitmap; //level.get_tile_map_tile_elevation_bitmap_filename();
         std::string tile_type_bitmap_filename = tile_map_bitmap_folder_location
                                               + tile_type_bitmap; //level.get_tile_map_tile_type_bitmap_filename();
         //float ceiling_height = level.get_tile_map_ceiling_height();
         //float ground_height = level.get_tile_map_groundlevel_height();
         //float floor_height = level.get_tile_map_floor_height();
         float ceiling_height = tile_map.get_ceiling_height();
         float ground_height = tile_map.get_groundlevel_height();
         float floor_height = tile_map.get_floor_height();


         std::cout << "-- elevation_bitmap_filename: " << elevation_bitmap_filename << std::endl;
         std::cout << "-- tile_type_bitmap_filename: " << tile_type_bitmap_filename << std::endl;

         if (!std::filesystem::exists(elevation_bitmap_filename))
         {
            AllegroFlare::Logger::throw_error(
               "Pipeline::Gameplay::Screen::load_tile_map_from_bitmap",
               "The tile_elevation bitmap \"" + elevation_bitmap_filename + "\" does not exist."
            );
         }
         if (!std::filesystem::exists(tile_type_bitmap_filename))
         {
            AllegroFlare::Logger::throw_error(
               "Pipeline::Gameplay::Screen::load_tile_map_from_bitmap",
               "The tile_type bitmap \"" + tile_type_bitmap_filename + "\" does not exist."
            );
         }

         LabyrinthOfLore::WorldMap::MultiBitmapFilenameToWorldBuilder world_builder(
            elevation_bitmap_filename,
            tile_type_bitmap_filename,
            ceiling_height,
            ground_height,
            floor_height
         );

         *local_result = world_builder.build();

         result.push_back(local_result);
      }
      else
      {
         // No bitmap data is available for this level
         AllegroFlare::Logger::warn_from(
            "Pipeline::Gameplay::Screen::load_tile_map_from_bitamp",
            "Level bitmaps are not used for the level with identifier \"" + level_identifier + "\""
               ". Proceeding with an empty tile_map for collisions."
         );
      }
   }

   return result;
}

LabyrinthOfLore::WorldMap::TileMap* Screen::load_tester_tile_map()
{
   //if (current_level_tile_map) delete current_level_tile_map;

   LabyrinthOfLore::WorldMap::TileMap *result_tile_map = new LabyrinthOfLore::WorldMap::TileMap();

   result_tile_map->resize(
      16,
      16,
      LabyrinthOfLore::WorldMap::Tile(
         LabyrinthOfLore::WorldMap::NORMAL_GROUND_TILE,
         0.0f
      )
   );

   // A pillar too high
   result_tile_map->set_tile(
      1, 1, LabyrinthOfLore::WorldMap::Tile(LabyrinthOfLore::WorldMap::NORMAL_GROUND_TILE, 2.0f));

   // A sequence of steps
   result_tile_map->set_tile(
      3, 1, LabyrinthOfLore::WorldMap::Tile(LabyrinthOfLore::WorldMap::NORMAL_GROUND_TILE, 0.2f));
   result_tile_map->set_tile(
      4, 1, LabyrinthOfLore::WorldMap::Tile(LabyrinthOfLore::WorldMap::NORMAL_GROUND_TILE, 0.4f));
   result_tile_map->set_tile(
      5, 1, LabyrinthOfLore::WorldMap::Tile(LabyrinthOfLore::WorldMap::NORMAL_GROUND_TILE, 0.6f));
   result_tile_map->set_tile(
      6, 1, LabyrinthOfLore::WorldMap::Tile(LabyrinthOfLore::WorldMap::NORMAL_GROUND_TILE, 0.8f));
   result_tile_map->set_tile(
      7, 1, LabyrinthOfLore::WorldMap::Tile(LabyrinthOfLore::WorldMap::NORMAL_GROUND_TILE, 1.0f));

   //current_level_tile_map = result_tile_map;

   return result_tile_map;
}

void Screen::create_plate_switch(std::string name, AllegroFlare::Vec3D position)
{
   // Create our renderable entity
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *item = 
         new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
      item->set_model_3d(model_bin->auto_get("switch_3x3-off-02.obj"));
      item->set_model_3d_texture(bitmap_bin->auto_get("switch_3x3-off-02.png"));
      item->get_placement_ref().position = position + AllegroFlare::Vec3D(0.0f, 0.11f, 0.0f);
      entity_pool.add(item);

      // Define our collision zone's bounding box
      AllegroFlare::Physics::AABB3D bounding_box;
      bounding_box.set_min(position + AllegroFlare::Vec3D(-1.5, -4, -1.5));
      bounding_box.set_max(position + AllegroFlare::Vec3D(1.5, 4, 1.5));

      // Build our switch plate zone
      Pipeline::Gameplay::LevelSwitchPlateZone level_switch_plate_zone;
      level_switch_plate_zone.set_name(name); //"dune_main_switch");
      level_switch_plate_zone.set_switch_entity(item);
      level_switch_plate_zone.set_is_activated(false);
      level_switch_plate_zone.set_bounding_box(bounding_box);

      level_switch_plate_zones.push_back(level_switch_plate_zone);
   return;
}

void Screen::create_boss_plate_switch(std::string name, AllegroFlare::Vec3D position)
{
   // Create our renderable entity
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *item = 
         new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
      item->set_model_3d(model_bin->auto_get("boss_switch_3x3-off-02.obj"));
      item->set_model_3d_texture(bitmap_bin->auto_get("boss_switch_3x3-off-02.png"));
      item->get_placement_ref().position = position + AllegroFlare::Vec3D(0.0f, 0.11f, 0.0f);
      entity_pool.add(item);

      // Define our collision zone's bounding box
      AllegroFlare::Physics::AABB3D bounding_box;
      bounding_box.set_min(position + AllegroFlare::Vec3D(-0.5, -4, -0.5));
      bounding_box.set_max(position + AllegroFlare::Vec3D(0.5, 4, 0.5));

      // Build our switch plate zone
      Pipeline::Gameplay::LevelSwitchPlateZone boss_switch_plate_zone;
      boss_switch_plate_zone.set_name(name); //"dune_main_switch");
      boss_switch_plate_zone.set_switch_entity(item);
      boss_switch_plate_zone.set_is_activated(false);
      boss_switch_plate_zone.set_bounding_box(bounding_box);

      boss_switch_plate_zones.push_back(boss_switch_plate_zone);
   return;
}

void Screen::spawn_king_turret(AllegroFlare::Vec3D position)
{
   std::string object_name = "king_turret";
   std::string model_name = "king-turret-02.obj";
   std::string texture_name = "king-turret-02.png";

    AllegroFlare::Vec3D object_position = position;

    AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *object = 
       new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
    object->set_model_3d(model_bin->auto_get(model_name));
    object->set_model_3d_texture(bitmap_bin->auto_get(texture_name));
    object->get_placement_ref().position = object_position + AllegroFlare::Vec3D(0, 3, 0);
    object->get_placement_ref().scale = { 4.0, 4.0, 4.0 };

    object->set(ATTRIBUTE_IS_KING_TURRET);

    entity_pool.add(object);
}

void Screen::spawn_real_time_gem(AllegroFlare::Vec3D position)
{
   std::string object_name = "gem";
   std::string model_name = "gem-01.obj";
      std::string texture_name = "gem-01.png";

          AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *object =
             new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
          object->set_model_3d(model_bin->auto_get(model_name));
          object->set_model_3d_texture(bitmap_bin->auto_get(texture_name));
          object->get_placement_ref().position = position;
          object->get_placement_ref().scale = { 0.2, 0.2, 0.2 };
          //object->get_placement_ref().rotation.y = 0.01;
          object->get_velocity_ref().rotation.y = 0.005;

          object->set(ATTRIBUTE_COLLIDABLE_BY_PLAYER);
          object->set(ATTRIBUTE_ITEM_TYPE, "gem");
          object->set(ATTRIBUTE_ITEM_PICKUP_SOUND, "gem_chime");
          object->set(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityRenderFlags::RENDER_WITH_SKYBOX);

          //env->get_placement_ref().position.y = 0.0; // NOTE: The objects will always be placed at 0
          entity_pool.add(object);
   // Create our renderable entity
      //AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *item = 
         //new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
      //item->set_model_3d(model_bin->auto_get("switch_3x3-off-02.obj"));
      //item->set_model_3d_texture(bitmap_bin->auto_get("switch_3x3-off-02.png"));
      //item->get_placement_ref().position = position + AllegroFlare::Vec3D(0.0f, 0.1f, 0.0f);
      //entity_pool.add(item);

      // Define our collision zone's bounding box
      //AllegroFlare::Physics::AABB3D bounding_box;
      //bounding_box.set_min(position + AllegroFlare::Vec3D(-1.5, -4, -1.5));
      //bounding_box.set_max(position + AllegroFlare::Vec3D(1.5, 4, 1.5));

      // Build our switch plate zone
      //Pipeline::Gameplay::LevelSwitchPlateZone level_switch_plate_zone;
      //level_switch_plate_zone.set_name(name); //"dune_main_switch");
      //level_switch_plate_zone.set_switch_entity(item);
      //level_switch_plate_zone.set_is_activated(false);
      //level_switch_plate_zone.set_bounding_box(bounding_box);

      //level_switch_plate_zones.push_back(level_switch_plate_zone);
   return;
}

void Screen::spawn_final_platform()
{
   std::string object_name = "final_platform";
   std::string model_name = "final-platform-02.obj";
      std::string texture_name = "final-platform-02.png";

          AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *object =
             new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
          object->set_model_3d(model_bin->auto_get(model_name));
          object->set_model_3d_texture(bitmap_bin->auto_get(texture_name));
          object->get_placement_ref().position = AllegroFlare::Vec3D(37, 2, -22);
          //object->get_placement_ref().scale = { 0.2, 0.2, 0.2 };
          //object->get_placement_ref().rotation.y = 0.01;
          //object->get_velocity_ref().rotation.y = 0.005;

          object->set("final_platform");
          //object->set(ATTRIBUTE_ITEM_TYPE, "gem");
          //object->set(ATTRIBUTE_ITEM_PICKUP_SOUND, "gem_chime");
          //object->set(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityRenderFlags::RENDER_WITH_SKYBOX);

          //env->get_placement_ref().position.y = 0.0; // NOTE: The objects will always be placed at 0
          entity_pool.add(object);
   // Create our renderable entity
      //AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *item = 
         //new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
      //item->set_model_3d(model_bin->auto_get("switch_3x3-off-02.obj"));
      //item->set_model_3d_texture(bitmap_bin->auto_get("switch_3x3-off-02.png"));
      //item->get_placement_ref().position = position + AllegroFlare::Vec3D(0.0f, 0.1f, 0.0f);
      //entity_pool.add(item);

      // Define our collision zone's bounding box
      //AllegroFlare::Physics::AABB3D bounding_box;
      //bounding_box.set_min(position + AllegroFlare::Vec3D(-1.5, -4, -1.5));
      //bounding_box.set_max(position + AllegroFlare::Vec3D(1.5, 4, 1.5));

      // Build our switch plate zone
      //Pipeline::Gameplay::LevelSwitchPlateZone level_switch_plate_zone;
      //level_switch_plate_zone.set_name(name); //"dune_main_switch");
      //level_switch_plate_zone.set_switch_entity(item);
      //level_switch_plate_zone.set_is_activated(false);
      //level_switch_plate_zone.set_bounding_box(bounding_box);

      //level_switch_plate_zones.push_back(level_switch_plate_zone);
   return;
}

void Screen::add_additional_entities_based_on_level_identifier(std::string level_identifier)
{
   if (level_identifier == "5-dune-temple")
   {
      //AllegroFlare::Vec3D position = { 0, 0.1, -3 };
      create_plate_switch("dune_main_switch2", { 0, 0, -3 });
      create_plate_switch("dune_main_switch", { -15, 0, -3 });
      create_plate_switch("statue_switch", { -27, 0, -16.5 });
      create_plate_switch("right_switch", { 28.5, 0, -1.5 });

      create_plate_switch("further_left_switch", { -5.5, 0, -40.5 });
      create_plate_switch("further_right_switch", { 5.5, 0, -40.5 });
   }
   else if (level_identifier == "8-turret-palace")
   {
      spawn_king_turret(AllegroFlare::Vec3D(37.5f, 2.0f, -24.0f));
      //spawn_king_turret(AllegroFlare::Vec3D(37.5f, 2.0f, -24.0f));
      create_boss_plate_switch("boss_switch_1", AllegroFlare::Vec3D(36.0f, 2.0f, -20.0f));
      create_boss_plate_switch("boss_switch_2", AllegroFlare::Vec3D(37.5f, 2.0f, -20.0f));
      create_boss_plate_switch("boss_switch_3", AllegroFlare::Vec3D(39.0f, 2.0f, -20.0f));
    //parameters:
   //- name: name
     //type: std::string
     //default_argument: '"[unset-name]"'
   //- name: position
     //type: AllegroFlare::Vec3D
     //default_argument: '{}'
   }
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
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Screen::load_level_by_identifier]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::load_level_by_identifier: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Screen::load_level_by_identifier]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::load_level_by_identifier: error: guard \"bitmap_bin\" not met");
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
   if (current_level) delete current_level;
   current_level = nullptr;
   for (auto &current_level_tile_map : current_level_tile_maps)
   {
      if (current_level_tile_map) delete current_level_tile_map;
   }
   current_level_tile_maps.clear();
   //if (current_level_tile_map) delete current_level_tile_map;
   //current_level_tile_map = nullptr;
   show_map_overlay = false;
   level_camera_zones.clear();
   level_switch_plate_zones.clear();
   level_switch_plate_zones_player_is_currently_colliding_with.clear();
   boss_switch_plate_zones.clear();
   boss_switch_plate_zones_player_is_currently_colliding_with.clear();


   // clear the king turret stats
   king_turret_health_max = 8;
   king_turret_health = king_turret_health_max;
   king_turret_is_defeated = false;
   king_turret_boss_mode_is_active = false;


   //
   // Find the Level record matching this identifier
   //

   //level = new Pipeline::Gameplay::Level();
   Pipeline::Gameplay::Level level = build_level(level_identifier);



   //*current_level = level;


   //level.set_tile_map_tile_elevation_bitmap_filename("the_cave.png");
   //level.set_tile_map_tile_type_bitmap_filename("the_cave-type.png");
   //level.set_tile_map_ceiling_height(10.0f);
   //level.set_tile_map_groundlevel_height(0.0f);
   //level.set_tile_map_floor_height(-2.0f);
   //level.set_tile_map_origin_offset({22, 25});




   //std::string world_model_name = level_identifier; //"world-1-01";
   std::string world_model_obj_name = level.get_world_model_obj_filename(); //world_model_name + ".obj";
   std::string world_model_texture_name = level.get_world_model_texture_filename(); //world_model_name + ".png";


   std::string background_music_identifier = level.get_background_music_identifier();


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
   // level_identifier
   current_level_identifier = level_identifier;

   // Begin OBJ World loader
   Pipeline::OBJWorldLoader obj_world_loader;
   obj_world_loader.set_bitmap_bin(bitmap_bin);
   obj_world_loader.set_model_bin(model_bin);
   obj_world_loader.set_world_model_obj_name(level.get_world_model_obj_filename());
   obj_world_loader.set_world_model_texture_name(level.get_world_model_texture_filename());
   entity_pool = obj_world_loader.load();

   goal_entity = obj_world_loader.get_goal_entity();
   exit_entity = obj_world_loader.get_exit_entity();
   //player_character = obj_world_loader.get_player_character();
   portal_entity_associations = obj_world_loader.get_portal_entity_associations();
   player_controlled_entity = obj_world_loader.get_player_character();
   level_camera_zones = obj_world_loader.get_level_camera_zones();

   add_additional_entities_based_on_level_identifier(level_identifier);



   //
   // Build the tile map (for physics)
   //

   load_tile_map(level_identifier);

   //
   // Set the current level
   //
   current_level = new Pipeline::Gameplay::Level();
   *current_level = level;

   current_level_identifier = level_identifier;




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
   //current_level_song_to_perform_identifier = "robot-holly_jolly";


   //
   // Assign our "special" items
   //

   //player_controlled_entity = obj_world_loader.get_player_character();
   //goal_entity = item;
   player_is_colliding_on_goal = false; // This needs to be changed to an "enter" collision e.g. "exit" collision
   player_is_colliding_on_exit = false; // This needs to be changed to an "enter" collision e.g. "exit" collision


   //
   // Set our initial positions
   //

   set_camera_to_gameplay_view(scene_renderer.find_primary_camera_3d());
                                          // This will be our default initialization position, and could be modified
                                          // in the next step by starting the game


   //
   // Start the game
   //

   set_state(STATE_PLAYING_GAME);



    //
    // Play the music!

    event_emitter->emit_play_music_track_event(background_music_identifier);



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
   //scene_renderer.setup_cubemapping(bitmap_bin->get_path() + "black_prism_1-01.png");
   scene_renderer.setup_cubemapping(bitmap_bin->get_path() + "green_prism_1-01.jpg");
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
   else if (colliding_entity->exists(ATTRIBUTE_ITEM_TYPE, "gem"))
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
      game_progress_and_state_info->add_item_to_inventory("gem");

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

Pipeline::Gameplay::LevelCameraZone* Screen::find_first_camera_zone_at(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* player_controlled_entity_as)
{
   if (!(player_controlled_entity_as))
   {
      std::stringstream error_message;
      error_message << "[Screen::find_first_camera_zone_at]: error: guard \"player_controlled_entity_as\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::find_first_camera_zone_at: error: guard \"player_controlled_entity_as\" not met");
   }
   Pipeline::Gameplay::LevelCameraZone *result = nullptr;
   AllegroFlare::Vec3D player_character_position = player_controlled_entity_as->get_placement_ref().position;

   int i=0;
   for (auto &level_camera_zone : level_camera_zones)
   {
      auto camera_zone_bounding_box = level_camera_zone.get_bounding_box_ref();
      if (level_camera_zone.get_bounding_box_ref().collides_with_point(player_character_position))
      {
         return &level_camera_zone;
      }
   }

   return result;
}

Pipeline::Gameplay::LevelSwitchPlateZone* Screen::find_first_switch_plate_zone_at__DEAD_METHOD(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* player_controlled_entity_as)
{
   if (!(player_controlled_entity_as))
   {
      std::stringstream error_message;
      error_message << "[Screen::find_first_switch_plate_zone_at__DEAD_METHOD]: error: guard \"player_controlled_entity_as\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::find_first_switch_plate_zone_at__DEAD_METHOD: error: guard \"player_controlled_entity_as\" not met");
   }
   Pipeline::Gameplay::LevelSwitchPlateZone *result = nullptr;
   AllegroFlare::Vec3D player_character_position = player_controlled_entity_as->get_placement_ref().position;

   int i=0;
   for (auto &level_switch_plate_zone : level_switch_plate_zones)
   {
      auto switch_plate_zone_bounding_box = level_switch_plate_zone.get_bounding_box_ref();
      if (level_switch_plate_zone.get_bounding_box_ref().collides_with_point(player_character_position))
      {
         return &level_switch_plate_zone;
      }
   }

   return result;
}

void Screen::start_smooth_camera_movement(float time_now)
{
   smooth_camera_started_at = time_now;
   smooth_camera_is_active = true;
   return;
}

void Screen::handle_on_enter_with_switch(Pipeline::Gameplay::LevelSwitchPlateZone* switch_plate_zone)
{
   if (!(switch_plate_zone))
   {
      std::stringstream error_message;
      error_message << "[Screen::handle_on_enter_with_switch]: error: guard \"switch_plate_zone\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::handle_on_enter_with_switch: error: guard \"switch_plate_zone\" not met");
   }
   if (switch_plate_zone->get_is_activated()) return;

   event_emitter->emit_play_sound_effect_event("massive_switch_on");

   // Modify the switch_entity in the entity pool to appear as "active"
   auto switch_entity = switch_plate_zone->get_switch_entity();
   switch_entity->set_model_3d(model_bin->auto_get("switch_3x3-on-02.obj"));
   switch_entity->set_model_3d_texture(bitmap_bin->auto_get("switch_3x3-on-02.png"));
   switch_entity->get_placement_ref().position.y -= 0.1; // Move the switch "down"

   AllegroFlare::Vec3D gem_center_place = switch_entity->get_placement_ref().position;
   gem_center_place.y = 0.0f;
   float distance = 2.5;

   // create some gems in the scene
   // HERE
   //float
   spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(-distance, 0, 0));
   spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(distance, 0, 0));
   spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(0, 0, -distance));
   spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(0, 0, distance));
   spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(-distance, 0, distance));
   spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(distance, 0, -distance));
   spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(-distance, 0, -distance));
   spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(distance, 0, distance));

   // Set this switch_plate_zone to active
   switch_plate_zone->set_is_activated(true);

   return;
}

void Screen::handle_on_exit_with_switch(std::string switch_name)
{
   //event_emitter->emit_play_sound_effect_event("massive_switch_off");

   //// Modify the switch_entity in the entity pool to appear as "active"
   //auto switch_entity = switch_plate_zone->get_switch_entity();
   //switch_entity->set_model_3d(model_bin->auto_get("switch_3x3-on-02.obj"));
   //switch_entity->set_model_3d_texture(bitmap_bin->auto_get("switch_3x3-on-02.png"));
   //switch_entity->get_placement_ref().position.y -= 0.1; // Move the switch "down"

   // Set this switch_plate_zone to active
   //switch_plate_zone->set_is_activated(true);
   return;
}

void Screen::handle_on_stay_on_switch(std::string switch_name)
{
   return;
}

void Screen::handle_on_stay_off_switch(std::string switch_name)
{
   return;
}

void Screen::handle_on_enter_with_boss_switch(Pipeline::Gameplay::LevelSwitchPlateZone* switch_plate_zone)
{
   if (!(switch_plate_zone))
   {
      std::stringstream error_message;
      error_message << "[Screen::handle_on_enter_with_boss_switch]: error: guard \"switch_plate_zone\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::handle_on_enter_with_boss_switch: error: guard \"switch_plate_zone\" not met");
   }
   if (switch_plate_zone->get_is_activated()) return;

   event_emitter->emit_play_sound_effect_event("massive_switch_on");

   // Modify the switch_entity in the entity pool to appear as "active"
   auto switch_entity = switch_plate_zone->get_switch_entity();
   switch_entity->set_model_3d(model_bin->auto_get("boss_switch_3x3-on-02.obj"));
   switch_entity->set_model_3d_texture(bitmap_bin->auto_get("boss_switch_3x3-on-02.png"));
   switch_entity->get_placement_ref().position.y -= 0.1; // Move the switch "down"

   //AllegroFlare::Vec3D gem_center_place = switch_entity->get_placement_ref().position;
   //gem_center_place.y = 0.0f;
   //float distance = 2.5;

   // create some gems in the scene
   // HERE
   //float
   //spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(-distance, 0, 0));
   //spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(distance, 0, 0));
   //spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(0, 0, -distance));
   //spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(0, 0, distance));
   //spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(-distance, 0, distance));
   //spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(distance, 0, -distance));
   //spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(-distance, 0, -distance));
   //spawn_real_time_gem(gem_center_place + AllegroFlare::Vec3D(distance, 0, distance));

   // Set this switch_plate_zone to active
   switch_plate_zone->set_is_activated(true);

   return;
}

void Screen::handle_on_exit_with_boss_switch(Pipeline::Gameplay::LevelSwitchPlateZone* switch_plate_zone)
{
   if (!switch_plate_zone->get_is_activated()) return;
   //event_emitter->emit_play_sound_effect_event("massive_switch_off");
   event_emitter->emit_play_sound_effect_event("massive_switch_on"); // TODO: Consider changing this sound

   auto switch_entity = switch_plate_zone->get_switch_entity();
   switch_entity->set_model_3d(model_bin->auto_get("boss_switch_3x3-off-02.obj"));
   switch_entity->set_model_3d_texture(bitmap_bin->auto_get("boss_switch_3x3-off-02.png"));
   switch_entity->get_placement_ref().position.y += 0.1; // Move the switch "down"

   switch_plate_zone->set_is_activated(false);

   //// Modify the switch_entity in the entity pool to appear as "active"
   //auto switch_entity = switch_plate_zone->get_switch_entity();
   //switch_entity->set_model_3d(model_bin->auto_get("switch_3x3-on-02.obj"));
   //switch_entity->set_model_3d_texture(bitmap_bin->auto_get("switch_3x3-on-02.png"));
   //switch_entity->get_placement_ref().position.y -= 0.1; // Move the switch "down"

   // Set this switch_plate_zone to active
   //switch_plate_zone->set_is_activated(true);
   return;
}

void Screen::handle_on_stay_on_boss_switch(std::string switch_name)
{
   return;
}

void Screen::handle_on_stay_off_boss_switch(std::string switch_name)
{
   return;
}

void Screen::init_boss_mode()
{
   if (king_turret_boss_mode_is_active) return;
   //king_turret_health_max = 8;
   //king_turret_health = king_turret_health_max;
   //king_turret_is_defeated = false;
   king_turret_boss_mode_is_active = true;
   return;
}

void Screen::boss_take_hit()
{
   if (!king_turret_boss_mode_is_active) return;

   king_turret_health--;
   if (king_turret_health <= 0)
   {
      king_turret_health = 0;
      end_boss_mode_aka_defeat_boss();
   }

   return;
}

void Screen::end_boss_mode_aka_defeat_boss()
{
   if (!king_turret_boss_mode_is_active) return;
   //king_turret_health_max = 8;
   //king_turret_health = king_turret_health_max;
   king_turret_is_defeated = true;
   king_turret_boss_mode_is_active = false;

   spawn_final_platform();

   //auto king_boss_entity = 
   auto *king_turret_base_entity = entity_pool.find_with_attribute(ATTRIBUTE_IS_KING_TURRET);
   if (king_turret_base_entity) entity_pool.remove(king_turret_base_entity);
   else
   {
      throw std::runtime_error("No boss entity, hmm");
   }

   return;
}

void Screen::update()
{
   if (!(current_level))
   {
      std::stringstream error_message;
      error_message << "[Screen::update]: error: guard \"current_level\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::update: error: guard \"current_level\" not met");
   }
   float time_now = al_get_time();
   update_state(); // Consider if this would need to be moved to a different place, or if it conflicts
                   // with the logic below

   // Spin our shadow casted light
   float light_spin = -1.0f;
   float light_time_of_day = 0.15f;
   AllegroFlare::Camera3D *light = scene_renderer.get_shadow_map_buffer_ref().get_light();
   //light->spin = light_spin;
   //light->tilt = 3.141592653 * light_time_of_day; // light_time_of_day = 0.05; // sunrise
                                                  //                     0.5; // high noon
                                                  //                     0.95; // sunset

   //light->spin = -0.5f;
   //light->tilt += 0.001;
   //light->spin = current_level->get_primary_light_spin();
   light->spin = current_level->get_primary_light_spin();
   light->tilt = 3.141592653 * current_level->get_primary_light_tilt_time_of_day();
   //light->spin -= 0.001;



   // Pan the camera
   AllegroFlare::Camera3D *primary_camera = scene_renderer.find_primary_camera_3d();
   //primary_camera->stepout.z += 0.01;
   //primary_camera->spin += 0.0005;
   //primary_camera->tilt += 0.0008;



   // HERE:
   // Check collisions on player in a camera zone

   //auto player_entity_as = get_player_controlled_entity_as();

   if (player_controlled_entity)
   {
      auto player_entity_as = get_player_controlled_entity_as();
      Pipeline::Gameplay::LevelCameraZone *level_camera_zone = find_first_camera_zone_at(player_entity_as);
      if (!level_camera_zone)
      {
         set_camera_to_gameplay_view(scene_renderer.find_primary_camera_3d());
         // Use the default settings
      }
      else
      {
         if (!smooth_camera_is_active)
         {
            if (level_camera_zone->get_name() == "camera-1")
            {
               //AllegroFlare::Camera3D from;
               //AllegroFlare::Camera3D to;
               smooth_camera_from = *scene_renderer.find_primary_camera_3d();
               set_camera_to_custom_view_1(scene_renderer.find_primary_camera_3d());
               smooth_camera_to = *scene_renderer.find_primary_camera_3d();

               smooth_camera_started_at = time_now;
               smooth_camera_duration = 2.5;
               smooth_camera_is_active = true;
            }
            else if (level_camera_zone->get_name() == "camera-2")
            {
               smooth_camera_from = *scene_renderer.find_primary_camera_3d();
               set_camera_to_custom_view_2(scene_renderer.find_primary_camera_3d());
               smooth_camera_to = *scene_renderer.find_primary_camera_3d();

               smooth_camera_started_at = time_now;
               smooth_camera_duration = 2.5;
               smooth_camera_is_active = true;
            }
            else if (level_camera_zone->get_name() == "camera-3")
            {
               smooth_camera_from = *scene_renderer.find_primary_camera_3d();
               set_camera_to_custom_view_3(scene_renderer.find_primary_camera_3d());
               smooth_camera_to = *scene_renderer.find_primary_camera_3d();

               smooth_camera_started_at = time_now;
               smooth_camera_duration = 1.5;
               smooth_camera_is_active = true;
            }
            else if (level_camera_zone->get_name() == "camera-4")
            {
               smooth_camera_from = *scene_renderer.find_primary_camera_3d();
               set_camera_to_custom_view_4(scene_renderer.find_primary_camera_3d());
               smooth_camera_to = *scene_renderer.find_primary_camera_3d();

               smooth_camera_started_at = time_now;
               smooth_camera_duration = 2.5;
               smooth_camera_is_active = true;
            }
            else if (level_camera_zone->get_name() == "camera-5") // Turret main chaimber
            {
               // we're going to use this camera zone as the main trigger for activating "boss mode"

               if (!king_turret_is_defeated)
               {
                  init_boss_mode();
                  //king_turret_health_max = 8;
                  //king_turret_health = king_turret_health_max;
                  //king_turret_is_defeated = false;
                  //king_turret_boss_mode_active = true;
               }


               smooth_camera_from = *scene_renderer.find_primary_camera_3d();
               set_camera_to_custom_view_5(scene_renderer.find_primary_camera_3d());
               smooth_camera_to = *scene_renderer.find_primary_camera_3d();

               smooth_camera_started_at = time_now;
               smooth_camera_duration = 3.0;
               smooth_camera_is_active = true;
            }
         }
      }
   }



   // HERE:
   // Test collision states on these switches
   {
      auto player_entity_as = get_player_controlled_entity_as();
      //Pipeline::Gameplay::LevelSwitchPlateZone *result = nullptr;
      AllegroFlare::Vec3D player_character_position = player_entity_as->get_placement_ref().position;

      for (auto &level_switch_plate_zone : level_switch_plate_zones)
      {
         const std::string &switch_name = level_switch_plate_zone.get_name();
         auto it = level_switch_plate_zones_player_is_currently_colliding_with.find(switch_name);
         bool player_was_previously_colliding =
            (it != level_switch_plate_zones_player_is_currently_colliding_with.end());
         bool player_is_currently_colliding = false;

         auto switch_plate_zone_bounding_box = level_switch_plate_zone.get_bounding_box_ref();
         player_is_currently_colliding =
            level_switch_plate_zone.get_bounding_box_ref().collides_with_point(player_character_position);

         if (!player_was_previously_colliding && player_is_currently_colliding)
         {
            // on enter
            // TODO: Test this
            level_switch_plate_zones_player_is_currently_colliding_with.insert(switch_name);
            handle_on_enter_with_switch(&level_switch_plate_zone);
         }
         else if (player_was_previously_colliding && player_is_currently_colliding)
         {
            // continuing to be on
            handle_on_stay_on_switch(switch_name);
         }
         else if (player_was_previously_colliding && !player_is_currently_colliding)
         {
            // on exit
            // TODO: Test this
            level_switch_plate_zones_player_is_currently_colliding_with.erase(it);
            handle_on_exit_with_switch(switch_name);
         }
         else if (!player_was_previously_colliding && !player_is_currently_colliding)
         {
            // continuing to be off
            handle_on_stay_off_switch(switch_name);
         }
      }
   }


   // HERE:
   // Test collision states on these switches
   {
      auto player_entity_as = get_player_controlled_entity_as();
      //Pipeline::Gameplay::LevelSwitchPlateZone *result = nullptr;
      AllegroFlare::Vec3D player_character_position = player_entity_as->get_placement_ref().position;

      for (auto &boss_switch_plate_zone : boss_switch_plate_zones)
      {
         const std::string &switch_name = boss_switch_plate_zone.get_name();
         auto it = boss_switch_plate_zones_player_is_currently_colliding_with.find(switch_name);
         bool player_was_previously_colliding =
            (it != boss_switch_plate_zones_player_is_currently_colliding_with.end());
         bool player_is_currently_colliding = false;

         auto switch_plate_zone_bounding_box = boss_switch_plate_zone.get_bounding_box_ref();
         player_is_currently_colliding =
            boss_switch_plate_zone.get_bounding_box_ref().collides_with_point(player_character_position);

         if (!player_was_previously_colliding && player_is_currently_colliding)
         {
            // on enter
            // TODO: Test this
            boss_switch_plate_zones_player_is_currently_colliding_with.insert(switch_name);
            handle_on_enter_with_boss_switch(&boss_switch_plate_zone);
         }
         else if (player_was_previously_colliding && player_is_currently_colliding)
         {
            // continuing to be on
            handle_on_stay_on_boss_switch(switch_name);
         }
         else if (player_was_previously_colliding && !player_is_currently_colliding)
         {
            // on exit
            // TODO: Test this
            boss_switch_plate_zones_player_is_currently_colliding_with.erase(it);
            handle_on_exit_with_boss_switch(&boss_switch_plate_zone);
         }
         else if (!player_was_previously_colliding && !player_is_currently_colliding)
         {
            // continuing to be off
            handle_on_stay_off_boss_switch(switch_name);
         }
      }
   }



   if (smooth_camera_is_active)
   {
      Pipeline::SmoothCamera smooth_camera;
      smooth_camera.set_camera_end(smooth_camera_to);
      smooth_camera.set_camera_start(smooth_camera_from);
      smooth_camera.set_time_starting_at(smooth_camera_started_at);
      smooth_camera.set_time_ending_at(smooth_camera_started_at + smooth_camera_duration);
      smooth_camera.set_time_now(time_now);

      float normalized_time = smooth_camera.get_normalized_time();
      if (normalized_time >= 1.0) smooth_camera_is_active = false;

      *scene_renderer.find_primary_camera_3d() = smooth_camera.update();
   }




   //player_control_velocity.x = -0.001;
   bool lock_light_on_player_controlled_entity = true;
   bool lock_camera_on_player_controlled_entity = true;

   if (player_controlled_entity)
   {


      // Update the player's current velocity for this frame
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
      //player_entity_as->get_placement_ref().position.x += x_prime;
      //player_entity_as->get_placement_ref().position.z += y_prime;

      //Pipeline::Physics::GravityStepper gravity_stepper;
      //gravity_stepper.set_entities({ player_entity_as });
      //gravity_stepper.process_step();



      // HERE
      float player_elevation = player_entity_as->get_placement_ref().position.y;
      std::map<int, float> floor_elevations = build_elevations_and_indices_for_floors();
      Pipeline::CurrentFloorInferencer floor_inferencer(floor_elevations);
      //floor_inferencer.set_elevationMap(floor_elevations);
      int closest_floor_index = floor_inferencer.find_closest_floor(player_elevation);

      //int inferred_floor_number =            { floor_height+4.0, floor_height, floor_height-4.0 };
      //float local_y



      // Update the player's position by applying its velocity in the stepper
      // Reposition player_character on map; Use a very fancy swapping of y-with-z variables, the stepper
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      LabyrinthOfLore::WorldMap::TileMap *current_level_tile_map = get_current_level_tile_map(closest_floor_index); // TODO: Pass argument
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      AllegroFlare::Vec2D current_level_tile_map_origin_offset = current_level->get_tile_maps()[closest_floor_index].get_origin_offset();
      //AllegroFlare::Vec2D current_level_tile_map_origin_offset = current_level->get_tile_map_origin_offset();

      AllegroFlare::Vec3D vswapper;
      AllegroFlare::Vec3D pswapper;
      player_entity_as->get_velocity_ref().position.x = x_prime;
      player_entity_as->get_velocity_ref().position.z = y_prime;


      // Apply gravity
      AllegroFlare::vec3d gravity(0.0f, -0.005f, 0.0f);
      player_entity_as->get_velocity_ref().position += gravity;



      player_entity_as->get_placement_ref().position.x += current_level_tile_map_origin_offset.x;
      player_entity_as->get_placement_ref().position.z += current_level_tile_map_origin_offset.y;
      player_entity_as->get_placement_ref().position.x += current_level_tile_map_tile_alignment_offset.x;
      player_entity_as->get_placement_ref().position.z += current_level_tile_map_tile_alignment_offset.y;
      vswapper = player_entity_as->get_velocity_ref().position;
      player_entity_as->get_velocity_ref().position.z = vswapper.y;
      player_entity_as->get_velocity_ref().position.y = vswapper.z;
      pswapper = player_entity_as->get_placement_ref().position;
      player_entity_as->get_placement_ref().position.z = pswapper.y;
      player_entity_as->get_placement_ref().position.y = pswapper.z;

      LabyrinthOfLore::Physics::EntityTileMapCollisionStepper collision_stepper;
      collision_stepper.set_tile_map(current_level_tile_map);
      collision_stepper.set_entities({ player_entity_as });
      collision_stepper.process_step();

      vswapper = player_entity_as->get_velocity_ref().position;
      player_entity_as->get_velocity_ref().position.z = vswapper.y;
      player_entity_as->get_velocity_ref().position.y = vswapper.z;
      pswapper = player_entity_as->get_placement_ref().position;
      player_entity_as->get_placement_ref().position.z = pswapper.y;
      player_entity_as->get_placement_ref().position.y = pswapper.z;
      player_entity_as->get_placement_ref().position.x -= current_level_tile_map_origin_offset.x;
      player_entity_as->get_placement_ref().position.z -= current_level_tile_map_origin_offset.y;
      player_entity_as->get_placement_ref().position.x -= current_level_tile_map_tile_alignment_offset.x;
      player_entity_as->get_placement_ref().position.z -= current_level_tile_map_tile_alignment_offset.y;



      // Update the rotation of all the objects in the scene if they have rotation velocity
      //for (auto &entity : entity
      //pool
      for (auto &entity : entity_pool.get_entity_pool_ref())
      {
         //- name: get_entity_as_dynamic_model_3d
         if (entity->exists(ATTRIBUTE_ITEM_TYPE, "gem"))
         {
            auto gem = get_entity_as_dynamic_model_3d(entity);
            gem->get_placement_ref().rotation += gem->get_velocity_ref().rotation;
         }
         //entity_pool.clear();
      }




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



   //if (entity_pool.exists(
   auto *king_turret_base_entity = entity_pool.find_with_attribute(ATTRIBUTE_IS_KING_TURRET);
   if (king_turret_base_entity)
   {
      auto *king_turret = get_entity_as_dynamic_model_3d(king_turret_base_entity);
      float spin = std::sin(al_get_time()) * 0.05;
      float tilt = std::sin(al_get_time() * 2.0) * 0.05;
      king_turret->get_placement_ref().rotation = AllegroFlare::Vec3D(tilt, spin, 0);
      //king_turret->
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

void Screen::toggle_showing_map_overlay()
{
   show_map_overlay = !show_map_overlay;
   return;
}

void Screen::render_hud_item_with_count(float x, float y, std::string item_name, std::string item_bitmap_identifier, int item_count)
{
   ALLEGRO_FONT *ui_font_stats = obtain_ui_font_stats();
   ALLEGRO_FONT *ui_font_text = obtain_ui_font_text();
   int ui_font_height = al_get_font_line_height(ui_font_stats);
   int ui_font_text_height = al_get_font_line_height(ui_font_text);
   ALLEGRO_FONT *ui_font_num = obtain_ui_font_text();
   ALLEGRO_COLOR color = al_color_name("cyan");
   float opacity = 0.6;
   color.r *= opacity;
   color.g *= opacity;
   color.b *= opacity;
   color.a *= opacity;

   std::stringstream item_count_str;
   item_count_str << item_count;

   float width = 100;
   float height = 100;
   float stroke_thickness = 6.0f;
   float hwidth = width / 2;
   float hheight = height / 2;
   float r = 6;

   // draw the rounded rectangle frame
   al_draw_rounded_rectangle(x - hwidth, y - hheight, x + hwidth, y + hheight, r, r, color, stroke_thickness);


   // Draw the bitmap
   ALLEGRO_BITMAP *img = bitmap_bin->auto_get(item_bitmap_identifier);
   if (img)
   {
      int bwidth = al_get_bitmap_width(img);
      int bheight = al_get_bitmap_height(img);
      al_draw_bitmap(img, x - bwidth/2, y - bheight/2, 0);
   }


   // Draw the number
   al_draw_text(
      ui_font_stats,
      color,
      x + hwidth + 40,
      y + hheight - ui_font_height,
      ALLEGRO_ALIGN_RIGHT,
      item_count_str.str().c_str()
   );

   // Draw the name
   al_draw_text(
      ui_font_text,
      color,
      x + hwidth,
      y + hheight + 4,
      ALLEGRO_ALIGN_RIGHT,
      item_name.c_str()
   );

   //al_draw_rounded_rectangle(
   return;
}

void Screen::render_hud()
{
   int num_mushrooms = game_progress_and_state_info->count_num_items_in_inventory_with_identifier("mushroom");
   int num_gems = game_progress_and_state_info->count_num_items_in_inventory_with_identifier("gem");

   int x_start = 1920 - 120;
   int y_start = 1080/2;
   int y_spacing = 160;
   int y_cursor = 0;

   int num_things = (num_mushrooms != 0) + (num_gems += 0);
   //y_start -= (num_things * y_spacing) - y_spacing / 2;

   if (num_mushrooms != 0)
   {
      render_hud_item_with_count(x_start, y_start+y_cursor, "MUSHROOMS", "mushroom_ui-01.png", num_mushrooms);
      y_cursor += y_spacing;
   }
   if (num_gems != 0)
   {
      render_hud_item_with_count(x_start, y_start+y_cursor, "GEMS", "gem_ui-01.png", num_gems);
      y_cursor += y_spacing;
   }

   //- name: count_num_items_in_inventory_with_identifier
   //return inventory_item_identifiers.count(item_identifier);

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
   if (!(current_level))
   {
      std::stringstream error_message;
      error_message << "[Screen::render]: error: guard \"current_level\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::render: error: guard \"current_level\" not met");
   }
   ALLEGRO_BITMAP *initial_target_bitmap = al_get_target_bitmap();

   // Render the scene
   scene_renderer.render();
   ALLEGRO_BITMAP *render_surface = scene_renderer.get_render_surface_ref().obtain_surface();

   al_set_target_bitmap(initial_target_bitmap);
   al_draw_bitmap(render_surface, 0, 0, 0);

   // Slopily render the tile map

   //al_clear_depth_buffer(1.0);

      auto player_entity_as = get_player_controlled_entity_as();
      float player_elevation = player_entity_as->get_placement_ref().position.y;
      std::map<int, float> floor_elevations = build_elevations_and_indices_for_floors();
      Pipeline::CurrentFloorInferencer floor_inferencer(floor_elevations);
      //floor_inferencer.set_elevationMap(floor_elevations);
      int closest_floor_index = floor_inferencer.find_closest_floor(player_elevation);

   //bool show_map_overlay = false;
   if (show_map_overlay)
   {
      ////////////////////////////////////////////////////// TODO: Pass along floor that you want to see on map ////////////////////
      LabyrinthOfLore::WorldMap::TileMap *current_level_tile_map = get_current_level_tile_map(closest_floor_index); // TODO: Pass argument

      int tile_size = 32;
      AllegroFlare::Vec2D current_level_tile_map_origin_offset = current_level->get_tile_maps()[closest_floor_index].get_origin_offset();
      float groundlevel_height = current_level->get_tile_maps()[closest_floor_index].get_groundlevel_height();
      //AllegroFlare::Vec2D current_level_tile_map_origin_offset = current_level_tile_map->get_origin_offset();
      AllegroFlare::Vec3D player_position = get_player_controlled_entity_as()->get_placement_ref().position;
      AllegroFlare::Vec2D tile_alignment_offset = current_level_tile_map_tile_alignment_offset;
      float player_map_position_x =
         (player_position.x + current_level_tile_map_origin_offset.x + tile_alignment_offset.x) * tile_size;
      float player_map_position_y =
         (player_position.z + current_level_tile_map_origin_offset.y + tile_alignment_offset.y) * tile_size;

      AllegroFlare::Placement3D map_placement;
      map_placement.position.x = 1920/2 - player_map_position_x;
      map_placement.position.y = 1080/2 - player_map_position_y;

      //map_placement.rotation.x = 0.35;
      map_placement.start_transform();
      LabyrinthOfLore::WorldMap::BasicRenderer basic_renderer;
      basic_renderer.set_tile_map(current_level_tile_map);
      basic_renderer.set_tile_width(tile_size);
      basic_renderer.set_tile_height(tile_size);
      basic_renderer.set_groundlevel_height(groundlevel_height);
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
   }


   // draw light coordinates
   bool draw_light_coordinates = false;
   if (draw_light_coordinates)
   {
      AllegroFlare::Camera3D *light = scene_renderer.get_shadow_map_buffer_ref().get_light();
      ALLEGRO_FONT *ui_font = obtain_ui_font();
      ALLEGRO_COLOR hud_text_color = al_color_name("cyan");
      std::stringstream coordinates;
      coordinates << "tilt (TOD): " << std::setprecision(3) << (light->tilt / 3.14159);
      al_draw_text(ui_font, hud_text_color, 1920 - 300, 1080 - 100, ALLEGRO_ALIGN_RIGHT, coordinates.str().c_str());
      std::stringstream coordinates2;
      coordinates2 << "spin: " << std::setprecision(3) << light->spin;
      al_draw_text(ui_font, hud_text_color, 1920 - 300, 1080 - 100 + 20, ALLEGRO_ALIGN_RIGHT, coordinates2.str().c_str());
   }


   //AllegroFlare::Camera3D *light = scene_renderer.get_shadow_map_buffer_ref().get_light();
      //al_draw_text(ui_font, hud_text_color, 1920 - 300, 1080 - 200 + 20, ALLEGRO_ALIGN_RIGHT, coordinates2.str().c_str());


   render_hud();
   //render_boss_mode_hud();

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
      if (!current_level)
      {
         AllegroFlare::Logger::throw_error(
            "Pipeline::Gameplay::Screen::game_event_func",
            "On game event \"perform_music\", current_level cannot be nullptr."
         );
      }

      std::string song_to_perform_identifier = current_level->get_song_to_perform_identifier();
      float song_to_perform_duration_sec = current_level->get_song_to_perform_duration_sec();
      activate_music_performance(song_to_perform_identifier, song_to_perform_duration_sec);
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

void Screen::display_switch_in_func()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::display_switch_in_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::display_switch_in_func: error: guard \"initialized\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::display_switch_in_func]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::display_switch_in_func: error: guard \"event_emitter\" not met");
   }
   load_tile_map(current_level_identifier); // DEVELOPMENT
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

      case ALLEGRO_KEY_L: {
         load_tile_map(current_level_identifier);
      } break;

      case ALLEGRO_KEY_M: {
         toggle_showing_map_overlay();
      } break;

      case ALLEGRO_KEY_S: {
         write_tile_elevation_value(0, 48, 94, 10.0f);
      } break;

      case ALLEGRO_KEY_B: {
         end_boss_mode_aka_defeat_boss();
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

void Screen::activate_music_performance(std::string music_identifier, float duration_sec)
{
   if (is_state(STATE_PERFORMING_MUSIC)) return; // TODO: Test this
   // Set our current song state variables
   // TODO: Validate music track exists
   currently_performing_song_identifier = music_identifier;
   currently_performing_song_duration_sec = duration_sec; // TODO: Replace this hard-coded value with the actual duration
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
   if (!(current_level))
   {
      std::stringstream error_message;
      error_message << "[Screen::deactivate_music_performance]: error: guard \"current_level\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::deactivate_music_performance: error: guard \"current_level\" not met");
   }
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

   std::string level_music_identifier = current_level->get_background_music_identifier();
   event_emitter->emit_play_music_track_event(level_music_identifier);
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
         set_camera_to_gameplay_view(scene_renderer.find_primary_camera_3d());
      } break;

      case STATE_SUSPEND_FOR_DIALOG: {
         set_camera_to_dialog_view(scene_renderer.find_primary_camera_3d());
         player_control_velocity = {0, 0};
         player_control_dashing = false;
      } break;

      case STATE_PERFORMING_MUSIC: {
         set_camera_to_music_performance_view(scene_renderer.find_primary_camera_3d());
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
         //for (//HERE
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

ALLEGRO_FONT* Screen::obtain_ui_font_text()
{
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Screen::obtain_ui_font_text]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::obtain_ui_font_text: error: guard \"font_bin\" not met");
   }
   return font_bin->auto_get("Oswald-Medium.ttf -32");
}

ALLEGRO_FONT* Screen::obtain_ui_font_stats()
{
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Screen::obtain_ui_font_stats]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::obtain_ui_font_stats: error: guard \"font_bin\" not met");
   }
   return font_bin->auto_get("Oswald-Medium.ttf -46");
}


} // namespace Gameplay
} // namespace Pipeline


