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

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_WIN_GKEY_H
#define MUMBLE_MUMBLE_GLOBALSHORTCUT_WIN_GKEY_H

#pragma pack(push, 1)

typedef struct
{
	unsigned int keyIdx         : 8;        // index of the G key or mouse button, for example, 6 for G6 or Button 6
	unsigned int keyDown        : 1;        // key up or down, 1 is down, 0 is up
	unsigned int mState         : 2;        // mState (1, 2 or 3 for M1, M2 and M3)
	unsigned int mouse          : 1;        // indicate if the Event comes from a mouse, 1 is yes, 0 is no.
	unsigned int reserved1      : 4;        // reserved1
	unsigned int reserved2      : 16;       // reserved2
} GkeyCode;

typedef void (__cdecl *logiGkeyCB)(GkeyCode gkeyCode, const wchar_t* gkeyOrButtonString, void* context);

typedef struct
{
	logiGkeyCB gkeyCallBack;
	void* gkeyContext;
} logiGkeyCBContext;

#pragma pack( pop )

typedef BOOL (*fnLogiGkeyInit)(logiGkeyCBContext* context);
typedef wchar_t* (*fnLogiGkeyGetMouseButtonString)(int button);
typedef wchar_t* (*fnLogiGkeyGetKeyboardGkeyString)(int key, int mode);
typedef BOOL (*fnLogiGkeyIsMouseButtonPressed)(int button);
typedef BOOL (*fnLogiGkeyIsKeyboardGkeyPressed)(int key, int mode);
typedef void (*fnLogiGkeyShutdown)();

class GlobalShortcutWinGkey
{
public:
	GlobalShortcutWinGkey();

	QLibrary lib;

	bool load();
	bool unload();

private:
	bool loaded;
	bool initialized;
	logiGkeyCBContext context;

	fnLogiGkeyInit pfnLogiGkeyInit;
	fnLogiGkeyGetMouseButtonString pfnLogiGkeyGetMouseButtonString;
	fnLogiGkeyGetKeyboardGkeyString pfnLogiGkeyGetKeyboardGkeyString;
	fnLogiGkeyIsMouseButtonPressed pfnLogiGkeyIsMouseButtonPressed;
	fnLogiGkeyIsKeyboardGkeyPressed pfnLogiGkeyIsKeyboardGkeyPressed;
	fnLogiGkeyShutdown pfnLogiGkeyShutdown;

	static void __cdecl keyCallback( GkeyCode key, const wchar_t* name, void* context );
};

#endif // GLOBALSHORTCUTWINGKEY_H
