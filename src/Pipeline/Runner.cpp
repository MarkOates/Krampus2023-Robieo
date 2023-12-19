

#include <Pipeline/Runner.hpp>

#include <AllegroFlare/Color.hpp>
#include <AllegroFlare/Elements/RollingCredits/SectionFactory.hpp>
#include <AllegroFlare/EventNames.hpp>
#include <AllegroFlare/Frameworks/Full.hpp>
#include <AllegroFlare/GameEventDatas/AchievementUnlocked.hpp>
#include <AllegroFlare/GameEventDatas/ScreenActivated.hpp>
#include <AllegroFlare/GameEventDatas/String.hpp>
#include <AllegroFlare/GameSession.hpp>
#include <AllegroFlare/LoadASavedGame/SaveSlots/Empty.hpp>
#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/RouteEventDatas/StartLevel.hpp>
#include <AllegroFlare/StoryboardFactory.hpp>
#include <AllegroFlare/UsefulPHP.hpp>
#include <Pipeline/GameConfigurations/Main.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Pipeline
{


Runner::Runner(AllegroFlare::Frameworks::Full* framework, AllegroFlare::EventEmitter* event_emitter, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::FontBin* font_bin, AllegroFlare::ModelBin* model_bin)
   : AllegroFlare::Screens::Base(Pipeline::Runner::TYPE)
   , framework(framework)
   , event_emitter(event_emitter)
   , bitmap_bin(bitmap_bin)
   , font_bin(font_bin)
   , model_bin(model_bin)
   , router()
   , intro_logos_screen()
   , intro_storyboard_screen()
   , title_screen()
   , achievements_screen()
   , version_screen()
   , load_a_saved_game_screen()
   , new_game_intro_storyboard_screen()
   , level_select_screen()
   , game_over_screen()
   , game_won_screen()
   , game_won_outro_storyboard_screen()
   , settings_screen()
   , rolling_credits_screen()
   , primary_gameplay_screen()
   , game_configuration(nullptr)
   , game_progress_and_state_info()
   , game_progress_and_state_info_filename("./game_progress_and_state_info.sav")
   , solid_black_background(ALLEGRO_COLOR{0, 0, 0, 1})
   , release_info({})
   , initialized(false)
{
}


Runner::~Runner()
{
}


void Runner::game_event_func(AllegroFlare::GameEvent* game_event)
{
   if (!(game_event))
   {
      std::stringstream error_message;
      error_message << "[Runner::game_event_func]: error: guard \"game_event\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::game_event_func: error: guard \"game_event\" not met");
   }
   // TODO: Handle top-level game events here
   //if (game_event->is_type(AllegroFlare::GameEventDatas::ScreenActivated::NAME))
   //{
      //AllegroFlare::GameEventDatas::ScreenActivated* as =
        //static_cast<AllegroFlare::GameEventDatas::ScreenActivated*>(game_event->get_data());

      //// TODO: Handle game-specific logic for a after a screen switch
   //}
   if (game_event->get_type() == AllegroFlare::GameEventDatas::AchievementUnlocked::NAME)
   {
      if (game_event->get_data()->is_type(AllegroFlare::GameEventDatas::AchievementUnlocked::TYPE))
      {
         AllegroFlare::GameEventDatas::AchievementUnlocked *as =
            static_cast<AllegroFlare::GameEventDatas::AchievementUnlocked *>(game_event->get_data());
         mark_achievement_as_unlocked_and_save_progress(as->get_achievement_identifier());
      }
      else
      {
         throw std::runtime_error("Gameplay/Runner/game_event_func: unexpected AchievementUnlocked event data type");
      }
   }
   else if (game_event->get_type() == "package_delivered")
   {
      // TODO: Extract the package name from the event
      if (game_event->get_data()->is_type(AllegroFlare::GameEventDatas::String::TYPE))
      {
         AllegroFlare::GameEventDatas::String *as =
            static_cast<AllegroFlare::GameEventDatas::String *>(game_event->get_data());
         //mark_achievement_as_unlocked_and_save_progress(as->get_achievement_identifier());
         mark_package_as_delivered_and_save_progress(as->get_string()); //"a-generic-package-name");
      }
      else
      {
         throw std::runtime_error("Gameplay/Runner/game_event_func: unexpected type, expecting String event data");
      }
   }
   return;
   return;
}

