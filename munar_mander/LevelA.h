#include "Scene.h"

class LevelA : public Scene {
public:
    int ENEMY_COUNT = 1;
    GLuint A_font_texture_id;
    
    ~LevelA();
    
    void initialise(ShaderProgram *program) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
