#ifndef PGE_2_H
#define PGE_2_H

#include "noise/noise.h"

#include "gtk/gtk.h"

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "common_ui_gtk_common.h"

#include "defines.h"

// GTK idle routines
gboolean idle_initialize_UI_2_cb (gpointer);
gboolean idle_finalize_UI_2_cb (gpointer);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
G_MODULE_EXPORT void scale_iterations_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_dt_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_diffusion_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_viscosity_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void button_reset_2_clicked_cb (GtkButton*, gpointer);
G_MODULE_EXPORT void button_clear_clicked_cb (GtkButton*, gpointer);
#ifdef __cplusplus
}
#endif /* __cplusplus */

struct Engine_UI_GTK_2_CBData
 : Common_UI_GTK_CBData
{
  bool  clearScreen;
  int   iterations;
  float dt;
  float diffusion;
  float viscosity;
};

//////////////////////////////////////////

// *NOTE*: see also: https://www.dgp.toronto.edu/public_user/stam/reality/Research/pdf/GDC03.pdf
class PGE_2
 : public olc::PixelGameEngine
{
 public:
  PGE_2 (struct Engine_UI_GTK_2_CBData*);
  ~PGE_2 ();

  inline void addDensity (int x, int y, float amount) { x_[IX(x, y)] += amount; }
  void addVelocity (int, int, float, float);

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void set_bounds (int, float[]);
  void solve (int, float[], float[], float, float, int);
  void diffuse (int, float[], float[], float, float, int);
  void project (float[], float[], float[], float[], int);
  void advect (int, float[], float[], float[], float[], float);
  int IX (int, int);

  void reset ();
  void render_density ();
  void render_velocity ();
  void fade_density ();

  // cb data
  struct Engine_UI_GTK_2_CBData* CBData_;

  // density
  float* x_;
  float* x0_;
  // velocity
  float* v_x_;
  float* v_y_;
  float* v_x0_;
  float* v_y0_;

  int    resolution_;

  // noise
  double                z_;
  noise::module::Perlin module_;
};

#endif // PGE_2_H
