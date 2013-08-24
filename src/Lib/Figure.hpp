// **********************************************************************
// * \author Martin Hecher                                              *
// *         mailto:martin.hecher@fraunhofer.at                         *
// * \date   2011-12-28                                                 *
// **********************************************************************

#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGTransform.h>

namespace BoardGame
{
  /*! A simple figure object that can move within the xy-plane.
   */
   class Figure
   {
   public:
      Figure();
      ~Figure();

      OSG::NodeTransitPtr getRoot();
      void setModel(OSG::Node *modelN);

      void moveLeft();
      void moveRight();
      void moveUp();
      void moveDown();

   private:
      // Animation control members:
      float m_speed;
      float m_moveToLength;
      float m_fps;

      // OpenSG members:
      OSG::TransformUnrecPtr m_transC;
      OSG::NodeUnrecPtr      m_modelN;
      OSG::NodeUnrecPtr      m_rootN;

      OSG::Vec3f             m_directions[4];

      void createAnimations();
      void createMoveToAnimation(OSG::Vec3f& dir,
                                 const std::string& name);

      void moveTo(int curFrame, int numFrames, int fps, void *userData);

      // Helper functions:
      OSG::Vec3f computeTranslation(OSG::Real32 t, const OSG::Vec3f& dir);
      OSG::Vec3f getCurrentTranslation(const OSG::Matrix& matrix);
   };

} // end namespace BoardGame

#endif // FIGURE_HPP
