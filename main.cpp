#include "stdafx.h"

#include <iostream>
#include <string>

#include "GL/freeglut.h"

#include "ace/config-lite.h"
#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Init_ACE.h"
#include "ace/Profile_Timer.h"
#include "ace/Time_Value.h"

#include "Common_config.h"

#include "common_file_tools.h"
#include "common_process_tools.h"
#include "common_tools.h"

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "common_log_tools.h"

#include "common_timer_tools.h"

#include "common_ui_gtk_common.h"
#include "common_ui_gtk_builder_definition.h"
#include "common_ui_gtk_manager_common.h"

#if defined (HAVE_CONFIG_H)
#include "config.h"
#endif // HAVE_CONFIG_H
//#include "resource.h"

#include "defines.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#include "pge.h"
#include "pge_2.h"
#include "glut_3.h"
#include "glut_4.h"
#include "pge_5.h"
#include "pge_6.h"
#include "pge_6_2.h"
#include "pge_7.h"
#include "pge_8.h"
#include "pge_9.h"
#include "pge_10.h"
#include "pge_11.h"
#include "pge_12.h"
#include "pge_13.h"
#include "pge_14.h"
#include "pge_15.h"
#include "glut_16.h"
#include "pge_17.h"
#include "glut_18.h"
#include "pge_19.h"
#include "pge_20.h"
#include "pge_21.h"
#include "pge_22.h"
#include "pge_23.h"
#include "pge_24.h"
#include "glut_25.h"
#include "pge_26.h"
#include "pge_27.h"
#include "pge_28.h"
#include "pge_29.h"
#include "pge_30.h"
#include "pge_31.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 1,
  ENGINE_MODE_2,
  ENGINE_MODE_3,
  ENGINE_MODE_4,
  ENGINE_MODE_5,
  ENGINE_MODE_6,
  ENGINE_MODE_7,
  ENGINE_MODE_8,
  ENGINE_MODE_9,
  ENGINE_MODE_10,
  ENGINE_MODE_11,
  ENGINE_MODE_12,
  ENGINE_MODE_13,
  ENGINE_MODE_14,
  ENGINE_MODE_15,
  ENGINE_MODE_16,
  ENGINE_MODE_17,
  ENGINE_MODE_18,
  ENGINE_MODE_19,
  ENGINE_MODE_20,
  ENGINE_MODE_21,
  ENGINE_MODE_22,
  ENGINE_MODE_23,
  ENGINE_MODE_24,
  ENGINE_MODE_25,
  ENGINE_MODE_26,
  ENGINE_MODE_27,
  ENGINE_MODE_28,
  ENGINE_MODE_29,
  ENGINE_MODE_30,
  ENGINE_MODE_31,
  ////////////////////////////////////////
  ENGINE_MODE_MAX,
  ENGINE_MODE_INVALID
};

void
do_print_usage (const std::string& programName_in)
{
  // enable verbatim boolean output
  std::cout.setf (std::ios::boolalpha);

  std::string path_root =
    Common_File_Tools::getWorkingDirectory ();

  std::cout << ACE_TEXT_ALWAYS_CHAR ("usage: ")
            << programName_in
            << ACE_TEXT_ALWAYS_CHAR (" [OPTIONS]")
            << std::endl
            << std::endl;
  std::cout << ACE_TEXT_ALWAYS_CHAR ("currently available options:")
            << std::endl;
  std::string ui_definition_file_path = path_root;
  ui_definition_file_path += ACE_DIRECTORY_SEPARATOR_CHAR_A;
  ui_definition_file_path +=
    ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_16_UI_DEFINITION_FILE);
  std::cout << ACE_TEXT_ALWAYS_CHAR ("-g[PATH]   : ui definition file [")
            << ui_definition_file_path
            << ACE_TEXT_ALWAYS_CHAR ("]")
            << std::endl;
  std::cout << ACE_TEXT_ALWAYS_CHAR ("-l         : log to a file [")
            << false
            << ACE_TEXT_ALWAYS_CHAR ("]")
            << std::endl;
  std::cout << ACE_TEXT_ALWAYS_CHAR ("-m         : program mode [")
            << ENGINE_MODE_DEFAULT
            << ACE_TEXT_ALWAYS_CHAR ("]")
            << std::endl;
  std::cout << ACE_TEXT_ALWAYS_CHAR ("-t         : trace information [")
            << false
            << ACE_TEXT_ALWAYS_CHAR ("]")
            << std::endl;
  std::cout << ACE_TEXT_ALWAYS_CHAR ("-v         : print version information and exit [")
            << false
            << ACE_TEXT_ALWAYS_CHAR ("]")
            << std::endl;
}

bool
do_process_arguments (int argc_in,
                      ACE_TCHAR** argv_in, // cannot be const...
                      std::string& UIDefinitionFilePath_out,
                      bool& logToFile_out,
                      enum Engine_ModeType& mode_out,
                      bool& traceInformation_out,
                      bool& printVersionAndExit_out)
{
  std::string path_root =
    Common_File_Tools::getWorkingDirectory();

  // initialize results
  UIDefinitionFilePath_out = path_root;
  UIDefinitionFilePath_out += ACE_DIRECTORY_SEPARATOR_CHAR_A;
  UIDefinitionFilePath_out +=
    ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_16_UI_DEFINITION_FILE);
  logToFile_out = false;
  mode_out = ENGINE_MODE_DEFAULT;
  traceInformation_out = false;
  printVersionAndExit_out = false;

  ACE_Get_Opt argument_parser (argc_in,
                               argv_in,
                               ACE_TEXT ("g::lm:tv"),
                               1,                         // skip command name
                               1,                         // report parsing errors
                               ACE_Get_Opt::PERMUTE_ARGS, // ordering
                               0);                        // for now, don't use long options

  int option = 0;
