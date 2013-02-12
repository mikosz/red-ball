#ifndef RED_BALL_GAME_GAME_HPP_
#define RED_BALL_GAME_GAME_HPP_

#include <list>

#include <boost/shared_ptr.hpp>

#include "red_ball/utils/Timer.hpp"
#include "Actor.hpp"

namespace red_ball {
namespace game {

class Game {
public:

    typedef std::list<boost::shared_ptr<Actor> > Actors;

    typedef Actors::iterator ActorId;

    void update();

    ActorId addActor(boost::shared_ptr<Actor> actor);

    void removeActor(ActorId actorId);

private:

    utils::Timer timer_;

    Actors actors_;

};

} // namespace game
} // namespace red_ball

#endif /* RED_BALL_GAME_GAME_HPP_ */
