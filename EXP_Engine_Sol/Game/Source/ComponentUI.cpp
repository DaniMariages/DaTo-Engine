#include "Application.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentUI.h"

ComponentUI::ComponentUI(typeComponent comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* str, uint x, uint y) : Component(parent)
{
	this->canvas = canvas;
	this->ui_type = type;
	height = h;
	width = w;
	fill_color = { 1,1,1,1 };

	ComponentTransform* compTrans = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);
	if (x != 0 || y != 0)
	{
		pos_x = x;
		pos_y = y;
	}
	else
	{
		pos_x = compTrans->GetPosition().x;
		pos_y = compTrans->GetPosition().y;

		if (type == UI_Label)
		{
			compTrans->SetPosition(float3(0, compTrans->GetPosition().y, compTrans->GetPosition().z));
			compTrans->SetPosition(float3(compTrans->GetPosition().x, 0, compTrans->GetPosition().z));
		}
	}

	if (type != UI_Label)
	{
		panel_in_scene.vertex[0] = float3(compTrans->GetPosition().x, compTrans->GetPosition().y + height, compTrans->GetPosition().z);
		panel_in_scene.vertex[1] = float3(compTrans->GetPosition().x + width, compTrans->GetPosition().y + height, compTrans->GetPosition().z);
		panel_in_scene.vertex[3] = float3(compTrans->GetPosition().x + width, compTrans->GetPosition().y, compTrans->GetPosition().z);
		panel_in_scene.vertex[2] = float3(compTrans->GetPosition().x, compTrans->GetPosition().y, compTrans->GetPosition().z);

		panel_in_game.vertex[0] = float3(pos_x, pos_y + height, compTrans->GetPosition().z);
		panel_in_game.vertex[1] = float3(pos_x + width, pos_y + height, compTrans->GetPosition().z);
		panel_in_game.vertex[3] = float3(pos_x + width, pos_y, compTrans->GetPosition().z);
		panel_in_game.vertex[2] = float3(pos_x, pos_y, compTrans->GetPosition().z);

		float2* UV_coord = new float2[4];

		if (type == UI_Character)
		{
			panel_in_scene.uv[0] = float2(0, 1);
			panel_in_scene.uv[1] = float2(1, 1);
			panel_in_scene.uv[3] = float2(1, 0);
			panel_in_scene.uv[2] = float2(0, 0);

			panel_in_game.uv[0] = float2(0, 1);
			panel_in_game.uv[1] = float2(1, 1);
			panel_in_game.uv[3] = float2(1, 0);
			panel_in_game.uv[2] = float2(0, 0);
		}
		else
		{
			panel_in_game.uv[2] = float2(0, 1);
			panel_in_game.uv[3] = float2(1, 1);
			panel_in_game.uv[1] = float2(1, 0);
			panel_in_game.uv[0] = float2(0, 0);

			panel_in_scene.uv[2] = float2(0, 1);
			panel_in_scene.uv[3] = float2(1, 1);
			panel_in_scene.uv[1] = float2(1, 0);
			panel_in_scene.uv[0] = float2(0, 0);
		}

		panel_in_scene.GenerateBuffers();
		panel_in_game.GenerateBuffers();
	}

}
ComponentUI::~ComponentUI()
{

}

bool ComponentUI::OnClicked()
{
	if (dragable)
	{
		Move();
	}

	return true;
}

bool ComponentUI::OnRelease()
{
	if (!dragable)
		fill_color = float4(1, 1, 1, fill_color.w);
	return true;
}

