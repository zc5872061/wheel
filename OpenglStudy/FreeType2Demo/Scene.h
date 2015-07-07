//
//  Scene.h
//  FreeType2Demo
//
//  Created by Chukie on 15/7/6.
//  Copyright (c) 2015年 com.dream.freetype. All rights reserved.
//

#ifndef __FreeType2Demo__Scene__
#define __FreeType2Demo__Scene__

#include <vector>
#include "Polygon.h"

class Scene
{
public:
    Scene();
    ~Scene();
    
    void tick();
    void draw();
    
    std::vector<MyPolygon*> polygons;
};
#endif /* defined(__FreeType2Demo__Scene__) */
