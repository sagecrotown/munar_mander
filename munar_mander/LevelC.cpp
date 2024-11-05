#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 60
#define LEVEL_HEIGHT 45

constexpr char SPRITESHEET_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/sprite_sheet.png",
           ENEMY_FILEPATH[]       = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/soph.png",
            MAP_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/mars_map.png",
            FONT_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/red_font.png",
            CSV_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/blank.csv";

unsigned int LEVELC_DATA[LEVEL_WIDTH * LEVEL_HEIGHT];

LevelC::~LevelC()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelC::initialise(ShaderProgram *program)
{
    m_game_state.next_scene_id = -1;
    
    C_font_texture_id = Utility::load_texture(FONT_FILEPATH);
    
    Utility::readCSV(CSV_FILEPATH, LEVELC_DATA, LEVEL_WIDTH * LEVEL_HEIGHT);
    GLuint map_texture_id = Utility::load_texture(MAP_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 7, 3);
    
    int player_walking_animation[4][4] = {
        { 0 , 0 , 0 , 0},   // dead
        { 1 , 1 , 1 , 1},   // landed safely
        { 2 , 2 , 2 , 2},   // playing
        { 3 , 3 , 3 , 3},   // blasting
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -4.81f, 0.0f);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    
    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        5.0f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        1,                         // animation column amount
        3,                         // animation row amount
        1.0f,                      // width
        1.0f,                       // height
        PLAYER
    );
        
    m_game_state.player->set_position(glm::vec3(5.0f, -5.0f, 0.0f));
    m_game_state.player->deactivate();
    
    /**
    Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, GUARD, IDLE);
    }

    m_game_state.enemies[0].set_position(glm::vec3(8.0f, 0.0f, 0.0f));
    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/dooblydoo.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(0.0f);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
    
    // Font stuff:
}

void LevelC::update(float delta_time) {
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
}

void LevelC::render(ShaderProgram *program) {
    m_game_state.map->render(program);
    
    Utility::draw_text(program, C_font_texture_id, "FUEL: ", 1.0f, 0.1f, glm::vec3(1.0f, -3.75f , 0.0f));
    Utility::draw_text(program, C_font_texture_id, std::to_string(fuel_count), 1.0f, 0.1f, glm::vec3(6.9f, -3.75f , 0.0f));
    
    Utility::draw_text(program, C_font_texture_id, "PRESS ENTER TO START", 0.15f, 0.01f, glm::vec3(1.0f, -7.0f, 0.0f));
    
}

