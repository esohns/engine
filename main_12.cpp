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
#include "engine_config.h"
#endif // HAVE_CONFIG_H
//#include "resource.h"

#include "defines.h"
#include "defines_12.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "pge_260.h"
#include "pge_261.h"
#include "pge_262.h"
#include "pge_263.h"
#include "glut_264.h"
#include "pge_265.h"
#include "pge_266.h"
#include "glut_267.h"
#include "pge_268.h"
#include "glut_269.h"
#include "pge_270.h"
#include "glut_271.h"
#include "glut_272.h"
#include "glut_273.h"
#include "glut_274.h"
#include "glut_275.h"
#include "pge_276.h"
#include "glut_277.h"
#include "glut_278.h"
#include "glut_279.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 260,
  ENGINE_MODE_261,
  ENGINE_MODE_262,
  ENGINE_MODE_263,
  ENGINE_MODE_264,
  ENGINE_MODE_265,
  ENGINE_MODE_266,
  ENGINE_MODE_267,
  ENGINE_MODE_268,
  ENGINE_MODE_269,
  ENGINE_MODE_270,
  ENGINE_MODE_271,
  ENGINE_MODE_272,
  ENGINE_MODE_273,
  ENGINE_MODE_274,
  ENGINE_MODE_275,
  ENGINE_MODE_276,
  ENGINE_MODE_277,
  ENGINE_MODE_278,
  ENGINE_MODE_279,
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
      PGE_260 example;
      if (example.Construct (ENGINE_PGE_260_DEFAULT_WIDTH, ENGINE_PGE_260_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_261:
    {
      PGE_261 example;
      if (example.Construct (ENGINE_PGE_261_DEFAULT_WIDTH, ENGINE_PGE_261_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_262:
    {
      PGE_262 example;
      if (example.Construct (ENGINE_PGE_262_DEFAULT_WIDTH, ENGINE_PGE_262_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_263:
    {
      PGE_263 example;
      if (example.Construct (ENGINE_PGE_263_DEFAULT_WIDTH, ENGINE_PGE_263_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_264:
    {
      struct Engine_OpenGL_GLUT_264_CBData cb_data_s;
      cb_data_s.time = 0.0f;

      cb_data_s.wireframe = true;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 1500.0f;
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
      glutInitWindowSize (ENGINE_GLUT_264_DEFAULT_WIDTH, ENGINE_GLUT_264_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 264");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_264_draw);
      glutReshapeFunc (engine_glut_264_reshape);
      glutVisibilityFunc (engine_glut_264_visible);

      glutKeyboardFunc (engine_glut_264_key);
      glutSpecialFunc (engine_glut_264_key_special);
      glutMouseFunc (engine_glut_264_mouse_button);
      glutMotionFunc (engine_glut_264_mouse_move);
      glutPassiveMotionFunc (engine_glut_264_mouse_move);
      glutTimerFunc (100, engine_glut_264_timer, 0);

      glutCreateMenu (engine_glut_264_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_265:
    {
      PGE_265 example;
      if (example.Construct (ENGINE_PGE_265_DEFAULT_WIDTH, ENGINE_PGE_265_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_266:
    {
      PGE_266 example;
      if (example.Construct (ENGINE_PGE_266_DEFAULT_WIDTH, ENGINE_PGE_266_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_267:
    {
      struct Engine_OpenGL_GLUT_267_CBData cb_data_s;
      cb_data_s.f = 0.0f;

      cb_data_s.wireframe = true;

      cb_data_s.camera.position.x = -400.0f;
      cb_data_s.camera.position.y = 200.0f;
      cb_data_s.camera.position.z = -800.0f;
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
      glutInitWindowSize (ENGINE_GLUT_267_DEFAULT_WIDTH, ENGINE_GLUT_267_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 267");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      glEnable (GL_LIGHTING);
      glEnable (GL_LIGHT0);
      glEnable (GL_LIGHT1);
      glEnable (GL_LIGHT2);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_267_draw);
      glutReshapeFunc (engine_glut_267_reshape);
      glutVisibilityFunc (engine_glut_267_visible);

      glutKeyboardFunc (engine_glut_267_key);
      glutSpecialFunc (engine_glut_267_key_special);
      glutMouseFunc (engine_glut_267_mouse_button);
      glutMotionFunc (engine_glut_267_mouse_move);
      glutPassiveMotionFunc (engine_glut_267_mouse_move);
      glutTimerFunc (100, engine_glut_267_timer, 0);

      glutCreateMenu (engine_glut_267_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_268:
    {
      PGE_268 example;
      if (example.Construct (ENGINE_PGE_268_DEFAULT_WIDTH, ENGINE_PGE_268_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_269:
    {
      struct Engine_OpenGL_GLUT_269_CBData cb_data_s;
      cb_data_s.scaleFactor = ENGINE_GLUT_269_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_269_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_269_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
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

      //cb_data_s.leftButtonIsDown = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_269_DEFAULT_WIDTH, ENGINE_GLUT_269_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 269");
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

      glutDisplayFunc (engine_glut_269_draw);
      glutReshapeFunc (engine_glut_269_reshape);
      glutVisibilityFunc (engine_glut_269_visible);

      glutKeyboardFunc (engine_glut_269_key);
      glutSpecialFunc (engine_glut_269_key_special);
      glutMouseFunc (engine_glut_269_mouse_button);
      glutMotionFunc (engine_glut_269_mouse_move);
      glutPassiveMotionFunc (engine_glut_269_mouse_move);
      glutTimerFunc (100, engine_glut_269_timer, 0);

      glutCreateMenu (engine_glut_269_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_269.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_269.vert")));
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
                    ACE_TEXT ("glut_269.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_269.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_269.frag")));
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
                    ACE_TEXT ("glut_269.frag"),
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
      cb_data_s.whirlsLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("whirls"));
      ACE_ASSERT (cb_data_s.whirlsLoc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glm::vec2 lastP (-5.0f, -5.0f);
      for (int i = 0; i < ENGINE_GLUT_269_DEFAULT_NUMBER_OF_WHIRLS; i++)
      {
        glm::vec2 p (Common_Tools::getRandomNumber (0.0f, 1.0f),
                     Common_Tools::getRandomNumber (0.0f, 1.0f));
        // update random position if it's too close to the previous ball
        while (glm::distance (p, lastP) < 0.3f)
        {
          p.x = Common_Tools::getRandomNumber (0.0f, 1.0f);
          p.y = Common_Tools::getRandomNumber (0.0f, 1.0f);
        } // end WHILE

        cb_data_s.whirls.push_back (whirl (p));
        lastP = p;
      } // end FOR

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_270:
    {
      PGE_270 example;
      if (example.Construct (ENGINE_PGE_270_DEFAULT_WIDTH, ENGINE_PGE_270_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_271:
    {
      struct Engine_OpenGL_GLUT_271_CBData cb_data_s;
      cb_data_s.maxAreaR = ENGINE_GLUT_271_DEFAULT_WIDTH / 2.5f;

      float maxSphereR = ENGINE_GLUT_271_DEFAULT_WIDTH / 5.0f;
      for (int i = 0; i < ENGINE_GLUT_271_DEFAULT_NUMBER_OF_SPHERES; i++)
      {
        cb_data_s.aryR.push_back (maxSphereR);

        float maxCentX = cb_data_s.maxAreaR - cb_data_s.aryR[i];
        float maxCentY = cb_data_s.maxAreaR - cb_data_s.aryR[i];
        float maxCentZ = cb_data_s.maxAreaR - cb_data_s.aryR[i];
        glm::vec3 sphere_center_s (maxCentX * Common_Tools::getRandomNumber (-1.0f, 1.0f),
                                   maxCentY * Common_Tools::getRandomNumber (-1.0f, 1.0f),
                                   maxCentZ * Common_Tools::getRandomNumber (-1.0f, 1.0f));
        cb_data_s.arySphereCenter.push_back (sphere_center_s);

        base_triangle (cb_data_s.aryR[i], i, cb_data_s.arySphereCenter[i], cb_data_s);
      } // end FOR

      cb_data_s.initNoiseXYZ.x = Common_Tools::getRandomNumber (0.0f, 100.0f);
      cb_data_s.initNoiseXYZ.y = Common_Tools::getRandomNumber (0.0f, 100.0f);
      cb_data_s.initNoiseXYZ.z = Common_Tools::getRandomNumber (0.0f, 100.0f);
      cb_data_s.initRot.x = Common_Tools::getRandomNumber (0.0f, 360.0f);
      cb_data_s.initRot.y = Common_Tools::getRandomNumber (0.0f, 360.0f);
      cb_data_s.initRot.z = Common_Tools::getRandomNumber (0.0f, 360.0f);
      cb_data_s.noiseRangeXYZ.x = 1.0f / 1.5f / 2.0f;
      cb_data_s.noiseRangeXYZ.y = 1.0f / 1.5f / 2.0f;
      cb_data_s.noiseRangeXYZ.z = 1.0f / 1.5f / 2.0f;

      cb_data_s.sphereR = ENGINE_GLUT_271_DEFAULT_WIDTH / 1000.0f;

      cb_data_s.count = 0;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 1500.0f;
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
      glutInitWindowSize (ENGINE_GLUT_271_DEFAULT_WIDTH, ENGINE_GLUT_271_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 271");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_271_draw);
      glutReshapeFunc (engine_glut_271_reshape);
      glutVisibilityFunc (engine_glut_271_visible);

      glutKeyboardFunc (engine_glut_271_key);
      glutSpecialFunc (engine_glut_271_key_special);
      glutMouseFunc (engine_glut_271_mouse_button);
      glutMotionFunc (engine_glut_271_mouse_move);
      glutPassiveMotionFunc (engine_glut_271_mouse_move);
      glutTimerFunc (100, engine_glut_271_timer, 0);

      glutCreateMenu (engine_glut_271_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_272:
    {
      struct Engine_OpenGL_GLUT_272_CBData cb_data_s;
      cb_data_s.f = 0;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 1500.0f;
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
      glutInitWindowSize (ENGINE_GLUT_272_DEFAULT_WIDTH, ENGINE_GLUT_272_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 272");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      glEnable (GL_LIGHTING);
      glEnable (GL_LIGHT0);
      glEnable (GL_LIGHT1);
      //glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_272_draw);
      glutReshapeFunc (engine_glut_272_reshape);
      glutVisibilityFunc (engine_glut_272_visible);

      glutKeyboardFunc (engine_glut_272_key);
      glutSpecialFunc (engine_glut_272_key_special);
      glutMouseFunc (engine_glut_272_mouse_button);
      glutMotionFunc (engine_glut_272_mouse_move);
      glutPassiveMotionFunc (engine_glut_272_mouse_move);
      glutTimerFunc (100, engine_glut_272_timer, 0);

      glutCreateMenu (engine_glut_272_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_273:
    {
      struct Engine_OpenGL_GLUT_273_CBData cb_data_s;
      cb_data_s.style = 0;
      for (float a = 0.0f; a < 150.0f * static_cast<float> (M_PI); a += 2.3999f) // <-- golden angle
      {
        float x =
          Common_GL_Tools::map (a, 0.0f, 150.0f * static_cast<float> (M_PI), 0.0f, ENGINE_GLUT_273_DEFAULT_HEIGHT / 3.0f);
        float y =
          Common_GL_Tools::map (a, 0.0f, 150.0f * static_cast<float> (M_PI), -ENGINE_GLUT_273_DEFAULT_HEIGHT / 3.0f, ENGINE_GLUT_273_DEFAULT_HEIGHT / 3.0f);
        float s =
          Common_GL_Tools::map (a, 0.0f, 150.0f * static_cast<float> (M_PI), ENGINE_GLUT_273_DEFAULT_HEIGHT / 48.0f, ENGINE_GLUT_273_DEFAULT_HEIGHT / 16.0f);
        glm::vec3 pos (x, y, 0.0f);
        glm::vec3 ang (a, -a / 150.0f, a / 150.0f);
        cb_data_s.shapes.push_back (shape (pos, ang, s));
      } // end FOR
      cb_data_s.tip = 0.0f;
      cb_data_s.angle = 0.0f;
      cb_data_s.angleVelocity = 0.0f;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 200.0f;
      cb_data_s.camera.position.z = 750.0f;
      cb_data_s.camera.looking_at.x = 0.0f;
      cb_data_s.camera.looking_at.y = 0.0f;
      cb_data_s.camera.looking_at.z = 0.0f;
      cb_data_s.camera.up.x = 0.0f;
      cb_data_s.camera.up.y = 1.0f;
      cb_data_s.camera.up.z = 0.0f;

      cb_data_s.deltaAngleX = 0.0f;
      cb_data_s.xOrigin = -1;
      cb_data_s.deltaZoomZ = 0.0f;
      cb_data_s.zOrigin = -1;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_273_DEFAULT_WIDTH, ENGINE_GLUT_273_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 273");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      glEnable (GL_LIGHTING);
      glEnable (GL_LIGHT0);
      glEnable (GL_LIGHT1);
      glDisable (GL_COLOR_MATERIAL);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glEnable (GL_NORMALIZE);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_273_draw);
      glutReshapeFunc (engine_glut_273_reshape);
      glutVisibilityFunc (engine_glut_273_visible);

      glutKeyboardFunc (engine_glut_273_key);
      glutSpecialFunc (engine_glut_273_key_special);
      glutMouseFunc (engine_glut_273_mouse_button);
      glutMotionFunc (engine_glut_273_mouse_move);
      glutPassiveMotionFunc (engine_glut_273_mouse_move);
      glutTimerFunc (100, engine_glut_273_timer, 0);

      glutCreateMenu (engine_glut_273_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_274:
    {
      struct Engine_OpenGL_GLUT_274_CBData cb_data_s;
      cb_data_s.scaleFactor = ENGINE_GLUT_274_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_274_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_274_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

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

      //cb_data_s.leftButtonIsDown = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_274_DEFAULT_WIDTH, ENGINE_GLUT_274_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 274");
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

      glutDisplayFunc (engine_glut_274_draw);
      glutReshapeFunc (engine_glut_274_reshape);
      glutVisibilityFunc (engine_glut_274_visible);

      glutKeyboardFunc (engine_glut_274_key);
      glutSpecialFunc (engine_glut_274_key_special);
      glutMouseFunc (engine_glut_274_mouse_button);
      glutMotionFunc (engine_glut_274_mouse_move);
      glutPassiveMotionFunc (engine_glut_274_mouse_move);
      glutTimerFunc (100, engine_glut_274_timer, 0);

      glutCreateMenu (engine_glut_274_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_274.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_274.vert")));
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
                    ACE_TEXT ("glut_274.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_274.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_274.frag")));
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
                    ACE_TEXT ("glut_274.frag"),
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
    case ENGINE_MODE_275:
    {
      struct Engine_OpenGL_GLUT_275_CBData cb_data_s;
      for (float x = -static_cast<float> (ENGINE_GLUT_275_DEFAULT_GRID_SIZE); x <= static_cast<float> (ENGINE_GLUT_275_DEFAULT_GRID_SIZE); x += ENGINE_GLUT_275_DEFAULT_GRID_STEP)
        for (float y = -static_cast<float> (ENGINE_GLUT_275_DEFAULT_GRID_SIZE); y <= static_cast<float> (ENGINE_GLUT_275_DEFAULT_GRID_SIZE); y += ENGINE_GLUT_275_DEFAULT_GRID_STEP)
          for (float z = -static_cast<float> (ENGINE_GLUT_275_DEFAULT_GRID_SIZE); z <= static_cast<float> (ENGINE_GLUT_275_DEFAULT_GRID_SIZE); z += ENGINE_GLUT_275_DEFAULT_GRID_STEP)
            cb_data_s.cells.push_back (cell (x, y, z));

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 500.0f;
      cb_data_s.camera.position.z = 1000.0f;
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
      glutInitWindowSize (ENGINE_GLUT_275_DEFAULT_WIDTH, ENGINE_GLUT_275_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 275");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      glEnable (GL_LIGHTING);
      glEnable (GL_LIGHT0);
      glEnable (GL_LIGHT1);
      glDisable (GL_COLOR_MATERIAL);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glEnable (GL_NORMALIZE);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_275_draw);
      glutReshapeFunc (engine_glut_275_reshape);
      glutVisibilityFunc (engine_glut_275_visible);

      glutKeyboardFunc (engine_glut_275_key);
      glutSpecialFunc (engine_glut_275_key_special);
      glutMouseFunc (engine_glut_275_mouse_button);
      glutMotionFunc (engine_glut_275_mouse_move);
      glutPassiveMotionFunc (engine_glut_275_mouse_move);
      glutTimerFunc (100, engine_glut_275_timer, 0);

      glutCreateMenu (engine_glut_275_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_276:
    {
      PGE_276 example;
      if (example.Construct (ENGINE_PGE_276_DEFAULT_WIDTH, ENGINE_PGE_276_DEFAULT_HEIGHT,
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
    case ENGINE_MODE_277:
    {
      struct Engine_OpenGL_GLUT_277_CBData cb_data_s;
      cb_data_s.scaleFactor = ENGINE_GLUT_277_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_277_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_277_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.programId = -1;
      cb_data_s.resolutionLoc = -1;
      cb_data_s.pixelDensityLoc = -1;
      cb_data_s.canvasLoc = -1;
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

      //cb_data_s.leftButtonIsDown = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_277_DEFAULT_WIDTH, ENGINE_GLUT_277_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 277");
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

      glutDisplayFunc (engine_glut_277_draw);
      glutReshapeFunc (engine_glut_277_reshape);
      glutVisibilityFunc (engine_glut_277_visible);

      glutKeyboardFunc (engine_glut_277_key);
      glutSpecialFunc (engine_glut_277_key_special);
      glutMouseFunc (engine_glut_277_mouse_button);
      glutMotionFunc (engine_glut_277_mouse_move);
      glutPassiveMotionFunc (engine_glut_277_mouse_move);
      glutTimerFunc (100, engine_glut_277_timer, 0);

      glutCreateMenu (engine_glut_277_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      GLuint vertexShader_id = glCreateShader (GL_VERTEX_SHADER);
      uint8_t* data_p = NULL;
      ACE_UINT64 file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_277.vert"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_277.vert")));
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
                    ACE_TEXT ("glut_277.vert"),
                    ACE_TEXT (info_log_a)));
        break;
      } // end IF

      GLuint fragmentShader_id = glCreateShader (GL_FRAGMENT_SHADER);
      data_p = NULL;
      file_size_i = 0;
      if (!Common_File_Tools::load (ACE_TEXT_ALWAYS_CHAR ("glut_277.frag"),
                                    data_p,
                                    file_size_i,
                                    0))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load \"%s\", aborting\n"),
                    ACE_TEXT ("glut_277.frag")));
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
                    ACE_TEXT ("glut_277.frag"),
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
      cb_data_s.pixelDensityLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("iPixelDensity"));
      //ACE_ASSERT (cb_data_s.pixelDensityLoc != -1);
      cb_data_s.canvasLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("iCanvas"));
      //ACE_ASSERT (cb_data_s.canvasLoc != -1);
      cb_data_s.mouseLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      //ACE_ASSERT (cb_data_s.mouseLoc != -1);
      cb_data_s.timeLoc =
        glGetUniformLocation (cb_data_s.programId, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.timeLoc != -1);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_278:
    {
      struct Engine_OpenGL_GLUT_278_CBData cb_data_s;
      cb_data_s.f = 0.0f;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 0.0f;
      cb_data_s.camera.position.z = 1500.0f;
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
      glutInitWindowSize (ENGINE_GLUT_278_DEFAULT_WIDTH, ENGINE_GLUT_278_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 278");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      glEnable (GL_LIGHTING);
      glEnable (GL_LIGHT0);
      glEnable (GL_LIGHT1);
      glDisable (GL_COLOR_MATERIAL);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glEnable (GL_NORMALIZE);

      glPolygonMode (GL_FRONT_AND_BACK,
                     GL_FILL);

      glutDisplayFunc (engine_glut_278_draw);
      glutReshapeFunc (engine_glut_278_reshape);
      glutVisibilityFunc (engine_glut_278_visible);

      glutKeyboardFunc (engine_glut_278_key);
      glutSpecialFunc (engine_glut_278_key_special);
      glutMouseFunc (engine_glut_278_mouse_button);
      glutMotionFunc (engine_glut_278_mouse_move);
      glutPassiveMotionFunc (engine_glut_278_mouse_move);
      glutTimerFunc (100, engine_glut_278_timer, 0);

      glutCreateMenu (engine_glut_278_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glutMainLoop ();

      result = true;

      break;
    }
    case ENGINE_MODE_279:
    {
      struct Engine_OpenGL_GLUT_279_CBData cb_data_s;
      createSponge (ENGINE_GLUT_279_DEFAULT_LEVEL,
                    0, 0, 0,
                    cb_data_s);
      cb_data_s.spongeSize =
        static_cast<int> (std::pow (3, ENGINE_GLUT_279_DEFAULT_LEVEL)) * ENGINE_GLUT_279_DEFAULT_CUBE_SIZE;

      cb_data_s.wireframe = false;

      cb_data_s.camera.position.x = 0.0f;
      cb_data_s.camera.position.y = 750.0f;
      cb_data_s.camera.position.z = 2000.0f;
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
      glutInitWindowSize (ENGINE_GLUT_279_DEFAULT_WIDTH, ENGINE_GLUT_279_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 279");
      glutSetWindow (window_i);
      glutSetWindowData (&cb_data_s);

      glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

      glEnable (GL_DEPTH_TEST);

      //glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      glutDisplayFunc (engine_glut_279_draw);
      glutReshapeFunc (engine_glut_279_reshape);
      glutVisibilityFunc (engine_glut_279_visible);

      glutKeyboardFunc (engine_glut_279_key);
      glutSpecialFunc (engine_glut_279_key_special);
      glutMouseFunc (engine_glut_279_mouse_button);
      glutMotionFunc (engine_glut_279_mouse_move);
      glutPassiveMotionFunc (engine_glut_279_mouse_move);
      glutTimerFunc (100, engine_glut_279_timer, 0);

      glutCreateMenu (engine_glut_279_menu);
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
