#pragma once

#include "Entity.h"
#include "common.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::unordered_map<std::string, EntityVec> EntityMap;

class EntityManager {
private:
	EntityVec	m_entities;
	EntityVec	m_entitiesToAdd;
	EntityMap	m_entityMap;
	size_t		m_totalEntities = 0;

	void removeDeadEntities(EntityVec& vec);

public:
	EntityManager();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	EntityVec& getEntities(const std::string& tag);
	EntityVec& getEntities();
	void update();
};