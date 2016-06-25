#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

#include "Collidable.h"

#define MAX_QT_NODES 4

namespace tdr {
  class QuadTreeNode {
    float x, y;
    float halfDim;
    Circle points[MAX_QT_NODES];
    QuadTreeNode* parent;
    QuadTreeNode *nw, *ne, *sw, *se;
    float maxRadius;
    int numPoints;
    void subdivide();
    bool contains(float x, float y);
    void insertInChild(Circle& c);
    bool isWithinRange(Circle& c);
    bool isWithinRange(Line& l);
    void percolateRadiusIncrease(float r);
  public:
    QuadTreeNode(QuadTreeNode* parent, float x, float y, float halfDim);
    ~QuadTreeNode();
    bool insert(Circle& c);
    bool query(Circle& c);
    bool query(Line& l);
  };
  class QuadTree {
    std::vector<Line> lines; // counted separately
    QuadTreeNode circles;
  };
}
