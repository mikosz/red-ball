#include "Game.hpp"

#include <algorithm>

#include <boost/bind.hpp>

using namespace red_ball::game;

void Game::update() {
    float dt = timer_.tick();
    std::for_each(actors_.begin(), actors_.end(), boost::bind(&Actor::update, _1, dt));
}

Game::ActorId Game::addActor(boost::shared_ptr<Actor> actor) {
    actors_.push_front(actor);
    return actors_.begin();
}

void Game::removeActor(ActorId actorId) {
    actors_.erase(actorId);
}
