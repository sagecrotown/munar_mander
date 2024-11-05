#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 60
#define LEVEL_HEIGHT 45

constexpr char SPRITESHEET_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/sprite_sheet.png",
           ENEMY_FILEPATH[]       = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/soph.png",
            MAP_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/mars_map.png",
            FONT_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/mars_font.png",
            CSV_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/blank.csv";

unsigned int LEVELB_DATA[LEVEL_WIDTH * LEVEL_HEIGHT];

std::vector<std::string> MESSAGE = {"WELCOME! YOU ARE A GENETICALLY MODIFIED ", "HUMAN (THOUGH WHO ISN'T, THESE DAYS), ", "BRED SPECIFICALLY TO HANDLE SPACE TRAVEL. ", "YOU'VE BEEN TRAINING FOR THIS MISSION EVER ", "SINCE YOUR PARENTS SURRENDERED YOU TO ", "RAYTHEONSPACEXDRPEPPER AT AGE SIX--AND YOU ", "BARELY REMEMBER THEM, ANYWAY. YOU AND YOUR ", "BEST FRIEND, A SMALL MARTIAN ROCK GIFTED ", "TO YOU WHEN YOUR ROCKET-IMPLANT SURGERIES ", "WERE COMPLETED, HAVE BEEN DREAMING OF THE ", "DAY YOU COULD LAND ON A PLANET WITH A ", "GRAVITATIONAL ACCELERATION OF 3.73 M/S^2. ", "AND THAT DAY HAS FINALLY COME! LUCKILY, ", "YOU BELONG TO A RELATIVELY BENEVOLENT ", "PARENT COMPANY, AND YOU SHOULD HAVE PLENTY ", "OF FUEL TO LAND SAFELY. ", "", "PRESS F TO GET FUEL."};

                     

LevelB::~LevelB()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelB::initialise(ShaderProgram *program)
{
    m_game_state.next_scene_id = -1;
    
    B_font_texture_id = Utility::load_texture(FONT_FILEPATH);
    
    Utility::readCSV(CSV_FILEPATH, LEVELB_DATA, LEVEL_WIDTH * LEVEL_HEIGHT);
    GLuint map_texture_id = Utility::load_texture(MAP_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 7, 3);
    
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

}

void LevelB::update(float delta_time) {
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
}

void LevelB::render(ShaderProgram *program) {
    m_game_state.map->render(program);
    
    for (int i = 0; i < MESSAGE.size(); i++) {
        Utility::draw_text(program, B_font_texture_id, MESSAGE[i], 0.2f, 0.01f, glm::vec3(0.75f, -0.75f - (i * 0.35f), 0.0f));
    }
}
