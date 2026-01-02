using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CUIPage : CObj
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _Quit(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CButton _AddButton(IntPtr _this, String filename, CRect rect, CVector2 size, UInt32 id, String text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern int _GetNumControls(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CControl _FindControl(IntPtr _this, String name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CControl _GetControlByIndex(IntPtr _this, int i);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetPressedControl(IntPtr _this, CControl c);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CControl _GetPressedControl(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _RenderToOffscreen(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _RenderControls(IntPtr _this);
        
        public CUIPage()
        {
        }

        public CUIPage(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CUIPage()
        {
        }

        public void Quit()
        {
            _Quit(_this);
        }

        public CButton AddButton(String filename, CRect rect, CVector2 size, UInt32 id, String text)
        {
            return _AddButton(_this, filename, rect, size, id, text);
        }

        public int GetNumControls()
        {
            return _GetNumControls(_this);
        }

        public CControl FindControl(String name)
        {
            return _FindControl(_this, name);
        }

        public CControl GetControlByIndex(int i)
        {
            return _GetControlByIndex(_this, i);
        }

        public void SetPressedControl(CControl c)
        {
            _SetPressedControl(_this, c);
        }

        public CControl GetPressedControl()
        {
            return _GetPressedControl(_this);
        }

        public void RenderToOffscreen()
        {
            _RenderToOffscreen(_this);
        }

        public void RenderControls()
        {
            _RenderControls(_this);
        }

        new public void OnCreate()
        {
        }

        new public void OnInit()
        {         
        }

        new public void OnQuit()
        {
            CControl PressedButton = GetPressedControl();
            if (PressedButton != null)
            {
                CUIPage NextUIPage = GlobalVar.Engine.FindUIPage(PressedButton.GetTargetName());
                if (NextUIPage != null)
                {
                    RenderToOffscreen();
                    GlobalVar.Engine.SetCurrentUIPage(NextUIPage);
                }
                else
                {
                    String Name = PressedButton.GetTargetName();
                    String ScriptClassName = "C" + Name;                    
                    NextUIPage = GlobalVar.Engine.CreateUIPage(Name, "", ScriptClassName);
                    GlobalVar.Engine.SetCurrentUIPage(NextUIPage);
                }
            }
        }
    }
}
