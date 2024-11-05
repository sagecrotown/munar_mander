#include "LevelE.h"
#include "Utility.h"

#define LEVEL_WIDTH 60
#define LEVEL_HEIGHT 45

constexpr char SPRITESHEET_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/sprites.png",
           ENEMY_FILEPATH[]       = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/soph.png",
            MAP_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/mars_map.png",
            FONT_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/mars_font.png",
            CSV_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/final_map.csv";

unsigned int LEVELE_DATA[LEVEL_WIDTH * LEVEL_HEIGHT];

static std::vector<std::string> MESSAGE = {"RAYTHEONSPACEXDRPEPPER NEVER COMES BACK FOR YOUR BODY, ", "THOUGH THEY DO PLAY A SLIDESHOW OF YOUR 'BEST MOMENTS' ", "AND GIVE EVERYONE ICE CREAM BACK AT THE DORMS. YOUR ", "ROCK IS NOT RECOVERED EITHER, BUT WHEN THE NEXT KID ", "RECOVERING FROM THE IMPLANTS IS GIVEN A NEW ONE, THEY ", "ARE TOLD IT ONCE BELONGED TO YOU. YOU DO NOT DIE ", "HAPPY, MOSTLY BECAUSE YOU DIED TOO QUICKLY TO REALIZE ", "IT WAS HAPPENING AT ALL."};

LevelE::~LevelE()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelE::initialise(ShaderProgram *program)
{
    m_game_state.next_scene_id = -1;
    
    E_font_texture_id = Utility::load_texture(FONT_FILEPATH);
    
    Utility::readCSV(CSV_FILEPATH, LEVELE_DATA, LEVEL_WIDTH * LEVEL_HEIGHT);
    GLuint map_texture_id = Utility::load_texture(MAP_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELE_DATA, map_texture_id, 1.0f, 7, 3);
    
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
        4,                         // animation row amount
        1.0f,                      // width
        1.0f,                       // height
        PLAYER
    );
        
    m_game_state.player->set_position(glm::vec3(30.0f, -40.0f, 0.0f));
    m_game_state.player->set_scale(glm::vec3(20.0f, 20.0f, 1.0f));
    m_game_state.player->face_left();
    
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

void LevelE::update(float delta_time) {
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
}

void LevelE::render(ShaderProgram *program) {
    m_game_state.map->render(program);
    Utility::draw_text(program, E_font_texture_id, "YOU LOSE!", 2.0f, 0.01f, glm::vec3(20.0f, -6.0f, 0.0f));
    for (int i = 0; i < MESSAGE.size(); i++) {
        Utility::draw_text(program, E_font_texture_id, MESSAGE[i], 1.0f, 0.01f, glm::vec3(3.0f, -10.5f - (i * 2.0f), 0.0f));
    }
    m_game_state.player->render(program);
    
}
