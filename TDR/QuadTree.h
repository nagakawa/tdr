#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <vector>

#include "Collidable.h"

// parameters from proj-youmu
#define MAX_QT_NODES 8
#define MAX_QT_NODES_COMPACT 4

namespace tdr {
  struct QuadTreeLeaf {
    Circle c;
    uint64_t id;
  };
  class QuadTreeNode {
    float x, y;
    float halfDim;
    QuadTreeLeaf points[MAX_QT_NODES];
    QuadTreeNode* parent;
    QuadTreeNode *nw, *ne, *sw, *se;
    float maxRadius;
    int numPoints;
    void subdivide();
    // bool compact();
    bool contains(float x, float y);
    void insertInChild(QuadTreeLeaf& h);
    bool isWithinRange(Circle& c);
    bool isWithinRange(Line& l);
    void percolateRadiusIncrease(float r);
  public:
    QuadTreeNode(QuadTreeNode* parent, float x, float y, float halfDim);
    ~QuadTreeNode();
    bool insert(QuadTreeLeaf& h);
    bool remove(float x, float y, QuadTreeLeaf* st = nullptr);
    bool query(Circle& c, QuadTreeLeaf* st = nullptr);
    bool query(Line& l, QuadTreeLeaf* st = nullptr);
  };
  class QuadTree {
    std::vector<Line> lines; // counted separately
    QuadTreeNode circles;
  };
}
