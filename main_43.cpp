#include "stdafx.h"

#include <algorithm>
#include <iostream>
#include <random>
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
#include "defines_43.h"
#include "engine_common.h"

#define OLC_PGE_APPLICATION
#define OPENSIMPLEXNOISE_STATIC_CONSTANTS
#include "glut_890.h"
#include "glut_891.h"
#include "glut_892.h"
#include "glut_893.h"
#include "glut_894.h"
#include "glut_895.h"
#include "glut_896.h"
#include "glut_897.h"
#include "glut_898.h"
#include "glut_899.h"
#include "glut_900.h"
#include "glut_901.h"
#include "glut_902.h"
#include "glut_903.h"
#include "glut_904.h"
#include "glut_905.h"
#include "glut_906.h"
#include "glut_907.h"
#include "glut_908.h"
#include "glut_909.h"

enum Engine_ModeType
{
  ENGINE_MODE_DEFAULT = 890,
  ENGINE_MODE_891,
  ENGINE_MODE_892,
  ENGINE_MODE_893,
  ENGINE_MODE_894,
  ENGINE_MODE_895,
  ENGINE_MODE_896,
  ENGINE_MODE_897,
  ENGINE_MODE_898,
  ENGINE_MODE_899,
  ENGINE_MODE_900,
  ENGINE_MODE_901,
  ENGINE_MODE_902,
  ENGINE_MODE_903,
  ENGINE_MODE_904,
  ENGINE_MODE_905,
  ENGINE_MODE_906,
  ENGINE_MODE_907,
  ENGINE_MODE_908,
  ENGINE_MODE_909,
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
      struct Engine_OpenGL_GLUT_890_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_890_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_890_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_890_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_890_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_890_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      cb_data_s.isRunning = true;
      cb_data_s.windowId = 0;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_890_DEFAULT_WIDTH, ENGINE_GLUT_890_DEFAULT_HEIGHT);

      cb_data_s.windowId = glutCreateWindow ("engine GLUT 890");
      if (!cb_data_s.windowId)
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to glutCreateWindow(), aborting\n")));
        break;
      } // end IF
      glutSetWindow (cb_data_s.windowId);
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

      glutDisplayFunc (engine_glut_890_draw);
      glutReshapeFunc (engine_glut_890_reshape);
      glutVisibilityFunc (engine_glut_890_visible);
      glutCloseFunc (engine_glut_890_close);

      glutKeyboardFunc (engine_glut_890_key);
      glutKeyboardUpFunc (engine_glut_890_key_up);
      glutSpecialFunc (engine_glut_890_key_special);
      glutMouseFunc (engine_glut_890_mouse_button);
      glutMotionFunc (engine_glut_890_mouse_move);
      glutPassiveMotionFunc (engine_glut_890_mouse_move);

      glutTimerFunc (100, engine_glut_890_timer, 1);

      glutCreateMenu (engine_glut_890_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_890.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_890.frag")))
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
      cb_data_s.isRunning = false;

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_891:
    {
      struct Engine_OpenGL_GLUT_891_CBData cb_data_s;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1channel0Loc = -1;
      cb_data_s.S1channel1Loc = -1;

      cb_data_s.S2resolutionLoc = -1;
      cb_data_s.S2timeLoc = -1;
      cb_data_s.S2frameLoc = -1;
      cb_data_s.S2mouseLoc = -1;
      cb_data_s.S2channel0Loc = -1;
      cb_data_s.S2channel1Loc = -1;

      cb_data_s.S3resolutionLoc = -1;
      cb_data_s.S3timeLoc = -1;
      cb_data_s.S3frameLoc = -1;
      cb_data_s.S3mouseLoc = -1;
      cb_data_s.S3channel0Loc = -1;
      cb_data_s.S3channel1Loc = -1;
      cb_data_s.S3channel2Loc = -1;
      cb_data_s.S3channel3Loc = -1;

      cb_data_s.S4resolutionLoc = -1;
      cb_data_s.S4channel0Loc = -1;
      cb_data_s.S4channel1Loc = -1;

      cb_data_s.S5resolutionLoc = -1;
      cb_data_s.S5timeLoc = -1;
      cb_data_s.S5mouseLoc = -1;
      cb_data_s.S5channel0Loc = -1;
      cb_data_s.S5channel1Loc = -1;

      cb_data_s.FBO1 = 0;
      cb_data_s.FBO2 = 0;
      cb_data_s.FBO3 = 0;
      cb_data_s.FBO4 = 0;

      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_891_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_891_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      //glutInitContextVersion (3, 3);
      //glutInitContextProfile (GLUT_CORE_PROFILE);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_891_DEFAULT_WIDTH, ENGINE_GLUT_891_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 891");
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

      glutDisplayFunc (engine_glut_891_draw);
      glutReshapeFunc (engine_glut_891_reshape);
      glutVisibilityFunc (engine_glut_891_visible);

      glutKeyboardFunc (engine_glut_891_key);
      glutKeyboardUpFunc (engine_glut_891_key_up);
      glutSpecialFunc (engine_glut_891_key_special);
      glutSpecialUpFunc (engine_glut_891_key_special_up);
      glutMouseFunc (engine_glut_891_mouse_button);
      glutMotionFunc (engine_glut_891_mouse_move);
      glutPassiveMotionFunc (engine_glut_891_mouse_move);
      glutTimerFunc (100, engine_glut_891_timer, 0);

      glutCreateMenu (engine_glut_891_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_891.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_891.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_891.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_891.frag_2")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader3.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_891.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_891.frag_3")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader4.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_891.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_891.frag_4")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader5.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_891.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_891.frag_5")))
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
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_891_S2channel0.png")))
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
      std::vector<GLfloat> initial_values_a (ENGINE_GLUT_891_DEFAULT_WIDTH * ENGINE_GLUT_891_DEFAULT_HEIGHT * 4, 0.0f);
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_891_DEFAULT_WIDTH, ENGINE_GLUT_891_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
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
      //                   the result of shader2
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_891_DEFAULT_WIDTH, ENGINE_GLUT_891_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
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
      //                   the result of shader3
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_891_DEFAULT_WIDTH, ENGINE_GLUT_891_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE4);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glGenTextures (1, &cb_data_s.textureS4.id_);
      ACE_ASSERT (cb_data_s.textureS4.id_);
      cb_data_s.textureS4.bind ();
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader4
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_891_DEFAULT_WIDTH, ENGINE_GLUT_891_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      glActiveTexture (GL_TEXTURE5);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture5.load (ACE_TEXT_ALWAYS_CHAR ("glut_566_channel0.png"))) // *TODO*: use custom texture
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture5.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glGenerateMipmap (GL_TEXTURE_2D);

      cb_data_s.shader1.use ();
      cb_data_s.S1resolutionLoc =
        glGetUniformLocation (cb_data_s.shader1.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S1resolutionLoc != -1);
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
      cb_data_s.S2timeLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S2timeLoc != -1);
      cb_data_s.S2frameLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iFrame"));
      ACE_ASSERT (cb_data_s.S2frameLoc != -1);
      cb_data_s.S2mouseLoc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.S2mouseLoc != -1);
      cb_data_s.S2channel0Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S2channel0Loc != -1);
      cb_data_s.S2channel1Loc =
        glGetUniformLocation (cb_data_s.shader2.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S2channel1Loc != -1);

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
        glGetUniformLocation (cb_data_s.shader3.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.S3mouseLoc != -1);
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
      cb_data_s.S4channel0Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S4channel0Loc != -1);
      cb_data_s.S4channel1Loc =
        glGetUniformLocation (cb_data_s.shader4.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S4channel1Loc != -1);

      cb_data_s.shader5.use ();
      cb_data_s.S5resolutionLoc =
        glGetUniformLocation (cb_data_s.shader5.id_, ACE_TEXT_ALWAYS_CHAR ("iResolution"));
      ACE_ASSERT (cb_data_s.S5resolutionLoc != -1);
      cb_data_s.S5timeLoc =
        glGetUniformLocation (cb_data_s.shader5.id_, ACE_TEXT_ALWAYS_CHAR ("iTime"));
      ACE_ASSERT (cb_data_s.S5timeLoc != -1);
      cb_data_s.S5mouseLoc =
        glGetUniformLocation (cb_data_s.shader5.id_, ACE_TEXT_ALWAYS_CHAR ("iMouse"));
      ACE_ASSERT (cb_data_s.S5mouseLoc != -1);
      cb_data_s.S5channel0Loc =
        glGetUniformLocation (cb_data_s.shader5.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel0"));
      ACE_ASSERT (cb_data_s.S5channel0Loc != -1);
      cb_data_s.S5channel1Loc =
        glGetUniformLocation (cb_data_s.shader5.id_, ACE_TEXT_ALWAYS_CHAR ("iChannel1"));
      ACE_ASSERT (cb_data_s.S5channel1Loc != -1);

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

      cb_data_s.texture0.reset ();
      cb_data_s.textureS1.reset ();
      cb_data_s.textureS2.reset ();
      cb_data_s.textureS3.reset ();
      cb_data_s.textureS4.reset ();
      cb_data_s.texture5.reset ();

      cb_data_s.shader1.reset ();
      cb_data_s.shader2.reset ();
      cb_data_s.shader3.reset ();
      cb_data_s.shader4.reset ();
      cb_data_s.shader5.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_892:
    {
      struct Engine_OpenGL_GLUT_892_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_892_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_892_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_892_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.frameLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_892_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_892_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_892_DEFAULT_WIDTH, ENGINE_GLUT_892_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 892");
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

      glutDisplayFunc (engine_glut_892_draw);
      glutReshapeFunc (engine_glut_892_reshape);
      glutVisibilityFunc (engine_glut_892_visible);

      glutKeyboardFunc (engine_glut_892_key);
      glutKeyboardUpFunc (engine_glut_892_key_up);
      glutSpecialFunc (engine_glut_892_key_special);
      glutMouseFunc (engine_glut_892_mouse_button);
      glutMotionFunc (engine_glut_892_mouse_move);
      glutPassiveMotionFunc (engine_glut_892_mouse_move);

      glutTimerFunc (100, engine_glut_892_timer, 0);

      glutCreateMenu (engine_glut_892_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_892.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_892.frag")))
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
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_873_channel1.jpg"))) // *TODO*: use custom texture
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

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
    case ENGINE_MODE_893:
    {
      struct Engine_OpenGL_GLUT_893_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_893_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_893_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_893_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_893_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_893_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_893_DEFAULT_WIDTH, ENGINE_GLUT_893_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 893");
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

      glutDisplayFunc (engine_glut_893_draw);
      glutReshapeFunc (engine_glut_893_reshape);
      glutVisibilityFunc (engine_glut_893_visible);

      glutKeyboardFunc (engine_glut_893_key);
      glutKeyboardUpFunc (engine_glut_893_key_up);
      glutSpecialFunc (engine_glut_893_key_special);
      glutMouseFunc (engine_glut_893_mouse_button);
      glutMotionFunc (engine_glut_893_mouse_move);
      glutPassiveMotionFunc (engine_glut_893_mouse_move);

      glutTimerFunc (100, engine_glut_893_timer, 0);

      glutCreateMenu (engine_glut_893_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_893.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_893.frag")))
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
    case ENGINE_MODE_894:
    {
      struct Engine_OpenGL_GLUT_894_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_894_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_894_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_894_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_894_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_894_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_894_DEFAULT_WIDTH, ENGINE_GLUT_894_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 894");
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

      glutDisplayFunc (engine_glut_894_draw);
      glutReshapeFunc (engine_glut_894_reshape);
      glutVisibilityFunc (engine_glut_894_visible);

      glutKeyboardFunc (engine_glut_894_key);
      glutKeyboardUpFunc (engine_glut_894_key_up);
      glutSpecialFunc (engine_glut_894_key_special);
      glutMouseFunc (engine_glut_894_mouse_button);
      glutMotionFunc (engine_glut_894_mouse_move);
      glutPassiveMotionFunc (engine_glut_894_mouse_move);

      glutTimerFunc (100, engine_glut_894_timer, 0);

      glutCreateMenu (engine_glut_894_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_894.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_894.frag")))
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
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_891_S2channel0.png"))) // *TODO*: use custom texture
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

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
    case ENGINE_MODE_895:
    {
      struct Engine_OpenGL_GLUT_895_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_895_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_895_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_895_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_895_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_895_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_895_DEFAULT_WIDTH, ENGINE_GLUT_895_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 895");
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

      glutDisplayFunc (engine_glut_895_draw);
      glutReshapeFunc (engine_glut_895_reshape);
      glutVisibilityFunc (engine_glut_895_visible);

      glutKeyboardFunc (engine_glut_895_key);
      glutKeyboardUpFunc (engine_glut_895_key_up);
      glutSpecialFunc (engine_glut_895_key_special);
      glutMouseFunc (engine_glut_895_mouse_button);
      glutMotionFunc (engine_glut_895_mouse_move);
      glutPassiveMotionFunc (engine_glut_895_mouse_move);

      glutTimerFunc (100, engine_glut_895_timer, 0);

      glutCreateMenu (engine_glut_895_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_895.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_895.frag")))
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
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_885_channel0.png"))) // *TODO*: use custom texture
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

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
    case ENGINE_MODE_896:
    {
      struct Engine_OpenGL_GLUT_896_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_896_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_896_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_896_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_896_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_896_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_896_DEFAULT_WIDTH, ENGINE_GLUT_896_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 896");
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

      glutDisplayFunc (engine_glut_896_draw);
      glutReshapeFunc (engine_glut_896_reshape);
      glutVisibilityFunc (engine_glut_896_visible);

      glutKeyboardFunc (engine_glut_896_key);
      glutKeyboardUpFunc (engine_glut_896_key_up);
      glutSpecialFunc (engine_glut_896_key_special);
      glutMouseFunc (engine_glut_896_mouse_button);
      glutMotionFunc (engine_glut_896_mouse_move);
      glutPassiveMotionFunc (engine_glut_896_mouse_move);

      glutTimerFunc (100, engine_glut_896_timer, 0);

      glutCreateMenu (engine_glut_896_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_896.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_896.frag")))
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
    case ENGINE_MODE_897:
    {
      struct Engine_OpenGL_GLUT_897_CBData cb_data_s;

      cb_data_s.S1resolutionLoc = -1;
      cb_data_s.S1timeLoc = -1;
      cb_data_s.S1frameLoc = -1;
      cb_data_s.S1mouseLoc = -1;
      cb_data_s.S1channel0Loc = -1;
      cb_data_s.S1channel1Loc = -1;

      cb_data_s.S2resolutionLoc = -1;
      cb_data_s.S2channel0Loc = -1;

      cb_data_s.FBO1 = 0;

      cb_data_s.VAO = 0;
      cb_data_s.VBO = 0;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_897_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_897_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      //glutInitContextVersion (3, 3);
      //glutInitContextProfile (GLUT_CORE_PROFILE);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_897_DEFAULT_WIDTH, ENGINE_GLUT_897_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 897");
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

      glutDisplayFunc (engine_glut_897_draw);
      glutReshapeFunc (engine_glut_897_reshape);
      glutVisibilityFunc (engine_glut_897_visible);

      glutKeyboardFunc (engine_glut_897_key);
      glutKeyboardUpFunc (engine_glut_897_key_up);
      glutSpecialFunc (engine_glut_897_key_special);
      glutSpecialUpFunc (engine_glut_897_key_special_up);

      glutMouseFunc (engine_glut_897_mouse_button);
      glutMotionFunc (engine_glut_897_mouse_move);
      glutPassiveMotionFunc (engine_glut_897_mouse_move);

      glutTimerFunc (100, engine_glut_897_timer, 0);

      glutCreateMenu (engine_glut_897_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader1.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_897.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_897.frag")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load shader, aborting\n")));
        break;
      } // end IF
      if (!cb_data_s.shader2.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_897.vert"),
                                           ACE_TEXT_ALWAYS_CHAR ("glut_897.frag_2")))
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
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_891_S2channel0.png"))) // *TODO*: use custom texture
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
      glGenTextures (1, &cb_data_s.textureS1.id_);
      ACE_ASSERT (cb_data_s.textureS1.id_);
      cb_data_s.textureS1.bind ();
      std::vector<GLfloat> initial_values_a (ENGINE_GLUT_897_DEFAULT_WIDTH * ENGINE_GLUT_897_DEFAULT_HEIGHT * 4, 0.0f);
      // *IMPORTANT NOTE*: generate a floating point-format texture to contain
      //                   the result of shader1
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, ENGINE_GLUT_897_DEFAULT_WIDTH, ENGINE_GLUT_897_DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, initial_values_a.data ());
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
    case ENGINE_MODE_898:
    {
      struct Engine_OpenGL_GLUT_898_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_898_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_898_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_898_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_898_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_898_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_898_DEFAULT_WIDTH, ENGINE_GLUT_898_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 898");
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

      glutDisplayFunc (engine_glut_898_draw);
      glutReshapeFunc (engine_glut_898_reshape);
      glutVisibilityFunc (engine_glut_898_visible);

      glutKeyboardFunc (engine_glut_898_key);
      glutKeyboardUpFunc (engine_glut_898_key_up);
      glutSpecialFunc (engine_glut_898_key_special);
      glutMouseFunc (engine_glut_898_mouse_button);
      glutMotionFunc (engine_glut_898_mouse_move);
      glutPassiveMotionFunc (engine_glut_898_mouse_move);

      glutTimerFunc (100, engine_glut_898_timer, 0);

      glutCreateMenu (engine_glut_898_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_898.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_898.frag")))
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
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_877_channel0.jpg"))) // *TODO*: use custom texture
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

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
    case ENGINE_MODE_899:
    {
      struct Engine_OpenGL_GLUT_899_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_899_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_899_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_899_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_899_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_899_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_899_DEFAULT_WIDTH, ENGINE_GLUT_899_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 899");
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

      glutDisplayFunc (engine_glut_899_draw);
      glutReshapeFunc (engine_glut_899_reshape);
      glutVisibilityFunc (engine_glut_899_visible);

      glutKeyboardFunc (engine_glut_899_key);
      glutKeyboardUpFunc (engine_glut_899_key_up);
      glutSpecialFunc (engine_glut_899_key_special);
      glutMouseFunc (engine_glut_899_mouse_button);
      glutMotionFunc (engine_glut_899_mouse_move);
      glutPassiveMotionFunc (engine_glut_899_mouse_move);

      glutTimerFunc (100, engine_glut_899_timer, 0);

      glutCreateMenu (engine_glut_899_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_899.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_899.frag")))
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
    case ENGINE_MODE_900:
    {
      struct Engine_OpenGL_GLUT_900_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_900_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_900_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_900_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_900_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_900_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_900_DEFAULT_WIDTH, ENGINE_GLUT_900_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 900");
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

      glutDisplayFunc (engine_glut_900_draw);
      glutReshapeFunc (engine_glut_900_reshape);
      glutVisibilityFunc (engine_glut_900_visible);

      glutKeyboardFunc (engine_glut_900_key);
      glutKeyboardUpFunc (engine_glut_900_key_up);
      glutSpecialFunc (engine_glut_900_key_special);
      glutMouseFunc (engine_glut_900_mouse_button);
      glutMotionFunc (engine_glut_900_mouse_move);
      glutPassiveMotionFunc (engine_glut_900_mouse_move);

      glutTimerFunc (100, engine_glut_900_timer, 0);

      glutCreateMenu (engine_glut_900_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_900.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_900.frag")))
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
    case ENGINE_MODE_901:
    {
      struct Engine_OpenGL_GLUT_901_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_901_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_901_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_901_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_901_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_901_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_901_DEFAULT_WIDTH, ENGINE_GLUT_901_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 901");
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

      glutDisplayFunc (engine_glut_901_draw);
      glutReshapeFunc (engine_glut_901_reshape);
      glutVisibilityFunc (engine_glut_901_visible);

      glutKeyboardFunc (engine_glut_901_key);
      glutKeyboardUpFunc (engine_glut_901_key_up);
      glutSpecialFunc (engine_glut_901_key_special);
      glutMouseFunc (engine_glut_901_mouse_button);
      glutMotionFunc (engine_glut_901_mouse_move);
      glutPassiveMotionFunc (engine_glut_901_mouse_move);

      glutTimerFunc (100, engine_glut_901_timer, 0);

      glutCreateMenu (engine_glut_901_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_901.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_901.frag")))
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
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_901_channel0.png")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

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
    case ENGINE_MODE_902:
    {
      struct Engine_OpenGL_GLUT_902_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_902_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_902_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_902_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_902_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_902_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_902_DEFAULT_WIDTH, ENGINE_GLUT_902_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 902");
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

      glutDisplayFunc (engine_glut_902_draw);
      glutReshapeFunc (engine_glut_902_reshape);
      glutVisibilityFunc (engine_glut_902_visible);

      glutKeyboardFunc (engine_glut_902_key);
      glutKeyboardUpFunc (engine_glut_902_key_up);
      glutSpecialFunc (engine_glut_902_key_special);
      glutMouseFunc (engine_glut_902_mouse_button);
      glutMotionFunc (engine_glut_902_mouse_move);
      glutPassiveMotionFunc (engine_glut_902_mouse_move);

      glutTimerFunc (100, engine_glut_902_timer, 0);

      glutCreateMenu (engine_glut_902_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_902.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_902.frag")))
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
    case ENGINE_MODE_903:
    {
      struct Engine_OpenGL_GLUT_903_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_903_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_903_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_903_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_903_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_903_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_903_DEFAULT_WIDTH, ENGINE_GLUT_903_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 903");
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

      glutDisplayFunc (engine_glut_903_draw);
      glutReshapeFunc (engine_glut_903_reshape);
      glutVisibilityFunc (engine_glut_903_visible);

      glutKeyboardFunc (engine_glut_903_key);
      glutKeyboardUpFunc (engine_glut_903_key_up);
      glutSpecialFunc (engine_glut_903_key_special);
      glutMouseFunc (engine_glut_903_mouse_button);
      glutMotionFunc (engine_glut_903_mouse_move);
      glutPassiveMotionFunc (engine_glut_903_mouse_move);

      glutTimerFunc (100, engine_glut_903_timer, 0);

      glutCreateMenu (engine_glut_903_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_903.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_903.frag")))
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
    case ENGINE_MODE_904:
    {
      struct Engine_OpenGL_GLUT_904_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_904_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_904_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_904_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_904_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_904_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_904_DEFAULT_WIDTH, ENGINE_GLUT_904_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 904");
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

      glutDisplayFunc (engine_glut_904_draw);
      glutReshapeFunc (engine_glut_904_reshape);
      glutVisibilityFunc (engine_glut_904_visible);

      glutKeyboardFunc (engine_glut_904_key);
      glutKeyboardUpFunc (engine_glut_904_key_up);
      glutSpecialFunc (engine_glut_904_key_special);
      glutMouseFunc (engine_glut_904_mouse_button);
      glutMotionFunc (engine_glut_904_mouse_move);
      glutPassiveMotionFunc (engine_glut_904_mouse_move);

      glutTimerFunc (100, engine_glut_904_timer, 0);

      glutCreateMenu (engine_glut_904_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_904.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_904.frag")))
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
    case ENGINE_MODE_905:
    {
      struct Engine_OpenGL_GLUT_905_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_905_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_905_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_905_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;
      cb_data_s.channel1Loc = -1;
      cb_data_s.channel2Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_905_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_905_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_905_DEFAULT_WIDTH, ENGINE_GLUT_905_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 905");
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

      glutDisplayFunc (engine_glut_905_draw);
      glutReshapeFunc (engine_glut_905_reshape);
      glutVisibilityFunc (engine_glut_905_visible);

      glutKeyboardFunc (engine_glut_905_key);
      glutKeyboardUpFunc (engine_glut_905_key_up);
      glutSpecialFunc (engine_glut_905_key_special);
      glutMouseFunc (engine_glut_905_mouse_button);
      glutMotionFunc (engine_glut_905_mouse_move);
      glutPassiveMotionFunc (engine_glut_905_mouse_move);

      glutTimerFunc (100, engine_glut_905_timer, 0);

      glutCreateMenu (engine_glut_905_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_905.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_905.frag")))
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
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_905_channel0.png")))
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
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture1.load (ACE_TEXT_ALWAYS_CHAR ("glut_873_channel1.jpg"))) // *TODO*: use custom texture
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
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      if (!cb_data_s.texture2.load (ACE_TEXT_ALWAYS_CHAR ("glut_905_channel2.jpg")))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture2.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

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
    case ENGINE_MODE_906:
    {
      struct Engine_OpenGL_GLUT_906_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_906_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_906_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_906_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_906_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_906_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_906_DEFAULT_WIDTH, ENGINE_GLUT_906_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 906");
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

      glutDisplayFunc (engine_glut_906_draw);
      glutReshapeFunc (engine_glut_906_reshape);
      glutVisibilityFunc (engine_glut_906_visible);

      glutKeyboardFunc (engine_glut_906_key);
      glutKeyboardUpFunc (engine_glut_906_key_up);
      glutSpecialFunc (engine_glut_906_key_special);
      glutMouseFunc (engine_glut_906_mouse_button);
      glutMotionFunc (engine_glut_906_mouse_move);
      glutPassiveMotionFunc (engine_glut_906_mouse_move);

      glutTimerFunc (100, engine_glut_906_timer, 0);

      glutCreateMenu (engine_glut_906_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_906.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_906.frag")))
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
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_873_channel1.jpg"))) // *TODO*: use custom texture
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

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
    case ENGINE_MODE_907:
    {
      struct Engine_OpenGL_GLUT_907_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_907_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_907_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_907_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_907_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_907_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_907_DEFAULT_WIDTH, ENGINE_GLUT_907_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 907");
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

      glutDisplayFunc (engine_glut_907_draw);
      glutReshapeFunc (engine_glut_907_reshape);
      glutVisibilityFunc (engine_glut_907_visible);

      glutKeyboardFunc (engine_glut_907_key);
      glutKeyboardUpFunc (engine_glut_907_key_up);
      glutSpecialFunc (engine_glut_907_key_special);
      glutMouseFunc (engine_glut_907_mouse_button);
      glutMotionFunc (engine_glut_907_mouse_move);
      glutPassiveMotionFunc (engine_glut_907_mouse_move);

      glutTimerFunc (100, engine_glut_907_timer, 0);

      glutCreateMenu (engine_glut_907_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_907.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_907.frag")))
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
    case ENGINE_MODE_908:
    {
      struct Engine_OpenGL_GLUT_908_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_908_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_908_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_908_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.keys0Loc = -1;
      cb_data_s.keys1Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_908_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_908_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      cb_data_s.isPKeyPressed = false;
      cb_data_s.isAKeyPressed = false;
      cb_data_s.isRKeyPressed = false;
      cb_data_s.isCKeyPressed = false;

      cb_data_s.isGKeyPressed = false;
      cb_data_s.isBKeyPressed = false;
      cb_data_s.isDKeyPressed = false;
      cb_data_s.isLKeyPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_908_DEFAULT_WIDTH, ENGINE_GLUT_908_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 908");
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

      glutDisplayFunc (engine_glut_908_draw);
      glutReshapeFunc (engine_glut_908_reshape);
      glutVisibilityFunc (engine_glut_908_visible);

      glutKeyboardFunc (engine_glut_908_key);
      glutKeyboardUpFunc (engine_glut_908_key_up);
      glutSpecialFunc (engine_glut_908_key_special);
      glutMouseFunc (engine_glut_908_mouse_button);
      glutMotionFunc (engine_glut_908_mouse_move);
      glutPassiveMotionFunc (engine_glut_908_mouse_move);

      glutTimerFunc (100, engine_glut_908_timer, 0);

      glutCreateMenu (engine_glut_908_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_908.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_908.frag")))
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
      cb_data_s.keys0Loc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iKeys0"));
      ACE_ASSERT (cb_data_s.keys0Loc != -1);
      cb_data_s.keys1Loc =
        glGetUniformLocation (cb_data_s.shader.id_, ACE_TEXT_ALWAYS_CHAR ("iKeys1"));
      ACE_ASSERT (cb_data_s.keys1Loc != -1);

      // START TIMING
      cb_data_s.tp1 = std::chrono::high_resolution_clock::now ();

      glutMainLoop ();

      cb_data_s.shader.reset ();

      result = true;

      break;
    }
    case ENGINE_MODE_909:
    {
      struct Engine_OpenGL_GLUT_909_CBData cb_data_s;

      cb_data_s.scaleFactor = ENGINE_GLUT_909_DEFAULT_SCALE_FACTOR;
      cb_data_s.columns = ENGINE_GLUT_909_DEFAULT_WIDTH / cb_data_s.scaleFactor;
      cb_data_s.rows = ENGINE_GLUT_909_DEFAULT_HEIGHT / cb_data_s.scaleFactor;

      cb_data_s.resolutionLoc = -1;
      cb_data_s.timeLoc = -1;
      cb_data_s.mouseLoc = -1;
      cb_data_s.channel0Loc = -1;

      cb_data_s.wireframe = false;

      cb_data_s.mouseX = ENGINE_GLUT_909_DEFAULT_WIDTH / 2;
      cb_data_s.mouseY = ENGINE_GLUT_909_DEFAULT_HEIGHT / 2;
      cb_data_s.mouseLMBPressed = false;

      // initialize GLUT
      glutInit (&argc_in, argv_in);
      glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
      glutInitWindowSize (ENGINE_GLUT_909_DEFAULT_WIDTH, ENGINE_GLUT_909_DEFAULT_HEIGHT);

      int window_i = glutCreateWindow ("engine GLUT 909");
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

      glutDisplayFunc (engine_glut_909_draw);
      glutReshapeFunc (engine_glut_909_reshape);
      glutVisibilityFunc (engine_glut_909_visible);

      glutKeyboardFunc (engine_glut_909_key);
      glutKeyboardUpFunc (engine_glut_909_key_up);
      glutSpecialFunc (engine_glut_909_key_special);
      glutMouseFunc (engine_glut_909_mouse_button);
      glutMotionFunc (engine_glut_909_mouse_move);
      glutPassiveMotionFunc (engine_glut_909_mouse_move);

      glutTimerFunc (100, engine_glut_909_timer, 0);

      glutCreateMenu (engine_glut_909_menu);
      glutAddMenuEntry (ACE_TEXT_ALWAYS_CHAR ("wireframe"), 0);
      glutAttachMenu (GLUT_RIGHT_BUTTON);

      if (!cb_data_s.shader.loadFromFile (ACE_TEXT_ALWAYS_CHAR ("glut_909.vert"),
                                          ACE_TEXT_ALWAYS_CHAR ("glut_909.frag")))
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
      if (!cb_data_s.texture0.load (ACE_TEXT_ALWAYS_CHAR ("glut_885_channel0.png"), false)) // *TODO*: use custom texture
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to load texture, aborting\n")));
        break;
      } // end IF
      cb_data_s.texture0.bind ();
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap (GL_TEXTURE_2D);

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
