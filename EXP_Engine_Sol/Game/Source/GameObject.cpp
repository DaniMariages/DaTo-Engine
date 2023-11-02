#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentMesh.h"
#include <vector>

GameObject::GameObject(std::string name) : Name(name), active(true)
{
	//AddComponent(new ComponentTransform(this));
}

GameObject::~GameObject() {}

bool GameObject::Enable() //Start up + bool toggle
{
	if (!active) {
		active = true;
		return true;
		//StartUp here...
	}
	return false;
}

bool GameObject::Disable()
{
	if (active) {
		active = false;
		return true;
	}
	return false;
}

void GameObject::Update() 
{
	std::vector<Component*>::iterator item = components.begin();
	bool ret = true;
}

void GameObject::SetParent(GameObject* parent)
{
	this->parent = parent;
}

Component* GameObject::AddComponent(Component* component)
{
	Component* ret = nullptr;

	switch (component->type)
	{
	case(typeComponent::Error):
		//LOG("Component Type Error! Something broke...");
		break;
	/*case(typeComponent::Transform):
		ret = new ComponentTransform(this);
		break;*/
	case(typeComponent::Material):
		if (this->GetComponent(typeComponent::Material) != nullptr) {
			return nullptr;
		}
		ret = new ComponentTexture(this);
		break;
	case(typeComponent::Mesh):
		ret = new ComponentMesh(this);
		LOG("Component mesh added to %s", component->parent->Name.c_str());
		break;

	}

	components.push_back(component);
	return ret;
}

GameObject* GameObject::AddChildren(GameObject* children) {

	if (!this->children.empty()) this->children.push_back(children);
	return children;
}

Component* GameObject::GetComponent(typeComponent type)
{
	std::vector<Component*>::iterator item = components.begin();

	for (; item != components.end(); ++item) 
	{
		if ((*item)->type == type) 
		{
			return (*item);
		}
	}
	return nullptr;
}

std::vector<Component*> GameObject::GetComponents(typeComponent type)
{
	std::vector<Component*>::iterator item = components.begin();

	std::vector<Component*> ret;

	for (; item != components.end(); ++item) 
	{
		if ((*item)->type == type) {
			ret.push_back((*item));
		}
	}

	return ret;
}
