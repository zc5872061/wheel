//
//  Polygon.cpp
//  OpenglStudy
//
//  Created by Chukie on 15/7/7.
//  Copyright (c) 2015年 com.dream.freetype. All rights reserved.
//

#include "Polygon.h"

MyPolygon::MyPolygon(int numVerts)
{
    m_vertics = new vector2f[numVerts];
    m_normals = new vector2f[numVerts];
    m_edgs = new MyEdge*[numVerts];
    m_numVerticies = numVerts;
    resetVertics();
}

void MyPolygon::draw()
{
    glBegin(GL_POLYGON);
        for(int i=0; i < m_numVerticies; i++) {
            glVertex3f(m_vertics[i].x, m_vertics[i].y, 0.0f);
        }
    glEnd();
}

void MyPolygon::draw(float* color)
{
    
}

void MyPolygon::drawNormals()
{
    
}

void MyPolygon::resetVertics()
{
    for(int i = 0; i < m_numVerticies; i++)
    {
        m_vertics[i] = vector2f();
    }
    m_clockwise = false;
}

std::vector<MyEdge*> MyPolygon::getFacingEdges(vector2f point)
{
}