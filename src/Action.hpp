// **********************************************************************
// * \author Martin Hecher                                              *
// *         mailto:martin.hecher@fraunhofer.at                         *
// * \date   2011-12-23                                                 *
// **********************************************************************

#ifndef ACTION_HPP
#define ACTION_HPP

#include "BaseTypes.hpp"

#include <string>
#include <boost/function.hpp>

namespace Tomato
{
   /*! An Action represents one part of an Animation. One or multiple Actions
       form an Animation. The framerate is determined by the Animation, not the
       Action itself.

       \see Animation
   */
   class Action
   {
   public:
      /* ---------------------------------------------------------------------*/
      /* Typedefs                                                             */
      typedef boost::function<void(int, int, int, void*)> Callback;

      /* ---------------------------------------------------------------------*/
      /* Constructor                                                          */

      Action(const std::string &name);

      /* ---------------------------------------------------------------------*/
      /* Action management                                                    */

      Callback onUpdate;

      void update(int curFrame, int numFrames, int fps, void* userData = NULL);

      /* ---------------------------------------------------------------------*/
      /* Action management                                                    */

      void setNumFrames(int frames);
      int  getNumFrames();

      void setStartFrame(int frame);
      int  getStartFrame();
      int  getEndFrame  ();

      void  setUserdata(void* userData);
      void* getUserdata();

      /* ---------------------------------------------------------------------*/
      /* Identification                                                       */

      const std::string& getName();

   private:
      friend class Controller;

      Action(const Action &source);
      void operator=(const Action &source);
      virtual ~Action();

      int         m_fps;

      int         m_numFrames;
      int         m_startFrame;
      int         m_curFrame;

      void       *m_userData;

      std::string m_name;
   };

} // end namespace Tomato

#endif // ACTION_HPP
