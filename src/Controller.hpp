// **********************************************************************
// * \author Martin Hecher                                              *
// *         mailto:martin.hecher@fraunhofer.at                         *
// * \date   2011-12-23                                                 *
// **********************************************************************

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "BaseTypes.hpp"
#include "Animation.hpp"
#include "Action.hpp"

namespace Tomato
{
   /*! A Controller is a management unit for the animation system. It stores a
       list of all animations and all actions and is responsible for their
       memory management. The Controller has to be updated each frame to
       internally update the animation cycles.

       \see Animation, Action
    */
   class Controller
   {
   public:
      /* ---------------------------------------------------------------------*/
      /* Singelton interface                                                  */
      static Controller* the();
      static void        destroy();

      /* ---------------------------------------------------------------------*/
      /* Runtime                                                              */

      void init  (Time time);
      void update(Time timeStamp);

      /* ---------------------------------------------------------------------*/
      /* Animation management                                                 */

      Animation* createAnimation(const std::string& name);
      Animation* getAnimation   (const std::string& name);

      /* ---------------------------------------------------------------------*/
      /* Action management                                                    */

      Action* createAction(const std::string& name);

   private:
      AnimationStore m_animations;
      ActionStore    m_actions;

      Time m_startTime;
      Time m_lastTime;

      static Controller* m_self;

      /* ---------------------------------------------------------------------*/
      /* Constructor/Destructor                                               */

      Controller();
      virtual ~Controller();
   };

} // end namespace Tomato

#endif // CONTROLLER_HPP
