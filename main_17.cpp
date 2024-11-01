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
#include "defines_17.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "glut_360.h"
#include "pge_361.h"
#include "pge_362.h"
#include "glut_363.h"
#include "pge_364.h"
#include "pge_365.h"
#include "glut_366.h"
#include "pge_367.h"
#include "pge_368.h"
#include "glut_369.h"
#include "glut_370.h"
#include "glut_371.h"
#include "pge_372.h"
#include "glut_373.h"
#include "glut_374.h"
#include "glut_375.h"
#include "pge_376.h"
#include "glut_377.h"
#include "pge_378.h"
#include "glut_379.h"
#include "pge_380.h"
#include "pge_381.h"
#include "glut_382.h"
#include "glut_383.h"
#include "pge_384.h"
#include "pge_385.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 360,
  ENGINE_MODE_361,
  ENGINE_MODE_362,
  ENGINE_MODE_363,
  ENGINE_MODE_364,
  ENGINE_MODE_365,
  ENGINE_MODE_366,
  ENGINE_MODE_367,
  ENGINE_MODE_368,
  ENGINE_MODE_369,
  ENGINE_MODE_370,
  ENGINE_MODE_371,
  ENGINE_MODE_372,
  ENGINE_MODE_373,
  ENGINE_MODE_374,
  ENGINE_MODE_375,
  ENGINE_MODE_376,
  ENGINE_MODE_377,
  ENGINE_MODE_378,
  ENGINE_MODE_379,
  ENGINE_MODE_380,
  ENGINE_MODE_381,
  ENGINE_MODE_382,
  ENGINE_MODE_383,
  ENGINE_MODE_384,
  ENGINE_MODE_385,
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
      struct Engine_OpenGL_GLUT_360_CBData cb_data_s;
      cb_data_s.zPos = 0.0f;
      cb_data_s.boxes.assign (400, platform ());      
      int counter = 0;
      for (int i = 12; i <= ENGINE_GLUT_360_DEFAULT_WIDTH - 12; i += 24)
        for (int j = 12; j <= ENGINE_GLUT_360_DEFAULT_HEIGHT - 12; j += 24)
        {
          cb_data_s.boxes[counter].position (static_cast<float> (i), static_cast<float> (j));
          ++counter;
        } // end FOR

      // *TODO*: something seems to be wrong with the depth testing of the box()
      //         implementation. This looks better for now
      cb_data_s.wireframe = true;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 400.0f;
      cb_data_s.camera.position.z = 750.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.mousex = ENGINE_GLUT_360_DEFAULT_WIDTH / 2;
      cb_data_s.mousey = ENGINE_GLUT_360_DEFAULT_HEIGHT / 2;
      cb_data_s.mouse_0_ButtonDown = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_360_DEFAULT_WIDTH, ENGINE_GLUT_360_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 360");
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

      glEnable (GL_NORMALIZE);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_360_draw);
      glutReshapeFunc (engine_glut_360_reshape);
      glutVisibilityFunc (engine_glut_360_visible);

      glutKeyboardFunc (engine_glut_360_key);
      glutSpecialFunc (engine_glut_360_key_special);
      glutMouseFunc (engine_glut_360_mouse_button);
      glutMotionFunc (engine_glut_360_mouse_move);
      glutPassiveMotionFunc (engine_glut_360_mouse_move);
      glutTimerFunc (100, engine_glut_360_timer, 0);

      glutCreateMenu (engine_glut_360_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_361:
    {
      PGE_361 example;
      if (example.Construct (ENGINE_PGE_361_DEFAULT_WIDTH, ENGINE_PGE_361_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_362:
    {
      PGE_362 example;
      if (example.Construct (ENGINE_PGE_362_DEFAULT_WIDTH, ENGINE_PGE_362_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_363:
    {
      struct Engine_OpenGL_GLUT_363_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_363_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_363_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_363_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_363_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_363_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_363_DEFAULT_WIDTH, ENGINE_GLUT_363_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 363");
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

      glutDisplayFunc (engine_glut_363_draw);
      glutReshapeFunc (engine_glut_363_reshape);
      glutVisibilityFunc (engine_glut_363_visible);

      glutKeyboardFunc (engine_glut_363_key);
      glutSpecialFunc (engine_glut_363_key_special);
      glutMouseFunc (engine_glut_363_mouse_button);
      glutMotionFunc (engine_glut_363_mouse_move);
      glutPassiveMotionFunc (engine_glut_363_mouse_move);
      glutTimerFunc (100, engine_glut_363_timer, 0);

      glutCreateMenu (engine_glut_363_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_363.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_363.vert")));
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
                    ACE_TEXT ("glut_363.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_363.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_363.frag")));
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
                    ACE_TEXT ("glut_363.frag"),
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
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_mouse"));
      ACE_ASSERT (cb_data_s.mouseLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_364:
    {
      PGE_364 example;
      if (example.Construct (ENGINE_PGE_364_DEFAULT_WIDTH, ENGINE_PGE_364_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_365:
    {
      PGE_365 example;
      if (example.Construct (ENGINE_PGE_365_DEFAULT_WIDTH, ENGINE_PGE_365_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_366:
    {
      struct Engine_OpenGL_GLUT_366_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_366_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_366_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_366_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_366_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_366_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_366_DEFAULT_WIDTH, ENGINE_GLUT_366_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 366");
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

      glutDisplayFunc (engine_glut_366_draw);
      glutReshapeFunc (engine_glut_366_reshape);
      glutVisibilityFunc (engine_glut_366_visible);

      glutKeyboardFunc (engine_glut_366_key);
      glutSpecialFunc (engine_glut_366_key_special);
      glutMouseFunc (engine_glut_366_mouse_button);
      glutMotionFunc (engine_glut_366_mouse_move);
      glutPassiveMotionFunc (engine_glut_366_mouse_move);
      glutTimerFunc (100, engine_glut_366_timer, 0);

      glutCreateMenu (engine_glut_366_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_366.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_366.vert")));
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
                    ACE_TEXT ("glut_366.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_366.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_366.frag")));
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
                    ACE_TEXT ("glut_366.frag"),
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
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_367:
    {
      PGE_367 example;
      if (example.Construct (ENGINE_PGE_367_DEFAULT_WIDTH, ENGINE_PGE_367_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_368:
    {
      PGE_368 example;
      if (example.Construct (ENGINE_PGE_368_DEFAULT_WIDTH, ENGINE_PGE_368_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_369:
    {
      struct Engine_OpenGL_GLUT_369_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_369_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_369_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_369_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
      cb_data_s.resolutionLoc = -1;
      cb_data_s.trailCountLoc = -1;
      cb_data_s.trailLoc = -1;
      cb_data_s.particleCountLoc = -1;
      cb_data_s.particlesLoc = -1;
      cb_data_s.colorsLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_369_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_369_DEFAULT_HEIGHT / 2;
      cb_data_s.pmouseX = ENGINE_GLUT_369_DEFAULT_WIDTH / 2;
      cb_data_s.pmouseY = ENGINE_GLUT_369_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseCenterPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_369_DEFAULT_WIDTH, ENGINE_GLUT_369_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 369");
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

      glutDisplayFunc (engine_glut_369_draw);
      glutReshapeFunc (engine_glut_369_reshape);
      glutVisibilityFunc (engine_glut_369_visible);

      glutKeyboardFunc (engine_glut_369_key);
      glutSpecialFunc (engine_glut_369_key_special);
      glutMouseFunc (engine_glut_369_mouse_button);
      glutMotionFunc (engine_glut_369_mouse_move);
      glutPassiveMotionFunc (engine_glut_369_mouse_move);
      glutTimerFunc (100, engine_glut_369_timer, 0);

      glutCreateMenu (engine_glut_369_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_369.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_369.vert")));
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
                    ACE_TEXT ("glut_369.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_369.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_369.frag")));
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
                    ACE_TEXT ("glut_369.frag"),
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
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.trailCountLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("trailCount"));
      ACE_ASSERT (cb_data_s.trailCountLoc != -1);
      cb_data_s.trailLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("trail"));
      ACE_ASSERT (cb_data_s.trailLoc != -1);
      cb_data_s.particleCountLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("particleCount"));
      ACE_ASSERT (cb_data_s.particleCountLoc != -1);
      cb_data_s.particlesLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("particles"));
      ACE_ASSERT (cb_data_s.particlesLoc != -1);
      cb_data_s.colorsLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("colors"));
      ACE_ASSERT (cb_data_s.colorsLoc != -1);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_370:
    {
      struct Engine_OpenGL_GLUT_370_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_370_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_370_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_370_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
      cb_data_s.resolutionLoc = -1;
      cb_data_s.particlesLoc = -1;
      cb_data_s.particleCountLoc = -1;
      cb_data_s.particleColorsLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_370_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_370_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_370_DEFAULT_WIDTH, ENGINE_GLUT_370_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 370");
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

      glutDisplayFunc (engine_glut_370_draw);
      glutReshapeFunc (engine_glut_370_reshape);
      glutVisibilityFunc (engine_glut_370_visible);

      glutKeyboardFunc (engine_glut_370_key);
      glutSpecialFunc (engine_glut_370_key_special);
      glutMouseFunc (engine_glut_370_mouse_button);
      glutMotionFunc (engine_glut_370_mouse_move);
      glutPassiveMotionFunc (engine_glut_370_mouse_move);
      glutTimerFunc (100, engine_glut_370_timer, 0);

      glutCreateMenu (engine_glut_370_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_370.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_370.vert")));
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
                    ACE_TEXT ("glut_370.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_370.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_370.frag")));
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
                    ACE_TEXT ("glut_370.frag"),
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
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.particlesLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("particles"));
      ACE_ASSERT (cb_data_s.particlesLoc != -1);
      cb_data_s.particleCountLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("particleCount"));
      ACE_ASSERT (cb_data_s.particleCountLoc != -1);
      cb_data_s.particleColorsLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("particleColors"));
      ACE_ASSERT (cb_data_s.particleColorsLoc != -1);

      static Common_GL_Color_t colorScheme1[1] = { Common_GL_Color_t (0x47, 0xCB, 0xB0) };
      static Common_GL_Color_t colorScheme2[1] = { Common_GL_Color_t (0xFF, 0xA9, 0x10) };
      static Common_GL_Color_t colorScheme3[1] = { Common_GL_Color_t (0x31, 0x1A, 0xE7) };

      int index_i = Common_Tools::getRandomNumber (0, 0);
      Common_GL_Color_t c1 = colorScheme1[index_i];
      index_i = Common_Tools::getRandomNumber (0, 0);
      Common_GL_Color_t c2 = colorScheme2[index_i];
      index_i = Common_Tools::getRandomNumber (0, 0);
      Common_GL_Color_t c3 = colorScheme3[index_i];

      for (int i = 0; i < ENGINE_GLUT_370_DEFAULT_MAX_PARTICLE_COUNT; i++)
      {
        cb_data_s.particles.push_back (particle_2 (ENGINE_GLUT_370_DEFAULT_WIDTH / 2, ENGINE_GLUT_370_DEFAULT_HEIGHT / 2, c1));
        cb_data_s.particles.push_back (particle_2 (ENGINE_GLUT_370_DEFAULT_WIDTH / 2, ENGINE_GLUT_370_DEFAULT_HEIGHT / 2, c2));
        cb_data_s.particles.push_back (particle_2 (ENGINE_GLUT_370_DEFAULT_WIDTH / 2, ENGINE_GLUT_370_DEFAULT_HEIGHT / 2, c3));
      } // end FOR

      cb_data_s.particleGroup.initialize (cb_data_s.particles);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_371:
    {
      struct Engine_OpenGL_GLUT_371_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_371_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_371_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_371_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
      cb_data_s.resolutionLoc = -1;
      cb_data_s.framesLoc = -1;
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

      cb_data_s.mouseX = ENGINE_GLUT_371_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_371_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_371_DEFAULT_WIDTH, ENGINE_GLUT_371_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 371");
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

      glutDisplayFunc (engine_glut_371_draw);
      glutReshapeFunc (engine_glut_371_reshape);
      glutVisibilityFunc (engine_glut_371_visible);

      glutKeyboardFunc (engine_glut_371_key);
      glutSpecialFunc (engine_glut_371_key_special);
      glutMouseFunc (engine_glut_371_mouse_button);
      glutMotionFunc (engine_glut_371_mouse_move);
      glutPassiveMotionFunc (engine_glut_371_mouse_move);
      glutTimerFunc (100, engine_glut_371_timer, 0);

      glutCreateMenu (engine_glut_371_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_371.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_371.vert")));
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
                    ACE_TEXT ("glut_371.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_371.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_371.frag")));
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
                    ACE_TEXT ("glut_371.frag"),
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
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.framesLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_frame"));
      ACE_ASSERT (cb_data_s.framesLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_mouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_372:
    {
      PGE_372 example;
      if (example.Construct (ENGINE_PGE_372_DEFAULT_WIDTH, ENGINE_PGE_372_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_373:
    {
      struct Engine_OpenGL_GLUT_373_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_373_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_373_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_373_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
      cb_data_s.resolutionLoc = -1;
      cb_data_s.mouseLoc = -1;
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

      cb_data_s.mouseX = ENGINE_GLUT_373_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_373_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_373_DEFAULT_WIDTH, ENGINE_GLUT_373_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 373");
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

      glutDisplayFunc (engine_glut_373_draw);
      glutReshapeFunc (engine_glut_373_reshape);
      glutVisibilityFunc (engine_glut_373_visible);

      glutKeyboardFunc (engine_glut_373_key);
      glutSpecialFunc (engine_glut_373_key_special);
      glutMouseFunc (engine_glut_373_mouse_button);
      glutMotionFunc (engine_glut_373_mouse_move);
      glutPassiveMotionFunc (engine_glut_373_mouse_move);
      glutTimerFunc (100, engine_glut_373_timer, 0);

      glutCreateMenu (engine_glut_373_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_373.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_373.vert")));
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
                    ACE_TEXT ("glut_373.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_373.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_373.frag")));
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
                    ACE_TEXT ("glut_373.frag"),
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
      //ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("mouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("time"));
      //ACE_ASSERT (cb_data_s.timeLoc != -1);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_374:
    {
      struct Engine_OpenGL_GLUT_374_CBData cb_data_s;
      cb_data_s.octaves = 1;

      cb_data_s.scaleFactor = ENGINE_GLUT_374_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_374_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_374_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.octavesLoc = -1;

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

      cb_data_s.mouseX = ENGINE_GLUT_374_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_374_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_374_DEFAULT_WIDTH, ENGINE_GLUT_374_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 374");
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

      glutDisplayFunc (engine_glut_374_draw);
      glutReshapeFunc (engine_glut_374_reshape);
      glutVisibilityFunc (engine_glut_374_visible);

      glutKeyboardFunc (engine_glut_374_key);
      glutSpecialFunc (engine_glut_374_key_special);
      glutMouseFunc (engine_glut_374_mouse_button);
      glutMotionFunc (engine_glut_374_mouse_move);
      glutPassiveMotionFunc (engine_glut_374_mouse_move);
      glutTimerFunc (100, engine_glut_374_timer, 0);

      glutCreateMenu (engine_glut_374_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_374.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_374.vert")));
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
                    ACE_TEXT ("glut_374.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_374.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_374.frag")));
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
                    ACE_TEXT ("glut_374.frag"),
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
      cb_data_s.octavesLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("noctaves"));
      ACE_ASSERT (cb_data_s.octavesLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_375:
    {
      struct Engine_OpenGL_GLUT_375_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_375_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_375_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_375_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
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

      cb_data_s.mouseX = ENGINE_GLUT_375_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_375_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_375_DEFAULT_WIDTH, ENGINE_GLUT_375_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 375");
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

      glutDisplayFunc (engine_glut_375_draw);
      glutReshapeFunc (engine_glut_375_reshape);
      glutVisibilityFunc (engine_glut_375_visible);

      glutKeyboardFunc (engine_glut_375_key);
      glutSpecialFunc (engine_glut_375_key_special);
      glutMouseFunc (engine_glut_375_mouse_button);
      glutMotionFunc (engine_glut_375_mouse_move);
      glutPassiveMotionFunc (engine_glut_375_mouse_move);
      glutTimerFunc (100, engine_glut_375_timer, 0);

      glutCreateMenu (engine_glut_375_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_375.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_375.vert")));
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
                    ACE_TEXT ("glut_375.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_375.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_375.frag")));
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
                    ACE_TEXT ("glut_375.frag"),
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
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      cb_data_s.frameLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_frame"));
      //ACE_ASSERT (cb_data_s.frameLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_mouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_376:
    {
      PGE_376 example;
      if (example.Construct (ENGINE_PGE_376_DEFAULT_WIDTH, ENGINE_PGE_376_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_377:
    {
      struct Engine_OpenGL_GLUT_377_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_377_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_377_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_377_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_377_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_377_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_377_DEFAULT_WIDTH, ENGINE_GLUT_377_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 377");
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

      glutDisplayFunc (engine_glut_377_draw);
      glutReshapeFunc (engine_glut_377_reshape);
      glutVisibilityFunc (engine_glut_377_visible);

      glutKeyboardFunc (engine_glut_377_key);
      glutSpecialFunc (engine_glut_377_key_special);
      glutMouseFunc (engine_glut_377_mouse_button);
      glutMotionFunc (engine_glut_377_mouse_move);
      glutPassiveMotionFunc (engine_glut_377_mouse_move);
      glutTimerFunc (100, engine_glut_377_timer, 0);

      glutCreateMenu (engine_glut_377_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_377.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_377.vert")));
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
                    ACE_TEXT ("glut_377.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_377.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_377.frag")));
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
                    ACE_TEXT ("glut_377.frag"),
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
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_resolution"));
      ACE_ASSERT (cb_data_s.resolutionLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_time"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("u_mouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_378:
    {
      PGE_378 example;
      if (example.Construct (ENGINE_PGE_378_DEFAULT_WIDTH, ENGINE_PGE_378_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_379:
    {
      struct Engine_OpenGL_GLUT_379_CBData cb_data_s;

      cb_data_s.columns = ENGINE_GLUT_379_DEFAULT_WIDTH / 2;
      cb_data_s.rows = ENGINE_GLUT_379_DEFAULT_HEIGHT / 2;

      cb_data_s.programId = -1;
      cb_data_s.positionLoc = -1;
      cb_data_s.texCoordLoc = -1;
      cb_data_s.randomVecLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.time = 0.0f;

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

      cb_data_s.mouseX = ENGINE_GLUT_379_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_379_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_379_DEFAULT_WIDTH, ENGINE_GLUT_379_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 379");
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

      glClearColor (0.0f, 0.0f, 0.0f, 0.1f);

      glutDisplayFunc (engine_glut_379_draw);
      glutReshapeFunc (engine_glut_379_reshape);
      glutVisibilityFunc (engine_glut_379_visible);

      glutKeyboardFunc (engine_glut_379_key);
      glutSpecialFunc (engine_glut_379_key_special);
      glutMouseFunc (engine_glut_379_mouse_button);
      glutMotionFunc (engine_glut_379_mouse_move);
      glutPassiveMotionFunc (engine_glut_379_mouse_move);
      //glutTimerFunc (100, engine_glut_379_timer, 0);

      glutCreateMenu (engine_glut_379_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_379.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_379.vert")));
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
                    ACE_TEXT ("glut_379.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_379.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_379.frag")));
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
                    ACE_TEXT ("glut_379.frag"),
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

      cb_data_s.positionLoc =
        glGetAttribLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("aPosition"));
      ACE_ASSERT (cb_data_s.positionLoc != -1);
      cb_data_s.texCoordLoc =
        glGetAttribLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("aTexCoord"));
      //ACE_ASSERT (cb_data_s.texCoordLoc != -1);
      cb_data_s.randomVecLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("uRandomVec2"));
      ACE_ASSERT (cb_data_s.randomVecLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("uTime"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);


      glGenVertexArrays(1, &cb_data_s.VAO);
      glBindVertexArray (cb_data_s.VAO);
      glGenBuffers (1, &cb_data_s.VBO);
      glBindBuffer (GL_ARRAY_BUFFER, cb_data_s.VBO);

      std::vector<float> positionTexCoordData;
      for (int col = 0; col < cb_data_s.columns; col++)
        for (int row = 0; row < cb_data_s.rows; row++)
        {
          positionTexCoordData.push_back (-1.0f + (2.0f / static_cast<float> (cb_data_s.columns)) * col + 1.0f / static_cast<float> (cb_data_s.columns));
          positionTexCoordData.push_back ( 1.0f - (2.0f / static_cast<float> (cb_data_s.rows))    * row - 1.0f / static_cast<float> (cb_data_s.rows));
          positionTexCoordData.push_back ((col + 1.0f / static_cast<float> (cb_data_s.columns)) * (1.0f / static_cast<float> (cb_data_s.columns)));
          positionTexCoordData.push_back ((row + 1.0f / static_cast<float> (cb_data_s.rows)) *    (1.0f / static_cast<float> (cb_data_s.rows)));
        } // end FOR
      glBufferData (GL_ARRAY_BUFFER, cb_data_s.columns * cb_data_s.rows * 4 * sizeof (float), positionTexCoordData.data (), GL_STATIC_DRAW);

      glEnableVertexAttribArray (cb_data_s.positionLoc); // Enable attribute first before specifiying attributes
      glVertexAttribPointer (cb_data_s.positionLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (float), 0);
      //glEnableVertexAttribArray (cb_data_s.texCoordLoc);
      //glVertexAttribPointer (cb_data_s.texCoordLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (float), (void*)(2 * sizeof (float))); // 2 offset from beginning

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_380:
    {
      PGE_380 example;
      if (example.Construct (ENGINE_PGE_380_DEFAULT_WIDTH, ENGINE_PGE_380_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_381:
    {
      PGE_381 example;
      if (example.Construct (ENGINE_PGE_381_DEFAULT_WIDTH, ENGINE_PGE_381_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_382:
    {
      struct Engine_OpenGL_GLUT_382_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_382_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_382_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_382_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_382_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_382_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_382_DEFAULT_WIDTH, ENGINE_GLUT_382_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 382");
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

      glutDisplayFunc (engine_glut_382_draw);
      glutReshapeFunc (engine_glut_382_reshape);
      glutVisibilityFunc (engine_glut_382_visible);

      glutKeyboardFunc (engine_glut_382_key);
      glutSpecialFunc (engine_glut_382_key_special);
      glutMouseFunc (engine_glut_382_mouse_button);
      glutMotionFunc (engine_glut_382_mouse_move);
      glutPassiveMotionFunc (engine_glut_382_mouse_move);
      glutTimerFunc (100, engine_glut_382_timer, 0);

      glutCreateMenu (engine_glut_382_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_382.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_382.vert")));
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
                    ACE_TEXT ("glut_382.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_382.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_382.frag")));
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
                    ACE_TEXT ("glut_382.frag"),
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
    case ENGINE_MODE_383:
    {
      struct Engine_OpenGL_GLUT_383_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_383_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_383_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_383_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      cb_data_s.mouseX = ENGINE_GLUT_383_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_383_DEFAULT_HEIGHT / 2;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_383_DEFAULT_WIDTH, ENGINE_GLUT_383_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 383");
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

      glutDisplayFunc (engine_glut_383_draw);
      glutReshapeFunc (engine_glut_383_reshape);
      glutVisibilityFunc (engine_glut_383_visible);

      glutKeyboardFunc (engine_glut_383_key);
      glutSpecialFunc (engine_glut_383_key_special);
      glutMouseFunc (engine_glut_383_mouse_button);
      glutMotionFunc (engine_glut_383_mouse_move);
      glutPassiveMotionFunc (engine_glut_383_mouse_move);
      glutTimerFunc (100, engine_glut_383_timer, 0);

      glutCreateMenu (engine_glut_383_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_383.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_383.vert")));
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
                    ACE_TEXT ("glut_383.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_383.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_383.frag")));
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
                    ACE_TEXT ("glut_383.frag"),
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
    case ENGINE_MODE_384:
    {
      PGE_384 example;
      if (example.Construct (ENGINE_PGE_384_DEFAULT_WIDTH, ENGINE_PGE_384_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_385:
    {
      PGE_385 example;
      if (example.Construct (ENGINE_PGE_385_DEFAULT_WIDTH, ENGINE_PGE_385_DEFAULT_HEIGHT,
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
