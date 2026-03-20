#include"EditorLayer.h"

#include"imgui.h"
#include<ImGuizmo.h>
#include"glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include"Platform/OpenGL/OpenGLShader.h"

#include"Resug/Util/FileUtil.h"


//TODO : 当删除最后一个渲染的实体时，它会任然保留的画面上。

//TODO : 当没有相机时，程序会崩溃。


#define PROFILE_SCOPE(name) Timer time##__LINE__("EditorLayer::OnUpdate", [&](ProfileResult profileResults) {m_ProfileResults.push_back(profileResults); })

EditorLayer::EditorLayer()
	:Layer("EditorLayer"), m_CameraController(1280.0f/720.0f)
{
}

void EditorLayer::OnAttach()
{
	
	RG_PROFILE_FUNCTION();
	m_Texture = Resug::Texture2D::Create("Assert/Image/im3_1.png");
	m_TextureInterface = Resug::SubTexture2D::CreateSubTex2D(Resug::Texture2D::Create("Assert/Image/PNG/interface/Tilemap/tilemap.png"), 
															glm::vec2(4.0f,2.0f),glm::vec2(1.0f,2.0f), glm::vec2(18.0f, 11.0f));
    Resug::FrameBufferSrecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_FrameBuffer = Resug::FrameBuffer::Create(fbSpec);

    m_ActiveScene = Resug::CreateRef<Resug::Scene>();


    Resug::Entity squareEntity = m_ActiveScene->CreateEntity("namename");

    squareEntity.AddComponent<Resug::SpriteRendererComponent>(glm::vec4{1.0f,1.0f,0.0f,1.0f});

    Resug::Entity squareEntity1 = m_ActiveScene->CreateEntity("namename1");

    squareEntity1.AddComponent<Resug::SpriteRendererComponent>(glm::vec4{ 1.0f,0.0f,0.0f,1.0f });



    auto& camearEntity = m_ActiveScene->CreateEntity("Camera");

    camearEntity.AddComponent<Resug::CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
    

    //TODO : 下面这个相机不能正常工作  疑似glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f)  没有之前处理。
    //m_SecondCameraEntity = m_ActiveScene->CreateEntity("SecondCamera");
    //m_SecondCameraEntity.AddComponent<Resug::CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));



    class CameraCotroller : public Resug::ScriptableEntity
    {
    public:
        void OnCreate()
        {
            std::cout << "Time\n";
        }
        void OnUpdate(Resug::Timestep ts)
        {
            auto& transform = m_Entity.GetComponent<Resug::TransformComponent>();

            //if (Resug::Input::IsKeyPressed(RG_KEY_D))transform.Transform[3][0] += ts;
            //if(Resug::Input::IsKeyPressed(RG_KEY_A)) transform.Transform[3][0] -= ts;
            //if(Resug::Input::IsKeyPressed(RG_KEY_W)) transform.Transform[3][1] += ts;
            //if(Resug::Input::IsKeyPressed(RG_KEY_S)) transform.Transform[3][1] -= ts;
        }
        void OnDestroy()
        {

        }
    };

    camearEntity.AddComponent<Resug::NativeScriptComponent>().Bind<CameraCotroller>();

    m_ActiveScene = Resug::CreateRef<Resug::Scene>();
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);


    Resug::Serializer serializer = Resug::Serializer(m_ActiveScene);
    serializer.Derialize("Assert/Scene/rigicandcoll.resug");

    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

    //Panel
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);



}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(Resug::Timestep& ts)
{
	RG_PROFILE_FUNCTION();

	{
		RG_PROFILE_SCOPE("Pre Pre");
        if (m_FrameTexSize != m_ViewportSize)
        {
            m_FrameTexSize = m_ViewportSize;
            //TODO: 去掉强制类型转换。
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditerCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        }
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

        if (m_EditerStatus == EditerStatus::Edit)
        {
            m_EditerCamera.OnUpdate();
            Resug::Renderer2D::BeginScene(m_EditerCamera.GetProjection(), m_EditerCamera.GetCameraTransform());
            
            m_ActiveScene->OnUpdateEditor(ts);
        }
        else if (m_EditerStatus == EditerStatus::Play)
        {
            m_ActiveScene->OnUpdateSimulation(ts);
            m_ActiveScene->OnUpdateRuntime(ts);
        }
        else if (m_EditerStatus == EditerStatus::Stop)
        {
               m_ActiveScene->OnUpdateRuntime(ts);

        }


        Resug::RendererCommand::Flush();
    
        m_FrameBuffer->UnBind();
    }
}

