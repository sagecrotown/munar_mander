#include "LevelD.h"
#include "Utility.h"

#define LEVEL_WIDTH 500
#define LEVEL_HEIGHT 140

constexpr char SPRITESHEET_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/sprites.png",
           ENEMY_FILEPATH[]       = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/soph.png",
            MAP_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/mars_map.png",
          FONT_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/blue_font.png",
            CSV_FILEPATH[] = "/Users/Sage/Downloads/Game Programming/munar_mander/munar_mander/assets/mars_map_bigger.csv";

unsigned int LEVELD_DATA[LEVEL_WIDTH * LEVEL_HEIGHT];

LevelD::~LevelD()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelD::initialise(ShaderProgram *program) {
    
    m_game_state.next_scene_id = -1;
    
    Utility::readCSV(CSV_FILEPATH, LEVELD_DATA, LEVEL_WIDTH * LEVEL_HEIGHT);
    GLuint map_texture_id = Utility::load_texture(MAP_FILEPATH);
    
//    for (int i = 0; i < LEVEL_HEIGHT; i++) {
//        for (int j = 0; j < LEVEL_WIDTH; j++) {
//            std::cout << LEVELD_DATA[i*LEVEL_WIDTH + j] << " ";
//        }
//        std::cout << std::endl;
//    }
    
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELD_DATA, map_texture_id, 1.0f, 7, 3);
    
    D_font_texture_id = Utility::load_texture(FONT_FILEPATH);
    
    // excess, too lazy to change array size
    int player_animation[4][4] = {
        { 0 , 0 , 0 , 0},   // dead
        { 1 , 1 , 1 , 1},   // landed safely
        { 2 , 2 , 2 , 2},   // playing
        { 3 , 3 , 3 , 3},   // blasting
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -3.73f, 0.0f);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    
    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        15.0f,                      // jumping power
        player_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        1,                         // animation column amount
        4,                         // animation row amount
        0.6f,                      // width
        0.8f,                       // height
        PLAYER
    );
        
    m_game_state.player->set_position(glm::vec3(2.0f, 10.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(5.0f);
    m_game_state.player->set_scale(glm::vec3(2.0f, 2.0f, 1.0f));
    
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
    
    // ShaderProgram stuff
    glm::mat4 new_projection_matrix = glm::ortho(-10.0f, 10.0f, -7.5f, 7.5f, -1.0f, 1.0f);
    
    program->set_projection_matrix(new_projection_matrix);
}

void LevelD::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    float view_left = std::max(m_game_state.player->get_position().x, 30.0f);
    float view_bottom = std::max(m_game_state.player->get_position().y - 18, -115.0f);
    
    message_pos = glm::vec3(view_left + 20, view_bottom + 20, 0.0f);
    fuel_pos = message_pos;
    fuel_pos.x += 5;
    
//    if (m_game_state.player->get_position().y < -10.0f) m_game_state.next_scene_id = 1;
    if (m_game_state.player->get_collided_left() || m_game_state.player->get_collided_right()) {
        std::cout << "loser! " << std::endl;
        m_game_state.next_scene_id = 4;     // LOSER
    }
    else if (m_game_state.player->get_collided_bottom() && m_game_state.player->get_on_triangle()) {
        std::cout << "loser! (triangle) " << std::endl;
        m_game_state.next_scene_id = 4;     // LOSER
    }
    else if (m_game_state.player->get_collided_bottom()) {
        std::cout << "winner! " << std::endl;
        m_game_state.next_scene_id = 5;     // WINNER
    }
}

void LevelD::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    Utility::draw_text(program, D_font_texture_id, "FUEL: ", 1.0f, 0.01f, message_pos);
    Utility::draw_text(program, D_font_texture_id, std::to_string(fuel_count),  1.0f, 0.01f, fuel_pos);
    m_game_state.player->render(program);
    
}
