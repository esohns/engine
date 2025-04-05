#include "stdafx.h"

#include <iostream>
#include <string>

#if defined (GLEW_SUPPORT)
#include "GL/glew.h"
#endif // GLEW_SUPPORT
#include "GL/freeglut.h"

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
#include "defines_29.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "glut_610.h"
#include "glut_611.h"
#include "glut_612.h"
#include "glut_613.h"
#include "glut_614.h"
#include "glut_615.h"
#include "glut_616.h"
#include "glut_617.h"
#include "glut_618.h"
#include "glut_619.h"
#include "glut_620.h"
#include "glut_621.h"
#include "glut_622.h"
#include "glut_623.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 610,
  ENGINE_MODE_611,
  ENGINE_MODE_612,
  ENGINE_MODE_613,
  ENGINE_MODE_614,
  ENGINE_MODE_615,
  ENGINE_MODE_616,
  ENGINE_MODE_617,
  ENGINE_MODE_618,
  ENGINE_MODE_619,
  ENGINE_MODE_620,
  ENGINE_MODE_621,
  ENGINE_MODE_622,
  ENGINE_MODE_623,
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
                    argument_parser.last_option ()));
        return false;
      }
      case 0:
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("found long option \"%s\", aborting\n"),
                    argument_parser.long_option ()));
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
      struct Engine_OpenGL_GLUT_610_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_610_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_610_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_610_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_610_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_610_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_610_DEFAULT_WIDTH, ENGINE_GLUT_610_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 610");
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

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_610_draw);
      glutReshapeFunc (engine_glut_610_reshape);
      glutVisibilityFunc (engine_glut_610_visible);

      glutKeyboardFunc (engine_glut_610_key);
      glutKeyboardUpFunc (engine_glut_610_key_up);
      glutSpecialFunc (engine_glut_610_key_special);
      glutMouseFunc (engine_glut_610_mouse_button);
      glutMotionFunc (engine_glut_610_mouse_move);
      glutPassiveMotionFunc (engine_glut_610_mouse_move);

      glutTimerFunc (100, engine_glut_610_timer, 0);

      glutCreateMenu (engine_glut_610_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glClampColor (GL_CLAMP_READ_COLOR, GL_FALSE);
      glClampColor (GL_CLAMP_VERTEX_COLOR, GL_FALSE);
      glClampColor (GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei (GL_PACK_ALIGNMENT, 1);

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      std::string neg_z_path_string = ACE_TEXT_ALWAYS_CHAR ("glut_484_negz.jpg"); // *TODO*: use custom texture(s)
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
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_610.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_610.frag")))
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
      cb_data_s.channel0Loc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.channel0Loc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      cb_data_s.texture0.reset ();

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_611:
    {
      struct Engine_OpenGL_GLUT_611_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_611_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_611_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_611_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_611_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_611_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_611_DEFAULT_WIDTH, ENGINE_GLUT_611_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 611");
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

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_611_draw);
      glutReshapeFunc (engine_glut_611_reshape);
      glutVisibilityFunc (engine_glut_611_visible);

      glutKeyboardFunc (engine_glut_611_key);
      glutKeyboardUpFunc (engine_glut_611_key_up);
      glutSpecialFunc (engine_glut_611_key_special);
      glutMouseFunc (engine_glut_611_mouse_button);
      glutMotionFunc (engine_glut_611_mouse_move);
      glutPassiveMotionFunc (engine_glut_611_mouse_move);

      glutTimerFunc (100, engine_glut_611_timer, 0);

      glutCreateMenu (engine_glut_611_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_611.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_611.frag")))
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

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_612:
    {
      struct Engine_OpenGL_GLUT_612_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_612_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_612_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_612_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_612_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_612_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_612_DEFAULT_WIDTH, ENGINE_GLUT_612_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 612");
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

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_612_draw);
      glutReshapeFunc (engine_glut_612_reshape);
      glutVisibilityFunc (engine_glut_612_visible);

      glutKeyboardFunc (engine_glut_612_key);
      glutKeyboardUpFunc (engine_glut_612_key_up);
      glutSpecialFunc (engine_glut_612_key_special);
      glutMouseFunc (engine_glut_612_mouse_button);
      glutMotionFunc (engine_glut_612_mouse_move);
      glutPassiveMotionFunc (engine_glut_612_mouse_move);

      glutTimerFunc (100, engine_glut_612_timer, 0);

      glutCreateMenu (engine_glut_612_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_612.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_612.frag")))
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

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_613:
    {
      struct Engine_OpenGL_GLUT_613_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_613_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_613_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_613_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_613_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_613_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_613_DEFAULT_WIDTH, ENGINE_GLUT_613_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 613");
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

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_613_draw);
      glutReshapeFunc (engine_glut_613_reshape);
      glutVisibilityFunc (engine_glut_613_visible);

      glutKeyboardFunc (engine_glut_613_key);
      glutKeyboardUpFunc (engine_glut_613_key_up);
      glutSpecialFunc (engine_glut_613_key_special);
      glutMouseFunc (engine_glut_613_mouse_button);
      glutMotionFunc (engine_glut_613_mouse_move);
      glutPassiveMotionFunc (engine_glut_613_mouse_move);

      glutTimerFunc (100, engine_glut_613_timer, 0);

      glutCreateMenu (engine_glut_613_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_613.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_613.frag")))
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

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_614:
    {
      struct Engine_OpenGL_GLUT_614_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_614_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_614_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_614_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_614_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_614_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_614_DEFAULT_WIDTH, ENGINE_GLUT_614_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 614");
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

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_614_draw);
      glutReshapeFunc (engine_glut_614_reshape);
      glutVisibilityFunc (engine_glut_614_visible);

      glutKeyboardFunc (engine_glut_614_key);
      glutKeyboardUpFunc (engine_glut_614_key_up);
      glutSpecialFunc (engine_glut_614_key_special);
      glutMouseFunc (engine_glut_614_mouse_button);
      glutMotionFunc (engine_glut_614_mouse_move);
      glutPassiveMotionFunc (engine_glut_614_mouse_move);

      glutTimerFunc (100, engine_glut_614_timer, 0);

      glutCreateMenu (engine_glut_614_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_614.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_614.frag")))
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

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_615:
    {
      struct Engine_OpenGL_GLUT_615_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_615_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_615_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_615_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_615_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_615_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_615_DEFAULT_WIDTH, ENGINE_GLUT_615_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 615");
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

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_615_draw);
      glutReshapeFunc (engine_glut_615_reshape);
      glutVisibilityFunc (engine_glut_615_visible);

      glutKeyboardFunc (engine_glut_615_key);
      glutKeyboardUpFunc (engine_glut_615_key_up);
      glutSpecialFunc (engine_glut_615_key_special);
      glutMouseFunc (engine_glut_615_mouse_button);
      glutMotionFunc (engine_glut_615_mouse_move);
      glutPassiveMotionFunc (engine_glut_615_mouse_move);

      glutTimerFunc (100, engine_glut_615_timer, 0);

      glutCreateMenu (engine_glut_615_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_615.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_615.frag")))
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

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_616:
    {
      struct Engine_OpenGL_GLUT_616_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_616_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_616_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_616_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_616_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_616_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_616_DEFAULT_WIDTH, ENGINE_GLUT_616_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 616");
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

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_616_draw);
      glutReshapeFunc (engine_glut_616_reshape);
      glutVisibilityFunc (engine_glut_616_visible);

      glutKeyboardFunc (engine_glut_616_key);
      glutKeyboardUpFunc (engine_glut_616_key_up);
      glutSpecialFunc (engine_glut_616_key_special);
      glutMouseFunc (engine_glut_616_mouse_button);
      glutMotionFunc (engine_glut_616_mouse_move);
      glutPassiveMotionFunc (engine_glut_616_mouse_move);

      glutTimerFunc (100, engine_glut_616_timer, 0);

      glutCreateMenu (engine_glut_616_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_616.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_616.frag")))
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

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_617:
    {
      struct Engine_OpenGL_GLUT_617_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_617_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_617_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_617_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_617_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_617_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_617_DEFAULT_WIDTH, ENGINE_GLUT_617_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 617");
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

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_617_draw);
      glutReshapeFunc (engine_glut_617_reshape);
      glutVisibilityFunc (engine_glut_617_visible);

      glutKeyboardFunc (engine_glut_617_key);
      glutKeyboardUpFunc (engine_glut_617_key_up);
      glutSpecialFunc (engine_glut_617_key_special);
      glutMouseFunc (engine_glut_617_mouse_button);
      glutMotionFunc (engine_glut_617_mouse_move);
      glutPassiveMotionFunc (engine_glut_617_mouse_move);

      glutTimerFunc (100, engine_glut_617_timer, 0);

      glutCreateMenu (engine_glut_617_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_617_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_617.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_617.frag")))
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
      cb_data_s.channel0Loc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.channel0Loc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      cb_data_s.texture0.reset ();

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_618:
    {
      struct Engine_OpenGL_GLUT_618_CBData cb_data_s;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1timeLoc = -1;
      cb_data_s.S1frameLoc = -1;
      cb_data_s.S1channel0Loc = -1;
      cb_data_s.S1channel0ResolutionLoc = -1;

      cb_data_s.S2resolutionLoc = -1;
      cb_data_s.S2channel0Loc = -1;

      cb_data_s.FBO1 = 0;

      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_618_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_618_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      //glutInitContextVersion (3, 3);
      //glutInitContextProfile (GLUT_CORE_PROFILE);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_618_DEFAULT_WIDTH, ENGINE_GLUT_618_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 618");
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

      //glEnable (GL_TEXTURE_2D);
      //glDisable (GL_DEPTH_TEST);
      //glDisable (GL_BLEND);
      ////glBlendFunc (GL_SRC_ALPHA, GL_DST_ALPHA);
      //glEnable (GL_CULL_FACE);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_618_draw);
      glutReshapeFunc (engine_glut_618_reshape);
      glutVisibilityFunc (engine_glut_618_visible);

      glutKeyboardFunc (engine_glut_618_key);
      glutKeyboardUpFunc (engine_glut_618_key_up);
      glutSpecialFunc (engine_glut_618_key_special);
      glutSpecialUpFunc (engine_glut_618_key_special_up);

      glutMouseFunc (engine_glut_618_mouse_button);
      glutMotionFunc (engine_glut_618_mouse_move);
      glutPassiveMotionFunc (engine_glut_618_mouse_move);

      glutTimerFunc (100, engine_glut_618_timer, 0);

      glutCreateMenu (engine_glut_618_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_618.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_618.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_618.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_618.frag_2")))
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
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_618_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE1);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS1.id_);
      ACE_ASSERT (cb_data_s.textureS1.id_);
      cb_data_s.textureS1.bind ();
      std::vector<GLfloat> initial_values_a (ENGINE_GLUT_618_DEFAULT_WIDTH * ENGINE_GLUT_618_DEFAULT_HEIGHT * 4, 0.0f);
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_618_DEFAULT_WIDTH, ENGINE_GLUT_618_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      cb_data_s.shader1.use ();
      cb_data_s.S1resolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S1resolutionLoc != -1);
      cb_data_s.S1timeLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S1timeLoc != -1);
      cb_data_s.S1frameLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S1frameLoc != -1);
      cb_data_s.S1channel0Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S1channel0Loc != -1);
      cb_data_s.S1channel0ResolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0Resolution"));
      ACE_ASSERT (cb_data_s.S1channel0ResolutionLoc != -1);

      cb_data_s.shader2.use ();
      cb_data_s.S2resolutionLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S2resolutionLoc != -1);
      cb_data_s.S2channel0Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S2channel0Loc != -1);

      glGenFramebuffers (1, &cb_data_s.FBO1);
      ACE_ASSERT (cb_data_s.FBO1);
      // draw render pass 1 to framebuffer object (--> textureS1)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO1);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS1.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);

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

      cb_data_s.texture0.reset ();
      cb_data_s.textureS1.reset ();

      cb_data_s.shader1.reset ();
      cb_data_s.shader2.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_619:
    {
      struct Engine_OpenGL_GLUT_619_CBData cb_data_s;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1timeLoc = -1;
      cb_data_s.S1frameLoc = -1;
      cb_data_s.S1mouseLoc = -1;
      cb_data_s.S1channel0Loc = -1;
      cb_data_s.S1channel0ResolutionLoc = -1;

      cb_data_s.S2resolutionLoc = -1;
      cb_data_s.S2timeLoc = -1;
      cb_data_s.S2frameLoc = -1;
      cb_data_s.S2channel0Loc = -1;
      cb_data_s.S2channel0ResolutionLoc = -1;

      cb_data_s.FBO1 = 0;

      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_619_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_619_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      //glutInitContextVersion (3, 3);
      //glutInitContextProfile (GLUT_CORE_PROFILE);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_619_DEFAULT_WIDTH, ENGINE_GLUT_619_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 619");
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

      //glEnable (GL_TEXTURE_2D);
      //glDisable (GL_DEPTH_TEST);
      //glDisable (GL_BLEND);
      ////glBlendFunc (GL_SRC_ALPHA, GL_DST_ALPHA);
      //glEnable (GL_CULL_FACE);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_619_draw);
      glutReshapeFunc (engine_glut_619_reshape);
      glutVisibilityFunc (engine_glut_619_visible);

      glutKeyboardFunc (engine_glut_619_key);
      glutKeyboardUpFunc (engine_glut_619_key_up);
      glutSpecialFunc (engine_glut_619_key_special);
      glutSpecialUpFunc (engine_glut_619_key_special_up);

      glutMouseFunc (engine_glut_619_mouse_button);
      glutMotionFunc (engine_glut_619_mouse_move);
      glutPassiveMotionFunc (engine_glut_619_mouse_move);

      glutTimerFunc (100, engine_glut_619_timer, 0);

      glutCreateMenu (engine_glut_619_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_619.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_619.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_619.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_619.frag_2")))
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

      glActiveTexture (GL_TEXTURE1);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS1.id_);
      ACE_ASSERT (cb_data_s.textureS1.id_);
      cb_data_s.textureS1.bind ();
      std::vector<GLfloat> initial_values_a (ENGINE_GLUT_619_DEFAULT_WIDTH * ENGINE_GLUT_619_DEFAULT_HEIGHT * 4, 0.0f);
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_619_DEFAULT_WIDTH, ENGINE_GLUT_619_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      cb_data_s.shader1.use ();
      cb_data_s.S1resolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S1resolutionLoc != -1);
      cb_data_s.S1timeLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S1timeLoc != -1);
      cb_data_s.S1frameLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S1frameLoc != -1);
      cb_data_s.S1mouseLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.S1mouseLoc != -1);
      cb_data_s.S1channel0Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S1channel0Loc != -1);
      cb_data_s.S1channel0ResolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0Resolution"));
      ACE_ASSERT (cb_data_s.S1channel0ResolutionLoc != -1);

      cb_data_s.shader2.use ();
      cb_data_s.S2resolutionLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S2resolutionLoc != -1);
      cb_data_s.S2timeLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S2timeLoc != -1);
      cb_data_s.S2frameLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      //ACE_ASSERT (cb_data_s.S2frameLoc != -1);
      cb_data_s.S2channel0Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S2channel0Loc != -1);
      cb_data_s.S2channel0ResolutionLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0Resolution"));
      ACE_ASSERT (cb_data_s.S2channel0ResolutionLoc != -1);

      glGenFramebuffers (1, &cb_data_s.FBO1);
      ACE_ASSERT (cb_data_s.FBO1);
      // draw render pass 1 to framebuffer object (--> textureS1)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO1);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS1.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);

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

      cb_data_s.textureS1.reset ();

      cb_data_s.shader1.reset ();
      cb_data_s.shader2.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_620:
    {
      struct Engine_OpenGL_GLUT_620_CBData cb_data_s;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1timeLoc = -1;
      cb_data_s.S1frameLoc = -1;
      cb_data_s.S1channel0Loc = -1;

      cb_data_s.S2frameLoc = -1;
      cb_data_s.S2channel0Loc = -1;
      cb_data_s.S2channel1Loc = -1;

      cb_data_s.S3frameLoc = -1;
      cb_data_s.S3channel0Loc = -1;
      cb_data_s.S3channel1Loc = -1;

      cb_data_s.FBO1 = 0;
      cb_data_s.FBO2 = 0;

      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_620_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_620_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      //glutInitContextVersion (3, 3);
      //glutInitContextProfile (GLUT_CORE_PROFILE);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_620_DEFAULT_WIDTH, ENGINE_GLUT_620_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 620");
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

      //glEnable (GL_TEXTURE_2D);
      //glDisable (GL_DEPTH_TEST);
      //glDisable (GL_BLEND);
      ////glBlendFunc (GL_SRC_ALPHA, GL_DST_ALPHA);
      //glEnable (GL_CULL_FACE);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_620_draw);
      glutReshapeFunc (engine_glut_620_reshape);
      glutVisibilityFunc (engine_glut_620_visible);

      glutKeyboardFunc (engine_glut_620_key);
      glutKeyboardUpFunc (engine_glut_620_key_up);
      glutSpecialFunc (engine_glut_620_key_special);
      glutSpecialUpFunc (engine_glut_620_key_special_up);

      glutMouseFunc (engine_glut_620_mouse_button);
      glutMotionFunc (engine_glut_620_mouse_move);
      glutPassiveMotionFunc (engine_glut_620_mouse_move);

      glutTimerFunc (100, engine_glut_620_timer, 0);

      glutCreateMenu (engine_glut_620_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_620.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_620.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_620.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_620.frag_2")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader3.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_620.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_620.frag_3")))
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

      glActiveTexture (GL_TEXTURE1);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS1.id_);
      ACE_ASSERT (cb_data_s.textureS1.id_);
      cb_data_s.textureS1.bind ();
      std::vector<GLfloat> initial_values_a (ENGINE_GLUT_620_DEFAULT_WIDTH * ENGINE_GLUT_620_DEFAULT_HEIGHT * 4, 0.0f);
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_620_DEFAULT_WIDTH, ENGINE_GLUT_620_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE2);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS2.id_);
      ACE_ASSERT (cb_data_s.textureS2.id_);
      cb_data_s.textureS2.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_620_DEFAULT_WIDTH, ENGINE_GLUT_620_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      cb_data_s.shader1.use ();
      cb_data_s.S1resolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S1resolutionLoc != -1);
      cb_data_s.S1timeLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S1timeLoc != -1);
      cb_data_s.S1frameLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S1frameLoc != -1);
      cb_data_s.S1channel0Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S1channel0Loc != -1);

      cb_data_s.shader2.use ();
      cb_data_s.S2frameLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S2frameLoc != -1);
      cb_data_s.S2channel0Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S2channel0Loc != -1);
      cb_data_s.S2channel1Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S2channel1Loc != -1);

      cb_data_s.shader3.use ();
      cb_data_s.S3frameLoc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S3frameLoc != -1);
      cb_data_s.S3channel0Loc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S3channel0Loc != -1);
      cb_data_s.S3channel1Loc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S3channel1Loc != -1);

      glGenFramebuffers (1, &cb_data_s.FBO1);
      ACE_ASSERT (cb_data_s.FBO1);
      // draw render pass 1 to framebuffer object (--> textureS1)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO1);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS1.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);

      glGenFramebuffers (1, &cb_data_s.FBO2);
      ACE_ASSERT (cb_data_s.FBO2);
      // draw render pass 2 to framebuffer object (--> textureS2)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO2);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS2.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);

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

      cb_data_s.textureS1.reset ();
      cb_data_s.textureS2.reset ();

      cb_data_s.shader1.reset ();
      cb_data_s.shader2.reset ();
      cb_data_s.shader3.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_621:
    {
      struct Engine_OpenGL_GLUT_621_CBData cb_data_s;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1timeLoc = -1;
      cb_data_s.S1frameLoc = -1;
      cb_data_s.S1channel0Loc = -1;

      cb_data_s.S2resolutionLoc = -1;
      cb_data_s.S2frameLoc = -1;
      cb_data_s.S2channel0Loc = -1;
      cb_data_s.S2channel1Loc = -1;

      cb_data_s.S3channel0Loc = -1;

      cb_data_s.S4resolutionLoc = -1;
      cb_data_s.S4channel0Loc = -1;
      cb_data_s.S4channel1Loc = -1;

      cb_data_s.FBO1 = 0;
      cb_data_s.FBO2 = 0;
      cb_data_s.FBO3 = 0;

      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_621_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_621_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      //glutInitContextVersion (3, 3);
      //glutInitContextProfile (GLUT_CORE_PROFILE);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_621_DEFAULT_WIDTH, ENGINE_GLUT_621_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 621");
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

      //glEnable (GL_TEXTURE_2D);
      //glDisable (GL_DEPTH_TEST);
      //glDisable (GL_BLEND);
      ////glBlendFunc (GL_SRC_ALPHA, GL_DST_ALPHA);
      //glEnable (GL_CULL_FACE);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_621_draw);
      glutReshapeFunc (engine_glut_621_reshape);
      glutVisibilityFunc (engine_glut_621_visible);

      glutKeyboardFunc (engine_glut_621_key);
      glutKeyboardUpFunc (engine_glut_621_key_up);
      glutSpecialFunc (engine_glut_621_key_special);
      glutSpecialUpFunc (engine_glut_621_key_special_up);

      glutMouseFunc (engine_glut_621_mouse_button);
      glutMotionFunc (engine_glut_621_mouse_move);
      glutPassiveMotionFunc (engine_glut_621_mouse_move);

      glutTimerFunc (100, engine_glut_621_timer, 0);

      glutCreateMenu (engine_glut_621_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_621.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_621.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_621.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_621.frag_2")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader3.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_621.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_621.frag_3")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader4.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_621.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_621.frag_4")))
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

      glActiveTexture (GL_TEXTURE1);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS1.id_);
      ACE_ASSERT (cb_data_s.textureS1.id_);
      cb_data_s.textureS1.bind ();
      std::vector<GLfloat> initial_values_a (ENGINE_GLUT_621_DEFAULT_WIDTH * ENGINE_GLUT_621_DEFAULT_HEIGHT * 4, 0.0f);
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_621_DEFAULT_WIDTH, ENGINE_GLUT_621_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE2);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS2.id_);
      ACE_ASSERT (cb_data_s.textureS2.id_);
      cb_data_s.textureS2.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_621_DEFAULT_WIDTH, ENGINE_GLUT_621_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE3);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS3.id_);
      ACE_ASSERT (cb_data_s.textureS3.id_);
      cb_data_s.textureS3.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_621_DEFAULT_WIDTH, ENGINE_GLUT_621_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      cb_data_s.shader1.use ();
      cb_data_s.S1resolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S1resolutionLoc != -1);
      cb_data_s.S1timeLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S1timeLoc != -1);
      cb_data_s.S1frameLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S1frameLoc != -1);
      cb_data_s.S1channel0Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S1channel0Loc != -1);

      cb_data_s.shader2.use ();
      cb_data_s.S2resolutionLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S2resolutionLoc != -1);
      cb_data_s.S2frameLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S2frameLoc != -1);
      cb_data_s.S2channel0Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S2channel0Loc != -1);
      cb_data_s.S2channel1Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S2channel1Loc != -1);

      cb_data_s.shader3.use ();
      cb_data_s.S3channel0Loc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S3channel0Loc != -1);

      cb_data_s.shader4.use ();
      cb_data_s.S4resolutionLoc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S4resolutionLoc != -1);
      cb_data_s.S4channel0Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S4channel0Loc != -1);
      cb_data_s.S4channel1Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      //ACE_ASSERT (cb_data_s.S4channel1Loc != -1);

      glGenFramebuffers (1, &cb_data_s.FBO1);
      ACE_ASSERT (cb_data_s.FBO1);
      // draw render pass 1 to framebuffer object (--> textureS1)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO1);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS1.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);

      glGenFramebuffers (1, &cb_data_s.FBO2);
      ACE_ASSERT (cb_data_s.FBO2);
      // draw render pass 2 to framebuffer object (--> textureS2)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO2);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS2.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);

      glGenFramebuffers (1, &cb_data_s.FBO3);
      ACE_ASSERT (cb_data_s.FBO3);
      // draw render pass 2 to framebuffer object (--> textureS2)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO3);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS3.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);

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
    case ENGINE_MODE_622:
    {
      struct Engine_OpenGL_GLUT_622_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_622_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_622_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_622_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.channel0Loc = -1;
      cb_data_s.channel1Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_622_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_622_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_622_DEFAULT_WIDTH, ENGINE_GLUT_622_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 622");
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

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_622_draw);
      glutReshapeFunc (engine_glut_622_reshape);
      glutVisibilityFunc (engine_glut_622_visible);

      glutKeyboardFunc (engine_glut_622_key);
      glutKeyboardUpFunc (engine_glut_622_key_up);
      glutSpecialFunc (engine_glut_622_key_special);
      glutMouseFunc (engine_glut_622_mouse_button);
      glutMotionFunc (engine_glut_622_mouse_move);
      glutPassiveMotionFunc (engine_glut_622_mouse_move);

      glutTimerFunc (100, engine_glut_622_timer, 0);

      glutCreateMenu (engine_glut_622_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_622_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE1);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture1.load (ACE_TEXT_ALWAYS_CHAR ("glut_622_channel1.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture1.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_622.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_622.frag")))
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
      cb_data_s.channel0Loc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      //ACE_ASSERT (cb_data_s.channel0Loc != -1);
      cb_data_s.channel1Loc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.channel1Loc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      cb_data_s.texture0.reset ();
      cb_data_s.texture1.reset ();

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_623:
    {
      struct Engine_OpenGL_GLUT_623_CBData cb_data_s;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1timeLoc = -1;
      cb_data_s.S1channel0Loc = -1;

      cb_data_s.S2resolutionLoc = -1;
      cb_data_s.S2channel0Loc = -1;

      cb_data_s.FBO1 = 0;

      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_623_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_623_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      //glutInitContextVersion (3, 3);
      //glutInitContextProfile (GLUT_CORE_PROFILE);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_623_DEFAULT_WIDTH, ENGINE_GLUT_623_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 623");
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

      //glEnable (GL_TEXTURE_2D);
      //glDisable (GL_DEPTH_TEST);
      //glDisable (GL_BLEND);
      ////glBlendFunc (GL_SRC_ALPHA, GL_DST_ALPHA);
      //glEnable (GL_CULL_FACE);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_623_draw);
      glutReshapeFunc (engine_glut_623_reshape);
      glutVisibilityFunc (engine_glut_623_visible);

      glutKeyboardFunc (engine_glut_623_key);
      glutKeyboardUpFunc (engine_glut_623_key_up);
      glutSpecialFunc (engine_glut_623_key_special);
      glutSpecialUpFunc (engine_glut_623_key_special_up);

      glutMouseFunc (engine_glut_623_mouse_button);
      glutMotionFunc (engine_glut_623_mouse_move);
      glutPassiveMotionFunc (engine_glut_623_mouse_move);

      glutTimerFunc (100, engine_glut_623_timer, 0);

      glutCreateMenu (engine_glut_623_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_623.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_623.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_623.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_623.frag_2")))
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
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_623_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE1);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS1.id_);
      ACE_ASSERT (cb_data_s.textureS1.id_);
      cb_data_s.textureS1.bind ();
      std::vector<GLfloat> initial_values_a (ENGINE_GLUT_623_DEFAULT_WIDTH * ENGINE_GLUT_623_DEFAULT_HEIGHT * 4, 0.0f);
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_623_DEFAULT_WIDTH, ENGINE_GLUT_623_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

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

      cb_data_s.shader2.use ();
      cb_data_s.S2resolutionLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S2resolutionLoc != -1);
      cb_data_s.S2channel0Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S2channel0Loc != -1);

      glGenFramebuffers (1, &cb_data_s.FBO1);
      ACE_ASSERT (cb_data_s.FBO1);
      // draw render pass 1 to framebuffer object (--> textureS1)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO1);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS1.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);

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

      cb_data_s.texture0.reset ();
      cb_data_s.textureS1.reset ();

      cb_data_s.shader1.reset ();
      cb_data_s.shader2.reset ();

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
  if (unlikely (!do_process_arguments (argc_in,
                                       argv_in,
                                       ui_definition_file_path,
                                       log_to_file,
                                       mode_type_e,
                                       trace_information,
                                       print_version_and_exit)))
  {
    do_print_usage (ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0], ACE_DIRECTORY_SEPARATOR_CHAR)));
    goto clean;
  } // end IF
  if (unlikely (print_version_and_exit))
  {
    do_print_version (ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0], ACE_DIRECTORY_SEPARATOR_CHAR)));
    result = EXIT_SUCCESS;
    goto clean;
  } // end IF

  // *TODO*: clean this up ASAP
  switch (mode_type_e)
  {
    default:
      break;
  } // end SWITCH
  if (unlikely (!Common_File_Tools::isReadable (ui_definition_file_path)))
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("invalid argument(s), aborting\n")));
    do_print_usage (ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0], ACE_DIRECTORY_SEPARATOR_CHAR)));
    goto clean;
  } // end IF

  // step1c: initialize logging and/or tracing
  if (log_to_file)
    log_file_name =
      Common_Log_Tools::getLogFilename (ACE_TEXT_ALWAYS_CHAR (engine_PACKAGE_NAME),
                                        ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0], ACE_DIRECTORY_SEPARATOR_CHAR)));
  if (unlikely (!Common_Log_Tools::initialize (ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0], ACE_DIRECTORY_SEPARATOR_CHAR)), // program name
                                               log_file_name,              // log file name
                                               false,                      // log to syslog ?
                                               false,                      // trace messages ?
                                               trace_information,          // debug messages ?
                                               NULL)))                     // logger ?
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
  if (unlikely (!result_3))
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
  if (unlikely (result_2 == -1))
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
  if (unlikely (result == -1))
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to ACE::fini(): \"%m\", continuing\n")));
#endif // ACE_WIN32 || ACE_WIN64

  return result;
}
