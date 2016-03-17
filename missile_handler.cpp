#include "missile_handler.h"
#include "context_scene.h"
#include "shoot.h"

extern ContextScene scene_;

MissileHandler::MissileHandler(const SDL_Color& color, const Uint32 cooldown) : color_(color), cooldown_(cooldown) {

}

MissileHandler::~MissileHandler() {

}

void MissileHandler::cast_missile(const Point& pos, const Point& focus) {
    if(Shoot::is_castable(last_cast_time_, cooldown_)) {
        ShootPtr shoot_ptr = std::make_shared<Shoot>(pos, focus, 20.f, color_);  
        scene_.add_entity(shoot_ptr);
    }
}