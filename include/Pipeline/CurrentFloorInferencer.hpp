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
      CurrentFloorInferencer(std::map<int, float> elevationMap={});
      ~CurrentFloorInferencer();

      void set_elevationMap(std::map<int, float> elevationMap);
      std::map<int, float> get_elevationMap() const;
      int find_closest_floor(float playerPosition=0.0f);
   };
}



