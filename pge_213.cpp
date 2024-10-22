#include "stdafx.h"

#include "pge_213.h"

#include "ace/Log_Msg.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_213::PGE_213 ()
 : olc::PixelGameEngine ()
 , circles_ ()
 , r1_ (0.0f)
 , r2_ (0.0f)
 , z1_ (0.0f, 0.0f)
 , z2_ (0.0f, 0.0f)
 , center_ (0.0f, 0.0f)
 , touchPoint_ (0.0f, 0.0f)
 , mouse_ (0.0f, 0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 213");
}

bool
PGE_213::OnUserCreate ()
{
  r1_ = ENGINE_PGE_213_DEFAULT_SIZE / 2.2f;
  center_ = std::complex<float> (ENGINE_PGE_213_DEFAULT_SIZE / 2.0f,
                                 ENGINE_PGE_213_DEFAULT_SIZE / 2.0f);
  z1_ = center_;
  touchPoint_ = center_ - std::complex<float> (r1_, 0.0f);
  r2_ = (2.0f / 3.0f) * r1_;
  z2_ = touchPoint_ + std::complex<float> (r2_, 0.0f);

  mouse_ = std::complex<float> (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                                static_cast<float> (olc::PixelGameEngine::GetMouseY ()));

  return true;
}

bool
PGE_213::OnUserUpdate (float fElapsedTime)
{
  if (std::abs (static_cast<float> (olc::PixelGameEngine::GetMouseX ()) - mouse_.real ()) > 1.0f)
  {
    mouse_ = std::complex<float> (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                                  static_cast<float> (olc::PixelGameEngine::GetMouseY ()));

    std::complex<float> relativeMouse = mouse_ - touchPoint_;
    if (std::abs (relativeMouse) > 275.0f && std::abs (mouse_ - z1_) < r1_)
    {
      r2_ = (0.5f * std::abs (relativeMouse)) / std::cos (std::arg (relativeMouse));
      z2_ = touchPoint_ + std::complex<float> (r2_, 0.0f);
    } // end IF
    float theta_f = std::arg (mouse_ - z2_);
    //curvatures
    float k1 = -1.0f / r1_;
    float k2 = 1.0f / r2_;

    //initial circles
    std::complex<float> z1_scaled = z1_ * k1;
    circle circle_1 (z1_scaled, k1);
    std::complex<float> z2_scaled = z2_ * k2;
    circle circle_2 (z2_scaled, k2);
    circle circle_3 = thirdCircle (circle_1, circle_2, theta_f);

    //we've set them up to be touching tangent to the other two
    circle_1.tangentCircles_.push_back (circle_2);
    circle_1.tangentCircles_.push_back (circle_3);
    circle_2.tangentCircles_.push_back (circle_1);
    circle_2.tangentCircles_.push_back (circle_3);
    circle_3.tangentCircles_.push_back (circle_2);
    circle_3.tangentCircles_.push_back (circle_1);
    
    circle_1.gray_ = 0;

    circles_.clear ();
    circles_.push_back (circle_1);
    circles_.push_back (circle_2);
    circles_.push_back (circle_3);

    int n = 0;
    while (circles_.size () < 1000 && n < 5)
    {
      static float r_min = 1.0f;
      n++;
      std::vector<circle> incompleteCircles_a;
      for (std::vector<circle>::iterator iterator = circles_.begin ();
           iterator != circles_.end ();
           ++iterator)
        if (!(*iterator).tangentCircles_.empty () && (*iterator).tangentCircles_.size () < 5)
          incompleteCircles_a.push_back (*iterator);
      std::vector<circle> completion_a;
      for (std::vector<circle>::iterator iterator = incompleteCircles_a.begin ();
           iterator != incompleteCircles_a.end ();
           ++iterator)
      {
        std::vector<circle> result_a = apollonian (*iterator, r_min);
        completion_a.insert (completion_a.end (), result_a.begin (), result_a.end ());
      } // end FOR
      circles_.insert (circles_.end (), completion_a.begin (), completion_a.end ());
    } // end WHILE

    //Clear the screen and draw all the circles!
    olc::PixelGameEngine::Clear (olc::WHITE);
    for (std::vector<circle>::iterator iterator = circles_.begin ();
         iterator != circles_.end ();
         ++iterator)
      (*iterator).draw (this);
  } // end IF
  else if (circles_.size () < 10000)
  {
    static float r_min = 0.5f;
    std::vector<circle> incompleteCircles_a;
    for (std::vector<circle>::iterator iterator = circles_.begin ();
         iterator != circles_.end ();
         ++iterator)
      if (!(*iterator).tangentCircles_.empty () && (*iterator).tangentCircles_.size () < 5)
        incompleteCircles_a.push_back (*iterator);
    std::vector<circle> completion_a;
    for (std::vector<circle>::iterator iterator = incompleteCircles_a.begin ();
         iterator != incompleteCircles_a.end ();
         ++iterator)
    {
      std::vector<circle> result_a = apollonian (*iterator, r_min);
      completion_a.insert (completion_a.end (), result_a.begin (), result_a.end ());
    } // end FOR

    //draw just the new circles
    for (std::vector<circle>::iterator iterator = completion_a.begin ();
         iterator != completion_a.end ();
         ++iterator)
      (*iterator).draw (this);

    circles_.insert (circles_.end (), completion_a.begin (), completion_a.end ());
  } // end ELSE IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::complex<float>
PGE_213::find_z3 (std::complex<float>& z2, float r2, float r3, float theta)
{
  std::complex<float> n (std::cos (theta), std::sin (theta));
  n *= (r2 + r3);
  return z2 + n;
}

float
PGE_213::find_r3 (std::complex<float>& z3, std::complex<float>& z1, float r1)
{
  std::complex<float> dz = z3 - z1;
  return r1 - std::abs (dz);
}

PGE_213::circle
PGE_213::thirdCircle (circle& c1, circle& c2, float theta)
{
  //first guess at z3 and r3
  //As a first guess assume the center of c3 lies on the circle that is the average of the first two
  //This is true at theta==0
  float r_a = 0.5f * (c1.r_ + c2.r_);
  std::complex<float> z_a = 0.5f * (c1.center_ + c2.center_);

  std::complex<float> dz3 (r_a * std::cos (theta), r_a * std::sin (theta));

  std::complex<float> z3 = z_a + dz3;
  float r3 = 0.0f;

  //iterativly improve our guess of r3,z3
  for (int i = 0; i < 100; i++)
  {
    r3 = find_r3 (z3, c1.center_, c1.r_);
    z3 = find_z3 (c2.center_, c2.r_, r3, theta);
  } // end FOR

  //curvature
  float k3 = 1.0f / r3;

  std::complex<float> z3_scaled = z3 * k3;
  return circle (z3_scaled, k3);
}

std::vector<PGE_213::circle>
PGE_213::apollonian (circle& c, float r_min)
{
  std::vector<circle> result_a;

  //Apply Decartes theorem iterativly to pack circles within a circle.
  //https://en.wikipedia.org/wiki/Apollonian_gasket
  if (c.tangentCircles_.size () < 2)
    return result_a;
  else if (c.tangentCircles_.size () == 2)
    return decartes (c, c.tangentCircles_[0], c.tangentCircles_[1]);

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
    if (!c1.isEqual (*iterator) && ((*iterator).r_ > r_min))
      c23.push_back (*iterator);
  std::vector<circle> c13;
  temp = decartes (c, c1, c3);
  for (std::vector<circle>::iterator iterator = temp.begin ();
       iterator != temp.end ();
       ++iterator)
    if (!c2.isEqual (*iterator) && ((*iterator).r_ > r_min))
      c13.push_back (*iterator);
  std::vector<circle> c12;
  temp = decartes (c, c1, c2);
  for (std::vector<circle>::iterator iterator = temp.begin ();
       iterator != temp.end ();
       ++iterator)
    if (!c3.isEqual (*iterator) && ((*iterator).r_ > r_min))
      c12.push_back (*iterator);

  result_a.insert (result_a.end (), c12.begin (), c12.end ());
  result_a.insert (result_a.end (), c13.begin (), c13.end ());
  result_a.insert (result_a.end (), c23.begin (), c23.end ());

  return result_a;
}

std::vector<PGE_213::circle>
PGE_213::decartes (circle& c1, circle& c2, circle& c3)
{
  std::vector<circle> result_a;

  //Decartes Theorem: Given three tangent circles we can find a fourth and a fifth.
  //https://en.wikipedia.org/wiki/Descartes%27_theorem
  float root_f = std::sqrt ((c1.k_ * c2.k_) + (c3.k_ * c2.k_) + (c1.k_ * c3.k_));
  float k_plus  = c1.k_ + c2.k_ + c3.k_ + (2.0f * root_f);
  float k_minus = c1.k_ + c2.k_ + c3.k_ - (2.0f * root_f);

  std::complex<float> c12 = c1.z_ * c2.z_;
  std::complex<float> c23 = c2.z_ * c3.z_;
  std::complex<float> c31 = c3.z_ * c1.z_;

  std::complex<float> t1 = c1.z_ + c2.z_ + c3.z_;
  std::complex<float> t2 = c12 + c23 + c31;
  std::complex<float> t3 = std::sqrt (t2) * 2.0f;

  std::complex<float> z_plus = t1 + t3;
  std::complex<float> z_minus = t1 - t3;

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
