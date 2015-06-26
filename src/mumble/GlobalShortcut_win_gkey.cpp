/* Copyright (C) 2015, Jordan J Klassen <forivall@gmail.com>

   Some rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// portions of this code are
// Copyright (C) 2011-2014 Logitech. All rights reserved.
// Author: Tiziano Pigliucci
// Email: tpigliucci@logitech.com
// http://gaming.logitech.com/en-us/developers

#include "mumble_pch.hpp"

#include "GlobalShortcut_win_gkey.h"

#ifdef _M_X64
#define LOGITECH_GKEY_DLL_REGKEY " HKEY_CLASSES_ROOT\\Wow6432Node\\CLSID\\{7bded654-f278-4977-a20f-6e72a0d07859}\\ServerBinary"
// #define LOGITECH_GKEY_DLL_LOC "C:\\Program Files\\Logitech Gaming Software\\SDK\\G-key\\x64\\LogitechGkey.dll"
#define LOGITECH_GKEY_DLL_LOC "C:/Program Files/Logitech Gaming Software/SDK/G-key/x64/LogitechGkey.dll"
#else
#define LOGITECH_GKEY_DLL_REGKEY "HKEY_CLASSES_ROOT\\CLSID\\{7bded654-f278-4977-a20f-6e72a0d07859}\\ServerBinary"
// #define LOGITECH_GKEY_DLL_LOC "C:\\Program Files\\Logitech Gaming Software\\SDK\\G-key\\x64\\LogitechGkey.dll"
#define LOGITECH_GKEY_DLL_LOC "C:/Program Files/Logitech Gaming Software/SDK/G-key/x86/LogitechGkey.dll"
#endif

GlobalShortcutWinGkey::GlobalShortcutWinGkey()
{
  qWarning("GlobalShortcutWinGkey: constructor");
  QString filename = QString::fromLatin1(LOGITECH_GKEY_DLL_LOC);
  lib.setFileName(filename);
}

bool GlobalShortcutWinGkey::load()
{
  qWarning("GlobalShortcutWinGkey: load");
  loaded = lib.load();
  if (loaded) {
    qWarning("GlobalShortcutWinGkey: load success");
    pfnLogiGkeyInit = (fnLogiGkeyInit)lib.resolve("LogiGkeyInit");
    pfnLogiGkeyGetMouseButtonString = (fnLogiGkeyGetMouseButtonString)lib.resolve("LogiGkeyGetMouseButtonString");
    pfnLogiGkeyGetKeyboardGkeyString = (fnLogiGkeyGetKeyboardGkeyString)lib.resolve("LogiGkeyGetKeyboardGkeyString");
    pfnLogiGkeyIsMouseButtonPressed = (fnLogiGkeyIsMouseButtonPressed)lib.resolve("LogiGkeyIsMouseButtonPressed");
    pfnLogiGkeyIsKeyboardGkeyPressed = (fnLogiGkeyIsKeyboardGkeyPressed)lib.resolve("LogiGkeyIsKeyboardGkeyPressed");
    pfnLogiGkeyShutdown = (fnLogiGkeyShutdown)lib.resolve("LogiGkeyShutdown");

    context.gkeyContext = this;
    context.gkeyCallBack = keyCallback;

    if (pfnLogiGkeyInit) {
      initialized = pfnLogiGkeyInit(&context);
      if (initialized) {
        qWarning("GlobalShortcutWinGkey: initialized success");
      }
    } else {
      qWarning("GlobalShortcutWinGkey: can't initialize!");
    }
  } else {
    qWarning("GlobalShortcutWinGkey: load failed!");
  }
  return initialized;
}

bool GlobalShortcutWinGkey::unload()
{

  if (initialized) {
    qWarning("GlobalShortcutWinGkey: shutdown");
    pfnLogiGkeyShutdown();
  }
  qWarning("GlobalShortcutWinGkey: unload");
  return lib.unload();
}

void GlobalShortcutWinGkey::keyCallback(GkeyCode key, const wchar_t* name, void* context) //-V813
{
  GlobalShortcutWinGkey* that = reinterpret_cast<GlobalShortcutWinGkey*>(context);

  qWarning("gkey pressed or released!");
  qDebug() << "gkey pressed or released!";
  // ScopedSRWLock lock( &sdk->mLock );
  //
  // sdk->mQueue.push( key );
}
