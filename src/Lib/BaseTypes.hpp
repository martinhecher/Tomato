// **********************************************************************
// * \author Martin Hecher                                              *
// *         mailto:martin.hecher@fraunhofer.at                         *
// * \date   2011-12-25                                                 *
// **********************************************************************

#ifndef BASETYPES_HPP
#define BASETYPES_HPP

#include <stdint.h>
#include <vector>

namespace Tomato
{
   class Animation;
   class Action;

   typedef std::vector<Animation*>        AnimationStore;
   typedef AnimationStore::iterator       AnimationStoreIter;
   typedef AnimationStore::const_iterator AnimationStoreConstIter;

   typedef std::vector<Action*>           ActionStore;
   typedef ActionStore::iterator          ActionStoreIter;
   typedef ActionStore::const_iterator    ActionStoreConstIter;

   typedef int64_t Time;

} // end namespace Tomato

#endif // BASETYPES_HPP
