﻿using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    class UIText : RagnarComponent
    {
        public UIText(UIntPtr ptr) : base(ptr)
        {
            type = ComponentType.UI_TEXT;
        }

        public extern string text
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }
    }
}