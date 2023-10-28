#include "stdafx.h"

#include "pge_214.h"

#include <numeric>

#include "ace/Log_Msg.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_214::PGE_214 ()
 : olc::PixelGameEngine ()
 , circles_ ()
 , r1_ (0.0f)
 , r2_ (0.0f)
 , r3_ (0.0f)
 , z1_ (0.0f, 0.0f)
 , z2_ (0.0f, 0.0f)
 , z3_ (0.0f, 0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 214");
}

bool
PGE_214::OnUserCreate ()
{
  z1_ = complex (ENGINE_PGE_214_DEFAULT_SIZE / 2.0f,
                 ENGINE_PGE_214_DEFAULT_SIZE / 2.0f);
  z2_ = complex (ENGINE_PGE_214_DEFAULT_SIZE / 2.0f - 100.0f,
                 ENGINE_PGE_214_DEFAULT_SIZE / 2.0f);
  z3_ = complex (ENGINE_PGE_214_DEFAULT_SIZE / 2.0f + 200.0f,
                 ENGINE_PGE_214_DEFAULT_SIZE / 2.0f);

  r1_ = 300.0f;
  r2_ = 200.0f;
  r3_ = 100.0f;

  //curvatures
  float k1 = -1.0f / r1_;
  float k2 = 1.0f / r2_;
  float k3 = 1.0f / r3_;

  //initial circles
  circle c1 (z1_.scale (k1), k1);
  circle c2 (z2_.scale (k2), k2);
  circle c3 (z3_.scale (k3), k3);

  //we've set them up to be touching tangent to the other two
  c1.tangentCircles_.push_back (c2);
  c1.tangentCircles_.push_back (c3);
  c2.tangentCircles_.push_back (c1);
  c2.tangentCircles_.push_back (c3);
  c3.tangentCircles_.push_back (c2);
  c3.tangentCircles_.push_back (c1);

  circles_.push_back (c1);
  circles_.push_back (c2);
  circles_.push_back (c3);

  return true;
}

bool
PGE_214::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  std::vector<circle> incompleteCircles_a;
  std::vector<circle> completion_a;
  std::vector<circle>::iterator ip;

  //if (circles_.size () > 15000)
  //  goto continue_;

  for (std::vector<circle>::iterator iterator = circles_.begin ();
        iterator != circles_.end ();
        ++iterator)
    if (!(*iterator).tangentCircles_.empty () && (*iterator).tangentCircles_.size () < 5)
      incompleteCircles_a.push_back (*iterator);
  completion_a =
    std::reduce (incompleteCircles_a.begin (), incompleteCircles_a.end (), completion_a,
                 [this] (std::vector<circle> first, circle& second)
                 { std::vector<circle> result_a = this->apollonian (second);
                   first.insert (first.end (), result_a.begin (), result_a.end ());
                   return first;
                 });
  circles_.insert (circles_.end (), completion_a.begin (), completion_a.end ());
  std::sort (circles_.begin (), circles_.end (), [] (const circle& rhs, const circle& lhs) { return rhs.isLessThan (lhs); });
  ip =
    std::unique (circles_.begin (), circles_.end (), [] (const circle& rhs, const circle& lhs) { return rhs.isEqual (lhs); });
  circles_.resize (std::distance (circles_.begin (), ip));

//continue_:
  for (std::vector<circle>::iterator iterator = circles_.begin ();
       iterator != circles_.end ();
       ++iterator)
    (*iterator).draw (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::vector<PGE_214::circle>
PGE_214::apollonian (circle& c)
{
  std::vector<circle> result_a;

  //Apply Decartes theorem iterativly to pack circles within a circle.
  //https://en.wikipedia.org/wiki/Apollonian_gasket
  if (c.tangentCircles_.size () < 2) return result_a;
  if (c.tangentCircles_.size () == 2) return decartes (c, c.tangentCircles_[0], c.tangentCircles_[1]);

  circle& c1 = c.tangentCircles_[0];
  circle& c2 = c.tangentCircles_[1];
  circle& c3 = c.tangentCircles_[2];

  //Each call to decartes returns a pair of circles. 
  //One we already have, so we filter it out. We'll also filter out circles that are too small.

  std::vector<circle> c23;
  std::vector<circle> temp = decartes (c, c2, c3);
  for (std::vector<circle>::iterator iterator = temp.begin ();
       iterator != temp.end ();
       ++iterator)
    if (!c1.isEqual (*iterator) && (*iterator).r_ > 2.0f)
      c23.push_back (*iterator);
  std::vector<circle> c13;
  temp = decartes (c, c1, c3);
  for (std::vector<circle>::iterator iterator = temp.begin ();
       iterator != temp.end ();
       ++iterator)
    if (!c2.isEqual (*iterator) && (*iterator).r_ > 2.0f)
      c13.push_back (*iterator);
  std::vector<circle> c12;
  temp = decartes (c, c1, c2);
  for (std::vector<circle>::iterator iterator = temp.begin ();
       iterator != temp.end ();
       ++iterator)
    if (!c3.isEqual (*iterator) && (*iterator).r_ > 2.0f)
      c12.push_back (*iterator);

  result_a.insert (result_a.end (), c23.begin (), c23.end ());
  result_a.insert (result_a.end (), c12.begin (), c12.end ());
  result_a.insert (result_a.end (), c13.begin (), c13.end ());

  return result_a;
}

std::vector<PGE_214::circle>
PGE_214::decartes (circle& c1, circle& c2, circle& c3)
{
  std::vector<circle> result_a;

  //Decartes Theorem: Given three tangent circles we can find a fourth and a fifth.
  //https://en.wikipedia.org/wiki/Descartes%27_theorem
  float root_f = std::sqrt (std::abs (c1.k_ * c2.k_ + c3.k_ * c2.k_ + c1.k_ * c3.k_));
  float k_plus  = c1.k_ + c2.k_ + c3.k_ + 2.0f * root_f;
  float k_minus = c1.k_ + c2.k_ + c3.k_ - 2.0f * root_f;

  complex c12 = c1.z_.mult (c2.z_);
  complex c23 = c2.z_.mult (c3.z_);
  complex c31 = c3.z_.mult (c1.z_);

  complex t1 = c1.z_.add (c2.z_.add (c3.z_));
  complex t2 = c12.add (c23.add (c31));
  complex t3 = t2.sqrt ().scale (2.0f);

  complex z_plus = t1.add (t3);
  complex z_minus = t1.minus (t3);

  circle c_plus (z_plus, k_plus);
  circle c_minus (z_minus, k_minus);

  c_plus.tangentCircles_.push_back (c1);
  c_plus.tangentCircles_.push_back (c2);
  c_plus.tangentCircles_.push_back (c3);
  c_minus.tangentCircles_.push_back (c1);
  c_minus.tangentCircles_.push_back (c2);
  c_minus.tangentCircles_.push_back (c3);

  //These now have a full set so we don't care anymore
  c1.tangentCircles_.clear ();
  c2.tangentCircles_.clear ();
  c3.tangentCircles_.clear ();

  result_a.push_back (c_plus);
  result_a.push_back (c_minus);

  return result_a;
}
