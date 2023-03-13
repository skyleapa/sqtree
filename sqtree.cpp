/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int, int> ul, int w, int h, RGBAPixel a, double v)
    : upLeft(ul), width(w), height(h), avg(a), var(v), NW(NULL), NE(NULL), SE(NULL), SW(NULL)
{
}

// Second Node constructor, given
SQtree::Node::Node(stats &s, pair<int, int> ul, int w, int h)
    : upLeft(ul), width(w), height(h), NW(NULL), NE(NULL), SE(NULL), SW(NULL)
{
  avg = s.getAvg(ul, w, h);
  var = s.getVar(ul, w, h);
}

// SQtree destructor, given.
SQtree::~SQtree()
{
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree &other)
{
  copy(other);
}

// SQtree assignment operator, given.
SQtree &SQtree::operator=(const SQtree &rhs)
{
  if (this != &rhs)
  {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  //clear();
  stats imgStats = stats(imIn);
  pair<int, int> startCoord = {0,0};
  root = buildTree(imgStats, startCoord, imIn.width(), imIn.height(), tol);
}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul, int w, int h, double tol) {
  
  if(w == 0 || h == 0){
    return NULL;
  }

  Node * subRoot = new Node(s, ul, w, h);

  double smallestVariance = subRoot->var;
  if(smallestVariance <=  tol) {
    return subRoot;
  }

  int parX = 0;
  int parY = 0;

  for(int y = 0; y <= h; y++) {
    for(int x = 0; x <= w; x++){
      if(!(x==0 && y==0) && !((x==w)&&(y==h)) && !((x==0)&&(y==h)) && !((y==0)&&(x==w)) ){
      double largestVariance = findLargestVariance(s, ul, w, h, x, y);
      if(largestVariance < smallestVariance) { 
          smallestVariance = largestVariance;
          parX = x;
          parY = y;
      }
      if(largestVariance == smallestVariance) {
         if(y > parY ){
            smallestVariance = largestVariance;
            parX = x;              
            parY = y;
          }
          else if(x > parX) {
            smallestVariance = largestVariance;
            parX = x;
            parY = y;
          }
        }
      }
    }
  }

  pair<int, int> nePair = {ul.first + parX, ul.second};
  pair<int, int> sePair = {ul.first + parX, ul.second+parY};
  pair<int, int> swPair = {ul.first, ul.second+parY};
  
  subRoot->NW = buildTree(s, ul, parX, parY, tol);
  subRoot->NE = buildTree(s, nePair, (w-parX), parY, tol);
  subRoot->SE = buildTree(s, sePair , (w-parX), (h-parY), tol);
  subRoot->SW = buildTree(s, swPair, parX, (h-parY), tol);

  return subRoot;
}

double SQtree::findLargestVariance(stats &s, pair<int, int> ul, int w, int h, int x, int y)
{
  double nwVar = 0;
  double neVar = 0;
  double seVar = 0;
  double swVar = 0;

  if (x > 0 && y > 0)
    nwVar = s.getVar(ul, x, y);
  if (y > 0 && w - x > 0)
    neVar = s.getVar({ul.first + x, ul.second}, w - x, y);
  if (w - x > 0 && h - y > 0)
    seVar = s.getVar({ul.first + x, ul.second + y}, w - x, h - y);
  if (x > 0 && h - y > 0)
    swVar = s.getVar({ul.first, ul.second + y}, x, h - y);

  double max = nwVar;
  if (neVar > nwVar)
    max = neVar;
  if (seVar > max)
    max = seVar;
  if (swVar > max)
    max = swVar;
  return max;
}

/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  PNG img = PNG(root->width, root->height);
  renderNode(&img, root);
  return img;
}

void SQtree::renderNode(PNG *img, Node *curr)
{
  if (curr == NULL) {
    return;
  }

  for(int x = curr->upLeft.first; x < curr->upLeft.first+curr->width; x++) {
    for(int y =  curr->upLeft.second; y < curr->upLeft.second+curr->height; y++) {
      RGBAPixel* pixel = img->getPixel(x,y);
      pixel->r = curr->avg.r;
      pixel->g = curr->avg.g;
      pixel->b = curr->avg.b;
    }
  }

  renderNode(img, curr->NW);
  renderNode(img, curr->NE);
  renderNode(img, curr->SE);
  renderNode(img, curr->SW);
}

/**
 * Delete allocated memory.
 */
void SQtree::clear()
{
  clearTraversal(root);
}

void SQtree::clearTraversal(Node *curr)
{
  if (curr == NULL)
    return;
  clearTraversal(curr->SW);
  clearTraversal(curr->SE);
  clearTraversal(curr->NW);
  clearTraversal(curr->NE);

  delete curr;
}

void SQtree::copy(const SQtree &other)
{
  //copyNodes(other.root, root);
  root = copyTraversal(other.root);
}

SQtree::Node* SQtree::copyTraversal(Node *toCopy)
{
  if(toCopy == NULL){
    return NULL;
  }

  Node * newNode = new Node(toCopy->upLeft, toCopy->width, toCopy->height, toCopy->avg, toCopy->var); 

    newNode->NW = copyTraversal(toCopy->NW);
    newNode->NE = copyTraversal(toCopy->NE);
    newNode->SE = copyTraversal(toCopy->SE);
    newNode->SW = copyTraversal(toCopy->SW);

    return newNode;
  }

int SQtree::size()
{
  int currSize = sizeTraversal(root);
  return currSize;
}

int SQtree::sizeTraversal(Node* curr) {
  if (curr == NULL)
    return 0;
  return 1 + sizeTraversal(curr->SW) + sizeTraversal(curr->SE) + sizeTraversal(curr->NW) + sizeTraversal(curr->NE);

}