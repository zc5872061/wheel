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

void MyPolygon::draw(float* colors)
{
    glBegin(GL_POLYGON);
    for(int i=0; i < m_numVerticies; i++) {
        glColor4f(colors[i*4], colors[i*4+1], colors[i*4+2], colors[i*4+3]);
        glVertex3f(m_vertics[i].x, m_vertics[i].y, 0.0f);
    }
    glEnd();
}

void MyPolygon::drawNormals()
{
    vector2f *normals = getNormals();
    glBegin(GL_LINES);
    for(int i=0; i < m_numVerticies; i++) {
        vector2f pos = (m_vertics[(i+1)%m_numVerticies] - m_vertics[i]) / 2 + m_vertics[i];
        glVertex2f(m_normals[i].x * 20 + pos.x, m_normals[i].y * 20 + pos.y);
        glVertex2f(pos.x, pos.y);
    }
    glEnd();
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
    std::vector<MyEdge*> facing;
    return facing;
}

void MyPolygon::updateAll()
{
    updateNormals();
    updateClockwise();
}

void MyPolygon::updateNormals() {
    for(int i=0; i < m_numVerticies; i++) {
        vector2f diff = getVertex(i) - getVertex(i + 1);
        float temp = diff.x;
        diff.x = diff.y;
        diff.y = -temp;
        if(!isClockwise()) diff *= -1;
        m_normals[i] = diff.normalized();
    }
}


void MyPolygon::updateClockwise() {
    double sum = 0;
    for(int i=0; i < m_numVerticies - 1; i++) {
        sum += (getVertex(i + 1).x - getVertex(i).x) * (getVertex(i + 1).y + getVertex(i).y);
    }
    m_clockwise = sum > 0;
}