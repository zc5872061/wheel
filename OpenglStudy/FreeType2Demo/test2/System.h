//
//  System.h
//  OpenglStudy
//
//  Created by Chukie on 15/7/15.
//  Copyright (c) 2015年 com.dream.freetype. All rights reserved.
//

#ifndef __OpenglStudy__System__
#define __OpenglStudy__System__

class System
{
public:
    System(const int &width,const int &height,const char* applicationname);
    ~System();
    
    bool initialize();
    static void shutDown();
    void run();
private:
    
    static void openglFrame();
    static void idleFunc();
    bool initializeWindows(int& screenWidth,int& screenHeight);
    int m_width;
    int m_height;
    char* m_applicationName;
};



#endif /* defined(__OpenglStudy__System__) */
