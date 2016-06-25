#include "QuadTree.h"

using namespace tdr;

tdr::QuadTreeNode::QuadTreeNode(QuadTreeNode* parent, float x, float y, float halfDim) {
  this->x = x;
  this->y = y;
  this->halfDim = halfDim;
  this->parent = parent;
  numPoints = 0;
  maxRadius = 0;
  nw = ne = sw = se = nullptr;
}

tdr::QuadTreeNode::~QuadTreeNode() {
  if (nw != nullptr) {
    delete nw;
    delete ne;
    delete sw;
    delete se;
  }
}

void tdr::QuadTreeNode::subdivide() {
  assert(nw == nullptr);
  float q = halfDim / 2;
  nw = new QuadTree(this, x - q, y - q, q);
  ne = new QuadTree(this, x + q, y - q, q);
  sw = new QuadTree(this, x - q, y + q, q);
  se = new QuadTree(this, x + q, y + q, q);
  for (int i = 0; i < numPoints; ++i)
    insertInChild(points[i]);
}

bool tdr::QuadTree::contains(float x, float y) {
  float nx = x - this->x;
  float ny = y - this->y;
  if (nx < -halfDim || nx > halfDim || ny < -halfDim || ny > halfDim)
    return false;
  return true;
}

bool tdr::QuadTree::insert(Circle& c) {
  if (!contains(c.x, c.y)) return false;
  if (numPoints < MAX_QT_NODES) {
    points[numPoints++] = c;
    return true;
  }
  if (nw == nullptr) subdivide();
  insertInChild(c);
  return true;
}

void tdr::QuadTree::insertInChild(Circle& c) {
  if (!(nw->insert(c) || ne->insert(c) || sw->insert(c) || se.insert(c))) {
    fprintf(stderr, "Somehow, the circle could not be inserted anywhere!\n");
    abort();
  }
}

// Thanks http://stackoverflow.com/a/1879223
bool tdr::QuadTreeNode::isWithinRange(Circle& c) {
  float rcx = c.x - x;
  float rcy = c.y - y;
  float closestX = clamp(rcx, -halfDim, halfDim);
  float closestY = clamp(rcy, -halfDim, halfDim);
  float dx = rcx - closestX;
  float dy = rcy - closestY;
  return hypot(dx, dy) < (c.r + maxRadius);
}

bool tdr::QuadTreeNode::isWithinRange(Line& l) {
  Line l2 = (Line) {x, y, 0, halfDim, halfDim};
  return doLinesIntersect(l, l2);
}

bool tdr::QuadTreeNode::query(Circle& c) {
  if (!isWithinRange(c)) return false;
  if (nw != nullptr) {
    for (int i = 0; i < numPoints; ++i) {
      if (doCirclesIntersect(c, points[i])) return true;
    }
    return false;
  } else {
    return nw->query(c) || ne->query(c) || sw->query(c) || se->query(c);
  }
}

bool tdr::QuadTreeNode::query(Line& l) {
  if (!isWithinRange(l)) return false;
  if (nw != nullptr) {
    for (int i = 0; i < numPoints; ++i) {
      if (doCircleAndLineIntersect(points[i], l)) return true;
    }
    return false;
  } else {
    return nw->query(l) || ne->query(l) || sw->query(l) || se->query(l);
  }
}
