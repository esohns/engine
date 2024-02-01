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
#include "defines_15.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "pge_320.h"
#include "glut_321.h"
#include "glut_322.h"
#include "pge_323.h"
#include "pge_324.h"
#include "pge_325.h"
#include "pge_326.h"
#include "glut_327.h"
#include "glut_328.h"
#include "glut_329.h"
#include "glut_330.h"
#include "glut_331.h"
#include "pge_332.h"
#include "glut_333.h"
#include "pge_334.h"
#include "glut_335.h"
#include "pge_336.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 320,
  ENGINE_MODE_321,
  ENGINE_MODE_322,
  ENGINE_MODE_323,
  ENGINE_MODE_324,
  ENGINE_MODE_325,
  ENGINE_MODE_326,
  ENGINE_MODE_327,
  ENGINE_MODE_328,
  ENGINE_MODE_329,
  ENGINE_MODE_330,
  ENGINE_MODE_331,
  ENGINE_MODE_332,
  ENGINE_MODE_333,
  ENGINE_MODE_334,
  ENGINE_MODE_335,
  ENGINE_MODE_336,
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
      PGE_320 example;
      if (example.Construct (ENGINE_PGE_320_DEFAULT_WIDTH, ENGINE_PGE_320_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_321:
    {
      struct Engine_OpenGL_GLUT_321_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_321_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_321_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_321_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_321_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_321_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_321_DEFAULT_WIDTH, ENGINE_GLUT_321_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 321");
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

      glutDisplayFunc (engine_glut_321_draw);
      glutReshapeFunc (engine_glut_321_reshape);
      glutVisibilityFunc (engine_glut_321_visible);

      glutKeyboardFunc (engine_glut_321_key);
      glutSpecialFunc (engine_glut_321_key_special);
      glutMouseFunc (engine_glut_321_mouse_button);
      glutMotionFunc (engine_glut_321_mouse_move);
      glutPassiveMotionFunc (engine_glut_321_mouse_move);
      glutTimerFunc (100, engine_glut_321_timer, 0);

      glutCreateMenu (engine_glut_321_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_321.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_321.vert")));
        break;
      } // end IF
      GLchar* array_a[2];
      array_a[0] = reinterpret_cast<GLchar*> (data_p);
      array_a[1] = NULL;
      GLint array_2[2];
      array_2[0] = static_cast<GLint> (file_size_i);
      array_2[1] = static_cast<GLint> (NULL);
      glShaderSource (vertexShader_id, 1, array_a, array_2);
      delete [] data_p; data_p = NULL;
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
                    ACE_TEXT ("glut_321.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_321.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_321.frag")));
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
                    ACE_TEXT ("glut_321.frag"),
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
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_322:
    {
      struct Engine_OpenGL_GLUT_322_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_322_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_322_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_322_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
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

      cb_data_s.mouseX = ENGINE_GLUT_322_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_322_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_322_DEFAULT_WIDTH, ENGINE_GLUT_322_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 322");
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

      glutDisplayFunc (engine_glut_322_draw);
      glutReshapeFunc (engine_glut_322_reshape);
      glutVisibilityFunc (engine_glut_322_visible);

      glutKeyboardFunc (engine_glut_322_key);
      glutSpecialFunc (engine_glut_322_key_special);
      glutMouseFunc (engine_glut_322_mouse_button);
      glutMotionFunc (engine_glut_322_mouse_move);
      glutPassiveMotionFunc (engine_glut_322_mouse_move);
      glutTimerFunc (100, engine_glut_322_timer, 0);

      glutCreateMenu (engine_glut_322_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_322.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_322.vert")));
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
                    ACE_TEXT ("glut_322.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_322.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_322.frag")));
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
                    ACE_TEXT ("glut_322.frag"),
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
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_323:
    {
      PGE_323 example;
      if (example.Construct (ENGINE_PGE_323_DEFAULT_WIDTH, ENGINE_PGE_323_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_324:
    {
      PGE_324 example;
      if (example.Construct (ENGINE_PGE_324_DEFAULT_WIDTH, ENGINE_PGE_324_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_325:
    {
      PGE_325 example;
      if (example.Construct (ENGINE_PGE_325_DEFAULT_WIDTH, ENGINE_PGE_325_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_326:
    {
      PGE_326 example;
      if (example.Construct (ENGINE_PGE_326_DEFAULT_WIDTH, ENGINE_PGE_326_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_327:
    {
      struct Engine_OpenGL_GLUT_327_CBData cb_data_s;
      cb_data_s.f = 0;
      cb_data_s.h = 0;

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
      glutInitWindowSize (ENGINE_GLUT_327_DEFAULT_WIDTH, ENGINE_GLUT_327_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 327");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_LIGHTING);
      //glEnable (GL_LIGHT0);
      //glEnable (GL_LIGHT1);
      //glEnable (GL_LIGHT2);
      //glDisable (GL_COLOR_MATERIAL);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //glEnable (GL_NORMALIZE);

      //glEnable (GL_CULL_FACE);
      //glCullFace (GL_FRONT);
      //glFrontFace (GL_CCW);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_327_draw);
      glutReshapeFunc (engine_glut_327_reshape);
      glutVisibilityFunc (engine_glut_327_visible);

      glutKeyboardFunc (engine_glut_327_key);
      glutSpecialFunc (engine_glut_327_key_special);
      glutMouseFunc (engine_glut_327_mouse_button);
      glutMotionFunc (engine_glut_327_mouse_move);
      glutPassiveMotionFunc (engine_glut_327_mouse_move);
      glutTimerFunc (100, engine_glut_327_timer, 0);

      glutCreateMenu (engine_glut_327_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_328:
    {
      struct Engine_OpenGL_GLUT_328_CBData cb_data_s;
      cb_data_s.N = Common_Tools::getRandomNumber (100, 200);
      cb_data_s.hue = Common_Tools::getRandomNumber (0.0f, 360.0f);
      cb_data_s.ofs.first =
        Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
      cb_data_s.ofs.second =
        Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
      cb_data_s.alt[0] = 5.0f;
      cb_data_s.alt[1] = 0.04f;
      cb_data_s.alt[2] = 5.0f;
      cb_data_s.t =
        2.0f * 1.0f / (cb_data_s.alt[0] * cb_data_s.alt[1]) - 1 / static_cast<float> (ENGINE_GLUT_328_DEFAULT_FPS);
      cb_data_s.w =
        static_cast<float> (std::max (ENGINE_GLUT_328_DEFAULT_WIDTH, ENGINE_GLUT_328_DEFAULT_HEIGHT));

      cb_data_s.wireframe = true;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 1200.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_328_DEFAULT_WIDTH, ENGINE_GLUT_328_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 328");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_LIGHTING);
      //glEnable (GL_LIGHT0);
      //glEnable (GL_LIGHT1);
      //glEnable (GL_LIGHT2);
      //glDisable (GL_COLOR_MATERIAL);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //glEnable (GL_NORMALIZE);

      //glEnable (GL_CULL_FACE);
      //glCullFace (GL_FRONT);
      //glFrontFace (GL_CCW);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_328_draw);
      glutReshapeFunc (engine_glut_328_reshape);
      glutVisibilityFunc (engine_glut_328_visible);

      glutKeyboardFunc (engine_glut_328_key);
      glutSpecialFunc (engine_glut_328_key_special);
      glutMouseFunc (engine_glut_328_mouse_button);
      glutMotionFunc (engine_glut_328_mouse_move);
      glutPassiveMotionFunc (engine_glut_328_mouse_move);
      glutTimerFunc (100, engine_glut_328_timer, 0);

      glutCreateMenu (engine_glut_328_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_329:
    {
      struct Engine_OpenGL_GLUT_329_CBData cb_data_s;
      cb_data_s.t = 0.0f;
      cb_data_s.s = 0.0f;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 1200.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_329_DEFAULT_WIDTH, ENGINE_GLUT_329_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 329");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_LIGHTING);
      //glEnable (GL_LIGHT0);
      //glEnable (GL_LIGHT1);
      //glEnable (GL_LIGHT2);
      //glDisable (GL_COLOR_MATERIAL);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //glEnable (GL_NORMALIZE);

      //glEnable (GL_CULL_FACE);
      //glCullFace (GL_FRONT);
      //glFrontFace (GL_CCW);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_329_draw);
      glutReshapeFunc (engine_glut_329_reshape);
      glutVisibilityFunc (engine_glut_329_visible);

      glutKeyboardFunc (engine_glut_329_key);
      glutSpecialFunc (engine_glut_329_key_special);
      glutMouseFunc (engine_glut_329_mouse_button);
      glutMotionFunc (engine_glut_329_mouse_move);
      glutPassiveMotionFunc (engine_glut_329_mouse_move);
      glutTimerFunc (100, engine_glut_329_timer, 0);

      glutCreateMenu (engine_glut_329_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_330:
    {
      struct Engine_OpenGL_GLUT_330_CBData cb_data_s;
      cb_data_s.angle = static_cast<float> (M_PI_2);

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 1200.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.mouseX = ENGINE_GLUT_330_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_330_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_330_DEFAULT_WIDTH, ENGINE_GLUT_330_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 330");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_LIGHTING);
      //glEnable (GL_LIGHT0);
      //glEnable (GL_LIGHT1);
      //glEnable (GL_LIGHT2);
      //glDisable (GL_COLOR_MATERIAL);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //glEnable (GL_NORMALIZE);

      //glEnable (GL_CULL_FACE);
      //glCullFace (GL_FRONT);
      //glFrontFace (GL_CCW);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_330_draw);
      glutReshapeFunc (engine_glut_330_reshape);
      glutVisibilityFunc (engine_glut_330_visible);

      glutKeyboardFunc (engine_glut_330_key);
      glutSpecialFunc (engine_glut_330_key_special);
      glutMouseFunc (engine_glut_330_mouse_button);
      glutMotionFunc (engine_glut_330_mouse_move);
      glutPassiveMotionFunc (engine_glut_330_mouse_move);
      glutTimerFunc (100, engine_glut_330_timer, 0);

      glutCreateMenu (engine_glut_330_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_331:
    {
      struct Engine_OpenGL_GLUT_331_CBData cb_data_s;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 1200.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.mouseX = ENGINE_GLUT_331_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_331_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_331_DEFAULT_WIDTH, ENGINE_GLUT_331_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 331");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      glEnable (GL_LIGHTING);
      glEnable (GL_LIGHT0);
      //glEnable (GL_LIGHT1);
      //glEnable (GL_LIGHT2);
      glDisable (GL_COLOR_MATERIAL);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glEnable (GL_NORMALIZE);

      //glEnable (GL_CULL_FACE);
      //glCullFace (GL_FRONT);
      //glFrontFace (GL_CCW);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_331_draw);
      glutReshapeFunc (engine_glut_331_reshape);
      glutVisibilityFunc (engine_glut_331_visible);

      glutKeyboardFunc (engine_glut_331_key);
      glutSpecialFunc (engine_glut_331_key_special);
      glutMouseFunc (engine_glut_331_mouse_button);
      glutMotionFunc (engine_glut_331_mouse_move);
      glutPassiveMotionFunc (engine_glut_331_mouse_move);
      glutTimerFunc (100, engine_glut_331_timer, 0);

      glutCreateMenu (engine_glut_331_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_332:
    {
      PGE_332 example;
      if (example.Construct (ENGINE_PGE_332_DEFAULT_WIDTH, ENGINE_PGE_332_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_333:
    {
      struct Engine_OpenGL_GLUT_333_CBData cb_data_s;
      cb_data_s.radiusNoise = Common_Tools::getRandomNumber (0.0f, 10.0f);
      cb_data_s.angleStep = 0.0f;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 1200.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.mouseX = ENGINE_GLUT_333_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_333_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_333_DEFAULT_WIDTH, ENGINE_GLUT_333_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 333");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_LIGHTING);
      //glEnable (GL_LIGHT0);
      //glEnable (GL_LIGHT1);
      //glEnable (GL_LIGHT2);
      //glDisable (GL_COLOR_MATERIAL);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //glEnable (GL_NORMALIZE);

      //glEnable (GL_CULL_FACE);
      //glCullFace (GL_FRONT);
      //glFrontFace (GL_CCW);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_333_draw);
      glutReshapeFunc (engine_glut_333_reshape);
      glutVisibilityFunc (engine_glut_333_visible);

      glutKeyboardFunc (engine_glut_333_key);
      glutSpecialFunc (engine_glut_333_key_special);
      glutMouseFunc (engine_glut_333_mouse_button);
      glutMotionFunc (engine_glut_333_mouse_move);
      glutPassiveMotionFunc (engine_glut_333_mouse_move);
      glutTimerFunc (100, engine_glut_333_timer, 0);

      glutCreateMenu (engine_glut_333_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_334:
    {
      PGE_334 example;
      if (example.Construct (ENGINE_PGE_334_DEFAULT_WIDTH, ENGINE_PGE_334_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_335:
    {
      struct Engine_OpenGL_GLUT_335_CBData cb_data_s;
      cb_data_s.cylinderR = ENGINE_GLUT_335_DEFAULT_WIDTH * 0.25f;
      cb_data_s.cylinderY = ENGINE_GLUT_335_DEFAULT_WIDTH * 0.9f;
      cb_data_s.torusR = ENGINE_GLUT_335_DEFAULT_WIDTH * 0.2f;
      cb_data_s.colH = Common_Tools::getRandomNumber (0.0f, 360.0f);
      cb_data_s.direction = Common_Tools::testRandomProbability (0.5f) ? -1.0f : 1.0f;
      for (int i = 0; i < ENGINE_GLUT_335_DEFAULT_NUMBER_OF_CUBES; i++)
        cb_data_s.cubes.push_back (cube (ENGINE_GLUT_335_DEFAULT_NUMBER_OF_TILE_ANGLES, ENGINE_GLUT_335_DEFAULT_NUMBER_OF_TILE_Y, cb_data_s.cylinderR, cb_data_s.cylinderY, i, cb_data_s.torusR));
      for (int j = 0; j < ENGINE_GLUT_335_DEFAULT_NUMBER_OF_CUBES; j++)
      {
        std::vector<std::vector<float> > temp_s;
        cb_data_s.initNoiseXYZ.push_back (temp_s);
        for (int i = 0; i < 4; i++)
        {
          std::vector<float> temp_2;
          cb_data_s.initNoiseXYZ[j].push_back (temp_2);
          for (int k = 0; k < ENGINE_GLUT_335_DEFAULT_NOISE_DIMENSION; k++)
            cb_data_s.initNoiseXYZ[j][i].push_back (Common_Tools::getRandomNumber (0.0f, 100.0f));
        } // end FOR
      } // end FOR
      cb_data_s.noiseRangeXYZ.push_back (2.0f);
      cb_data_s.noiseRangeXYZ.push_back (1.25f);
      cb_data_s.noiseRangeXYZ.push_back (2.0f);
      for (int i = 0; i < 3; i++)
        cb_data_s.initRot.push_back (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)));
      cb_data_s.count = 0;

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

      cb_data_s.mouseX = ENGINE_GLUT_333_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_333_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_335_DEFAULT_WIDTH, ENGINE_GLUT_335_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 335");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_LIGHTING);
      //glEnable (GL_LIGHT0);
      //glEnable (GL_LIGHT1);
      //glEnable (GL_LIGHT2);
      //glDisable (GL_COLOR_MATERIAL);

      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //glEnable (GL_NORMALIZE);

      //glEnable (GL_CULL_FACE);
      //glCullFace (GL_FRONT);
      //glFrontFace (GL_CCW);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_335_draw);
      glutReshapeFunc (engine_glut_335_reshape);
      glutVisibilityFunc (engine_glut_335_visible);

      glutKeyboardFunc (engine_glut_335_key);
      glutSpecialFunc (engine_glut_335_key_special);
      glutMouseFunc (engine_glut_335_mouse_button);
      glutMotionFunc (engine_glut_335_mouse_move);
      glutPassiveMotionFunc (engine_glut_335_mouse_move);
      glutTimerFunc (100, engine_glut_335_timer, 0);

      glutCreateMenu (engine_glut_335_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_336:
    {
      PGE_336 example;
      if (example.Construct (ENGINE_PGE_336_DEFAULT_WIDTH, ENGINE_PGE_336_DEFAULT_HEIGHT,
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