void Runner::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Runner::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::initialize: error: guard \"(!initialized)\" not met");
   }
   if (!(framework))
   {
      std::stringstream error_message;
      error_message << "[Runner::initialize]: error: guard \"framework\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::initialize: error: guard \"framework\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Runner::initialize]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::initialize: error: guard \"event_emitter\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Runner::initialize]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::initialize: error: guard \"bitmap_bin\" not met");
   }
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Runner::initialize]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::initialize: error: guard \"font_bin\" not met");
   }
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Runner::initialize]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::initialize: error: guard \"model_bin\" not met");
   }
   // Create the resources
   AllegroFlare::Achievements &achievements = framework->get_achievements_ref();
   AllegroFlare::AudioController &audio_controller = framework->get_audio_controller_ref();

   // Create a storyboard factory and page factory
   AllegroFlare::StoryboardFactory storyboard_factory;
   storyboard_factory.set_font_bin(font_bin);
   storyboard_factory.set_event_emitter(event_emitter);
   AllegroFlare::StoryboardPageFactory page_factory;
   page_factory.set_font_bin(font_bin);
   page_factory.set_bitmap_bin(bitmap_bin);
   page_factory.set_model_bin(model_bin);

   // Setup our router
   setup_router();

   // Set the framework to use our router
   framework->set_router(&router);

   // TODO: Create the game configuration for our game
   // Setup the configuration
   game_configuration = new Pipeline::GameConfigurations::Main();

   // Fill our dialog bank
   framework->set_dialog_system_dialog_node_bank(game_configuration->build_dialog_bank_by_identifier());

   // TODO: Setup achievements
   achievements.set_achievements({
      { "stay_through_the_credits",
         new AllegroFlare::Achievement("Stay Through the Credits", "Watch the credits"),
         false,
         false,
      },
   });

   // TODO: Setup intro logos screen
   intro_logos_screen.set_event_emitter(event_emitter);
   intro_logos_screen.set_font_bin(font_bin);
   intro_logos_screen.set_auto_advance(true);
   intro_logos_screen.set_background(&solid_black_background);
   intro_logos_screen.initialize();
   intro_logos_screen.get_storyboard_element_ref().set_pages({
      //page_factory.create_clubcatt_logo_page(),
      //page_factory.create_image_page(bitmap_bin->operator[]("clubcatt-website-01.jpg")),
   });

   // TODO: Setup intro storyboard screen
   intro_storyboard_screen.set_event_emitter(event_emitter);
   intro_storyboard_screen.set_font_bin(font_bin);
   intro_storyboard_screen.set_auto_advance(true);
   intro_storyboard_screen.set_background(&solid_black_background);
   intro_storyboard_screen.initialize();
   intro_storyboard_screen.get_storyboard_element_ref().set_pages({
      //page_factory.create_image_with_advancing_text_page(
         //"storyboard-1-01-1165x500.png",
         //"Once upon a time, in a magical kingdom ruled by a wise and just queen, a young hero sets out on a "
            //"journey to prove himself and save his people from a terrible curse."
      //),
      //page_factory.create_image_with_advancing_text_page(
         //"storyboard-2-01-1165x500.png",
         //"With the help of his trusty sidekick and a band of unlikely allies, he must navigate treacherous "
            //"terrain and battle fierce foes."
      //),
      //page_factory.create_advancing_text_page(
        //"And achieve his goal to save the kingdom."
      //),
   });

   // TODO: Setup title screen
   title_screen.set_event_emitter(event_emitter);
   title_screen.set_menu_options( build_title_screen_menu_options());
   title_screen.set_font_bin(font_bin);
   title_screen.set_bitmap_bin(bitmap_bin);
   title_screen.set_title_bitmap_name("robieo-logo-1-02.png");
   title_screen.set_title_position_x(1920/2);
   title_screen.set_title_position_y(1080/2-50);
   title_screen.set_menu_position_x(1920/2);
   title_screen.set_menu_position_y(1080/2+100);
   std::string copyright_text = "© 2023 CLUBCATT Games         clubcatt.com         version " + release_info.get_version();
   title_screen.set_copyright_text(copyright_text);
   title_screen.set_background(&solid_black_background);
   //title_screen.initialize(); // NOTE: Initialization is not necessary for this screen

   // TODO: Setup achievements screen
   achievements_screen.set_achievements(&achievements);
   achievements_screen.set_event_emitter(event_emitter);
   achievements_screen.set_font_bin(font_bin);
   achievements_screen.set_background(&solid_black_background);
   achievements_screen.initialize();

   // TODO: Setup version screen
   version_screen.set_bitmap_bin(bitmap_bin);
   version_screen.set_font_bin(font_bin);
   version_screen.set_model_bin(model_bin);
   version_screen.set_event_emitter(event_emitter);
   version_screen.set_background(&solid_black_background);
   version_screen.initialize();

   // TODO: Setup new game intro storyboard screen
   new_game_intro_storyboard_screen.set_event_emitter(event_emitter);
   new_game_intro_storyboard_screen.set_font_bin(font_bin);
   new_game_intro_storyboard_screen.set_background(&solid_black_background);
   new_game_intro_storyboard_screen.initialize();
   new_game_intro_storyboard_screen.get_storyboard_element_ref().set_pages({
       //page_factory.create_advancing_text_page(
         //"In a land beset by darkness, a young hero embarks on a perilous journey to retrieve a powerful "
            //"artifact that can restore light to the kingdom."
       //),
       //page_factory.create_advancing_text_page(
         //"With nothing but a trusty sword and his wits to guide him, he sets out to face whatever dangers "
            //"lie ahead."
       //),
   });

   // TODO: Setup load a saved game screen
   load_a_saved_game_screen.set_event_emitter(event_emitter);
   load_a_saved_game_screen.set_bitmap_bin(bitmap_bin);
   load_a_saved_game_screen.set_font_bin(font_bin);
   load_a_saved_game_screen.set_model_bin(model_bin); // Currently not used, but required
   load_a_saved_game_screen.initialize();

   load_a_saved_game_screen.set_background(&solid_black_background);
   load_a_saved_game_screen.set_save_slots({
      new AllegroFlare::LoadASavedGame::SaveSlots::Empty(), // TODO: Fill this list with items from save file
      new AllegroFlare::LoadASavedGame::SaveSlots::Empty(),
      new AllegroFlare::LoadASavedGame::SaveSlots::Empty(),
   });


   // TODO: Setup level select screen
   level_select_screen.set_event_emitter(event_emitter);
   level_select_screen.set_bitmap_bin(bitmap_bin);
   level_select_screen.set_font_bin(font_bin);
   //level_select_screen.set_levels_list({
      //{ "Candy Kingdom", "candy_kingdom" },
      //{ "Cherry Blossom Grove", "cherry_blossom_grove" },
      //{ "Bubble Pop Bay", "bubble_pop_bay" },
      //{ "Neon City Nights", "neon_city_nights" },
      //{ "Wonderland Woods", "wonderland_woods" },
   //});
   level_select_screen.set_levels_list(
      game_configuration->build_level_list_for_level_select_screen_by_identifier("discarded-for-now")
   );
   level_select_screen.set_background(&solid_black_background);
   level_select_screen.initialize();

   // TODO: Setup game over screen
   game_over_screen.set_event_emitter(event_emitter);
   game_over_screen.set_font_bin(font_bin);
   game_over_screen.set_background(&solid_black_background);
   game_over_screen.initialize();

   // TODO: Setup game won screen
   game_won_screen.set_font_bin(font_bin);
   game_won_screen.set_background(&solid_black_background);
   //game_won_screen.initialize(); // NOTE: Initialization is not necessary for this screen

   // TODO: Setup game won outro storyboard screen
   game_won_outro_storyboard_screen.set_event_emitter(event_emitter);
   game_won_outro_storyboard_screen.set_background(&solid_black_background);
   game_won_outro_storyboard_screen.initialize();

   // TODO: Setup the settings screen
   settings_screen.set_event_emitter(event_emitter); // TODO: See if this is necessary
   settings_screen.set_bitmap_bin(bitmap_bin); // Currently not used, but required
   settings_screen.set_font_bin(font_bin);
   settings_screen.set_model_bin(model_bin); // Currently not used, but required
   settings_screen.set_background(&solid_black_background);
   settings_screen.initialize();

   // TODO: Setup rolling credits screen
   AllegroFlare::Elements::RollingCredits::SectionFactory section_factory;
   rolling_credits_screen.set_event_emitter(event_emitter); // TODO: See if this is necessary
   rolling_credits_screen.set_font_bin(font_bin);
   rolling_credits_screen.set_background(&solid_black_background);
   rolling_credits_screen.set_sections({
      section_factory.create_spacer(),

      section_factory.create_text(
         "A game by Mark Oates"
      ),

      section_factory.create_spacer(),

      section_factory.create_text(
         "KrampusHack 2023\nhttps://tins.amarillion.org/krampu23"
      ),

      section_factory.create_section_spacer(),

      section_factory.create_header("Programming"),
      section_factory.create_column_with_labels({
         { "Made with", "Allegro (5.2.9)" },
         {          "", "https://liballeg.org/" },
         //{          "", "by Allegro Developers" },
         { "Framework", "AllegroFlare (0.8.11-wip)" },
         {          "", "https://github.com/allegroflare/allegro_flare" },
         {          "", "by Mark Oates" },
      }),

      section_factory.create_section_spacer(),

      section_factory.create_header("People who have contributed code to Allegro 5"),
      section_factory.create_allegro5_contributors_list(3),

      section_factory.create_section_spacer(),
   });
   rolling_credits_screen.append_sections(section_factory.create_standard_disclaimers_text());
   rolling_credits_screen.initialize();

   // Setup our main gameplay screen
   primary_gameplay_screen.set_framework(framework); // NOTE: Only required to set the node bank. Definitely open
                                                     // to alternatives so that framework does not need to be
                                                     // injected here
   primary_gameplay_screen.set_event_emitter(event_emitter);
   primary_gameplay_screen.set_bitmap_bin(bitmap_bin);
   primary_gameplay_screen.set_font_bin(font_bin);
   primary_gameplay_screen.set_model_bin(model_bin);
   primary_gameplay_screen.set_game_configuration(game_configuration); // TODO: Consider how to remove this dependency
   primary_gameplay_screen.set_game_progress_and_state_info(&game_progress_and_state_info);
   primary_gameplay_screen.initialize();



   setup_sound_effects_and_music(&audio_controller);



   // Load game_progress_and_state_info file
   bool save_file_exists = std::filesystem::exists(game_progress_and_state_info_filename);
   if (save_file_exists)
   {
      AllegroFlare::Logger::info_from(
         "Pipeline::Runner::initialize",
         "Save file found, loading..."
      );

      std::string save_file_contents = AllegroFlare::php::file_get_contents(game_progress_and_state_info_filename);
      game_progress_and_state_info.import_from_string(save_file_contents);

      AllegroFlare::Logger::info_from(
         "Pipeline::Runner::initialize",
         "Save file loaded successfully."
      );
   }



   // TODO: Load up our sound effects
   //audio_controller.set_and_load_sound_effect_elements({
      // { "menu_move", { "menu_move_tink-02.ogg", false, "restart" } }, // TODO: Throw on an unknown replay type
   //});

   // TODO: Load up our music tracks
   //audio_controller.set_and_load_music_track_elements({
      // An example of how to load a music track:
      //{ "intro_music", { "wanderer-01.ogg", true, "ignore" } },
   //});

   // An example of how to play a music track:
   // event_emitter->emit_play_music_track_event("intro_music");

   return;
}

