#include "EntityManager.h"

EntityManager::EntityManager() {}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {
	auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));	// this kinda sucks(not using make_shared)
	m_entitiesToAdd.emplace_back(e);

	return e;
}

EntityVec& EntityManager::getEntities() {
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag) {
	return m_entityMap[tag];
}

// trying some weird sh*t below might fk up

void EntityManager::removeDeadEntities(EntityVec& entityVec) {
	EntityVec temp;

	for (std::shared_ptr<Entity> e : entityVec) {
		if (e->isActive()) {
			temp.emplace_back(e);
		}
	}

	std::swap(temp, entityVec);
}

void EntityManager::update() {
	for (auto e : m_entitiesToAdd) {
		m_entities.emplace_back(e);
		m_entityMap[e->tag()].emplace_back(e);
	}

	m_entitiesToAdd.clear();

	removeDeadEntities(m_entities);

	for (auto& [tag, entityVec] : m_entityMap) {
		removeDeadEntities(entityVec);
	}
}