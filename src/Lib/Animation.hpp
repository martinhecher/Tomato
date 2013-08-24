// **********************************************************************
// * \author Martin Hecher                                              *
// *         mailto:martin.hecher@fraunhofer.at                         *
// * \date   2011-12-23                                                 *
// **********************************************************************

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "Controller.hpp" // TODO: replace with something like BaseConfig.hpp?

#include <string>
#include <boost/function.hpp>

namespace Tomato
{
   /*! An Animation consists of one or multiple Actions. A framerate and a total
       amount of frames has to be specified. The Animation (respectively its
       contained Actions) is updated by the AnimationController according to the
       framerate.

       \see Action, AnimationController
   */
   class Animation
   {
   public:
      /* ---------------------------------------------------------------------*/
      /* Constructor                                                          */

      Animation(const std::string &name);

      /* ---------------------------------------------------------------------*/
      /* Action management                                                    */

      void add(Action* action);

      /* ---------------------------------------------------------------------*/
      /* Update                                                               */

      void update(Time timeStamp);

      /* ---------------------------------------------------------------------*/
      /* Frame statistics                                                     */

      void setFrameRate(int fps);
      int  getFrameRate();

      int  getNumFrames();

      void setLastRunTime(Time time);
      Time getLastRunTime();

      /* ---------------------------------------------------------------------*/
      /* State management                                                     */

      void setActive(bool flag);
      bool isActive();

      /* ---------------------------------------------------------------------*/
      /* Identification                                                       */

      const std::string& getName() { return m_name; }

   private:
      friend class Controller;

      Animation(const Animation &source);
      void operator=(const Animation &source);
      virtual ~Animation();

      ActionStore m_actions;

      int         m_fps;
      int         m_numFrames;
      int         m_curFrame;

      Time        m_elapsedTime;
      Time        m_startTime;
      Time        m_lastRunTime;

      bool        m_isActive;

      std::string m_name;
   };

} // end namespace Tomato

#endif // ANIMATION_HPP
