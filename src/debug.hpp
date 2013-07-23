#pragma once
#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <functional>
#include "glid.hpp"

struct ErrorMessage {
    static std::string resolveSourceName(GLenum source) {
        switch (source) {
        case gl::DEBUG_SOURCE_API: return "API";
        case gl::DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
        case gl::DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
        case gl::DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
        case gl::DEBUG_SOURCE_APPLICATION: return "Application";
        case gl::DEBUG_SOURCE_OTHER: return "Other";
        default: return "Invalid source type";
        }
    }

    static std::string resolveErrorType(GLenum errorType) {
        switch (errorType) {
        case gl::DEBUG_TYPE_ERROR: return "Error";
        case gl::DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Functionality";
        case gl::DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behavior";
        case gl::DEBUG_TYPE_PORTABILITY: return "Portability";
        case gl::DEBUG_TYPE_PERFORMANCE: return "Performance";
        case gl::DEBUG_TYPE_OTHER: return "Other";
        default: return "Invalid Error Type";
        }
    }

    static std::string resolveSeverity(GLenum severity) {
        switch (severity) {
        case gl::DEBUG_SEVERITY_HIGH: return "High";
        case gl::DEBUG_SEVERITY_MEDIUM: return "Medium";
        case gl::DEBUG_SEVERITY_LOW: return "Low";
        default: return "Invalid Message Severity";
        }
    }

    GLenum source, type, severity;
    GLuint id;
    std::string message;

    std::string toString() const {
        std::string sourceName = resolveSourceName(source);
        std::string errorType = resolveErrorType(type);
        std::string severityName = resolveSeverity(severity);

        std::stringstream out;
        out << '[' << errorType << " from " << sourceName << ",\t" << severityName << "priority]\n"
            << "Message: " << message << '\n';
        return out.str();
    }

    ErrorMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
        const GLchar* message)
        : source(source)
        , type(type)
        , id(id)
        , severity(severity)
        , message(message, message+length)
    { }
};

std::ostream& operator<< (std::ostream& str, ErrorMessage const& em) {
    return str << em.toString();
}

inline
void debugFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar* message, GLvoid* userParam)
{
    ErrorMessage em(source, type, id, severity, length, message);

    typedef std::function<void(ErrorMessage)> cb_t;

    auto cbFunction = reinterpret_cast<cb_t*>(userParam);
    if (cbFunction && *cbFunction) {
        (*cbFunction)(em);
    }
}

inline
std::vector<ErrorMessage> getDebugLog() {
    const unsigned maxMessageCount = 100;
    const int bufSize = 2048;

    std::vector<GLuint> sources(maxMessageCount);
    std::vector<GLuint> types(maxMessageCount);
    std::vector<GLuint> ids(maxMessageCount);
    std::vector<GLuint> severities(maxMessageCount);
    std::vector<GLint> lengths(maxMessageCount);
    std::vector<char> messageData(bufSize);

    unsigned messageCount = gl::GetDebugMessageLog(
        maxMessageCount, bufSize, sources.data(), types.data(), ids.data(),
        severities.data(), lengths.data(), messageData.data());

    std::vector<ErrorMessage> messages;

    unsigned pos = 0;
    for (unsigned i = 0; i < messageCount; ++i) {
        messages.push_back(ErrorMessage(sources[i], types[i], ids[i], severities[i], lengths[i], messageData.data() + pos));
        pos += lengths[i];
    }
    return messages;
}

enum class DebugMessageSource {
    SourceApi = gl::DEBUG_SOURCE_API,
    WindowSystem = gl::DEBUG_SOURCE_WINDOW_SYSTEM,
    ShaderCompiler = gl::DEBUG_SOURCE_SHADER_COMPILER,
    ThirdParty = gl::DEBUG_SOURCE_THIRD_PARTY,
    Application = gl::DEBUG_SOURCE_APPLICATION,
    Other = gl::DEBUG_SOURCE_OTHER,
    DontCare = gl::DONT_CARE
};

enum class DebugMessageType {
    Error = gl::DEBUG_TYPE_ERROR,
    Behavior = gl::DEBUG_TYPE_DEPRECATED_BEHAVIOR,
    UndefinedBehavior = gl::DEBUG_TYPE_UNDEFINED_BEHAVIOR,
    Portability = gl::DEBUG_TYPE_PORTABILITY,
    Performance = gl::DEBUG_TYPE_PERFORMANCE,
    Marker = gl::DEBUG_TYPE_MARKER,
    PushGroup = gl::DEBUG_TYPE_PUSH_GROUP,
    PopGroup = gl::DEBUG_TYPE_POP_GROUP,
    Other = gl::DEBUG_TYPE_OTHER,
    DontCare = gl::DONT_CARE
};

enum class DebugMessageSeverity {
    High = gl::DEBUG_SEVERITY_HIGH,
    Medium = gl::DEBUG_SEVERITY_MEDIUM,
    Low = gl::DEBUG_SEVERITY_LOW,
    Info = gl::DEBUG_SEVERITY_NOTIFICATION,
    DontCare = gl::DONT_CARE
};

inline void debugMessageControl(DebugMessageSource source, DebugMessageType type, DebugMessageSeverity severity, bool enabled) {
    gl::DebugMessageControl(
        static_cast<GLenum>(source),
        static_cast<GLenum>(type),
        static_cast<GLenum>(severity),
        0, nullptr,
        enabled ? gl::TRUE_ : gl::FALSE_
    );
}

inline void debugMessageControl(std::vector<GLuint> ids, bool enabled) {
    gl::DebugMessageControl(gl::DONT_CARE, gl::DONT_CARE, gl::DONT_CARE, ids.size(), ids.data(),
        enabled ? gl::TRUE_ : gl::FALSE_
    );
}

