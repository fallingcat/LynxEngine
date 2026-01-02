using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CSampleScriptGame : CObj
    {
        new public void OnInit()
        {
            GlobalVar.Engine.OpenPackage("../Game1Data.zip");
            CUIPage MainMenuPage = GlobalVar.Engine.CreateUIPage("MainMenuPage", "../script/CMainMenuPage.lua", "CMainMenuPage");            
            GlobalVar.Engine.SetCurrentUIPage(MainMenuPage);
        }
    }
}
