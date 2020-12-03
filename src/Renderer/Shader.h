#pragma once
#include "../pch.h"

namespace lwvl {
	class Uniform {
		int m_location = -1;

	public:
		Uniform() = default;
		Uniform(int location);
		Uniform(const Uniform& other) = default;
		Uniform(Uniform&& other) = default;
		~Uniform() = default;

		Uniform& operator=(const Uniform& other) = default;
		Uniform& operator=(Uniform&& other) = default;

		void set1i(const int i0);
		void set1f(const float f0);
		void set1u(const unsigned int u0);

		void set2i(const int i0, const int i1);
		void set2f(const float f0, const float f1);
		void set2u(const unsigned int u0, const unsigned int u1);

		void set3i(const int i0, const int i1, const int i2);
		void set3f(const float f0, const float f1, const float f2);
		void set3u(const unsigned int u0, const unsigned int u1, const unsigned int u2);

		void set4i(const int i0, const int i1, const int i2, const int i3);
		void set4f(const float f0, const float f1, const float f2, const float f3);
		void set4u(const unsigned int u0, const unsigned int u1, const unsigned int u2, const unsigned int u3);

		void setMatrix4(const float* data);
		void setOrthographic(float top, float bottom, float right, float left, float far, float near);
		void set2DOrthographic(float top, float bottom, float right, float left);

		int location();
	};


	class shader_compilation_failure : public std::exception {
	public:
		shader_compilation_failure(const std::string& msg);
	};


	enum class ShaderType {
		Vertex = GL_VERTEX_SHADER,
		TessCtrl = GL_TESS_CONTROL_SHADER,
		TessEval = GL_TESS_EVALUATION_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Compute = GL_COMPUTE_SHADER
	};


	template<ShaderType target>
	class Shader {
		unsigned int m_id{ reserve() };
		friend class ShaderProgram;

		static unsigned int reserve() {
			unsigned int id = glCreateShader(static_cast<GLenum>(target));
			return id;
		}

	public:
		Shader(const std::string& source) {
			const char* src = source.c_str();
			glShaderSource(m_id, 1, &src, nullptr);
			glCompileShader(m_id);

			int result;
			glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE) {
				int length;
				glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);
				char* message = static_cast<char*>(_malloca(length * sizeof(char)));
				glGetShaderInfoLog(m_id, length, &length, message);
				std::stringstream error;
				error << "Failed to compile ";

				switch (target) {
				case ShaderType::Vertex:
					error << "vertex";
					break;
				case ShaderType::Fragment:
					error << "fragment";
					break;
				default:
					error << "unknown type of ";
					break;
				}

				error << " shader" << std::endl << message << std::endl;
				throw std::invalid_argument(error.str().c_str());
			}
		}
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept : m_id(other.m_id) {
			other.m_id = 0;
		}

		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept {
			m_id = other.m_id;
			other.m_id = 0;
			return *this;
		}

		~Shader() {
			// An id of 0 will be silently ignored.
			glDeleteShader(m_id);
		}
	};


	/* ****** Shader Types ****** */
	typedef Shader<ShaderType::Vertex>   VertexShader;
	typedef Shader<ShaderType::TessCtrl> TesselationControlShader;
	typedef Shader<ShaderType::TessEval> TesselationEvaluationShader;
	typedef Shader<ShaderType::Geometry> GeometryShader;
	typedef Shader<ShaderType::Fragment> FragmentShader;
	typedef Shader<ShaderType::Compute>  ComputeShader;


	/* ****** Shader Program ******
	* A simple abstraction over the code required to create a shader in OpenGL.
	* Not feature complete.
	*
	* Usage:
	*   ShaderProgram myShader;
	*   myShader.attach(VertexShader);
	*   myShader.attach(GeometryShader);
	*   myShader.attach(FragmentShader);
	*   myShader.link();
	*
	* Quick Linking (Vertex and Fragment Shaders only):
	*   ShaderProgram myShader;
	*   myShader.link(VertexShader, FragmentShader);
	*/
	class ShaderProgram {
		unsigned int m_id{ reserve() };

		int uniformLocation(const std::string& name) const;
		static unsigned int reserve();

	public:
		ShaderProgram();
		ShaderProgram(const ShaderProgram& other) = delete;
		ShaderProgram(ShaderProgram&& other) noexcept;
		~ShaderProgram();

		ShaderProgram& operator=(const ShaderProgram& other) = delete;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;

		unsigned int id();
		Uniform uniform(const std::string& name);

		template<ShaderType target>
		void attach(const Shader<target>& shader) { glAttachShader(m_id, shader.m_id); }

		template<ShaderType target>
		void detach(const Shader<target>& shader) { glDetachShader(m_id, shader.m_id); }
		void link();
		void link(const VertexShader& vs, const FragmentShader& fs);

		void bind() const;
		static void clear();
	};
}
