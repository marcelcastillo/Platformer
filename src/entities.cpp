#include <map>
#include "entities.h"

/* Vec2 Class */


/* Entity Manager */

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    std::shared_ptr<Entity> e = std::make_shared<Entity>(tag, m_totalEntities++);  // Create new Entity object
    m_entities.push_back(e);
    m_entityMap[tag].push_back(e);
    return e;
}