//  std::stringstream converter;
  while ((option = argument_parser ()) != EOF)
  {
    switch (option)
    {
      case 'g':
      {
        UIDefinitionFilePath_out =
          (argument_parser.opt_arg () ? ACE_TEXT_ALWAYS_CHAR (argument_parser.opt_arg ())
                                      : ACE_TEXT_ALWAYS_CHAR (""));
        break;
      }
      case 'l':
      {
        logToFile_out = true;
        break;
      }
      case 'm':
      {
        std::istringstream converter (ACE_TEXT_ALWAYS_CHAR (argument_parser.opt_arg ()),
                                      std::ios_base::in);
        int i = 0;
        converter >> i;
        mode_out = static_cast<enum Engine_ModeType> (i);
        break;
      }
      case 't':
      {
        traceInformation_out = true;
        break;
      }
      case 'v':
      {
        printVersionAndExit_out = true;
        break;
      }
      // error handling
      case ':':
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("option \"%c\" requires an argument, aborting\n"),
                    argument_parser.opt_opt ()));
        return false;
      }
      case '?':
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("unrecognized option \"%s\", aborting\n"),
                    ACE_TEXT (argument_parser.last_option ())));
        return false;
      }
      case 0:
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("found long option \"%s\", aborting\n"),
                    ACE_TEXT (argument_parser.long_option ())));
        return false;
      }
      default:
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("parse error, aborting\n")));
        return false;
      }
    } // end SWITCH
  } // end WHILE

  return true;
}

bool
do_work (int argc_in,
         ACE_TCHAR* argv_in[],
         const std::string& UIDefinitionFilePath_in,
         enum Engine_ModeType mode_in)
{
  bool result = false;

#if defined (ACE_WIN32) || defined (ACE_WIN64)
  Common_Tools::initialize (false, // initialize COM ?
                            true); // initialize RNG
#else
  Common_Tools::initialize (true); // initialize RNG
#endif // ACE_WIN32 || ACE_WIN64

