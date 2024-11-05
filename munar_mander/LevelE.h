#include "Scene.h"

class LevelE : public Scene {
public:
    int ENEMY_COUNT = 1;
    GLuint E_font_texture_id;
    glm::vec3 message_pos;
    glm::vec3 fuel_pos;
    
    ~LevelE();
    
    void initialise(ShaderProgram *program) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
