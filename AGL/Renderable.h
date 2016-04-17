#pragma once
class Renderable {
public:
	Renderable();
	virtual ~Renderable();
	virtual void setUp();
	virtual void tearDown();
	virtual void tick();
	virtual void update();
protected:
	void markForUpdate() { shouldUpdate = true; }
private:
	bool shouldUpdate;
};
