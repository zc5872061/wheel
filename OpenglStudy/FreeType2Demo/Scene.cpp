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
        
        glBegin( GL_TRIANGLES );//开始画三角形
        
        {
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 25.f, 25.f, 0 );
            glVertex3f( 50.f, 0, 0 );
            
        }
        glEnd();//结束
        
    }
}
