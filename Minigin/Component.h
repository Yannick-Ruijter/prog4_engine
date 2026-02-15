#pragma once

class Component {
public:
	virtual void Update(float deltaTime);
	virtual void Render() const;
};