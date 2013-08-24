// **********************************************************************
// * \author Martin Hecher                                              *
// *         mailto:martin.hecher@fraunhofer.at                         *
// * \date   2011-12-28                                                 *
// **********************************************************************

#include "Figure.hpp"
#include "Controller.hpp"

#include <OpenSG/OSGTransform.h>
#include <OpenSG/OSGVoidPAttachment.h>

#include <boost/bind.hpp>

namespace BoardGame
{
   Figure::Figure()
   {
      OSG::GroupUnrecPtr groupC = OSG::Group::create();
      m_modelN = OSG::makeNodeFor(groupC);

      m_transC = OSG::Transform::create();
      m_rootN  = OSG::makeNodeFor(m_transC);

      m_rootN->addChild(m_modelN);

      m_speed        =  2;
      m_moveToLength = 10;
      m_fps          = 25;

      m_directions[0] = OSG::Vec3f(-1, 0, 0) * m_speed;
      m_directions[1] = OSG::Vec3f( 1, 0, 0) * m_speed;
      m_directions[2] = OSG::Vec3f( 0, 0, 1) * m_speed;
      m_directions[3] = OSG::Vec3f( 0, 0,-1) * m_speed;

      createAnimations();
   }

   Figure::~Figure()
   {
      m_transC = 0;
      m_modelN = 0;
      m_rootN  = 0;
   }

   void Figure::moveLeft()
   {
      Tomato::Animation* moveRight =
            Tomato::Controller::the()->getAnimation("Figure.moveLeft");
      moveRight->setActive(true);
   }

   void Figure::moveRight()
   {
      Tomato::Animation* moveRight =
            Tomato::Controller::the()->getAnimation("Figure.moveRight");
      moveRight->setActive(true);
   }

   void Figure::moveUp()
   {
      Tomato::Animation* moveRight =
            Tomato::Controller::the()->getAnimation("Figure.moveUp");
      moveRight->setActive(true);
   }

   void Figure::moveDown()
   {
      Tomato::Animation* moveRight =
            Tomato::Controller::the()->getAnimation("Figure.moveDown");
      moveRight->setActive(true);
   }

   void Figure::setModel(OSG::Node* modelN)
   {
      m_modelN->addChild(modelN);

      // Allow to get the figure corresponding to the model node via a
      // VoidP attachment:
      OSG::setVoidP(modelN, this);
   }

   OSG::NodeTransitPtr Figure::getRoot()
   {
      return OSG::NodeTransitPtr(m_rootN);
   }
   /* ---------------------------------------------------------------------------*/
   /* Create animations in scene:                                                */
   void Figure::createAnimations()
   {
      createMoveToAnimation(m_directions[0] , "Figure.moveRight");
      createMoveToAnimation(m_directions[1] , "Figure.moveLeft");
      createMoveToAnimation(m_directions[2] , "Figure.moveUp");
      createMoveToAnimation(m_directions[3] , "Figure.moveDown");
   }

   /* ---------------------------------------------------------------------------*/
   /* Creates an (inactive) animation that moves the figure into direction 'dir' */
   /* with the given animation parameters:                                       */
   void Figure::createMoveToAnimation(OSG::Vec3f& dir,
                                      const std::string& name)
   {
      Tomato::Action* moveToAction =
            Tomato::Controller::the()->createAction(name);
      moveToAction->setNumFrames(m_moveToLength);
      moveToAction->setUserdata(&dir);
      moveToAction->onUpdate =
            boost::bind<void>(&Figure::moveTo, this, _1, _2, _3, _4);

      Tomato::Animation* moveToAnimation =
            Tomato::Controller::the()->createAnimation(name);
      moveToAnimation->setFrameRate(m_fps);
      moveToAnimation->add(moveToAction);
      moveToAnimation->setActive(false);
   }

   /*! Linearly moves the figure into the diretion LinearMovement.dir with
       the speed LinearMovement.speed.
    */
   void Figure::moveTo(int curFrame, int numFrames, int fps, void* userData)
   {
      OSG::Vec3f* dir = static_cast<OSG::Vec3f*>(userData);

      float t = curFrame / fps;

      //std::cout << "curFrame: " << curFrame << std::endl;
      //std::cout << "numFrames: " << numFrames << std::endl;

      OSG::Vec3f newAbsolutePosition = computeTranslation(t, *dir);
      //std::cout << "newAbsolutePosition:  " << newAbsolutePosition << std::endl;

      OSG::Vec3f lastAbsolutePosition(0,0,0);
      if(curFrame - 1 > 0)
      {
         t = (curFrame - 1) / (float)fps;
         lastAbsolutePosition = computeTranslation(t, *dir);
      }
      //std::cout << "lastAbsolutePosition: " << lastAbsolutePosition << std::endl;

      OSG::Vec3f curPosition = getCurrentTranslation(m_transC->getMatrix());

      //std::cout << "curPosition: " << curPosition << std::endl;

      OSG::Vec3f diffPosition = newAbsolutePosition - lastAbsolutePosition;
      OSG::Vec3f newPosition = curPosition + diffPosition;

      OSG::Matrix& mat = m_transC->editMatrix();
      mat.setIdentity();
      mat.setTranslate(newPosition);

      //std::cout << "newPosition: " << newPosition << std::endl;
   }

   OSG::Vec3f Figure::computeTranslation(OSG::Real32 t, const OSG::Vec3f& dir)
   {
      // The 'dir' vector encodes direction and velocity
      OSG::Real32 v = dir.length();
      OSG::Real32 s = v * t;

      std::cout << "t: " << t << std::endl;
      std::cout << "dir: " << dir << std::endl;
      std::cout << "v: " << v << std::endl;
      std::cout << "s: " << s << std::endl;

      return(dir * s);
   }

   OSG::Vec3f Figure::getCurrentTranslation(const OSG::Matrix& matrix)
   {
      OSG::Vec3f curTrans, scale;
      OSG::Quaternion rot;
      matrix.getTransform(curTrans, rot, scale, rot);

      return(curTrans);
   }

} // end namespace BoardGame
