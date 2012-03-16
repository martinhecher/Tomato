// Tomato Tutorial Example: Simple Animation
//
// This example shows how to create two Actions and combine them to an
// Animation.

#include "OSGGLUT.h"
#include "OSGConfig.h"
#include "OSGSimpleGeometry.h"
#include "OSGGLUTWindow.h"
#include "OSGSimpleSceneManager.h"
#include "OSGSceneFileHandler.h"
#include "OSGGradientBackground.h"
#include "OSGFieldContainerUtils.h"
#include "OSGLine.h"
#include "OSGMatrixUtility.h"
#include "OSGVoidPAttachment.h"

// Tomato includes:
#include "Controller.hpp"

// BoardGame includes:
#include "Figure.hpp"

/* ---------------------------------------------------------------------------*/
/* Globals:                                                                   */

struct Globals
{
   OSG::SimpleSceneManagerRefPtr mgr;

   OSG::UInt16 fps;
   OSG::UInt16 speed;            // in meter per second
   OSG::UInt16 figureAnimLength; // in fps

   OSG::Vec3f moveLeft;
   OSG::Vec3f moveRight;
   OSG::Vec3f moveUp;
   OSG::Vec3f moveDown;

   OSG::Vec3f camDirection;

   OSG::TransformUnrecPtr figure1TransC;
   OSG::TransformUnrecPtr figure2TransC;

   OSG::NodeUnrecPtr selectedNode;
} G;

/* ---------------------------------------------------------------------------*/
/* Init globals:                                                              */

void initGlobals()
{
   G.fps   = 25;
   G.speed = 3;
   G.figureAnimLength = 30;

   // Initialize direction vectors. The vectors encode direction and
   // velocity:
   G.moveLeft     = OSG::Vec3f(-1, 0, 0) * G.speed;
   G.moveRight    = OSG::Vec3f( 1, 0, 0) * G.speed;
   G.moveUp       = OSG::Vec3f( 0, 0,-1) * G.speed;
   G.moveDown     = OSG::Vec3f( 0, 0, 1) * G.speed;

   G.camDirection = OSG::Vec3f( 0, 1, 0) * G.speed;

   G.selectedNode = 0;
}

/* ---------------------------------------------------------------------------*/
/* Helper functions:                                                          */

void setEyePositionHelper(const OSG::Pnt3f& from,
                          const OSG::Pnt3f& at,
                          const OSG::Vec3f& up)
{
   G.mgr->getNavigator()->setFrom(from);
   G.mgr->getNavigator()->setAt  (at);
   G.mgr->getNavigator()->setUp  (up);
}

OSG::Vec3f calcTranslation(OSG::Real32 t, OSG::Vec3f dir)
{
   // The 'dir' vector encodes direction and velocity
   OSG::Real32 v = dir.length();
   OSG::Real32 s = v * t;
   return(dir * s);
}

OSG::Vec3f getCurrentTranslation(OSG::Matrix& matrix)
{
   OSG::Vec3f curTrans, scale;
   OSG::Quaternion rot;
   matrix.getTransform(curTrans, rot, scale, rot);

   return(curTrans);
}

void moveTo(int curFrame, int numFrames, int fps, void* userData)
{
   OSG::Vec3f* dir = static_cast<OSG::Vec3f*>(userData);
   float t = curFrame / (float)fps;
   OSG::Vec3f newAbsolutePosition = calcTranslation(t, *dir);

   OSG::Vec3f lastAbsolutePosition(0,0,0);
   if(curFrame - 1 > 0)
   {
      t = (curFrame - 1) / (float)fps;
      lastAbsolutePosition = calcTranslation(t, *dir);
   }

   OSG::Matrix& mat       = G.figure1TransC->editMatrix();
   OSG::Vec3f curPosition = getCurrentTranslation(mat);

   OSG::Vec3f diffPosition = newAbsolutePosition - lastAbsolutePosition;
   OSG::Vec3f newPosition = curPosition + diffPosition;

   mat.setIdentity();
   mat.setTranslate(newPosition);
   G.figure1TransC->setMatrix(mat);
}