  switch (mode_in)
  {
    case ENGINE_MODE_DEFAULT:
    {
      PGE example;
      if (example.Construct (640, 480,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_2:
    {
      // initialize GTK
      Common_UI_GTK_Configuration_t gtk_configuration;
      struct Engine_UI_GTK_2_CBData ui_cb_data;
      ui_cb_data.clearScreen = false;
      ui_cb_data.iterations = ENGINE_PGE_2_DEFAULT_NUM_ITERATIONS;
      ui_cb_data.dt = ENGINE_PGE_2_DEFAULT_DT;
      ui_cb_data.diffusion = ENGINE_PGE_2_DEFAULT_DIFFUSION;
      ui_cb_data.viscosity = ENGINE_PGE_2_DEFAULT_VISCOSITY;
      Common_UI_GtkBuilderDefinition_t gtk_ui_definition;
      Common_UI_GTK_Manager_t* gtk_manager_p =
        COMMON_UI_GTK_MANAGER_SINGLETON::instance ();
      ACE_ASSERT (gtk_manager_p);
      Common_UI_GTK_State_t& state_r =
        const_cast<Common_UI_GTK_State_t&> (gtk_manager_p->getR ());

      gtk_configuration.argc = argc_in;
      gtk_configuration.argv = argv_in;
      gtk_configuration.CBData = &ui_cb_data;
      gtk_configuration.eventHooks.finiHook = idle_finalize_UI_2_cb;
      gtk_configuration.eventHooks.initHook = idle_initialize_UI_2_cb;
      gtk_configuration.definition = &gtk_ui_definition;

      ui_cb_data.UIState = &state_r;
      ui_cb_data.progressData.state = &state_r;

      state_r.builders[ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN)] =
        std::make_pair (UIDefinitionFilePath_in, static_cast<GtkBuilder*> (NULL));

      bool result_2 = gtk_manager_p->initialize (gtk_configuration);
      if (!result_2)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to Common_UI_GTK_Manager_T::initialize(), aborting\n")));
        return false;
      } // end IF

      gtk_manager_p->start ();
      if (!gtk_manager_p->isRunning ())
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to start GTK event dispatch, aborting\n")));
        return false;
      } // end IF

      PGE_2 example (&ui_cb_data);
      if (example.Construct (ENGINE_PGE_2_DEFAULT_SIZE, ENGINE_PGE_2_DEFAULT_SIZE,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      gtk_manager_p->stop (true,   // wait ?
                           false);

      break;
    }
    case ENGINE_MODE_3:
    {
      struct Engine_OpenGL_GLUT_3_CBData cb_data_s;
#if defined (ACE_WIN32) || defined (ACE_WIN64)
      cb_data_s.size.cx = 1500;
      cb_data_s.size.cy = 1500;
#else
      cb_data_s.size.width = 1500;
      cb_data_s.size.height = 1500;
#endif // ACE_WIN32 || ACE_WIN64
      cb_data_s.scaleFactor = 15;
#if defined (ACE_WIN32) || defined (ACE_WIN64)
      cb_data_s.columns = cb_data_s.size.cx / cb_data_s.scaleFactor;
      cb_data_s.rows = cb_data_s.size.cy / cb_data_s.scaleFactor;
#else
      cb_data_s.columns = cb_data_s.size.width / cb_data_s.scaleFactor;
      cb_data_s.rows = cb_data_s.size.height / cb_data_s.scaleFactor;
#endif // ACE_WIN32 || ACE_WIN64
      cb_data_s.wireframe = true;
      cb_data_s.color = false;

      cb_data_s.angle = 0.0F;
#if defined (ACE_WIN32) || defined (ACE_WIN64)
      cb_data_s.camera.position.x = cb_data_s.size.cx / 2.0F;
#else
      cb_data_s.camera.position.x = cb_data_s.size.width / 2.0F;
#endif // ACE_WIN32 || ACE_WIN64
      cb_data_s.camera.position.y = -300.0F;
      cb_data_s.camera.position.z = 600.0F;
#if defined (ACE_WIN32) || defined (ACE_WIN64)
      cb_data_s.camera.looking_at.x = cb_data_s.size.cx / 2.0F;
      cb_data_s.camera.looking_at.y = cb_data_s.size.cy / 2.0F;
#else
      cb_data_s.camera.looking_at.x = cb_data_s.size.width / 2.0F;
      cb_data_s.camera.looking_at.y = cb_data_s.size.height / 2.0F;
#endif // ACE_WIN32 || ACE_WIN64
      cb_data_s.camera.looking_at.z = 0.0F;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 0.0F;
      cb_data_s.camera.up.z = 1.0F;

      cb_data_s.deltaAngle = 0.0F;
      cb_data_s.xOrigin = -1;

      cb_data_s.x = ENGINE_GLUT_3_DEFAULT_NOISE_X;
      cb_data_s.y = ENGINE_GLUT_3_DEFAULT_NOISE_Y;
      cb_data_s.z = ENGINE_GLUT_3_DEFAULT_NOISE_Z;
      cb_data_s.step = ENGINE_GLUT_3_DEFAULT_NOISE_STEP;
      cb_data_s.module.SetSeed (static_cast<int> (Common_Tools::randomSeed));
      cb_data_s.module.SetFrequency (ENGINE_GLUT_3_DEFAULT_NOISE_FREQUENCY);
      cb_data_s.module.SetOctaveCount (ENGINE_GLUT_3_DEFAULT_NOISE_OCTAVES);
      cb_data_s.module.SetPersistence (ENGINE_GLUT_3_DEFAULT_NOISE_PERSISTENCE);
      cb_data_s.module.SetLacunarity (ENGINE_GLUT_3_DEFAULT_NOISE_LACUNARITY);
      //cb_data_s.module.SetNoiseQuality (noise::QUALITY_BEST);

      cb_data_s.level = ENGINE_GLUT_3_DEFAULT_NOISE_LEVEL;
      ACE_NEW_NORETURN (cb_data_s.terrain,
                        float[cb_data_s.columns * cb_data_s.rows]);
      ACE_ASSERT (cb_data_s.terrain);
      cb_data_s.offset = 0.03;

      // initialize GTK
      Common_UI_GTK_Configuration_t gtk_configuration;
      struct Engine_UI_GTK_3_CBData ui_cb_data;
      ui_cb_data.GLUT_CBData = &cb_data_s;
      Common_UI_GtkBuilderDefinition_t gtk_ui_definition;
      Common_UI_GTK_Manager_t* gtk_manager_p =
        COMMON_UI_GTK_MANAGER_SINGLETON::instance ();
      ACE_ASSERT (gtk_manager_p);
      Common_UI_GTK_State_t& state_r =
        const_cast<Common_UI_GTK_State_t&> (gtk_manager_p->getR ());

      gtk_configuration.argc = argc_in;
      gtk_configuration.argv = argv_in;
      gtk_configuration.CBData = &ui_cb_data;
      gtk_configuration.eventHooks.finiHook = idle_finalize_UI_3_cb;
      gtk_configuration.eventHooks.initHook = idle_initialize_UI_3_cb;
      gtk_configuration.definition = &gtk_ui_definition;

      ui_cb_data.UIState = &state_r;
      ui_cb_data.progressData.state = &state_r;

      state_r.builders[ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN)] =
        std::make_pair (UIDefinitionFilePath_in, static_cast<GtkBuilder*> (NULL));

      bool result_2 = gtk_manager_p->initialize (gtk_configuration);
      if (!result_2)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to Common_UI_GTK_Manager_T::initialize(), aborting\n")));
        return false;
      } // end IF

      gtk_manager_p->start ();
      if (!gtk_manager_p->isRunning ())
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to start GTK event dispatch, aborting\n")));
        return false;
      } // end IF

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (640, 480);

      int window_i = glutCreateWindow ("engine GLUT 3");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0F, 0.0F, 0.0F, 0.0F); // Black Background
      COMMON_GL_ASSERT;

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      COMMON_GL_ASSERT;

      glutDisplayFunc (engine_glut_3_draw);
      glutReshapeFunc (engine_glut_3_reshape);
      glutVisibilityFunc (engine_glut_3_visible);

      glutKeyboardFunc (engine_glut_3_key);
      glutSpecialFunc (engine_glut_3_key_special);
      glutMouseFunc (engine_glut_3_mouse_button);
      glutMotionFunc (engine_glut_3_mouse_move);
      glutTimerFunc (100, engine_glut_3_timer, 0);

