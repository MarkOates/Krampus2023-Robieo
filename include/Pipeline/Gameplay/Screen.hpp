#pragma once


#include <AllegroFlare/ALLEGRO_VERTEX_WITH_NORMAL.hpp>
#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/FontBin.hpp>
#include <AllegroFlare/Frameworks/Full.hpp>
#include <AllegroFlare/GameConfigurations/Base.hpp>
#include <AllegroFlare/GameEvent.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/Base.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityPool.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/SceneRenderer2.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Player.hpp>
#include <AllegroFlare/Screens/Base.hpp>
#include <AllegroFlare/Vec2D.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <AllegroFlare/VirtualControllers/Base.hpp>
#include <Pipeline/GameProgressAndStateInfo.hpp>
#include <Pipeline/Gameplay/Level.hpp>
#include <Pipeline/Gameplay/Screen.hpp>
#include <allegro5/allegro.h>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>


namespace Pipeline
{
   namespace Gameplay
   {
      class Screen : public AllegroFlare::Screens::Base
      {
      public:
         static constexpr char* TYPE = (char*)"Pipeline/Gameplay/Screen";
         static constexpr char* ATTRIBUTE_COLLECTABLE_BY_PLAYER = (char*)"collectable_by_player";
         static constexpr char* ATTRIBUTE_ITEM_TYPE = (char*)"item_type";

      private:
         enum State
         {
            STATE_UNDEF = 0,
            STATE_REVEALING,
            STATE_PLAYING_GAME,
            STATE_SUSPEND_FOR_DIALOG,
            STATE_PERFORMING_MUSIC,
         };
         AllegroFlare::Frameworks::Full* framework;
         AllegroFlare::EventEmitter* event_emitter;
         AllegroFlare::BitmapBin* bitmap_bin;
         AllegroFlare::FontBin* font_bin;
         AllegroFlare::ModelBin* model_bin;
         AllegroFlare::GameConfigurations::Base* game_configuration;
         Pipeline::GameProgressAndStateInfo* game_progress_and_state_info;
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool entity_pool;
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* player_controlled_entity;
         AllegroFlare::Vec2D player_control_velocity;
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* goal_entity;
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* exit_entity;
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::SceneRenderer2 scene_renderer;
         std::string current_level_identifier;
         Pipeline::Gameplay::Level* current_level;
         std::string current_level_song_to_perform_identifier;
         float current_level_song_to_perform_duration_sec;
         std::string currently_performing_song_identifier;
         float currently_performing_song_duration_sec;
         std::function<void(Pipeline::Gameplay::Screen*, void*)> on_finished_callback_func;
         void* on_finished_callback_func_user_data;
         bool initialized;
         uint32_t state;
         bool state_is_busy;
         float state_changed_at;
         bool player_is_colliding_on_goal;
         bool player_is_colliding_on_exit;

      protected:


      public:
         Screen(AllegroFlare::Frameworks::Full* framework=nullptr, AllegroFlare::EventEmitter* event_emitter=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::FontBin* font_bin=nullptr, AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::GameConfigurations::Base* game_configuration=nullptr);
         virtual ~Screen();

         void set_game_configuration(AllegroFlare::GameConfigurations::Base* game_configuration);
         void set_game_progress_and_state_info(Pipeline::GameProgressAndStateInfo* game_progress_and_state_info);
         void set_on_finished_callback_func(std::function<void(Pipeline::Gameplay::Screen*, void*)> on_finished_callback_func);
         void set_on_finished_callback_func_user_data(void* on_finished_callback_func_user_data);
         AllegroFlare::GameConfigurations::Base* get_game_configuration() const;
         Pipeline::GameProgressAndStateInfo* get_game_progress_and_state_info() const;
         std::function<void(Pipeline::Gameplay::Screen*, void*)> get_on_finished_callback_func() const;
         void* get_on_finished_callback_func_user_data() const;
         uint32_t get_state() const;
         void set_framework(AllegroFlare::Frameworks::Full* framework=nullptr);
         void set_event_emitter(AllegroFlare::EventEmitter* event_emitter=nullptr);
         void set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin=nullptr);
         void set_font_bin(AllegroFlare::FontBin* font_bin=nullptr);
         void set_model_bin(AllegroFlare::ModelBin* model_bin=nullptr);
         bool trivial_collide(AllegroFlare::Vec3D p1={0.0f, 0.0f, 0.0f}, AllegroFlare::Vec3D p2={0.0f, 0.0f, 0.0f}, float min_distance=10.0f);
         void set_primary_camera_to_gameplay_view();
         void set_primary_camera_to_dialog_view();
         void set_primary_camera_to_music_performance_view();
         AllegroFlare::DialogTree::NodeBank build_dialog_node_bank();
         AllegroFlare::Vec3D lowest_y_vertex(std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_NORMAL> vertices={});
         void load_level_by_identifier(std::string level_identifier="[unset-level_identifier]");
         void initialize();
         virtual void on_activate() override;
         virtual void on_deactivate() override;
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* get_player_controlled_entity_as();
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* get_goal_entity_as();
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* get_exit_entity_as();
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* get_entity_as_dynamic_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* colliding_entity=nullptr);
         void on_player_entity_collide(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* colliding_entity=nullptr);
         void update();
         void render();
         void save_bitmap_buffers_to_files();
         void call_on_finished_callback_func();
         virtual void on_event(ALLEGRO_EVENT* ev=nullptr) override;
         virtual void game_event_func(AllegroFlare::GameEvent* game_event=nullptr) override;
         virtual void primary_timer_func() override;
         virtual void key_up_func(ALLEGRO_EVENT* ev=nullptr) override;
         virtual void key_down_func(ALLEGRO_EVENT* ev=nullptr) override;
         virtual void virtual_control_button_up_func(AllegroFlare::Player* player=nullptr, AllegroFlare::VirtualControllers::Base* virtual_controller=nullptr, int virtual_controller_button_num=0, bool is_repeat=false) override;
         virtual void virtual_control_button_down_func(AllegroFlare::Player* player=nullptr, AllegroFlare::VirtualControllers::Base* virtual_controller=nullptr, int virtual_controller_button_num=0, bool is_repeat=false) override;
         virtual void virtual_control_axis_change_func(ALLEGRO_EVENT* ev=nullptr) override;
         void activate_music_performance(std::string music_identifier="[unset-music_identifier]");
         void deactivate_music_performance();
         void set_state(uint32_t state=STATE_UNDEF, bool override_if_busy=false);
         static float normalize_age_no_clamp(float start_time=0.0f, float end_time=0.0f, float time_now=0.0f);
         void update_state(float time_now=al_get_time());
         static bool is_valid_state(uint32_t state=STATE_UNDEF);
         bool is_state(uint32_t possible_state=STATE_UNDEF);
         float infer_current_state_age(float time_now=al_get_time());
      };
   }
}



