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
  nw = new QuadTreeNode(this, x - q, y - q, q);
  ne = new QuadTreeNode(this, x + q, y - q, q);
  sw = new QuadTreeNode(this, x - q, y + q, q);
  se = new QuadTreeNode(this, x + q, y + q, q);
  for (int i = 0; i < numPoints; ++i)
    insertInChild(points[i]);
}

bool tdr::QuadTreeNode::contains(float x, float y) {
  float nx = x - this->x;
  float ny = y - this->y;
  if (nx >= -halfDim || nx < halfDim || ny >= -halfDim || ny < halfDim)
    return false;
  return true;
}

void tdr::QuadTreeNode::percolateRadiusIncrease(float r) {
  if (r > maxRadius) maxRadius = r;
  else return;
  if (parent != nullptr) parent->percolateRadiusIncrease(r);
}

bool tdr::QuadTreeNode::insert(QuadTreeLeaf& h) {
  if (!contains(h.c.x, h.c.y)) return false;
  if (numPoints < MAX_QT_NODES) {
    points[numPoints++] = h;
    percolateRadiusIncrease(h.c.radius);
    return true;
  }
  if (nw == nullptr) subdivide();
  insertInChild(h);
  return true;
}

// TODO compaction
bool tdr::QuadTreeNode::remove(float x, float y, QuadTreeLeaf* st) {
  if (!contains(x, y)) return false;
  if (nw == nullptr) { // No children
    for (int i = 0; i < numPoints; ++i) {
      // Remove the point, and stash it if necessary.
      if (points[i].c.x == x && points[i].c.y == y) {
        --numPoints;
        if (st != nullptr) *st = points[i];
        points[i] = points[numPoints];
        return true;
      }
    }
    // No appropriate point was found.
    return false;
  } else { // there are children
    if (x < this->x) {
      if (y < this->y) return nw->remove(x, y, st);
      else return sw->remove(x, y, st);
    } else {
      if (y < this->y) return ne->remove(x, y, st);
      else return se->remove(x, y, st);
    }
  }
  fprintf(stderr, "Not placed in a child?\n");
  abort();
  return false;
}

void tdr::QuadTreeNode::insertInChild(QuadTreeLeaf& h) {
  if (!(nw->insert(h) || ne->insert(h) || sw->insert(h) || se->insert(h))) {
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
  return hypot(dx, dy) < (c.radius + maxRadius);
}

bool tdr::QuadTreeNode::isWithinRange(Line& l) {
  Line l1 = (Line) {l.x, l.y, l.angle, l.width + maxRadius, l.length};
  Line l2 = (Line) {x, y, 0, halfDim, halfDim};
  return doLinesIntersect(l1, l2);
}

bool tdr::QuadTreeNode::query(Circle& c, QuadTreeLeaf* st) {
  if (!isWithinRange(c)) return false;
  if (nw != nullptr) {
    for (int i = 0; i < numPoints; ++i) {
      if (doCirclesIntersect(c, points[i].c)) {
        if (st != nullptr) *st = points[i];
        return true;
      }
    }
    return false;
  } else {
    return nw->query(c) || ne->query(c) || sw->query(c) || se->query(c);
  }
}

bool tdr::QuadTreeNode::query(Line& l, QuadTreeLeaf* st) {
  if (!isWithinRange(l)) return false;
  if (nw != nullptr) {
    for (int i = 0; i < numPoints; ++i) {
      if (doCircleAndLineIntersect(points[i].c, l)) {
        if (st != nullptr) *st = points[i];
        return true;
      }
    }
    return false;
  } else {
    return nw->query(l) || ne->query(l) || sw->query(l) || se->query(l);
  }
}
