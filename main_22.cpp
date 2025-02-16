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
#include "defines_22.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "glut_470.h"
#include "glut_471.h"
#include "glut_472.h"
#include "glut_473.h"
#include "glut_474.h"
#include "glut_475.h"
#include "glut_476.h"
#include "glut_477.h"
#include "glut_478.h"
#include "glut_479.h"
#include "glut_480.h"
#include "glut_481.h"
#include "glut_482.h"
#include "glut_483.h"
#include "glut_484.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 470,
  ENGINE_MODE_471,
  ENGINE_MODE_472,
  ENGINE_MODE_473,
  ENGINE_MODE_474,
  ENGINE_MODE_475,
  ENGINE_MODE_476,
  ENGINE_MODE_477,
  ENGINE_MODE_478,
  ENGINE_MODE_479,
  ENGINE_MODE_480,
  ENGINE_MODE_481,
  ENGINE_MODE_482,
  ENGINE_MODE_483,
  ENGINE_MODE_484,
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
      struct Engine_OpenGL_GLUT_470_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_470_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_470_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_470_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_470_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_470_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_470_DEFAULT_WIDTH, ENGINE_GLUT_470_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 470");
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

      glutDisplayFunc (engine_glut_470_draw);
      glutReshapeFunc (engine_glut_470_reshape);
      glutVisibilityFunc (engine_glut_470_visible);

      glutKeyboardFunc (engine_glut_470_key);
      glutSpecialFunc (engine_glut_470_key_special);
      glutMouseFunc (engine_glut_470_mouse_button);
      glutMotionFunc (engine_glut_470_mouse_move);
      glutPassiveMotionFunc (engine_glut_470_mouse_move);
      glutTimerFunc (100, engine_glut_470_timer, 0);

      glutCreateMenu (engine_glut_470_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_470.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_470.frag")))
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

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_471:
    {
      struct Engine_OpenGL_GLUT_471_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_471_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_471_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_471_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_471_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_471_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_471_DEFAULT_WIDTH, ENGINE_GLUT_471_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 471");
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

      glutDisplayFunc (engine_glut_471_draw);
      glutReshapeFunc (engine_glut_471_reshape);
      glutVisibilityFunc (engine_glut_471_visible);

      glutKeyboardFunc (engine_glut_471_key);
      glutSpecialFunc (engine_glut_471_key_special);
      glutMouseFunc (engine_glut_471_mouse_button);
      glutMotionFunc (engine_glut_471_mouse_move);
      glutPassiveMotionFunc (engine_glut_471_mouse_move);
      //glutTimerFunc (100, engine_glut_471_timer, 0);

      glutCreateMenu (engine_glut_471_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_471.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_471.frag")))
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

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_472:
    {
      struct Engine_OpenGL_GLUT_472_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_472_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_472_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_472_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_472_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_472_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_472_DEFAULT_WIDTH, ENGINE_GLUT_472_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 472");
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

      glutDisplayFunc (engine_glut_472_draw);
      glutReshapeFunc (engine_glut_472_reshape);
      glutVisibilityFunc (engine_glut_472_visible);

      glutKeyboardFunc (engine_glut_472_key);
      glutSpecialFunc (engine_glut_472_key_special);
      glutMouseFunc (engine_glut_472_mouse_button);
      glutMotionFunc (engine_glut_472_mouse_move);
      glutPassiveMotionFunc (engine_glut_472_mouse_move);
      //glutTimerFunc (100, engine_glut_472_timer, 0);

      glutCreateMenu (engine_glut_472_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_472.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_472.frag")))
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

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_473:
    {
      struct Engine_OpenGL_GLUT_473_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_473_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_473_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_473_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_473_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_473_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_473_DEFAULT_WIDTH, ENGINE_GLUT_473_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 473");
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

      glutDisplayFunc (engine_glut_473_draw);
      glutReshapeFunc (engine_glut_473_reshape);
      glutVisibilityFunc (engine_glut_473_visible);

      glutKeyboardFunc (engine_glut_473_key);
      glutSpecialFunc (engine_glut_473_key_special);
      glutMouseFunc (engine_glut_473_mouse_button);
      glutMotionFunc (engine_glut_473_mouse_move);
      glutPassiveMotionFunc (engine_glut_473_mouse_move);
      glutTimerFunc (100, engine_glut_473_timer, 0);

      glutCreateMenu (engine_glut_473_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_473.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_473.frag")))
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

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_474:
    {
      struct Engine_OpenGL_GLUT_474_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_474_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_474_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_474_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.frameLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_474_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_474_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_474_DEFAULT_WIDTH, ENGINE_GLUT_474_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 474");
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

      glutDisplayFunc (engine_glut_474_draw);
      glutReshapeFunc (engine_glut_474_reshape);
      glutVisibilityFunc (engine_glut_474_visible);

      glutKeyboardFunc (engine_glut_474_key);
      glutSpecialFunc (engine_glut_474_key_special);
      glutMouseFunc (engine_glut_474_mouse_button);
      glutMotionFunc (engine_glut_474_mouse_move);
      glutPassiveMotionFunc (engine_glut_474_mouse_move);
      glutTimerFunc (100, engine_glut_474_timer, 0);

      glutCreateMenu (engine_glut_474_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_474.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_474.frag")))
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
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.mouseLoc != -1);
      cb_data_s.frameLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.frameLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_475:
    {
      struct Engine_OpenGL_GLUT_475_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_475_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_475_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_475_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_475_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_475_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_475_DEFAULT_WIDTH, ENGINE_GLUT_475_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 475");
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

      glutDisplayFunc (engine_glut_475_draw);
      glutReshapeFunc (engine_glut_475_reshape);
      glutVisibilityFunc (engine_glut_475_visible);

      glutKeyboardFunc (engine_glut_475_key);
      glutSpecialFunc (engine_glut_475_key_special);
      glutMouseFunc (engine_glut_475_mouse_button);
      glutMotionFunc (engine_glut_475_mouse_move);
      glutPassiveMotionFunc (engine_glut_475_mouse_move);
      glutTimerFunc (100, engine_glut_475_timer, 0);

      glutCreateMenu (engine_glut_475_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_475.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_475.frag")))
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

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_476:
    {
      struct Engine_OpenGL_GLUT_476_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_476_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_476_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_476_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.channel0Loc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_476_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_476_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_476_DEFAULT_WIDTH, ENGINE_GLUT_476_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 476");
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

      glutDisplayFunc (engine_glut_476_draw);
      glutReshapeFunc (engine_glut_476_reshape);
      glutVisibilityFunc (engine_glut_476_visible);

      glutKeyboardFunc (engine_glut_476_key);
      glutSpecialFunc (engine_glut_476_key_special);
      glutMouseFunc (engine_glut_476_mouse_button);
      glutMotionFunc (engine_glut_476_mouse_move);
      glutPassiveMotionFunc (engine_glut_476_mouse_move);
      glutTimerFunc (100, engine_glut_476_timer, 0);

      glutCreateMenu (engine_glut_476_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_476.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_476.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_476_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);
      cb_data_s.texture0.unbind ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.mouseLoc != -1);
      cb_data_s.channel0Loc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.channel0Loc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      cb_data_s.texture0.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_477:
    {
      struct Engine_OpenGL_GLUT_477_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_477_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_477_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_477_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_477_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_477_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_477_DEFAULT_WIDTH, ENGINE_GLUT_477_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 477");
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

      glutDisplayFunc (engine_glut_477_draw);
      glutReshapeFunc (engine_glut_477_reshape);
      glutVisibilityFunc (engine_glut_477_visible);

      glutKeyboardFunc (engine_glut_477_key);
      glutSpecialFunc (engine_glut_477_key_special);
      glutMouseFunc (engine_glut_477_mouse_button);
      glutMotionFunc (engine_glut_477_mouse_move);
      glutPassiveMotionFunc (engine_glut_477_mouse_move);
      glutTimerFunc (100, engine_glut_477_timer, 0);

      glutCreateMenu (engine_glut_477_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_477.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_477.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_477_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);
      cb_data_s.texture0.unbind ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      cb_data_s.channel0Loc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.channel0Loc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      cb_data_s.texture0.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_478:
    {
      struct Engine_OpenGL_GLUT_478_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_478_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_478_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_478_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1timeLoc = -1;
      cb_data_s.S1channel0Loc = -1;
      cb_data_s.S1channel1Loc = -1;
      cb_data_s.S1channel2Loc = -1;
      cb_data_s.S2resolutionLoc = -1;
      cb_data_s.S2channel0Loc = -1;

      cb_data_s.FBO = 0;
      cb_data_s.VAO = 0;
      cb_data_s.VAO_2 = 0;
      cb_data_s.VBO = 0;
      cb_data_s.VBO_2 = 0;

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

      cb_data_s.mouseX = ENGINE_GLUT_478_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_478_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_478_DEFAULT_WIDTH, ENGINE_GLUT_478_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 478");
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

      glutDisplayFunc (engine_glut_478_draw);
      glutReshapeFunc (engine_glut_478_reshape);
      glutVisibilityFunc (engine_glut_478_visible);

      glutKeyboardFunc (engine_glut_478_key);
      glutSpecialFunc (engine_glut_478_key_special);
      glutMouseFunc (engine_glut_478_mouse_button);
      glutMotionFunc (engine_glut_478_mouse_move);
      glutPassiveMotionFunc (engine_glut_478_mouse_move);
      glutTimerFunc (100, engine_glut_478_timer, 0);

      glutCreateMenu (engine_glut_478_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_478.vert_2"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_478.frag_2")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_478.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_478.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_478_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);
      cb_data_s.texture0.unbind ();

      glActiveTexture (GL_TEXTURE1);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture1.load (ACE_TEXT_ALWAYS_CHAR ("glut_478_channel1.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture1.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);
      cb_data_s.texture1.unbind ();

      glActiveTexture (GL_TEXTURE2);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture2.load (ACE_TEXT_ALWAYS_CHAR ("glut_478_channel2.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture2.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);
      cb_data_s.texture2.unbind ();

      glActiveTexture (GL_TEXTURE3);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glGenTextures (1, &cb_data_s.textureS1.id_);
      ACE_ASSERT (cb_data_s.textureS1.id_);
      cb_data_s.textureS1.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_478_DEFAULT_WIDTH, ENGINE_GLUT_478_DEFAULT_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      //cb_data_s.textureS1.load ();
      //cb_data_s.textureS1.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glGenerateMipmap (GL_TEXTURE_2D);
      cb_data_s.textureS1.unbind ();

      cb_data_s.shader1.use ();
      cb_data_s.S1resolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S1resolutionLoc != -1);
      cb_data_s.S1timeLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S1timeLoc != -1);
      cb_data_s.S1channel0Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S1channel0Loc != -1);
      cb_data_s.S1channel1Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S1channel1Loc != -1);
      cb_data_s.S1channel2Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel2"));
      ACE_ASSERT (cb_data_s.S1channel2Loc != -1);

      cb_data_s.shader2.use ();
      cb_data_s.S2resolutionLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S2resolutionLoc != -1);
      cb_data_s.S2channel0Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S2channel0Loc != -1);

      glGenFramebuffersEXT(1, &cb_data_s.FBO);
      ACE_ASSERT (cb_data_s.FBO);
      cb_data_s.textureS1.bind ();
      // draw render pass 1 to framebuffer object (--> textureS1)
      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, cb_data_s.FBO);
      glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, cb_data_s.textureS1.id_, 0);
      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);
      cb_data_s.textureS1.unbind ();

      glGenVertexArrays (1, &cb_data_s.VAO);
      ACE_ASSERT (cb_data_s.VAO);
      glGenBuffers (1, &cb_data_s.VBO);
      ACE_ASSERT (cb_data_s.VBO);

      glBindVertexArray (cb_data_s.VAO);
      GLfloat vertices_a[] =
      {
        // Rectangle vertices (Texture Coordinates: 0,0 = bottom left... 1,1 = top right)
        //-------------------------
        -1.0f ,  1.0f, 0.0f,  0.0f, 1.0f, // left top ......... 0
        -1.0f , -1.0f, 0.0f,  0.0f, 0.0f, // left bottom .......1
         1.0f ,  1.0f, 0.0f,  1.0f, 1.0f, // right top .........2
 
         1.0f ,  1.0f, 0.0f,  1.0f, 1.0f, // right top .........3
        -1.0f , -1.0f, 0.0f,  0.0f, 0.0f, // left bottom .......4
         1.0f , -1.0f, 0.0f,  1.0f, 0.0f, // right bottom ......5
      };
      glBindBuffer (GL_ARRAY_BUFFER, cb_data_s.VBO);
      glBufferData (GL_ARRAY_BUFFER, sizeof (vertices_a), vertices_a, GL_STATIC_DRAW);

      // position attribute
      glEnableVertexAttribArray (0);
      glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)0);
      //// color attribute
      //glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(3 * sizeof (float)));
      //glEnableVertexAttribArray (2);
      // texture coord attribute
      glEnableVertexAttribArray (1);
      glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(3 * sizeof (float)));

      glBindBuffer (GL_ARRAY_BUFFER, 0);
      glBindVertexArray (0);

      glGenVertexArrays (1, &cb_data_s.VAO_2);
      ACE_ASSERT (cb_data_s.VAO_2);
      glGenBuffers (1, &cb_data_s.VBO_2);
      ACE_ASSERT (cb_data_s.VBO_2);

      glBindVertexArray (cb_data_s.VAO_2);
      GLfloat vertices_2[] =
      {
        // Rectangle vertices (Texture Coordinates: 0,0 = bottom left... 1,1 = top right)
        //-------------------------
        -1.0f ,  1.0f, 0.0f,  0.0f, 1.0f, // left top ......... 0
        -1.0f , -1.0f, 0.0f,  0.0f, 0.0f, // left bottom .......1
         1.0f ,  1.0f, 0.0f,  1.0f, 1.0f, // right top .........2
 
         1.0f ,  1.0f, 0.0f,  1.0f, 1.0f, // right top .........3
        -1.0f , -1.0f, 0.0f,  0.0f, 0.0f, // left bottom .......4
         1.0f , -1.0f, 0.0f,  1.0f, 0.0f, // right bottom ......5
      };
      glBindBuffer (GL_ARRAY_BUFFER, cb_data_s.VBO_2);
      glBufferData (GL_ARRAY_BUFFER, sizeof (vertices_2), vertices_2, GL_STATIC_DRAW);

      // position attribute
      glEnableVertexAttribArray (0);
      glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)0);
      //// color attribute
      //glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(3 * sizeof (float)));
      //glEnableVertexAttribArray (2);
      // texture coord attribute
      glEnableVertexAttribArray (1);
      glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(3 * sizeof (float)));

      glBindBuffer (GL_ARRAY_BUFFER, 0);
      glBindVertexArray (0);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      glDeleteBuffers (1, &cb_data_s.VBO);
      glDeleteVertexArrays (1, &cb_data_s.VAO);
      glDeleteBuffers (1, &cb_data_s.VBO_2);
      glDeleteVertexArrays (1, &cb_data_s.VAO_2);
      glDeleteFramebuffersEXT (1, &cb_data_s.FBO);

      cb_data_s.texture0.reset ();
      cb_data_s.texture1.reset ();
      cb_data_s.texture2.reset ();
      cb_data_s.textureS1.reset ();

      cb_data_s.shader1.reset ();
      cb_data_s.shader2.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_479:
    {
      struct Engine_OpenGL_GLUT_479_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_479_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_479_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_479_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.frameLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_479_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_479_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_479_DEFAULT_WIDTH, ENGINE_GLUT_479_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 479");
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

      glutDisplayFunc (engine_glut_479_draw);
      glutReshapeFunc (engine_glut_479_reshape);
      glutVisibilityFunc (engine_glut_479_visible);

      glutKeyboardFunc (engine_glut_479_key);
      glutSpecialFunc (engine_glut_479_key_special);
      glutMouseFunc (engine_glut_479_mouse_button);
      glutMotionFunc (engine_glut_479_mouse_move);
      glutPassiveMotionFunc (engine_glut_479_mouse_move);
      glutTimerFunc (100, engine_glut_479_timer, 0);

      glutCreateMenu (engine_glut_479_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_479.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_479.frag")))
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
      cb_data_s.frameLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.frameLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_480:
    {
      struct Engine_OpenGL_GLUT_480_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_480_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_480_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_480_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_480_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_480_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_480_DEFAULT_WIDTH, ENGINE_GLUT_480_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 480");
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

      glutDisplayFunc (engine_glut_480_draw);
      glutReshapeFunc (engine_glut_480_reshape);
      glutVisibilityFunc (engine_glut_480_visible);

      glutKeyboardFunc (engine_glut_480_key);
      glutSpecialFunc (engine_glut_480_key_special);
      glutMouseFunc (engine_glut_480_mouse_button);
      glutMotionFunc (engine_glut_480_mouse_move);
      glutPassiveMotionFunc (engine_glut_480_mouse_move);
      glutTimerFunc (100, engine_glut_480_timer, 0);

      glutCreateMenu (engine_glut_480_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_480.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_480.frag")))
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

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_481:
    {
      struct Engine_OpenGL_GLUT_481_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_481_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_481_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_481_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1timeLoc = -1;
      cb_data_s.S1mouseLoc = -1;
      cb_data_s.S1channel0Loc = -1;
      cb_data_s.S1channel1Loc = -1;
      cb_data_s.S2resolutionLoc = -1;
      cb_data_s.S2channel0Loc = -1;

      cb_data_s.FBO = 0;
      cb_data_s.VAO = 0;
      cb_data_s.VAO_2 = 0;
      cb_data_s.VBO = 0;
      cb_data_s.VBO_2 = 0;

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

      cb_data_s.mouseX = ENGINE_GLUT_481_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_481_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_481_DEFAULT_WIDTH, ENGINE_GLUT_481_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 481");
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

      glutDisplayFunc (engine_glut_481_draw);
      glutReshapeFunc (engine_glut_481_reshape);
      glutVisibilityFunc (engine_glut_481_visible);

      glutKeyboardFunc (engine_glut_481_key);
      glutSpecialFunc (engine_glut_481_key_special);
      glutMouseFunc (engine_glut_481_mouse_button);
      glutMotionFunc (engine_glut_481_mouse_move);
      glutPassiveMotionFunc (engine_glut_481_mouse_move);
      glutTimerFunc (100, engine_glut_481_timer, 0);

      glutCreateMenu (engine_glut_481_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_481.vert_2"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_481.frag_2")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_481.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_481.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_481_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);
      cb_data_s.texture0.unbind ();

      glActiveTexture (GL_TEXTURE1);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glGenTextures (1, &cb_data_s.textureS1.id_);
      ACE_ASSERT (cb_data_s.textureS1.id_);
      cb_data_s.textureS1.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_481_DEFAULT_WIDTH, ENGINE_GLUT_481_DEFAULT_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      //cb_data_s.textureS1.load ();
      //cb_data_s.textureS1.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glGenerateMipmap (GL_TEXTURE_2D);
      cb_data_s.textureS1.unbind ();

      cb_data_s.shader1.use ();
      cb_data_s.S1resolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S1resolutionLoc != -1);
      cb_data_s.S1timeLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S1timeLoc != -1);
      cb_data_s.S1mouseLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.S1mouseLoc != -1);
      cb_data_s.S1channel0Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S1channel0Loc != -1);
      cb_data_s.S1channel1Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S1channel1Loc != -1);

      cb_data_s.shader2.use ();
      cb_data_s.S2resolutionLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S2resolutionLoc != -1);
      cb_data_s.S2channel0Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S2channel0Loc != -1);

      glGenFramebuffersEXT(1, &cb_data_s.FBO);
      ACE_ASSERT (cb_data_s.FBO);
      cb_data_s.textureS1.bind ();
      // draw render pass 1 to framebuffer object (--> textureS1)
      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, cb_data_s.FBO);
      glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, cb_data_s.textureS1.id_, 0);
      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);
      cb_data_s.textureS1.unbind ();

      glGenVertexArrays (1, &cb_data_s.VAO);
      ACE_ASSERT (cb_data_s.VAO);
      glGenBuffers (1, &cb_data_s.VBO);
      ACE_ASSERT (cb_data_s.VBO);

      glBindVertexArray (cb_data_s.VAO);
      GLfloat vertices_a[] =
      {
        // Rectangle vertices (Texture Coordinates: 0,0 = bottom left... 1,1 = top right)
        //-------------------------
        -1.0f ,  1.0f, 0.0f,  0.0f, 1.0f, // left top ......... 0
        -1.0f , -1.0f, 0.0f,  0.0f, 0.0f, // left bottom .......1
         1.0f ,  1.0f, 0.0f,  1.0f, 1.0f, // right top .........2
 
         1.0f ,  1.0f, 0.0f,  1.0f, 1.0f, // right top .........3
        -1.0f , -1.0f, 0.0f,  0.0f, 0.0f, // left bottom .......4
         1.0f , -1.0f, 0.0f,  1.0f, 0.0f, // right bottom ......5
      };
      glBindBuffer (GL_ARRAY_BUFFER, cb_data_s.VBO);
      glBufferData (GL_ARRAY_BUFFER, sizeof (vertices_a), vertices_a, GL_STATIC_DRAW);

      // position attribute
      glEnableVertexAttribArray (0);
      glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)0);
      //// color attribute
      //glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(3 * sizeof (float)));
      //glEnableVertexAttribArray (2);
      // texture coord attribute
      glEnableVertexAttribArray (1);
      glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(3 * sizeof (float)));

      glBindBuffer (GL_ARRAY_BUFFER, 0);
      glBindVertexArray (0);

      glGenVertexArrays (1, &cb_data_s.VAO_2);
      ACE_ASSERT (cb_data_s.VAO_2);
      glGenBuffers (1, &cb_data_s.VBO_2);
      ACE_ASSERT (cb_data_s.VBO_2);

      glBindVertexArray (cb_data_s.VAO_2);
      GLfloat vertices_2[] =
      {
        // Rectangle vertices (Texture Coordinates: 0,0 = bottom left... 1,1 = top right)
        //-------------------------
        -1.0f ,  1.0f, 0.0f,  0.0f, 1.0f, // left top ......... 0
        -1.0f , -1.0f, 0.0f,  0.0f, 0.0f, // left bottom .......1
         1.0f ,  1.0f, 0.0f,  1.0f, 1.0f, // right top .........2
 
         1.0f ,  1.0f, 0.0f,  1.0f, 1.0f, // right top .........3
        -1.0f , -1.0f, 0.0f,  0.0f, 0.0f, // left bottom .......4
         1.0f , -1.0f, 0.0f,  1.0f, 0.0f, // right bottom ......5
      };
      glBindBuffer (GL_ARRAY_BUFFER, cb_data_s.VBO_2);
      glBufferData (GL_ARRAY_BUFFER, sizeof (vertices_2), vertices_2, GL_STATIC_DRAW);

      // position attribute
      glEnableVertexAttribArray (0);
      glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)0);
      //// color attribute
      //glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(3 * sizeof (float)));
      //glEnableVertexAttribArray (2);
      // texture coord attribute
      glEnableVertexAttribArray (1);
      glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(3 * sizeof (float)));

      glBindBuffer (GL_ARRAY_BUFFER, 0);
      glBindVertexArray (0);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      glDeleteBuffers (1, &cb_data_s.VBO);
      glDeleteVertexArrays (1, &cb_data_s.VAO);
      glDeleteBuffers (1, &cb_data_s.VBO_2);
      glDeleteVertexArrays (1, &cb_data_s.VAO_2);
      glDeleteFramebuffersEXT (1, &cb_data_s.FBO);

      cb_data_s.texture0.reset ();
      cb_data_s.textureS1.reset ();

      cb_data_s.shader1.reset ();
      cb_data_s.shader2.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_482:
    {
      struct Engine_OpenGL_GLUT_482_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_482_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_482_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_482_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_482_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_482_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_482_DEFAULT_WIDTH, ENGINE_GLUT_482_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 482");
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

      glutDisplayFunc (engine_glut_482_draw);
      glutReshapeFunc (engine_glut_482_reshape);
      glutVisibilityFunc (engine_glut_482_visible);

      glutKeyboardFunc (engine_glut_482_key);
      glutSpecialFunc (engine_glut_482_key_special);
      glutMouseFunc (engine_glut_482_mouse_button);
      glutMotionFunc (engine_glut_482_mouse_move);
      glutPassiveMotionFunc (engine_glut_482_mouse_move);
      glutTimerFunc (100, engine_glut_482_timer, 0);

      glutCreateMenu (engine_glut_482_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_482.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_482.frag")))
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

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_483:
    {
      struct Engine_OpenGL_GLUT_483_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_483_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_483_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_483_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_483_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_483_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_483_DEFAULT_WIDTH, ENGINE_GLUT_483_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 483");
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

      glutDisplayFunc (engine_glut_483_draw);
      glutReshapeFunc (engine_glut_483_reshape);
      glutVisibilityFunc (engine_glut_483_visible);

      glutKeyboardFunc (engine_glut_483_key);
      glutSpecialFunc (engine_glut_483_key_special);
      glutMouseFunc (engine_glut_483_mouse_button);
      glutMotionFunc (engine_glut_483_mouse_move);
      glutPassiveMotionFunc (engine_glut_483_mouse_move);
      glutTimerFunc (100, engine_glut_483_timer, 0);

      glutCreateMenu (engine_glut_483_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_483.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_483.frag")))
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

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_484:
    {
      struct Engine_OpenGL_GLUT_484_CBData cb_data_s;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1frameLoc = -1;
      cb_data_s.S1channel0Loc = -1;
      cb_data_s.S2resolutionLoc = -1;
      cb_data_s.S2channel0Loc = -1;
      cb_data_s.S3resolutionLoc = -1;
      cb_data_s.S3timeLoc = -1;
      cb_data_s.S3frameLoc = -1;
      cb_data_s.S3mouseLoc = -1;
      cb_data_s.S3keysLoc = -1;
      cb_data_s.S3channel0Loc = -1;
      cb_data_s.S3channel1Loc = -1;
      cb_data_s.S4resolutionLoc = -1;
      cb_data_s.S4mouseLoc = -1;
      cb_data_s.S4channel0Loc = -1;
      cb_data_s.S4channel1Loc = -1;
      cb_data_s.S4channel2Loc = -1;
      cb_data_s.S4channel3Loc = -1;

      cb_data_s.FBO1 = 0;
      cb_data_s.FBO2 = 0;
      cb_data_s.FBO3 = 0;
      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_484_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_484_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;
      cb_data_s.spacePressed = false;
      cb_data_s.upPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      //glutInitContextVersion (3, 3);
      //glutInitContextProfile (GLUT_CORE_PROFILE);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_484_DEFAULT_WIDTH, ENGINE_GLUT_484_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 484");
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

      glEnable (GL_TEXTURE_2D);
      glDisable (GL_DEPTH_TEST);
      glDisable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_DST_ALPHA);
      glEnable (GL_CULL_FACE);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_484_draw);
      glutReshapeFunc (engine_glut_484_reshape);
      glutVisibilityFunc (engine_glut_484_visible);

      glutKeyboardFunc (engine_glut_484_key);
      glutKeyboardUpFunc (engine_glut_484_key_up);
      glutSpecialFunc (engine_glut_484_key_special);
      glutSpecialUpFunc (engine_glut_484_key_special_up);
      glutMouseFunc (engine_glut_484_mouse_button);
      glutMotionFunc (engine_glut_484_mouse_move);
      glutPassiveMotionFunc (engine_glut_484_mouse_move);
      glutTimerFunc (100, engine_glut_484_timer, 0);

      glutCreateMenu (engine_glut_484_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_484.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_484.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_484.vert_2"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_484.frag_2")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader3.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_484.vert_3"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_484.frag_3")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader4.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_484.vert_4"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_484.frag_4")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF

      glClampColor (GL_CLAMP_READ_COLOR, GL_FALSE);
      glClampColor (GL_CLAMP_VERTEX_COLOR, GL_FALSE);
      glClampColor (GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei (GL_PACK_ALIGNMENT, 1);

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      std::string neg_z_path_string = ACE_TEXT_ALWAYS_CHAR ("glut_484_negz.jpg");
      std::string pos_z_path_string = ACE_TEXT_ALWAYS_CHAR ("glut_484_posz.jpg");
      std::string pos_y_path_string = ACE_TEXT_ALWAYS_CHAR ("glut_484_posy.jpg");
      std::string neg_y_path_string = ACE_TEXT_ALWAYS_CHAR ("glut_484_negy.jpg");
      std::string neg_x_path_string = ACE_TEXT_ALWAYS_CHAR ("glut_484_negx.jpg");
      std::string pos_x_path_string = ACE_TEXT_ALWAYS_CHAR ("glut_484_posx.jpg");
      if (!cb_data_s.texture0.load (neg_z_path_string,
                                    pos_z_path_string,
                                    pos_y_path_string,
                                    neg_y_path_string,
                                    neg_x_path_string,
                                    pos_x_path_string))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      glBindTexture (GL_TEXTURE_CUBE_MAP, cb_data_s.texture0.id_);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glGenerateMipmap (GL_TEXTURE_CUBE_MAP);
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      //glBindTexture (GL_TEXTURE_CUBE_MAP, 0);

      glActiveTexture (GL_TEXTURE1);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS1.id_);
      ACE_ASSERT (cb_data_s.textureS1.id_);
      cb_data_s.textureS1.bind ();
      std::vector<GLfloat> initial_values_a (ENGINE_GLUT_484_DEFAULT_WIDTH * ENGINE_GLUT_484_DEFAULT_HEIGHT * 4, 0.0f);
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_484_DEFAULT_WIDTH, ENGINE_GLUT_484_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glGenerateMipmap (GL_TEXTURE_2D);
      //cb_data_s.textureS1.unbind ();

      glActiveTexture (GL_TEXTURE2);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS2.id_);
      ACE_ASSERT (cb_data_s.textureS2.id_);
      cb_data_s.textureS2.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader2
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_484_DEFAULT_WIDTH, ENGINE_GLUT_484_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glGenerateMipmap (GL_TEXTURE_2D);
      //cb_data_s.textureS2.unbind ();

      glActiveTexture (GL_TEXTURE3);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS3.id_);
      ACE_ASSERT (cb_data_s.textureS3.id_);
      cb_data_s.textureS3.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader3
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_484_DEFAULT_WIDTH, ENGINE_GLUT_484_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glGenerateMipmap (GL_TEXTURE_2D);
      //cb_data_s.textureS3.unbind ();

      cb_data_s.shader1.use ();
      cb_data_s.S1resolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S1resolutionLoc != -1);
      cb_data_s.S1frameLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      //ACE_ASSERT (cb_data_s.S1frameLoc != -1);
      cb_data_s.S1channel0Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S1channel0Loc != -1);

      cb_data_s.shader2.use ();
      cb_data_s.S2resolutionLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S2resolutionLoc != -1);
      cb_data_s.S2channel0Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S2channel0Loc != -1);

      cb_data_s.shader3.use ();
      cb_data_s.S3resolutionLoc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S3resolutionLoc != -1);
      cb_data_s.S3timeLoc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S3timeLoc != -1);
      cb_data_s.S3frameLoc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S3frameLoc != -1);
      cb_data_s.S3mouseLoc =
        glGetUniformLocation(cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT(cb_data_s.S3mouseLoc != -1);
      cb_data_s.S3keysLoc =
        glGetUniformLocation(cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iKeys"));
      ACE_ASSERT(cb_data_s.S3keysLoc != -1);
      cb_data_s.S3channel0Loc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S3channel0Loc != -1);
      cb_data_s.S3channel1Loc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S3channel1Loc != -1);

      cb_data_s.shader4.use ();
      cb_data_s.S4resolutionLoc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S4resolutionLoc != -1);
      cb_data_s.S4mouseLoc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.S4mouseLoc != -1);
      cb_data_s.S4channel0Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S4channel0Loc != -1);
      cb_data_s.S4channel1Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S4channel1Loc != -1);
      //cb_data_s.S4channel2Loc =
      //  glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel2"));
      //ACE_ASSERT (cb_data_s.S4channel2Loc != -1);
      cb_data_s.S4channel3Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel3"));
      ACE_ASSERT (cb_data_s.S4channel3Loc != -1);

      glGenFramebuffers (1, &cb_data_s.FBO1);
      ACE_ASSERT (cb_data_s.FBO1);
      //glActiveTexture (GL_TEXTURE1);
      //cb_data_s.textureS1.bind ();
      // draw render pass 1 to framebuffer object (--> textureS1)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO1);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS1.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);
      //cb_data_s.textureS1.unbind ();

      //unsigned int rbo;
      //glGenRenderbuffers (1, &rbo);
      //glBindRenderbuffer (GL_RENDERBUFFER, rbo);
      //glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ENGINE_GLUT_484_DEFAULT_WIDTH, ENGINE_GLUT_484_DEFAULT_HEIGHT);
      //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

      glGenFramebuffers (1, &cb_data_s.FBO2);
      ACE_ASSERT (cb_data_s.FBO2);
      //glActiveTexture (GL_TEXTURE2);
      //cb_data_s.textureS2.bind ();
      // draw render pass 2 to framebuffer object (--> textureS2)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO2);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS2.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);
      //cb_data_s.textureS2.unbind ();

      //unsigned int rbo_2;
      //glGenRenderbuffers (1, &rbo_2);
      //glBindRenderbuffer (GL_RENDERBUFFER, rbo_2);
      //glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ENGINE_GLUT_484_DEFAULT_WIDTH, ENGINE_GLUT_484_DEFAULT_HEIGHT);
      //glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_2);

      glGenFramebuffers (1, &cb_data_s.FBO3);
      ACE_ASSERT (cb_data_s.FBO3);
      //glActiveTexture (GL_TEXTURE3);
      //cb_data_s.textureS3.bind ();
      // draw render pass 3 to framebuffer object (--> textureS3)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO3);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS3.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);
      //cb_data_s.textureS3.unbind ();

      //unsigned int rbo_3;
      //glGenRenderbuffers (1, &rbo_3);
      //glBindRenderbuffer (GL_RENDERBUFFER, rbo_3);
      //glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ENGINE_GLUT_484_DEFAULT_WIDTH, ENGINE_GLUT_484_DEFAULT_HEIGHT);
      //glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_3);

      glGenVertexArrays (1, &cb_data_s.VAO);
      ACE_ASSERT (cb_data_s.VAO);
      glGenBuffers (1, &cb_data_s.VBO);
      ACE_ASSERT (cb_data_s.VBO);

      glBindVertexArray (cb_data_s.VAO);
      GLfloat vertices_a[] =
      {
        // Triangle vertices (Texture Coordinates: 0,0 = bottom left... 1,1 = top right)
        //-------------------------
        -1.0f ,  1.0f, 0.0f,  0.0f, 1.0f, // left top ......... 0
        -1.0f , -1.0f, 0.0f,  0.0f, 0.0f, // left bottom .......1
         1.0f ,  1.0f, 0.0f,  1.0f, 1.0f, // right top .........2
 
         1.0f ,  1.0f, 0.0f,  1.0f, 1.0f, // right top .........3
        -1.0f , -1.0f, 0.0f,  0.0f, 0.0f, // left bottom .......4
         1.0f , -1.0f, 0.0f,  1.0f, 0.0f, // right bottom ......5

        // *WARNING*: GL_QUADS have been removed from core OpenGL 3.1 and above. see: https://www.khronos.org/opengl/wiki/Primitive#Quads
        // Quad vertices (Texture Coordinates: 0,0 = bottom left... 1,1 = top right)
        //-------------------------
        //-1.0f, -1.0f, 0.0f,     0.0f, 0.0f, // left bottom        0
        // 1.0f, -1.0f, 0.0f,     1.0f, 0.0f, // right bottom       1
        // 1.0f,  1.0f, 0.0f,     1.0f, 1.0f, // right top          2
        //-1.0f,  1.0f, 0.0f,     0.0f, 1.0f  // left top           4
      };
      glBindBuffer (GL_ARRAY_BUFFER, cb_data_s.VBO);
      glBufferData (GL_ARRAY_BUFFER, sizeof (vertices_a), vertices_a, GL_STATIC_DRAW);

      // position attribute
      glEnableVertexAttribArray (0);
      glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (void*)NULL);
      //// color attribute
      //glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (void*)(3 * sizeof (GLfloat)));
      //glEnableVertexAttribArray (2);
      // texture coord attribute
      glEnableVertexAttribArray (1);
      glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (void*)(3 * sizeof (GLfloat)));

      glBindBuffer (GL_ARRAY_BUFFER, 0);
      glBindVertexArray (0);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      glDeleteBuffers (1, &cb_data_s.VBO);
      glDeleteVertexArrays (1, &cb_data_s.VAO);
      glDeleteFramebuffers (1, &cb_data_s.FBO1);
      glDeleteFramebuffers (1, &cb_data_s.FBO2);
      glDeleteFramebuffers (1, &cb_data_s.FBO3);

      cb_data_s.texture0.reset ();
      cb_data_s.textureS1.reset ();
      cb_data_s.textureS2.reset ();
      cb_data_s.textureS3.reset ();

      cb_data_s.shader1.reset ();
      cb_data_s.shader2.reset ();
      cb_data_s.shader3.reset ();
      cb_data_s.shader4.reset ();

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
