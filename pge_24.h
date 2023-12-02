#ifndef PGE_24_H
#define PGE_24_H

#include <bitset>
#include <stack>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines.h"

class PGE_24
  : public olc::PixelGameEngine
{
 private:
  class cell
  {
   public:
    cell (int i, int j)
     : i_ (i)
     , j_ (j)
     , walls_ (0xF)
     , visited_ (false)
    {}

    void show (olc::PixelGameEngine* engine_in,
               int width_in)
    {
      int x = i_ * width_in;
      int y = j_ * width_in;

      if (walls_.test (0))
        engine_in->DrawLine (x, y, x + width_in, y, olc::WHITE, 0xFFFFFFFF);
      if (walls_.test (1))
        engine_in->DrawLine (x + width_in, y, x + width_in, y + width_in, olc::WHITE, 0xFFFFFFFF);
      if (walls_.test (2))
        engine_in->DrawLine (x + width_in, y + width_in, x, y + width_in, olc::WHITE, 0xFFFFFFFF);
      if (walls_.test (3))
        engine_in->DrawLine (x, y + width_in, x, y, olc::WHITE, 0xFFFFFFFF);

      if (visited_)
        engine_in->FillRect (x, y, width_in, width_in, {255, 0, 255, 100});
    }

    void highlight (olc::PixelGameEngine* engine_in,
                    int width_in)
    {
      int x = i_ * width_in;
      int y = j_ * width_in;

      engine_in->FillRect (x, y, width_in, width_in, {0, 0, 255, 100});
    }

    int index (int i_in, int j_in, int columns_in, int rows_in)
    {
      if (i_in < 0 || j_in < 0 || i_in > columns_in - 1 || j_in > rows_in - 1)
        return -1;

      return i_in + j_in * columns_in;
    }

    cell* checkNeighbors (int columns_in,
                          int rows_in,
                          std::vector<cell>& grid_in)
    {
      std::vector<cell*> neighbors;

      int index_i = index (i_, j_ - 1, columns_in, rows_in);
      cell* top = index_i < 0 ? NULL : &grid_in[index_i];
      index_i = index (i_ + 1, j_, columns_in, rows_in);
      cell* right = index_i < 0 ? NULL : &grid_in[index_i];
      index_i = index (i_, j_ + 1, columns_in, rows_in);
      cell* bottom = index_i < 0 ? NULL : &grid_in[index_i];
      index_i = index (i_ - 1, j_, columns_in, rows_in);
      cell* left = index_i < 0 ? NULL : &grid_in[index_i];

      if (top && !top->visited_)
        neighbors.push_back (top);
      if (right && !right->visited_)
        neighbors.push_back (right);
      if (bottom && !bottom->visited_)
        neighbors.push_back (bottom);
      if (left && !left->visited_)
        neighbors.push_back (left);

      if (neighbors.size () > 0)
      {
        index_i =
          Common_Tools::getRandomNumber (0, static_cast<int> (neighbors.size () - 1));
        return neighbors[index_i];
      } // end IF

      return NULL;
    }

    int i_;
    int j_;
    std::bitset<4> walls_;
    bool visited_;
  };
 public:
  PGE_24 ();
  inline virtual ~PGE_24 () {}

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int width_;
  int columns_;
  int rows_;
  std::vector<cell> grid_;
  cell* current_;
  std::stack<cell*> stack_;

  void removeWalls (cell&, cell&);
};

#endif // PGE_24_H