void Runner::setup_sound_effects_and_music(AllegroFlare::AudioController* audio_controller_ptr)
{
   if (!(audio_controller_ptr))
   {
      std::stringstream error_message;
      error_message << "[Runner::setup_sound_effects_and_music]: error: guard \"audio_controller_ptr\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::setup_sound_effects_and_music: error: guard \"audio_controller_ptr\" not met");
   }
   auto &audio_controller = *audio_controller_ptr;

   std::map<std::string, AllegroFlare::AudioRepositoryElement>{
     { "robot-holly_jolly", { "robio-sound-chip--have-a-holly-jolly--01.ogg", true, "ignore" } },
     //{ "level_1_music", { "8bit-dungeon-level-kevin-macleod-80kbps.ogg", true, "restart", 0.5 } },
     //{ "final_level_music", { "final-level-music-kevin-macleod-01.ogg", true, "restart", 1.0 } },
     //{ "boss_music", { "krampus-boss-01.ogg", true, "restart", 0.78 } },
     //{ "dead_krampus_music", { "dead-krampus-01.ogg", false, "restart", 0.78 } },
     //{ "win_music", { "win-music-01.ogg", false, "restart" } },
     //{ "end_music", { "jingle-bells-calm.ogg", false, "restart", 0.8 } },
   };

   audio_controller.set_and_load_sound_effect_elements({
      // { "menu_move", { "menu_move_tink-02.ogg", false, "restart" } }, // TODO: Throw on an unknown replay type
   });

   // TODO: Load up our music tracks
   audio_controller.set_and_load_music_track_elements(
      std::map<std::string, AllegroFlare::AudioRepositoryElement>{
        { "robot-holly_jolly", { "robio-sound-chip--have-a-holly-jolly--01.ogg", false, "ignore" } },
        //{ "level_1_music", { "8bit-dungeon-level-kevin-macleod-80kbps.ogg", true, "restart", 0.5 } },
        //{ "final_level_music", { "final-level-music-kevin-macleod-01.ogg", true, "restart", 1.0 } },
        //{ "boss_music", { "krampus-boss-01.ogg", true, "restart", 0.78 } },
        //{ "dead_krampus_music", { "dead-krampus-01.ogg", false, "restart", 0.78 } },
        //{ "win_music", { "win-music-01.ogg", false, "restart" } },
        //{ "end_music", { "jingle-bells-calm.ogg", false, "restart", 0.8 } },
      }
   );

   return;
}

