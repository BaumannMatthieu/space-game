#include <iostream>
#include "ship.h"
#include "math.h"

Ship::Ship(const Rectangle& box) : Entity(box), life_(100),
rad_focus_(500), rad_attack_(300), rad_near_(100) {
	mass_ = 5;
    max_velocity_ = 10.0f;
    max_force_ = 15; 
    //velocity_ = Vector2<float>((static_cast<float>(std::rand())/RAND_MAX)*2.f - 1.f, (static_cast<float>(std::rand())/RAND_MAX)*2.f - 1.f); 
	velocity_ = Vector2<float>(-0.1f, -0.1f);
	
	StatePtr wandering = std::make_shared<State>([this] (const StatePtr current_state) {
		/* wandering algorithme */
		force_ = Vector2<float>(0.f, 0.f);
		std::cout << "wandering" << std::endl;

		Point seek_pos(512.f, 384.f);   
		float distance = Vector2<float>::distance(seek_pos, center_mass_);
		
		if(distance <= rad_near_) {
			current_states_.erase(current_state);

			current_states_.insert(current_state->get_next_state(State::FLEEING));
		} else if(distance > rad_near_ && distance <= rad_attack_) {
			current_states_.erase(current_state);

			//velocity_ = Vector2<float>(0.0f, 0.f);
			current_states_.insert(current_state->get_next_state(State::CIRCULAR_MOVING));
			//current_states_.insert(current_state->get_next_state(State::ATTACKING));
		} else if(distance > rad_attack_ && distance <= rad_focus_) {
			current_states_.erase(current_state);
		
			current_states_.insert(current_state->get_next_state(State::SEEKING));
		}
	});

	StatePtr seeking = std::make_shared<State>([this] (const StatePtr current_state) {
		force_ = compute_seek_force();

		std::cout << "seeking" << std::endl;
		Point seek_pos(512.f, 384.f);   
		float distance = Vector2<float>::distance(seek_pos, center_mass_);
		
		if(distance > rad_near_ && distance <= rad_attack_) {
			current_states_.erase(current_state);
			//velocity_ = Vector2<float>(0.0f, 0.f);
			current_states_.insert(current_state->get_next_state(State::CIRCULAR_MOVING));
			//current_states_.insert(current_state->get_next_state(State::ATTACKING));
		} else if(distance > rad_focus_) {
			current_states_.erase(current_state);
			current_states_.insert(current_state->get_next_state(State::WANDERING));
		}
	});

	StatePtr fleeing = std::make_shared<State>([this] (const StatePtr current_state) {
		force_ = compute_flee_force();
		std::cout << "fleeing" << std::endl;
		Point seek_pos(512.f, 384.f);   
		float distance = Vector2<float>::distance(seek_pos, center_mass_);
		
		if(distance >= rad_attack_) {
			current_states_.erase(current_state);

			//velocity_ = Vector2<float>(0.0f, 0.f);
			current_states_.insert(current_state->get_next_state(State::SEEKING));
			//current_states_.insert(current_state->get_next_state(State::ATTACKING));
		}
	});
	
	StatePtr circular_moving = std::make_shared<State>([this] (const StatePtr current_state) {
		force_ = compute_seek_force();
		std::cout << "moving circular & attacking" << std::endl;
		
		Point seek_pos(512.f, 384.f);   
		float distance = Vector2<float>::distance(seek_pos, center_mass_);
		
		if(distance <= rad_near_) {
			current_states_.erase(current_state);

			current_states_.insert(current_state->get_next_state(State::FLEEING));
		} else if(distance > rad_attack_ && distance <= rad_focus_) {
			current_states_.erase(current_state);
		
			current_states_.insert(current_state->get_next_state(State::SEEKING));
		}
	});

	wandering->set_next_state(State::FLEEING, fleeing);
	wandering->set_next_state(State::CIRCULAR_MOVING, circular_moving);
	wandering->set_next_state(State::SEEKING, seeking);

	fleeing->set_next_state(State::SEEKING, seeking);

	seeking->set_next_state(State::WANDERING, wandering);
	seeking->set_next_state(State::CIRCULAR_MOVING, circular_moving);

	circular_moving->set_next_state(State::FLEEING, fleeing);
	circular_moving->set_next_state(State::SEEKING, seeking);

	current_states_.insert(wandering);
}

Ship::~Ship() {
    
}

void Ship::update() {
	//execute();
    force_ = compute_arrive_force();
    Math::truncate(force_, max_force_);
	
	acceleration_ = force_/mass_;
    velocity_ = velocity_ + acceleration_;
    Math::truncate(velocity_, max_velocity_);

	move();
}

void Ship::move() {
    box_.translate(velocity_);
    center_mass_ = center_mass_ + velocity_;
}
