#pragma once
#include "../pch.h"

namespace lwvl {
    namespace details {
        enum class BufferTarget {
            Array = GL_ARRAY_BUFFER,
            Element = GL_ELEMENT_ARRAY_BUFFER
        };
    }

    enum class Usage {
        Static = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
        Stream = GL_STREAM_DRAW
    };


    template<details::BufferTarget target>
    class Buffer {
        class ID {
            static unsigned int reserve() {
                unsigned int id;
                glGenBuffers(1, &id);
                return id;
            }

        public:
            ~ID() {
                glDeleteBuffers(1, &id);
            }

            operator unsigned int() const {
                return id;
            }

            const unsigned int id = reserve();
        };

        // We make this shareable so we can make copies of a Buffer instead of references to it.
        std::shared_ptr<Buffer::ID> m_id = std::make_shared<Buffer::ID>();
        Usage m_usage = Usage::Dynamic;

    public:
        unsigned int id() {
            return m_id->id;
        }

        Buffer() = default;

        Buffer(const Buffer& other) = default;
        Buffer& operator=(const Buffer& other) = default;

        Buffer(Buffer&& other) noexcept = default;
        Buffer& operator=(Buffer&& other) noexcept = default;

        template<typename T>
        void construct(const T* data, GLsizei count) {
            bind();
            glBufferData(
                static_cast<GLenum>(target), sizeof(T) * count, 
                data, static_cast<GLenum>(m_usage)
            );
            clear();
        }

        template<class _Iter>
        void construct(_Iter first, _Iter last) {
            bind();
            glBufferData(
                static_cast<GLenum>(target), sizeof(*first) * (last - first),
                &(*first), static_cast<GLenum>(m_usage)
            );
            clear();
        }

        template<typename T>
        void update(const T* data, GLsizei count) {
            bind();
            glBufferSubData(
                static_cast<GLenum>(target), sizeof(T) * count,
                data, static_cast<GLenum>(m_usage)
            );
            clear();
        }

        template<class _Iter>
        void update(_Iter first, _Iter last, ptrdiff_t offset = 0) {
            bind();
            glBufferSubData(static_cast<GLenum>(target), offset, sizeof(*first) * (last - first), &(*first));
            clear();
        }

        void usage(Usage usage) { m_usage = usage; }
        Usage usage() { return m_usage; }

        // These operations should not be const because they modify GL state.
        void bind() {
            glBindBuffer(
                static_cast<GLenum>(target),
                static_cast<unsigned int>(*m_id)
            );
        }

        static void clear() {
            glBindBuffer(static_cast<GLenum>(target), 0);
        }
    };


    typedef Buffer<details::BufferTarget::Array> ArrayBuffer;
    typedef Buffer<details::BufferTarget::Element> ElementBuffer;
}
