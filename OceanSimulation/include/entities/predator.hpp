#ifndef PREDATOR_HPP
#define PREDATOR_HPP
#include "entity.hpp"

class Predator : public Entity
{
public:
    Predator() = default;                  // hunger_=0, age_=0
    void update(class Ocean& oc,int x,int y) override;
    void draw (sf::RenderTarget&,float,float,float) const override;
private:
    int hunger_ = 0;
    int age_    = 0;
};
#endif