      glutCreateMenu (engine_glut_3_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), ENGINE_GLUT_MODE_WIREFRAME);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("color"), ENGINE_GLUT_MODE_COLOR);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      gtk_manager_p->stop (true,   // wait ?
                           false);

      result = true;

      break;
    }
    case ENGINE_MODE_4:
    {
      struct Engine_OpenGL_GLUT_3_CBData cb_data_s;
#if defined (ACE_WIN32) || defined (ACE_WIN64)
      cb_data_s.size.cx = 1500;
      cb_data_s.size.cy = 1500;
#else
      cb_data_s.size.width = 1500;
      cb_data_s.size.height = 1500;
#endif // ACE_WIN32 || ACE_WIN64
      cb_data_s.scaleFactor = 15;
#if defined (ACE_WIN32) || defined (ACE_WIN64)
      cb_data_s.columns = cb_data_s.size.cx / cb_data_s.scaleFactor;
      cb_data_s.rows = cb_data_s.size.cy / cb_data_s.scaleFactor;
#else
      cb_data_s.columns = cb_data_s.size.width / cb_data_s.scaleFactor;
      cb_data_s.rows = cb_data_s.size.height / cb_data_s.scaleFactor;
#endif // ACE_WIN32 || ACE_WIN64
      cb_data_s.wireframe = true;
      cb_data_s.color = false;

      cb_data_s.angle = 0.0F;
#if defined (ACE_WIN32) || defined (ACE_WIN64)
      cb_data_s.camera.position.x = cb_data_s.size.cx / 2.0F;
#else
      cb_data_s.camera.position.x = cb_data_s.size.width / 2.0F;
#endif // ACE_WIN32 || ACE_WIN64
      cb_data_s.camera.position.y = -300.0F;
      cb_data_s.camera.position.z = 600.0F;
#if defined (ACE_WIN32) || defined (ACE_WIN64)
      cb_data_s.camera.looking_at.x = cb_data_s.size.cx / 2.0F;
      cb_data_s.camera.looking_at.y = cb_data_s.size.cy / 2.0F;
#else
      cb_data_s.camera.looking_at.x = cb_data_s.size.width / 2.0F;
      cb_data_s.camera.looking_at.y = cb_data_s.size.height / 2.0F;
#endif // ACE_WIN32 || ACE_WIN64
      cb_data_s.camera.looking_at.z = 0.0F;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 0.0F;
      cb_data_s.camera.up.z = 1.0F;

      cb_data_s.deltaAngle = 0.0F;
      cb_data_s.xOrigin = -1;

      cb_data_s.x = ENGINE_GLUT_3_DEFAULT_NOISE_X;
      cb_data_s.y = ENGINE_GLUT_3_DEFAULT_NOISE_Y;
      cb_data_s.z = ENGINE_GLUT_3_DEFAULT_NOISE_Z;
      cb_data_s.step = ENGINE_GLUT_3_DEFAULT_NOISE_STEP;
      cb_data_s.module.SetSeed (static_cast<int> (Common_Tools::randomSeed));
      cb_data_s.module.SetFrequency (ENGINE_GLUT_3_DEFAULT_NOISE_FREQUENCY);
      cb_data_s.module.SetOctaveCount (ENGINE_GLUT_3_DEFAULT_NOISE_OCTAVES);
      cb_data_s.module.SetPersistence (ENGINE_GLUT_3_DEFAULT_NOISE_PERSISTENCE);
      cb_data_s.module.SetLacunarity (ENGINE_GLUT_3_DEFAULT_NOISE_LACUNARITY);
      //cb_data_s.module.SetNoiseQuality (noise::QUALITY_BEST);

      cb_data_s.level = ENGINE_GLUT_3_DEFAULT_NOISE_LEVEL;
      ACE_NEW_NORETURN (cb_data_s.terrain,
                        float[cb_data_s.columns * cb_data_s.rows]);
      ACE_ASSERT (cb_data_s.terrain);
      cb_data_s.offset = 0.03;

      // initialize GTK
      Common_UI_GTK_Configuration_t gtk_configuration;
      struct Engine_UI_GTK_3_CBData ui_cb_data;
      ui_cb_data.GLUT_CBData = &cb_data_s;
      Common_UI_GtkBuilderDefinition_t gtk_ui_definition;
      Common_UI_GTK_Manager_t* gtk_manager_p =
        COMMON_UI_GTK_MANAGER_SINGLETON::instance ();
      ACE_ASSERT (gtk_manager_p);
      Common_UI_GTK_State_t& state_r =
        const_cast<Common_UI_GTK_State_t&> (gtk_manager_p->getR ());

      gtk_configuration.argc = argc_in;
      gtk_configuration.argv = argv_in;
      gtk_configuration.CBData = &ui_cb_data;
      gtk_configuration.eventHooks.finiHook = idle_finalize_UI_3_cb;
      gtk_configuration.eventHooks.initHook = idle_initialize_UI_3_cb;
      gtk_configuration.definition = &gtk_ui_definition;

      ui_cb_data.UIState = &state_r;
      ui_cb_data.progressData.state = &state_r;

      state_r.builders[ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN)] =
        std::make_pair (UIDefinitionFilePath_in, static_cast<GtkBuilder*> (NULL));

      bool result_2 = gtk_manager_p->initialize (gtk_configuration);
      if (!result_2)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to Common_UI_GTK_Manager_T::initialize(), aborting\n")));
        return false;
      } // end IF

      gtk_manager_p->start ();
      if (!gtk_manager_p->isRunning ())
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to start GTK event dispatch, aborting\n")));
        return false;
      } // end IF

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (640, 480);

      int window_i = glutCreateWindow ("engine GLUT 4");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0F, 0.0F, 0.0F, 0.0F); // Black Background
      COMMON_GL_ASSERT;

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      COMMON_GL_ASSERT;

      glutDisplayFunc (engine_glut_4_draw);
      glutReshapeFunc (engine_glut_3_reshape);
      glutVisibilityFunc (engine_glut_3_visible);

      glutKeyboardFunc (engine_glut_3_key);
      glutSpecialFunc (engine_glut_3_key_special);
      glutMouseFunc (engine_glut_3_mouse_button);
      glutMotionFunc (engine_glut_3_mouse_move);
      glutTimerFunc (100, engine_glut_3_timer, 0);

      glutCreateMenu (engine_glut_3_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), ENGINE_GLUT_MODE_WIREFRAME);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("color"), ENGINE_GLUT_MODE_COLOR);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      gtk_manager_p->stop (true,   // wait ?
                           false);

      result = true;

      break;
    }
    case ENGINE_MODE_5:
    {
      PGE_5 example;
      if (example.Construct (ENGINE_PGE_5_DEFAULT_WIDTH, ENGINE_PGE_5_DEFAULT_HEIGHT,
                             4, 4,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_6:
    {
      PGE_6_2 example;
      if (example.Construct (ENGINE_PGE_6_2_DEFAULT_WIDTH, ENGINE_PGE_6_2_DEFAULT_HEIGHT,
                             2, 2,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_7:
    {
      PGE_7 example;
      if (example.Construct (ENGINE_PGE_7_DEFAULT_WIDTH, ENGINE_PGE_7_DEFAULT_HEIGHT,
                             4, 4,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_8:
    {
      PGE_8 example;
      if (example.Construct (ENGINE_PGE_8_DEFAULT_WIDTH, ENGINE_PGE_8_DEFAULT_HEIGHT,
                             4, 4,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_9:
    {
      PGE_9 example;
      if (example.Construct (ENGINE_PGE_9_DEFAULT_WIDTH, ENGINE_PGE_9_DEFAULT_HEIGHT,
                             2, 2,
                             true,   // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_10:
    {
      PGE_10 example;
      if (example.Construct (ENGINE_PGE_10_DEFAULT_WIDTH, ENGINE_PGE_10_DEFAULT_HEIGHT,
                             2, 2,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_11:
    {
      PGE_11 example;
      if (example.Construct (ENGINE_PGE_11_DEFAULT_WIDTH, ENGINE_PGE_11_DEFAULT_HEIGHT,
                             2, 2,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_12:
    {
      PGE_12 example;
      if (example.Construct (ENGINE_PGE_12_DEFAULT_WIDTH, ENGINE_PGE_12_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_13:
    {
      PGE_13 example;
      if (example.Construct (ENGINE_PGE_13_DEFAULT_WIDTH, ENGINE_PGE_13_DEFAULT_HEIGHT,
                             2, 2,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_14:
    {
      PGE_14 example;
      if (example.Construct (ENGINE_PGE_14_DEFAULT_WIDTH, ENGINE_PGE_14_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             true,   // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_15:
    {
      PGE_15 example;
      if (example.Construct (ENGINE_PGE_15_DEFAULT_WIDTH, ENGINE_PGE_15_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             true,   // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_16:
    {
      struct Engine_OpenGL_GLUT_16_CBData cb_data_s;
      cb_data_s.wireframe = true;
      cb_data_s.color = false;

      cb_data_s.camera.position.x = 0.0F;
      cb_data_s.camera.position.y = -600.0F;
      cb_data_s.camera.position.z = 0.0F;
      cb_data_s.camera.looking_at.x = 0.0F;
      cb_data_s.camera.looking_at.y = 0.0F;
      cb_data_s.camera.looking_at.z = 0.0F;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 0.0F;
      cb_data_s.camera.up.z = 1.0F;

      cb_data_s.spinning = false;
      cb_data_s.xangle = 0.0F;
      cb_data_s.xdeltaAngle = 0.0F;
      cb_data_s.xOrigin = -1;
      cb_data_s.yangle = 0.0F;
      cb_data_s.ydeltaAngle = 0.0F;
      cb_data_s.yOrigin = -1;

      cb_data_s.total = 75;
      cb_data_s.radius = 200;
      ACE_NEW_NORETURN (cb_data_s.globe,
                        glm::vec3[(cb_data_s.total + 1) * (cb_data_s.total + 1)]);
      ACE_ASSERT (cb_data_s.globe);
      cb_data_s.m = ENGINE_GLUT_16_DEFAULT_M;
      cb_data_s.n1 = ENGINE_GLUT_16_DEFAULT_N1;
      cb_data_s.n2 = ENGINE_GLUT_16_DEFAULT_N2;
      cb_data_s.n3 = ENGINE_GLUT_16_DEFAULT_N3;
      cb_data_s.a = 1.0F;
      cb_data_s.b = 1.0F;

      // initialize GTK
      Common_UI_GTK_Configuration_t gtk_configuration;
      struct Engine_UI_GTK_16_CBData ui_cb_data;
      ui_cb_data.GLUT_CBData = &cb_data_s;
      Common_UI_GtkBuilderDefinition_t gtk_ui_definition;
      Common_UI_GTK_Manager_t* gtk_manager_p =
        COMMON_UI_GTK_MANAGER_SINGLETON::instance ();
      ACE_ASSERT (gtk_manager_p);
      Common_UI_GTK_State_t& state_r =
        const_cast<Common_UI_GTK_State_t&> (gtk_manager_p->getR ());

      gtk_configuration.argc = argc_in;
      gtk_configuration.argv = argv_in;
      gtk_configuration.CBData = &ui_cb_data;
      gtk_configuration.eventHooks.finiHook = idle_finalize_UI_16_cb;
      gtk_configuration.eventHooks.initHook = idle_initialize_UI_16_cb;
      gtk_configuration.definition = &gtk_ui_definition;

      ui_cb_data.UIState = &state_r;
      ui_cb_data.progressData.state = &state_r;

      state_r.builders[ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN)] =
        std::make_pair (UIDefinitionFilePath_in, static_cast<GtkBuilder*> (NULL));

      bool result_2 = gtk_manager_p->initialize (gtk_configuration);
      if (!result_2)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to Common_UI_GTK_Manager_T::initialize(), aborting\n")));
        return false;
      } // end IF

      gtk_manager_p->start ();
      if (!gtk_manager_p->isRunning ())
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to start GTK event dispatch, aborting\n")));
        return false;
      } // end IF

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (640, 480);

      int window_i = glutCreateWindow ("engine GLUT 16");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0F, 0.0F, 0.0F, 0.0F); // Black Background
      COMMON_GL_ASSERT;

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      COMMON_GL_ASSERT;

      glutDisplayFunc (engine_glut_16_draw);
      glutReshapeFunc (engine_glut_3_reshape);
      glutVisibilityFunc (engine_glut_3_visible);

      glutKeyboardFunc (engine_glut_3_key);
      glutSpecialFunc (engine_glut_16_key_special);
      glutMouseFunc (engine_glut_16_mouse_button);
      glutMotionFunc (engine_glut_16_mouse_move);
      glutTimerFunc (100, engine_glut_16_timer, 0);

      glutCreateMenu (engine_glut_16_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), ENGINE_GLUT_MODE_WIREFRAME);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("color"), ENGINE_GLUT_MODE_COLOR);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      gtk_manager_p->stop (true,   // wait ?
                           false);

      result = true;

      break;
    }
    case ENGINE_MODE_17:
    {
      PGE_17 example;
      if (example.Construct (ENGINE_PGE_17_DEFAULT_WIDTH, ENGINE_PGE_17_DEFAULT_HEIGHT,
                             2, 2,
                             false,  // fullscreen ?
                             true,   // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_18:
    {
      struct Engine_OpenGL_GLUT_18_CBData cb_data_s;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = -ENGINE_GLUT_18_DEFAULT_HEIGHT / 2.0f;
      cb_data_s.camera.position.z = -1500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 0.0f;
      cb_data_s.camera.up.z = 1.0f;

      cb_data_s.xangle = 0.0f;
      cb_data_s.xDeltaAngle = 0.0f;
      cb_data_s.xOrigin = -1;

      cb_data_s.rules.push_back (std::make_pair ('F', ACE_TEXT_ALWAYS_CHAR ("FF+[+F-F-F]-[-F+F+F]")));
      cb_data_s.sentence = ACE_TEXT_ALWAYS_CHAR ("F");
      //cb_data_s.angle = static_cast<float> (25.0 * M_PI / 180.0);
      cb_data_s.angle = 25.0f;
      cb_data_s.length = 100.0f;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
      glutInitWindowSize (ENGINE_GLUT_18_DEFAULT_WIDTH, ENGINE_GLUT_18_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 18");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0F, 0.0F, 0.0F, 0.0F); // Black Background
      COMMON_GL_ASSERT;

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      COMMON_GL_ASSERT;

      glDisable (GL_CULL_FACE);

      glutDisplayFunc (engine_glut_18_draw);
      glutReshapeFunc (engine_glut_18_reshape);
      glutVisibilityFunc (engine_glut_3_visible);

      glutKeyboardFunc (engine_glut_3_key);
      //glutSpecialFunc (engine_glut_3_key_special);
      glutMouseFunc (engine_glut_18_mouse_button);
      //glutMotionFunc (engine_glut_3_mouse_move);
      //glutTimerFunc (100, engine_glut_3_timer, 0);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_19:
    {
      PGE_19 example;
      if (example.Construct (ENGINE_PGE_19_DEFAULT_WIDTH, ENGINE_PGE_19_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             true,   // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_20:
    {
      PGE_20 example;
      if (example.Construct (ENGINE_PGE_20_DEFAULT_WIDTH, ENGINE_PGE_20_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             true,   // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_21:
    {
      PGE_21 example;
      if (example.Construct (ENGINE_PGE_21_DEFAULT_WIDTH, ENGINE_PGE_21_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             true,   // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_22:
    {
      PGE_22 example;
      if (example.Construct (ENGINE_PGE_22_DEFAULT_WIDTH, ENGINE_PGE_22_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             true,   // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_23:
    {
      PGE_23 example;
      if (example.Construct (ENGINE_PGE_23_DEFAULT_WIDTH, ENGINE_PGE_23_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_24:
    {
      PGE_24 example;
      if (example.Construct (ENGINE_PGE_24_DEFAULT_WIDTH, ENGINE_PGE_24_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_25:
    {
      struct Engine_OpenGL_GLUT_25_CBData cb_data_s;

      cb_data_s.camera.init (Camera::QWERTY,
                             0.0f,
                             0.0f,
                             -500.0f,
                             4.0f * static_cast<float> (M_PI) / 7.0f,
                             static_cast<float> (M_PI) / 4.0f,
                             0.01f,
                             0.5f,
                             ENGINE_GLUT_25_DEFAULT_WIDTH,
                             ENGINE_GLUT_25_DEFAULT_HEIGHT);
      cb_data_s.dimension = ENGINE_GLUT_25_DEFAULT_DIMENSION;

      engine_glut_25_generate (cb_data_s);

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
      glutInitWindowSize (ENGINE_GLUT_25_DEFAULT_WIDTH, ENGINE_GLUT_25_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 25");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0F, 0.0F, 0.0F, 0.0F); // Black Background
      COMMON_GL_ASSERT;

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      COMMON_GL_ASSERT;

      glDisable (GL_CULL_FACE);

      glutDisplayFunc (engine_glut_25_draw);
      glutReshapeFunc (engine_glut_25_reshape);
      glutVisibilityFunc (engine_glut_3_visible);

      glutKeyboardFunc (engine_glut_25_key_down);
      glutKeyboardUpFunc (engine_glut_25_key_up);
      glutSpecialFunc (engine_glut_25_key_special);
      //glutMouseFunc (engine_glut_25_mouse_button);
      glutMotionFunc (engine_glut_25_mouse_move);
      //glutTimerFunc (100, engine_glut_25_timer, 0);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_26:
    {
      PGE_26 example;
      if (example.Construct (ENGINE_PGE_26_DEFAULT_WIDTH, ENGINE_PGE_26_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_27:
    {
      PGE_27 example (UIDefinitionFilePath_in);
      if (example.Construct (ENGINE_PGE_27_DEFAULT_WIDTH, ENGINE_PGE_27_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      Common_UI_GTK_Manager_t* gtk_manager_p =
        COMMON_UI_GTK_MANAGER_SINGLETON::instance ();
      ACE_ASSERT (gtk_manager_p);
      gtk_manager_p->stop (true,   // wait ?
                           false);

      break;
    }
    case ENGINE_MODE_28:
    {
      PGE_28 example;
      if (example.Construct (ENGINE_PGE_28_DEFAULT_WIDTH, ENGINE_PGE_28_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_29:
    {
      PGE_29 example;
      if (example.Construct (ENGINE_PGE_29_DEFAULT_WIDTH, ENGINE_PGE_29_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_30:
    {
      PGE_30 example;
      if (example.Construct (ENGINE_PGE_30_DEFAULT_WIDTH, ENGINE_PGE_30_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    case ENGINE_MODE_31:
    {
      PGE_31 example;
      if (example.Construct (ENGINE_PGE_31_DEFAULT_WIDTH, ENGINE_PGE_31_DEFAULT_HEIGHT,
                             1, 1,
                             false,  // fullscreen ?
                             false,  // vsync ?
                             false)) // cohesion ?
      {
        example.Start ();
        result = true;
      } // end IF

      break;
    }
    default:
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("invalid/unknown mode (was: %d), returning\n"),
                  mode_in));
      break;
    }
  } // end SWITCH

//clean:
  return result;
}

COMMON_DEFINE_PRINTVERSION_FUNCTION (do_printVersion, MAKE_VERSION_STRING_VARIABLE (programName_in, ACE_TEXT_ALWAYS_CHAR (engine_PACKAGE_VERSION_FULL), version_string), version_string)

int
ACE_TMAIN (int argc_in,
           ACE_TCHAR* argv_in[])
{
  int result = EXIT_FAILURE, result_2 = -1;
  bool result_3 = false;

  // *PORTABILITY*: on Windows, initialize ACE
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  result_2 = ACE::init ();
  if (result_2 == -1)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to ACE::init(): \"%m\", aborting\n")));
    return EXIT_FAILURE;
  } // end IF
#endif // ACE_WIN32 || ACE_WIN64
  ACE_Profile_Timer process_profile;
  process_profile.start ();

  ACE_High_Res_Timer timer;
  ACE_Time_Value working_time;
  ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;
  ACE_Profile_Timer::Rusage elapsed_rusage;
  ACE_Time_Value user_time, system_time;

  // step1a set defaults
  std::string path_root = Common_File_Tools::getWorkingDirectory ();
  std::string ui_definition_file_path = path_root;
  ui_definition_file_path += ACE_DIRECTORY_SEPARATOR_CHAR_A;
  ui_definition_file_path +=
    ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_16_UI_DEFINITION_FILE);
  bool log_to_file = false;
  std::string log_file_name;
  enum Engine_ModeType mode_type_e = ENGINE_MODE_DEFAULT;
  bool trace_information = false;
  bool print_version_and_exit = false;

  // step1b: parse/process/validate configuration
  if (!do_process_arguments (argc_in,
                             argv_in,
                             ui_definition_file_path,
                             log_to_file,
                             mode_type_e,
                             trace_information,
                             print_version_and_exit))
  {
    do_print_usage (ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0])));
    goto clean;
  } // end IF
  if (print_version_and_exit)
  {
    do_printVersion (ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0])));
    result = EXIT_SUCCESS;
    goto clean;
  } // end IF

  // *TODO*: clean this up ASAP
  switch (mode_type_e)
  {
    case ENGINE_MODE_2:
    {
      ui_definition_file_path = Common_File_Tools::directory (ui_definition_file_path);
      ui_definition_file_path += ACE_DIRECTORY_SEPARATOR_CHAR_A;
      ui_definition_file_path +=
        ACE_TEXT_ALWAYS_CHAR (ENGINE_PGE_2_UI_DEFINITION_FILE);
    } // end IF
    case ENGINE_MODE_3:
    case ENGINE_MODE_4:
    {
      ui_definition_file_path = Common_File_Tools::directory (ui_definition_file_path);
      ui_definition_file_path += ACE_DIRECTORY_SEPARATOR_CHAR_A;
      ui_definition_file_path +=
        ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_3_UI_DEFINITION_FILE);
    } // end IF
    case ENGINE_MODE_16:
    {
      ui_definition_file_path = Common_File_Tools::directory (ui_definition_file_path);
      ui_definition_file_path += ACE_DIRECTORY_SEPARATOR_CHAR_A;
      ui_definition_file_path +=
        ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_16_UI_DEFINITION_FILE);
    } // end IF
    case ENGINE_MODE_27:
    {
      ui_definition_file_path = Common_File_Tools::directory (ui_definition_file_path);
      ui_definition_file_path += ACE_DIRECTORY_SEPARATOR_CHAR_A;
      ui_definition_file_path +=
        ACE_TEXT_ALWAYS_CHAR (ENGINE_PGE_27_UI_DEFINITION_FILE);
    } // end IF
    default:
      break;
  } // end SWITCH
  if (!Common_File_Tools::isReadable (ui_definition_file_path))
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("invalid argument(s), aborting\n")));
    do_print_usage (ACE::basename (argv_in[0]));
    goto clean;
  } // end IF

  // step1c: initialize logging and/or tracing
  if (log_to_file)
    log_file_name =
      Common_Log_Tools::getLogFilename (ACE_TEXT_ALWAYS_CHAR (engine_PACKAGE_NAME),
                                        ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0])));
  if (!Common_Log_Tools::initializeLogging (ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0])), // program name
                                            log_file_name,              // log file name
                                            false,                      // log to syslog ?
                                            false,                      // trace messages ?
                                            trace_information,          // debug messages ?
                                            NULL))                      // logger ?
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to Common_Log_Tools::initializeLogging(), aborting\n")));
    goto clean;
  } // end IF

  timer.start ();
  // step2: do actual work
  result_3 = do_work (argc_in,
                      argv_in,
                      ui_definition_file_path,
                      mode_type_e);
  timer.stop ();
  if (!result_3)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to do_work(), aborting\n")));
    goto clean;
  } // end IF

  // debug info
  timer.elapsed_time (working_time);
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("total working time (h:m:s.us): \"%s\"\n"),
              ACE_TEXT (Common_Timer_Tools::periodToString (working_time).c_str ())));

  // step3: shut down
  process_profile.stop ();

  elapsed_time.real_time = 0.0;
  elapsed_time.user_time = 0.0;
  elapsed_time.system_time = 0.0;
  result_2 = process_profile.elapsed_time (elapsed_time);
  if (result_2 == -1)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to ACE_Profile_Timer::elapsed_time: \"%m\", aborting\n")));
    goto clean;
  } // end IF
  ACE_OS::memset (&elapsed_rusage, 0, sizeof (elapsed_rusage));
  process_profile.elapsed_rusage (elapsed_rusage);
  user_time.set (elapsed_rusage.ru_utime);
  system_time.set (elapsed_rusage.ru_stime);
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" --> Process Profile <--\nreal time = %A seconds\nuser time = %A seconds\nsystem time = %A seconds\n --> Resource Usage <--\nuser time used: %s\nsystem time used: %s\n"),
              elapsed_time.real_time,
              elapsed_time.user_time,
              elapsed_time.system_time,
              ACE_TEXT (Common_Timer_Tools::periodToString (user_time).c_str ()),
              ACE_TEXT (Common_Timer_Tools::periodToString (system_time).c_str ())));
#else
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" --> Process Profile <--\nreal time = %A seconds\nuser time = %A seconds\nsystem time = %A seconds\n --> Resource Usage <--\nuser time used: %s\nsystem time used: %s\nmaximum resident set size = %d\nintegral shared memory size = %d\nintegral unshared data size = %d\nintegral unshared stack size = %d\npage reclaims = %d\npage faults = %d\nswaps = %d\nblock input operations = %d\nblock output operations = %d\nmessages sent = %d\nmessages received = %d\nsignals received = %d\nvoluntary context switches = %d\ninvoluntary context switches = %d\n"),
              elapsed_time.real_time,
              elapsed_time.user_time,
              elapsed_time.system_time,
              ACE_TEXT (Common_Timer_Tools::periodToString (user_time).c_str ()),
              ACE_TEXT (Common_Timer_Tools::periodToString (system_time).c_str ()),
              elapsed_rusage.ru_maxrss,
              elapsed_rusage.ru_ixrss,
              elapsed_rusage.ru_idrss,
              elapsed_rusage.ru_isrss,
              elapsed_rusage.ru_minflt,
              elapsed_rusage.ru_majflt,
              elapsed_rusage.ru_nswap,
              elapsed_rusage.ru_inblock,
              elapsed_rusage.ru_oublock,
              elapsed_rusage.ru_msgsnd,
              elapsed_rusage.ru_msgrcv,
              elapsed_rusage.ru_nsignals,
              elapsed_rusage.ru_nvcsw,
              elapsed_rusage.ru_nivcsw));
#endif // ACE_WIN32 || ACE_WIN64

  result = EXIT_SUCCESS;

clean:
  Common_Log_Tools::finalizeLogging ();

  // *PORTABILITY*: on Windows, finalize ACE
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  result = ACE::fini ();
  if (result == -1)
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to ACE::fini(): \"%m\", continuing\n")));
#endif // ACE_WIN32 || ACE_WIN64

  return result;
}
