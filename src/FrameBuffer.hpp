#pragma once

#include "Bindable.hpp"
#include "Texture.hpp"
#include "Util.hpp"

/*!
Framebuffer Class. This class encapsulates the Framebuffer
(FBO) OpenGL spec. See the official spec at:
    http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_object.txt

for details.

A framebuffer object (FBO) is conceptually a structure containing pointers
to GPU memory. The memory pointed to is either an OpenGL texture or an
OpenGL RenderBuffer. FBOs can be used to render to one or more textures,
share depth buffers between multiple sets of color buffers/textures and
are a complete replacement for pbuffers.

Performance Notes:
  1) It is more efficient (but not required) to call Bind() 
     on an FBO before making multiple method calls. For example:
        
      Framebuffer fbo;
      fbo.Bind();
      fbo.AttachTexture(gl::TEXTURE_2D, texId0, gl::COLOR_ATTACHMENT0_EXT);
      fbo.AttachTexture(gl::TEXTURE_2D, texId1, gl::COLOR_ATTACHMENT1_EXT);
      fbo.IsValid();

    To provide a complete encapsulation, the following usage
    pattern works correctly but is less efficient:

      Framebuffer fbo;
      // NOTE : No Bind() call
      fbo.AttachTexture(gl::TEXTURE_2D, texId0, gl::COLOR_ATTACHMENT0_EXT);
      fbo.AttachTexture(gl::TEXTURE_2D, texId1, gl::COLOR_ATTACHMENT1_EXT);
      fbo.IsValid();

    The first usage pattern binds the FBO only once, whereas
    the second usage binds/unbinds the FBO for each method call.

  2) Use Framebuffer::Disable() sparingly. We have intentionally
     left out an "Unbind()" method because it is largely unnecessary
     and encourages rendundant Bind/Unbind coding. Binding an FBO is
     usually much faster than enabling/disabling a pbuffer, but is
     still a costly operation. When switching between multiple FBOs
     and a visible OpenGL framebuffer, the following usage pattern 
     is recommended:

      Framebuffer fbo1, fbo2;
      fbo1.Bind();
        ... Render ...
      // NOTE : No Unbind/Disable here...

      fbo2.Bind();
        ... Render ...

      // Disable FBO rendering and return to visible window
      // OpenGL framebuffer.
      Framebuffer::Disable();
*/

namespace gldr {

namespace fbo_desc {
    enum class Attachment : GLenum {
        Color_0 = gl::COLOR_ATTACHMENT0,
        Color_1 = gl::COLOR_ATTACHMENT1,
        Color_2 = gl::COLOR_ATTACHMENT2,
        Color_3 = gl::COLOR_ATTACHMENT3,
        Color_4 = gl::COLOR_ATTACHMENT4,
        Color_5 = gl::COLOR_ATTACHMENT5,
        Color_6 = gl::COLOR_ATTACHMENT6,
        Color_7 = gl::COLOR_ATTACHMENT7,
        Color_8 = gl::COLOR_ATTACHMENT8,
        Color_9 = gl::COLOR_ATTACHMENT9,
        Color_10 = gl::COLOR_ATTACHMENT10,
        Depth = gl::DEPTH_ATTACHMENT,
        Stencil = gl::STENCIL_ATTACHMENT,
        DepthAndStencil = gl::DEPTH_STENCIL_ATTACHMENT
    };
}

class Framebuffer : public Bindable<Framebuffer>
{
public:
    static GLuint create() {
        GLuint id;
        gl::GenFramebuffers(1, &id);
        if (!id) {
            throw std::runtime_error("Error creating the FBO");
        }
        return id;
    }
    static void destroy(GLuint id) {
        gl::DeleteFramebuffers(1, &id);
    }
    static void bindObject(GLuint id) {
        gl::BindFramebuffer(gl::FRAMEBUFFER, id);
    }
    static GLuint getCurrentlyBound() {
        // TODO both targets
        return util::getState(gl::FRAMEBUFFER_BINDING);
    }

