#ifndef PGE_440_H
#define PGE_440_H

#include <vector>

#include "gtk/gtk.h"

#include "olcPixelGameEngine.h"

#include "common_ui_gtk_common.h"

// GTK idle routines
gboolean idle_initialize_UI_440_cb (gpointer);
gboolean idle_finalize_UI_440_cb (gpointer);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
G_MODULE_EXPORT void scale_alpha_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_beta_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_gamma_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void button_reset_clicked_cb (GtkButton*, gpointer);
G_MODULE_EXPORT void button_clear_clicked_cb (GtkButton*, gpointer);
#ifdef __cplusplus
}
#endif /* __cplusplus */

struct Engine_UI_GTK_440_CBData
 : Common_UI_GTK_CBData
{
  float* alpha;
  float* beta;
  float* gamma;

  bool   clearScreen;
};

//////////////////////////////////////////

class PGE_440
 : public olc::PixelGameEngine
{
 public:
  PGE_440 (struct Engine_UI_GTK_440_CBData&);
  inline virtual ~PGE_440 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void reset ();

  std::vector<std::vector<std::vector<float> > > a_, b_, c_;
  int columns_, rows_;
  int size_;
  int p_, q_;
  float alpha_, beta_, gamma_;
  struct Engine_UI_GTK_440_CBData* CBData_;
};

#endif // PGE_440_H
