#include "ocean.hpp"
#include "entities/algae.hpp"
#include "entities/herbivore.hpp"
#include "entities/predator.hpp"
#include "utils/rng.hpp"
#include "constants.hpp"

#include <vector>
#include <memory>

// Impl
struct Ocean::Impl {
    int w{}, h{};
    std::vector<std::vector<std::unique_ptr<Entity>>> grid;
    Impl(int W,int H):w(W),h(H){ grid.resize(H); for(auto& r:grid) r.resize(W); }
};

// реализация spawn<T>
template<class T,class... Args>
void Ocean::spawn(int x,int y, Args&&... args)
{
    if (isEmpty(x,y))
        pImpl->grid[y][x] =
            std::make_unique<T>(std::forward<Args>(args)...);
}

// явные инстанциации — чтобы шаблон сгенерировался один раз
template void Ocean::spawn<Herbivore>(int,int);
template void Ocean::spawn<Predator >(int,int);
// конструктор: посев
Ocean::Ocean(int w,int h):pImpl(std::make_unique<Impl>(w,h))
{
    // ковёр водорослей в нижнем ряду
    for(int x=0;x<w;++x)
        if(Rng::instance().chance(60))
            pImpl->grid[h-1][x]=std::make_unique<Algae>();

    // 4 травоядных, 2 хищника — над зеленушкой
    const int waterTop = h - MAX_ALGAE - 5;
    const std::array<int,4> herbx{ w/2, w/2-12, w/3, 2*w/3 };
    for(int ix:herbx)
        pImpl->grid[waterTop][ix] = std::make_unique<Herbivore>();

    const std::array<int,2> predx{ w/4, 3*w/4 };
    for(int ix:predx)
        pImpl->grid[waterTop-4][ix] = std::make_unique<Predator>();
}
Ocean::~Ocean()=default;
Ocean& Ocean::operator=(Ocean&&o)noexcept{pImpl=std::move(o.pImpl);return *this;}

// размеры
int Ocean::width()  const{ return pImpl->w; }
int Ocean::height() const{ return pImpl->h; }
int Ocean::minAlgaeRow() const{ return pImpl->h - MAX_ALGAE; }

// клетки 
bool Ocean::isEmpty(int x,int y)const{
    return x>=0&&y>=0&&x<width()&&y<height()&&!pImpl->grid[y][x];
}
Entity* Ocean::entityAt(int x,int y)const{
    return (x>=0&&y>=0&&x<width()&&y<height())?pImpl->grid[y][x].get():nullptr;
}
void Ocean::plantAlgae(int x,int y){
    if(isEmpty(x,y))
        pImpl->grid[y][x]=std::make_unique<Algae>();
}


void Ocean::moveEntity(int x1,int y1,int x2,int y2){
    if(!isEmpty(x2,y2)) return;
    pImpl->grid[y2][x2]=std::move(pImpl->grid[y1][x1]);
}
void Ocean::clearCell(int x,int y){ if(!isEmpty(x,y)) pImpl->grid[y][x].reset(); }

// ▼ опустить стебель после укуса
void Ocean::shiftColumnDown(int x,int fromY)
{
    for(int y=fromY; y>=0; --y)
    {
        if(y+1>=height()) break;
        if(!isEmpty(x,y+1)) break;       // ниже уже занято
        if(isEmpty(x,y))   continue;     // на этом уровне пусто
        moveEntity(x,y,x,y+1);
    }
}

// статистика, update, draw (без изменений, не робит)
Population Ocean::population() const{ Population p{}; for(auto& r:pImpl->grid)for(auto&e:r)if(e){ if( dynamic_cast<Algae*>(e.get()))++p.algae; else if(dynamic_cast<Herbivore*>(e.get()))++p.herbivores; else ++p.predators; } return p; }
void Ocean::update(){ for(int y=height()-1;y>=0;--y) for(int x=0;x<width();++x) if(pImpl->grid[y][x]) pImpl->grid[y][x]->update(*this,x,y); }
void Ocean::draw(sf::RenderTarget& tgt) const{ for(int y=0;y<height();++y) for(int x=0;x<width();++x) if(pImpl->grid[y][x]) pImpl->grid[y][x]->draw(tgt,CELL,float(x),float(y)); }
