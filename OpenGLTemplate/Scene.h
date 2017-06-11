#pragma once
#include <vector>
#include "Object.h"

struct Node
{
	Node* parent;
	Object* object;
	std::vector<Node*> children;

	Node(Object* object, Node* parent);
	~Node();

	void draw(Camera* camera, GLfloat z);
	void addChild(Object* object);
private:
	void destroy();
	bool Visible(Camera* camera, GLfloat z);
};

class QuadTree;

class Scene
{
public:
	Scene();
	~Scene();

	Node* root;
	void draw(Camera* camera);

private:
	QuadTree* quadtree;
	void buildQuadTree(Node* root, Camera* camera);

};

// Axis-aligned bounding box with half dimension and center
struct AABB
{
	glm::vec2 center;
	GLfloat halfDimension;

	AABB(glm::vec2 center, GLfloat halfDimension);
	bool containsPoint(glm::vec2 point);
	bool intersects(AABB other);
	bool containsObject(Object* object);
	bool containsRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

};

class QuadTree
{
	
public:
	static const int QT_NODE_CAPACITY = 4;
	AABB box;
	std::vector<Object*> objects;
	QuadTree* children[4];

	QuadTree(AABB _box);
	~QuadTree();
	bool insert(Object* object);
	void subdivide();
	std::vector<Object*> queryRange(AABB range);


};