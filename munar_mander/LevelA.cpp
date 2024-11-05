#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 10
#define LEVEL_HEIGHT 8

constexpr char SPRITESHEET_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/sprites.png",
           ENEMY_FILEPATH[]       = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/soph.png",
            MAP_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/mars_map.png",
          FONT_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/blue_font.png";

unsigned int LEVELA_DATA[] = {
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 4 , 4 , 4 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
    
};

LevelA::~LevelA() {
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise(ShaderProgram *program) {
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture(MAP_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 7, 3);
    
    A_font_texture_id = Utility::load_texture(FONT_FILEPATH);
    
    // Existing
    int player_animation[4][4] = {
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
        player_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        1,                         // animation column amount
        4,                         // animation row amount
        0.3f,                      // width
        0.4f,                       // height
        PLAYER
    );
        
    m_game_state.player->set_position(glm::vec3(2.0f, 0.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(3.0f);
    
    /**
    Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++) {
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
}

void LevelA::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    if (m_game_state.player->get_position().y < -10.0f) m_game_state.next_scene_id = 1;
}

void LevelA::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    Utility::draw_text(program, A_font_texture_id, "WELCOME TO MARS!", 0.5f, 0.01f, glm::vec3(1.0f, -1.0f , 0.0f));
    m_game_state.player->render(program);
    
}
