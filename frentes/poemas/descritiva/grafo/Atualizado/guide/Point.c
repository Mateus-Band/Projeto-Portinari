#include "Point.h"

//***************************************************
//
//   initialize_Point
//      allocates memory and constructs a new Point
//
//**************************************************
void initialize_Point(Point *p, coord_t x, coord_t y, coord_t z, coord_t w, size_tt dim)
{
  p->coord = (coord_t *)malloc(dim * sizeof(coord_t));
  p->coord[0] = x;
  p->coord[1] = y;
  if (dim > 2)
    p->coord[2] = z;
  if (dim > 3)
    p->coord[3] = w;
  p->dim = dim;
}


//***************************************************
//
//   construct_Point
//      constructs a new Point
//
//**************************************************
Point * construct_Point( coord_t x, coord_t y, coord_t z, coord_t w, size_tt dim)
{
  Point *p = (Point *)malloc(sizeof(Point));
  p->coord = (coord_t *)malloc(dim * sizeof(coord_t));
  p->coord[0] = x;
  p->coord[1] = y;
  if (dim > 2)
    p->coord[2] = z;
  if (dim > 3)
    p->coord[3] = w;
  p->dim = dim;
  return p;
}

//***************************************************
//
//   free_Point
//      free memory used by a Point
//
//**************************************************
void free_Point(Point * p1)
{
  free(p1->coord);
}

//*************************************************
//
//  adds two points, puts result in dest
//
//*************************************************
void point_plus(Point *dest, Point *p1, Point *p2)
{
  int i;
  if (p1->dim != p2->dim)
    debug("TRYING TO ADD POINTS THAT ARE OF DIFFERENT DIMENSIONS\n");
  dest->dim = p1->dim;
  dest->coord = (coord_t *)malloc(dest->dim * sizeof(coord_t));
  for(i = 0; i < dest->dim; i ++)
    dest->coord[i] = p1->coord[i] + p2->coord[i];
}


//*************************************************
//
//  adds two points, puts result in the first point
//
//*************************************************
void point_plus_eq(Point *p1, Point *p2)
{
  int i;
  if (p1->dim != p2->dim)
    debug("TRYING TO ADD EQ POINTS THAT ARE OF DIFFERENT DIMENSIONS\n");

  for(i = 0; i < p1->dim; i ++)
    p1->coord[i] = p1->coord[i] + p2->coord[i];
}


//***************************************************************
//
//     subtracts two points, puts result in dest
//
//**************************************************************
void point_minus(Point *dest, Point *p1, Point *p2)
{
  int i;
  if (p1->dim != p2->dim)
    debug("TRYING TO SUBTRACT POINTS THAT ARE OF DIFFERENT DIMENSIONS\n");

  dest->dim = p1->dim;
  dest->coord = (coord_t *)malloc(dest->dim * sizeof(coord_t));
  for(i = 0; i < dest->dim; i ++)
    dest->coord[i] = p1->coord[i] - p2->coord[i];
}

//***************************************************************
//
//     subtracts two points, puts result in first point
//
//**************************************************************
void point_minus_eq(Point *p1, Point *p2)
{
  int i;
  if (p1->dim != p2->dim)
    debug("TRYING TO SUBTRACT EQ POINTS THAT ARE OF DIFFERENT DIMENSIONS\n");


  for(i = 0; i < p1->dim; i ++)
    p1->coord[i] = p1->coord[i] - p2->coord[i];
}

//***************************************************************
//
//     divides two points, puts result in dest
//
//**************************************************************
Point *point_div(Point *p1, coord_t constant)
{
  int i;
  Point *sum = (Point *)malloc(sizeof(Point));
  sum->dim = p1->dim;

  sum->coord = (coord_t *)malloc(sum->dim * sizeof(coord_t));
  for(i = 0; i < sum->dim; i ++)
    sum->coord[i] = p1->coord[i] / constant;
  return sum;
}

//***************************************************************
//
//     divides two points, puts result in first point
//
//**************************************************************
void point_div_eq(Point *p1, coord_t constant)
{
  int i;

  for(i = 0; i < p1->dim; i ++)
    p1->coord[i] = p1->coord[i] / constant;
}

//***************************************************************
//
//     multiplies two points, puts result in dest
//
//**************************************************************
Point *point_mult(Point *p1, coord_t constant)
{
  int i;
  Point *sum = (Point *)malloc(sizeof(Point));
  sum->dim = p1->dim;

  sum->coord = (coord_t *)malloc(sum->dim * sizeof(coord_t));
  for(i = 0; i < sum->dim; i ++)
    sum->coord[i] = p1->coord[i] * constant;
  return sum;
}

//***************************************************************
//
//     multiplies two points, puts result in first point
//
//**************************************************************
void point_mult_eq(Point *p1, coord_t constant)
{
  int i;

  for(i = 0; i < p1->dim; i ++)
    p1->coord[i] = p1->coord[i] * constant;
}


//**************************************************************
//
//	Method name :   point_scalar_product()
//
//      Description : scalar product of vectors associated with
//      points
//
//**************************************************************
const coord_t point_scalar_product(Point *p1, Point *p2)
{
    coord_t result = 0;
    size_tt ind;

    if (p1->dim != p2->dim)
      debug("TRYING TO COMPUTE SCALAR PRODUCT OF POINTS THAT ARE OF DIFFERENT DIMENSIONS\n");

    for(ind = 0; ind < p2->dim; ind++)
        result += p1->coord[ind] * p2->coord[ind];

    return result;
}


//*************************************************************************
//
//         creates a copy of a Point
//
//*************************************************************************
void point_duplicate(Point *p2, Point *p1) /* copy coordinates of p1 into p2 */
{
  int i;

  if (p1->dim != p2->dim)
    debug("TRYING TO DUPLICATE POINTS THAT ARE OF DIFFERENT DIMENSIONS\n");

  for (i = 0; i < p1->dim; i ++)
  	p2->coord[i] = p1->coord[i];
}

//**************************************************************
//
//	Method name : norm2
//
//	Description : the square of the norm
//
//**************************************************************
const coord_t norm2(Point *p1)
{
    coord_t result = 0;
    size_tt ind;

	for(ind = 0; ind < p1->dim; ind++)
	  result += p1->coord[ind] * p1->coord[ind];
  return result;
}

//**************************************************************
//
//	Method name : norm
//
//	Description : the norm of a vector
//
//**************************************************************
const coord_t norm(Point *p1)
{
    return sqrt( norm2(p1) ) ;
}

/* Compares the x coordinate of two points.
 * Returns -1 if point1 < point2,
 *          1 if point1 > point2,
 *          0 if points are equal
 */

int compare_x_coord(Point *p1, Point *p2)
{
  if(p1->coord[0] < p2->coord[0])
    return -1;
  else if (p1->coord[0] > p2->coord[0])
    return 1;
  else return 0;
}

//**************************************************************
//
//     returns the specified coordinate of a point
//     pos has following values: 0 - x, 1 - y, 2 - z
//
//**************************************************************
coord_t get_coord(Point *p1, int pos)
{
  if (p1->dim < (pos + 1))
    {
      debug("TRYING TO GET A COORDINATE FROM A SMALLER DIMENSION POINT\n");
      return -999;
    }
  return p1->coord[pos];
}


//**************************************************************
//
//   reset point coordinates to 0
//
//**************************************************************
void set_to_zero(Point *p1)
{
      p1->coord[0] = 0;
      p1->coord[1] = 0;
      if (p1->dim > 2)
        p1->coord[2] = 0;
      if (p1->dim > 3)
        p1->coord[3] = 0;
}















