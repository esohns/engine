#include "stdafx.h"

#include <iostream>
#include <string>

#include "GL/glew.h"
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
#include "defines_10.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "glut_220.h"
#include "pge_221.h"
#include "glut_222.h"
#include "pge_223.h"
#include "pge_224.h"
#include "pge_225.h"
#include "pge_226.h"
#include "glut_227.h"
#include "glut_228.h"
#include "glut_229.h"
#include "pge_230.h"
#include "pge_231.h"
#include "glut_232.h"
#include "glut_233.h"
#include "pge_234.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 220,
  ENGINE_MODE_221,
  ENGINE_MODE_222,
  ENGINE_MODE_223,
  ENGINE_MODE_224,
  ENGINE_MODE_225,
  ENGINE_MODE_226,
  ENGINE_MODE_227,
  ENGINE_MODE_228,
  ENGINE_MODE_229,
  ENGINE_MODE_230,
  ENGINE_MODE_231,
  ENGINE_MODE_232,
  ENGINE_MODE_233,
  ENGINE_MODE_234,
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
      struct Engine_OpenGL_GLUT_220_CBData cb_data_s;
      cb_data_s.i = 2.0f * static_cast<float> (M_PI);
      cb_data_s.f = 0.0f;
      cb_data_s.m = 0;
      cb_data_s.n = 0;
      cb_data_s.mod = ENGINE_GLUT_220_DEFAULT_MOD;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.deltaAngle = 0.0f;
      cb_data_s.xOrigin = -1;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_220_DEFAULT_WIDTH, ENGINE_GLUT_220_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 220");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_220_draw);
      glutReshapeFunc (engine_glut_220_reshape);
      glutVisibilityFunc (engine_glut_220_visible);

      glutKeyboardFunc (engine_glut_220_key);
      glutSpecialFunc (engine_glut_220_key_special);
      glutMouseFunc (engine_glut_220_mouse_button);
      glutMotionFunc (engine_glut_220_mouse_move);
      glutPassiveMotionFunc (engine_glut_220_mouse_move);
      glutTimerFunc (100, engine_glut_220_timer, 0);

      glutCreateMenu (engine_glut_220_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_221:
    {
      PGE_221 example;
      if (example.Construct (ENGINE_PGE_221_DEFAULT_WIDTH, ENGINE_PGE_221_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_222:
    {
      struct Engine_OpenGL_GLUT_222_CBData cb_data_s;
      cb_data_s.scaleFactor = ENGINE_GLUT_222_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_222_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_222_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.angle = 0.0F;
      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0F;
      cb_data_s.camera.up.y = 1.0F;
      cb_data_s.camera.up.z = 0.0F;

      cb_data_s.deltaAngle = 0.0F;
      cb_data_s.xOrigin = -1;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_222_DEFAULT_WIDTH, ENGINE_GLUT_222_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 222");
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

      glutDisplayFunc (engine_glut_222_draw);
      glutReshapeFunc (engine_glut_222_reshape);
      glutVisibilityFunc (engine_glut_222_visible);

      glutKeyboardFunc (engine_glut_222_key);
      glutSpecialFunc (engine_glut_222_key_special);
      glutMouseFunc (engine_glut_222_mouse_button);
      glutMotionFunc (engine_glut_222_mouse_move);
      glutPassiveMotionFunc (engine_glut_222_mouse_move);
      glutTimerFunc (100, engine_glut_222_timer, 0);

      glutCreateMenu (engine_glut_222_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_222.vert"), data_p, file_size_i, 0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_222.vert")));
        break;
      } // end IF
      GLchar* array_a[2];
      array_a[0] = reinterpret_cast<GLchar*> (data_p);
      array_a[1] = NULL;
      GLint array_2[2];
      array_2[0] = static_cast<GLint> (file_size_i);
      array_2[1] = static_cast<GLint> (NULL);
      glShaderSource (vertexShader_id, 1, array_a, array_2);
      delete [] data_p;
      glCompileShader (vertexShader_id);
      GLint success = 0;
      glGetShaderiv (vertexShader_id, GL_COMPILE_STATUS, &success);
      if (success == GL_FALSE)
      {
        GLchar info_log_a[1024];
        GLsizei buf_size_i = 0;
        glGetShaderInfoLog (vertexShader_id,
                            sizeof (GLchar) * 1024,
                            &buf_size_i,
                            info_log_a);
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to compile \"%s\": \"%s\", aborting\n"),
                    ACE_TEXT ("glut_222.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_222.frag"), data_p, file_size_i, 0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_222.frag")));
        break;
      } // end IF
      array_a[0] = reinterpret_cast<GLchar*> (data_p);
      array_a[1] = NULL;
      array_2[0] = static_cast<GLint> (file_size_i);
      array_2[1] = static_cast<GLint> (NULL);
      glShaderSource (fragmentShader_id, 1, array_a, array_2);
      delete [] data_p; data_p = NULL;
      glCompileShader (fragmentShader_id);
      success = 0;
      glGetShaderiv (fragmentShader_id, GL_COMPILE_STATUS, &success);
      if (success == GL_FALSE)
      {
        GLchar info_log_a[4096];
        GLsizei buf_size_i = 0;
        glGetShaderInfoLog (fragmentShader_id,
                            sizeof (GLchar) * 4096,
                            &buf_size_i,
                            info_log_a);
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to compile \"%s\": \"%s\", aborting\n"),
                    ACE_TEXT ("glut_222.frag"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      cb_data_s.programId = glCreateProgram ();
      glAttachShader (cb_data_s.programId, vertexShader_id);
      glAttachShader (cb_data_s.programId, fragmentShader_id);
      glLinkProgram (cb_data_s.programId);
      success = 0;
      glGetProgramiv (cb_data_s.programId, GL_LINK_STATUS, &success);
      if (success == GL_FALSE)
      {
        GLchar info_log_a[4096];
        GLsizei buf_size_i = 0;
        glGetProgramInfoLog (cb_data_s.programId,
                             sizeof (GLchar) * 4096,
                             &buf_size_i,
                             info_log_a);
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to link GL program: \"%s\", aborting\n"),
                    ACE_TEXT (info_log_a)));

        glDetachShader (cb_data_s.programId, vertexShader_id);
        glDetachShader (cb_data_s.programId, fragmentShader_id);
        glDeleteShader (vertexShader_id);
        glDeleteShader (fragmentShader_id);
        break;
      } // end IF
      glDetachShader (cb_data_s.programId, vertexShader_id);
      glDetachShader (cb_data_s.programId, fragmentShader_id);
      glDeleteShader (vertexShader_id);
      glDeleteShader (fragmentShader_id);

      glUseProgram (cb_data_s.programId);

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_223:
    {
      PGE_223 example;
      if (example.Construct (ENGINE_PGE_223_DEFAULT_WIDTH, ENGINE_PGE_223_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_224:
    {
      PGE_224 example;
      if (example.Construct (ENGINE_PGE_224_DEFAULT_WIDTH, ENGINE_PGE_224_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_225:
    {
      PGE_225 example;
      if (example.Construct (ENGINE_PGE_225_DEFAULT_WIDTH, ENGINE_PGE_225_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_226:
    {
      PGE_226 example;
      if (example.Construct (ENGINE_PGE_226_DEFAULT_WIDTH, ENGINE_PGE_226_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_227:
    {
      struct Engine_OpenGL_GLUT_227_CBData cb_data_s;
      cb_data_s.angleX = 0.0f;
      cb_data_s.angleY = 0.0f;
      cb_data_s.angleZ = 0.0f;

      cb_data_s.wireframe = true;

      cb_data_s.angle = 0.0f;
      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 800.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.deltaAngle = 0.0f;
      cb_data_s.xOrigin = -1;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_227_DEFAULT_WIDTH, ENGINE_GLUT_227_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 227");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_229_draw);
      glutReshapeFunc (engine_glut_229_reshape);
      glutVisibilityFunc (engine_glut_229_visible);

      glutKeyboardFunc (engine_glut_229_key);
      glutSpecialFunc (engine_glut_229_key_special);
      glutMouseFunc (engine_glut_229_mouse_button);
      glutMotionFunc (engine_glut_229_mouse_move);
      glutPassiveMotionFunc (engine_glut_229_mouse_move);
      glutTimerFunc (100, engine_glut_229_timer, 0);

      glutCreateMenu (engine_glut_229_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_228:
    {
      struct Engine_OpenGL_GLUT_228_CBData cb_data_s;
      cb_data_s.distScale = ENGINE_GLUT_228_DEFAULT_DISTANCE_SCALE;
      cb_data_s.colorOffset = ENGINE_GLUT_228_DEFAULT_COLOR_OFFSET;

      cb_data_s.scaleFactor = ENGINE_GLUT_228_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_228_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_228_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
      cb_data_s.ballsXLoc = -1;
      cb_data_s.ballsYLoc = -1;
      cb_data_s.radiiLoc = -1;
      cb_data_s.ballsLoc = -1;
      cb_data_s.distScaleLoc = -1;
      cb_data_s.colorOffsetLoc = -1;
      cb_data_s.widthLoc = -1;
      cb_data_s.heightLoc = -1;

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

      cb_data_s.leftButtonIsDown = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_228_DEFAULT_WIDTH, ENGINE_GLUT_228_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 228");
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

      glutDisplayFunc (engine_glut_228_draw);
      glutReshapeFunc (engine_glut_228_reshape);
      glutVisibilityFunc (engine_glut_228_visible);

      glutKeyboardFunc (engine_glut_228_key);
      glutSpecialFunc (engine_glut_228_key_special);
      glutMouseFunc (engine_glut_228_mouse_button);
      glutMotionFunc (engine_glut_228_mouse_move);
      glutPassiveMotionFunc (engine_glut_228_mouse_move);
      glutTimerFunc (100, engine_glut_228_timer, 0);

      glutCreateMenu (engine_glut_228_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_228.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_228.vert")));
        break;
      } // end IF
      GLchar* array_a[2];
      array_a[0] = reinterpret_cast<GLchar*> (data_p);
      array_a[1] = NULL;
      GLint array_2[2];
      array_2[0] = static_cast<GLint> (file_size_i);
      array_2[1] = static_cast<GLint> (NULL);
      glShaderSource (vertexShader_id, 1, array_a, array_2);
      delete [] data_p;
      glCompileShader (vertexShader_id);
      GLint success = 0;
      glGetShaderiv (vertexShader_id, GL_COMPILE_STATUS, &success);
      if (success == GL_FALSE)
      {
        GLchar info_log_a[1024];
        GLsizei buf_size_i = 0;
        glGetShaderInfoLog (vertexShader_id,
                            sizeof (GLchar) * 1024,
                            &buf_size_i,
                            info_log_a);
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to compile \"%s\": \"%s\", aborting\n"),
                    ACE_TEXT ("glut_228.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_228.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_228.frag")));
        break;
      } // end IF
      array_a[0] = reinterpret_cast<GLchar*> (data_p);
      array_a[1] = NULL;
      array_2[0] = static_cast<GLint> (file_size_i);
      array_2[1] = NULL;
      glShaderSource (fragmentShader_id, 1, array_a, array_2);
      delete [] data_p; data_p = NULL;
      glCompileShader (fragmentShader_id);
      success = 0;
      glGetShaderiv (fragmentShader_id, GL_COMPILE_STATUS, &success);
      if (success == GL_FALSE)
      {
        GLchar info_log_a[4096];
        GLsizei buf_size_i = 0;
        glGetShaderInfoLog (fragmentShader_id,
                            sizeof (GLchar) * 4096,
                            &buf_size_i,
                            info_log_a);
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to compile \"%s\": \"%s\", aborting\n"),
                    ACE_TEXT ("glut_228.frag"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      cb_data_s.programId = glCreateProgram ();
      glAttachShader (cb_data_s.programId, vertexShader_id);
      glAttachShader (cb_data_s.programId, fragmentShader_id);
      glLinkProgram (cb_data_s.programId);
      success = 0;
      glGetProgramiv (cb_data_s.programId, GL_LINK_STATUS, &success);
      if (success == GL_FALSE)
      {
        GLchar info_log_a[4096];
        GLsizei buf_size_i = 0;
        glGetProgramInfoLog (cb_data_s.programId,
                             sizeof (GLchar) * 4096,
                             &buf_size_i,
                             info_log_a);
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to link GL program: \"%s\", aborting\n"),
                    ACE_TEXT (info_log_a)));

        glDetachShader (cb_data_s.programId, vertexShader_id);
        glDetachShader (cb_data_s.programId, fragmentShader_id);
        glDeleteShader (vertexShader_id);
        glDeleteShader (fragmentShader_id);
        break;
      } // end IF
      glDetachShader (cb_data_s.programId, vertexShader_id);
      glDetachShader (cb_data_s.programId, fragmentShader_id);
      glDeleteShader (vertexShader_id);
      glDeleteShader (fragmentShader_id);

      glUseProgram (cb_data_s.programId);

      cb_data_s.ballsXLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_ballsX"));
      ACE_ASSERT (cb_data_s.ballsXLoc != -1);
      cb_data_s.ballsYLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_ballsY"));
      ACE_ASSERT (cb_data_s.ballsYLoc != -1);
      cb_data_s.radiiLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_radii"));
      ACE_ASSERT (cb_data_s.radiiLoc != -1);
      cb_data_s.ballsLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_balls"));
      ACE_ASSERT (cb_data_s.ballsLoc != -1);
      cb_data_s.distScaleLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_distScale"));
      ACE_ASSERT (cb_data_s.distScaleLoc != -1);
      cb_data_s.colorOffsetLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_colorOffset"));
      ACE_ASSERT (cb_data_s.colorOffsetLoc != -1);
      cb_data_s.widthLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_width"));
      ACE_ASSERT (cb_data_s.widthLoc != -1);
      cb_data_s.heightLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_height"));
      ACE_ASSERT (cb_data_s.heightLoc != -1);

      for (int i = 0; i < ENGINE_GLUT_228_DEFAULT_NUMBER_OF_BALLS; ++i)
        addBall (cb_data_s,
                 Common_Tools::getRandomNumber (ENGINE_GLUT_228_DEFAULT_WIDTH / 4.0f, ENGINE_GLUT_228_DEFAULT_WIDTH / 4.0f * 3.0f),
                 Common_Tools::getRandomNumber (ENGINE_GLUT_228_DEFAULT_HEIGHT / 4.0f, ENGINE_GLUT_228_DEFAULT_HEIGHT / 4.0f * 3.0f),
                 Common_Tools::getRandomNumber (1.0f, ENGINE_GLUT_228_DEFAULT_MAX_RADIUS));

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_229:
    {
      struct Engine_OpenGL_GLUT_229_CBData cb_data_s;

      cb_data_s.wireframe = true;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 1500.0f;
      cb_data_s.camera.position.z = 900.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 0.0f;
      cb_data_s.camera.up.z = 1.0f;

      cb_data_s.lastTimeStamp = std::chrono::high_resolution_clock::now ();

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_229_DEFAULT_WIDTH, ENGINE_GLUT_229_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 229");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_229_draw);
      glutReshapeFunc (engine_glut_229_reshape);
      glutVisibilityFunc (engine_glut_229_visible);

      glutKeyboardFunc (engine_glut_229_key);
      glutSpecialFunc (engine_glut_229_key_special);
      glutMouseFunc (engine_glut_229_mouse_button);
      glutMotionFunc (engine_glut_229_mouse_move);
      glutPassiveMotionFunc (engine_glut_229_mouse_move);
      glutTimerFunc (100, engine_glut_229_timer, 0);

      glutCreateMenu (engine_glut_229_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_230:
    {
      PGE_230 example;
      if (example.Construct (ENGINE_PGE_230_DEFAULT_WIDTH, ENGINE_PGE_230_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_231:
    {
      PGE_231 example;
      if (example.Construct (ENGINE_PGE_231_DEFAULT_WIDTH, ENGINE_PGE_231_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_232:
    {
      struct Engine_OpenGL_GLUT_232_CBData cb_data_s;
      for (int i = 0; i < ENGINE_GLUT_232_DEFAULT_NUMBER_OF_OBJECTS; i++)
        cb_data_s.objects.push_back (object ());

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_232_DEFAULT_WIDTH, ENGINE_GLUT_232_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 232");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 10.0f / 255.0f);

      glEnable (GL_DEPTH_TEST);

      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_232_draw);
      glutReshapeFunc (engine_glut_232_reshape);
      glutVisibilityFunc (engine_glut_232_visible);

      glutKeyboardFunc (engine_glut_232_key);
      glutSpecialFunc (engine_glut_232_key_special);
      glutMouseFunc (engine_glut_232_mouse_button);
      glutMotionFunc (engine_glut_232_mouse_move);
      glutPassiveMotionFunc (engine_glut_232_mouse_move);
      glutTimerFunc (100, engine_glut_232_timer, 0);

      glutCreateMenu (engine_glut_232_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_233:
    {
      struct Engine_OpenGL_GLUT_233_CBData cb_data_s;
      cb_data_s.f = 0.0f;
      cb_data_s.n = 0;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 500.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_233_DEFAULT_WIDTH, ENGINE_GLUT_233_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 233");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_LIGHTING);
      //glEnable (GL_LIGHT0);
      //glEnable (GL_LIGHT1);

      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      glutDisplayFunc (engine_glut_233_draw);
      glutReshapeFunc (engine_glut_233_reshape);
      glutVisibilityFunc (engine_glut_233_visible);

      glutKeyboardFunc (engine_glut_233_key);
      glutSpecialFunc (engine_glut_233_key_special);
      glutMouseFunc (engine_glut_233_mouse_button);
      glutMotionFunc (engine_glut_233_mouse_move);
      glutPassiveMotionFunc (engine_glut_233_mouse_move);
      glutTimerFunc (100, engine_glut_233_timer, 0);

      glutCreateMenu (engine_glut_233_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_234:
    {
      PGE_234 example;
      if (example.Construct (ENGINE_PGE_234_DEFAULT_WIDTH, ENGINE_PGE_234_DEFAULT_HEIGHT,
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
    do_printVersion (ACE_TEXT_ALWAYS_CHAR (ACE::basename (argv_in[0])));
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
