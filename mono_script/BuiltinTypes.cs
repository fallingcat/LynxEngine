//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.com
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2011/10/05
//  Last Update : 
//
//###########################################################################

using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public enum MATHORDER
    {
        LYNX_MATH_PRE = 0,
        LYNX_MATH_POST
    };

    public class CRect
    {
        public int x1, y1, x2, y2;

        public CRect()
        {
            x1 = 0;
            y1 = 0;
            x2 = 0;
            y2 = 0;
        }

        public CRect(int _x1, int _y1, int _x2, int _y2)
        {
            x1 = _x1;
            y1 = _y1;
            x2 = _x2;
            y2 = _y2;
        }

        ~CRect()
        {
        }
    }

    public class CColor4i
    {
        public byte r, g, b, a;

        public CColor4i()
        {
            r = 0;
            g = 0;
            b = 0;
            a = 0;
        }

        public CColor4i(byte _r, byte _g, byte _b, byte _a)
        {
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }

        ~CColor4i()
        {
        }
    }

    public class CPoint2
    {
        public int x, y;

        public CPoint2()
        {
            x = 0;
            y = 0;            
        }

        public CPoint2(int _x, int _y)
        {
            x = _x;
            y = _y;
        }

        ~CPoint2()
        {
        }
    }

    public class CVector2
    {
        public float x, y;

        public CVector2()
        {
            x = 0.0f;
            y = 0.0f;
        }

        public CVector2(float _x, float _y)
        {
            x = _x;
            y = _y;
        }

        ~CVector2()
        {
        }
    }

    public class CVector3
    {
        public float x, y, z;

        public CVector3()
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }

        public CVector3(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        ~CVector3()
        {
        }        
    }
}

