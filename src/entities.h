
#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <tuple>
#include <string>
#include <utility>
#include <SFML/Graphics.hpp>
#include <map>

/* Vec2 class */
class Vec2
{
public:
    float x = 0;
    float y = 0;
    
    Vec2(float xin, float yin)
        : x(xin), y(yin) {}

    Vec2& add(const Vec2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vec2& mult(float f)
    {
        x *= f;
        y *= f;
        return *this;
    }

    Vec2 operator + (const Vec2& v)
    {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator * (float f)
    {
        return Vec2(x * f, y * f);
    }

    void print() const
    {
        std::cout << x << " " << y << std::endl;
    }
};

/* Entities and Components */
class Component
{
public:
    bool exists = false;
};

class CTransform : public Component {
    public:
        Vec2 pos = {0,0};
        Vec2 velocity = {0,0};
        CTransform() {}
        CTransform(const Vec2& p, const Vec2& v)
            : pos(p), velocity(v) {}
};
class CShape : public Component{
    public:
        sf::CircleShape shape;
        CShape() {}
        CShape(const float radius, const sf::Color& color)
            : shape(radius)
            {
                shape.setFillColor(color);
            }
};
class CLifespan  : public Component {
    public:
        int totalLifespan = 0;
        int remainingLifespan = 0;
        CLifespan() {}
        CLifespan(int tls)
            : totalLifespan(tls), remainingLifespan(tls) {}
};
class CInput     : public Component {};
class CBBox      : public Component {};
class CAnimation : public Component {};
class CGravity   : public Component {};
class CState     : public Component {};

/* Component Tuple */
using ComponentTuple = std::tuple<
    CTransform,
    CLifespan,
    CShape,
    CInput,
    CBBox,
    CAnimation,
    CGravity,
    CState
>;

class Entity
{
    ComponentTuple  m_components;       // m_... private member entity
    bool            m_alive = true;     // if false, entity will be deleted
    std::string     m_tag = "default";  // type of entity
    size_t          m_id = 0;           // unique integer id

public:
    Entity() = default;
    Entity(const std::string& tag, size_t id)
        : m_id(id), m_tag(tag)
    {}
    template <typename T, typename... TArgs>    // Generate overloaded functions
    T& add(TArgs&&... mArgs)
    {   
        auto& component = get<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.exists = true;
        return component;
    }

    template <typename T>
    T& get()
    {
        return std::get<T>(m_components);
    }
    // const version to keep const correctness
    template <typename T>
    T& get() const
    {
        return std::get<T>(m_components);
    }

    template <typename T>
    bool has() const{
        return get<T>().exists;
    }

    template <typename T>
    void remove()
    {
        get<T>() = T(); // Default construct (blanks) the component in the tuple
    }

    size_t id() const
    {
        return m_id;
    }

    bool isAlive() const
    {
        return m_alive;
    }

    void destroy()
    {
        m_alive = false;
    }

    const std::string& tag() const
    {
        return m_tag;
    }
};

/* Entity Manager */

using EntityVec = std::vector <std::shared_ptr<Entity>>;
using EntityMap = std::map    <std::string, EntityVec>;

class EntityManager
{
    EntityVec   m_entities;
    EntityMap   m_entityMap;
    size_t      m_totalEntities = 0;

public:
    EntityManager();
    std::shared_ptr<Entity> addEntity(const std::string& tag);
    EntityVec& getEntities();
    EntityVec& getEntities(const std::string& tag);

};

/* End Entities */

#endif