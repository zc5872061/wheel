//
//  Scene.cpp
//  FreeType2Demo
//
//  Created by Chukie on 15/7/6.
//  Copyright (c) 2015年 com.dream.freetype. All rights reserved.
//

#include "Scene.h"
#include "glCommon.h"

Scene::Scene()
{
    MyPolygon *poly = new MyPolygon(3);
    poly->setVertex(0, vector2f(300, 100));
    poly->setVertex(1, vector2f(300, 200));
    poly->setVertex(2, vector2f(500, 200));
//    poly->setVertex(3, vector2f(250, 150));
//    poly->setVertex(4, vector2f(200, 100));
    polygons.push_back(poly);
    poly = new MyPolygon(4);
    poly->setVertex(0, vector2f(100, 100));
    poly->setVertex(1, vector2f(100, 200));
    poly->setVertex(2, vector2f(200, 200));
    poly->setVertex(3, vector2f(200, 100));
    //poly->setVertex(3, vector2f(400, 150));
    //poly->setVertex(3, vector2f(450, 350));
    polygons.push_back(poly);
}


Scene::~Scene()
{
    
}

void Scene::tick()
{
    
}

void Scene::draw()
{
    glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充模式
    glPolygonMode(GL_BACK, GL_LINE); // 设置反面为线形模式
    
    //绘制一个三角形
    
    {
        glColor3f( 1.0f, 0.0f, 0.0f );//指定颜色,红色
        
//        glBegin( GL_TRIANGLES );//开始画三角形
//        
//        {
//            glVertex3f( 0.0f, 0.0f, 0.0f );
//            glVertex3f( 25.f, 25.f, 0 );
//            glVertex3f( 50.f, 0, 0 );
//            
//        }
//        glEnd();//结束
        glPushAttrib(GL_COLOR_BUFFER_BIT);
        for(int i=0; i < polygons.size(); i++) {
            float cols[] = { 1.0f, 0.0f, .5f, 1.0f,
                1.0f, 1.0f, .5f, 1.0f,
                0.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, .5f, 1.0f,
                1.0f, 0.0f, .5f, 1.0f };
            polygons[i]->draw(cols);
            //polygons[i]->drawNormals();
        }
        glPopAttrib();
    }
}
