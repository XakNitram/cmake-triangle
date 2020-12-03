#pragma once
#include "../pch.h"

namespace lwvl {
    namespace debug {
        enum class Source {
            API             = GL_DEBUG_SOURCE_API,
            APPLICATION     = GL_DEBUG_SOURCE_APPLICATION,
            SHADER_COMPILER = GL_DEBUG_SOURCE_SHADER_COMPILER,
            THIRD_PARTY     = GL_DEBUG_SOURCE_THIRD_PARTY,
            WINDOW_SYSTEM   = GL_DEBUG_SOURCE_WINDOW_SYSTEM,
            OTHER           = GL_DEBUG_SOURCE_OTHER,
            DONT_CARE       = GL_DONT_CARE
        };

        enum class Type {
            DEPRECATED_BEHAVIOR = GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
            ERROR               = GL_DEBUG_TYPE_ERROR,
            MARKER              = GL_DEBUG_TYPE_MARKER,
            PERFORMANCE         = GL_DEBUG_TYPE_PERFORMANCE,
            PORTABILITY         = GL_DEBUG_TYPE_PORTABILITY,
            POP_GROUP           = GL_DEBUG_TYPE_POP_GROUP,
            PUSH_GROUP          = GL_DEBUG_TYPE_PUSH_GROUP,
            UNDEFINED_BEHAVIOR  = GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
            OTHER               = GL_DEBUG_TYPE_OTHER,
            DONT_CARE           = GL_DONT_CARE
        };

        enum class Severity {
            LOW          = GL_DEBUG_SEVERITY_LOW,
            MEDIUM       = GL_DEBUG_SEVERITY_MEDIUM,
            HIGH         = GL_DEBUG_SEVERITY_HIGH,
            NOTIFICATION = GL_DEBUG_SEVERITY_NOTIFICATION,
            DONT_CARE    = GL_DONT_CARE
        };

#ifdef _WIN32
        using LWVLDebugProc = void(__stdcall *)(
#else
        using LWVLDebugProc = void(*)(
#endif
            Source source, Type type, unsigned int id,
            Severity severity, int length, const char* message,
            const void* userState
        );

        class GLEventListener {
            GLuint m_unusedIDs = 0;
            LWVLDebugProc m_callback;
            void* m_userPtr;

            void assign(LWVLDebugProc callback);

        public:
            GLEventListener(void* userPtr, LWVLDebugProc callback, bool enabled = true);
            GLEventListener(LWVLDebugProc callback, bool enabled = true);
            ~GLEventListener();

            void control(Source sourceFilter, Type typeFilter, Severity severityFilter, GLsizei count, bool enabled);
            void control(bool enabled);

            void invoke(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message);
        };

        void simpleDebugCallback(GLDEBUGPROC callback, void* userPtr);

        void clearErrors();
    }
}