void setEyePosition(int curFrame, int numFrames, int fps, void* userData)
{
   OSG::Navigator* nav = G.mgr->getNavigator();
   OSG::Vec3f *dir = static_cast<OSG::Vec3f*>(userData);
   float t = curFrame / (float)fps;

   OSG::Vec3f newAbsolutePosition = calcTranslation(t, *dir);
   OSG::Vec3f lastAbsolutePosition(0,0,0);
   if(curFrame - 1 > 0)
   {
      t = (curFrame - 1) / (float)fps;
      lastAbsolutePosition = calcTranslation(t, *dir);
   }

   OSG::Vec3f diffPosition = newAbsolutePosition - lastAbsolutePosition;
   OSG::Pnt3f newPosition  = nav->getFrom().subZero() + diffPosition;

   setEyePositionHelper(newPosition, nav->getAt(), nav->getUp());
}

/* ---------------------------------------------------------------------------*/
/* Creates an (inactive) animation that moves the figure into direction 'dir' */
/* with the given animation parameters:                                       */

void createMoveToAnimation(OSG::Vec3f& dir,
                           const std::string& name,
                           int numFrames,
                           int fps)
{
   Tomato::Action* moveToAction =
         Tomato::Controller::the()->createAction(name);
   moveToAction->setNumFrames(numFrames);
   moveToAction->setUserdata(&dir);
   moveToAction->onUpdate = boost::bind<void>(&moveTo, _1, _2, _3, _4);

   Tomato::Animation* moveToAnimation =
         Tomato::Controller::the()->createAnimation(name);
   moveToAnimation->setFrameRate(fps);
   moveToAnimation->add(moveToAction);
   moveToAnimation->setActive(false);
}

/* ---------------------------------------------------------------------------*/
/* Create animations in scene:                                                */
void createAnimations()
{
   Tomato::Time time = OSG::getTimeStampMsecs(OSG::getTimeStamp());
   Tomato::Controller::the()->init(time);

//   Tomato::Animation* camAnimation =
//         Tomato::Controller::the()->createAnimation("MyCameraAnimation");
//   camAnimation->setFrameRate(25);
//   camAnimation->add(moveCameraAction);

   createMoveToAnimation(G.moveDown , "moveDown" , G.figureAnimLength, G.fps);
   createMoveToAnimation(G.moveUp   , "moveUp"   , G.figureAnimLength, G.fps);
   createMoveToAnimation(G.moveLeft , "moveLeft" , G.figureAnimLength, G.fps);
   createMoveToAnimation(G.moveRight, "moveRight", G.figureAnimLength, G.fps);
}

/* ---------------------------------------------------------------------------*/
/* Create scene geometry:                                                     */

OSG::NodeTransitPtr createScene(OSG::Window *win)
{
   OSG::NodeRecPtr rootN = OSG::makeNodeFor(OSG::Group::create());

   // Create ground:
   OSG::NodeUnrecPtr groundN = OSG::makePlane(25,25,1,1);
   OSG::Matrix m;
   OSG::Quaternion q;
   q.setValueAsAxisDeg(OSG::Vec3f(1,0,0), -90);
   m.setRotate(q);

   OSG::TransformUnrecPtr groundTransC = OSG::Transform::create();
   groundTransC->setMatrix(m);

   OSG::NodeUnrecPtr groundTransN = OSG::makeNodeFor(groundTransC);
   groundTransN->addChild(groundN);
   rootN->addChild(groundTransN);

   // Set ground material:
   OSG::SimpleMaterialUnrecPtr mat = OSG::SimpleMaterial::create();
   mat->setDiffuse(OSG::Color3f(0.8,0.8,0.8));
   dynamic_cast<OSG::Geometry*>(groundN->getCore())->setMaterial(mat);

//   // Create figure:
//   OSG::NodeUnrecPtr figure1N =
//         OSG::SceneFileHandler::the()->read("../Models/Figure.obj");

//   G.figure1TransC = OSG::Transform::create();
//   OSG::NodeUnrecPtr trans1N = OSG::makeNodeFor(G.figure1TransC);
//   trans1N->addChild(figure1N);
//   rootN->addChild(trans1N);

   OSG::NodeUnrecPtr figureModelA =
         OSG::SceneFileHandler::the()->read("../assets/Figure.obj");

   BoardGame::Figure* figureA = new BoardGame::Figure();
   figureA->setModel(figureModelA);
   rootN->addChild(figureA->getRoot());

   OSG::NodeUnrecPtr figureModelB =
         OSG::SceneFileHandler::the()->read("../assets/Figure.obj");

   BoardGame::Figure* figureB = new BoardGame::Figure();
   figureB->setModel(figureModelB);
   rootN->addChild(figureB->getRoot());

   G.selectedNode = figureModelA;

   return(OSG::NodeTransitPtr(rootN));
}

