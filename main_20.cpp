#include "stdafx.h"

#include <iostream>
#include <string>

#if defined (GLEW_SUPPORT)
#include "GL/glew.h"
#endif // GLEW_SUPPORT
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

#include "common_image_tools.h"

#include "common_log_tools.h"

#include "common_timer_tools.h"

#include "common_ui_gtk_common.h"
#include "common_ui_gtk_builder_definition.h"
#include "common_ui_gtk_manager_common.h"

#if defined (HAVE_CONFIG_H)
#include "engine_config.h"
#endif // HAVE_CONFIG_H
//#include "resource.h"

#include "defines.h"
#include "defines_20.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "glut_430.h"
#include "glut_431.h"
#include "pge_432.h"
#include "glut_433.h"
#include "glut_434.h"
#include "pge_435.h"
#include "glut_436.h"
#include "pge_437.h"
#include "pge_438.h"
#include "glut_439.h"
#include "pge_440.h"
#include "glut_441.h"
#include "glut_442.h"
#include "pge_443.h"
#include "glut_444.h"
#include "glut_445.h"
#include "glut_446.h"
#include "pge_447.h"
#include "glut_448.h"
#include "glut_449.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 430,
  ENGINE_MODE_431,
  ENGINE_MODE_432,
  ENGINE_MODE_433,
  ENGINE_MODE_434,
  ENGINE_MODE_435,
  ENGINE_MODE_436,
  ENGINE_MODE_437,
  ENGINE_MODE_438,
  ENGINE_MODE_439,
  ENGINE_MODE_440,
  ENGINE_MODE_441,
  ENGINE_MODE_442,
  ENGINE_MODE_443,
  ENGINE_MODE_444,
  ENGINE_MODE_445,
  ENGINE_MODE_446,
  ENGINE_MODE_447,
  ENGINE_MODE_448,
  ENGINE_MODE_449,
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
  ui_definition_file_path += ACE_TEXT_ALWAYS_CHAR ("");
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
  UIDefinitionFilePath_out += ACE_TEXT_ALWAYS_CHAR ("");
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
      struct Engine_OpenGL_GLUT_430_CBData cb_data_s;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 400.0f;
      cb_data_s.camera.position.z = 750.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.pmousex = ENGINE_GLUT_430_DEFAULT_WIDTH / 2;
      cb_data_s.pmousey = ENGINE_GLUT_430_DEFAULT_HEIGHT / 2;
      cb_data_s.mousex = ENGINE_GLUT_430_DEFAULT_WIDTH / 2;
      cb_data_s.mousey = ENGINE_GLUT_430_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse_0_ButtonDown = false;

      cb_data_s.amp = 0.0f;
      cb_data_s.orient = { 0.0f, 0.0f, 0.0f };
      cb_data_s.otarget = { 0.0f, 0.0f, 0.0f };
      cb_data_s.tpos = { 0.0f, 0.0f, 0.0f };
      cb_data_s.tvel = { 0.0f, 0.0f, 0.0f };
      cb_data_s.twist = { 0.0f, 0.0f, 0.0f };
      cb_data_s.ttwist = { 0.0f, 0.0f, 0.0f };
      cb_data_s.wiggle = 0.0f;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_430_DEFAULT_WIDTH, ENGINE_GLUT_430_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 430");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      //glEnable (GL_DEPTH_TEST);

      //glDisable (GL_CULL_FACE);
      //glFrontFace (GL_CCW);

      //glEnable (GL_LIGHTING);
      //glEnable (GL_LIGHT0);
      //glEnable (GL_LIGHT1);
      //glDisable (GL_COLOR_MATERIAL);

      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //glEnable (GL_NORMALIZE);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_430_draw);
      glutReshapeFunc (engine_glut_430_reshape);
      glutVisibilityFunc (engine_glut_430_visible);

      glutKeyboardFunc (engine_glut_430_key);
      glutSpecialFunc (engine_glut_430_key_special);
      glutMouseFunc (engine_glut_430_mouse_button);
      glutMotionFunc (engine_glut_430_mouse_move);
      glutPassiveMotionFunc (engine_glut_430_mouse_move);
      glutTimerFunc (100, engine_glut_430_timer, 0);

      glutCreateMenu (engine_glut_430_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      makeScales (cb_data_s);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_431:
    {
      struct Engine_OpenGL_GLUT_431_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_431_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_431_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_431_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 400.0f;
      cb_data_s.camera.position.z = 750.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.mouseX = ENGINE_GLUT_431_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_431_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_431_DEFAULT_WIDTH, ENGINE_GLUT_431_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 431");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      // initialize GLEW
      GLenum err = glewInit ();
      if (GLEW_OK != err)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glewInit(): \"%s\", aborting\n"),
                    ACE_TEXT (glewGetErrorString (err))));
        break;
      } // end IF
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("using GLEW version: %s\n"),
                  ACE_TEXT (glewGetString (GLEW_VERSION))));

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_431_draw);
      glutReshapeFunc (engine_glut_431_reshape);
      glutVisibilityFunc (engine_glut_431_visible);

      glutKeyboardFunc (engine_glut_431_key);
      glutSpecialFunc (engine_glut_431_key_special);
      glutMouseFunc (engine_glut_431_mouse_button);
      glutMotionFunc (engine_glut_431_mouse_move);
      glutPassiveMotionFunc (engine_glut_431_mouse_move);
      glutTimerFunc (100, engine_glut_431_timer, 0);

      glutCreateMenu (engine_glut_431_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_431.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_431.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_432:
    {
      PGE_432 example;
      if (example.Construct (ENGINE_PGE_432_DEFAULT_WIDTH, ENGINE_PGE_432_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_433:
    {
      struct Engine_OpenGL_GLUT_433_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_433_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_433_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_433_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 400.0f;
      cb_data_s.camera.position.z = 750.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.mouseX = ENGINE_GLUT_433_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_433_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_433_DEFAULT_WIDTH, ENGINE_GLUT_433_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 433");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      // initialize GLEW
      GLenum err = glewInit ();
      if (GLEW_OK != err)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glewInit(): \"%s\", aborting\n"),
                    ACE_TEXT (glewGetErrorString (err))));
        break;
      } // end IF
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("using GLEW version: %s\n"),
                  ACE_TEXT (glewGetString (GLEW_VERSION))));

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_433_draw);
      glutReshapeFunc (engine_glut_433_reshape);
      glutVisibilityFunc (engine_glut_433_visible);

      glutKeyboardFunc (engine_glut_433_key);
      glutSpecialFunc (engine_glut_433_key_special);
      glutMouseFunc (engine_glut_433_mouse_button);
      glutMotionFunc (engine_glut_433_mouse_move);
      glutPassiveMotionFunc (engine_glut_433_mouse_move);
      glutTimerFunc (100, engine_glut_433_timer, 0);

      glutCreateMenu (engine_glut_433_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_433.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_433.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_434:
    {
      struct Engine_OpenGL_GLUT_434_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_434_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_434_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_434_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 400.0f;
      cb_data_s.camera.position.z = 750.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.mouseX = ENGINE_GLUT_434_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_434_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_434_DEFAULT_WIDTH, ENGINE_GLUT_434_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 434");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      // initialize GLEW
      GLenum err = glewInit ();
      if (GLEW_OK != err)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glewInit(): \"%s\", aborting\n"),
                    ACE_TEXT (glewGetErrorString (err))));
        break;
      } // end IF
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("using GLEW version: %s\n"),
                  ACE_TEXT (glewGetString (GLEW_VERSION))));

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_434_draw);
      glutReshapeFunc (engine_glut_434_reshape);
      glutVisibilityFunc (engine_glut_434_visible);

      glutKeyboardFunc (engine_glut_434_key);
      glutSpecialFunc (engine_glut_434_key_special);
      glutMouseFunc (engine_glut_434_mouse_button);
      glutMotionFunc (engine_glut_434_mouse_move);
      glutPassiveMotionFunc (engine_glut_434_mouse_move);
      glutTimerFunc (100, engine_glut_434_timer, 0);

      glutCreateMenu (engine_glut_434_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_434.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_434.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_mouse"));
      ACE_ASSERT (cb_data_s.mouseLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_435:
    {
      PGE_435 example;
      if (example.Construct (ENGINE_PGE_435_DEFAULT_WIDTH, ENGINE_PGE_435_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_436:
    {
      struct Engine_OpenGL_GLUT_436_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_436_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_436_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_436_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.samplerLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.speedLoc = -1;
      cb_data_s.tilingLoc = -1;
      cb_data_s.strengthLoc = -1;

      cb_data_s.speed = ENGINE_GLUT_436_DEFAULT_SPEED;
      cb_data_s.tiling = ENGINE_GLUT_436_DEFAULT_TILING;
      cb_data_s.strength = ENGINE_GLUT_436_DEFAULT_STRENGTH;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 1.0F;
      cb_data_s.camera.up.z = 0.0F;

      cb_data_s.mouseX = ENGINE_GLUT_436_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_436_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_436_DEFAULT_WIDTH, ENGINE_GLUT_436_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 436");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      // initialize GLEW
      GLenum err = glewInit ();
      if (GLEW_OK != err)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glewInit(): \"%s\", aborting\n"),
                    ACE_TEXT (glewGetErrorString (err))));
        break;
      } // end IF
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("using GLEW version: %s\n"),
                  ACE_TEXT (glewGetString (GLEW_VERSION))));

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_436_draw);
      glutReshapeFunc (engine_glut_436_reshape);
      glutVisibilityFunc (engine_glut_436_visible);

      glutKeyboardFunc (engine_glut_436_key);
      glutSpecialFunc (engine_glut_436_key_special);
      glutMouseFunc (engine_glut_436_mouse_button);
      glutMotionFunc (engine_glut_436_mouse_move);
      glutPassiveMotionFunc (engine_glut_436_mouse_move);
      glutTimerFunc (100, engine_glut_436_timer, 0);

      glutCreateMenu (engine_glut_436_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_436.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_436.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.samplerLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uSampler"));
      ACE_ASSERT (cb_data_s.samplerLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      cb_data_s.speedLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_speed"));
      ACE_ASSERT (cb_data_s.speedLoc != -1);
      cb_data_s.tilingLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_tiling"));
      ACE_ASSERT (cb_data_s.tilingLoc != -1);
      cb_data_s.strengthLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_strength"));
      ACE_ASSERT (cb_data_s.strengthLoc != -1);

      if (!cb_data_s.texture.load (ACE_TEXT_ALWAYS_CHAR ("kitty-cat-kitten-pet.jpg"),
                                   false)) // don't flip image in this case
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_437:
    {
      PGE_437 example;
      if (example.Construct (ENGINE_PGE_437_DEFAULT_WIDTH, ENGINE_PGE_437_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_438:
    {
      PGE_438 example;
      if (example.Construct (ENGINE_PGE_438_DEFAULT_WIDTH, ENGINE_PGE_438_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_439:
    {
      struct Engine_OpenGL_GLUT_439_CBData cb_data_s;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = -200.0f;
      cb_data_s.camera.position.z = 750.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = -1.0f; // *NOTE*: <-- yep, it's 'upside down'
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.mousex = ENGINE_GLUT_439_DEFAULT_WIDTH / 2;
      cb_data_s.mousey = ENGINE_GLUT_439_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse_0_ButtonDown = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_439_DEFAULT_WIDTH, ENGINE_GLUT_439_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 439");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      //glDisable (GL_DEPTH_TEST);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_439_draw);
      glutReshapeFunc (engine_glut_439_reshape);
      glutVisibilityFunc (engine_glut_439_visible);

      glutKeyboardFunc (engine_glut_439_key);
      glutSpecialFunc (engine_glut_439_key_special);
      glutMouseFunc (engine_glut_439_mouse_button);
      glutMotionFunc (engine_glut_439_mouse_move);
      glutPassiveMotionFunc (engine_glut_439_mouse_move);
      glutTimerFunc (100, engine_glut_439_timer, 0);

      glutCreateMenu (engine_glut_439_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      cb_data_s.cf = Common_Tools::getRandomNumber (0.0f, 360.0f);
      float h, r, g, b;
      Common_GL_Color_t c;
      for (int i = 0; i < ENGINE_GLUT_439_DEFAULT_M; i++)
      {
        cb_data_s.random[i] = Common_Tools::getRandomNumber (0.0f, 1.0f);

        h = std::fmod (cb_data_s.cf + Common_Tools::getRandomNumber (-90.0f, 90.0f), 360.0f);
        Common_Image_Tools::HSVToRGB (h,
                                      Common_Tools::getRandomNumber (80.0f, 90.0f) / 100.0f,
                                      Common_Tools::getRandomNumber (80.0f, 90.0f) / 100.0f,
                                      r, g, b);
        c.r = static_cast<glm::u8> (r * 255.0f);
        c.g = static_cast<glm::u8> (g * 255.0f);
        c.b = static_cast<glm::u8> (b * 255.0f);
        cb_data_s.color[i] = c;

        cb_data_s.o[i] = Common_Tools::getRandomNumber (2, 5);
      } // end FOR

      cb_data_s.xm[0] = -ENGINE_GLUT_439_DEFAULT_WIDTH  / 2.0f + ENGINE_GLUT_439_DEFAULT_WIDTH  / 4.0f;
      cb_data_s.ym[0] = -ENGINE_GLUT_439_DEFAULT_HEIGHT / 2.0f + ENGINE_GLUT_439_DEFAULT_HEIGHT / 4.0f;
      cb_data_s.sz[0] =  ENGINE_GLUT_439_DEFAULT_WIDTH  / 2.0f;
  
      cb_data_s.xm[1] = -ENGINE_GLUT_439_DEFAULT_WIDTH  / 2.0f + ENGINE_GLUT_439_DEFAULT_WIDTH  / 2.0f + ENGINE_GLUT_439_DEFAULT_WIDTH / 4.0f;
      cb_data_s.ym[1] = -ENGINE_GLUT_439_DEFAULT_HEIGHT / 2.0f + ENGINE_GLUT_439_DEFAULT_HEIGHT / 4.0f;
      cb_data_s.sz[1] =  ENGINE_GLUT_439_DEFAULT_WIDTH  / 2.0f;

      cb_data_s.xm[2] = -ENGINE_GLUT_439_DEFAULT_WIDTH  / 2.0f + ENGINE_GLUT_439_DEFAULT_WIDTH  / 2.0f + ENGINE_GLUT_439_DEFAULT_WIDTH  / 4.0f;
      cb_data_s.ym[2] = -ENGINE_GLUT_439_DEFAULT_HEIGHT / 2.0f + ENGINE_GLUT_439_DEFAULT_HEIGHT / 2.0f + ENGINE_GLUT_439_DEFAULT_HEIGHT / 4.0f;
      cb_data_s.sz[2] =  ENGINE_GLUT_439_DEFAULT_WIDTH  / 2.0f;
  
      cb_data_s.xm[3] = -ENGINE_GLUT_439_DEFAULT_WIDTH  / 2.0f + ENGINE_GLUT_439_DEFAULT_WIDTH  / 4.0f;
      cb_data_s.ym[3] = -ENGINE_GLUT_439_DEFAULT_HEIGHT / 2.0f + ENGINE_GLUT_439_DEFAULT_HEIGHT / 2.0f + ENGINE_GLUT_439_DEFAULT_HEIGHT / 4.0f;
      cb_data_s.sz[3] =  ENGINE_GLUT_439_DEFAULT_WIDTH  / 2.0f;
 
      static std::random_device rd;
      static std::mt19937 m (rd ());
      static std::normal_distribution dist (0.0f, 1.0f);
      cb_data_s.top = 3;
      for (int k = 0; k < 175; k++)
      {
        float x = dist (m) * 0.15f * ENGINE_GLUT_439_DEFAULT_WIDTH;
        float y = dist (m) * 0.15f * ENGINE_GLUT_439_DEFAULT_HEIGHT;
    
        int h = -1;
        int i = 0;
        while ((h == -1) && (i <= cb_data_s.top))
        {
          if ((x > cb_data_s.xm[i] - cb_data_s.sz[i] / 2.0f) &&
              (x < cb_data_s.xm[i] + cb_data_s.sz[i] / 2.0f) &&
              (y > cb_data_s.ym[i] - cb_data_s.sz[i] / 2.0f) &&
              (y < cb_data_s.ym[i] + cb_data_s.sz[i] / 2.0f))
            h = i;
          i++;
        } // end WHILE
        if (h >= 0)
        {
          cb_data_s.sz[h] = cb_data_s.sz[h] / 2.0f;
     
          cb_data_s.top++;
      
          cb_data_s.xm[cb_data_s.top] = cb_data_s.xm[h] + cb_data_s.sz[h] / 2.0f;
          cb_data_s.ym[cb_data_s.top] = cb_data_s.ym[h] - cb_data_s.sz[h] / 2.0f;
          cb_data_s.sz[cb_data_s.top] = cb_data_s.sz[h];
      
          cb_data_s.top++;
      
          cb_data_s.xm[cb_data_s.top] = cb_data_s.xm[h] + cb_data_s.sz[h] / 2.0f;
          cb_data_s.ym[cb_data_s.top] = cb_data_s.ym[h] + cb_data_s.sz[h] / 2.0f;
          cb_data_s.sz[cb_data_s.top] = cb_data_s.sz[h];
     
          cb_data_s.top++;
      
          cb_data_s.xm[cb_data_s.top] = cb_data_s.xm[h] - cb_data_s.sz[h] / 2.0f;
          cb_data_s.ym[cb_data_s.top] = cb_data_s.ym[h] + cb_data_s.sz[h] / 2.0f;
          cb_data_s.sz[cb_data_s.top] = cb_data_s.sz[h];
      
          cb_data_s.xm[h] = cb_data_s.xm[h] - cb_data_s.sz[h] / 2.0f;
          cb_data_s.ym[h] = cb_data_s.ym[h] - cb_data_s.sz[h] / 2.0f;
      
          cb_data_s.top++;
        } // end IF
      } // end FOR

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_440:
    {
      // initialize GTK
      Common_UI_GTK_Configuration_t gtk_configuration;
      struct Engine_UI_GTK_440_CBData ui_cb_data;
      ui_cb_data.clearScreen = false;
      Common_UI_GtkBuilderDefinition_t gtk_ui_definition;
      Common_UI_GTK_Manager_t* gtk_manager_p =
        COMMON_UI_GTK_MANAGER_SINGLETON::instance ();
      ACE_ASSERT (gtk_manager_p);
      Common_UI_GTK_State_t& state_r =
        const_cast<Common_UI_GTK_State_t&> (gtk_manager_p->getR ());

      gtk_configuration.argc = argc_in;
      gtk_configuration.argv = argv_in;
      gtk_configuration.CBData = &ui_cb_data;
      gtk_configuration.eventHooks.finiHook = idle_finalize_UI_440_cb;
      gtk_configuration.eventHooks.initHook = idle_initialize_UI_440_cb;
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

      PGE_440 example (ui_cb_data);
      if (example.Construct (ENGINE_PGE_440_DEFAULT_WIDTH, ENGINE_PGE_440_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_441:
    {
      struct Engine_OpenGL_GLUT_441_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_441_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_441_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_441_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.frameLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 1.0F;
      cb_data_s.camera.up.z = 0.0F;

      cb_data_s.mouseX = ENGINE_GLUT_441_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_441_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_441_DEFAULT_WIDTH, ENGINE_GLUT_441_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 441");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      // initialize GLEW
      GLenum err = glewInit ();
      if (GLEW_OK != err)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glewInit(): \"%s\", aborting\n"),
                    ACE_TEXT (glewGetErrorString (err))));
        break;
      } // end IF
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("using GLEW version: %s\n"),
                  ACE_TEXT (glewGetString (GLEW_VERSION))));

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_441_draw);
      glutReshapeFunc (engine_glut_441_reshape);
      glutVisibilityFunc (engine_glut_441_visible);

      glutKeyboardFunc (engine_glut_441_key);
      glutSpecialFunc (engine_glut_441_key_special);
      glutMouseFunc (engine_glut_441_mouse_button);
      glutMotionFunc (engine_glut_441_mouse_move);
      glutPassiveMotionFunc (engine_glut_441_mouse_move);
      glutTimerFunc (100, engine_glut_441_timer, 0);

      glutCreateMenu (engine_glut_441_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_441.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_441.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      //cb_data_s.frameLoc =
      //  glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_frame"));
      //ACE_ASSERT (cb_data_s.frameLoc != -1);
      //cb_data_s.mouseLoc =
      //  glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_mouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_442:
    {
      struct Engine_OpenGL_GLUT_442_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_442_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_442_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_442_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 1.0F;
      cb_data_s.camera.up.z = 0.0F;

      cb_data_s.mouseX = ENGINE_GLUT_442_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_442_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_442_DEFAULT_WIDTH, ENGINE_GLUT_442_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 442");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      // initialize GLEW
      GLenum err = glewInit ();
      if (GLEW_OK != err)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glewInit(): \"%s\", aborting\n"),
                    ACE_TEXT (glewGetErrorString (err))));
        break;
      } // end IF
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("using GLEW version: %s\n"),
                  ACE_TEXT (glewGetString (GLEW_VERSION))));

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_442_draw);
      glutReshapeFunc (engine_glut_442_reshape);
      glutVisibilityFunc (engine_glut_442_visible);

      glutKeyboardFunc (engine_glut_442_key);
      glutSpecialFunc (engine_glut_442_key_special);
      glutMouseFunc (engine_glut_442_mouse_button);
      glutMotionFunc (engine_glut_442_mouse_move);
      glutPassiveMotionFunc (engine_glut_442_mouse_move);
      glutTimerFunc (100, engine_glut_442_timer, 0);

      glutCreateMenu (engine_glut_442_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_442.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_442.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      //cb_data_s.mouseLoc =
      //  glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_443:
    {
      PGE_443 example;
      if (example.Construct (ENGINE_PGE_443_DEFAULT_WIDTH, ENGINE_PGE_443_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_444:
    {
      struct Engine_OpenGL_GLUT_444_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_444_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_444_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_444_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 1.0F;
      cb_data_s.camera.up.z = 0.0F;

      cb_data_s.mouseX = ENGINE_GLUT_444_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_444_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_444_DEFAULT_WIDTH, ENGINE_GLUT_444_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 444");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      // initialize GLEW
      GLenum err = glewInit ();
      if (GLEW_OK != err)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glewInit(): \"%s\", aborting\n"),
                    ACE_TEXT (glewGetErrorString (err))));
        break;
      } // end IF
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("using GLEW version: %s\n"),
                  ACE_TEXT (glewGetString (GLEW_VERSION))));

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_444_draw);
      glutReshapeFunc (engine_glut_444_reshape);
      glutVisibilityFunc (engine_glut_444_visible);

      glutKeyboardFunc (engine_glut_444_key);
      glutSpecialFunc (engine_glut_444_key_special);
      glutMouseFunc (engine_glut_444_mouse_button);
      glutMotionFunc (engine_glut_444_mouse_move);
      glutPassiveMotionFunc (engine_glut_444_mouse_move);
      glutTimerFunc (100, engine_glut_444_timer, 0);

      glutCreateMenu (engine_glut_444_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_444.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_444.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      //cb_data_s.mouseLoc =
      //  glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_445:
    {
      struct Engine_OpenGL_GLUT_445_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_445_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_445_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_445_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 1.0F;
      cb_data_s.camera.up.z = 0.0F;

      cb_data_s.mouseX = ENGINE_GLUT_445_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_445_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_445_DEFAULT_WIDTH, ENGINE_GLUT_445_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 445");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      // initialize GLEW
      GLenum err = glewInit ();
      if (GLEW_OK != err)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glewInit(): \"%s\", aborting\n"),
                    ACE_TEXT (glewGetErrorString (err))));
        break;
      } // end IF
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("using GLEW version: %s\n"),
                  ACE_TEXT (glewGetString (GLEW_VERSION))));

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_445_draw);
      glutReshapeFunc (engine_glut_445_reshape);
      glutVisibilityFunc (engine_glut_445_visible);

      glutKeyboardFunc (engine_glut_445_key);
      glutSpecialFunc (engine_glut_445_key_special);
      glutMouseFunc (engine_glut_445_mouse_button);
      glutMotionFunc (engine_glut_445_mouse_move);
      glutPassiveMotionFunc (engine_glut_445_mouse_move);
      glutTimerFunc (100, engine_glut_445_timer, 0);

      glutCreateMenu (engine_glut_445_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_445.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_445.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_446:
    {
      struct Engine_OpenGL_GLUT_446_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_446_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_446_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_446_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 1.0F;
      cb_data_s.camera.up.z = 0.0F;

      cb_data_s.mouseX = ENGINE_GLUT_446_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_446_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_446_DEFAULT_WIDTH, ENGINE_GLUT_446_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 446");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      // initialize GLEW
      GLenum err = glewInit ();
      if (GLEW_OK != err)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glewInit(): \"%s\", aborting\n"),
                    ACE_TEXT (glewGetErrorString (err))));
        break;
      } // end IF
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("using GLEW version: %s\n"),
                  ACE_TEXT (glewGetString (GLEW_VERSION))));

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_446_draw);
      glutReshapeFunc (engine_glut_446_reshape);
      glutVisibilityFunc (engine_glut_446_visible);

      glutKeyboardFunc (engine_glut_446_key);
      glutSpecialFunc (engine_glut_446_key_special);
      glutMouseFunc (engine_glut_446_mouse_button);
      glutMotionFunc (engine_glut_446_mouse_move);
      glutPassiveMotionFunc (engine_glut_446_mouse_move);
      glutTimerFunc (100, engine_glut_446_timer, 0);

      glutCreateMenu (engine_glut_446_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_446.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_446.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_447:
    {
      PGE_447 example;
      if (example.Construct (ENGINE_PGE_447_DEFAULT_WIDTH, ENGINE_PGE_447_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_448:
    {
      struct Engine_OpenGL_GLUT_448_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_448_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_448_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_448_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 1.0F;
      cb_data_s.camera.up.z = 0.0F;

      cb_data_s.mouseX = ENGINE_GLUT_448_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_448_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_448_DEFAULT_WIDTH, ENGINE_GLUT_448_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 448");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      // initialize GLEW
      GLenum err = glewInit ();
      if (GLEW_OK != err)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glewInit(): \"%s\", aborting\n"),
                    ACE_TEXT (glewGetErrorString (err))));
        break;
      } // end IF
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("using GLEW version: %s\n"),
                  ACE_TEXT (glewGetString (GLEW_VERSION))));

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_448_draw);
      glutReshapeFunc (engine_glut_448_reshape);
      glutVisibilityFunc (engine_glut_448_visible);

      glutKeyboardFunc (engine_glut_448_key);
      glutSpecialFunc (engine_glut_448_key_special);
      glutMouseFunc (engine_glut_448_mouse_button);
      glutMotionFunc (engine_glut_448_mouse_move);
      glutPassiveMotionFunc (engine_glut_448_mouse_move);
      glutTimerFunc (100, engine_glut_448_timer, 0);

      glutCreateMenu (engine_glut_448_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_448.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_448.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      //cb_data_s.mouseLoc =
      //  glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_mouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_449:
    {
      struct Engine_OpenGL_GLUT_449_CBData cb_data_s;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 2000.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.mousex = ENGINE_GLUT_449_DEFAULT_WIDTH / 2;
      cb_data_s.mousey = ENGINE_GLUT_449_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse_0_ButtonDown = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | /*GLUT_DOUBLE |*/ GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_449_DEFAULT_WIDTH, ENGINE_GLUT_449_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 449");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      //glDisable (GL_DEPTH_TEST);

      glClearColor (0.0f, 1.0f, 1.0f, 1.0f); // 'aqua'
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_449_draw);
      glutReshapeFunc (engine_glut_449_reshape);
      glutVisibilityFunc (engine_glut_449_visible);

      glutKeyboardFunc (engine_glut_449_key);
      glutSpecialFunc (engine_glut_449_key_special);
      glutMouseFunc (engine_glut_449_mouse_button);
      glutMotionFunc (engine_glut_449_mouse_move);
      glutPassiveMotionFunc (engine_glut_449_mouse_move);
      glutTimerFunc (100, engine_glut_449_timer, 0);

      glutCreateMenu (engine_glut_449_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      cb_data_s.gradient.push_back ({ 0.0f  , 0.0f   , 0.5176f, 0.2078f, 1.0f });
      cb_data_s.gradient.push_back ({ 0.125f, 0.2f   , 0.8f   , 0.0f   , 1.0f });
      cb_data_s.gradient.push_back ({ 0.25f , 0.9569f, 0.9412f, 0.4431f, 1.0f });
      cb_data_s.gradient.push_back ({ 0.5f  , 0.9569f, 0.7412f, 0.2706f, 1.0f });
      cb_data_s.gradient.push_back ({ 0.75f , 0.6f   , 0.3922f, 0.1686f, 1.0f });
      cb_data_s.gradient.push_back ({ 1.0f  , 1.0f   , 1.0f   , 1.0f   , 1.0f });

      glutMainLoop ();

      result = true;

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

COMMON_DEFINE_PRINTVERSION_FUNCTION (do_print_version, MAKE_VERSION_STRING_VARIABLE (programName_in, ACE_TEXT_ALWAYS_CHAR (engine_PACKAGE_VERSION_FULL), version_string), version_string)

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
  ui_definition_file_path += ACE_TEXT_ALWAYS_CHAR ("");
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
    do_print_version (ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0])));
    result = EXIT_SUCCESS;
    goto clean;
  } // end IF

  // *TODO*: clean this up ASAP
  switch (mode_type_e)
  {
    case ENGINE_MODE_440:
    {
      ui_definition_file_path = Common_File_Tools::directory (ui_definition_file_path);
      ui_definition_file_path += ACE_DIRECTORY_SEPARATOR_CHAR_A;
      ui_definition_file_path +=
        ACE_TEXT_ALWAYS_CHAR (ENGINE_PGE_440_GTK_UI_DEFINITION_FILE);
      break;
    }
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
  if (!Common_Log_Tools::initialize (ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0])), // program name
                                     log_file_name,              // log file name
                                     false,                      // log to syslog ?
                                     false,                      // trace messages ?
                                     trace_information,          // debug messages ?
                                     NULL))                      // logger ?
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to Common_Log_Tools::initialize(), aborting\n")));
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
  Common_Log_Tools::finalize ();

  // *PORTABILITY*: on Windows, finalize ACE
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  result = ACE::fini ();
  if (result == -1)
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to ACE::fini(): \"%m\", continuing\n")));
#endif // ACE_WIN32 || ACE_WIN64

  return result;
}
