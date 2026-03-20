
#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <tuple>
#include <string>
#include <utility>

/* Vec2 class */
class Vec2
{
public:
    float x = 0;
    float y = 0;
    
    Vec2() {}
    Vec2(float xin, float yin)
        : x(xin), y(yin) {}

    void print()
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

struct CTransform : public Component {};
struct CLifespan  : public Component {};
struct CInput     : public Component {};
struct CBBox      : public Component {};
struct CAnimation : public Component {};
struct CGravity   : public Component {};
struct CState     : public Component {};

using ComponentTuple = std::tuple<
    CTransform,
    CLifespan,
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

/* End Entities */

#endif