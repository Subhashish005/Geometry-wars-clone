#include "Entity.h"

Entity::Entity(const std::string& tag, size_t id) : m_active(true), m_id(id), m_tag(tag) {}

bool Entity::isActive() const {
	return m_active;
}

const std::string& Entity::tag () const {
	return m_tag;
}

const size_t Entity::id() const {
	return m_id;
}

void Entity::destroy() {
	m_active = false;
}