std::vector<std::pair<std::string, std::string>> Runner::build_title_screen_menu_options()
{
   std::vector<std::pair<std::string, std::string>> options = {
      //{ "Continue",          "continue_from_last_save" },       // TODO: If game session is saved and valid
      //{ "Load a Saved Game", "goto_load_a_saved_game_screen" }, // TODO: If game session is saved and valid,
                                                                // and the game supports save slots
      { "Start New Game",    "start_new_game" },                // TODO: If the game session has not begun
      //{ "Achievements",      "goto_achievements_screen" },
      //{ "Settings",          "goto_settings_screen" },
      //{ "Version",           "goto_version_screen" },
      //{ "Credits",           "goto_credits_screen" },           // TODO: If game has been won
      { "Quit",              "quit" },
   };
   return options;
}

void Runner::mark_achievement_as_unlocked_and_save_progress(std::string achievement_identifier)
{
   // guards: [ game_progress_and_state_info ]
   game_progress_and_state_info.mark_achievement_as_unlocked(achievement_identifier);
   std::string updated_save_file_content = game_progress_and_state_info.export_to_string();
   AllegroFlare::php::file_put_contents(game_progress_and_state_info_filename, updated_save_file_content);
   // TODO: Post some notification "saved" notification
   return;
}

void Runner::mark_package_as_delivered_and_save_progress(std::string package_identifier)
{
   // guards: [ game_progress_and_state_info ]
   game_progress_and_state_info.mark_package_as_delivered(package_identifier);
   std::string updated_save_file_content = game_progress_and_state_info.export_to_string();
   AllegroFlare::php::file_put_contents(game_progress_and_state_info_filename, updated_save_file_content);
   // TODO: Post some notification "saved" notification
   return;
}

