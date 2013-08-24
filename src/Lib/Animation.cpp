// **********************************************************************
// * \author Martin Hecher                                              *
// *         mailto:martin.hecher@fraunhofer.at                         *
// * \date   2011-12-23                                                 *
// **********************************************************************

#include "Animation.hpp"
#include <iostream>

// #define DEBUG_UPDATE

namespace Tomato
{
   Animation::Animation(const std::string& name)
      : m_actions       (    )
      , m_fps           (  -1)
      , m_numFrames     (   0)
      , m_curFrame      (   0)
      , m_isActive      (true)
      , m_elapsedTime   (  -1)
      , m_startTime     (  -1)
      , m_lastRunTime   (  -1)
      , m_name          (name)
   {
   }

   /*! Adds an Action to the Animation. As default the Action is appended on the
       end of the Animation (e.g. the Action's start frame is set to the
       Animations total number of frames). To change the start of the Action's
       setStartFrame() method.
    */
   void Animation::add(Action* action)
   {
      m_actions.push_back(action);

      action->setStartFrame(this->getNumFrames());
      m_numFrames += action->getNumFrames();
   }

   /*! Updates the Animation.
 */
   void Animation::update(Time timeStamp)
   {
#ifdef DEBUG_UPDATE_VERBOSE
      std::cout << "Processing Animation " << m_name
                << " at time " << timeStamp << std::endl;
#endif

      if(m_elapsedTime == -1)
      {
         m_elapsedTime = 0;
         m_startTime   = timeStamp;
      }

      // TODO: add possibility to slow down update ratio!

      ActionStoreIter actionIter(m_actions.begin());
      ActionStoreIter end       (m_actions.end());
      for(;actionIter!=end;++actionIter)
      {
         Action *action = *actionIter;

         if(m_curFrame >= action->getStartFrame() &&
            m_curFrame <= action->getStartFrame() + action->getNumFrames())
         {
#ifdef DEBUG_UPDATE
      std::cout << "    Updating Action " << m_name
                << " at time " << timeStamp << std::endl;
#endif
            action->update(m_curFrame,
                           action->getNumFrames(),
                           m_fps,
                           action->getUserdata());
         }
      }

      if(m_curFrame == m_numFrames)
      {
         m_isActive = false;
         m_elapsedTime = -1;
         m_startTime   = -1;
         m_curFrame    =  0;

#ifdef DEBUG_UPDATE
      std::cout << "Finished Animation " << m_name
                << " at time " << timeStamp << std::endl;
#endif
      }
      else
      {
         m_curFrame++;
         m_elapsedTime = timeStamp - m_startTime;
      }
   }

   /*! Sets the framerate per second for this Animation.
    */
   void Animation::setFrameRate(int fps)
   {
      m_fps = fps;
   }

   /*! Returns the framerate for this Animation.
    */
   int Animation::getFrameRate()
   {
      return m_fps;
   }

   /*! Returns the number of total frames for this Animation.
    */
   int Animation::getNumFrames()
   {
      return m_numFrames;
   }

   /*! Sets the time the update method of the Animation was executed for the
       last time.
    */
   void Animation::setLastRunTime(Time time)
   {
      m_lastRunTime = time;
   }

   /*! Returns the time the update method of the Animation was executed for the
       last time.
    */
   Time Animation::getLastRunTime()
   {
      return m_lastRunTime;
   }

   /*! Activates/Deactivates the frame. On creation an Animation is activated.
    */
   void Animation::setActive(bool flag)
   {
      m_isActive = flag;
   }

   /*! Returns true if an Animation is active, false otherwise.
    */
   bool Animation::isActive()
   {
      return m_isActive;
   }

   Animation::Animation(const Animation& source)
   {
      // NIY
   }

   void Animation::operator=(const Animation& source)
   {
      // NIY
   }

   Animation::~Animation()
   {
   }

} // end namespace Tomato
