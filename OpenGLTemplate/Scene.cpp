#include "Scene.h"

Node::Node(Object* object, Node* parent)
{
	this->object = object;
	this->parent = parent;
}

Node::~Node()
{
	destroy();
}

void Node::draw(Camera* camera, GLfloat z)
{
	glm::mat4 parent_model = (parent && parent->object) ? parent->object->getModel() : glm::mat4(1.0);
	if (Visible(camera, z)) {
		if(object) object->draw(camera, parent_model);
		for (int i = 0; i < children.size(); ++i) {
			children[i]->draw(camera, z * 0.75f);
		}
	}
}

bool Node::Visible(Camera* camera, GLfloat z)
{
	if (!camera || !(this->object)) return false;
	glm::vec3 camPos = camera->position;
	glm::vec3 objPos = object->position;
	glm::vec3 vec = objPos - camPos;
	GLfloat length = glm::length(vec);
	if (length < z) return true;
	return false;
}

void Node::addChild(Object* object)
{
	if (object) {
		children.push_back(new Node(object, this));
	}
}

void Node::destroy()
{
	parent = 0;
	if (object) {
		delete object;
		object = 0;
	}
	for (int i = 0; i < children.size(); ++i) {
		if (children[i]) {
			delete children[i];
		}
	}
	children.clear();
}

Scene::Scene()
{
	root = new Node(0, 0);
	quadtree = 0;
}

Scene::~Scene()
{
	if (root) delete root;
	if (quadtree) delete quadtree;
}

void Scene::draw(Camera* camera)
{
	/*
	buildQuadTree(root, camera);
	auto objects = quadtree->queryRange(AABB(camera->position, 200.0f));
	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->draw(camera, glm::mat4(1.0f));
	}
	*/
	if (root) root->draw(camera, 120.0f);
}

void Scene::buildQuadTree(Node* root, Camera* camera)
{
	if (!root) return;
	if (!quadtree) quadtree = new QuadTree(AABB(glm::vec2(camera->position.x, -camera->position.z), 200.0f));
	if (root->object) quadtree->insert(root->object);
	for (int i = 0; i < root->children.size(); ++i) {
		if (root->children[i]) {
			buildQuadTree(root->children[i], camera);
		}
	}
}


AABB::AABB(glm::vec2 center, GLfloat halfDimension)
{
	if (center.x != center.y)
		center.x = center.y = (center.x > center.y) ? center.x : center.y;
	this->center = center;
	this->halfDimension = halfDimension;
}

bool AABB::containsPoint(glm::vec2 point)
{
	if (point.x >= (center.x - halfDimension) && point.x <= (center.x + halfDimension)
		&& point.y >= (center.y - halfDimension) && point.y <= (center.y + halfDimension))
		return true;
	return false;
}

bool AABB::intersects(AABB other)
{
	//this
	GLfloat x1 = center.x - halfDimension;
	GLfloat y1 = center.y - halfDimension;
	GLfloat this_width, this_height;
	this_width = this_height = 2 * halfDimension;

	//other
	GLfloat x2 = other.center.x - other.halfDimension;
	GLfloat y2 = other.center.y - other.halfDimension;
	GLfloat other_width, other_height;
	other_width = other_height = 2 * other.halfDimension;

	return !(x1 > x2 + other_width || x1 + this_width < x2 || y1 > y2 + other_height || y1 + this_height < y2);

}

bool AABB::containsObject(Object* object)
{
	if (object) {
		BBox bbox = object->getBBox();
		if (containsRect(bbox.min.x, -bbox.min.z, bbox.max.x, -bbox.max.z))
			return true;
	}
	return false;
}

bool AABB::containsRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	if (containsPoint(glm::vec2(x1, y1)) && containsPoint(glm::vec2(x2, y2)))
		return true;
	return false;
}


QuadTree::QuadTree(AABB _box) : box(_box)
{
	children[0] = 0;	//top-left
	children[1] = 0;	//top-right
	children[2] = 0;	//bottom-left
	children[3] = 0;	//bottom-right
}

QuadTree::~QuadTree()
{
	for (int i = 0; i < 4; ++i) delete children[i];
}

bool QuadTree::insert(Object* object)
{
	if (!box.containsObject(object)) return false;
	
	if (objects.size() < QT_NODE_CAPACITY) {
		objects.push_back(object);
		return true;
	}
	if (!children[0]) subdivide();

	if (children[0]->insert(object)) return true;
	if (children[1]->insert(object)) return true;
	if (children[2]->insert(object)) return true;
	if (children[3]->insert(object)) return true;

	return false;
}

void QuadTree::subdivide()
{
	children[0] = new QuadTree(AABB(glm::vec2(box.center.x - box.halfDimension, box.center.y - box.halfDimension), box.halfDimension / 2.0));
	children[1] = new QuadTree(AABB(glm::vec2(box.center.x + box.halfDimension, box.center.y - box.halfDimension), box.halfDimension / 2.0));
	children[2] = new QuadTree(AABB(glm::vec2(box.center.x - box.halfDimension, box.center.y + box.halfDimension), box.halfDimension / 2.0));
	children[3] = new QuadTree(AABB(glm::vec2(box.center.x + box.halfDimension, box.center.y + box.halfDimension), box.halfDimension / 2.0));
}

std::vector<Object*> QuadTree::queryRange(AABB range)
{
	std::vector<Object*> result;
	if (!box.intersects(range))
		return result;

	//check objects at this quad level
	for (int i = 0; i < objects.size(); ++i) {
		if (range.containsObject(objects[i])) {
			result.push_back(objects[i]);
		}
	}

	if (!children[0]) return result;

	auto result0 = children[0]->queryRange(range);
	auto result1 = children[1]->queryRange(range);
	auto result2 = children[2]->queryRange(range);
	auto result3 = children[3]->queryRange(range);

	result.insert(result.end(), result0.begin(), result0.end());
	result.insert(result.end(), result1.begin(), result1.end());
	result.insert(result.end(), result2.begin(), result2.end());
	result.insert(result.end(), result3.begin(), result3.end());

	return result;
}
