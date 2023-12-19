#pragma once


#include <AllegroFlare/AudioController.hpp>
#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/Elements/Backgrounds/ClearToColor.hpp>
#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/FontBin.hpp>
#include <AllegroFlare/Frameworks/Full.hpp>
#include <AllegroFlare/GameConfigurations/Base.hpp>
#include <AllegroFlare/GameEvent.hpp>
#include <AllegroFlare/GameSession.hpp>
#include <AllegroFlare/LoadASavedGame/Screen.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Routers/Standard.hpp>
#include <AllegroFlare/Screens/Achievements.hpp>
#include <AllegroFlare/Screens/Base.hpp>
#include <AllegroFlare/Screens/GameOverScreen.hpp>
#include <AllegroFlare/Screens/GameWonScreen.hpp>
#include <AllegroFlare/Screens/LevelSelectScreen.hpp>
#include <AllegroFlare/Screens/RollingCredits.hpp>
#include <AllegroFlare/Screens/SettingsScreen.hpp>
#include <AllegroFlare/Screens/Storyboard.hpp>
#include <AllegroFlare/Screens/TitleScreen.hpp>
#include <AllegroFlare/Screens/Version.hpp>
#include <Pipeline/GameProgressAndStateInfo.hpp>
#include <Pipeline/Gameplay/Screen.hpp>
#include <ReleaseInfo.hpp>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>


namespace Pipeline
{
   class Runner : public AllegroFlare::Screens::Base
   {
   public:
      static constexpr uint32_t MY_ROUTE_EVENT_ACTIVATE_MY_CUSTOM_SCREEN = AllegroFlare::Routers::Standard::EVENT_LAST_EVENT+1;
      static constexpr char* MY_CUSTOM_SCREEN_IDENTIFIER = (char*)"my_custom_screen";
      static constexpr char* TYPE = (char*)"Pipeline/Runner";

   private:
      AllegroFlare::Frameworks::Full* framework;
      AllegroFlare::EventEmitter* event_emitter;
      AllegroFlare::BitmapBin* bitmap_bin;
      AllegroFlare::FontBin* font_bin;
      AllegroFlare::ModelBin* model_bin;
      AllegroFlare::Routers::Standard router;
      AllegroFlare::Screens::Storyboard intro_logos_screen;
      AllegroFlare::Screens::Storyboard intro_storyboard_screen;
      AllegroFlare::Screens::TitleScreen title_screen;
      AllegroFlare::Screens::Achievements achievements_screen;
      AllegroFlare::Screens::Version version_screen;
      AllegroFlare::LoadASavedGame::Screen load_a_saved_game_screen;
      AllegroFlare::Screens::Storyboard new_game_intro_storyboard_screen;
      AllegroFlare::Screens::LevelSelectScreen level_select_screen;
      AllegroFlare::Screens::GameOverScreen game_over_screen;
      AllegroFlare::Screens::GameWonScreen game_won_screen;
      AllegroFlare::Screens::Storyboard game_won_outro_storyboard_screen;
      AllegroFlare::Screens::SettingsScreen settings_screen;
      AllegroFlare::Screens::RollingCredits rolling_credits_screen;
      Pipeline::Gameplay::Screen primary_gameplay_screen;
      AllegroFlare::GameConfigurations::Base* game_configuration;
      Pipeline::GameProgressAndStateInfo game_progress_and_state_info;
      AllegroFlare::Elements::Backgrounds::ClearToColor solid_black_background;
      ReleaseInfo release_info;
      bool initialized;

   protected:


   public:
      Runner(AllegroFlare::Frameworks::Full* framework=nullptr, AllegroFlare::EventEmitter* event_emitter=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::FontBin* font_bin=nullptr, AllegroFlare::ModelBin* model_bin=nullptr);
      virtual ~Runner();

      virtual void game_event_func(AllegroFlare::GameEvent* game_event=nullptr) override;
      void initialize();
      void setup_sound_effects_and_music(AllegroFlare::AudioController* audio_controller_ptr=nullptr);
      std::vector<std::pair<std::string, std::string>> build_title_screen_menu_options();
      void mark_achievement_as_unlocked_and_save_progress(std::string achievement_identifier="[unset-achievement_identifier]");
      static bool on_route_event_unhandled_func(uint32_t unhandled_event=0, AllegroFlare::Routers::Standard* router=nullptr, void* user_data=nullptr);
      void continue_from_last_save();
      void setup_new_game_progress_and_state_info(AllegroFlare::GameSession* game_session=nullptr);
      void load_last_played_session_or_start_new(AllegroFlare::GameSession* game_session=nullptr);
      void setup_router();
      static void run(std::string deployment_environment_mode="[unset-deployment_environment_mode]");
   };
}



