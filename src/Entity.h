#pragma once

#include "common.h"
#include "Components.h"

class Entity {
private:
	bool		m_active =	true;
	size_t		m_id	 =	0;
	std::string m_tag    =	"default";

	Entity(const std::string& tag, size_t id);
public:
	friend class EntityManager;

	// component pointers
	
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape>     cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CScore>     cScore;
	std::shared_ptr<CLifespan>  cLifespan;
	std::shared_ptr<CInput>		cInput;

	// private member access functions

	bool isActive() const;
	const std::string& tag() const;
	const size_t id() const;
	void destroy();
};