bool Runner::on_route_event_unhandled_func(uint32_t unhandled_event, AllegroFlare::Routers::Standard* router, void* user_data)
{
   if (!(router))
   {
      std::stringstream error_message;
      error_message << "[Runner::on_route_event_unhandled_func]: error: guard \"router\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::on_route_event_unhandled_func: error: guard \"router\" not met");
   }
   if (!(user_data))
   {
      std::stringstream error_message;
      error_message << "[Runner::on_route_event_unhandled_func]: error: guard \"user_data\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::on_route_event_unhandled_func: error: guard \"user_data\" not met");
   }
   Pipeline::Runner* this_runner = static_cast<Pipeline::Runner*>(user_data);

   bool handled = false;

   switch(unhandled_event)
   {
      //case MY_ROUTE_EVENT_ACTIVATE_MY_CUSTOM_SCREEN:
         //router->activate_screen(MY_CUSTOM_SCREEN_IDENTIFIER);
         //handled = true;
      //break;
   }

   return handled;
}

void Runner::continue_from_last_save()
{
   // TODO: This method
   return;
}

void Runner::setup_new_game_progress_and_state_info(AllegroFlare::GameSession* game_session)
{
   if (!(game_session))
   {
      std::stringstream error_message;
      error_message << "[Runner::setup_new_game_progress_and_state_info]: error: guard \"game_session\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::setup_new_game_progress_and_state_info: error: guard \"game_session\" not met");
   }
   // TODO: This method
   AllegroFlare::GameProgressAndStateInfos::Base *game_progress_and_state_info =
     game_session->get_game_progress_and_state_info();
   return;
}