void ComponentUI::Move()
{
	ComponentTransform* comp_trans = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);
	float2 move = float2(ExternalApp->input->GetMouseXMotion() * ExternalApp->DT() * 20, ExternalApp->input->GetMouseYMotion() * ExternalApp->DT() * 20);

	panel_in_scene.vertex[0] = float3(comp_trans->GetPosition().x + move.x, comp_trans->GetPosition().y + height + move.y, comp_trans->GetPosition().z);
	panel_in_scene.vertex[1] = float3(comp_trans->GetPosition().x + width + move.x, comp_trans->GetPosition().y + height + move.y, comp_trans->GetPosition().z);
	panel_in_scene.vertex[3] = float3(comp_trans->GetPosition().x + width + move.x, comp_trans->GetPosition().y + move.y, comp_trans->GetPosition().z);
	panel_in_scene.vertex[2] = float3(comp_trans->GetPosition().x + move.x, comp_trans->GetPosition().y + move.y, comp_trans->GetPosition().z);

	pos_x += move.x;
	pos_y += move.y;
	panel_in_game.vertex[0] = float3(pos_x, pos_y + height, comp_trans->GetPosition().z);
	panel_in_game.vertex[1] = float3(pos_x + width, pos_y + height, comp_trans->GetPosition().z);
	panel_in_game.vertex[3] = float3(pos_x + width, pos_y, comp_trans->GetPosition().z);
	panel_in_game.vertex[2] = float3(pos_x, pos_y, comp_trans->GetPosition().z);

	panel_in_scene.RegenerateVertexBuffers();
	panel_in_game.RegenerateVertexBuffers();
}

void ComponentUI::DebugDraw()
{
	ComponentTransform* comp_trans = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);

	glBegin(GL_LINE_LOOP);
	glColor4f(fill_color.x, fill_color.y, fill_color.z, fill_color.w);

	float3 pos = comp_trans->GetPosition();

	float3 v1 = float3(pos.x, pos.y, pos.z);
	float3 v2 = float3(pos.x + width, pos.y, pos.z);
	float3 v3 = float3(pos.x, pos.y + height, pos.z);
	float3 v4 = float3(pos.x + width, pos.y + height, pos.z);

	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glVertex3f(v4.x, v4.y, v4.z);
	glVertex3f(v3.x, v3.y, v3.z);

	glEnd();
}

void ComponentUI::Draw()
{
	//DebugDraw();
	UIPanel* ui_panel = nullptr;
	ComponentTransform* compTrans = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);

	/*if (ExternalApp->viewport->is_game_mode)
		ui_panel = &panel_in_game;
	else
		ui_panel = &panel_in_scene;*/

	/*if (App->viewport->is_game_mode && App->viewport->draw_ui)
	{*/
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(0.0, ExternalApp->editor->, ExternalApp->editor->panel_play->window_size.y, 0.0, 1.0, -1.0);


		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	//}

	/*if (!App->viewport->is_game_mode)
	{*/
		glPushMatrix();
		glMultMatrixf((float*)compTrans->GetGlobalTransform().Transposed().ptr());
	//}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glColor4f(fill_color.x, fill_color.y, fill_color.z, fill_color.w);

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, ui_panel->textureID);

	glBindBuffer(GL_ARRAY_BUFFER, ui_panel->buffer[0]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, ui_panel->buffer[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_panel->buffer[1]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);

	/*if (!App->viewport->is_game_mode)
		glPopMatrix();*/

}

void ComponentUI::UpdateTransform()
{
	ComponentTransform* comp_trans = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);

	pos_x = comp_trans->GetPosition().x;
	pos_y = comp_trans->GetPosition().y;
	comp_trans->GetGlobalTransform();

	panel_in_scene.vertex[0] = float3(comp_trans->GetPosition().x, comp_trans->GetPosition().y + height, comp_trans->GetPosition().z);
	panel_in_scene.vertex[1] = float3(comp_trans->GetPosition().x + width, comp_trans->GetPosition().y + height, comp_trans->GetPosition().z);
	panel_in_scene.vertex[3] = float3(comp_trans->GetPosition().x + width, comp_trans->GetPosition().y, comp_trans->GetPosition().z);
	panel_in_scene.vertex[2] = float3(comp_trans->GetPosition().x, comp_trans->GetPosition().y, comp_trans->GetPosition().z);

	panel_in_game.vertex[0] = float3(pos_x, pos_y + height, comp_trans->GetPosition().z);
	panel_in_game.vertex[1] = float3(pos_x + width, pos_y + height, comp_trans->GetPosition().z);
	panel_in_game.vertex[3] = float3(pos_x + width, pos_y, comp_trans->GetPosition().z);
	panel_in_game.vertex[2] = float3(pos_x, pos_y, comp_trans->GetPosition().z);

	panel_in_scene.RegenerateVertexBuffers();
	panel_in_game.RegenerateVertexBuffers();
}

