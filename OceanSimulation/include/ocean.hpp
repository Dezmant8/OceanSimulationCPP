#ifndef OCEAN_HPP
#define OCEAN_HPP

#include <memory>
#include <SFML/Graphics.hpp>

struct Population { int algae{}, herbivores{}, predators{}; };

class Entity;      class Algae;
class Herbivore;   class Predator;

// ─────────────────────────────────────────────────────────────
class Ocean
{
public:
    Ocean(int w, int h);
    ~Ocean();

    Ocean(const Ocean&)            = delete;
    Ocean& operator=(const Ocean&) = delete;
    Ocean(Ocean&&)  noexcept       = default;
    Ocean& operator=(Ocean&&) noexcept;

    // ── основной цикл ───────────────────────────────────────
    void update();
    void draw(sf::RenderTarget& tgt) const;

    // ── геометрия поля ─────────────────────────────────────
    int width()  const;
    int height() const;
    int minAlgaeRow() const;              // y-координата ковра водорослей

    // ── работа с клетками ─────────────────────────────────
    bool    isEmpty(int x,int y) const;
    Entity* entityAt(int x,int y) const;
    void    plantAlgae(int x,int y);
    void    moveEntity(int x1,int y1,int x2,int y2);
    void    clearCell(int x,int y);
    void    shiftColumnDown(int x,int fromY);

    // ── появление нового организма (реализация в ocean.cpp)
    template<class T,class... Args>
    void spawn(int x,int y, Args&&... args);

    // ── статистика ────────────────────────────────────────
    Population population() const;

private:
    struct Impl;                       // определена в ocean.cpp
    std::unique_ptr<Impl> pImpl;
};

#endif
