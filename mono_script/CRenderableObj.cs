using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CRenderableObj : CGameObj
    {
        public enum DEPTHLAYERTYPE
        {
            DEPTH_LAYER_0 = 0,
            DEPTH_LAYER_1,
            DEPTH_LAYER_2,
            DEPTH_LAYER_3,
            DEPTH_LAYER_4,
            DEPTH_LAYER_5,
            NUM_DEPTHLAYERTYPE,
        };

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CContainer _GetContainer(IntPtr _this);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetDepthLayer(IntPtr _this, DEPTHLAYERTYPE layer);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _Forward(IntPtr _this, float f);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _Yaw(IntPtr _this, float d, MATHORDER order);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _Pitch(IntPtr _this, float d, MATHORDER order);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetPosition(IntPtr _this, CVector3 pos);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CVector3 _GetPosition(IntPtr _this);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CVector3 _GetDirection(IntPtr _this);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _UpdateTransform(IntPtr _this);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CAnimation _LoadAnimation(IntPtr _this, String name);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetCurrentAnimation(IntPtr _this, CAnimation ani);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CAnimation _SetCurrentAnimationByName(IntPtr _this, String name);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _IsCurrentAnimationStopped(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CPhysicsObj _GetPhysicsObj(IntPtr _this);        

        public CRenderableObj()
        {
        }

        public CRenderableObj(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CRenderableObj()
        {
        }

        public CContainer GetContainer()
        {
            return _GetContainer(_this);
        }

        public void SetDepthLayer(DEPTHLAYERTYPE layer)
        {
            _SetDepthLayer(_this, layer);
        }

        public void Forward(float f)
        {
            _Forward(_this, f);
        }

        public void Yaw(float d, MATHORDER order)
        {
            _Yaw(_this, d, order);
        }

        public void Pitch(float d, MATHORDER order)
        {
            _Pitch(_this, d, order);
        }

        public void SetPosition(CVector3 pos)
        {
            _SetPosition(_this, pos);
        }

        public CVector3 GetPosition()
        {
            return _GetPosition(_this);
        }

        public CVector3 GetDirection()
        {
            return _GetDirection(_this);
        }

        public void UpdateTransform()
        {
            _UpdateTransform(_this);
        }

        public CAnimation LoadAnimation(String name)
        {
            return _LoadAnimation(_this, name);
        }

        public void SetCurrentAnimation(CAnimation ani)
        {
            _SetCurrentAnimation(_this, ani);
        }

        public CAnimation SetCurrentAnimationByName(String name)
        {
            return _SetCurrentAnimationByName(_this, name);
        }

        public bool IsCurrentAnimationStopped()
        {
            return _IsCurrentAnimationStopped(_this);
        }

        public CPhysicsObj GetPhysicsObj()       
        {
            return _GetPhysicsObj(_this);
        }
    }
}

