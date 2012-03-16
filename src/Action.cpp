// **********************************************************************
// * \author Martin Hecher                                              *
// *         mailto:martin.hecher@fraunhofer.at                         *
// * \date   2011-12-23                                                 *
// **********************************************************************

#include "Action.hpp"
#include <iostream>
#include <cmath>

namespace Tomato
{

   Action::Action(const std::string& name)
      : m_fps           (  -1)
      , m_numFrames     (  -1)
      , m_startFrame    (   0)
      , m_curFrame      (   0)
      , m_name          (name)
   {
   }

   /*! Updates the Action.
    */
   void Action::update(int curFrame, int numFrames, int fps, void* userData)
   {
      //std::cout << "Updated Action " << m_name
      //          << " at frame " << frameNumber << std::endl;

      // TODO: add possibility to slow down update ratio!

      if(numFrames == 0)
      {
         std::cout << "WARNING: Action " << this->getName()
                   << "has a length of 0 frames. Skipping..." << std::endl;
         return;
      }

      bool useEaseFunction = false;
      if(useEaseFunction)
      {
         float fac = std::sin(M_PI_2 / float(numFrames) * m_curFrame);
         std::cout << "m_curFrame / m_curFrame * fac = " << m_curFrame << " / "
                   << m_curFrame * fac << std::endl;

         onUpdate(m_curFrame * fac, m_numFrames, fps, userData);
      }
      else
      {
         std::cout << "m_curFrame / m_numFrames = " << m_curFrame << " / "
                   << numFrames << std::endl;
         onUpdate(m_curFrame, m_numFrames, fps, userData);
      }

      if(m_curFrame == m_numFrames)
      {
         m_curFrame = 0;
      }
      else
      {
         m_curFrame++;
      }
   }

   void Action::setNumFrames(int frames)
   {
      m_numFrames = frames;
   }

   int Action::getNumFrames()
   {
      return m_numFrames;
   }

   void Action::setStartFrame(int frame)
   {
      m_startFrame = frame;
   }

   int Action::getStartFrame()
   {
      return m_startFrame;
   }

   int Action::getEndFrame()
   {
      return m_startFrame + m_numFrames;
   }

   void Action::setUserdata(void* userData)
   {
      m_userData = userData;
   }

   void* Action::getUserdata()
   {
      return m_userData;
   }

   const std::string& Action::getName()
   {
      return m_name;
   }

   Action::Action(const Action& source)
   {
      // NIY
   }

   void Action::operator=(const Action& source)
   {
      // NIY
   }

   Action::~Action()
   {
      m_userData = NULL;
   }

} // end namespace Tomato
