#pragma once


#include <map>


namespace Pipeline
{
   class CurrentFloorInferencer
   {
   private:
      std::map<int, float> elevationMap;

   protected:


   public:
      CurrentFloorInferencer();
      ~CurrentFloorInferencer();

      int find_closest_floor(float playerPosition=0.0f);
   };
}



