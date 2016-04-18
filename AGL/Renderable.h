#pragma once
class Renderable {
public:
	Renderable();
	~Renderable();
	virtual void setUp();
	void tearDown();
	virtual void tick();
	virtual void update();
	bool isTornDown() { return tornDown; }
protected:
	void markForUpdate() { shouldUpdate = true; }
	virtual void _tearDown();
private:
	bool shouldUpdate;
	bool tornDown;
};
