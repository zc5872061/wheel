//
//  Polygon.h
//  OpenglStudy
//
//  Created by Chukie on 15/7/7.
//  Copyright (c) 2015年 com.dream.freetype. All rights reserved.
//

#ifndef __OpenglStudy__Polygon__
#define __OpenglStudy__Polygon__

#include "Vector2.h"
#include "glCommon.h"
#include <vector>

class MyEdge
{
public:
    MyEdge()
    {
        p1 = vector2f();
        p2 = vector2f();
    }
    
    MyEdge(const vector2f mp1,const vector2f mp2)
    {
        p1 = mp1;
        p2 = mp2;
    }
    
    vector2f getMiddle()
    {
        return ((p2-p1)/2 + p1);
    }
    
protected:
    vector2f p1;
    vector2f p2;
};

class MyPolygon
{
public:
    MyPolygon(int numVerts);
    void draw();
    void draw(float* color);
    void drawNormals();
    
    std::vector<MyEdge*> getFacingEdges(vector2f point);
protected:
    void resetVertics();
protected:
    vector2f *m_vertics;
    bool m_clockwise;
    MyEdge** m_edgs;
    vector2f* m_normals;
    int m_numVerticies;
};

#endif /* defined(__OpenglStudy__Polygon__) */
