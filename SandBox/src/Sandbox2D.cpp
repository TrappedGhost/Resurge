#include"Sandbox2D.h"

#include"imgui.h"
#include"glm/gtc/type_ptr.hpp"

#include"Platform/OpenGL/OpenGLShader.h"


#define PROFILE_SCOPE(name) Timer time##__LINE__("Sandbox2D::OnUpdate", [&](ProfileResult profileResults) {m_ProfileResults.push_back(profileResults); })

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f/720.0f)
{
}

void Sandbox2D::OnAttach()
{
	
	RG_PROFILE_FUNCTION();
	m_Texture = Resug::Texture2D::Create("Assert/Image/im3_1.png");
	m_TextureInterface = Resug::SubTexture2D::CreateSubTex2D(Resug::Texture2D::Create("Assert/Image/PNG/interface/Tilemap/tilemap.png"), 
															glm::vec2(4.0f,2.0f),glm::vec2(1.0f,2.0f), glm::vec2(18.0f, 11.0f));
    Resug::FrameBufferSrecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_FrameBuffer = Resug::FrameBuffer::Create(fbSpec);

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Resug::Timestep& ts)
{
	RG_PROFILE_FUNCTION();

	{
		RG_PROFILE_SCOPE("Camera");
		m_CameraController.OnUpdate(ts);
	}
	{
		RG_PROFILE_SCOPE("Renderer Pre");
        m_FrameBuffer->Bind();
		Resug::Renderer2D::ResetStats();
		Resug::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Resug::RendererCommand::Clear();
	}
	{
		RG_PROFILE_SCOPE("Rendering");
		static float rotation = 0.0f;

		rotation += ts *50.0f;
		Resug::Renderer2D::BeginScene(m_CameraController.GetCamera());

       

		Resug::Renderer2D::DrawRotatedQuad(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), rotation, glm::vec4(1.0f,0.2f,0.23f,1.0f));
		Resug::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_SquareColor);
		Resug::Renderer2D::DrawQuad(glm::vec2(-1.0f, -1.0f), glm::vec2(4.0f, 4.0f), m_Texture, 10.0f, glm::vec4(0.3f,0.3f,0.3f,1.0f));
		Resug::Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 2.0f), m_TextureInterface, 1.0f, glm::vec4(0.3f,0.3f,0.3f,1.0f));
		Resug::Renderer2D::EndScene();
    
        m_FrameBuffer->UnBind();
    }
}

void Sandbox2D::OnImGuiRender()
{
	RG_PROFILE_FUNCTION();

	

    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, dockspaceOpen != NULL))
                dockspaceOpen = false;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Begin("Setting");

    auto stats = Resug::Renderer2D::GetStats();
    ImGui::Text("DrawCalls : %d", stats.DrawCalls);
    ImGui::Text("QuadCount : %d", stats.QuadCount);
    ImGui::Text("QuadVertexCount : %d", stats.GetTotalVertexCount());
    ImGui::Text("QuadIndexCount : %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit3("ColorSetting", glm::value_ptr(m_SquareColor));

    uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ 1280.0f,720.0f });

    ImGui::End();
    ImGui::End();
}

void Sandbox2D::OnEvent(Resug::Event& event)
{
	m_CameraController.OnEvent(event);
}
