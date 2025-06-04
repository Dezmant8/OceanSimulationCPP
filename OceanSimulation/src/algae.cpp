#include "entities/algae.hpp"
#include "ocean.hpp"
#include "utils/rng.hpp"
#include "constants.hpp"

void Algae::update(Ocean& ocean,int x,int y)
{
    if (y <= ocean.minAlgaeRow()) return;                 // выше нельзя
    if (Rng::instance().chance(20) && ocean.isEmpty(x,y-1))
        ocean.plantAlgae(x,y-1);
}

void Algae::draw(sf::RenderTarget& tgt,float c,float gx,float gy) const
{
    sf::ConvexShape leaf(3);
    leaf.setPoint(0,{c*0.5f,0});
    leaf.setPoint(1,{c*0.8f,c});
    leaf.setPoint(2,{c*0.2f,c});
    leaf.setFillColor(sf::Color(30,180,30));
    leaf.setPosition({c*gx,c*gy});
    tgt.draw(leaf);
}
