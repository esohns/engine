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
#include "defines_25.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "glut_530.h"
#include "glut_531.h"
#include "glut_532.h"
#include "glut_533.h"
#include "glut_534.h"
#include "glut_535.h"
#include "glut_536.h"
#include "glut_537.h"
#include "glut_538.h"
#include "glut_539.h"
#include "glut_540.h"
#include "glut_541.h"
#include "glut_542.h"
#include "glut_543.h"
#include "glut_544.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 530,
  ENGINE_MODE_531,
  ENGINE_MODE_532,
  ENGINE_MODE_533,
  ENGINE_MODE_534,
  ENGINE_MODE_535,
  ENGINE_MODE_536,
  ENGINE_MODE_537,
  ENGINE_MODE_538,
  ENGINE_MODE_539,
  ENGINE_MODE_540,
  ENGINE_MODE_541,
  ENGINE_MODE_542,
  ENGINE_MODE_543,
  ENGINE_MODE_544,
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
      struct Engine_OpenGL_GLUT_530_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_530_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_530_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_530_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_530_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_530_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_530_DEFAULT_WIDTH, ENGINE_GLUT_530_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 530");
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

      glutDisplayFunc (engine_glut_530_draw);
      glutReshapeFunc (engine_glut_530_reshape);
      glutVisibilityFunc (engine_glut_530_visible);

      glutKeyboardFunc (engine_glut_530_key);
      glutSpecialFunc (engine_glut_530_key_special);
      glutMouseFunc (engine_glut_530_mouse_button);
      glutMotionFunc (engine_glut_530_mouse_move);
      glutPassiveMotionFunc (engine_glut_530_mouse_move);
      glutTimerFunc (100, engine_glut_530_timer, 0);

      glutCreateMenu (engine_glut_530_menu);
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
      std::string neg_z_path_string = ACE_TEXT_ALWAYS_CHAR ("glut_484_negz.jpg"); // *TODO*: use dedicated cubemap
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
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_CUBE_MAP);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_530.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_530.frag")))
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

      cb_data_s.shader.reset ();

      cb_data_s.texture0.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_531:
    {
      struct Engine_OpenGL_GLUT_531_CBData cb_data_s;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1channel3Loc = -1;

      cb_data_s.S2resolutionLoc = -1;
      cb_data_s.S2channel0Loc = -1;
      cb_data_s.S2channel2Loc = -1;
      cb_data_s.S2channel3Loc = -1;

      cb_data_s.S3resolutionLoc = -1;
      cb_data_s.S3frameLoc = -1;
      cb_data_s.S3channel0Loc = -1;
      cb_data_s.S3channel1Loc = -1;
      cb_data_s.S3channel2Loc = -1;
      cb_data_s.S3channel3Loc = -1;

      cb_data_s.S4resolutionLoc = -1;
      cb_data_s.S4frameLoc = -1;
      cb_data_s.S4channel0Loc = -1;
      cb_data_s.S4channel1Loc = -1;
      cb_data_s.S4channel2Loc = -1;
      cb_data_s.S4channel3Loc = -1;

      cb_data_s.S5resolutionLoc = -1;
      cb_data_s.S5timeLoc = -1;
      cb_data_s.S5frameLoc = -1;
      cb_data_s.S5channel2Loc = -1;
      cb_data_s.S5channel3Loc = -1;

      cb_data_s.FBO1 = 0;
      cb_data_s.FBO2 = 0;
      cb_data_s.FBO3 = 0;
      cb_data_s.FBO4 = 0;

      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_531_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_531_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      //glutInitContextVersion (3, 3);
      //glutInitContextProfile (GLUT_CORE_PROFILE);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_531_DEFAULT_WIDTH, ENGINE_GLUT_531_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 531");
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

      glutDisplayFunc (engine_glut_531_draw);
      glutReshapeFunc (engine_glut_531_reshape);
      glutVisibilityFunc (engine_glut_531_visible);

      glutKeyboardFunc (engine_glut_531_key);
      glutKeyboardUpFunc (engine_glut_531_key_up);
      glutSpecialFunc (engine_glut_531_key_special);
      glutSpecialUpFunc (engine_glut_531_key_special_up);
      glutMouseFunc (engine_glut_531_mouse_button);
      glutMotionFunc (engine_glut_531_mouse_move);
      glutPassiveMotionFunc (engine_glut_531_mouse_move);
      glutTimerFunc (100, engine_glut_531_timer, 0);

      glutCreateMenu (engine_glut_531_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_531.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_531.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_531.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_531.frag_2")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader3.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_531.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_531.frag_3")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader4.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_531.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_531.frag_4")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader5.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_531.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_531.frag_5")))
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
      std::vector<GLfloat> initial_values_a (ENGINE_GLUT_531_DEFAULT_WIDTH * ENGINE_GLUT_531_DEFAULT_HEIGHT * 4, 0.0f);
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_531_DEFAULT_WIDTH, ENGINE_GLUT_531_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE2);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS2.id_);
      ACE_ASSERT (cb_data_s.textureS2.id_);
      cb_data_s.textureS2.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader2
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_531_DEFAULT_WIDTH, ENGINE_GLUT_531_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE3);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS3.id_);
      ACE_ASSERT (cb_data_s.textureS3.id_);
      cb_data_s.textureS3.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader3
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_531_DEFAULT_WIDTH, ENGINE_GLUT_531_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE4);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS4.id_);
      ACE_ASSERT (cb_data_s.textureS4.id_);
      cb_data_s.textureS4.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader4
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_531_DEFAULT_WIDTH, ENGINE_GLUT_531_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      //glGenerateMipmap (GL_TEXTURE_2D);

      cb_data_s.shader1.use ();
      cb_data_s.S1resolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S1resolutionLoc != -1);
      cb_data_s.S1channel3Loc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel3"));
      ACE_ASSERT (cb_data_s.S1channel3Loc != -1);

      cb_data_s.shader2.use ();
      cb_data_s.S2resolutionLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S2resolutionLoc != -1);
      cb_data_s.S2channel0Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S2channel0Loc != -1);
      cb_data_s.S2channel2Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel2"));
      ACE_ASSERT (cb_data_s.S2channel2Loc != -1);
      cb_data_s.S2channel3Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel3"));
      ACE_ASSERT (cb_data_s.S2channel3Loc != -1);

      cb_data_s.shader3.use ();
      cb_data_s.S3resolutionLoc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S3resolutionLoc != -1);
      cb_data_s.S3frameLoc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S3frameLoc != -1);
      cb_data_s.S3channel0Loc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S3channel0Loc != -1);
      cb_data_s.S3channel1Loc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S3channel1Loc != -1);
      cb_data_s.S3channel2Loc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel2"));
      ACE_ASSERT (cb_data_s.S3channel2Loc != -1);
      cb_data_s.S3channel3Loc =
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel3"));
      ACE_ASSERT (cb_data_s.S3channel3Loc != -1);

      cb_data_s.shader4.use ();
      cb_data_s.S4resolutionLoc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S4resolutionLoc != -1);
      cb_data_s.S4frameLoc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S4frameLoc != -1);
      cb_data_s.S4channel0Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S4channel0Loc != -1);
      cb_data_s.S4channel1Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S4channel1Loc != -1);
      cb_data_s.S4channel2Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel2"));
      ACE_ASSERT (cb_data_s.S4channel2Loc != -1);
      cb_data_s.S4channel3Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel3"));
      ACE_ASSERT (cb_data_s.S4channel3Loc != -1);

      cb_data_s.shader5.use ();
      cb_data_s.S5resolutionLoc =
        glGetUniformLocation (cb_data_s.shader5.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S5resolutionLoc != -1);
      cb_data_s.S5timeLoc =
        glGetUniformLocation (cb_data_s.shader5.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S5timeLoc != -1);
      cb_data_s.S5frameLoc =
        glGetUniformLocation (cb_data_s.shader5.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S5frameLoc != -1);
      cb_data_s.S5channel2Loc =
        glGetUniformLocation (cb_data_s.shader5.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel2"));
      //ACE_ASSERT (cb_data_s.S5channel2Loc != -1);
      cb_data_s.S5channel3Loc =
        glGetUniformLocation (cb_data_s.shader5.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel3"));
      ACE_ASSERT (cb_data_s.S5channel3Loc != -1);

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
      // draw render pass 3 to framebuffer object (--> textureS3)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO3);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS3.id_, 0);
      ACE_ASSERT (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer (GL_FRAMEBUFFER, 0);

      glGenFramebuffers (1, &cb_data_s.FBO4);
      ACE_ASSERT (cb_data_s.FBO4);
      // draw render pass 4 to framebuffer object (--> textureS4)
      glBindFramebuffer (GL_FRAMEBUFFER, cb_data_s.FBO4);
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb_data_s.textureS4.id_, 0);
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
      glDeleteFramebuffers (1, &cb_data_s.FBO4);

      cb_data_s.textureS1.reset ();
      cb_data_s.textureS2.reset ();
      cb_data_s.textureS3.reset ();
      cb_data_s.textureS4.reset ();

      cb_data_s.shader1.reset ();
      cb_data_s.shader2.reset ();
      cb_data_s.shader3.reset ();
      cb_data_s.shader4.reset ();
      cb_data_s.shader5.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_532:
    {
      struct Engine_OpenGL_GLUT_532_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_532_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_532_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_532_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_532_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_532_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_532_DEFAULT_WIDTH, ENGINE_GLUT_532_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 532");
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

      glutDisplayFunc (engine_glut_532_draw);
      glutReshapeFunc (engine_glut_532_reshape);
      glutVisibilityFunc (engine_glut_532_visible);

      glutKeyboardFunc (engine_glut_532_key);
      glutSpecialFunc (engine_glut_532_key_special);
      glutMouseFunc (engine_glut_532_mouse_button);
      glutMotionFunc (engine_glut_532_mouse_move);
      glutPassiveMotionFunc (engine_glut_532_mouse_move);
      glutTimerFunc (100, engine_glut_532_timer, 0);

      glutCreateMenu (engine_glut_532_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_532.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_532.frag")))
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
    case ENGINE_MODE_533:
    {
      struct Engine_OpenGL_GLUT_533_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_533_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_533_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_533_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_533_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_533_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_533_DEFAULT_WIDTH, ENGINE_GLUT_533_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 533");
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

      glutDisplayFunc (engine_glut_533_draw);
      glutReshapeFunc (engine_glut_533_reshape);
      glutVisibilityFunc (engine_glut_533_visible);

      glutKeyboardFunc (engine_glut_533_key);
      glutSpecialFunc (engine_glut_533_key_special);
      glutMouseFunc (engine_glut_533_mouse_button);
      glutMotionFunc (engine_glut_533_mouse_move);
      glutPassiveMotionFunc (engine_glut_533_mouse_move);
      glutTimerFunc (100, engine_glut_533_timer, 0);

      glutCreateMenu (engine_glut_533_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glClampColor (GL_CLAMP_READ_COLOR, GL_FALSE);
      glClampColor (GL_CLAMP_VERTEX_COLOR, GL_FALSE);
      glClampColor (GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei (GL_PACK_ALIGNMENT, 1);

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_533_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_533.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_533.frag")))
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

      cb_data_s.shader.reset ();

      cb_data_s.texture0.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_534:
    {
      struct Engine_OpenGL_GLUT_534_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_534_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_534_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_534_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;
      cb_data_s.channel1Loc = -1;
      cb_data_s.channel2Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_534_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_534_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_534_DEFAULT_WIDTH, ENGINE_GLUT_534_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 534");
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

      glutDisplayFunc (engine_glut_534_draw);
      glutReshapeFunc (engine_glut_534_reshape);
      glutVisibilityFunc (engine_glut_534_visible);

      glutKeyboardFunc (engine_glut_534_key);
      glutSpecialFunc (engine_glut_534_key_special);
      glutMouseFunc (engine_glut_534_mouse_button);
      glutMotionFunc (engine_glut_534_mouse_move);
      glutPassiveMotionFunc (engine_glut_534_mouse_move);
      glutTimerFunc (100, engine_glut_534_timer, 0);

      glutCreateMenu (engine_glut_534_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glClampColor (GL_CLAMP_READ_COLOR, GL_FALSE);
      glClampColor (GL_CLAMP_VERTEX_COLOR, GL_FALSE);
      glClampColor (GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei (GL_PACK_ALIGNMENT, 1);

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_534_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE1);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      if (!cb_data_s.texture1.load (ACE_TEXT_ALWAYS_CHAR ("glut_534_channel1.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture1.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE2);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      if (!cb_data_s.texture2.load (ACE_TEXT_ALWAYS_CHAR ("glut_534_channel2.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture2.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_534.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_534.frag")))
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
      cb_data_s.channel1Loc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.channel1Loc != -1);
      cb_data_s.channel2Loc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel2"));
      ACE_ASSERT (cb_data_s.channel2Loc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      cb_data_s.shader.reset ();

      cb_data_s.texture0.reset ();
      cb_data_s.texture1.reset ();
      cb_data_s.texture2.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_535:
    {
      struct Engine_OpenGL_GLUT_535_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_535_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_535_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_535_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_535_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_535_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_535_DEFAULT_WIDTH, ENGINE_GLUT_535_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 535");
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

      glutDisplayFunc (engine_glut_535_draw);
      glutReshapeFunc (engine_glut_535_reshape);
      glutVisibilityFunc (engine_glut_535_visible);

      glutKeyboardFunc (engine_glut_535_key);
      glutSpecialFunc (engine_glut_535_key_special);
      glutMouseFunc (engine_glut_535_mouse_button);
      glutMotionFunc (engine_glut_535_mouse_move);
      glutPassiveMotionFunc (engine_glut_535_mouse_move);
      glutTimerFunc (100, engine_glut_535_timer, 0);

      glutCreateMenu (engine_glut_535_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      glClampColor (GL_CLAMP_READ_COLOR, GL_FALSE);
      glClampColor (GL_CLAMP_VERTEX_COLOR, GL_FALSE);
      glClampColor (GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei (GL_PACK_ALIGNMENT, 1);

      glActiveTexture (GL_TEXTURE0);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_535_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_535.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_535.frag")))
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

      cb_data_s.shader.reset ();

      cb_data_s.texture0.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_536:
    {
      struct Engine_OpenGL_GLUT_536_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_536_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_536_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_536_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.frameLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_536_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_536_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_536_DEFAULT_WIDTH, ENGINE_GLUT_536_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 536");
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

      glutDisplayFunc (engine_glut_536_draw);
      glutReshapeFunc (engine_glut_536_reshape);
      glutVisibilityFunc (engine_glut_536_visible);

      glutKeyboardFunc (engine_glut_536_key);
      glutSpecialFunc (engine_glut_536_key_special);
      glutMouseFunc (engine_glut_536_mouse_button);
      glutMotionFunc (engine_glut_536_mouse_move);
      glutPassiveMotionFunc (engine_glut_536_mouse_move);
      glutTimerFunc (100, engine_glut_536_timer, 0);

      glutCreateMenu (engine_glut_536_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_536.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_536.frag")))
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
    case ENGINE_MODE_537:
    {
      struct Engine_OpenGL_GLUT_537_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_537_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_537_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_537_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_537_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_537_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_537_DEFAULT_WIDTH, ENGINE_GLUT_537_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 537");
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

      glutDisplayFunc (engine_glut_537_draw);
      glutReshapeFunc (engine_glut_537_reshape);
      glutVisibilityFunc (engine_glut_537_visible);

      glutKeyboardFunc (engine_glut_537_key);
      glutSpecialFunc (engine_glut_537_key_special);
      glutMouseFunc (engine_glut_537_mouse_button);
      glutMotionFunc (engine_glut_537_mouse_move);
      glutPassiveMotionFunc (engine_glut_537_mouse_move);
      glutTimerFunc (100, engine_glut_537_timer, 0);

      glutCreateMenu (engine_glut_537_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_537.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_537.frag")))
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
    case ENGINE_MODE_538:
    {
      struct Engine_OpenGL_GLUT_538_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_538_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_538_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_538_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_538_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_538_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_538_DEFAULT_WIDTH, ENGINE_GLUT_538_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 538");
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

      glutDisplayFunc (engine_glut_538_draw);
      glutReshapeFunc (engine_glut_538_reshape);
      glutVisibilityFunc (engine_glut_538_visible);

      glutKeyboardFunc (engine_glut_538_key);
      glutSpecialFunc (engine_glut_538_key_special);
      glutMouseFunc (engine_glut_538_mouse_button);
      glutMotionFunc (engine_glut_538_mouse_move);
      glutPassiveMotionFunc (engine_glut_538_mouse_move);
      glutTimerFunc (100, engine_glut_538_timer, 0);

      glutCreateMenu (engine_glut_538_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_538.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_538.frag")))
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
    case ENGINE_MODE_539:
    {
      struct Engine_OpenGL_GLUT_539_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_539_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_539_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_539_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_539_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_539_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_539_DEFAULT_WIDTH, ENGINE_GLUT_539_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 539");
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

      glutDisplayFunc (engine_glut_539_draw);
      glutReshapeFunc (engine_glut_539_reshape);
      glutVisibilityFunc (engine_glut_539_visible);

      glutKeyboardFunc (engine_glut_539_key);
      glutSpecialFunc (engine_glut_539_key_special);
      glutMouseFunc (engine_glut_539_mouse_button);
      glutMotionFunc (engine_glut_539_mouse_move);
      glutPassiveMotionFunc (engine_glut_539_mouse_move);
      glutTimerFunc (100, engine_glut_539_timer, 0);

      glutCreateMenu (engine_glut_539_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_539.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_539.frag")))
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
    case ENGINE_MODE_540:
    {
      struct Engine_OpenGL_GLUT_540_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_540_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_540_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_540_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_540_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_540_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_540_DEFAULT_WIDTH, ENGINE_GLUT_540_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 540");
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

      glutDisplayFunc (engine_glut_540_draw);
      glutReshapeFunc (engine_glut_540_reshape);
      glutVisibilityFunc (engine_glut_540_visible);

      glutKeyboardFunc (engine_glut_540_key);
      glutSpecialFunc (engine_glut_540_key_special);
      glutMouseFunc (engine_glut_540_mouse_button);
      glutMotionFunc (engine_glut_540_mouse_move);
      glutPassiveMotionFunc (engine_glut_540_mouse_move);
      glutTimerFunc (100, engine_glut_540_timer, 0);

      glutCreateMenu (engine_glut_540_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_540.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_540.frag")))
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
    case ENGINE_MODE_541:
    {
      struct Engine_OpenGL_GLUT_541_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_541_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_541_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_541_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_541_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_541_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_541_DEFAULT_WIDTH, ENGINE_GLUT_541_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 541");
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

      glutDisplayFunc (engine_glut_541_draw);
      glutReshapeFunc (engine_glut_541_reshape);
      glutVisibilityFunc (engine_glut_541_visible);

      glutKeyboardFunc (engine_glut_541_key);
      glutSpecialFunc (engine_glut_541_key_special);
      glutMouseFunc (engine_glut_541_mouse_button);
      glutMotionFunc (engine_glut_541_mouse_move);
      glutPassiveMotionFunc (engine_glut_541_mouse_move);
      glutTimerFunc (100, engine_glut_541_timer, 0);

      glutCreateMenu (engine_glut_541_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_541.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_541.frag")))
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
    case ENGINE_MODE_542:
    {
      struct Engine_OpenGL_GLUT_542_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_542_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_542_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_542_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.frameLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_542_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_542_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_542_DEFAULT_WIDTH, ENGINE_GLUT_542_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 542");
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

      glutDisplayFunc (engine_glut_542_draw);
      glutReshapeFunc (engine_glut_542_reshape);
      glutVisibilityFunc (engine_glut_542_visible);

      glutKeyboardFunc (engine_glut_542_key);
      glutSpecialFunc (engine_glut_542_key_special);
      glutMouseFunc (engine_glut_542_mouse_button);
      glutMotionFunc (engine_glut_542_mouse_move);
      glutPassiveMotionFunc (engine_glut_542_mouse_move);
      glutTimerFunc (100, engine_glut_542_timer, 0);

      glutCreateMenu (engine_glut_542_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_542.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_542.frag")))
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
    case ENGINE_MODE_543:
    {
      struct Engine_OpenGL_GLUT_543_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_543_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_543_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_543_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_543_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_543_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_543_DEFAULT_WIDTH, ENGINE_GLUT_543_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 543");
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

      glutDisplayFunc (engine_glut_543_draw);
      glutReshapeFunc (engine_glut_543_reshape);
      glutVisibilityFunc (engine_glut_543_visible);

      glutKeyboardFunc (engine_glut_543_key);
      glutSpecialFunc (engine_glut_543_key_special);
      glutMouseFunc (engine_glut_543_mouse_button);
      glutMotionFunc (engine_glut_543_mouse_move);
      glutPassiveMotionFunc (engine_glut_543_mouse_move);
      glutTimerFunc (100, engine_glut_543_timer, 0);

      glutCreateMenu (engine_glut_543_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_543.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_543.frag")))
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
    case ENGINE_MODE_544:
    {
      struct Engine_OpenGL_GLUT_544_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_544_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_544_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_544_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_544_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_544_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_544_DEFAULT_WIDTH, ENGINE_GLUT_544_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 544");
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

      glutDisplayFunc (engine_glut_544_draw);
      glutReshapeFunc (engine_glut_544_reshape);
      glutVisibilityFunc (engine_glut_544_visible);

      glutKeyboardFunc (engine_glut_544_key);
      glutSpecialFunc (engine_glut_544_key_special);
      glutMouseFunc (engine_glut_544_mouse_button);
      glutMotionFunc (engine_glut_544_mouse_move);
      glutPassiveMotionFunc (engine_glut_544_mouse_move);
      glutTimerFunc (100, engine_glut_544_timer, 0);

      glutCreateMenu (engine_glut_544_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_544.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_544.frag")))
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