/* ---------------------------------------------------------------------------*/
/* Command line argument handling:                                            */

void handleCommandLineArguments(int argc, char *argv[])
{
   //    if(argc == 4)
   //    {
   //        gmlGlobals.userlib      = argv[1];
   //        gmlGlobals.script       = argv[2];
   //        gmlGlobals.materialPath = argv[3];
   //    }
   //    else
   //    {
   //        std::string usage = "Wrong arguments!\nUsage:\n   test-osgrimaldo-v1 \"/path/to/userlib.xgml\" \"my.script\" \"/path/to/tubs.mtl\"\n";
   //        std::cout << usage << std::endl;
   //        exit(-1);
   //    }
}

/* ---------------------------------------------------------------------------*/
/* GLUT functions:                                                            */

void display(void)
{
   Tomato::Controller::the()->update(OSG::getTimeStampMsecs(OSG::getTimeStamp()));

   G.mgr->redraw();

   OSG::Thread::getCurrentChangeList()->clear();
}

void reshape(int w, int h)
{
   G.mgr->resize(w, h);
   glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
   if (state)
   {
      G.mgr->mouseButtonRelease(button, x, y);
   }
   else
   {
      OSG::Line ray = G.mgr->calcViewRay(x, y);
      OSG::IntersectActionRefPtr ia = OSG::IntersectAction::create();
      ia->setLine(ray);
      ia->apply(G.mgr->getRoot());

      if(ia->didHit())
      {
         G.selectedNode = ia->getHitObject();

         if(OSG::getVoidP(G.selectedNode))
         {
            if(static_cast<BoardGame::Figure*>(OSG::getVoidP(G.selectedNode)))
            {
               std::cout << "Selected figure" << std::endl;
               ia = 0;
               return;
            }
         }
      }
      ia = 0;

      G.mgr->mouseButtonPress(button, x, y);
   }

   glutPostRedisplay();
}

void motion(int x, int y)
{
   G.mgr->mouseMove(x, y);

   glutPostRedisplay();
}

void moveLeft()
{
   if(G.selectedNode)
   {
      BoardGame::Figure* figure =
            static_cast<BoardGame::Figure*>(OSG::getVoidP(G.selectedNode));

      if(figure)
      {
         figure->moveLeft();
      }
      else
      {
         std::cout << "No figure selected!" << std::endl;
      }
   }
   else
   {
      std::cout << "Select a figure first!" << std::endl;
   }
}

void moveRight()
{
   if(G.selectedNode)
   {
      BoardGame::Figure* figure =
            static_cast<BoardGame::Figure*>(OSG::getVoidP(G.selectedNode));

      if(figure)
      {
         figure->moveRight();
      }
      else
      {
         std::cout << "No figure selected!" << std::endl;
      }
   }
   else
   {
      std::cout << "Select a figure first!" << std::endl;
   }
}

void moveUp()
{
   if(G.selectedNode)
   {
      BoardGame::Figure* figure =
            static_cast<BoardGame::Figure*>(OSG::getVoidP(G.selectedNode));

      if(figure)
      {
         figure->moveUp();
      }
      else
      {
         std::cout << "No figure selected!" << std::endl;
      }
   }
   else
   {
      std::cout << "Select a figure first!" << std::endl;
   }
}

void moveDown()
{
   if(G.selectedNode)
   {
      BoardGame::Figure* figure =
            static_cast<BoardGame::Figure*>(OSG::getVoidP(G.selectedNode));

      if(figure)
      {
         figure->moveDown();
      }
      else
      {
         std::cout << "No figure selected!" << std::endl;
      }
   }
   else
   {
      std::cout << "Select a figure first!" << std::endl;
   }
}