void ComponentUI::Update()
{
	Draw();

	/*if (!App->viewport->is_over_game)
		return false;*/

	UILogic();

	switch (state)
	{
	case UI_Idle:
		break;
	case UI_Hover:
		OnHover();
		break;
	case UI_Click:
		OnClick();
		break;
	case UI_Clicked:
		OnClicked();
		break;
	case UI_Release:
		OnRelease();
		break;
	default:
		break;
	}

	UpdateUI(ExternalApp->DT());
}

void ComponentUI::UILogic()
{
	ImVec2 cursorPos = ImGui::GetMousePos();
	ImVec2 windowSize = ImGui::GetWindowSize();

	float2 origin = float2(cursorPos.x / windowSize.x, cursorPos.y / windowSize.y);
	origin.x = (origin.x - 0.5F) * 2;
	origin.y = -(origin.y - 0.5F) * 2;

	float2 mouse_pos = float2(cursorPos.x, cursorPos.y);

	switch (state)
	{
	case UI_Idle:
		if (CheckMouseInside(mouse_pos))
			state = UI_Hover;
		break;
	case UI_Hover:
		if (ExternalApp->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			state = UI_Click;
		if (!CheckMouseInside(mouse_pos))
			state = UI_Release;
		break;
	case UI_Click:
		if (ExternalApp->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			state = UI_Clicked;
		break;
	case UI_Clicked:
		if (ExternalApp->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && !CheckMouseInside(mouse_pos))
			state = UI_Release;
		else if (ExternalApp->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && CheckMouseInside(mouse_pos))
			state = UI_Hover;
		break;
	case UI_Release:
		state = UI_Idle;
		break;

	}
}

void ComponentUI::ChangeColor(float4 new_color)
{
	for (auto it_go = parent->children.begin(); it_go != parent->children.end(); it_go++)
	{
		if ((*it_go)->GetComponent(typeComponent::UI))
		{
			ComponentUI* compUI = (ComponentUI*)(*it_go)->GetComponent(typeComponent::UI);
			compUI->ChangeColor(new_color);
		}
	}

	fill_color = float4(new_color.x, new_color.y, new_color.z, fill_color.w);
}

bool ComponentUI::Fade()
{
	for (auto it_go = parent->children.begin(); it_go != parent->children.end(); it_go++)
	{
		if ((*it_go)->GetComponent(typeComponent::UI))
		{
			ComponentUI* compUI = (ComponentUI*)(*it_go)->GetComponent(typeComponent::UI);
			compUI->Fade();
		}
	}

	if (fill_color.w <= 0.05)
	{
		parent->active = false;
		return true;
	}
	else
	{
		fill_color.w -= 0.5 * ExternalApp->DT();
		return false;
	}

}

bool ComponentUI::CheckMouseInside(float2 mouse_pos)
{
	return (mouse_pos.x >= pos_x && mouse_pos.x <= pos_x + width && mouse_pos.y >= pos_y && mouse_pos.y <= pos_y + height);
}

void UIPanel::GenerateBuffers()
{
	uint* index = new uint[6];
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 2;
	index[4] = 1;
	index[5] = 3;

	//Cube Vertex
	glGenBuffers(1, &buffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, vertex, GL_STATIC_DRAW);

	//Cube Vertex definition
	glGenBuffers(1, &buffer[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, index, GL_STATIC_DRAW);

	//UVs definition
	glGenBuffers(1, &buffer[2]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * 4, uv, GL_STATIC_DRAW);

}

void UIPanel::RegenerateVertexBuffers()
{
	//Cube Vertex
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, vertex, GL_STATIC_DRAW);
}
