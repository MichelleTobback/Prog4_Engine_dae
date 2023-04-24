#pragma once
#include <memory>
#include <map>
#include <set>

namespace dae
{
	class Keyboard final
	{
	public:
        enum class KeyCode
        {
            Unknown = 0,

            /**
             *  \name Usage page 0x07
             *
             *  These values are the same as in SDL_SCANCODE.
             */
             /* @{ */

            A = 4,
            B = 5,
            C = 6,
            D = 7,
            E = 8,
            F = 9,
            G = 10,
            H = 11,
            I = 12,
            J = 13,
            K = 14,
            L = 15,
            M = 16,
            N = 17,
            O = 18,
            P = 19,
            Q = 20,
            R = 21,
            S = 22,
            T = 23,
            U = 24,
            V = 25,
            W = 26,
            X = 27,
            Y = 28,
            Z = 29,

            Num_1 = 30,
            Num_2 = 31,
            Num_3 = 32,
            Num_4 = 33,
            Num_5 = 34,
            Num_6 = 35,
            Num_7 = 36,
            Num_8 = 37,
            Num_9 = 38,
            Num_0 = 39,

            Return = 40,
            Escape = 41,
            Backslash = 42,
            Tab = 43,
            Space = 44,

            Minus = 45,
            Equals = 46,
            LeftBracket = 47,
            RightBracket = 48,
            BackSlash = 49, 
            NonuSlash = 50, 
            Semicolon = 51,
            Apostrophe = 52,
            Grave = 53, 
            Comma = 54,
            Period = 55,
            Slash = 56,

            CapsLock = 57,

            F1 = 58,
            F2 = 59,
            F3 = 60,
            F4 = 61,
            F5 = 62,
            F6 = 63,
            F7 = 64,
            F8 = 65,
            F9 = 66,
            F10 = 67,
            F11 = 68,
            F12 = 69,

            PrintScreen = 70,
            ScrollBack = 71,
            Pause = 72,
            Insert = 73, /**< insert on PC, help on some Mac keyboards (but
                                           does send code 73, not 117) */
            Home = 74,
            PageUp = 75,
            Delete = 76,
            End = 77,
            PageDown = 78,
            Right = 79,
            Left = 80,
            Down = 81,
            Up = 82,

            NumlockClear = 83, /**< num lock on PC, clear on Mac keyboards
                                             */
            KP_Divide = 84,
            KP_Multiply = 85,
            KP_Minus = 86,
            KP_Plus = 87,
            KP_Enter = 88,
            KP_1 = 89,
            KP_2 = 90,
            KP_3 = 91,
            KP_4 = 92,
            KP_5 = 93,
            KP_6 = 94,
            KP_7 = 95,
            KP_8 = 96,
            KP_9 = 97,
            KP_0 = 98,
            KP_PERIOD = 99,
            KP_Equals = 103,
            F13 = 104,
            F14 = 105,
            F15 = 106,
            F16 = 107,
            F17 = 108,
            F18 = 109,
            F19 = 110,
            F20 = 111,
            F21 = 112,
            F22 = 113,
            F23 = 114,
            F24 = 115
        };

		enum class KeyState
		{
			Down, Released, Pressed
		};

		explicit Keyboard();
		~Keyboard();

		void Update();
        bool ProcessInput();

        bool IsKeyPressed(KeyCode key);
        bool IsKeyUp(KeyCode key);
        bool IsKeyDown(KeyCode key);

	private:
        std::map<KeyCode, KeyCode> m_KeysPressed;
        std::set<KeyCode> m_KeysToRelease;

        void SetKeyDown(KeyCode key);
        void SetKeyReleased(KeyCode key);
        bool WasDown(KeyCode key);
	};
}