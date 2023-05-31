#ifndef PGE_27_H
#define PGE_27_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_ui_gtk_builder_definition.h"
#include "common_ui_gtk_common.h"

#include "defines.h"

// GTK idle routines
gboolean idle_initialize_UI_27_cb (gpointer);
gboolean idle_finalize_UI_27_cb (gpointer);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
G_MODULE_EXPORT void scale_delta_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_angle_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void button_reset_27_clicked_cb (GtkButton*, gpointer);
#ifdef __cplusplus
}
#endif /* __cplusplus */

struct Engine_UI_GTK_27_CBData
 : Common_UI_GTK_CBData
{
  int delta;
  int angle; // degrees
};


class PGE_27
 : public olc::PixelGameEngine
{
  class hankin
  {
   public:
    hankin (const olc::vf2d& a,
            const olc::vf2d& v)
     : a_ (a)
     , v_ (v)
     , b_ ()
     //, end_ ()
     //, prevD_ (0.0f)
    {
      b_ = a_ + v_;
      //end_ = a_ + v_;
    }
    inline ~hankin () {}

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawLine (a_, b_, olc::WHITE, 0xFFFFFFFF);
    }

    //void findEnd (const hankin& other_in)
    //{
    //  // line line intersection???
    //  // this.a, this.v  (P1, P2-P1)
    //  // other.a, other.v (P3, P4-P3)

    //  // From: http://paulbourke.net/geometry/pointlineplane/
    //  float den = (other_in.v_.y * v_.x) - (other_in.v_.x * v_.y);
    //  if (!den)
    //    return;
    //  float numa =
    //    (other_in.v_.x * (a_.y - other_in.a_.y)) - (other_in.v_.y * (a_.x - other_in.a_.x));
    //  float numb =
    //    (v_.x * (a_.y - other_in.a_.y)) - (v_.y * (a_.x - other_in.a_.x));
    //  float ua = numa / den;
    //  float ub = numb / den;
    //  float x = a_.x + (ua * v_.x);
    //  float y = a_.y + (ua * v_.y);

    //  if (ua > 0.0f && ub > 0.0f)
    //  {
    //    olc::vf2d candidate = { x, y };
    //    float d1 = candidate.dist (a_);
    //    float d2 = candidate.dist (other_in.a_);
    //    float d = d1 + d2;
    //    float diff = abs (d1 - d2);
    //    if (diff < 0.001)
    //    {
    //      if (!end_.x && !end_.y)
    //      {
    //        end_ = candidate;
    //        prevD_ = d;
    //      } // end IF
    //      else if (d < prevD_)
    //      {
    //        prevD_ = d;
    //        end_ = candidate;
    //      } // end ELSE IF
    //    } // end IF
    //  } // end IF
    //}

   private:
    olc::vf2d a_;
    olc::vf2d v_;
    olc::vf2d b_;
    //olc::vf2d end_;
    //float prevD_;
  };

  class edge
  {
   public:
    edge (const olc::vf2d& a,
          const olc::vf2d& b)
     : a_ (a)
     , b_ (b)
     , h1_ (NULL)
     , h2_ (NULL)
    {}
    ~edge ()
    {
      delete h1_;
      delete h2_;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      h1_->show (engine_in);
      h2_->show (engine_in);
    }

    void generate (int delta_in,
                   int angle_in,
                   int sides_in)
    {
      olc::vf2d mid = a_;
      mid += b_;
      mid *= 0.5;

      olc::vf2d v1 = a_ - mid;
      olc::vf2d v2 = b_ - mid;

      // edge length
      float elen = v1.mag () + delta_in;

      olc::vf2d offset1 = mid;
      olc::vf2d offset2 = mid;
      if (delta_in > 0)
      {
        v1 = v1.norm ();
        v1 *= static_cast<float> (delta_in);
        v2 = v2.norm ();
        v2 *= static_cast<float> (delta_in);
        offset1 = mid + v2;
        offset2 = mid + v1;
      } // end IF
      v1 = v1.norm ();
      v2 = v2.norm ();

      // rotate in 2D
      float angle_f = angle_in * static_cast<float> (M_PI / 180.0);
      olc::vf2d temp;
      temp.x = cosf (-angle_f) * v1.x - sinf (-angle_f) * v1.y;
      temp.y = sinf (-angle_f) * v1.x + cosf (-angle_f) * v1.y;
      v1 = temp;
      temp.x = cosf (angle_f) * v2.x - sinf (angle_f) * v2.y;
      temp.y = sinf (angle_f) * v2.x + cosf (angle_f) * v2.y;
      v2 = temp;

      // Law of sines
      float alpha = static_cast<float> ((sides_in - 2) * M_PI / sides_in) * 0.5f;
      float beta = static_cast<float> (M_PI) - angle_f - alpha;
      float hlen = elen * sinf (alpha) / sinf (beta);

      v1 *= hlen;
      v2 *= hlen;

      h1_ = new hankin (offset1, v1);
      h2_ = new hankin (offset2, v2);
    }

    //void findEnds (const edge& edge_in)
    //{
    //  h1_->findEnd (*edge_in.h1_);
    //  h1_->findEnd (*edge_in.h2_);
    //  h2_->findEnd (*edge_in.h1_);
    //  h2_->findEnd (*edge_in.h2_);
    //}

   public:
    olc::vf2d a_;
    olc::vf2d b_;
    hankin* h1_;
    hankin* h2_;
  };

  class polygon
  {
   public:
    polygon (int n)
     : edges_ ()
     , vertices_ ()
     , sides_ (n)
    {}
    ~polygon ()
    {
      for (std::vector<edge*>::iterator iterator = edges_.begin ();
           iterator != edges_.end ();
           ++iterator)
        delete *iterator;
    }

    void addVertex (int x, int y)
    {
      olc::vf2d a = {static_cast<float> (x), static_cast<float> (y)};
      int total = static_cast<int> (vertices_.size ());
      if (total > 0)
      {
        edge* edge_p = new edge (vertices_[total - 1], a);
        edges_.push_back (edge_p);
      } // end IF
      vertices_.push_back (a);
    }

    void close ()
    {
      int total = static_cast<int> (vertices_.size ());
      olc::vf2d last = vertices_[total - 1];
      olc::vf2d first = vertices_[0];
      edge* edge_p = new edge (last, first);
      edges_.push_back (edge_p);
    }

    void hankin (int delta_in,
                 int angle_in)
    {
      for (std::vector<edge*>::iterator iterator = edges_.begin ();
           iterator != edges_.end ();
           ++iterator)
        (*iterator)->generate (delta_in, angle_in, sides_);

      //for (int i = 0; i < edges_.size (); i++)
      //  for (int j = 0; j < edges_.size (); j++)
      //    if (i != j)
      //      edges_[i]->findEnds (*edges_[j]);
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      for (std::vector<edge*>::iterator iterator = edges_.begin ();
           iterator != edges_.end ();
           ++iterator)
        (*iterator)->show (engine_in);
    }

   private:
    typedef std::vector<edge*> edges_t;
    edges_t edges_;
    typedef std::vector<olc::vf2d> vertices_t;
    vertices_t vertices_;
    int sides_;
  };

 public:
  PGE_27 (const std::string&); // GTK ui definition file path
  inline virtual ~PGE_27 () {}

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  typedef std::vector<polygon*> polygons_t;
  polygons_t                       polygons_;
  struct Engine_UI_GTK_27_CBData   CBData_;
  Common_UI_GTK_Configuration_t    GTKConfiguration_;
  Common_UI_GtkBuilderDefinition_t GTKBuilderDefinition_;
};

#endif // PGE_27_H
