#pragma once
#include <glm/glm.hpp>

/**
 * An interface for transforming a matrix into another matrix
 */
class Transformer {
protected:
	virtual void transform(glm::mat4 & context) const = 0;
};