    /// Bind a texture to the "attachment" point of this FBO
    template<texture_desc::Type target>
    void AttachTexture( Texture<target> const& texture,
                        fbo_desc::Attachment attachment,
                        int mipLevel      = 0,
                        int zSlice        = 0 ) {
        /*GLenum attachment = gl::COLOR_ATTACHMENT0;
        switch (texture.getFormat()) {
        case texture_desc::Format::Depth:
            attachment = gl::DEPTH_ATTACHMENT;
            
        }*/

        AttachTexture(static_cast<GLenum>(target),
                      texture.id.get(),
                      static_cast<GLenum>(attachment),
                      mipLevel,
                      zSlice);
    }

    /// "Raw" version
    /// Bind a texture to the "attachment" point of this FBO
    void AttachTexture( GLenum texTarget, 
                        GLuint texId,
                        GLenum attachment,
                        int mipLevel      = 0,
                        int zSlice        = 0 );

    /// Bind an array of textures to multiple "attachment" points of this FBO
    ///  - By default, the first 'numTextures' attachments are used,
    ///    starting with gl::COLOR_ATTACHMENT0_EXT
    /*virtual void AttachTextures( int numTextures, 
                                GLenum texTarget[], 
                                GLuint texId[],
                                GLenum attachment[] = NULL,
                                int mipLevel[]      = NULL,
                                int zSlice[]        = NULL );*/

    /// Bind a render buffer to the "attachment" point of this FBO
    virtual void AttachRenderBuffer( GLuint buffId,
                                    GLenum attachment);

    /// Bind an array of render buffers to corresponding "attachment" points
    /// of this FBO.
    /// - By default, the first 'numBuffers' attachments are used,
    ///   starting with gl::COLOR_ATTACHMENT0_EXT
    virtual void AttachRenderBuffers( int numBuffers, GLuint buffId[],
                                    GLenum attachment[] = NULL );

    /// Free any resource bound to the "attachment" point of this FBO
    void Unattach( GLenum attachment );

    /// Free any resources bound to any attachment points of this FBO
    void UnattachAll();

    /// Is this FBO currently a valid render target?
    ///  - Sends output to std::cerr by default but can
    ///    be a user-defined C++ stream
    bool IsValid( std::ostream& ostr = std::cerr );

    /// BEGIN : Accessors
    /// Is attached type gl::RENDERBUFFER_EXT or gl::TEXTURE?
    GLenum GetAttachedType( GLenum attachment );

    /// What is the Id of Renderbuffer/texture currently 
    /// attached to "attachement?"
    GLuint GetAttachedId( GLenum attachment );

    /// Which mipmap level is currently attached to "attachement?"
    GLint  GetAttachedMipLevel( GLenum attachment );

    /// Which cube face is currently attached to "attachment?"
    GLint  GetAttachedCubeFace( GLenum attachment );

    /// Which z-slice is currently attached to "attachment?"
    GLint  GetAttachedZSlice( GLenum attachment );
    /// END : Accessors


    /// Return number of color attachments permitted
    static int GetMaxColorAttachments();

protected:
    void  _FramebufferTextureND( GLenum attachment, GLenum texTarget, 
                                GLuint texId, int mipLevel, int zSlice );

};

} // namespace gldr

namespace gldr {

    void
        Framebuffer::AttachTexture(GLenum texTarget, GLuint texId,
        GLenum attachment, int mipLevel, int zSlice)
    {
        bind();

        _FramebufferTextureND(attachment, texTarget,
            texId, mipLevel, zSlice);
    }

    void Framebuffer::AttachRenderBuffer(GLuint buffId, GLenum attachment)
    {
        auto scope = scopedBind();

        gl::FramebufferRenderbufferEXT(gl::FRAMEBUFFER, attachment,
            gl::RENDERBUFFER, buffId);
    }

    void Framebuffer::AttachRenderBuffers(int numBuffers, GLuint buffId [], GLenum attachment [])
    {
        for (int i = 0; i < numBuffers; ++i) {
            AttachRenderBuffer(buffId[i],
                attachment ? attachment[i] : (gl::COLOR_ATTACHMENT0 + i));
        }
    }