void keyboard(unsigned char k, int , int )
{
   switch(k)
   {
   case 27:
   {
      G.figure1TransC = 0;
      G.figure2TransC = 0;

      G.mgr = 0;

      Tomato::Controller::destroy();

      OSG::osgExit();
      exit(0);
   }
      break;

      // a,s,d,w movement control ----------------------------------------------
   case 'a':
   {
      moveLeft();
   }
      break;

   case 'd':
   {
      moveRight();
   }
      break;

   case 'w':
   {
      moveUp();
   }
      break;

   case 's':
   {
      moveDown();
   }
      break;
   // --------------------------------------------------------------------------

   case 'n':
   {
      G.figure1TransC->editMatrix().setIdentity();
      Tomato::Animation* figureAnim =
            Tomato::Controller::the()->getAnimation("MyFigureAnimation");
      assert(figureAnim);
      figureAnim->setActive(true);

      // Set the eye position:
      OSG::Pnt3f from(18,18,18);
      OSG::Pnt3f at  (0,0,0);
      OSG::Vec3f up  (0,1,0);
      setEyePositionHelper(from, at, up);

      Tomato::Animation* camAnim =
            Tomato::Controller::the()->getAnimation("MyCameraAnimation");
      assert(camAnim);
      camAnim->setActive(true);
   }
      break;

   case 'f':
      G.mgr->setNavigationMode(OSG::Navigator::FLY);
      break;
   case 't':
      G.mgr->setNavigationMode(OSG::Navigator::TRACKBALL);
      break;
   case 'q':
      G.mgr->setStatistics(true);
      break;
   case 'Q':
      G.mgr->setStatistics(false);
      break;

   case 'r':
   {
      bool useTrav = !G.mgr->getUseTraversalAction();
      G.mgr->setUseTraversalAction(useTrav);
      printf("Using %s action.\n", useTrav ? "render traversal" : "render");
   }
      break;

   case 'p':
   {
      std::cout << "Scanning memory consumption." << std::endl;
      OSG::MemoryConsumption mc;
      mc.scan();
      mc.print(std::cout);
   }
      break;
   }
}

/* ---------------------------------------------------------------------------*/
/* Setup GLUT:                                                                */

int setupGLUT(int *argc, char *argv[])
{
   glutInit(argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

   int winid = glutCreateWindow("OpenSG");

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);

   return winid;
}

/* ---------------------------------------------------------------------------*/
/* Main method:                                                               */
int main(int argc, char **argv)
{
   handleCommandLineArguments(argc, argv);

   // OSG init
   OSG::osgInit(argc,argv);

   // GLUT init
   int winid = setupGLUT(&argc, argv);

   initGlobals();

   {
      OSG::GLUTWindowUnrecPtr gwin= OSG::GLUTWindow::create();
      gwin->setGlutId(winid);
      gwin->init();

      OSG::NodeUnrecPtr scene = createScene(gwin);

      createAnimations();

      OSG::commitChanges();

      // Create the SimpleSceneManager helper
      G.mgr = OSG::SimpleSceneManager::create();
      G.mgr->setUseTraversalAction(true);

      // Tell the manager what to manage:
      G.mgr->setWindow(gwin );
      G.mgr->setRoot  (scene);

      // Create a background:
      OSG::GradientBackgroundUnrecPtr bg = OSG::GradientBackground::create();
      bg->addLine(OSG::Color3f(0.9f, 0.9f, 1.0f), 0);
      bg->addLine(OSG::Color3f(0.0f, 0.3f, 0.9f), 1);
      G.mgr->setBackground(bg);

      // Set the eye position:
      OSG::Pnt3f from(18,18,18);
      OSG::Pnt3f at  (0,0,0);
      OSG::Vec3f up  (0,1,0);
      setEyePositionHelper(from, at, up);
   }

   // GLUT main loop
   glutMainLoop();

   return 0;
}

/* ---------------------------------------------------------------------------*/


//void myAnimationCallback(int curFrame, int numFrames, int fps)
//{
//   std::cout << "Updated myAnimationCallback (curFrame = "
//             << curFrame << " / numFrames = " << numFrames
//             << " / fps = " << fps << std::endl;

//   OSG::Matrix m;
//   OSG::Vec3f from(0,1,0);
//   OSG::Vec3f to  (0,1,0);
//   OSG::Quaternion rot_from;
//   rot_from.setValueAsAxisDeg(from, 0);
//   OSG::Quaternion rot_to;
//   rot_to.setValueAsAxisDeg(to, 90);

//   std::cout << "Percentage of Action = "
//             << 100.f/numFrames * curFrame << std::endl;

//   OSG::Quaternion result =
//         OSG::Quaternion::slerp(rot_from, rot_to,
//                                (int)(numFrames/100.f * curFrame));
//   G.rotMatrix.setIdentity();
//   G.rotMatrix.setRotate(result);

//   G.transC->setMatrix(G.rotMatrix);
//}
