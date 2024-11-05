#include "Scene.h"

class LevelF : public Scene {
public:
    int ENEMY_COUNT = 1;
    GLuint F_font_texture_id;
    glm::vec3 message_pos;
    glm::vec3 fuel_pos;
    
    ~LevelF();
    
    void initialise(ShaderProgram *program) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
