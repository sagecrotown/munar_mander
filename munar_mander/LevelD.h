#include "Scene.h"

class LevelD : public Scene {
public:
    int ENEMY_COUNT = 1;
    GLuint D_font_texture_id;
    glm::vec3 message_pos;
    glm::vec3 fuel_pos;
    
    ~LevelD();
    
    void initialise(ShaderProgram *program) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
