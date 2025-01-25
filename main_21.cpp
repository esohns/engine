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
#include "defines_21.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "glut_450.h"
#include "glut_451.h"
#include "pge_452.h"
#include "glut_453.h"
#include "glut_454.h"
#include "pge_455.h"
#include "glut_456.h"
#include "glut_457.h"
#include "glut_458.h"
#include "glut_459.h"
#include "glut_460.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 450,
  ENGINE_MODE_451,
  ENGINE_MODE_452,
  ENGINE_MODE_453,
  ENGINE_MODE_454,
  ENGINE_MODE_455,
  ENGINE_MODE_456,
  ENGINE_MODE_457,
  ENGINE_MODE_458,
  ENGINE_MODE_459,
  ENGINE_MODE_460,
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
      struct Engine_OpenGL_GLUT_450_CBData cb_data_s;

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

      cb_data_s.mousex = ENGINE_GLUT_450_DEFAULT_WIDTH / 2;
      cb_data_s.mousey = ENGINE_GLUT_450_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse_0_ButtonDown = false;

      cb_data_s.f = 0.0f;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_450_DEFAULT_WIDTH, ENGINE_GLUT_450_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 450");
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

      glutDisplayFunc (engine_glut_450_draw);
      glutReshapeFunc (engine_glut_450_reshape);
      glutVisibilityFunc (engine_glut_450_visible);

      glutKeyboardFunc (engine_glut_450_key);
      glutSpecialFunc (engine_glut_450_key_special);
      glutMouseFunc (engine_glut_450_mouse_button);
      glutMotionFunc (engine_glut_450_mouse_move);
      glutPassiveMotionFunc (engine_glut_450_mouse_move);
      glutTimerFunc (100, engine_glut_450_timer, 0);

      glutCreateMenu (engine_glut_450_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_451:
    {
      struct Engine_OpenGL_GLUT_451_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_451_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_451_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_451_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseXLoc = -1;
      cb_data_s.mouseYLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_451_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_451_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_451_DEFAULT_WIDTH, ENGINE_GLUT_451_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 451");
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

      glutDisplayFunc (engine_glut_451_draw);
      glutReshapeFunc (engine_glut_451_reshape);
      glutVisibilityFunc (engine_glut_451_visible);

      glutKeyboardFunc (engine_glut_451_key);
      glutSpecialFunc (engine_glut_451_key_special);
      glutMouseFunc (engine_glut_451_mouse_button);
      glutMotionFunc (engine_glut_451_mouse_move);
      glutPassiveMotionFunc (engine_glut_451_mouse_move);
      glutTimerFunc (100, engine_glut_451_timer, 0);

      glutCreateMenu (engine_glut_451_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_451.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_451.frag")))
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
      cb_data_s.mouseXLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_mouseX"));
      ACE_ASSERT (cb_data_s.mouseXLoc != -1);
      cb_data_s.mouseYLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("u_mouseY"));
      ACE_ASSERT (cb_data_s.mouseYLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_452:
    {
      PGE_452 example;
      if (example.Construct (ENGINE_PGE_452_DEFAULT_WIDTH, ENGINE_PGE_452_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_453:
    {
      struct Engine_OpenGL_GLUT_453_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_453_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_453_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_453_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_453_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_453_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_453_DEFAULT_WIDTH, ENGINE_GLUT_453_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 453");
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

      glutDisplayFunc (engine_glut_453_draw);
      glutReshapeFunc (engine_glut_453_reshape);
      glutVisibilityFunc (engine_glut_453_visible);

      glutKeyboardFunc (engine_glut_453_key);
      glutSpecialFunc (engine_glut_453_key_special);
      glutMouseFunc (engine_glut_453_mouse_button);
      glutMotionFunc (engine_glut_453_mouse_move);
      glutPassiveMotionFunc (engine_glut_453_mouse_move);
      glutTimerFunc (100, engine_glut_453_timer, 0);

      glutCreateMenu (engine_glut_453_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_453.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_453.frag")))
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
    case ENGINE_MODE_454:
    {
      struct Engine_OpenGL_GLUT_454_CBData cb_data_s;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 1000.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.mousex = ENGINE_GLUT_454_DEFAULT_WIDTH / 2;
      cb_data_s.mousey = ENGINE_GLUT_454_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse_0_ButtonDown = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_454_DEFAULT_WIDTH, ENGINE_GLUT_454_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 454");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (180.0f / 255.0f, 180.0f / 255.0f, 180.0f / 255.0f, 1.0f);

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

      glutDisplayFunc (engine_glut_454_draw);
      glutReshapeFunc (engine_glut_454_reshape);
      glutVisibilityFunc (engine_glut_454_visible);

      glutKeyboardFunc (engine_glut_454_key);
      glutSpecialFunc (engine_glut_454_key_special);
      glutMouseFunc (engine_glut_454_mouse_button);
      glutMotionFunc (engine_glut_454_mouse_move);
      glutPassiveMotionFunc (engine_glut_454_mouse_move);
      glutTimerFunc (100, engine_glut_454_timer, 0);

      glutCreateMenu (engine_glut_454_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      adjacencies[S_FRONT] = { S_TOP, S_RIGHT, S_BOTTOM, S_LEFT };
      adjacencies[S_RIGHT] = {S_TOP, S_BACK, S_BOTTOM, S_FRONT};
      adjacencies[S_BACK] = { S_TOP, S_LEFT, S_BOTTOM, S_RIGHT };
      adjacencies[S_LEFT] = { S_TOP, S_FRONT, S_BOTTOM, S_BACK };
      adjacencies[S_TOP] = { S_BACK, S_RIGHT, S_FRONT, S_LEFT };
      adjacencies[S_BOTTOM] = { S_FRONT, S_RIGHT, S_BACK, S_LEFT };

      rotations[S_FRONT] = { 0, 0, 0, 0 };
      rotations[S_RIGHT] = { 270, 0, 90, 0 };
      rotations[S_BACK] = { 180, 0, 180, 0 };
      rotations[S_LEFT] = { 90, 0, 270, 0 };
      rotations[S_TOP] = { 180, 90, 0, 270 };
      rotations[S_BOTTOM] = { 0, 270, 180, 90 };

      for (int i = 0; i < ENGINE_GLUT_454_DEFAULT_NUMBER_OF_AGENTS; i++)
        cb_data_s.agents.push_back (agent ());

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_455:
    {
      PGE_455 example;
      if (example.Construct (ENGINE_PGE_455_DEFAULT_WIDTH, ENGINE_PGE_455_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_456:
    {
      struct Engine_OpenGL_GLUT_457_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_456_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_456_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_456_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      //cb_data_s.mouseLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_456_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_456_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_456_DEFAULT_WIDTH, ENGINE_GLUT_456_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 456");
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

      glutDisplayFunc (engine_glut_456_draw);
      glutReshapeFunc (engine_glut_456_reshape);
      glutVisibilityFunc (engine_glut_456_visible);

      glutKeyboardFunc (engine_glut_456_key);
      glutSpecialFunc (engine_glut_456_key_special);
      glutMouseFunc (engine_glut_456_mouse_button);
      glutMotionFunc (engine_glut_456_mouse_move);
      glutPassiveMotionFunc (engine_glut_456_mouse_move);
      glutTimerFunc (100, engine_glut_456_timer, 0);

      glutCreateMenu (engine_glut_456_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_456.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_456.frag")))
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
      //cb_data_s.mouseLoc =
      //  glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("mouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_457:
    {
      struct Engine_OpenGL_GLUT_457_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_457_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_457_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_457_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.pointerCountLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_457_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_457_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_457_DEFAULT_WIDTH, ENGINE_GLUT_457_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 457");
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

      glutDisplayFunc (engine_glut_457_draw);
      glutReshapeFunc (engine_glut_457_reshape);
      glutVisibilityFunc (engine_glut_457_visible);

      glutKeyboardFunc (engine_glut_457_key);
      glutSpecialFunc (engine_glut_457_key_special);
      glutMouseFunc (engine_glut_457_mouse_button);
      glutMotionFunc (engine_glut_457_mouse_move);
      glutPassiveMotionFunc (engine_glut_457_mouse_move);
      glutTimerFunc (100, engine_glut_457_timer, 0);

      glutCreateMenu (engine_glut_457_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_457.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_457.frag")))
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
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("touch"));
      ACE_ASSERT (cb_data_s.mouseLoc != -1);
      cb_data_s.pointerCountLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("pointerCount"));
      ACE_ASSERT (cb_data_s.pointerCountLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_458:
    {
      struct Engine_OpenGL_GLUT_458_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_458_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_458_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_458_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.pointerCountLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_458_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_458_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_458_DEFAULT_WIDTH, ENGINE_GLUT_458_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 458");
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

      glutDisplayFunc (engine_glut_458_draw);
      glutReshapeFunc (engine_glut_458_reshape);
      glutVisibilityFunc (engine_glut_458_visible);

      glutKeyboardFunc (engine_glut_458_key);
      glutSpecialFunc (engine_glut_458_key_special);
      glutMouseFunc (engine_glut_458_mouse_button);
      glutMotionFunc (engine_glut_458_mouse_move);
      glutPassiveMotionFunc (engine_glut_458_mouse_move);
      glutTimerFunc (100, engine_glut_458_timer, 0);

      glutCreateMenu (engine_glut_458_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_458.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_458.frag")))
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
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("touch"));
      ACE_ASSERT (cb_data_s.mouseLoc != -1);
      cb_data_s.pointerCountLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("pointerCount"));
      ACE_ASSERT (cb_data_s.pointerCountLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_459:
    {
      struct Engine_OpenGL_GLUT_459_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_458_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_459_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_459_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.phaseLoc = -1;
      cb_data_s.scaleLoc = -1;
      cb_data_s.octavesLoc = -1;
      cb_data_s.frequencyLoc = -1;
      cb_data_s.noiseFactorLoc = -1;
      cb_data_s.sharpnessLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.patternLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_459_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_459_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      cb_data_s.phase = {0.0f, 0.0f};
      cb_data_s.scale = ENGINE_GLUT_459_DEFAULT_NOISE_SCALE;
      cb_data_s.octaves = ENGINE_GLUT_459_DEFAULT_NOISE_OCTAVES;
      cb_data_s.noiseFactor = ENGINE_GLUT_459_DEFAULT_NOISE_FACTOR;
      cb_data_s.frequency = ENGINE_GLUT_459_DEFAULT_FREQUENCY;
      cb_data_s.sharpness = ENGINE_GLUT_459_DEFAULT_SHARPNESS;
      cb_data_s.pattern = ENGINE_GLUT_459_DEFAULT_PATTERN;

      // initialize GTK
      Common_UI_GTK_Configuration_t gtk_configuration;
      struct Engine_UI_GTK_459_CBData ui_cb_data;
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
      gtk_configuration.eventHooks.finiHook = idle_finalize_UI_459_cb;
      gtk_configuration.eventHooks.initHook = idle_initialize_UI_459_cb;
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
      glutInitWindowSize (ENGINE_GLUT_459_DEFAULT_WIDTH, ENGINE_GLUT_459_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 459");
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

      glutDisplayFunc (engine_glut_459_draw);
      glutReshapeFunc (engine_glut_459_reshape);
      glutVisibilityFunc (engine_glut_459_visible);

      glutKeyboardFunc (engine_glut_459_key);
      glutSpecialFunc (engine_glut_459_key_special);
      glutMouseFunc (engine_glut_459_mouse_button);
      glutMotionFunc (engine_glut_459_mouse_move);
      glutPassiveMotionFunc (engine_glut_459_mouse_move);
      glutTimerFunc (100, engine_glut_459_timer, 0);

      glutCreateMenu (engine_glut_459_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_459.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_459.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uResolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.phaseLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uPhase"));
      ACE_ASSERT (cb_data_s.phaseLoc != -1);
      cb_data_s.scaleLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uScale"));
      ACE_ASSERT (cb_data_s.scaleLoc != -1);
      cb_data_s.octavesLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uOctaves"));
      ACE_ASSERT (cb_data_s.octavesLoc != -1);
      cb_data_s.frequencyLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uFreq"));
      ACE_ASSERT (cb_data_s.frequencyLoc != -1);
      cb_data_s.noiseFactorLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uNoiseFactor"));
      ACE_ASSERT (cb_data_s.noiseFactorLoc != -1);
      cb_data_s.sharpnessLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uSharpness"));
      ACE_ASSERT (cb_data_s.sharpnessLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uTime"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      cb_data_s.patternLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uPattern"));
      ACE_ASSERT (cb_data_s.patternLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      gtk_manager_p->stop (true,   // wait ?
                           false);

      result = true;

      break;
    }
    case ENGINE_MODE_460:
    {
      struct Engine_OpenGL_GLUT_460_CBData cb_data_s;

      cb_data_s.positionLoc = -1;
      cb_data_s.colorLoc = -1;
      cb_data_s.modelViewLoc = -1;
      cb_data_s.projectionLoc = -1;

      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.numPoints = 0;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position_.x = 0.0f;
      cb_data_s.camera.position_.y = 0.0f;
      cb_data_s.camera.position_.z = 1500.0f;
      cb_data_s.camera.looking_at_.x = 0.0f;
      cb_data_s.camera.looking_at_.y = 0.0f;
      cb_data_s.camera.looking_at_.z = 0.0f;
      cb_data_s.camera.up_.x = 0.0f;
      cb_data_s.camera.up_.y = 1.0f;
      cb_data_s.camera.up_.z = 0.0f;

      cb_data_s.mouseX = ENGINE_GLUT_460_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_460_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_460_DEFAULT_WIDTH, ENGINE_GLUT_460_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 460");
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

      glutDisplayFunc (engine_glut_460_draw);
      glutReshapeFunc (engine_glut_460_reshape);
      glutVisibilityFunc (engine_glut_460_visible);

      glutKeyboardFunc (engine_glut_460_key);
      glutSpecialFunc (engine_glut_460_key_special);
      glutMouseFunc (engine_glut_460_mouse_button);
      glutMotionFunc (engine_glut_460_mouse_move);
      glutPassiveMotionFunc (engine_glut_460_mouse_move);
      glutTimerFunc (100, engine_glut_460_timer, 0);

      glutCreateMenu (engine_glut_460_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_460.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_460.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.positionLoc =
        glGetAttribLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("aPosition"));
      ACE_ASSERT(cb_data_s.positionLoc != -1);
      cb_data_s.colorLoc =
        glGetAttribLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("aColor"));
      ACE_ASSERT(cb_data_s.colorLoc != -1);
      cb_data_s.modelViewLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uModelViewMatrix"));
      ACE_ASSERT (cb_data_s.modelViewLoc != -1);
      cb_data_s.projectionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("uProjectionMatrix"));
      ACE_ASSERT (cb_data_s.projectionLoc != -1);

      glGenVertexArrays(1, &cb_data_s.VAO);
      glBindVertexArray (cb_data_s.VAO);
      glGenBuffers (1, &cb_data_s.VBO);
      glBindBuffer (GL_ARRAY_BUFFER, cb_data_s.VBO);

      std::vector<float> positionColorData;
      std::string line_string;
      // *NOTE*: see https://raw.githubusercontent.com/cansik/p5js-pointcloud/master/data/forest-blk360_centered.ply
      std::ifstream input_file_stream ("forest-blk360_centered.ply");
      bool in_header_b = true;
      while (input_file_stream.good ())
      {
        std::getline (input_file_stream, line_string);
        if (line_string == ACE_TEXT_ALWAYS_CHAR ("end_header"))
        {
          in_header_b = false;
          continue;
        } // end IF
        if (in_header_b)
          continue;

        std::istringstream line_stream (line_string);
        glm::vec3 position_s;
        line_stream >> position_s.x;
        line_stream >> position_s.y;
        line_stream >> position_s.z;

        glm::vec3 color_s;
        line_stream >> color_s.r;
        line_stream >> color_s.g;
        line_stream >> color_s.b;

        positionColorData.push_back (position_s.x * ENGINE_GLUT_460_DEFAULT_SCALE + ENGINE_GLUT_460_DEFAULT_X_ADD);
        positionColorData.push_back (position_s.y * ENGINE_GLUT_460_DEFAULT_SCALE + ENGINE_GLUT_460_DEFAULT_Y_ADD);
        positionColorData.push_back (position_s.z * ENGINE_GLUT_460_DEFAULT_SCALE + ENGINE_GLUT_460_DEFAULT_Z_ADD);

        positionColorData.push_back (color_s.r / 255.0f);
        positionColorData.push_back (color_s.g / 255.0f);
        positionColorData.push_back (color_s.b / 255.0f);
      
        ++cb_data_s.numPoints;
      } // end WHILE
      input_file_stream.close ();

      glBufferData (GL_ARRAY_BUFFER, positionColorData.size () * sizeof (float), positionColorData.data (), GL_STATIC_DRAW);

      glEnableVertexAttribArray (cb_data_s.positionLoc); // Enable attribute first before specifiying attributes
      glVertexAttribPointer (cb_data_s.positionLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), 0); // 0 offset from beginning
      glEnableVertexAttribArray (cb_data_s.colorLoc);
      glVertexAttribPointer (cb_data_s.colorLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), (void*)(3 * sizeof (float))); // 3 offset from beginning

      glBindBuffer (GL_ARRAY_BUFFER, 0);
      glBindVertexArray (0);

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
    case ENGINE_MODE_459:
    {
      ui_definition_file_path = Common_File_Tools::directory(ui_definition_file_path);
      ui_definition_file_path += ACE_DIRECTORY_SEPARATOR_CHAR_A;
      ui_definition_file_path +=
        ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_459_UI_GTK_DEFINITION_FILE);
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
