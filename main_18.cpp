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
#include "defines_18.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "glut_390.h"
#include "glut_391.h"
#include "glut_392.h"
#include "pge_393.h"
#include "glut_394.h"
#include "pge_395.h"
#include "pge_396.h"
#include "pge_397.h"
#include "glut_398.h"
#include "pge_399.h"
#include "pge_400.h"
#include "pge_401.h"
#include "pge_402.h"
#include "pge_403.h"
#include "pge_404.h"
#include "pge_405.h"
#include "pge_406.h"
#include "glut_407.h"
#include "glut_408.h"
#include "pge_409.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 390,
  ENGINE_MODE_391,
  ENGINE_MODE_392,
  ENGINE_MODE_393,
  ENGINE_MODE_394,
  ENGINE_MODE_395,
  ENGINE_MODE_396,
  ENGINE_MODE_397,
  ENGINE_MODE_398,
  ENGINE_MODE_399,
  ENGINE_MODE_400,
  ENGINE_MODE_401,
  ENGINE_MODE_402,
  ENGINE_MODE_403,
  ENGINE_MODE_404,
  ENGINE_MODE_405,
  ENGINE_MODE_406,
  ENGINE_MODE_407,
  ENGINE_MODE_408,
  ENGINE_MODE_409,
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
      struct Engine_OpenGL_GLUT_390_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_390_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_390_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_390_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
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

      cb_data_s.mouseX = ENGINE_GLUT_390_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_390_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_390_DEFAULT_WIDTH, ENGINE_GLUT_390_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 390");
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

      glutDisplayFunc (engine_glut_390_draw);
      glutReshapeFunc (engine_glut_390_reshape);
      glutVisibilityFunc (engine_glut_390_visible);

      glutKeyboardFunc (engine_glut_390_key);
      glutSpecialFunc (engine_glut_390_key_special);
      glutMouseFunc (engine_glut_390_mouse_button);
      glutMotionFunc (engine_glut_390_mouse_move);
      glutPassiveMotionFunc (engine_glut_390_mouse_move);
      glutTimerFunc (100, engine_glut_390_timer, 0);

      glutCreateMenu (engine_glut_390_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_390.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_390.vert")));
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
                    ACE_TEXT ("glut_390.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_390.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_390.frag")));
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
                    ACE_TEXT ("glut_390.frag"),
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
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("canvasSize"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_391:
    {
      struct Engine_OpenGL_GLUT_391_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_391_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_391_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_391_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
      cb_data_s.resolutionLoc = -1;
      cb_data_s.ballsLoc = -1;
      cb_data_s.influencesLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_391_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_391_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_391_DEFAULT_WIDTH, ENGINE_GLUT_391_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 391");
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

      glutDisplayFunc (engine_glut_391_draw);
      glutReshapeFunc (engine_glut_391_reshape);
      glutVisibilityFunc (engine_glut_391_visible);

      glutKeyboardFunc (engine_glut_391_key);
      glutSpecialFunc (engine_glut_391_key_special);
      glutMouseFunc (engine_glut_391_mouse_button);
      glutMotionFunc (engine_glut_391_mouse_move);
      glutPassiveMotionFunc (engine_glut_391_mouse_move);
      glutTimerFunc (100, engine_glut_391_timer, 0);

      glutCreateMenu (engine_glut_391_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_391.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_391.vert")));
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
                    ACE_TEXT ("glut_391.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_391.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_391.frag")));
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
                    ACE_TEXT ("glut_391.frag"),
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
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("dim"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.ballsLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("balls"));
      ACE_ASSERT (cb_data_s.ballsLoc != -1);
      cb_data_s.influencesLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("influences"));
      ACE_ASSERT (cb_data_s.influencesLoc != -1);

      float maxVelocity =
        std::min (10.0f, std::floor (ENGINE_GLUT_391_DEFAULT_HEIGHT / Common_Tools::getRandomNumber (200.0f, 300.0f)));
      //int ballCount = Common_Tools::getRandomNumber (50, 100);
      int ballCount = ENGINE_GLUT_391_DEFAULT_BALL_COUNT;
      float minInfluence = Common_Tools::getRandomNumber (1200.0f, 6000.0f);
      float maxInfluence = Common_Tools::getRandomNumber (2000.0f, 10000.0f);
      struct ball ball_s;
      for (int i = 0; i < ballCount; i++)
      {
        ball_s.x = static_cast<float> (Common_Tools::getRandomNumber (0, ENGINE_GLUT_391_DEFAULT_WIDTH - 1));
        ball_s.y = static_cast<float> (Common_Tools::getRandomNumber (0, ENGINE_GLUT_391_DEFAULT_HEIGHT - 1));
        ball_s.dx = Common_Tools::getRandomNumber (-maxVelocity / 3.0f, maxVelocity / 3.0f);
        ball_s.dy = Common_Tools::getRandomNumber (-maxVelocity, maxVelocity);
        ball_s.influence = Common_Tools::getRandomNumber (minInfluence, maxInfluence);

        cb_data_s.balls.push_back (ball_s);
      } // end FOR

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_392:
    {
      struct Engine_OpenGL_GLUT_392_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_392_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_392_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_392_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.clickedLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_392_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_392_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse0Clicked = false;
      cb_data_s.viewX = ENGINE_GLUT_392_DEFAULT_WIDTH / 4;
      cb_data_s.viewY = ENGINE_GLUT_392_DEFAULT_HEIGHT / 4;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_392_DEFAULT_WIDTH, ENGINE_GLUT_392_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 392");
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

      glutDisplayFunc (engine_glut_392_draw);
      glutReshapeFunc (engine_glut_392_reshape);
      glutVisibilityFunc (engine_glut_392_visible);

      glutKeyboardFunc (engine_glut_392_key);
      glutSpecialFunc (engine_glut_392_key_special);
      glutMouseFunc (engine_glut_392_mouse_button);
      glutMotionFunc (engine_glut_392_mouse_move);
      glutPassiveMotionFunc (engine_glut_392_mouse_move);
      glutTimerFunc (100, engine_glut_392_timer, 0);

      glutCreateMenu (engine_glut_392_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_392.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_392.vert")));
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
                    ACE_TEXT ("glut_392.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_392.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_392.frag")));
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
                    ACE_TEXT ("glut_392.frag"),
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
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.mouseLoc != -1);
      cb_data_s.clickedLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("iView"));
      //ACE_ASSERT (cb_data_s.clickedLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_393:
    {
      PGE_393 example;
      if (example.Construct (ENGINE_PGE_393_DEFAULT_WIDTH, ENGINE_PGE_393_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_394:
    {
      struct Engine_OpenGL_GLUT_394_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_394_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_394_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_394_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.clickedLoc = -1;
      cb_data_s.textureLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_394_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_394_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse0Clicked = false;
      cb_data_s.viewX = ENGINE_GLUT_394_DEFAULT_WIDTH / 2;
      cb_data_s.viewY = ENGINE_GLUT_394_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_394_DEFAULT_WIDTH, ENGINE_GLUT_394_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 394");
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

      glutDisplayFunc (engine_glut_394_draw);
      glutReshapeFunc (engine_glut_394_reshape);
      glutVisibilityFunc (engine_glut_394_visible);

      glutKeyboardFunc (engine_glut_394_key);
      glutSpecialFunc (engine_glut_394_key_special);
      glutMouseFunc (engine_glut_394_mouse_button);
      glutMotionFunc (engine_glut_394_mouse_move);
      glutPassiveMotionFunc (engine_glut_394_mouse_move);
      glutTimerFunc (100, engine_glut_394_timer, 0);

      glutCreateMenu (engine_glut_394_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_394.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_394.frag")))
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
      //ACE_ASSERT (cb_data_s.timeLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.mouseLoc != -1);
      cb_data_s.clickedLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iView"));
      //ACE_ASSERT (cb_data_s.clickedLoc != -1);
      cb_data_s.textureLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iTexture01"));
      ACE_ASSERT (cb_data_s.textureLoc != -1);

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      if (!cb_data_s.texture.load (ACE_TEXT_ALWAYS_CHAR ("opengl_logo.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glGenerateMipmap (GL_TEXTURE_2D);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_395:
    {
      PGE_395 example;
      if (example.Construct (ENGINE_PGE_395_DEFAULT_WIDTH, ENGINE_PGE_395_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_396:
    {
      PGE_396 example;
      if (example.Construct (ENGINE_PGE_396_DEFAULT_WIDTH, ENGINE_PGE_396_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_397:
    {
      PGE_397 example;
      if (example.Construct (ENGINE_PGE_397_DEFAULT_WIDTH, ENGINE_PGE_397_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_398:
    {
      struct Engine_OpenGL_GLUT_398_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_398_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_398_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_398_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_398_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_398_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_398_DEFAULT_WIDTH, ENGINE_GLUT_398_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 398");
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

      glutDisplayFunc (engine_glut_398_draw);
      glutReshapeFunc (engine_glut_398_reshape);
      glutVisibilityFunc (engine_glut_398_visible);

      glutKeyboardFunc (engine_glut_398_key);
      glutSpecialFunc (engine_glut_398_key_special);
      glutMouseFunc (engine_glut_398_mouse_button);
      glutMotionFunc (engine_glut_398_mouse_move);
      glutPassiveMotionFunc (engine_glut_398_mouse_move);
      glutTimerFunc (100, engine_glut_398_timer, 0);

      glutCreateMenu (engine_glut_398_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_398.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_398.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      cb_data_s.shader.use ();

      cb_data_s.resolutionLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("canvasSize"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("mouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_399:
    {
      PGE_399 example;
      if (example.Construct (ENGINE_PGE_399_DEFAULT_WIDTH, ENGINE_PGE_399_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_400:
    {
      PGE_400 example;
      if (example.Construct (ENGINE_PGE_400_DEFAULT_WIDTH, ENGINE_PGE_400_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_401:
    {
      PGE_401 example;
      if (example.Construct (ENGINE_PGE_401_DEFAULT_WIDTH, ENGINE_PGE_401_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_402:
    {
      PGE_402 example;
      if (example.Construct (ENGINE_PGE_402_DEFAULT_WIDTH, ENGINE_PGE_402_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_403:
    {
      PGE_403 example;
      if (example.Construct (ENGINE_PGE_403_DEFAULT_WIDTH, ENGINE_PGE_403_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_404:
    {
      PGE_404 example;
      if (example.Construct (ENGINE_PGE_404_DEFAULT_WIDTH, ENGINE_PGE_404_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_405:
    {
      PGE_405 example;
      if (example.Construct (ENGINE_PGE_405_DEFAULT_WIDTH, ENGINE_PGE_405_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_406:
    {
      PGE_406 example;
      if (example.Construct (ENGINE_PGE_406_DEFAULT_WIDTH, ENGINE_PGE_406_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_407:
    {
      struct Engine_OpenGL_GLUT_407_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_407_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_407_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_407_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.colorPhaseLoc = -1;
      cb_data_s.displaceWeiLoc = -1;
      cb_data_s.floorUVLoc = -1;
      cb_data_s.UVScaleLoc = -1;
      cb_data_s.seedLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_407_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_407_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_407_DEFAULT_WIDTH, ENGINE_GLUT_407_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 407");
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

      glutDisplayFunc (engine_glut_407_draw);
      glutReshapeFunc (engine_glut_407_reshape);
      glutVisibilityFunc (engine_glut_407_visible);

      glutKeyboardFunc (engine_glut_407_key);
      glutSpecialFunc (engine_glut_407_key_special);
      glutMouseFunc (engine_glut_407_mouse_button);
      glutMotionFunc (engine_glut_407_mouse_move);
      glutPassiveMotionFunc (engine_glut_407_mouse_move);
      glutTimerFunc (100, engine_glut_407_timer, 0);

      glutCreateMenu (engine_glut_407_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_407.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_407.frag")))
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
      cb_data_s.colorPhaseLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iColorPhase"));
      ACE_ASSERT (cb_data_s.colorPhaseLoc != -1);
      cb_data_s.displaceWeiLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iDisplaceWei"));
      ACE_ASSERT (cb_data_s.displaceWeiLoc != -1);
      cb_data_s.floorUVLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iFloorUvMode"));
      ACE_ASSERT (cb_data_s.floorUVLoc != -1);
      cb_data_s.UVScaleLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iUvScale"));
      ACE_ASSERT (cb_data_s.UVScaleLoc != -1);
      cb_data_s.seedLoc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iSeed"));
      ACE_ASSERT (cb_data_s.seedLoc != -1);

      static std::vector<float> color_phases_a[7] = { {0.1f, 0.9f, 0.9f},
                                                      {0.0f, 0.0f, 0.0f},
                                                      {0.9f, 0.1f, 0.5f}, 
                                                      {0.0f, 0.0f, 0.2f}, 
                                                      {0.0f, 0.33f, 0.67f}, 
                                                      {0.0f, 0.1f, 0.2f}, 
                                                      {0.3f, 0.2f, 0.2f} };
      int index_i = Common_Tools::getRandomNumber (0, 6);
      cb_data_s.colorPhase = color_phases_a[index_i];

      static float displace_weis_a[3] = { 0.0f, 0.01f, 0.015f };
      index_i = Common_Tools::getRandomNumber (0, 2);
      cb_data_s.displaceWei = displace_weis_a[index_i];

      static int floor_UV_modes_a[5] = { 0, 1, 2, 3, 4 };
      index_i = Common_Tools::getRandomNumber (0, 4);
      cb_data_s.floorUVMode = floor_UV_modes_a[index_i];

      static int UV_scales_a[4] = { 0, 50, 100, 1000 };
      index_i = Common_Tools::getRandomNumber (0, 3);
      cb_data_s.UVScale = UV_scales_a[index_i];

      cb_data_s.seed = { Common_Tools::getRandomNumber (-20.0f, 20.0f),
                         Common_Tools::getRandomNumber (-20.0f, 20.0f) };

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_408:
    {
      struct Engine_OpenGL_GLUT_408_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_408_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_408_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_408_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_408_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_408_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_408_DEFAULT_WIDTH, ENGINE_GLUT_408_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 408");
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

      glutDisplayFunc (engine_glut_408_draw);
      glutReshapeFunc (engine_glut_408_reshape);
      glutVisibilityFunc (engine_glut_408_visible);

      glutKeyboardFunc (engine_glut_408_key);
      glutSpecialFunc (engine_glut_408_key_special);
      glutMouseFunc (engine_glut_408_mouse_button);
      glutMotionFunc (engine_glut_408_mouse_move);
      glutPassiveMotionFunc (engine_glut_408_mouse_move);
      glutTimerFunc (100, engine_glut_408_timer, 0);

      glutCreateMenu (engine_glut_408_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_408.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_408.frag")))
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

      result = true;

      break;
    }
    case ENGINE_MODE_409:
    {
      PGE_409 example;
      if (example.Construct (ENGINE_PGE_409_DEFAULT_WIDTH, ENGINE_PGE_409_DEFAULT_HEIGHT,
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