    void Framebuffer::Unattach(GLenum attachment)
    {
        auto scope = scopedBind();
        GLenum type = GetAttachedType(attachment);

        switch (type) {
        case gl::NONE:
            break;
        case gl::RENDERBUFFER:
            AttachRenderBuffer(0, attachment);
            break;
        case gl::TEXTURE:
            AttachTexture(gl::TEXTURE_2D, 0, attachment);
            break;
        default:
            throw std::runtime_error("Framebuffer::unbind_attachment ERROR: Unknown attached resource type");
        }
        
    }

    void Framebuffer::UnattachAll()
    {
        int numAttachments = GetMaxColorAttachments();
        for (int i = 0; i < numAttachments; ++i) {
            Unattach(gl::COLOR_ATTACHMENT0 + i);
        }
    }

    GLint Framebuffer::GetMaxColorAttachments()
    {
        GLint maxAttach = 0;
        gl::GetIntegerv(gl::MAX_COLOR_ATTACHMENTS, &maxAttach);
        return maxAttach;
    }

    void Framebuffer::_FramebufferTextureND(GLenum attachment, GLenum texTarget,
        GLuint texId, int mipLevel,
        int zSlice)
    {
        if (texTarget == gl::TEXTURE_1D) {
            gl::FramebufferTexture1D(gl::FRAMEBUFFER, attachment,
                gl::TEXTURE_1D, texId, mipLevel);
        }
        else if (texTarget == gl::TEXTURE_3D) {
            gl::FramebufferTexture3D(gl::FRAMEBUFFER, attachment,
                gl::TEXTURE_3D, texId, mipLevel, zSlice);
        }
        else {
            // Default is gl::TEXTURE_2D, gl::TEXTURE_RECTANGLE_ARB, or cube faces
            gl::FramebufferTexture2D(gl::FRAMEBUFFER, attachment,
                texTarget, texId, mipLevel);
        }
    }

    bool Framebuffer::IsValid(std::ostream& ostr)
    {
        auto scope = scopedBind();

        bool isOK = false;

        GLenum status;
        status = gl::CheckFramebufferStatusEXT(gl::FRAMEBUFFER);
        switch (status) {
        case gl::FRAMEBUFFER_COMPLETE: // Everything's OK
            isOK = true;
            break;
        case gl::FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                << "gl::FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n";
            isOK = false;
            break;
        case gl::FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                << "gl::FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n";
            isOK = false;
            break;
        case gl::FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                << "gl::FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n";
            isOK = false;
            break;
        case gl::FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                << "gl::FRAMEBUFFER_INCOMPLETE_FORMATS\n";
            isOK = false;
            break;
        case gl::FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                << "gl::FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n";
            isOK = false;
            break;
        case gl::FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                << "gl::FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n";
            isOK = false;
            break;
        case gl::FRAMEBUFFER_UNSUPPORTED:
            ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                << "gl::FRAMEBUFFER_UNSUPPORTED\n";
            isOK = false;
            break;
        default:
            ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                << "Unknown ERROR\n";
            isOK = false;
        }

        
        return isOK;
    }

    /// Accessors
    GLenum Framebuffer::GetAttachedType(GLenum attachment)
    {
        // Returns gl::RENDERBUFFER or gl::TEXTURE
        auto scope = scopedBind();
        GLint type = 0;
        gl::GetFramebufferAttachmentParameteriv(gl::FRAMEBUFFER, attachment,
            gl::FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
            &type);
        
        return GLenum(type);
    }

    GLuint Framebuffer::GetAttachedId(GLenum attachment)
    {
        auto scope = scopedBind();
        GLint id = 0;
        gl::GetFramebufferAttachmentParameteriv(gl::FRAMEBUFFER, attachment,
            gl::FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
            &id);
        
        return GLuint(id);
    }

    GLint Framebuffer::GetAttachedMipLevel(GLenum attachment)
    {
        auto s = scopedBind();
        GLint level = 0;
        gl::GetFramebufferAttachmentParameteriv(gl::FRAMEBUFFER, attachment,
            gl::FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL,
            &level);
        return level;
    }

    GLint Framebuffer::GetAttachedCubeFace(GLenum attachment)
    {
        auto scope = scopedBind();
        GLint level = 0;
        gl::GetFramebufferAttachmentParameteriv(gl::FRAMEBUFFER, attachment,
            gl::FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE,
            &level);
        
        return level;
    }
}