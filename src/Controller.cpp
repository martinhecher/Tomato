// **********************************************************************
// * \author Martin Hecher                                              *
// *         mailto:martin.hecher@fraunhofer.at                         *
// * \date   2011-12-23                                                 *
// **********************************************************************

#include "Controller.hpp"
#include <iostream>

namespace Tomato
{
   Controller* Controller::m_self = NULL;

   Controller* Controller::the()
   {
      if(m_self == NULL)
      {
         m_self = new Controller();
      }

      return m_self;
   }

   void Controller::destroy()
   {
      delete m_self;
   }

   /*! Sets the internal start time of the Controller.
    */
   void Controller::init(Time time)
   {
      m_startTime = time;
      m_lastTime  = m_startTime;
   }

   /*! Updates the animation with the given timestamp.
    */
   void Controller::update(Time timeStamp)
   {
      // The timespan since this method was last called:
      Time controllerLastCalledTimeSpan = timeStamp - m_lastTime;

      AnimationStoreIter animationIter(m_animations.begin());
      AnimationStoreIter end          (m_animations.end());

      for(;animationIter!=end;++animationIter)
      {
         Animation* animation = *animationIter;

         if(animation->isActive())
         {
//            std::cout << "Processing animation " << animation->getName()
//                      << std::endl;

            float animFrameDuration   = 1000 / animation->getFrameRate();
            Time  animLastCalledTimeSpan = timeStamp - animation->getLastRunTime();
//            std::cout << "animFrameDuration            = "
//                      << animFrameDuration << std::endl;
//            std::cout << "animLastCalledTimeSpan       = "
//                      << animLastCalledTimeSpan << std::endl;
//            std::cout << "controllerLastCalledTimeSpan = "
//                      << controllerLastCalledTimeSpan << std::endl;

            if(animLastCalledTimeSpan >= animFrameDuration)
            {
               animation->update(timeStamp);

               // TODO: this is not very accurate, as each time a few
               // milliseconds get lost. Therefore the desired framerate
               // will not be accurate...
               animation->setLastRunTime(timeStamp);
            }
         }
      }

      m_lastTime = timeStamp;
   }

   /*! Creates a new animation with the given name.
    */
   Animation* Controller::createAnimation(const std::string& name)
   {
      m_animations.push_back(new Animation(name));
      m_animations.back()->setLastRunTime(0); // TODO: not correct, but works
      return m_animations.back();
   }

   /*! Returns the animation with the given name. If the animation does not
       exist NULL is returned.
    */
   Animation* Controller::getAnimation(const std::string& name)
   {
      AnimationStoreConstIter iter(m_animations.begin());
      AnimationStoreConstIter end (m_animations.end());

      for(;iter!=end;++iter)
      {
         if((*iter)->getName().compare(name) == 0)
         {
            return *iter;
         }
      }

      return NULL;
   }

   Action* Controller::createAction(const std::string& name)
   {
      m_actions.push_back(new Action(name));
      return m_actions.back();
   }

   Controller::Controller()
      : m_animations(    )
      , m_startTime (  -1)
   {
   }

   Controller::~Controller()
   {
      AnimationStoreIter iter(m_animations.begin());
      AnimationStoreIter end (m_animations.end());

      for(;iter!=end;++iter)
      {
         delete *iter;
      }
      m_animations.clear();
   }
} // end namespace Tomato