void Runner::load_last_played_session_or_start_new(AllegroFlare::GameSession* game_session)
{
   if (!(game_session))
   {
      std::stringstream error_message;
      error_message << "[Runner::load_last_played_session_or_start_new]: error: guard \"game_session\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::load_last_played_session_or_start_new: error: guard \"game_session\" not met");
   }
   // TODO: This method
   AllegroFlare::GameProgressAndStateInfos::Base *game_progress_and_state_info =
     game_session->get_game_progress_and_state_info();
   return;
}

void Runner::setup_router()
{
   router.set_screen_manager(&framework->get_screen_manager_ref());
   router.set_event_emitter(event_emitter);

   // Set handlers for route events
   router.set_on_route_event_unhandled_func(on_route_event_unhandled_func);
   router.set_on_route_event_unhandled_func_user_data(this);

   // Set the "load_level" handler
   router.set_load_level_handler([this](AllegroFlare::RouteEventDatas::Base* route_event_data) -> bool {
      // TODO: Fill in the body of this function:
      // TODO: Validate route_event_data is non-void
      // TODO: Validate route_event_data is StartLevel type

      AllegroFlare::RouteEventDatas::StartLevel *as_start_level =
         static_cast<AllegroFlare::RouteEventDatas::StartLevel*>(route_event_data);

      std::string level_identifier_to_load = as_start_level->get_level_identifier();

      // TODO: Modify this design to emit a level loading event, instead of loading here explicitly.
      primary_gameplay_screen.load_level_by_identifier(level_identifier_to_load);

      // TODO: Improve design and address a "return false" if the level did not load or does not exist
      // TODO: Address concern that return type of lambda may not be the same signature of the load_level_handler
      return true;
   });

   // Set the callbacks related related to GameSession
   router.set_on_create_new_session_func(
      [this](AllegroFlare::Routers::Standard* screen, void* user_data) {
         // TODO: Test this method
         AllegroFlare::GameSession &game_session = screen->get_game_session_ref();
         this->setup_new_game_progress_and_state_info(&game_session); // NOTE: user_data is not necessary
      }
   );
   //router.set_on_create_new_session_func_user_data(this);

   router.set_on_load_last_played_session_or_start_new_func(
      [this](AllegroFlare::Routers::Standard* screen, void* user_data) {
         // TODO: Test this method
         AllegroFlare::GameSession &game_session = screen->get_game_session_ref();
         // NOTE: user_data is not necessary
         this->load_last_played_session_or_start_new(&game_session);
      }
   );
   //router.set_on_load_last_played_session_or_start_new_func_user_data(this); // NOTE: user_data is not necessary

   // Set the routes
   router.register_screen(
      AllegroFlare::Routers::Standard::INTRO_LOGOS_SCREEN_IDENTIFIER,
      &intro_logos_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::INTRO_STORYBOARD_SCREEN_IDENTIFIER,
      &intro_storyboard_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::TITLE_SCREEN_IDENTIFIER,
      &title_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::ACHIEVEMENTS_SCREEN_IDENTIFIER,
      &achievements_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::VERSION_SCREEN_IDENTIFIER,
      &version_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::NEW_GAME_INTRO_STORYBOARD_SCREEN_IDENTIFIER,
      &new_game_intro_storyboard_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::LEVEL_SELECT_SCREEN_IDENTIFIER,
      &level_select_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::GAME_OVER_SCREEN_IDENTIFIER,
      &game_over_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::GAME_WON_SCREEN_IDENTIFIER,
      &game_won_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::SETTINGS_SCREEN_IDENTIFIER,
      &settings_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::GAME_WON_OUTRO_STORYBOARD_SCREEN_IDENTIFIER,
      &game_won_outro_storyboard_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::LOAD_A_SAVED_GAME_SCREEN_IDENTIFIER,
      &load_a_saved_game_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::CREDITS_SCREEN_IDENTIFIER,
      &rolling_credits_screen
   );
   router.register_screen(
      AllegroFlare::Routers::Standard::PRIMARY_GAMEPLAY_SCREEN_IDENTIFIER,
      &primary_gameplay_screen
   );

   // Set the callbacks
   intro_logos_screen.set_on_finished_callback_func(
      [this](AllegroFlare::Screens::Storyboard* screen, void* data) {
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_INTRO_LOGOS_SCREEN_FINISHED,
            nullptr,
            al_get_time()
         );
      }
   );
   intro_storyboard_screen.set_on_finished_callback_func(
      [this](AllegroFlare::Screens::Storyboard* screen, void* data) {
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_INTRO_STORYBOARD_SCREEN_FINISHED,
            nullptr,
            al_get_time()
         );
      }
   );
   load_a_saved_game_screen.set_on_menu_choice_callback_func(
      [this](AllegroFlare::LoadASavedGame::Screen* screen, void* data) {
         // TODO: Handle here
         AllegroFlare::Logger::info_from(
           "Router::setup_router",
           "in on_menu_choice_callback_func, Currently there is no action implemented on this callback"
        );
      }
   );
   load_a_saved_game_screen.set_on_erase_focused_save_slot_func(
      [this](AllegroFlare::LoadASavedGame::Screen* screen, void* data) {
         // TODO: Handle here
         AllegroFlare::Logger::info_from(
           "Router::setup_router",
           "in on_erase_focused_save_slot_func, Currently there is no action implemented on this callback"
        );
      }
   );
   load_a_saved_game_screen.set_on_exit_callback_func(
      [this](AllegroFlare::LoadASavedGame::Screen* screen, void* data) {
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_ACTIVATE_TITLE_SCREEN,
            nullptr,
            al_get_time()
         );
      }
   );
   new_game_intro_storyboard_screen.set_on_finished_callback_func(
      [this](AllegroFlare::Screens::Storyboard* screen, void* data) {
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_NEW_GAME_INTRO_STORYBOARD_SCREEN_FINISHED,
            nullptr,
            al_get_time()
         );
      }
   );
   level_select_screen.set_on_menu_choice_callback_func(
      [this](AllegroFlare::Screens::LevelSelectScreen* screen, void* data) {
         // TODO: validate "screen" is non-void

         std::string selected_menu_option_value = screen->infer_current_menu_option_value();

         AllegroFlare::RouteEventDatas::StartLevel *route_event_data = new AllegroFlare::RouteEventDatas::StartLevel;
         route_event_data->set_level_identifier(selected_menu_option_value);

         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_START_LEVEL,
            route_event_data,
            al_get_time()
         );
      }
   );
   primary_gameplay_screen.set_on_finished_callback_func(
      [this](Pipeline::Gameplay::Screen* screen, void* data) {
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_PRIMARY_GAMEPLAY_SCREEN_FINISHED,
            nullptr,
            al_get_time()
         );
      }
   );
   settings_screen.set_on_exit_callback_func(
      [this](AllegroFlare::Screens::SettingsScreen* screen, void* data) {
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_ACTIVATE_TITLE_SCREEN, // NOTE: If the settings screen is
                                                                          // made accessible during gameplay,
                                                                          // this event will need to modified
                                                                          // for that case
            nullptr,
            al_get_time()
         );
      }
   );
   game_won_screen.set_on_submit_callback_func(
      [this](AllegroFlare::Screens::GameWonScreen* screen, void* data) {
         // TODO: validate "screen" is non-void

         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_ACTIVATE_TITLE_SCREEN,
            nullptr,
            al_get_time()
         );
      }
   );
   game_over_screen.set_on_menu_choice_callback_func(
      [this](AllegroFlare::Screens::GameOverScreen* screen, void* data) {
         // TODO: validate "screen" is non-void

         std::string menu_choice = screen->infer_current_menu_option_value();

         if (menu_choice == "try_again")
         {
            this->router.emit_route_event(
               AllegroFlare::Routers::Standard::EVENT_CONTINUE_FROM_LAST_SAVE, // NOTE: This event currently does not
                                                                               // have an effect
               nullptr,
               al_get_time()
            );
         }
         else if (menu_choice == "start_title_screen")
         {
            this->router.emit_route_event(
               AllegroFlare::Routers::Standard::EVENT_ACTIVATE_TITLE_SCREEN, // TODO: Have this activate a
                                                                             // confirmation dialog
               nullptr,
               al_get_time()
            );
         }
         else
         {
            AllegroFlare::Logger::throw_error("Boobaz", "buzz");
         }
      }
   );
   game_won_outro_storyboard_screen.set_on_finished_callback_func(
      [this](AllegroFlare::Screens::Storyboard* screen, void* data) {
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_GAME_WON_OUTRO_STORYBOARD_SCREEN_FINISHED,
            nullptr,
            al_get_time()
         );
      }
   );
   rolling_credits_screen.set_on_finished_callback_func(
      [this](AllegroFlare::Screens::RollingCredits* screen, void* data) {
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_CREDITS_SCREEN_FINISHED,
            nullptr,
            al_get_time()
         );
      }
   );
   title_screen.set_on_finished_callback_func(
      [this](AllegroFlare::Screens::TitleScreen* screen, void* data) {
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_TITLE_SCREEN_FINISHED,
            nullptr,
            al_get_time()
         );
      }
   );
   title_screen.set_on_menu_choice_callback_func(
      [this](AllegroFlare::Screens::TitleScreen* screen, void* data) {
         std::string menu_choice = screen->infer_current_menu_option_value();

         if (menu_choice == "start_new_game")
         {
            // TODO: If current game session is valid and has a running GameProgressAndStateInfo,
            // Emit a confirmation dialog "this will destroy the current game, do you wish to
            // continue" OR move the gamer to a "pick a save slot to start new game" screen
            this->router.emit_route_event(
               AllegroFlare::Routers::Standard::EVENT_START_NEW_GAME,
               nullptr,
               al_get_time()
            );
         }
         else if (menu_choice == "continue_from_last_save")
         {
            this->continue_from_last_save();
         }
         else if (menu_choice == "quit")
         {
            this->router.emit_route_event(
               AllegroFlare::Routers::Standard::EVENT_EXIT_GAME,
               nullptr,
               al_get_time()
            );
         }
         else if (menu_choice == "goto_load_a_saved_game_screen")
         {
            this->router.emit_route_event(
               AllegroFlare::Routers::Standard::EVENT_ACTIVATE_LOAD_A_SAVED_GAME_SCREEN,
               nullptr,
               al_get_time()
            );
         }
         else if (menu_choice == "goto_achievements_screen")
         {
            this->router.emit_route_event(
               AllegroFlare::Routers::Standard::EVENT_ACTIVATE_ACHIEVEMENTS_SCREEN,
               nullptr,
               al_get_time()
            );
         }
         else if (menu_choice == "goto_version_screen")
         {
            this->router.emit_route_event(
               AllegroFlare::Routers::Standard::EVENT_ACTIVATE_VERSION_SCREEN,
               nullptr,
               al_get_time()
            );
         }
         else if (menu_choice == "goto_settings_screen")
         {
            this->router.emit_route_event(
               AllegroFlare::Routers::Standard::EVENT_ACTIVATE_SETTINGS_SCREEN,
               nullptr,
               al_get_time()
            );
         }
         else if (menu_choice == "goto_credits_screen")
         {
            this->router.emit_route_event(
               AllegroFlare::Routers::Standard::EVENT_ACTIVATE_CREDITS_SCREEN,
               nullptr,
               al_get_time()
            );
         }
         else
         {
            AllegroFlare::Logger::throw_error("Foobar", "baz");
         }
      }
   );
   achievements_screen.set_on_exit_callback_func(
      [this](AllegroFlare::Screens::Achievements* screen, void* data) {
         // TODO: This should be a push/pop'd screen
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_ACTIVATE_TITLE_SCREEN,
            nullptr,
            al_get_time()
         );
      }
   );
   version_screen.set_on_exit_callback_func(
      [this](AllegroFlare::Screens::Version* screen, void* data) {
         // TODO: This should be a push/pop'd screen
         this->router.emit_route_event(
            AllegroFlare::Routers::Standard::EVENT_ACTIVATE_TITLE_SCREEN,
            nullptr,
            al_get_time()
         );
      }
   );

   return;
}

void Runner::run(std::string deployment_environment_mode)
{
   // setup the framework
   AllegroFlare::Frameworks::Full framework;
   framework.set_deployment_environment(deployment_environment_mode);
   //if (disable_escape_key_will_shutdown) framework.disable_escape_key_will_shutdown();
   framework.set_render_surface_multisamples(0);
   framework.disable_mipmapping();
   framework.initialize();

   // instantiate our actual game runner
   Runner runner(
      &framework,
      &framework.get_event_emitter_ref(),
      &framework.get_bitmap_bin_ref(),
      &framework.get_font_bin_ref(),
      &framework.get_model_bin_ref()
   );
   runner.initialize();
   framework.register_screen("runner", &runner);

   framework.run_loop();
   return;
}


} // namespace Pipeline