void EditorLayer::OnImGuiRender()
{
	RG_PROFILE_FUNCTION();



    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            //TODO 做成函数，加快捷键。
            if (ImGui::MenuItem("New"))
            {
                m_ActiveScene = Resug::CreateRef<Resug::Scene>();
                m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
                m_SceneHierarchyPanel.SetContext(m_ActiveScene);
            }

            if (ImGui::MenuItem("Open"))
            {
                std::string filepath = Resug::Files::OpenFile("Resug Scene (*.resug)\0*.resug\0");

                if (!filepath.empty())
                {
                    m_ActiveScene = Resug::CreateRef<Resug::Scene>();
                    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
               

                    Resug::Serializer serializer = Resug::Serializer(m_ActiveScene);
                    serializer.Derialize(filepath);

                    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
                    RG_CORE_INFO("Open new File");
                }
            }
            if (ImGui::MenuItem("Save"))
            {
                //std::string filepath = Resug::Files::SaveFile("Resug Scene (*.resug)\0*.resug\0");
                std::string filepath = Resug::Files::SaveFile("Resug Scene (*.resug)\0*.resug\0\0");
                if (!filepath.empty())
                {
                    if (filepath.find(".resug") == std::string::npos)
                    {
                        filepath += ".resug";
                    }
                    Resug::Serializer serializer = Resug::Serializer(m_ActiveScene);
                    serializer.Serialize(filepath);
                }
            }

            if (ImGui::MenuItem("Close")) Resug::Application::Get().Close();
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

    if (ImGui::Button("Play"))
        m_EditerStatus = EditerStatus::Play;
    if (ImGui::Button("Stop"))
        m_EditerStatus = EditerStatus::Stop;
    if (ImGui::Button("Edit"))
        m_EditerStatus = EditerStatus::Edit;


    ImGui::End();

    ImGui::Begin("Viewport");

    m_ViewportFoucses = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Resug::Application::Get().GetImGuiLayer()->BlackEvent(!m_ViewportFoucses||!m_ViewportHovered);

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    if (m_ViewportSize != *((glm::vec2*)&viewportSize))
        m_ViewportSize = {viewportSize.x, viewportSize.y};
    

    uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

    // Gizmos

    Resug::Entity selectedEntity = m_SceneHierarchyPanel.GetSeletedEntity();
    if (selectedEntity && m_GizomType != -1)
    {
      
        ImGuizmo::SetOrthographic(false);  
        ImGuizmo::SetDrawlist();

        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
        
        // Camera
        auto& cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
        auto& camera = cameraEntity.GetComponent<Resug::CameraComponent>().Camera;
        
        const glm::mat4& cameraProjection = camera.GetProjection();
        glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<Resug::TransformComponent>().GetTransform());
        
        // Entity transform
        auto& tc = selectedEntity.GetComponent<Resug::TransformComponent>();
        glm::mat4 transform = tc.GetTransform();


        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
            (ImGuizmo::OPERATION)m_GizomType, ImGuizmo::LOCAL, glm::value_ptr(transform));
        
        if (ImGuizmo::IsUsing())
        {
            //TODO: 优化体验
            glm::vec3 skew;
            glm::quat rotation;
            glm::vec3 scale, translation;
            glm::vec4 perspective;

            glm::decompose(transform, scale, rotation, translation, skew, perspective);
            
            tc.Position = translation;
            tc.Scale = scale;
            tc.Rotation = glm::degrees(glm::eulerAngles(rotation));

        }
        
    }


    ImGui::End();


    m_SceneHierarchyPanel.OnImGuiLayer();



    m_EntityPropertiesPanel.OnImGuiLayer(m_SceneHierarchyPanel.GetSeletedEntity());

    ImGui::End();
}

void EditorLayer::OnEvent(Resug::Event& event)
{
	m_CameraController.OnEvent(event);
    Resug::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Resug::KeyPressedEvent>(RG_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
}

bool EditorLayer::OnKeyPressed(Resug::KeyPressedEvent& event)
{
    if (event.GetRepeatCount() > 0)
    {
        return false;
    }
    std::cout << event.GetKeyCode() << " ";
    switch (event.GetKeyCode())
    {
    case RG_KEY_Q:
    {
        m_GizomType = 0;
        break;
    }
    case RG_KEY_W:
    {
        m_GizomType = 1;
        break;
    }
    case RG_KEY_E:
    {
        m_GizomType = 2;
        break;
    }
    case RG_KEY_R:
    {
        m_GizomType = -1;
        break;
    }
    }
}


