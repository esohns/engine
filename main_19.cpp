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
#include "defines_19.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "glut_410.h"
#include "pge_411.h"
#include "glut_412.h"
#include "glut_413.h"
#include "pge_414.h"
#include "glut_415.h"
#include "pge_416.h"
#include "glut_417.h"
#include "glut_418.h"
#include "pge_419.h"
#include "pge_420.h"
#include "pge_421.h"
#include "pge_422.h"
#include "glut_423.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 410,
  ENGINE_MODE_411,
  ENGINE_MODE_412,
  ENGINE_MODE_413,
  ENGINE_MODE_414,
  ENGINE_MODE_415,
  ENGINE_MODE_416,
  ENGINE_MODE_417,
  ENGINE_MODE_418,
  ENGINE_MODE_419,
  ENGINE_MODE_420,
  ENGINE_MODE_421,
  ENGINE_MODE_422,
  ENGINE_MODE_423,
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
      struct Engine_OpenGL_GLUT_410_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_410_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_410_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_410_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.imageLoc = -1;
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

      cb_data_s.mouseX = ENGINE_GLUT_410_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_410_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_410_DEFAULT_WIDTH, ENGINE_GLUT_410_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 410");
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

      glutDisplayFunc (engine_glut_410_draw);
      glutReshapeFunc (engine_glut_410_reshape);
      glutVisibilityFunc (engine_glut_410_visible);

      glutKeyboardFunc (engine_glut_410_key);
      glutSpecialFunc (engine_glut_410_key_special);
      glutMouseFunc (engine_glut_410_mouse_button);
      glutMotionFunc (engine_glut_410_mouse_move);
      glutPassiveMotionFunc (engine_glut_410_mouse_move);
      glutTimerFunc (100, engine_glut_410_timer, 0);

      glutCreateMenu (engine_glut_410_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_410.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_410.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.imageLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iImage"));
      ACE_ASSERT (cb_data_s.imageLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.mouseLoc != -1);

      if (!cb_data_s.texture.load (ACE_TEXT_ALWAYS_CHAR ("cat.jpg"),
                                   false)) // don't flip image in this case
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_411:
    {
      PGE_411 example;
      if (example.Construct (ENGINE_PGE_411_DEFAULT_WIDTH, ENGINE_PGE_411_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_412:
    {
      struct Engine_OpenGL_GLUT_412_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_412_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_412_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_412_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.aspectLoc = -1;
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

      cb_data_s.mouseX = ENGINE_GLUT_412_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_412_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_412_DEFAULT_WIDTH, ENGINE_GLUT_412_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 412");
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

      glutDisplayFunc (engine_glut_412_draw);
      glutReshapeFunc (engine_glut_412_reshape);
      glutVisibilityFunc (engine_glut_412_visible);

      glutKeyboardFunc (engine_glut_412_key);
      glutSpecialFunc (engine_glut_412_key_special);
      glutMouseFunc (engine_glut_412_mouse_button);
      glutMotionFunc (engine_glut_412_mouse_move);
      glutPassiveMotionFunc (engine_glut_412_mouse_move);
      glutTimerFunc (100, engine_glut_412_timer, 0);

      glutCreateMenu (engine_glut_412_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_412.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_412.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.aspectLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("aspect"));
      ACE_ASSERT (cb_data_s.aspectLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_413:
    {
      struct Engine_OpenGL_GLUT_413_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_413_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_413_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_413_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.whirlsLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_413_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_413_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_413_DEFAULT_WIDTH, ENGINE_GLUT_413_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 413");
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

      glutDisplayFunc (engine_glut_413_draw);
      glutReshapeFunc (engine_glut_413_reshape);
      glutVisibilityFunc (engine_glut_413_visible);

      glutKeyboardFunc (engine_glut_413_key);
      glutSpecialFunc (engine_glut_413_key_special);
      glutMouseFunc (engine_glut_413_mouse_button);
      glutMotionFunc (engine_glut_413_mouse_move);
      glutPassiveMotionFunc (engine_glut_413_mouse_move);
      glutTimerFunc (100, engine_glut_413_timer, 0);

      glutCreateMenu (engine_glut_413_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_413.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_413.frag")))
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
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);
      cb_data_s.whirlsLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("whirls"));
      ACE_ASSERT (cb_data_s.whirlsLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glm::vec2 lastP (-5.0f, -5.0f);
      struct whirl whirl_s;
      for (int i = 0; i < ENGINE_GLUT_413_DEFAULT_NUMBER_OF_WHIRLS; i++)
      {
        glm::vec2 p (Common_Tools::getRandomNumber (0.0f, 1.0f),
                     Common_Tools::getRandomNumber (0.0f, 1.0f));
        while (glm::distance (p, lastP) < 0.3f)
          p = glm::vec2 (Common_Tools::getRandomNumber (0.0f, 1.0f),
                         Common_Tools::getRandomNumber (0.0f, 1.0f));

        whirl_s.id = Common_Tools::getRandomNumber (0, 100000);
        whirl_s.p = p;
        whirl_s.r = Common_Tools::getRandomNumber (0.1f, 0.25f);
        whirl_s.distortForce =
          Common_Tools::getRandomNumber (0.0f, 1.0f) * Common_Tools::getRandomNumber (0.0f, 0.2f);
        cb_data_s.whirls.push_back (whirl_s);

        lastP = p;
      } // end FOR

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_414:
    {
      PGE_414 example;
      if (example.Construct (ENGINE_PGE_414_DEFAULT_WIDTH, ENGINE_PGE_414_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_415:
    {
      struct Engine_OpenGL_GLUT_415_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_415_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_415_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_415_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_415_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_415_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_415_DEFAULT_WIDTH, ENGINE_GLUT_415_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 415");
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

      glutDisplayFunc (engine_glut_415_draw);
      glutReshapeFunc (engine_glut_415_reshape);
      glutVisibilityFunc (engine_glut_415_visible);

      glutKeyboardFunc (engine_glut_415_key);
      glutSpecialFunc (engine_glut_415_key_special);
      glutMouseFunc (engine_glut_415_mouse_button);
      glutMotionFunc (engine_glut_415_mouse_move);
      glutPassiveMotionFunc (engine_glut_415_mouse_move);
      glutTimerFunc (100, engine_glut_415_timer, 0);

      glutCreateMenu (engine_glut_415_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_415.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_415.frag")))
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
    case ENGINE_MODE_416:
    {
      PGE_416 example;
      if (example.Construct (ENGINE_PGE_416_DEFAULT_WIDTH, ENGINE_PGE_416_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_417:
    {
      struct Engine_OpenGL_GLUT_417_CBData cb_data_s;

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

      cb_data_s.mousex = ENGINE_GLUT_417_DEFAULT_WIDTH / 2;
      cb_data_s.mousey = ENGINE_GLUT_417_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse_0_ButtonDown = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_417_DEFAULT_WIDTH, ENGINE_GLUT_417_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 417");
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

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //glEnable (GL_NORMALIZE);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_417_draw);
      glutReshapeFunc (engine_glut_417_reshape);
      glutVisibilityFunc (engine_glut_417_visible);

      glutKeyboardFunc (engine_glut_417_key);
      glutSpecialFunc (engine_glut_417_key_special);
      glutMouseFunc (engine_glut_417_mouse_button);
      glutMotionFunc (engine_glut_417_mouse_move);
      glutPassiveMotionFunc (engine_glut_417_mouse_move);
      glutTimerFunc (100, engine_glut_417_timer, 0);

      glutCreateMenu (engine_glut_417_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      cb_data_s.r = std::min (ENGINE_GLUT_417_DEFAULT_WIDTH, ENGINE_GLUT_417_DEFAULT_HEIGHT) / 3.0f;

      cb_data_s.xOffset = Common_Tools::getRandomNumber (0.0, 10000.0);
      cb_data_s.yOffset = Common_Tools::getRandomNumber (0.0, 10000.0);
      cb_data_s.zOffset = Common_Tools::getRandomNumber (0.0, 10000.0);

      cb_data_s.points = getPointDistribution (ENGINE_GLUT_417_DEFAULT_N, cb_data_s.r);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_418:
    {
      struct Engine_OpenGL_GLUT_418_CBData cb_data_s;

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

      cb_data_s.mousex = ENGINE_GLUT_418_DEFAULT_WIDTH / 2;
      cb_data_s.mousey = ENGINE_GLUT_418_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse_0_ButtonDown = false;
      cb_data_s.spin = 0.0f;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_418_DEFAULT_WIDTH, ENGINE_GLUT_418_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 418");
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

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //glEnable (GL_NORMALIZE);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_418_draw);
      glutReshapeFunc (engine_glut_418_reshape);
      glutVisibilityFunc (engine_glut_418_visible);

      glutKeyboardFunc (engine_glut_418_key);
      glutSpecialFunc (engine_glut_418_key_special);
      glutMouseFunc (engine_glut_418_mouse_button);
      glutMotionFunc (engine_glut_418_mouse_move);
      glutPassiveMotionFunc (engine_glut_418_mouse_move);
      glutTimerFunc (100, engine_glut_418_timer, 0);

      glutCreateMenu (engine_glut_418_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      cb_data_s.falling = true;
      cb_data_s.counter = 0;
      cb_data_s.size = ENGINE_GLUT_418_DEFAULT_HEIGHT / 4.0f;

      int count = 90;
      for (float ya = -static_cast<float> (M_PI_2); ya < 3.0f * static_cast<float> (M_PI_2); ya += static_cast<float> (M_PI_2) / 12.0f)
        for (float xa = -static_cast<float> (M_PI); xa < static_cast<float> (M_PI); xa += static_cast<float> (M_PI) / 3.0f)
        {
          float x = std::cos (xa) * std::cos (ya) * cb_data_s.size;
          float y = (-cb_data_s.size / 2.0f) + std::sin (xa) * std::cos (ya) * cb_data_s.size;
          float z = cb_data_s.size * std::sin (ya);
          glm::vec3 pos (x, y, z);
          cb_data_s.beads.push_back (bead (count, pos));
          count += 4;
        } // end FOR

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_419:
    {
      PGE_419 example;
      if (example.Construct (ENGINE_PGE_419_DEFAULT_WIDTH, ENGINE_PGE_419_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_420:
    {
      PGE_420 example;
      if (example.Construct (ENGINE_PGE_420_DEFAULT_WIDTH, ENGINE_PGE_420_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_421:
    {
      PGE_421 example;
      if (example.Construct (ENGINE_PGE_421_DEFAULT_WIDTH, ENGINE_PGE_421_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_422:
    {
      PGE_422 example;
      if (example.Construct (ENGINE_PGE_422_DEFAULT_WIDTH, ENGINE_PGE_422_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_423:
    {
      struct Engine_OpenGL_GLUT_423_CBData cb_data_s;

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

      cb_data_s.mousex = ENGINE_GLUT_423_DEFAULT_WIDTH / 2;
      cb_data_s.mousey = ENGINE_GLUT_423_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse_0_ButtonDown = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_423_DEFAULT_WIDTH, ENGINE_GLUT_423_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 423");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_423_draw);
      glutReshapeFunc (engine_glut_423_reshape);
      glutVisibilityFunc (engine_glut_423_visible);

      glutKeyboardFunc (engine_glut_423_key);
      glutSpecialFunc (engine_glut_423_key_special);
      glutMouseFunc (engine_glut_423_mouse_button);
      glutMotionFunc (engine_glut_423_mouse_move);
      glutPassiveMotionFunc (engine_glut_423_mouse_move);
      glutTimerFunc (100, engine_glut_423_timer, 0);

      glutCreateMenu (engine_glut_423_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

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
