#include "entities/predator.hpp"
#include "entities/herbivore.hpp"
#include "entities/algae.hpp"
#include "ocean.hpp"
#include "utils/rng.hpp"
#include "constants.hpp"
#include <array>
#include <algorithm>

namespace { const std::array<std::pair<int,int>,4> dirs{{{0,-1},{-1,0},{1,0},{0,1}}}; }

static bool isHerb(const Ocean& oc,int x,int y){ return dynamic_cast<Herbivore*>(oc.entityAt(x,y)); }
static bool isAlgae(const Ocean& oc,int x,int y){ return dynamic_cast<Algae*   >(oc.entityAt(x,y)); }


void Predator::update(Ocean& oc,int x,int y)
{
    ++age_;                                 // ▲ старение
    ++hunger_;

    if(age_ >= AGE_LIMIT && oc.population().predators >= MAX_POP)
    { oc.clearCell(x,y); return; }

    /* ближайшая цель в воде */
    int tx=-1,ty=-1,minD=99;
    for(int yy=0; yy<oc.minAlgaeRow(); ++yy)
        for(int xx=0; xx<oc.width(); ++xx)
            if(isHerb(oc,xx,yy)){
                int d=std::abs(xx-x)+std::abs(yy-y);
                if(d<minD){minD=d;tx=xx;ty=yy;}
            }

    int dir;
    if(tx>=0 && hunger_>=HUNGRY_PRED){
        dir = (std::abs(tx-x)>std::abs(ty-y))
              ? ((tx<x)?1:2) : ((ty<y)?0:3);
    }else{
        std::array<int,4> rnd{0,1,2,3};
        std::shuffle(rnd.begin(),rnd.end(),Rng::instance().engine());
        dir=rnd[0];
    }

    int nx=x+dirs[dir].first, ny=y+dirs[dir].second;
    if(nx<0||ny<0||nx>=oc.width()||ny>=oc.minAlgaeRow()) return;

    if(!oc.isEmpty(nx,ny) && !isAlgae(oc,nx,ny) && !isHerb(oc,nx,ny))
        return; // другой хищник

    bool ate=false;
    if(isHerb(oc,nx,ny)){ oc.clearCell(nx,ny); ate=true; hunger_=0; }
    if(isAlgae(oc,nx,ny)) oc.clearCell(nx,ny);

    oc.moveEntity(x,y,nx,ny);

    /* размножение с 30 % */
    if(ate && Rng::instance().chance(BIRTH_PRED) && oc.isEmpty(x,y))
        oc.spawn<Predator>(x,y);
}

void Predator::draw(sf::RenderTarget& tgt,float c,float gx,float gy) const
{
    sf::ConvexShape tri(3);
    tri.setPoint(0,{0,0}); tri.setPoint(1,{c,c*0.5f}); tri.setPoint(2,{0,c});
    tri.setFillColor(sf::Color(200,40,40));
    tri.setPosition({c*gx,c*gy});
    tgt.draw(tri);
}
